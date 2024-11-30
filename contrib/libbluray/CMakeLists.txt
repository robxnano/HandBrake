set(libbluray_VERSION 1.3.4)
set(libbluray_SHA256 478ffd68a0f5dde8ef6ca989b7f035b5a0a22c599142e5cd3ff7b03bbebe5f2b)
set(libbluray_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/libbluray-${libbluray_VERSION}.tar.bz2
  https://download.videolan.org/pub/videolan/libbluray/${libbluray_VERSION}/libbluray-${libbluray_VERSION}.tar.bz2
)

pkg_check_modules(libbluray REQUIRED IMPORTED_TARGET "libbluray >= ${libbluray_VERSION}")

target_link_libraries(libhb PUBLIC PkgConfig::libbluray)
