/* power-monitor.c
 *
 * Copyright (C) 2023-2025 HandBrake Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "model/power-monitor.h"

#include "application.h"
#include "callbacks.h"
#include "notifications.h"
#include "queuehandler.h"

#define UPOWER_PATH "org.freedesktop.UPower"
#define UPOWER_OBJECT "/org/freedesktop/UPower"
#define DEVICE_OBJECT "/org/freedesktop/UPower/devices/DisplayDevice"
#define UPOWER_INTERFACE "org.freedesktop.UPower"
#define DEVICE_INTERFACE "org.freedesktop.UPower.Device"

struct _GhbPowerMonitor
{
    GObject parent_instance;

    GhbPrefs *prefs;
    GDBusProxy *upower_proxy;
    GDBusProxy *battery_proxy;
#if GLIB_CHECK_VERSION(2, 70, 0)
    GPowerProfileMonitor *profile_monitor;
#endif

    /* We want to ensure that the encode is only paused when the battery
     * level first drops from normal to low, so the user can resume encoding
     * without it being paused again. So this variable tracks the previous
     * battery level, and if it was low already, we don't do anything. */
    int prev_battery_level;
    GhbPowerState power_state;
    gboolean has_battery;
};

G_DEFINE_TYPE (GhbPowerMonitor, ghb_power_monitor, G_TYPE_OBJECT)

static void ghb_power_monitor_dispose(GObject *object);
static void ghb_power_monitor_finalize(GObject *object);

static void
ghb_power_monitor_class_init (GhbPowerMonitorClass *class_)
{
    GObjectClass *object_class = G_OBJECT_CLASS(class_);

    object_class->finalize = ghb_power_monitor_finalize;
    object_class->dispose = ghb_power_monitor_dispose;
}

static void
battery_level_cb (GDBusProxy *proxy, GVariant *changed_properties,
                  GStrv invalidated_properties, GhbPowerMonitor *self)
{
    int battery_level = -1;
    const char *prop_name;
    int queue_state;
    GVariant *var;
    GVariantIter iter;
    int low_battery_level = 0;

    if (!ghb_prefs_get_boolean(self->prefs, "pause-encoding-on-low-battery"))
        return;

    low_battery_level = ghb_prefs_get_int(self->prefs, "low-battery-level");

    g_variant_iter_init(&iter, changed_properties);
    while (g_variant_iter_next(&iter, "{&sv}", &prop_name, &var))
    {
        if (g_strcmp0("Percentage", prop_name) == 0)
            battery_level = (int) g_variant_get_double(var);

        g_variant_unref(var);
    }
    if (battery_level < 0)
        return;

    queue_state = ghb_get_queue_state();

    if (battery_level <= low_battery_level
        && self->prev_battery_level > low_battery_level
        && (queue_state & GHB_STATE_WORKING)
        && !(queue_state & GHB_STATE_PAUSED))
    {
        self->power_state = GHB_POWER_PAUSED_LOW_BATTERY;
        ghb_log("Battery level %d%%: pausing encode", battery_level);
        ghb_send_notification(GHB_NOTIFY_PAUSED_LOW_BATTERY, 0, ghb_ud());
        ghb_pause_queue();
    }
    else if (battery_level > low_battery_level
             && self->prev_battery_level <= low_battery_level
             && (self->power_state == GHB_POWER_PAUSED_LOW_BATTERY))
    {
        if (queue_state & GHB_STATE_PAUSED)
        {
            ghb_resume_queue();
            ghb_log("Battery level %d%%: resuming encode", battery_level);
            ghb_withdraw_notification(GHB_NOTIFY_PAUSED_LOW_BATTERY);
        }
        self->power_state = GHB_POWER_OK;
    }
    self->prev_battery_level = battery_level;
}

