# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.16...4.0)
project(handbrake LANGUAGES C CXX)

if(APPLE)
  enable_language(OBJC)
elseif(WIN32)
  # We need to ensure that the Windows executable is linked to
  # the private libraries listed in the .pc files. As CMake doesn't
  # have an option to do this automatically, we override the pkg-config
  # arguments with --static as a workaround. Requires CMake 3.22.
  set(PKG_CONFIG_ARGN ${PKG_CONFIG_ARGN} --static)
endif()
include(CheckFunctionExists)
include(CTest)
include(FindGettext)
include(FindPkgConfig)
include(GNUInstallDirs)

# Options set by the configure.py script
include(${CMAKE_CURRENT_BINARY_DIR}/hb_options.cmake)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# HandBrake static/shared libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libhb libhb)

# HandBrakeCLI binary
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../test cli)

# Linux GUI
if(FEATURE_gtk)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../gtk gtk)
endif()

# Uninstall command
if(NOT TARGET uninstall)
  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake)
endif()
