set(libgnurx_VERSION 2.5.1)
set(libgnurx_SHA256 7147b7f806ec3d007843b38e19f42a5b7c65894a57ffc297a76b0dcd5f675d76)
set(libgnurx_URL
  https://github.com/HandBrake/HandBrake-contribs/releases/download/contribs/mingw-libgnurx-2.5.1-src.tar.gz
  https://sourceforge.net/projects/mingw/files/Other/UserContributed/regex/mingw-regex-2.5.1/mingw-libgnurx-2.5.1-src.tar.gz
)

if(WIN32)
  find_library(libgnurx NAMES regex REQUIRED)
  target_link_libraries(libhb PRIVATE ${libgnurx})
endif()
