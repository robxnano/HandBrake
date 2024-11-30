set(PO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../po)
set(METAINFO_FILE_IN_IN ${CMAKE_CURRENT_SOURCE_DIR}/fr.handbrake.ghb.metainfo.xml.in.in)
set(METAINFO_FILE_IN ${CMAKE_CURRENT_BINARY_DIR}/fr.handbrake.ghb.metainfo.xml.in)
set(METAINFO_FILE ${CMAKE_CURRENT_BINARY_DIR}/fr.handbrake.ghb.metainfo.xml)
set(DESKTOP_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/fr.handbrake.ghb.desktop.in)
set(DESKTOP_FILE ${CMAKE_CURRENT_BINARY_DIR}/fr.handbrake.ghb.desktop)

#TODO: Fix the version and release date in the metainfo
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
  configure_file(${METAINFO_FILE} ${METAINFO_FILE_IN_IN})
  install(FILES ${METAINFO_FILE} DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)

  install(FILES ${DESKTOP_FILE_IN} DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    RENAME fr.handbrake.ghb.desktop)
endif()
