# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: BSD-3-Clause

include(FindPkgConfig)

#[[
  .. command:: target_add_pkg

  Helper function for linking pkg-config libraries. Wraps calls to
  pkg_check_modules, and links the resulting imported target with
  <target>. If a list of MODULES are not supplied, this function
  will search for <prefix> instead.

  .. code-block:: cmake

  target_add_pkg(<target> <prefix>
                 [QUIET] [GLOBAL]
                 [PUBLIC|PRIVATE]
                 [MODULES <moduleName>...])
#]]
function(target_add_pkg _target _name)
  set(multi_value_args MODULES)
  set(options PRIVATE PUBLIC QUIET GLOBAL)
  cmake_parse_arguments(arg "${options}" "" "${multi_value_args}" ${ARGN})
  if(arg_PUBLIC)
    set(_pkg_visibility PUBLIC)
  else()
    set(_pkg_visibility PRIVATE)
  endif()
  if(arg_QUIET)
    set(_pkg_quiet QUIET)
  endif()
  if(arg_GLOBAL)
    set(_pkg_global GLOBAL)
  endif()
  if(NOT arg_MODULES)
    set(arg_MODULES ${_name})
  endif()
  pkg_check_modules(${_name} REQUIRED IMPORTED_TARGET ${_pkg_quiet} ${_pkg_global} ${arg_MODULES})
  target_link_libraries(${_target} ${_pkg_visibility} PkgConfig::${_name})
endfunction()
