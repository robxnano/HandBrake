# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

add_executable(handbrake-cli)
set_target_properties(handbrake-cli PROPERTIES OUTPUT_NAME HandBrakeCLI)

target_sources(handbrake-cli PRIVATE parsecsv.c test.c)
target_link_libraries(handbrake-cli PRIVATE libhb)

install(TARGETS handbrake-cli)
