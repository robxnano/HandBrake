set(svt_av1_VERSION 2.3.0)
set(svt_av1_SHA256 ebb0b484ef4a0dc281e94342a9f73ad458496f5d3457eca7465bec943910c6c3)
list(APPEND svt_av1_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/SVT-AV1-v${svt_av1_VERSION}.tar.gz
  https://gitlab.com/AOMediaCodec/SVT-AV1/-/archive/v${svt_av1_VERSION}/SVT-AV1-v${svt_av1_VERSION}.tar.gz
)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_APPS OFF)
set(REPRODUCIBLE_BUILDS ON)

CPMAddPackage(
  NAME svt_av1
  VERSION ${svt_av1_VERSION}
  URL ${svt_av1_URL}
  URL_HASH SHA256=${svt_av1_SHA256}
  EXCLUDE_FROM_ALL
)

target_link_libraries(libhb PUBLIC SvtAv1Enc)