static void
battery_proxy_new_cb (GObject *source, GAsyncResult *result,
                      GhbPowerMonitor *self)
{
    GDBusProxy *proxy;
    GVariant *is_present;
    GError *error = NULL;

    proxy = g_dbus_proxy_new_for_bus_finish(result, &error);
    if (proxy != NULL)
    {
        is_present = g_dbus_proxy_get_cached_property(proxy, "IsPresent");
        if (g_variant_get_boolean(is_present))
        {
            g_signal_connect(proxy, "g-properties-changed",
                         G_CALLBACK(battery_level_cb), self);
            self->has_battery = TRUE;
            self->battery_proxy = proxy;
        }
        else
        {
            g_debug("No battery present. Disconnecting UPower proxy.");
            g_clear_object(&proxy);
            g_clear_object(&self->upower_proxy);
        }
        g_variant_unref(is_present);
    }
    else
    {
        g_debug("Could not get DisplayDevice proxy: %s", error->message);
        g_error_free(error);
        g_clear_object(&self->upower_proxy);
    }
}

static void
battery_proxy_new_async (GhbPowerMonitor *self)
{
    g_dbus_proxy_new_for_bus(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, NULL,
                             UPOWER_PATH, DEVICE_OBJECT, DEVICE_INTERFACE, NULL,
                             (GAsyncReadyCallback) battery_proxy_new_cb, self);
}

static void
upower_status_cb (GDBusProxy *proxy, GVariant *changed_properties,
                  GStrv invalidated_properties, GhbPowerMonitor *self)
{
    gboolean on_battery;
    int queue_state;

    if (!ghb_prefs_get_boolean(self->prefs, "pause-encoding-on-battery-power") ||
        !g_variant_lookup(changed_properties, "OnBattery", "b", &on_battery))
        return;

    queue_state = ghb_get_queue_state();

    if (on_battery && (queue_state & GHB_STATE_WORKING)
                   && !(queue_state & GHB_STATE_PAUSED))
    {
        self->power_state = GHB_POWER_PAUSED_ON_BATTERY;
        ghb_log("Charger disconnected: pausing encode");
        ghb_send_notification(GHB_NOTIFY_PAUSED_ON_BATTERY, 0, ghb_ud());
        ghb_pause_queue();
    }
    else if (!on_battery && (self->power_state == GHB_POWER_PAUSED_ON_BATTERY))
    {
        if (queue_state & GHB_STATE_PAUSED)
        {
            ghb_resume_queue();
            ghb_log("Charger connected: resuming encode");
            ghb_withdraw_notification(GHB_NOTIFY_PAUSED_ON_BATTERY);
        }
        self->power_state = GHB_POWER_OK;
    }
}

static void
upower_proxy_new_cb (GObject *source, GAsyncResult *result,
                     GhbPowerMonitor *self)
{
    GDBusProxy *proxy;
    GError *error = NULL;

    proxy = g_dbus_proxy_new_for_bus_finish(result, &error);
    if (proxy != NULL)
    {
        g_signal_connect(proxy, "g-properties-changed",
                         G_CALLBACK(upower_status_cb), self);
        self->upower_proxy = proxy;
        battery_proxy_new_async(self);
    }
    else
    {
        g_debug("Could not create UPower proxy: %s", error->message);
        g_error_free(error);
    }
}

static void
upower_proxy_new_async (GhbPowerMonitor *self)
{
    g_dbus_proxy_new_for_bus(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, NULL,
                             UPOWER_PATH, UPOWER_OBJECT, UPOWER_INTERFACE, NULL,
                             (GAsyncReadyCallback) upower_proxy_new_cb, self);
}

