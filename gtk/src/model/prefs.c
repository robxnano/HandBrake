/* Copyright (C) 2025 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "model/prefs.h"

#include "application.h"
#include "presets.h"
#include "resources.h"

#define G_SETTINGS_ENABLE_BACKEND 1
#include "gio/gsettingsbackend.h"

#define PREFS_SCHEMA_PATH "/fr/handbrake/ghb/"
#define PREFS_SCHEMA_ID   "fr.handbrake.ghb.Preferences"
#define PREFS_FILENAME    "preferences.ini"

struct _GhbPrefs
{
    GObject parent_instance;
    GSettings *gsettings;

    GhbValue *dict;
};

G_DEFINE_TYPE (GhbPrefs, ghb_prefs, G_TYPE_OBJECT)

static void ghb_prefs_dispose(GObject *object);
static void ghb_prefs_finalize(GObject *object);

static void
ghb_prefs_class_init (GhbPrefsClass *class_)
{
    GObjectClass *object_class = G_OBJECT_CLASS(class_);

    object_class->finalize = ghb_prefs_finalize;
    object_class->dispose = ghb_prefs_dispose;
}

static void
ghb_prefs_init (GhbPrefs *self)
{
    g_debug("Initializing prefs manager");
}

static void
ghb_prefs_dispose (GObject *object)
{
    GhbPrefs *self = GHB_PREFS(object);
    g_return_if_fail(GHB_IS_PREFS(self));

    g_debug("Disposing of prefs manager");
    g_settings_sync();
    G_OBJECT_CLASS(ghb_prefs_parent_class)->dispose(object);
}

static void
ghb_prefs_finalize (GObject *object)
{
    GhbPrefs *self = GHB_PREFS(object);
    g_return_if_fail(GHB_IS_PREFS(self));

    g_debug("Finalizing prefs manager");
    g_object_unref(self->gsettings);
    G_OBJECT_CLASS(ghb_prefs_parent_class)->finalize(object);
}

GhbPrefs *
ghb_prefs_new (void)
{
    GhbPrefs *prefs = g_object_new(GHB_TYPE_PREFS, NULL);
    const char *app_dir = ghb_application_get_app_dir(GHB_APPLICATION_DEFAULT);
    g_autoptr(GSettingsSchema) schema = NULL;
    g_autoptr(GSettingsSchemaSource) source = NULL;
    GSettingsSchemaSource *def_src = g_settings_schema_source_get_default();

    // If the application is not installed, load the settings schema from the build dir
    // to allow testing without installing and avoid clashes with outdated versions
    if (!g_strrstr(app_dir, "/bin"))
    {
        g_autoptr(GError) error = NULL;
        g_autofree char *path = g_strconcat(app_dir, "/../data", NULL);
        source = g_settings_schema_source_new_from_directory(path, def_src, FALSE, &error);
        if (error)
            g_warning("Could not load schema: %s", error->message);
    }

    if (source)
        schema = g_settings_schema_source_lookup(source, PREFS_SCHEMA_ID, TRUE);
    else
        schema = g_settings_schema_source_lookup(def_src, PREFS_SCHEMA_ID, TRUE);

    if (schema == NULL)
        g_error("Settings schema '" PREFS_SCHEMA_ID "' is not installed");

    g_autofree char *config_dir = ghb_get_user_config_dir(NULL);
    g_autofree char *config_file = g_strconcat(config_dir, "/", PREFS_FILENAME, NULL);
    GSettingsBackend *backend = g_keyfile_settings_backend_new(config_file, "/fr/handbrake/ghb/", NULL);
    prefs->gsettings = g_settings_new_full(schema, backend, NULL);
    return prefs;
}

void
ghb_prefs_set_string (GhbPrefs *self, const char *key, const char *val)
{
    g_return_if_fail(GHB_IS_PREFS(self));
    if (!g_settings_set_string(self->gsettings, key, val))
        g_debug("Failed to set preference %s", key);
}

void
ghb_prefs_set_double (GhbPrefs *self, const char *key, double val)
{
    g_return_if_fail(GHB_IS_PREFS(self));
    if (!g_settings_set_double(self->gsettings, key, val))
        g_debug("Failed to set preference %s", key);
}

void
ghb_prefs_set_int (GhbPrefs *self, const char *key, int val)
{
    g_return_if_fail(GHB_IS_PREFS(self));
    if (!g_settings_set_int(self->gsettings, key, val))
        g_debug("Failed to set preference %s", key);
}

void
ghb_prefs_set_enum (GhbPrefs *self, const char *key, int val)
{
    g_return_if_fail(GHB_IS_PREFS(self));
    if (!g_settings_set_enum(self->gsettings, key, val))
        g_debug("Failed to set preference %s", key);
}

void
ghb_prefs_set_boolean (GhbPrefs *self, const char *key, gboolean val)
{
    g_return_if_fail(GHB_IS_PREFS(self));
    if (!g_settings_set_boolean(self->gsettings, key, val))
        g_debug("Failed to set preference %s", key);
}

gboolean
ghb_prefs_get_boolean (GhbPrefs *self, const char *key)
{
    g_return_val_if_fail(GHB_IS_PREFS(self), FALSE);
    return g_settings_get_boolean(self->gsettings, key);
}

int
ghb_prefs_get_int (GhbPrefs *self, const char *key)
{
    g_return_val_if_fail(GHB_IS_PREFS(self), 0);
    return g_settings_get_int(self->gsettings, key);
}

int
ghb_prefs_get_enum (GhbPrefs *self, const char *key)
{
    g_return_val_if_fail(GHB_IS_PREFS(self), 0);
    return g_settings_get_enum(self->gsettings, key);
}

double
ghb_prefs_get_double (GhbPrefs *self, const char *key)
{
    g_return_val_if_fail(GHB_IS_PREFS(self), 0);
    return g_settings_get_double(self->gsettings, key);
}

char **
ghb_prefs_get_strv (GhbPrefs *self, const char *key)
{
    g_return_val_if_fail(GHB_IS_PREFS(self), NULL);
    return g_settings_get_strv(self->gsettings, key);
}

char *
ghb_prefs_get_string (GhbPrefs *self, const char *key)
{
    g_return_val_if_fail(GHB_IS_PREFS(self), NULL);
    return g_settings_get_string(self->gsettings, key);
}

char *
ghb_prefs_get_string_or (GhbPrefs *self, const char *key, const char *def)
{
    char *str = ghb_prefs_get_string(self, key);
    return str && str[0] ? str : g_strdup(def);
}

GSettings *
ghb_prefs_get_gsettings (GhbPrefs *self)
{
    g_return_val_if_fail(GHB_IS_PREFS(self), NULL);
    return self->gsettings;
}
