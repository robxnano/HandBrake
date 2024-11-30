set(dvdnav_VERSION 6.1.1)
set(dvdnav_SHA256 c191a7475947d323ff7680cf92c0fb1be8237701885f37656c64d04e98d18d48)
set(dvdnav_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/libdvdnav-${dvdnav_VERSION}.tar.bz2
  https://download.videolan.org/pub/videolan/libdvdnav/${dvdnav_VERSION}/libdvdnav-${dvdnav_VERSION}.tar.bz2
)

pkg_check_modules(dvdnav REQUIRED IMPORTED_TARGET "dvdnav >= ${dvdnav_VERSION}")

target_link_libraries(libhb PUBLIC PkgConfig::dvdnav)
