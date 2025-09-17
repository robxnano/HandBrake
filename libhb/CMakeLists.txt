# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

configure_file(handbrake/project.h.in handbrake/project.h @ONLY)

if(APPLE)
  # Need to build a static library for the Xcode project
  set(LIBHB_TYPE STATIC)
else()
  set(LIBHB_TYPE OBJECT)
endif()

add_library(libhandbrake ${LIBHB_TYPE})
set_target_properties(libhandbrake PROPERTIES OUTPUT_NAME handbrake)
target_compile_options(libhandbrake PRIVATE -Wall)

if(WIN32)
  add_library(handbrake-dll SHARED)
  set_target_properties(handbrake-dll PROPERTIES LINKER_LANGUAGE CXX)
  target_link_libraries(handbrake-dll PRIVATE libhandbrake)
  target_link_options(handbrake-dll PRIVATE -static -static-libstdc++)
  # Strip the DLL in-place as packages are created from the build directory
  target_link_options(handbrake-dll PRIVATE $<$<CONFIG:RELEASE>:-s>)
  # We need the DLL name to be hb.dll
  set_target_properties(handbrake-dll PROPERTIES PREFIX "")
  set_target_properties(handbrake-dll PROPERTIES OUTPUT_NAME hb)
endif()

target_include_directories(libhandbrake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(LIBHB_HEADERS
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

set(LIBHB_SOURCES
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
  list(APPEND LIBHB_SOURCES qsv_common.c)
endif()

if(APPLE)
  # Copy headers to the build directory for the Xcode project
  foreach(header ${LIBHB_HEADERS})
    add_custom_command(TARGET libhandbrake POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${header} ${CMAKE_CURRENT_BINARY_DIR}/${header}
    )
  endforeach()
  add_subdirectory(platform/macosx)
endif()

target_sources(libhandbrake PRIVATE ${LIBHB_SOURCES} ${LIBHB_HEADERS})

# Public dependencies are the ones used directly by the CLI/GUI
target_add_pkg(libhandbrake dvdnav MODULES dvdnav dvdread PUBLIC)
target_add_pkg(libhandbrake ffmpeg MODULES libavcodec libavfilter libavformat libavutil libswresample libswscale PUBLIC)
target_add_pkg(libhandbrake jansson PUBLIC)
# Private dependencies are only used internally by libhandbrake
target_add_pkg(libhandbrake libass PRIVATE)
target_add_pkg(libhandbrake libbluray PRIVATE)
target_add_pkg(libhandbrake libturbojpeg PRIVATE)
target_add_pkg(libhandbrake ogg PRIVATE)
target_add_pkg(libhandbrake SvtAv1Enc PRIVATE)
target_add_pkg(libhandbrake theoraenc PRIVATE)
target_add_pkg(libhandbrake vorbis MODULES vorbis vorbisenc PRIVATE)
target_add_pkg(libhandbrake x264 PRIVATE)

if(FEATURE_libdovi)
  target_add_pkg(libhandbrake dovi PRIVATE)
endif()

if(FEATURE_fdk_aac)
  target_add_pkg(libhandbrake fdk-aac PRIVATE)
endif()

if(FEATURE_qsv)
  if(CMAKE_SYSTEM_NAME MATCHES "^(Linux|FreeBSD)$")
    target_add_pkg(libhandbrake vpl MODULES vpl libva libva-drm PUBLIC)
  else()
    target_add_pkg(libhandbrake vpl PUBLIC)
  endif()
endif()

if(FEATURE_x265)
  target_add_pkg(libhandbrake x265 PRIVATE)
  if(FEATURE_numa)
    target_add_pkg(libhandbrake numa PRIVATE)
  endif()
endif()

find_package(Iconv REQUIRED)
target_link_libraries(libhandbrake PRIVATE Iconv::Iconv)

if(FEATURE_mf)
  list(APPEND LIBHB_EXTRA_LIBS mfplat strmiids)
endif()

set(LIBHB_DEFS __LIBHB__)

if(WIN32)
  list(APPEND LIBHB_DEFS SYS_MINGW)
  find_library(libgnurx REQUIRED NAMES regex)
  target_link_libraries(libhandbrake PRIVATE ${libgnurx})
  list(APPEND LIBHB_EXTRA_LIBS bcrypt ntdll ole32 userenv uuid ws2_32)
elseif(APPLE)
  set(LIBHB_FRAMEWORKS IOKit CoreServices CoreText CoreGraphics AudioToolbox VideoToolbox CoreMedia CoreVideo IOSurface Foundation DiskArbitration Metal)
  foreach(name ${LIBHB_FRAMEWORKS})
    find_library(framework-${name} NAMES ${name} REQUIRED)
    target_link_libraries(libhandbrake PRIVATE ${framework-${name}})
  endforeach()
  list(APPEND LIBHB_DEFS SYS_DARWIN)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBHB_DEFS SYS_LINUX)
  list(APPEND LIBHB_EXTRA_LIBS mvec)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "FreeBSD")
  list(APPEND LIBHB_DEFS SYS_FREEBSD LIBICONV_PLUG)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "NetBSD")
  list(APPEND LIBHB_DEFS SYS_NETBSD LIBICONV_PLUG)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "OpenBSD")
  list(APPEND LIBHB_DEFS SYS_OPENBSD)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "SunOS")
  list(APPEND LIBHB_DEFS SYS_SunOS _POSIX_C_SOURCE=200112L __EXTENSIONS__)
else()
  list(APPEND LIBHB_DEFS SYS_UNKNOWN)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|AMD64|amd64)")
  list(APPEND LIBHB_DEFS ARCH_X86_64 HAVE_MMX)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86|i.86)")
  list(APPEND LIBHB_DEFS ARCH_X86_32 HAVE_MMX)
endif()

# These need to be public as they're exposed in compat.h
check_function_exists(strerror_r HAS_STRERROR_R)
if(HAS_STRERROR_R)
  target_compile_definitions(libhandbrake PUBLIC HAS_STRERROR_R)
endif()

check_function_exists(strtok_r HAS_STRTOK_R)
if(NOT HAS_STRTOK_R)
  target_compile_definitions(libhandbrake PUBLIC HB_NEED_STRTOK_R)
endif()

if(FEATURE_qsv)
  list(APPEND LIBHB_DEFS ONEVPL_EXPERIMENTAL HAVE_THREADS=1)
endif()

target_compile_definitions(libhandbrake PRIVATE ${LIBHB_DEFS})
target_link_libraries(libhandbrake PRIVATE ${LIBHB_EXTRA_LIBS})
