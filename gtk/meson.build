# Copyright (C) 2023-2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

i18n = import('i18n')
gnome = import('gnome')

# Minimum versions for dependencies
glib_min = '>= 2.68'
gtk4_min = '>= 4.4'

ghb_deps = [
  dependency('glib-2.0', version: glib_min),
  dependency('gio-2.0', version: glib_min),
  dependency('gthread-2.0', version: glib_min),
  dependency('gmodule-2.0', version: glib_min),
  dependency('gtk4', version: gtk4_min),
  dependency('libxml-2.0'),
]

ghb_args = [
  '-DGDK_VERSION_MIN_REQUIRED=GDK_VERSION_4_4',
  '-DGDK_VERSION_MAX_ALLOWED=GDK_VERSION_4_4',
]

ghb_args += cc.get_supported_arguments(
  '-Wno-missing-field-initializers',
  '-Wno-sign-compare',
  '-Wno-unused-parameter',
  '-Wignored-qualifiers',
  '-Wimplicit-function-declaration',
  '-Wincompatible-pointer-types',
  '-Wint-conversion',
  '-Wint-to-pointer-cast',
  '-Wlogical-op',
  '-Wmissing-format-attribute',
  '-Wmissing-noreturn',
  '-Wmissing-include-dirs',
  '-Wnested-externs',
  '-Wold-style-definition',
  '-Woverflow',
  '-Wpointer-to-int-cast',
  '-Wshadow',
  '-Wstrict-prototypes',
  '-Wunused',
  '-Wwrite-strings')

ghb_link_args = cpp.get_supported_link_arguments('-Wl,--export-dynamic')

subdir('po')
subdir('icons')
subdir('data')
subdir('src')
