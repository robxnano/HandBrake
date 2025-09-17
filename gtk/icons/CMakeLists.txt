# Copyright (C) 2025 HandBrake Team
# SPDX-License-Identifier: GPL-2.0-or-later

foreach(size 32x32 128x128 256x256)
  install(FILES ${size}/apps/fr.handbrake.ghb.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${size}/apps)
endforeach()

install(FILES scalable/apps/fr.handbrake.ghb.svg
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
