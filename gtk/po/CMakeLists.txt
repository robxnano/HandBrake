# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(Gettext)

if(GETTEXT_FOUND)
  file(GLOB PO_FILES *.po)
  set(_mo_files)
  
  foreach(file ${PO_FILES})
    get_filename_component(_po_lang ${file} NAME_WE)
    set(_mo_file ${CMAKE_CURRENT_BINARY_DIR}/${_po_lang}/LC_MESSAGES/${GETTEXT_PACKAGE}.mo)
    set(_pot_file ${CMAKE_CURRENT_SOURCE_DIR}/${GETTEXT_PACKAGE}.pot)
    set(_po_file ${CMAKE_CURRENT_SOURCE_DIR}/${_po_lang}.po)
    add_custom_command(OUTPUT ${_mo_file}
                       COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} ${_po_file} -o ${_mo_file}
                       DEPENDS ${_po_file} ${_pot_file})
    list(APPEND _mo_files ${_mo_file})
    install(FILES ${_mo_file} DESTINATION ${CMAKE_INSTALL_LOCALEDIR}/${_po_lang}/LC_MESSAGES)
  endforeach()
  add_custom_target(${GETTEXT_PACKAGE}-mo ALL DEPENDS ${_mo_files})
else()
  message(WARNING "The gettext tools were not found. No translations will be installed.")
endif()
