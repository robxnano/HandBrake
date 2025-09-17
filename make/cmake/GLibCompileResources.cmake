# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: BSD-3-Clause

#[[
  .. command:: glib_compile_resources

  Compiles a GResource XML file in order to embed data into an executable or
  library target. Automatically sets up dependencies on the input files listed
  in the XML file. If the embedded data is generated at compile time, you need
  to specify the SOURCE_DIRS where the generated files are located.

  .. code-block:: cmake

  glib_compile_resources(<name> XML_FILE <xmlFile>
                        [C_NAME <cName>|GRESOURCE]
                        [TARGET <target>]
                        [SOURCE_DIRS <sourceDir>...])
#]]
function(glib_compile_resources _name)
  find_program(GCR REQUIRED NAMES glib-compile-resources)

  set(options GRESOURCE)
  set(one_value_args C_NAME XML_FILE TARGET)
  set(multi_value_args SOURCE_DIRS)
  cmake_parse_arguments(arg "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT arg_GRESOURCE)
    if(NOT arg_XML_FILE)
      message(FATAL_ERROR "XML_FILE is required.")
    endif()
    if(NOT arg_C_NAME)
      message(FATAL_ERROR "C_NAME is required unless GRESOURCE is used.")
    endif()
    if(NOT arg_SOURCE_DIRS)
      get_filename_component(arg_SOURCE_DIRS ${arg_XML_FILE} DIRECTORY)
    endif()
    foreach(dir ${arg_SOURCE_DIRS})
      list(APPEND _source_dirs --sourcedir=${dir})
    endforeach()
    add_custom_command(OUTPUT ${_name}.c ${_name}.h
      COMMAND ${GCR} --generate-source --target=${_name}.c --c-name=${arg_C_NAME} ${_source_dirs} --dependency-file=${_name}.deps ${arg_XML_FILE}
      COMMAND ${GCR} --generate-header --target=${_name}.h --c-name=${arg_C_NAME} ${_source_dirs} ${arg_XML_FILE}
      DEPFILE ${_name}.deps
    )
    if(arg_TARGET)
      add_custom_target(glib_resource_${_name} DEPENDS ${_name}.c ${_name}.h)
      add_dependencies(${arg_TARGET} glib_resource_${_name})
      target_sources(${arg_TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${_name}.c ${CMAKE_CURRENT_BINARY_DIR}/${_name}.h)
    endif()
  endif()
endfunction()