#if GLIB_CHECK_VERSION(2, 70, 0)
static void
power_save_cb (GPowerProfileMonitor *monitor, GParamSpec *pspec,
               GhbPowerMonitor *self)
{
    if (!ghb_prefs_get_boolean(self->prefs, "pause-encoding-on-power-save"))
        return;

    int queue_state = ghb_get_queue_state();
    gboolean power_save = g_power_profile_monitor_get_power_saver_enabled(monitor);

    if (power_save && (queue_state & GHB_STATE_WORKING)
                   && !(queue_state & GHB_STATE_PAUSED))
    {
        self->power_state = GHB_POWER_PAUSED_POWER_SAVE;
        ghb_log("Power saver enabled: pausing encode");
        ghb_pause_queue();
        ghb_send_notification(GHB_NOTIFY_PAUSED_POWER_SAVE, 0, ghb_ud());
    }
    else if (!power_save && (self->power_state == GHB_POWER_PAUSED_POWER_SAVE))
    {
        if (queue_state & GHB_STATE_PAUSED)
        {
            ghb_resume_queue();
            ghb_log("Power saver disabled: resuming encode");
            ghb_withdraw_notification(GHB_NOTIFY_PAUSED_POWER_SAVE);
        }
        self->power_state = GHB_POWER_OK;
    }
}

static GPowerProfileMonitor *
power_profile_monitor_new (GhbPowerMonitor *self)
{
    GPowerProfileMonitor *monitor = g_power_profile_monitor_dup_default();

    if (monitor != NULL)
    {
        g_signal_connect(monitor, "notify::power-saver-enabled",
                         G_CALLBACK(power_save_cb), self);
    }
    else
    {
        g_debug("Could not get power profile monitor");
    }
    return monitor;
}
#endif

/* Initializes the D-Bus connections to monitor power state. */
static void
ghb_power_monitor_init (GhbPowerMonitor *self)
{
    g_debug("Initializing power monitor");
    upower_proxy_new_async(self);
#if GLIB_CHECK_VERSION(2, 70, 0)
    self->profile_monitor = power_profile_monitor_new(self);
#endif
}

GhbPowerMonitor *
ghb_power_monitor_new (GhbPrefs *prefs)
{
    g_return_val_if_fail(GHB_IS_PREFS(prefs), NULL);

    GhbPowerMonitor *monitor = g_object_new(GHB_TYPE_POWER_MONITOR, NULL);
    monitor->prefs = g_object_ref(prefs);
    return monitor;
}

/* Resets the status when the start/pause button is clicked, in order to
 * avoid phantom resumes. */
void
ghb_power_monitor_reset (GhbPowerMonitor *self)
{
    g_return_if_fail(GHB_IS_POWER_MONITOR(self));
    self->power_state = GHB_POWER_OK;
}

/* Disposes of the signals and other objects before shutdown. */
static void
ghb_power_monitor_dispose (GObject *object)
{
    GhbPowerMonitor *self = GHB_POWER_MONITOR(object);
    g_return_if_fail(GHB_IS_POWER_MONITOR(self));

    if (self->upower_proxy != NULL)
    {
        g_debug("Disconnecting UPower proxy");
        g_signal_handlers_disconnect_by_func(self->upower_proxy,
                                             upower_status_cb, self);
        g_clear_object(&self->upower_proxy);
    }
    if (self->battery_proxy != NULL)
    {
        g_debug("Disconnecting battery level proxy");
        g_signal_handlers_disconnect_by_func(self->battery_proxy,
                                             battery_level_cb, self);
        g_clear_object(&self->battery_proxy);
    }
#if GLIB_CHECK_VERSION(2, 70, 0)
    if (self->profile_monitor != NULL)
    {
        g_debug("Disconnecting power monitor\n");
        g_signal_handlers_disconnect_by_func(self->profile_monitor,
                                             power_save_cb, self);
        g_clear_object(&self->profile_monitor);
    }
#endif
    g_clear_object(&self->prefs);
    G_OBJECT_CLASS(ghb_power_monitor_parent_class)->dispose(object);
}

static void
ghb_power_monitor_finalize (GObject *object)
{
    g_debug("Finalizing power monitor");
    G_OBJECT_CLASS(ghb_power_monitor_parent_class)->finalize(object);
}

gboolean
ghb_power_monitor_has_battery (GhbPowerMonitor *self)
{
    g_return_val_if_fail(GHB_IS_POWER_MONITOR(self), FALSE);
    return self->has_battery;
}
