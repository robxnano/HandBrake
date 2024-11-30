set(libass_VERSION 0.17.3)
set(libass_SHA256 da7c348deb6fa6c24507afab2dee7545ba5dd5bbf90a137bfe9e738f7df68537)
set(libass_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/libass-${libass_VERSION}.tar.gz
  https://github.com/libass/libass/releases/download/${libass_VERSION}/libass-${libass_VERSION}.tar.gz
)

pkg_check_modules(libass REQUIRED IMPORTED_TARGET "libass >= ${libass_VERSION}")

target_link_libraries(libhb PUBLIC PkgConfig::libass)
