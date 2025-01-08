/* Copyright (C) 2025 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "common.h"

#include "model/prefs.h"

G_BEGIN_DECLS

#define GHB_TYPE_PREFS_DIALOG (ghb_prefs_dialog_get_type())

G_DECLARE_FINAL_TYPE(GhbPrefsDialog, ghb_prefs_dialog, GHB, PREFS_DIALOG, GtkWindow)

GtkWidget *ghb_prefs_dialog_new(GhbPrefs *prefs);

G_END_DECLS
