# Copyright (C) 2023-2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

add_languages('objc')

libhb_src += files(
  'chroma_smooth_vt.m',
  'comb_detect_vt.m',
  'config.m',
  'cropscale_vt.c',
  'cv_utils.c',
  'deinterlace_vt.m',
  'encca_aac.c',
  'encvt.c',
  'grayscale_vt.m',
  'lapsharp_vt.m',
  'metal_utils.m',
  'motion_metric_vt.m',
  'pad_vt.m',
  'prefilter_vt.c',
  'rotate_vt.c',
  'unsharp_vt.m',
  'vt_common.c',
)

libhb_metal_src = files(
  'shaders/blend_vt.metal',
  'shaders/bwdif_vt.metal',
  'shaders/chroma_smooth_vt.metal',
  'shaders/comb_detect_vt.metal',
  'shaders/grayscale_vt.metal',
  'shaders/lapsharp_vt.metal',
  'shaders/motion_metric_vt.metal',
  'shaders/pad_vt.metal',
  'shaders/unsharp_vt.metal',
  'shaders/yadif_vt.metal',
)

# metal_gen = generator(['xcrun', 'metal'],
#   arguments: ['-O2', '-std=macos-metal2.4', '@INPUT@'])
#