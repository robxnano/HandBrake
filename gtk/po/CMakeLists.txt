if(GETTEXT_FOUND)
  file(GLOB PO_FILES *.po)
  set(PO_LANGUAGES)

  foreach(PO_FILE ${PO_FILES})
    get_filename_component(PO_LANG ${PO_FILE} NAME_WE)
    list(APPEND PO_LANGUAGES ${PO_LANG})
    file(COPY ${PO_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  endforeach()

  gettext_process_pot_file(ghb.pot ALL INSTALL_DESTINATION ${CMAKE_INSTALL_LOCALEDIR} LANGUAGES ${PO_LANGUAGES})
else()
  message(WARNING "The gettext tools are required for translations.")
endif()
