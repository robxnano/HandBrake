add_executable(handbrake-gtk)
set_target_properties(handbrake-gtk PROPERTIES LINKER_LANGUAGE CXX)
if(WIN32)
  set_target_properties(handbrake-gtk PROPERTIES OUTPUT_NAME HandBrakeGTK)
else()
  set_target_properties(handbrake-gtk PROPERTIES OUTPUT_NAME ghb)
endif()

pkg_check_modules(gtk4 REQUIRED IMPORTED_TARGET "gtk4 >= 4.4")
pkg_check_modules(glib REQUIRED IMPORTED_TARGET "glib-2.0 >= 2.68" gio-2.0 gthread-2.0 gmodule-2.0)
pkg_check_modules(libxml2 REQUIRED IMPORTED_TARGET libxml-2.0)

target_link_libraries(handbrake-gtk PRIVATE libhb PkgConfig::gtk4 PkgConfig::glib PkgConfig::libxml2)
target_include_directories(handbrake-gtk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

if(ENABLE_MF)
  target_link_libraries(handbrake-gtk PRIVATE mfplat strmiids)
endif()

if(NOT CMAKE_C_EXTENSIONS)
  target_compile_definitions(handbrake-gtk PRIVATE _DEFAULT_SOURCE)
endif()

check_function_exists(strerror_r HAVE_STRERROR_R)
if(HAVE_STRERROR_R)
  target_compile_definitions(handbrake-gtk PRIVATE HAS_STRERROR_R)
endif()

target_compile_definitions(handbrake-gtk PRIVATE
  GDK_VERSION_MIN_REQUIRED=GDK_VERSION_4_4
  GDK_VERSION_MAX_ALLOWED=GDK_VERSION_4_4
)

target_compile_options(handbrake-gtk PRIVATE
  -Wno-missing-field-initializers
  -Wno-sign-compare
  -Wno-unused-parameter
  -Wignored-qualifiers
  -Wimplicit-function-declaration
  -Wincompatible-pointer-types
  -Wint-conversion
  -Wint-to-pointer-cast
  -Wlogical-op
  -Wmissing-format-attribute
  -Wmissing-noreturn
  -Wmissing-include-dirs
  -Wnested-externs
  -Wold-style-definition
  -Woverflow
  -Wpointer-to-int-cast
  -Wshadow
  -Wstrict-prototypes
  -Wunused
  -Wwrite-strings
)

target_link_options(handbrake-gtk PRIVATE -Wl,--export-dynamic -Wl,--exclude-libs,ALL)

#add_subdirectory(po)
#add_subdirectory(data)
add_subdirectory(src)


find_program(GCR_EXE glib-compile-resources)

set(ICONS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/icons)
add_custom_command(OUTPUT src/icon_res.c src/icon_res.h
  COMMAND ${GCR_EXE} --generate-source --target=src/icon_res.c --c-name ghb_icon ${ICONS_DIR}/icon_res.gresource.xml --sourcedir ${ICONS_DIR} --dependency-file icon_res.deps
  COMMAND ${GCR_EXE} --generate-header --target=src/icon_res.h --c-name ghb_icon ${ICONS_DIR}/icon_res.gresource.xml --sourcedir ${ICONS_DIR}
  DEPFILE icon_res.deps
  DEPENDS ${ICONS_DIR}/icon_res.gresource.xml
)

set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
add_custom_command(OUTPUT src/data_res.c src/data_res.h
  COMMAND ${GCR_EXE} --generate-source --target=src/data_res.c --c-name ghb_data ${DATA_DIR}/data_res.gresource.xml --sourcedir ${DATA_DIR} --dependency-file data_res.deps
  COMMAND ${GCR_EXE} --generate-header --target=src/data_res.h --c-name ghb_data ${DATA_DIR}/data_res.gresource.xml --sourcedir ${DATA_DIR}
  DEPFILE data_res.deps
  DEPENDS ${DATA_DIR}/data_res.gresource.xml
)

set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)
add_custom_command(OUTPUT src/ui_res.c src/ui_res.h
  COMMAND ${GCR_EXE} --generate-source --target=src/ui_res.c --c-name ghb_ui ${UI_DIR}/ui_res.gresource.xml --sourcedir ${UI_DIR} --dependency-file ui_res.deps
  COMMAND ${GCR_EXE} --generate-header --target=src/ui_res.h --c-name ghb_ui ${UI_DIR}/ui_res.gresource.xml --sourcedir ${UI_DIR}
  DEPFILE ui_res.deps
  DEPENDS ${UI_DIR}/ui_res.gresource.xml
)
target_sources(handbrake-gtk PRIVATE src/icon_res.c src/data_res.c src/ui_res.c)

install(TARGETS handbrake-gtk)
install(FILES icons/32x32/apps/fr.handbrake.ghb.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/32x32/apps)
install(FILES icons/128x128/apps/fr.handbrake.ghb.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/128x128/apps)
install(FILES icons/256x256/apps/fr.handbrake.ghb.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps)
install(FILES icons/scalable/apps/fr.handbrake.ghb.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)

add_subdirectory(po)
add_subdirectory(data)
