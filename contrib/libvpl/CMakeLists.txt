set(vpl_VERSION 2.13.0)
set(vpl_SHA256 1c740e2b58f7853f56b618bdb7d4a7e5d37f8c1a9b30105a0b79ba80873e1cbd)
set(vpl_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/libvpl-${vpl_VERSION}.tar.gz
  https://github.com/intel/libvpl/archive/refs/tags/v${vpl_VERSION}.tar.gz
)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_EXPERIMENTAL ON)
set(BUILD_TOOLS OFF)
set(DBUILD_EXAMPLES OFF)

CPMAddPackage(
  NAME vpl
  VERSION ${vpl_VERSION}
  URL ${vpl_URL}
  URL_HASH SHA256=${vpl_SHA256}
  EXCLUDE_FROM_ALL
)

target_link_libraries(libhb PUBLIC vpl-api)
