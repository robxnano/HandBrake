# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

set(HB_PROJECT_BUILD 0x1)
set(HB_PROJECT_REPO_REV 0xffffffff)
set(HB_PROJECT_REPO_OFFICIAL 0)
configure_file(handbrake/project.h.in handbrake/project.h)

if(WIN32)
  add_library(libhb SHARED)
  # We need the DLL name to be hb.dll
  set_target_properties(libhb PROPERTIES PREFIX "")
else()
  add_library(libhb STATIC)
  set_target_properties(libhb PROPERTIES OUTPUT_NAME handbrake)
endif()

target_include_directories(libhb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

target_sources(libhb PRIVATE
  audio_remap.c
  audio_resample.c
  avfilter.c
  batch.c
  bd.c
  bitstream.c
  blend.c
  chroma_smooth.c
  colormap.c
  colorspace.c
  comb_detect.c
  common.c
  compat.c
  cropscale.c
  deblock.c
  decavcodec.c
  decavsub.c
  declpcm.c
  decomb.c
  decsrtsub.c
  decssasub.c
  dectx3gsub.c
  deinterlace.c
  demuxmpeg.c
  denoise.c
  detelecine.c
  dovi_common.c
  dvd.c
  dvdnav.c
  eedi2.c
  encavcodecaudio.c
  encavcodec.c
  encsvtav1.c
  enctheora.c
  encvorbis.c
  encavsub.c
  enctx3gsub.c
  encx264.c
  encx265.c
  extradata.c
  fifo.c
  format.c
  grayscale.c
  hbavfilter.c
  hb.c
  hb_dict.c
  hbffmpeg.c
  hb_json.c
  hdr10plus.c
  hwaccel.c
  lang.c
  lapsharp.c
  mf_common.c
  motion_metric.c
  mt_frame_filter.c
  muxavformat.c
  muxcommon.c
  nal_units.c
  nlmeans.c
  nlmeans_x86.c
  nvenc_common.c
  pad.c
  param.c
  ports.c
  preset.c
  reader.c
  rendersub.c
  rotate.c
  rpu.c
  scan.c
  ssautil.c
  stream.c
  sync.c
  taskset.c
  unsharp.c
  vce_common.c
  vfr.c
  work.c
  workpass.c
)

if(ENABLE_QSV)
  target_sources(libhb PRIVATE
    qsv_common.c
  )
endif()

if(APPLE)
  target_sources(libhb PRIVATE
    platform/macosx/cropscale_vt.c
    platform/macosx/cv_utils.c
    platform/macosx/encca_aac.c
    platform/macosx/encvt.c
    platform/macosx/prefilter_vt.c
    platform/macosx/rotate_vt.c
    platform/macosx/vt_common.c
  )
endif()

# External dependencies (required)
set(libhb_pc_deps
  dvdnav
  dvdread
  jansson
  libass
  libavcodec
  libavfilter
  libavformat
  libavutil
  libbluray
  libswresample
  libswscale
  libturbojpeg
  ogg
  SvtAv1Enc
  theoradec
  theoraenc
  vorbis
  vorbisenc
  x264
)

if(ENABLE_LIBDOVI)
  list(APPEND libhb_pc_deps dovi)
endif()

if(ENABLE_FDK_AAC)
  list(APPEND libhb_pc_deps fdk-aac)
endif()

if(ENABLE_QSV)
  list(APPEND libhb_pc_deps vpl)
endif()

if(ENABLE_X265)
  list(APPEND libhb_pc_deps x265)
  if(ENABLE_NUMA)
    list(APPEND libhb_pc_deps numa)
  endif()
endif()

foreach(pc ${libhb_pc_deps})
  pkg_check_modules(${pc} REQUIRED IMPORTED_TARGET ${pc})
  target_link_libraries(libhb PUBLIC PkgConfig::${pc})
endforeach()

find_package(cpuinfo REQUIRED CONFIG)
find_package(Iconv REQUIRED)
target_link_libraries(libhb PRIVATE cpuinfo::cpuinfo Iconv::Iconv)

if(ENABLE_MF)
  list(APPEND libhb_libs mfplat strmiids)
endif()

if(WIN32)
  set(SYS_DEFS SYS_MINGW)
  list(APPEND libhb_libs bcrypt ws2_32 uuid ole32 gdi32 userenv ntdll)
elseif(APPLE)
  set(SYS_DEFS SYS_DARWIN)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
  set(SYS_DEFS SYS_LINUX)
  list(APPEND libhb_libs mvec)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL FreeBSD)
  set(SYS_DEFS SYS_FREEBSD LIBICONV_PLUG)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL NetBSD)
  set(SYS_DEFS SYS_NETBSD LIBICONV_PLUG)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL OpenBSD)
  set(SYS_DEFS SYS_OPENBSD)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL SunOS)
  set(SYS_DEFS SYS_SunOS _POSIX_C_SOURCE=200112L __EXTENSIONS__)
else()
  set(SYS_DEFS SYS_UNKNOWN)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|AMD64)")
  list(APPEND SYS_DEFS ARCH_X86_64 HAVE_MMX)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86|i386)")
  list(APPEND SYS_DEFS ARCH_X86_32 HAVE_MMX)
endif()

if(ENABLE_QSV)
  target_compile_definitions(libhb PRIVATE ONEVPL_EXPERIMENTAL HAVE_THREADS=1)
endif()

target_link_libraries(libhb PRIVATE ${libhb_libs})
target_compile_definitions(libhb PRIVATE __LIBHB__ ${SYS_DEFS})
