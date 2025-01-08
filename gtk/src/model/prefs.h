/* Copyright (C) 2025 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "common.h"

#include "values.h"

G_BEGIN_DECLS

#define GHB_TYPE_PREFS (ghb_prefs_get_type())

G_DECLARE_FINAL_TYPE (GhbPrefs, ghb_prefs, GHB, PREFS, GObject)

GhbPrefs *ghb_prefs_new(void);
void ghb_prefs_save_to_file(void);
void ghb_prefs_restore_defaults(void);
GSettings *ghb_prefs_get_gsettings(GhbPrefs *self);
void ghb_prefs_set_string(GhbPrefs *self, const char *key, const char *val);
void ghb_prefs_set_double(GhbPrefs *self, const char *key, double val);
void ghb_prefs_set_int(GhbPrefs *self, const char *key, int val);
void ghb_prefs_set_enum(GhbPrefs *self, const char *key, int val);
void ghb_prefs_set_boolean(GhbPrefs *self, const char *key, gboolean val);

gboolean ghb_prefs_get_boolean(GhbPrefs *self, const char *key);
int ghb_prefs_get_int(GhbPrefs *self, const char *key);
int ghb_prefs_get_enum(GhbPrefs *self, const char *key);
double ghb_prefs_get_double(GhbPrefs *self, const char *key);
char *ghb_prefs_get_string(GhbPrefs *self, const char *key);
char *ghb_prefs_get_string_or(GhbPrefs *self, const char *key, const char *def);
char **ghb_prefs_get_strv(GhbPrefs *self, const char *key);

G_END_DECLS
