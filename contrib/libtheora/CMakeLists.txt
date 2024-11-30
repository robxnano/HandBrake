set(theora_VERSION 1.1.1)
set(theora_SHA256 b6ae1ee2fa3d42ac489287d3ec34c5885730b1296f0801ae577a35193d3affbc)
set(theora_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/libtheora-${theora_VERSION}.tar.bz2
  https://downloads.xiph.org/releases/theora/libtheora-${theora_VERSION}.tar.bz2
)

pkg_check_modules(theora REQUIRED IMPORTED_TARGET theora theoraenc)
if(NOT theora_FOUND)
  ExternalProject_Add(theora
    URL ${theora_URL}
    URL_HASH SHA256=${theora_SHA256}
  )
endif()

target_link_libraries(libhb PUBLIC PkgConfig::theora)
