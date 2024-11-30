set(dovi_VERSION 3.3.1)
set(dovi_SHA256 4cd7a4c418fd8af1da13278ce7524c15b7fdf61e1fe53663aa291c68c5062777)
set(dovi_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/dovi_tool-libdovi-${dovi_VERSION}.tar.gz
  https://github.com/quietvoid/dovi_tool/archive/refs/tags/libdovi-${dovi_VERSION}.tar.gz
)

pkg_check_modules(dovi REQUIRED IMPORTED_TARGET "dovi >= ${dovi_VERSION}")

target_link_libraries(libhb PUBLIC PkgConfig::dovi)
