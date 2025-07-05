# Copyright (C) 2023-2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

keyval = import('keyval')
values = keyval.load(build_config)

conf = configuration_data()
conf.set_quoted('HB_PROJECT_TITLE', values['HB_title'])
conf.set_quoted('HB_PROJECT_NAME', values['HB_name'])
conf.set_quoted('HB_PROJECT_NAME_LOWER', values['HB_name_lower'])
conf.set_quoted('HB_PROJECT_NAME_UPPER', values['HB_name_upper'])
conf.set_quoted('HB_PROJECT_URL_WEBSITE', values['HB_url_website'])
conf.set_quoted('HB_PROJECT_URL_COMMUNITY', values['HB_url_community'])
conf.set_quoted('HB_PROJECT_URL_IRC', values['HB_url_irc'])
conf.set_quoted('HB_PROJECT_URL_APPCAST', values['HB_url_appcast'])
conf.set('HB_PROJECT_VERSION_MAJOR', values['HB_version_major'])
conf.set('HB_PROJECT_VERSION_MINOR', values['HB_version_minor'])
conf.set('HB_PROJECT_VERSION_POINT', values['HB_version_point'])
conf.set_quoted('HB_PROJECT_VERSION', values['HB_version'])
conf.set('HB_PROJECT_VERSION_HEX', '0x@0@LL'.format(values['HB_version_hex'].to_upper()))
conf.set('HB_PROJECT_BUILD', values['HB_build'])
conf.set_quoted('HB_PROJECT_REPO_URL', values['HB_repo_url'])
conf.set_quoted('HB_PROJECT_REPO_TAG', values['HB_repo_tag'])
conf.set('HB_PROJECT_REPO_REV', values['HB_repo_rev'])
conf.set_quoted('HB_PROJECT_REPO_HASH', values['HB_repo_hash'])
conf.set_quoted('HB_PROJECT_REPO_BRANCH', values['HB_repo_branch'])
conf.set_quoted('HB_PROJECT_REPO_REMOTE', values['HB_repo_remote'])
conf.set_quoted('HB_PROJECT_REPO_TYPE', values['HB_repo_type'])
conf.set('HB_PROJECT_REPO_OFFICIAL', values['HB_repo_official'])
conf.set_quoted('HB_PROJECT_REPO_DATE', values['HB_repo_date'])

conf.set_quoted('HB_PROJECT_HOST_SPEC', values['HOST_spec'])
conf.set_quoted('HB_PROJECT_HOST_MACHINE', values['HOST_machine'])
conf.set_quoted('HB_PROJECT_HOST_VENDOR', values['HOST_vendor'])
conf.set_quoted('HB_PROJECT_HOST_SYSTEM', values['HOST_system'])
conf.set_quoted('HB_PROJECT_HOST_SYSTEMF', values['HOST_systemf'])
conf.set_quoted('HB_PROJECT_HOST_RELEASE', values['HOST_release'])
conf.set_quoted('HB_PROJECT_HOST_TITLE', values['HOST_title'])
conf.set_quoted('HB_PROJECT_HOST_ARCH', values['HOST_arch'])

conf.set('HB_PROJECT_FEATURE_ASM', get_option('asm').to_int())
conf.set('HB_PROJECT_FEATURE_FDK_AAC', get_option('fdk-aac').enabled().to_int())
conf.set('HB_PROJECT_FEATURE_FFMPEG_AAC', (not get_option('fdk-aac').enabled()).to_int())
conf.set('HB_PROJECT_FEATURE_FLATPAK', get_option('flatpak').to_int())
conf.set('HB_PROJECT_FEATURE_GTK', gtk_enabled.to_int())
conf.set('HB_PROJECT_FEATURE_MF', get_option('mf').enabled().to_int())
conf.set('HB_PROJECT_FEATURE_NVENC', get_option('nvenc').enabled().to_int())
conf.set('HB_PROJECT_FEATURE_NVDEC', get_option('nvdec').enabled().to_int())
conf.set('HB_PROJECT_FEATURE_QSV', get_option('qsv').enabled().to_int())
conf.set('HB_PROJECT_FEATURE_VCE', get_option('vcn').enabled().to_int())
conf.set('HB_PROJECT_FEATURE_X265', get_option('x265').enabled().to_int())
conf.set('HB_PROJECT_FEATURE_NUMA', get_option('numa').enabled().to_int())
conf.set('HB_PROJECT_FEATURE_LIBDOVI', get_option('libdovi').enabled().to_int())
conf.set('HB_PROJECT_SECURITY_HARDEN', get_option('harden').to_int())

project_h = configure_file(configuration: conf, output: 'project.h')

libhb_src += project_h

if host_machine.system() == 'darwin'
  run_command('sh', '-c', 'cp *.h ' + meson.current_build_dir(), check: true)
endif