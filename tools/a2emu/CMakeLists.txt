# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

# The windowed emulator: the same host and engine as a2run, plus a window.
# a2emu.cpp itself is only the F1/F2 keys and the debugger helpers.
add_executable(a2emu a2emu.cpp bolo.h robotron2084.h)
# Just the two front halves: a2host_gui already carries a2host and sokol,
# engine6502 already carries cpuemu, d6502 and support.
target_link_libraries(a2emu a2host_gui engine6502)

if (EMSCRIPTEN)
  target_link_options(a2emu PRIVATE --shell-file ${APPLE2TC_ROOT_DIR}/lib/sokol/shell.html)
endif ()
