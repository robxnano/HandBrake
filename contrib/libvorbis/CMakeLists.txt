set(vorbis_VERSION 1.3.7)
set(vorbis_SHA256 0e982409a9c3fc82ee06e08205b1355e5c6aa4c36bca58146ef399621b0ce5ab)
set(vorbis_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/libvorbis-${vorbis_VERSION}.tar.gz
  https://downloads.xiph.org/releases/vorbis/libvorbis-${vorbis_VERSION}.tar.gz
)

pkg_check_modules(vorbis REQUIRED IMPORTED_TARGET vorbis vorbisenc)
if(NOT vorbis_FOUND)
  ExternalProject_Add(vorbis
    URL ${vorbis_URL}
    URL_HASH SHA256=${vorbis_SHA256}
  )
endif()

target_link_libraries(libhb PUBLIC PkgConfig::vorbis)
