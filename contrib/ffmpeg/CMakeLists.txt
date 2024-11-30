set(ffmpeg_VERSION 7.1)
set(ffmpeg_SHA256 fd59e6160476095082e94150ada5a6032d7dcc282fe38ce682a00c18e7820528)
set(ffmpeg_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/ffmpeg-${ffmpeg_VERSION}.tar.bz2
  https://ffmpeg.org/releases/ffmpeg-${ffmpeg_VERSION}.tar.bz2
)

pkg_check_modules(ffmpeg REQUIRED IMPORTED_TARGET libavcodec libavfilter libavformat libswscale libavutil libswresample)

target_link_libraries(libhb PUBLIC PkgConfig::ffmpeg)
