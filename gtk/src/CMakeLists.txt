# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

target_sources(handbrake-gtk PRIVATE
  application.c
  audiohandler.c
  callbacks.c
  chapters.c
  color-scheme.c
  ghb-button.c
  ghb-chapter-row.c
  ghb-file-button.c
  ghb-queue-row.c
  ghb-string-list.c
  hb-backend.c
  hb-dvd.c
  jobdict.c
  main.c
  notifications.c
  power-manager.c
  presets.c
  preview.c
  queuehandler.c
  resources.c
  settings.c
  subtitlehandler.c
  title-add.c
  titledict.c
  util.c
  values.c
  videohandler.c
)

set(GETTEXT_PACKAGE ghb)
set(ENABLE_NLS ON)
set(PACKAGE_LOCALE_DIR ${CMAKE_INSTALL_LOCALEDIR})

configure_file(config.h.in config.h)
