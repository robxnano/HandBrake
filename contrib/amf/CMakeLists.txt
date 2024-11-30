set(amf_VERSION 2.3.0)
set(amf_SHA256 afad0b15b09ebc80554ecd16aabbbc2f5fef76ae2983062d4cf6740d4242c7f6)
list(APPEND amf_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs2/AMF-1.4.35-slim.tar.gz
)

if(ENABLE_VCN)
  CPMAddPackage(NAME amf
    VERSION ${amf_VERSION}
    URL ${amf_URL}
    URL_HASH SHA256=${amf_SHA256}
  )
endif()
