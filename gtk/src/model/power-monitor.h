/* Copyright (C) 2023-2025 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "common.h"
#include "model/prefs.h"
#include "settings.h"

G_BEGIN_DECLS

#define GHB_TYPE_POWER_MONITOR (ghb_power_monitor_get_type())

G_DECLARE_FINAL_TYPE(GhbPowerMonitor, ghb_power_monitor, GHB, POWER_MONITOR, GObject)

GhbPowerMonitor *ghb_power_monitor_new (GhbPrefs *prefs);
void ghb_power_monitor_reset (GhbPowerMonitor *self);
gboolean ghb_power_monitor_has_battery (GhbPowerMonitor *self);

G_END_DECLS
