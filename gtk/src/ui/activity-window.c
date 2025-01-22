/* Copyright (C) 2025 HandBrake Team
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "ui/activity-window.h"

#include "callbacks.h"

struct _GhbActivityWindow
{
    GtkWindow parent_instance;

    GtkTextBuffer   *buffer;

    // template child widgets
    GtkLabel        *log_location;
    GtkTextView     *activity_view;
};

G_DEFINE_TYPE (GhbActivityWindow, ghb_activity_window, GTK_TYPE_WINDOW)

static void ghb_activity_window_dispose(GObject *object);
static void ghb_activity_window_finalize(GObject *object);

static void
ghb_activity_window_class_init (GhbActivityWindowClass *class_)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class_);
    GObjectClass *object_class = G_OBJECT_CLASS(class_);

    gtk_widget_class_set_template_from_resource(widget_class, "/fr/handbrake/ghb/ui/activity-window.ui");
    gtk_widget_class_bind_template_child(widget_class, GhbActivityWindow, log_location);
    gtk_widget_class_bind_template_child(widget_class, GhbActivityWindow, activity_view);

    object_class->finalize = ghb_activity_window_finalize;
    object_class->dispose = ghb_activity_window_dispose;
}

static void
ghb_activity_window_init (GhbActivityWindow *self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
}

static void
ghb_activity_window_dispose (GObject *object)
{
    GhbActivityWindow *self = GHB_ACTIVITY_WINDOW(object);
    gtk_text_view_set_buffer(self->activity_view, NULL);
    G_OBJECT_CLASS(ghb_activity_window_parent_class)->dispose(object);

#if GTK_CHECK_VERSION(4, 8, 0)
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    gtk_widget_dispose_template(GTK_WIDGET(object), GHB_TYPE_ACTIVITY_WINDOW);
G_GNUC_END_IGNORE_DEPRECATIONS
#endif
}

static void
ghb_activity_window_finalize (GObject *object)
{
    G_OBJECT_CLASS(ghb_activity_window_parent_class)->finalize(object);
}

GtkWidget *
ghb_activity_window_new (GhbApplication *app, GtkTextBuffer *buffer)
{
    GhbActivityWindow *window = g_object_new(GHB_TYPE_ACTIVITY_WINDOW, NULL);
    gtk_text_view_set_buffer(window->activity_view, buffer);
    gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(window));
    g_autofree char *log_file = ghb_application_get_log_file_name(app);
    gtk_label_set_label(window->log_location, log_file);

    return GTK_WIDGET(window);
}

G_MODULE_EXPORT void
show_activity_action_cb(GSimpleAction *action, GVariant *value,
                        signal_user_data_t *ud)
{
    static GtkWidget *activity_window = NULL;

    if (!activity_window)
        activity_window = ghb_activity_window_new(GHB_APPLICATION_DEFAULT, ud->activity_buffer);

    gtk_window_present(GTK_WINDOW(activity_window));
}
