# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

project('HandBrake', 'c', 'cpp',
  version: run_command('scripts/version.py', check: true).stdout().strip(),
  license: 'GPL-2.0-only',
  meson_version: '>= 0.60.0',
  default_options: ['warning_level=1', 'buildtype=release'])

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')
hb_incdirs = include_directories('libhb')
hb_libdirs = meson.current_build_dir() / 'contrib/lib'
build_config = meson.project_build_root() / 'build.config'

fs = import('fs')
if not fs.is_file(build_config)
  error('The project has not been configured yet.\nPlease use ./configure and make instead of using Meson directly.')
endif

# External dependencies (required)
hb_deps = [
  dependency('dvdnav'),
  dependency('dvdread'),
  dependency('SvtAv1Enc'),
  dependency('jansson'),
  dependency('libass'),
  dependency('libavcodec'),
  dependency('libavfilter'),
  dependency('libavformat'),
  dependency('libavutil'),
  dependency('libbluray'),
  dependency('libcpuinfo'),
  dependency('liblzma'),
  dependency('libswresample'),
  dependency('libswscale'),
  dependency('ogg'),
  dependency('theoradec'),
  dependency('theoraenc'),
  dependency('threads'),
  dependency('libturbojpeg'),
  dependency('vorbis'),
  dependency('vorbisenc'),
  dependency('vpx'),
  dependency('x264'),
]

if get_option('libdovi').enabled()
  hb_deps += dependency('dovi')
endif

if get_option('fdk-aac').enabled()
  hb_deps += dependency('fdk-aac')
endif

if get_option('qsv').enabled()
  hb_deps += dependency('vpl')
endif

if get_option('x265').enabled()
  hb_deps += dependency('x265')
  if get_option('numa').enabled()
    numa = dependency('numa', required: false)
    if not numa.found()
      numa = cc.find_library('numa')
    endif
    hb_deps += numa
  endif
endif

if get_option('mf').enabled()
  hb_deps += [cc.find_library('mfplat'), cc.find_library('strmiids')]
endif

# OS-specific dependencies

if host_machine.system() == 'windows'
  win_dlls = ['bcrypt', 'ntdll', 'ole32', 'regex', 'userenv', 'uuid', 'ws2_32']
  foreach dll: win_dlls
    hb_deps += cc.find_library(dll, dirs: hb_libdirs)
  endforeach
elif not cc.has_function('dlopen')
  hb_deps += cc.find_library('dl')
endif

if not cc.has_function('iconv')
  libiconv = cc.find_library('iconv', required: false)
  if not libiconv.found()
    libiconv = cc.find_library('iconv', dirs: hb_libdirs)
  endif
  hb_deps += libiconv
  if host_machine.system() != 'windows'
    add_project_arguments('-DLIBICONV_PLUG', language: ['c', 'cpp', 'objc'])
  endif
endif

if not get_option('c_std').contains('gnu')
  add_project_arguments('-D_DEFAULT_SOURCE', language: ['c', 'cpp', 'objc'])
endif

link_args = cpp.get_supported_link_arguments('-Wl,--exclude-libs,ALL')

add_project_link_arguments(link_args, language: 'cpp')

gtk_enabled = get_option('gtk').enabled()

subdir('libhb')

if host_machine.system() == 'darwin'
  subdir('macosx')
endif

if gtk_enabled
  subdir('gtk')
endif

# CLI
cli_src = files(
  'test/parsecsv.c',
  'test/test.c',
)

executable('HandBrakeCLI',
  sources: cli_src,
  dependencies: hb_deps,
  link_with: libhb,
  include_directories: hb_incdirs,
  link_language: 'cpp',
  link_args: get_option('c_link_args'),
  install: true)
