/* Copyright (C) 2025 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "common.h"

#include "application.h"

G_BEGIN_DECLS

#define GHB_TYPE_ACTIVITY_WINDOW (ghb_activity_window_get_type())

G_DECLARE_FINAL_TYPE(GhbActivityWindow, ghb_activity_window, GHB, ACTIVITY_WINDOW, GtkWindow)

GtkWidget *ghb_activity_window_new(GhbApplication *app, GtkTextBuffer *buffer);

G_END_DECLS
