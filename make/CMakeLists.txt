# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

# CMake build system for HandBrake
# This is the root CMakeLists.txt file, placed in the make directory
# to discourage users from trying to use it directly. Instead users
# should run the configure script and make in the build directory.

cmake_minimum_required(VERSION 3.16...4.1)
project(HandBrake LANGUAGES C CXX)

if(APPLE)
  enable_language(OBJC)
elseif(WIN32)
  # We need to ensure that the Windows executable is linked to
  # the private libraries listed in the .pc files. As CMake doesn't
  # have an option to do this automatically, we override the pkg-config
  # arguments with --static as a workaround. Requires CMake 3.22.
  set(PKG_CONFIG_ARGN ${PKG_CONFIG_ARGN} --static)
endif()
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(CheckFunctionExists)
include(CTest)
include(GLibCompileResources)
include(GNUInstallDirs)
include(TargetPkgConfig)
include(VersionInfo)
# Options set by the configure.py script
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/hb_options.cmake)
  include(${CMAKE_CURRENT_BINARY_DIR}/hb_options.cmake)
else()
  message(FATAL_ERROR "Build directory has not been configured yet. Please run ./configure and make instead of using CMake directly.")
endif()

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# HandBrake static library/Windows DLL
add_subdirectory(${CMAKE_SOURCE_DIR}/../libhb libhb)

# HandBrakeCLI binary
add_subdirectory(${CMAKE_SOURCE_DIR}/../test cli)

# Linux GUI
if(FEATURE_gtk)
  add_subdirectory(${CMAKE_SOURCE_DIR}/../gtk gtk)
endif()

if(NOT TARGET uninstall)
  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake)
endif()
