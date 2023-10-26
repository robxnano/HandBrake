/* main.c
 *
 * Copyright (C) 2008-2024 John Stebbins <stebbins@stebbins>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "application.h"
#include "ghb_rs.h"
#include "server.h"
#include "settings.h"
#include "ui_res.h"

int
main (int argc, char *argv[])
{
    rust_hello1();
    g_autofree char *app_cmd = (argc > 0) ? g_strdup(argv[0]) : NULL;

    // Quick redirect to worker process without initializing GUI
    if (argc > 1 && !g_strcmp0(argv[1], "--worker"))
        return ghb_worker_main(argc, argv);

    if (argc > 0)
        app_cmd = g_strdup(argv[0]);

#if defined(_WIN32)
    // Tell gdk pixbuf where it's loader config file is.
    _putenv_s("GDK_PIXBUF_MODULE_FILE", "ghb.exe.local/loaders.cache");
#endif

    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    g_autoptr(GApplication) app = G_APPLICATION(ghb_application_new(app_cmd));
    return g_application_run(app, argc, argv);
}
