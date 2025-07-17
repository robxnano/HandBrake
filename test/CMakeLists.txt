# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

add_executable(handbrake-cli parsecsv.c test.c)
target_link_libraries(handbrake-cli PRIVATE libhandbrake)
target_compile_options(handbrake-cli PRIVATE -Wall)
if(WIN32)
  target_link_options(handbrake-cli PRIVATE -static -static-libstdc++)
  target_link_options(handbrake-cli PRIVATE $<$<CONFIG:RELEASE>:-s>)
endif()
set_target_properties(handbrake-cli PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(handbrake-cli PROPERTIES OUTPUT_NAME HandBrakeCLI)

install(TARGETS handbrake-cli)
