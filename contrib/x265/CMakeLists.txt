set(x265_VERSION 4.1)
set(x265_SHA256 ef05940bdbf8e2f24c93d799ae8c5d19f43cd235d01c17fa5acf3fa77b48c032)
set(x265_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/x265_${x265_VERSION}.tar.gz
  https://bitbucket.org/multicoreware/x265_git/downloads/x265_${x265_VERSION}.tar.gz
)

pkg_check_modules(x265 REQUIRED IMPORTED_TARGET "x265 >= ${x265_VERSION}")

target_link_libraries(libhb PUBLIC PkgConfig::x265)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  pkg_check_modules(numa REQUIRED IMPORTED_TARGET numa)
  target_link_libraries(libhb PUBLIC PkgConfig::numa)
endif()
