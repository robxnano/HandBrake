/* Copyright (C) 2025 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "ui/prefs-dialog.h"

#include "application.h"
#include "callbacks.h"
#include "power-manager.h"
#include "ui/file-button.h"
#include "ui/string-list.h"

struct _GhbPrefsDialog
{
    GtkWindow parent_instance;

    GhbPrefs        *prefs;
    gboolean         requires_restart;

    // template child widgets
    GtkSpinButton   *activity_font_size;
    GtkCheckButton  *auto_name;
    GtkEntry        *auto_name_template;
    GtkCheckButton  *auto_scan;
    GhbFileButton   *custom_tmp_dir;
    GtkCheckButton  *custom_tmp_enable;
    GtkCheckButton  *disk_free_check;
    GtkSpinButton   *disk_free_limit;
    GtkCheckButton  *encode_log_location;
    GhbStringList   *excluded_file_extensions;
    GtkCheckButton  *hbfd_feature;
    GtkCheckButton  *keep_duplicate_titles;
    GtkDropDown     *log_longevity;
    GtkDropDown     *logging_level;
    GtkCheckButton  *limit_max_duration;
    GtkSpinButton   *max_title_duration;
    GtkSpinButton   *min_title_duration;
    GtkCheckButton  *notify_on_encode_done;
    GtkCheckButton  *notify_on_queue_done;
    GtkCheckButton  *pause_encoding_on_battery_power;
    GtkCheckButton  *pause_encoding_on_low_battery;
    GtkCheckButton  *pause_encoding_on_power_save;
    GtkSpinButton   *preview_count;
    GtkCheckButton  *reduce_hd_preview;
    GtkCheckButton  *remove_finished_jobs;
    GtkCheckButton  *send_file_to;
    GtkEntry        *send_file_to_target;
    GtkCheckButton  *show_mini_preview;
    GtkCheckButton  *sync_title_settings;
    GtkDropDown     *ui_language;
    GtkCheckButton  *use_dvdnav;
    GtkCheckButton  *use_m4v;
    GtkDropDown     *video_quality_granularity;
    GtkDropDown     *when_complete;
};

G_DEFINE_TYPE (GhbPrefsDialog, ghb_prefs_dialog, GTK_TYPE_WINDOW)

static void ghb_prefs_dialog_dispose(GObject *object);
static void ghb_prefs_dialog_finalize(GObject *object);

#define BIND_TEMPLATE_CHILD(n) gtk_widget_class_bind_template_child(widget_class, GhbPrefsDialog, n)

static void
ghb_prefs_dialog_class_init (GhbPrefsDialogClass *class_)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class_);
    GObjectClass *object_class = G_OBJECT_CLASS(class_);

    gtk_widget_class_set_template_from_resource(widget_class, "/fr/handbrake/ghb/ui/prefs-dialog.ui");
    BIND_TEMPLATE_CHILD(activity_font_size);
    BIND_TEMPLATE_CHILD(auto_name);
    BIND_TEMPLATE_CHILD(auto_name_template);
    BIND_TEMPLATE_CHILD(auto_scan);
    BIND_TEMPLATE_CHILD(custom_tmp_dir);
    BIND_TEMPLATE_CHILD(custom_tmp_enable);
    BIND_TEMPLATE_CHILD(disk_free_check);
    BIND_TEMPLATE_CHILD(disk_free_limit);
    BIND_TEMPLATE_CHILD(encode_log_location);
    BIND_TEMPLATE_CHILD(excluded_file_extensions);
    BIND_TEMPLATE_CHILD(hbfd_feature);
    BIND_TEMPLATE_CHILD(keep_duplicate_titles);
    BIND_TEMPLATE_CHILD(log_longevity);
    BIND_TEMPLATE_CHILD(logging_level);
    BIND_TEMPLATE_CHILD(limit_max_duration);
    BIND_TEMPLATE_CHILD(max_title_duration);
    BIND_TEMPLATE_CHILD(min_title_duration);
    BIND_TEMPLATE_CHILD(notify_on_encode_done);
    BIND_TEMPLATE_CHILD(notify_on_queue_done);
    BIND_TEMPLATE_CHILD(pause_encoding_on_battery_power);
    BIND_TEMPLATE_CHILD(pause_encoding_on_low_battery);
    BIND_TEMPLATE_CHILD(pause_encoding_on_power_save);
    BIND_TEMPLATE_CHILD(preview_count);
    BIND_TEMPLATE_CHILD(reduce_hd_preview);
    BIND_TEMPLATE_CHILD(remove_finished_jobs);
    BIND_TEMPLATE_CHILD(send_file_to);
    BIND_TEMPLATE_CHILD(send_file_to_target);
    BIND_TEMPLATE_CHILD(show_mini_preview);
    BIND_TEMPLATE_CHILD(sync_title_settings);
    BIND_TEMPLATE_CHILD(use_dvdnav);
    BIND_TEMPLATE_CHILD(ui_language);
    BIND_TEMPLATE_CHILD(use_m4v);
    BIND_TEMPLATE_CHILD(video_quality_granularity);
    BIND_TEMPLATE_CHILD(when_complete);

    object_class->finalize = ghb_prefs_dialog_finalize;
    object_class->dispose = ghb_prefs_dialog_dispose;
}

void
restart_required_cb (GSettings *settings, char *key, GhbPrefsDialog *self)
{
    g_return_if_fail(GHB_IS_PREFS_DIALOG(self));
    self->requires_restart = TRUE;
}

static void
ghb_prefs_dialog_init (GhbPrefsDialog *self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
}

static void
ghb_prefs_dialog_dispose (GObject *object)
{
    G_OBJECT_CLASS(ghb_prefs_dialog_parent_class)->dispose(object);

#if GTK_CHECK_VERSION(4, 8, 0)
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    gtk_widget_dispose_template(GTK_WIDGET(object), GHB_TYPE_PREFS_DIALOG);
G_GNUC_END_IGNORE_DEPRECATIONS
#endif
}

static void
ghb_prefs_dialog_finalize (GObject *object)
{
    GhbPrefsDialog *self = GHB_PREFS_DIALOG(object);
    g_signal_handlers_disconnect_by_func(ghb_prefs_get_gsettings(self->prefs), restart_required_cb, self);
    g_clear_object(&self->prefs);
    G_OBJECT_CLASS(ghb_prefs_dialog_parent_class)->finalize(object);
}

G_MODULE_EXPORT void
preferences_action_cb (GSimpleAction *action, GVariant *param, gpointer data)
{
    GhbApplication *app = GHB_APPLICATION(g_application_get_default());
    GhbPrefs *prefs = ghb_application_get_prefs(app);
    GtkWidget *dialog = ghb_prefs_dialog_new(prefs);
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(ghb_builder_widget("hb_window")));
    gtk_window_present(GTK_WINDOW(dialog));
}

typedef struct {
    const char *string;
    const char *id;
    double val;
} GhbStringMap;

static GhbStringMap logging_level_map[] = {
    {"0", "0", 0},
    {"1", "1", 1},
    {"2", "2", 2},
    {"3", "3", 3},
    {NULL},
};

static GhbStringMap log_longevity_map[] = {
    {N_("Week"),     "week",     GHB_LONGEVITY_WEEK    },
    {N_("Month"),    "month",    GHB_LONGEVITY_MONTH   },
    {N_("Year"),     "year",     GHB_LONGEVITY_YEAR    },
    {N_("Immortal"), "immortal", GHB_LONGEVITY_IMMORTAL},
    {NULL},
};

// The list of display languages selectable in the preferences.
// Please keep this list up to date with the actually translated
// languages in po/LINGUAS. The country codes are glibc locales.
static GhbStringMap ui_language_map[] =
{
    {N_("Use System Language"),         "",       0},
//  {"Afrikaans (Afrikaans)",           "af_ZA",  1},
//  {"Basque (Euskara)",                "eu_ES",  2},
    {"Български (Bulgarian)",           "bg_BG",  3},
    {"Català (Catalan)",                "ca_ES",  4},
    {"简体中文 (Simplified Chinese)",       "zh_CN",  5},
//  {"正體中文 (Traditional Chinese)",      "zh_TW",  6},
    {"Corsu (Corsican)",                "co_CO",  7},
//  {"Hrvatski (Croatian)",             "hr_HR",  8},
//  {"čeština (Czech)",                 "cs_CZ",  9},
//  {"Dansk (Danish)",                  "da_DK", 10},
    {"Nederlands (Dutch)",              "nl_NL", 11},
    {"English",                         "en_US", 12},
    {"Suomi (Finnish)",                 "fi_FI", 13},
    {"Français (French)",               "fr_FR", 14},
//  {"ქართული (Georgian)",              "ka_GE", 15},
    {"Deutsch (Deutsch)",               "de_DE", 16},
//  {"עברית (Hebrew)",                  "he_IL", 17},
    {"Italiano (Italian)",              "it_IT", 18},
    {"日本語 (Japanese)",                  "ja_JP", 19},
    {"한국어 (Korean)",                    "ko_KR", 20},
//  {"Norsk (Norwegian)",               "no_NO", 21},
//  {"Polski (Polish)",                 "pl_PL", 22},
//  {"Portugues (Portuguese)",          "pt_PT", 23},
    {"Português do Brasil (Brazilian Portuguese)", "pt_BR", 24},
//  {"Română (Romanian)",               "ro_RO", 25},
//  {"Русский (Russian)",               "ru_RU", 26},
//  {"සිංහල (Sinhala)",                 "si_LK", 27},
//  {"slovenčina (Slovak)",             "sk_SK", 28},
    {"slovenščina (Slovenian)",         "sl_SI", 29},
    {"Español (Spanish)",               "es_ES", 30},
    {"Svenska (Swedish)",               "sv_SE", 31},
//  {"ไทย (Thai)",                      "th_TH", 32},
//  {"Türkçe (Turkish)",                "tr_TR", 33},
//  {"Українська (Ukranian)",           "uk_UA", 34},
    {NULL},
};

static GhbStringMap video_quality_granularity_map[] = {
    {"0.2",  "0.2",  0.2 },
    {"0.25", "0.25", 0.25},
    {"0.5",  "0.5",  0.5 },
    {"1",    "1",    1   },
    {NULL},
};

static GhbStringMap when_complete_map[] = {
    {N_("Do Nothing"),  "nothing" },
    {N_("Quit"),        "quit",   },
    {N_("Sleep"),       "sleep",  },
    {N_("Shut Down"),   "shutdown"},
    {NULL},
};

static guint
ghb_string_map_n_items (GhbStringMap map[])
{
    guint n = 0;
    while (map[n].string != NULL) n++;
    return n;
}

static const double EPSILON = 0.0000001;

static gboolean
settings_get_value (GValue *value,
                    GVariant *variant, GhbStringMap map[])
{
    const char *type = g_variant_get_type_string(variant);
    if (!strncmp(type, "s", 1))
    {
        const char *id = g_variant_get_string(variant, NULL);
        for (int i = 0; map[i].string != NULL; i++)
        {
            if (!g_strcmp0(id, map[i].id))
            {
                g_value_set_uint(value, i);
                return TRUE;
            }
        }
    }
    else if (!strncmp(type, "i", 1))
    {
        int val = g_variant_get_int32(variant);
        for (int i = 0; map[i].string != NULL; i++)
        {
            if (val == map[i].val)
            {
                g_value_set_uint(value, i);
                return TRUE;
            }
        }
    }
    else if (!strncmp(type, "d", 1))
    {
        double val = g_variant_get_double(variant);
        for (int i = 0; map[i].string != NULL; i++)
        {
            if (fabs(val - map[i].val) < EPSILON)
            {
                g_value_set_uint(value, i);
                return TRUE;
            }
        }
    }
    return FALSE;
}

static GVariant *
settings_set_value (const GValue *value,
                    const GVariantType *expected_type, GhbStringMap map[])
{

    const char *type = g_variant_type_peek_string(expected_type); // not null-terminated
    guint idx = g_value_get_uint(value);

    if (idx < ghb_string_map_n_items(map))
    {
        if (!strncmp(type, "s", 1))
        {
            return g_variant_new_string(map[idx].id);
        }
        else if (!strncmp(type, "i", 1))
        {
            return g_variant_new_int32((int) map[idx].val);
        }
        else if (!strncmp(type, "d", 1))
        {
            return g_variant_new_double(map[idx].val);
        }
    }
    return NULL;
}

static void
_prefs_populate_drop_down (GtkDropDown *drop_down, GhbStringMap map[])
{
    GtkStringList *string_list = GTK_STRING_LIST(gtk_drop_down_get_model(drop_down));
    for (int i = 0; map[i].string != NULL; i++)
    {
        gtk_string_list_append(string_list, _(map[i].string));
    }
}

GtkWidget *
ghb_prefs_dialog_new (GhbPrefs *prefs)
{
    GhbPrefsDialog *dialog = g_object_new(GHB_TYPE_PREFS_DIALOG, NULL);
    dialog->prefs = g_object_ref(prefs);
    GSettings *gsettings = ghb_prefs_get_gsettings(prefs);
    GSettingsBindFlags flags = G_SETTINGS_BIND_DEFAULT | G_SETTINGS_BIND_NO_SENSITIVITY;
    dialog->requires_restart = FALSE;
    g_signal_connect(gsettings, "changed::custom-tmp-enable", G_CALLBACK(restart_required_cb), dialog);
    g_signal_connect(gsettings, "changed::custom-tmp-dir", G_CALLBACK(restart_required_cb), dialog);
    g_signal_connect(gsettings, "changed::ui-language", G_CALLBACK(restart_required_cb), dialog);

    if (ghb_power_manager_has_battery())
    {
        gtk_widget_set_visible(GTK_WIDGET(dialog->pause_encoding_on_low_battery), TRUE);
        gtk_widget_set_visible(GTK_WIDGET(dialog->pause_encoding_on_battery_power), TRUE);
    }
    g_settings_bind(gsettings, "activity-font-size", dialog->activity_font_size, "value", flags);
    g_settings_bind(gsettings, "auto-name", dialog->auto_name, "active", flags);
    g_settings_bind(gsettings, "auto-name-template", dialog->auto_name_template, "text", flags);
    g_settings_bind(gsettings, "auto-scan", dialog->auto_scan, "active", flags);
    g_settings_bind(gsettings, "custom-tmp-dir", dialog->custom_tmp_dir, "file", flags);
    g_settings_bind(gsettings, "custom-tmp-enable", dialog->custom_tmp_enable, "active", flags);
    g_settings_bind(gsettings, "disk-free-check", dialog->disk_free_check, "active", flags);
    g_settings_bind(gsettings, "disk-free-limit", dialog->disk_free_limit, "value", flags);
    g_settings_bind(gsettings, "encode-log-location", dialog->encode_log_location, "active", flags);
    g_settings_bind(gsettings, "excluded-file-extensions", dialog->excluded_file_extensions, "items", flags);
    g_settings_bind(gsettings, "hbfd-feature", dialog->hbfd_feature, "active", flags);
    g_settings_bind(gsettings, "keep-duplicate-titles", dialog->keep_duplicate_titles, "active", flags);
    g_settings_bind(gsettings, "limit-max-duration", dialog->limit_max_duration, "active", flags);
    g_settings_bind(gsettings, "max-title-duration", dialog->max_title_duration, "value", flags);
    g_settings_bind(gsettings, "min-title-duration", dialog->min_title_duration, "value", flags);
    g_settings_bind(gsettings, "notify-on-encode-done", dialog->notify_on_encode_done, "active", flags);
    g_settings_bind(gsettings, "notify-on-queue-done", dialog->notify_on_queue_done, "active", flags);
    g_settings_bind(gsettings, "pause-encoding-on-battery-power", dialog->pause_encoding_on_battery_power, "active", flags);
    g_settings_bind(gsettings, "pause-encoding-on-low-battery", dialog->pause_encoding_on_low_battery, "active", flags);
    g_settings_bind(gsettings, "pause-encoding-on-power-save", dialog->pause_encoding_on_power_save, "active", flags);
    g_settings_bind(gsettings, "preview-count", dialog->preview_count, "value", flags);
    g_settings_bind(gsettings, "remove-finished-jobs", dialog->remove_finished_jobs, "active", flags);
    g_settings_bind(gsettings, "send-file-to", dialog->send_file_to, "active", flags);
    g_settings_bind(gsettings, "send-file-to-target", dialog->send_file_to_target, "text", flags);
    g_settings_bind(gsettings, "show-mini-preview", dialog->show_mini_preview, "active", flags);
    g_settings_bind(gsettings, "sync-title-settings", dialog->sync_title_settings, "active", flags);
    g_settings_bind(gsettings, "use-dvdnav", dialog->use_dvdnav, "active", flags);
    g_settings_bind(gsettings, "use-m4v", dialog->use_m4v, "active", flags);

    _prefs_populate_drop_down(dialog->log_longevity, log_longevity_map);
    g_settings_bind_with_mapping(gsettings, "log-longevity", dialog->log_longevity, "selected", flags,
                                 (GSettingsBindGetMapping) settings_get_value,
                                 (GSettingsBindSetMapping) settings_set_value, &log_longevity_map, NULL);
    _prefs_populate_drop_down(dialog->logging_level, logging_level_map);
    g_settings_bind_with_mapping(gsettings, "logging-level", dialog->logging_level, "selected", flags,
                                 (GSettingsBindGetMapping) settings_get_value,
                                 (GSettingsBindSetMapping) settings_set_value, &logging_level_map, NULL);
    _prefs_populate_drop_down(dialog->ui_language, ui_language_map);
    g_settings_bind_with_mapping(gsettings, "ui-language", dialog->ui_language, "selected", flags,
                                 (GSettingsBindGetMapping) settings_get_value,
                                 (GSettingsBindSetMapping) settings_set_value, &ui_language_map, NULL);
    _prefs_populate_drop_down(dialog->video_quality_granularity, video_quality_granularity_map);
    g_settings_bind_with_mapping(gsettings, "video-quality-granularity", dialog->video_quality_granularity, "selected", flags,
                                 (GSettingsBindGetMapping) settings_get_value,
                                 (GSettingsBindSetMapping) settings_set_value, &video_quality_granularity_map, NULL);
    _prefs_populate_drop_down(dialog->when_complete, when_complete_map);
    g_settings_bind_with_mapping(gsettings, "when-complete", dialog->when_complete, "selected", flags,
                                 (GSettingsBindGetMapping) settings_get_value,
                                 (GSettingsBindSetMapping) settings_set_value, &when_complete_map, NULL);

    return GTK_WIDGET(dialog);
}

static void
restart_dialog_response_cb (GtkMessageDialog *dialog, int response, gpointer data)
{
    ghb_application_quit();
}

G_MODULE_EXPORT gboolean
prefs_dialog_close_request_cb (GhbPrefsDialog *self, gpointer data)
{
    g_return_val_if_fail(GHB_IS_PREFS_DIALOG(self), TRUE);
    if (self->requires_restart)
    {
        GtkWindow *main_window = gtk_window_get_transient_for(GTK_WINDOW(self));
        GtkMessageDialog *dialog = ghb_question_dialog_new(main_window, GHB_ACTION_NORMAL, _("_Quit"), NULL,
                                _("Settings Changed"),
                                _("You must restart HandBrake now."));
        g_signal_connect(dialog, "response", G_CALLBACK(restart_dialog_response_cb), NULL);
        gtk_window_present(GTK_WINDOW(dialog));
    }
    return FALSE;
}

G_MODULE_EXPORT void
easter_egg_multi_cb (GtkGesture *gest, int n_press, double x, double y, GhbPrefsDialog *self)
{
    g_return_if_fail(GHB_IS_PREFS_DIALOG(self));
    if (n_press == 3)
    {
        GtkWidget *widget = GTK_WIDGET(self->hbfd_feature);
        gtk_widget_set_visible(widget, !gtk_widget_get_visible(widget));
    }
}