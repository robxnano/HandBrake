set(libturbojpeg_VERSION 3.0.4)
set(libturbojpeg_SHA256 0e982409a9c3fc82ee06e08205b1355e5c6aa4c36bca58146ef399621b0ce5ab)
set(libturbojpeg_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/libjpeg-turbo-${libturbojpeg_VERSION}.tar.gz
  https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/${libturbojpeg_VERSION}.tar.gz
)

pkg_check_modules(libturbojpeg REQUIRED IMPORTED_TARGET libturbojpeg)
if(NOT libturbojpeg_FOUND)
  ExternalProject_Add(libturbojpeg
    URL ${libturbojpeg_URL}
    URL_HASH SHA256=${libturbojpeg_SHA256}
  )
endif()

target_link_libraries(libhb PUBLIC PkgConfig::libturbojpeg)
