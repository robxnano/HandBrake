# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

target_sources(handbrake-gtk PRIVATE
  application.c application.h
  audiohandler.c audiohandler.h
  callbacks.c callbacks.h
  chapters.c chapters.h
  color-scheme.c color-scheme.h
  ghb-button.c ghb-button.h
  ghb-chapter-row.c ghb-chapter-row.h
  ghb-file-button.c ghb-file-button.h
  ghb-queue-row.c ghb-queue-row.h
  ghb-string-list.c ghb-string-list.h
  hb-backend.c hb-backend.h
  hb-dvd.c hb-dvd.h
  jobdict.c jobdict.h
  main.c
  notifications.c notifications.h
  power-manager.c power-manager.h
  presets.c presets.h
  preview.c preview.h
  queuehandler.c queuehandler.h
  resources.c resources.h
  settings.c settings.h
  subtitlehandler.c subtitlehandler.h
  title-add.c title-add.h
  titledict.c titledict.h
  util.c util.h
  values.c values.h
  videohandler.c videohandler.h
)

glib_compile_resources(icon_res
  C_NAME ghb_icon
  XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../icons/icon_res.gresource.xml
  TARGET handbrake-gtk)

glib_compile_resources(ui_res
  C_NAME ghb_ui
  XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ui/ui_res.gresource.xml
  TARGET handbrake-gtk)

glib_compile_resources(data_res
  C_NAME ghb_data
  XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../data/data_res.gresource.xml
  TARGET handbrake-gtk)

configure_file(config.h.in config.h)
