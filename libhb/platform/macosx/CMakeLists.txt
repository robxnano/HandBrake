# Copyright (C) 2023-2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

target_sources(libhandbrake PRIVATE
  blend_vt.m
  chroma_smooth_vt.m
  comb_detect_vt.m
  config.m
  cropscale_vt.c
  cv_utils.c
  deinterlace_vt.m
  encca_aac.c
  encvt.c
  grayscale_vt.m
  lapsharp_vt.m
  metal_utils.m
  motion_metric_vt.m
  pad_vt.m
  prefilter_vt.c
  rotate_vt.c
  unsharp_vt.m
  vt_common.c
)

find_program(XCRUN REQUIRED NAMES xcrun)
find_program(BIN2C REQUIRED NAMES bin2c PATHS ${CMAKE_BINARY_DIR})

set(HB_METAL_SRC
  shaders/blend_vt.metal
  shaders/bwdif_vt.metal
  shaders/chroma_smooth_vt.metal
  shaders/comb_detect_vt.metal
  shaders/grayscale_vt.metal
  shaders/lapsharp_vt.metal
  shaders/motion_metric_vt.metal
  shaders/pad_vt.metal
  shaders/unsharp_vt.metal
  shaders/yadif_vt.metal
)

add_library(hb_metal OBJECT)

foreach(name ${HB_METAL_SRC})
  get_filename_component(name_we ${name} NAME_WE)
  add_custom_command(OUTPUT ${name_we}.c
    COMMAND ${XCRUN} metal -O2 -std=macos-metal2.4 -c -frecord-sources ${CMAKE_CURRENT_SOURCE_DIR}/${name} -o ${CMAKE_CURRENT_BINARY_DIR}/${name_we}.air
    COMMAND ${XCRUN} metallib ${CMAKE_CURRENT_BINARY_DIR}/${name_we}.air -o ${CMAKE_CURRENT_BINARY_DIR}/${name_we}.metallib
    COMMAND ${BIN2C} ${CMAKE_CURRENT_BINARY_DIR}/${name_we}.metallib ${CMAKE_CURRENT_BINARY_DIR}/${name_we}.c
    DEPENDS ${name}
    BYPRODUCTS ${name_we}.air ${name_we}.metallib
  )
  target_sources(hb_metal PRIVATE ${name_we}.c)
endforeach()

target_sources(libhandbrake PRIVATE $<TARGET_OBJECTS:hb_metal>)

configure_file(${CMAKE_SOURCE_DIR}/../macosx/Info.plist.in ${CMAKE_BINARY_DIR}/macosx/Info.plist @ONLY)
