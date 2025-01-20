/* Copyright (C) 2025 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define G_LOG_USE_STRUCTURED
#ifndef G_LOG_DOMAIN
#define G_LOG_DOMAIN "ghb"
#endif

#include "config.h"

#include <gio/gio.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ghb_log_func() g_debug("Function: %s", __func__)
#define ghb_log_func_str(x) g_debug("Function: %s (%s)", __func__, (x))

#pragma once

/*
 * Enum definitions for use with GObject properties and GSettings
 * Ensure these enum definitions match the ones in
 * data/fr.handbrake.ghb.gschema.xml
 */

typedef enum {
    GHB_ACTION_NORMAL,
    GHB_ACTION_SUGGESTED,
    GHB_ACTION_DESTRUCTIVE,
} GhbActionStyle;

typedef enum {
    GHB_ACTION_NOTHING,
    GHB_ACTION_QUIT,
    GHB_ACTION_SLEEP,
    GHB_ACTION_SHUTDOWN,
} GhbWhenCompleteAction;

typedef enum {
    GHB_LONGEVITY_IMMORTAL = 0,
    GHB_LONGEVITY_WEEK = 7,
    GHB_LONGEVITY_MONTH = 30,
    GHB_LONGEVITY_YEAR = 365,
} GhbLogLongevity;

typedef enum {
    GHB_QUEUE_STATUS_READY,
    GHB_QUEUE_STATUS_RUNNING,
    GHB_QUEUE_STATUS_PAUSED,
    GHB_QUEUE_STATUS_FINISHED,
    GHB_QUEUE_STATUS_FAILED,
} GhbQueueStatus;

typedef enum /*< flags >*/ {
    GHB_POWER_OK = 0,
    GHB_POWER_PAUSED_POWER_SAVE = 1,
    GHB_POWER_PAUSED_ON_BATTERY = 2,
    GHB_POWER_PAUSED_LOW_BATTERY = 4,
} GhbPowerState;

G_END_DECLS
