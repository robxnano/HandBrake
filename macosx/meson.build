# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

plist = configuration_data()
plist.set('HB_PROJECT_NAME', values['HB_name'])
plist.set('HB_PROJECT_BUILD', values['HB_build'])
plist.set('HB_PROJECT_VERSION', values['HB_version'])
plist.set('HB_PROJECT_URL_APPCAST', values['HB_url_appcast'])

configure_file(input: 'Info.plist.in',
  output: 'Info.plist',
  configuration: plist)