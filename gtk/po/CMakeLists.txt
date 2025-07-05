# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

if(GETTEXT_FOUND)
  file(GLOB PO_FILES *.po)
  foreach(PO_FILE ${PO_FILES})
    get_filename_component(PO_LANG ${PO_FILE} NAME_WE)
    gettext_process_po_files(${PO_LANG} ALL
      INSTALL_DESTINATION ${CMAKE_INSTALL_LOCALEDIR}
      PO_FILES ${PO_FILE}
    )
  endforeach()
else()
  message(WARNING "The gettext tools are required for translations.")
endif()
