# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(Gettext)

# Definitions needed by gettext
set(GETTEXT_PACKAGE ghb)
set(ENABLE_NLS ON)
set(PACKAGE_LOCALE_DIR ${CMAKE_INSTALL_LOCALEDIR})

set(GTK_MIN_VER 4.6)
set(GLIB_MIN_VER 2.72)

add_executable(handbrake-gtk WIN32 MACOSX_BUNDLE)
set_target_properties(handbrake-gtk PROPERTIES LINKER_LANGUAGE CXX)
if(WIN32 OR APPLE)
  set_target_properties(handbrake-gtk PROPERTIES OUTPUT_NAME HandBrakeGTK)
else()
  set_target_properties(handbrake-gtk PROPERTIES OUTPUT_NAME ghb)
endif()

target_add_pkg(handbrake-gtk gtk4 PRIVATE MODULES "gtk4 >= ${GTK_MIN_VER}")
target_add_pkg(handbrake-gtk glib PRIVATE MODULES "glib-2.0 >= ${GLIB_MIN_VER}" gio-2.0 gthread-2.0 gmodule-2.0)
target_add_pkg(handbrake-gtk libxml-2.0 PRIVATE)

target_link_libraries(handbrake-gtk PRIVATE libhb)
target_include_directories(handbrake-gtk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

if(NOT CMAKE_C_EXTENSIONS)
  target_compile_definitions(handbrake-gtk PRIVATE _DEFAULT_SOURCE)
endif()

# Don't allow GTK functions introduced after the minimum required version
string(REPLACE "." "_" GTK_MIN_VER_U ${GTK_MIN_VER})
target_compile_definitions(handbrake-gtk PRIVATE
  GDK_VERSION_MIN_REQUIRED=GDK_VERSION_${GTK_MIN_VER_U}
  GDK_VERSION_MAX_ALLOWED=GDK_VERSION_${GTK_MIN_VER_U}
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

# Needed to allow callbacks in UI files to work
target_link_options(handbrake-gtk PRIVATE LINKER:--export-dynamic LINKER:--exclude-libs,ALL)

add_subdirectory(po)
add_subdirectory(data)
add_subdirectory(icons)
add_subdirectory(src)

install(TARGETS handbrake-gtk)

# Rebuild icon and desktop file caches after install
configure_file(data/post_install.cmake.in post_install.cmake @ONLY)
install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/post_install.cmake)
