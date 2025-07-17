# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(Gettext)

if(GETTEXT_FOUND)
  file(GLOB PO_FILES *.po)
  foreach(po_file ${PO_FILES})
    get_filename_component(PO_LANG ${po_file} NAME_WE)
    gettext_process_po_files(${PO_LANG} ALL
      INSTALL_DESTINATION ${CMAKE_INSTALL_LOCALEDIR}
      PO_FILES ${po_file}
    )
  endforeach()
else()
  message(WARNING "The gettext tools were not found. No translations will be installed.")
endif()
