# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

link_libraries(d6502 cpuemu a2io support sokol)

add_executable(a2emu a2emu.cpp
  bolo.h robotron2084.h
  apple2plus_rom.cpp apple2plus_rom.h
  )

if (EMSCRIPTEN)
  target_link_options(a2emu PRIVATE --shell-file ${APPLE2TC_ROOT_DIR}/lib/sokol/shell.html)
endif ()
