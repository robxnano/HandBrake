set(zimg_VERSION 3.0.5)
set(zimg_SHA256 a9a0226bf85e0d83c41a8ebe4e3e690e1348682f6a2a7838f1b8cbff1b799bcf)
set(zimg_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/zimg-${zimg_VERSION}.tar.gz
  https://github.com/sekrit-twc/zimg/archive/refs/tags/release-${zimg_VERSION}.tar.gz
)

pkg_check_modules(zimg REQUIRED IMPORTED_TARGET "zimg >= ${zimg_VERSION}")

target_link_libraries(libhb PUBLIC PkgConfig::zimg)
