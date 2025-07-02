# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

libhb_defs = ['__LIBHB__']
libhb_args = []
libhb_link_args = []

libhb_src = files(
  'audio_remap.c',
  'audio_resample.c',
  'avfilter.c',
  'batch.c',
  'bd.c',
  'bitstream.c',
  'blend.c',
  'chroma_smooth.c',
  'colormap.c',
  'colorspace.c',
  'comb_detect.c',
  'common.c',
  'compat.c',
  'cropscale.c',
  'deblock.c',
  'decavcodec.c',
  'decavsub.c',
  'declpcm.c',
  'decomb.c',
  'decsrtsub.c',
  'decssasub.c',
  'dectx3gsub.c',
  'deinterlace.c',
  'demuxmpeg.c',
  'denoise.c',
  'detelecine.c',
  'dovi_common.c',
  'dvd.c',
  'dvdnav.c',
  'eedi2.c',
  'encavcodecaudio.c',
  'encavcodec.c',
  'encavsub.c',
  'encsvtav1.c',
  'enctheora.c',
  'encvorbis.c',
  'enctx3gsub.c',
  'encx264.c',
  'encx265.c',
  'extradata.c',
  'fifo.c',
  'format.c',
  'grayscale.c',
  'hbavfilter.c',
  'hb.c',
  'hb_dict.c',
  'hbffmpeg.c',
  'hb_json.c',
  'hdr10plus.c',
  'hwaccel.c',
  'lang.c',
  'lapsharp.c',
  'motion_metric.c',
  'mt_frame_filter.c',
  'muxavformat.c',
  'muxcommon.c',
  'nal_units.c',
  'nlmeans.c',
  'nlmeans_x86.c',
  'nvenc_common.c',
  'pad.c',
  'param.c',
  'ports.c',
  'preset.c',
  'reader.c',
  'rendersub.c',
  'rotate.c',
  'rpu.c',
  'scan.c',
  'ssautil.c',
  'stream.c',
  'sync.c',
  'taskset.c',
  'unsharp.c',
  'vce_common.c',
  'vfr.c',
  'work.c',
  'workpass.c',
)

if get_option('qsv').enabled()
  libhb_src += files(
    'qsv_common.c',
  )
endif

if host_machine.system() == 'windows'
  libhb_defs += 'SYS_MINGW'
  if not cc.has_function('pthread_create')
    libhb_defs += 'PTW32_STATIC_LIB'
  endif
  libhb_link_args += cpp.get_supported_link_arguments('-Wl,--enable-auto-import')
elif host_machine.system() == 'darwin'
  libhb_defs += 'SYS_DARWIN'
elif host_machine.system() == 'linux'
  libhb_defs += 'SYS_LINUX'
elif host_machine.system() == 'freebsd'
  libhb_defs += 'SYS_FREEBSD'
elif host_machine.system() == 'netbsd'
  libhb_defs += 'SYS_NETBSD'
elif host_machine.system() == 'openbsd'
  libhb_defs += 'SYS_OPENBSD'
elif host_machine.system() == 'sunos'
  libhb_defs += ['SYS_SunOS', '_POSIX_C_SOURCE=200112L', '__EXTENSIONS__']
else
  libhb_defs += 'SYS_UNKNOWN'
endif

if get_option('qsv').enabled()
  libhb_defs += 'ONEVPL_EXPERIMENTAL'
  libhb_defs += 'HAVE_THREADS=1'
endif

if not cc.has_function('strtok_r')
  libhb_defs += 'HB_NEED_STRTOK_R'
endif

if cc.has_function('strerror_r')
  libhb_defs += 'HAS_STRERROR_R'
endif

foreach d: libhb_defs
  libhb_args += '-D' + d
endforeach

subdir('handbrake')

if host_machine.system() == 'darwin'
  subdir('platform/macosx')
endif

libhb = static_library('handbrake',
  sources: libhb_src,
  dependencies: hb_deps,
  include_directories: hb_incdirs,
  link_language: 'cpp',
  c_args: libhb_args,
  objc_args: libhb_args,
  link_args: libhb_link_args,
  install: false)

if host_machine.system() == 'windows'
  libhb_objs = libhb.extract_all_objects(recursive: true)
  shared_library('hb',
    name_prefix: '',
    objects: libhb_objs,
    dependencies: hb_deps,
    include_directories: hb_incdirs,
    link_language: 'cpp',
    c_args: libhb_args,
    objc_args: libhb_args,
    link_args: libhb_link_args,
    install: true)
endif
