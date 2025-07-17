# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(Gettext)

set(PO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../po)

get_version_info(${CMAKE_SOURCE_DIR}/..)

set(METAINFO_FILE_IN_IN ${CMAKE_CURRENT_SOURCE_DIR}/fr.handbrake.ghb.metainfo.xml.in.in)
set(METAINFO_FILE_IN ${CMAKE_CURRENT_BINARY_DIR}/fr.handbrake.ghb.metainfo.xml.in)
set(METAINFO_FILE ${CMAKE_CURRENT_BINARY_DIR}/fr.handbrake.ghb.metainfo.xml)

set(DESKTOP_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/fr.handbrake.ghb.desktop.in)
set(DESKTOP_FILE ${CMAKE_CURRENT_BINARY_DIR}/fr.handbrake.ghb.desktop)

if(GETTEXT_FOUND)
  configure_file(${METAINFO_FILE_IN_IN} ${METAINFO_FILE_IN})

  add_custom_command(OUTPUT ${METAINFO_FILE}
    COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --xml --template ${METAINFO_FILE_IN} -o ${METAINFO_FILE} -d ${PO_DIR}
    DEPENDS ${METAINFO_FILE_IN_IN}
  )
  add_custom_target(ghb-metainfo ALL DEPENDS ${METAINFO_FILE})

  install(FILES ${METAINFO_FILE} DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)

  add_custom_command(OUTPUT ${DESKTOP_FILE}
    COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --desktop --template ${DESKTOP_FILE_IN} -o ${DESKTOP_FILE} -d ${PO_DIR}
    DEPENDS ${DESKTOP_FILE_IN})

  add_custom_target(ghb-desktop ALL DEPENDS ${DESKTOP_FILE})

  install(FILES ${DESKTOP_FILE} DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
else()
  # Just use the untranslated file as gettext is unavailable
  configure_file(${METAINFO_FILE_IN_IN} ${METAINFO_FILE})

  install(FILES ${METAINFO_FILE}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)

  install(FILES ${DESKTOP_FILE_IN}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    RENAME fr.handbrake.ghb.desktop)
endif()

if(BUILD_TESTING)
  find_program(APPSTREAMCLI_EXECUTABLE NAMES appstreamcli)
  if(APPSTREAMCLI_EXECUTABLE)
    add_test(NAME "Validate metainfo file"
      # We want to keep the developer_name tag for older Flatpak installations
      COMMAND ${APPSTREAMCLI_EXECUTABLE} validate --strict --no-net --explain
              --override developer-name-tag-deprecated=pedantic
              fr.handbrake.ghb.metainfo.xml
    )
  endif()

  find_program(DESKTOP_FILE_VALIDATE_EXECUTABLE NAMES desktop-file-validate)
  if(DESKTOP_FILE_VALIDATE_EXECUTABLE)
    add_test(NAME "Validate desktop file"
      COMMAND ${DESKTOP_FILE_VALIDATE_EXECUTABLE} fr.handbrake.ghb.desktop
    )
  endif()
endif()
