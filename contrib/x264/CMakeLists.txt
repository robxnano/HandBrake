set(x264_VERSION 20240314-3186)
set(x264_SHA256 fd59e6160476095082e94150ada5a6032d7dcc282fe38ce682a00c18e7820528)
set(x264_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/x264-snapshot-${x264_VERSION}.tar.gz
)

pkg_check_modules(x264 REQUIRED IMPORTED_TARGET x264)

target_link_libraries(libhb PUBLIC PkgConfig::x264)
