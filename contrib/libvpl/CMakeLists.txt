set(vpl_VERSION 2.13.0)
set(vpl_PKGCONFIG_VERSION 2.13)
set(vpl_SHA256 1c740e2b58f7853f56b618bdb7d4a7e5d37f8c1a9b30105a0b79ba80873e1cbd)
set(vpl_URL
  https://github.com/intel/libvpl/archive/refs/tags/v${vpl_VERSION}.tar.gz
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/libvpl-${vpl_VERSION}.tar.gz
)

pkg_check_modules(vpl REQUIRED IMPORTED_TARGET "vpl >= ${vpl_PKGCONFIG_VERSION}")

target_link_libraries(libhb PUBLIC PkgConfig::vpl)
