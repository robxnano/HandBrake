set(dvdread_VERSION 6.1.3)
set(dvdread_SHA256 ce35454997a208cbe50e91232f0e73fb1ac3471965813a13b8730a8f18a15369)
set(dvdread_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/libdvdread-${dvdread_VERSION}.tar.bz2
  https://download.videolan.org/pub/videolan/libdvdread/${dvdread_VERSION}/libdvdread-${dvdread_VERSION}.tar.bz2
)

pkg_check_modules(dvdread REQUIRED IMPORTED_TARGET "dvdread >= ${dvdread_VERSION}")

target_link_libraries(libhb PUBLIC PkgConfig::dvdread)
