vcpkg_download_distfile(ARCHIVE
    URLS "https://download.videolan.org/pub/videolan/libbluray/${VERSION}/libbluray-${VERSION}.tar.bz2"
    FILENAME "libbluray-${VERSION}.tar.bz2"
    SHA512 94dbf3b68d1c23fe4648c153cc2f0c251886fac0a6b6bbe3a77caabaa5322682f712afe4a7b6b16ca3f06744fbc0e1ca872209a32898dcf0ae182055d335aec1
)

vcpkg_extract_source_archive(
    SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
)

if(NOT "fontconfig" IN_LIST FEATURES)
  list(APPEND OPTIONS --without-fontconfig)
endif()
if(NOT "freetype" IN_LIST FEATURES)
  list(APPEND OPTIONS --without-freetype)
endif()
if(NOT "libxml2" IN_LIST FEATURES)
  list(APPEND OPTIONS --without-libxml2)
endif()

vcpkg_configure_make(
    SOURCE_PATH "${SOURCE_PATH}"
    AUTOCONFIG
    OPTIONS
        ${OPTIONS}
        "--disable-doxygen-doc"
        "--disable-examples"
        "--disable-bdjava-jar"
        ${cppflags}
)
vcpkg_install_make()
vcpkg_copy_pdbs()
vcpkg_fixup_pkgconfig()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/COPYING")