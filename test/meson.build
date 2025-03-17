# Copyright (C) 2023-2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

cli_src = files(
  'parsecsv.c',
  'test.c',
)

cli = executable('HandBrakeCLI',
  sources: cli_src,
  dependencies: hb_deps,
  link_with: libhb,
  include_directories: hb_incdirs,
  link_language: 'cpp',
  link_args: get_option('c_link_args'),
  install: true)
