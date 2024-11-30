set(jansson_VERSION 2.14)
set(jansson_SHA256 fba956f27c6ae56ce6dfd52fbf9d20254aad42821f74fa52f83957625294afb9)
set(jansson_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/jansson-${jansson_VERSION}.tar.bz2
  https://github.com/akheron/jansson/releases/download/v${jansson_VERSION}/jansson-${jansson_VERSION}.tar.bz2
)

pkg_check_modules(jansson REQUIRED IMPORTED_TARGET jansson)

target_link_libraries(libhb PUBLIC PkgConfig::jansson)
