# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# The host proper: no sokol, so a console front end links a graphics stack
# not at all. See include/apple2tc/a2host_api.h.
add_library(a2host
  a2host.c ${A2TC_INC}/a2host_api.h
  ${A2TC_INC}/a2engine.h ${A2TC_INC}/a2host.h ${A2TC_INC}/system.h
  )
target_link_libraries(a2host a2io)

# The windowed front end.
#
# An OBJECT library, not a static one. sokol supplies main() and calls
# sokol_main(), which lives here -- so nothing in the executable's own objects
# references this translation unit, and a static archive would never be pulled
# in. Objects are always linked.
add_library(a2host_gui OBJECT a2host_gui.c)
target_link_libraries(a2host_gui PUBLIC a2host sokol a2io)

# The console front end. An OBJECT library for the same reason as a2host_gui:
# nothing in a program's own objects references main(), so a static archive
# would never be pulled in.
add_library(a2host_console OBJECT console_main.c)
target_link_libraries(a2host_console PUBLIC a2host)
