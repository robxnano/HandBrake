# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: BSD-3-Clause

function(get_version_info _source_dir)
  find_package(Git)
  if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --exact-match HEAD
      WORKING_DIRECTORY ${_source_dir}
      OUTPUT_VARIABLE _release_tag
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

    execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --date=format:%Y-%m-%d --format=%cd
      WORKING_DIRECTORY ${_source_dir}
      OUTPUT_VARIABLE _release_date
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

    if(_release_date AND NOT _release_tag)
      execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${_source_dir}
        OUTPUT_VARIABLE _git_rev
        OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

      string(REPLACE "-" "" _date_short ${_release_date})
      set(_release_tag "${_date_short}-git-${_git_rev}")
    endif()
  endif()

  if(NOT _release_tag)
    file(STRINGS ${_source_dir}/version.txt _version_txt)
    foreach(var ${_version_txt})
      string(REPLACE "=" ";" var ${var})
      list(GET var 0 _var_name)
      list(GET var 1 _var_value)
      set(_txt_${_var_name} ${_var_value})
    endforeach()
    if(_txt_DATE)
      string(REPLACE " " ";" _date_list ${_txt_DATE})
      list(GET _date_list 0 _release_date)
    endif()
    if(_txt_TAG)
      set(_release_tag ${_txt_TAG})
    elseif(_txt_SHORTHASH)
      string(REPLACE "-" "" _date_short ${_txt_DATE})
      set(_release_tag ${_date_short}-git-${_txt_SHORTHASH})
    endif()
  endif()

  if(NOT _release_date)
    set(_release_date 9999-12-31)
  endif()
  if(NOT _release_tag)
    string(REPLACE "-" "" _date_short ${_release_date})
    set(_release_tag ${_date_short}-unknown)
  endif()

  set(RELEASE_DATE ${_release_date} CACHE INTERNAL "The build release date" FORCE)
  set(RELEASE_TAG ${_release_tag} CACHE INTERNAL "The build release tag" FORCE)
endfunction()
