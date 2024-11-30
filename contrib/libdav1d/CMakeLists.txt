set(dav1d_VERSION 1.5.0)
set(dav1d_SHA256 a6ca64e34cec56ae1c2d359e1da5c5386ecd7a3a62f931d026ac4f2ff72ade64)
set(dav1d_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/dav1d-${dav1d_VERSION}.tar.bz2
  https://code.videolan.org/videolan/dav1d/-/archive/${dav1d_VERSION}/dav1d-${dav1d_VERSION}.tar.bz2
)

pkg_check_modules(dav1d REQUIRED IMPORTED_TARGET "dav1d >= ${dav1d_VERSION}")
if(NOT dav1d_FOUND)
  ExternalProject_Add(dav1d
    URL ${dav1d_URL}
    URL_HASH SHA256=${dav1d_SHA256}
  )
endif()

target_link_libraries(libhb PUBLIC PkgConfig::dav1d)
