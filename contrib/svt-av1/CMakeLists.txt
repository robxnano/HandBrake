set(svt_av1_VERSION 2.3.0)
set(svt_av1_SHA256 ebb0b484ef4a0dc281e94342a9f73ad458496f5d3457eca7465bec943910c6c3)
list(APPEND svt_av1_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/SVT-AV1-v${svt_av1_VERSION}.tar.gz
  https://gitlab.com/AOMediaCodec/SVT-AV1/-/archive/v${svt_av1_VERSION}/SVT-AV1-v${svt_av1_VERSION}.tar.gz
)
list(APPEND svt_av1_OPTIONS
  "BUILD_SHARED_LIBS OFF"
  "BUILD_APPS OFF"
  "REPRODUCIBLE_BUILDS ON"
)

pkg_check_modules(SvtAv1Enc REQUIRED IMPORTED_TARGET "SvtAv1Enc >= ${svt_av1_VERSION}")

if(SvtAv1Enc_FOUND)
  target_link_libraries(libhb PUBLIC PkgConfig::SvtAv1Enc)
else()
  CPMAddPackage(NAME svt_av1
    VERSION ${svt_av1_VERSION}
    URL ${svt_av1_URL}
    URL_HASH SHA256=${svt_av1_SHA256}
    OPTIONS ${svt_av1_OPTIONS}
  )
endif()

