# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# The console emulator: a2host plus the interpreter engine, no window.
#
# Its counterpart is a2emu, the GUI one. Two executables rather than a flag,
# because on Windows console versus GUI is a link-time subsystem property. All
# the shared behaviour lives in lib/a2host, so this target has no sources of
# its own beyond the console main().
add_executable(a2run
  $<TARGET_OBJECTS:a2host_console> $<TARGET_OBJECTS:engine6502>)
target_link_libraries(a2run a2host engine6502)
