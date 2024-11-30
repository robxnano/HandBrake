# CMake build system for HandBrake
# Copyright (C) 2024 HandBrake Team
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16..3.30)
cmake_policy(SET CMP0116 NEW)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-tree builds are not supported. Create a build directory first.")
endif()

add_compile_options(-Wall)

project(handbrake VERSION 1.9.0
  LANGUAGES C CXX
  HOMEPAGE_URL https://handbrake.fr)

set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_SOURCE_DIR}/download)
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CheckFunctionExists)
include(CPM)
include(ExternalProject)
include(FindGettext)
include(FindPkgConfig)
include(GNUInstallDirs)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if(UNIX AND NOT APPLE)
  set(GTK_DEFAULT ON)
else()
  set(GTK_DEFAULT OFF)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(NUMA_DEFAULT ON)
else()
  set(NUMA_DEFAULT OFF)
endif()

option(ENABLE_X265 "Enable x265 encoder" ON)
option(ENABLE_NUMA "Enable x265 NUMA support" ${NUMA_DEFAULT})
if(WIN32)
  option(ENABLE_MF "Enable ARM MediaFoundation hardware encoder" OFF)
endif()
option(ENABLE_VCN "Enable AMD VCN hardware encoder" OFF)
option(ENABLE_QSV "Enable Intel QSV hardware encoder" OFF)
option(ENABLE_NVENC "Enable Nvidia hardware encoder" OFF)
option(ENABLE_NVDEC "Enable Nvidia hardware decoder" OFF)
option(ENABLE_LIBDOVI "Enable Dolby Vision support" ON)
option(ENABLE_FDK_AAC "Enable FDK AAC audio encoder" OFF)
option(ENABLE_GTK "Enable GTK GUI" ${GTK_DEFAULT})

if(NOT APPLE)
  set(HB_PROJECT_FEATURE_FFMPEG_AAC ON)
endif()
if(ENABLE_FDK_AAC)
  message(WARNING "Builds with FDK AAC encoder are for personal use only and cannot be legally distributed.")
  set(HB_PROJECT_FEATURE_FDK_AAC ON)
endif()
if(ENABLE_MF)
  set(HB_PROJECT_FEATURE_MF ON)
endif()
if(ENABLE_NVENC)
  set(HB_PROJECT_FEATURE_NVENC ON)
endif()
if(ENABLE_NVDEC)
  set(HB_PROJECT_FEATURE_NVDEC ON)
endif()
if(ENABLE_QSV)
  set(HB_PROJECT_FEATURE_QSV ON)
endif()
if(ENABLE_VCN)
  set(HB_PROJECT_FEATURE_VCE ON)
endif()
if(ENABLE_X265)
  set(HB_PROJECT_FEATURE_X265 ON)
endif()
if(ENABLE_LIBDOVI)
  set(HB_PROJECT_FEATURE_LIBDOVI ON)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(libhb)
add_subdirectory(test)
add_subdirectory(contrib)

if(ENABLE_GTK)
  set(HB_PROJECT_FEATURE_GTK ON)
  add_subdirectory(gtk)
endif()
