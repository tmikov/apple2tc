# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

link_libraries(decapplib)

add_executable(rom
  ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  rom.c
  )

if (EMSCRIPTEN)
  target_link_options(rom PRIVATE --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html)
endif ()
