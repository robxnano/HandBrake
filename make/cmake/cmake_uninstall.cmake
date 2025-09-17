if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: ${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
endif()

file(READ "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  if(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    message(STATUS "Uninstalling: $ENV{DESTDIR}${file}")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E rm "$ENV{DESTDIR}${file}"
      OUTPUT_VARIABLE rm_out
      ERROR_VARIABLE  rm_err
      RESULT_VARIABLE rm_retval
    )
    if(NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "${rm_err}")
    endif()
  else()
    message(STATUS "File does not exist: $ENV{DESTDIR}${file}")
  endif()
endforeach()
