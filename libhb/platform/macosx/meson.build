# Copyright (C) 2023-2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

add_languages('objc', native: false)
hb_deps += dependency('appleframeworks', modules: [
  'IOKit', 'CoreServices', 'CoreText', 'CoreGraphics', 'AudioToolbox',
  'VideoToolbox', 'CoreMedia', 'CoreVideo', 'IOSurface', 'Foundation',
  'DiskArbitration', 'Metal'])
xcrun = find_program('xcrun')
bin2c = find_program(meson.project_build_root() / 'bin2c')

libhb_src += files(
  'blend_vt.m',
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

metal_flags = []

metal_gen = generator(xcrun,
  arguments: ['metal',
    '-O2', '-std=macos-metal2.4', '-c', '-frecord-sources',
    '@INPUT@', '-o', '@OUTPUT@'],
  output: '@BASENAME@.air',
)
libhb_air = metal_gen.process(libhb_metal_src)
metallib_gen = generator(xcrun,
  arguments: ['metallib', '@INPUT@', '-o', '@OUTPUT@'],
  output: '@BASENAME@.metallib',
)
libhb_metallib = metallib_gen.process(libhb_air)

bin2c_gen = generator(bin2c,
  arguments: ['@INPUT@', '@OUTPUT@'],
  output: '@BASENAME@.c',
)
libhb_src += bin2c_gen.process(libhb_metallib)
