# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_a2_program(rom
  ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  rom.c
  )
add_a2_program(romc1
  ${A2TC_INC}/system.h ${A2TC_INC}/system2-inc.h
  romc1.c
  )

if (EMSCRIPTEN)
  target_link_options(rom PRIVATE --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html)
endif ()
