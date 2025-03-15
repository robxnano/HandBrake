vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/sekrit-twc/zimg/archive/refs/tags/release-${VERSION}.tar.gz"
    FILENAME "zimg-${VERSION}.tar.gz"
    SHA512 85467ec9fcf81ea1ae3489b539ce751772a1dab6c6159928b3c5aa9f1cb029f0570b4624a254d4620886f3376fbf80f9bb829a88c3fe543f99f38947951dc500
)

vcpkg_extract_source_archive(
    SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
)

vcpkg_configure_make(
    SOURCE_PATH "${SOURCE_PATH}"
    AUTOCONFIG
    OPTIONS
        ${cppflags}
)
vcpkg_install_make()
vcpkg_copy_pdbs()
vcpkg_fixup_pkgconfig()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/COPYING")