# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

configure_file(handbrake/project.h.in handbrake/project.h @ONLY)

if(APPLE)
  # A static library is needed for the Xcode project
  set(libhandbrake_TYPE STATIC)
else()
  set(libhandbrake_TYPE OBJECT)
endif()

add_library(libhandbrake ${libhandbrake_TYPE})
set_target_properties(libhandbrake PROPERTIES OUTPUT_NAME handbrake)
target_compile_options(libhandbrake PRIVATE -Wall)

if(WIN32)
  add_library(handbrake-dll SHARED)
  target_link_options(handbrake-dll PRIVATE -static -static-libstdc++)
  target_link_libraries(handbrake-dll PRIVATE libhandbrake)
  # We need the DLL name to be hb.dll
  set_target_properties(handbrake-dll PROPERTIES PREFIX "")
  set_target_properties(handbrake-dll PROPERTIES OUTPUT_NAME hb)
  set_target_properties(handbrake-dll PROPERTIES LINKER_LANGUAGE CXX)
  install(TARGETS handbrake-dll)
endif()

target_include_directories(libhandbrake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(HB_HEADERS
  handbrake/audio_remap.h
  handbrake/audio_resample.h
  handbrake/av1_common.h
  handbrake/avfilter_priv.h
  handbrake/bitstream.h
  handbrake/colormap.h
  handbrake/common.h
  handbrake/compat.h
  handbrake/decavsub.h
  handbrake/decomb.h
  handbrake/dovi_common.h
  handbrake/dvd.h
  handbrake/eedi2.h
  handbrake/encavsub.h
  handbrake/encx264.h
  handbrake/extradata.h
  handbrake/h264_common.h
  handbrake/h265_common.h
  handbrake/handbrake.h
  handbrake/hbavfilter.h
  handbrake/hb_dict.h
  handbrake/hbffmpeg.h
  handbrake/hb_json.h
  handbrake/hbtypes.h
  handbrake/hdr10plus.h
  handbrake/hwaccel.h
  handbrake/internal.h
  handbrake/lang.h
  handbrake/mf_common.h
  handbrake/nal_units.h
  handbrake/nlmeans.h
  handbrake/nvenc_common.h
  handbrake/param.h
  handbrake/ports.h
  handbrake/preset_builtin.h
  handbrake/preset.h
  handbrake/qsv_common.h
  handbrake/rpu.h
  handbrake/ssautil.h
  handbrake/taskset.h
  handbrake/vce_common.h
)

target_sources(libhandbrake PRIVATE
  ${HB_HEADERS}
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

if(FEATURE_qsv)
  target_sources(libhandbrake PRIVATE
    qsv_common.c
  )
endif()

if(APPLE)
  # Copy headers to the build directory for the Xcode project
  foreach(header ${HB_HEADERS})
    add_custom_command(TARGET libhandbrake POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${header} ${CMAKE_CURRENT_BINARY_DIR}/${header}
    )
  endforeach()
  add_subdirectory(platform/macosx)
endif()

# External dependencies (required)
set(HB_PC_DEPS
  libavcodec
  libavfilter
  libavformat
  libavutil
  libswresample
  libswscale
  SvtAv1Enc
  dvdnav
  dvdread
  jansson
  libass
  libbluray
  libturbojpeg
  ogg
  theoraenc
  vorbis
  vorbisenc
  x264
  libcpuinfo
)

if(FEATURE_libdovi)
  list(APPEND HB_PC_DEPS dovi)
endif()

if(FEATURE_fdk_aac)
  list(APPEND HB_PC_DEPS fdk-aac)
endif()

if(FEATURE_qsv)
  list(APPEND HB_PC_DEPS vpl)
endif()

if(FEATURE_x265)
  list(APPEND HB_PC_DEPS x265)
  if(FEATURE_numa)
    list(APPEND HB_PC_DEPS numa)
  endif()
endif()

foreach(name ${HB_PC_DEPS})
  pkg_check_modules(${name} REQUIRED IMPORTED_TARGET ${name})
  target_link_libraries(libhandbrake PUBLIC PkgConfig::${name})
endforeach()

find_package(Iconv REQUIRED)
target_link_libraries(libhandbrake PRIVATE Iconv::Iconv)

if(FEATURE_mf)
  list(APPEND HB_EXTRA_LIBS mfplat strmiids)
endif()

if(WIN32)
  set(SYS_DEFS SYS_MINGW)
  find_library(libgnurx REQUIRED NAMES regex)
  target_link_libraries(libhandbrake PRIVATE ${libgnurx})
  list(APPEND HB_EXTRA_LIBS bcrypt ntdll ole32 userenv uuid ws2_32)
elseif(APPLE)
  set(HB_FRAMEWORKS IOKit CoreServices CoreText CoreGraphics AudioToolbox VideoToolbox CoreMedia CoreVideo IOSurface Foundation DiskArbitration Metal)
  foreach(name ${HB_FRAMEWORKS})
    find_library(${name}-framework NAMES ${name} REQUIRED)
    target_link_libraries(libhandbrake PRIVATE ${${name}-framework})
  endforeach()
  set(SYS_DEFS SYS_DARWIN)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(SYS_DEFS SYS_LINUX)
  list(APPEND HB_EXTRA_LIBS mvec)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "FreeBSD")
  set(SYS_DEFS SYS_FREEBSD LIBICONV_PLUG)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "NetBSD")
  set(SYS_DEFS SYS_NETBSD LIBICONV_PLUG)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "OpenBSD")
  set(SYS_DEFS SYS_OPENBSD)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "SunOS")
  set(SYS_DEFS SYS_SunOS _POSIX_C_SOURCE=200112L __EXTENSIONS__)
else()
  set(SYS_DEFS SYS_UNKNOWN)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|AMD64|amd64)")
  list(APPEND SYS_DEFS ARCH_X86_64 HAVE_MMX)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86|i.86)")
  list(APPEND SYS_DEFS ARCH_X86_32 HAVE_MMX)
endif()

if(FEATURE_qsv)
  target_compile_definitions(libhandbrake PRIVATE ONEVPL_EXPERIMENTAL HAVE_THREADS=1)
endif()

target_link_libraries(libhandbrake PRIVATE ${HB_EXTRA_LIBS})
target_compile_definitions(libhandbrake PRIVATE __LIBHB__ ${SYS_DEFS})
