# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

link_libraries(a2host a2host_gui)

add_executable(snake-byte
  snake-byte.c ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  )

# The reference build. snake-bytec1.c is generated with the Apple II ROM
# decompiled alongside the game, so it is self-contained and links on its own.
# play.frames was recorded from this, and it is the control that snake-bytec1-ext
# is checked against. Do not make this one depend on hand-written code.
add_executable(snake-bytec1
  snake-bytec1.c ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  )

# The extern build: the ROM entry points listed in rom.externs are supplied by
# the hand-written a2rom.c instead of being decompiled. snake-byte-ext.c is the
# only file compiled here -- it #includes both snake-bytec1-ext.c (generated) and
# a2rom.c (hand-written), which must share a translation unit because
# system2-inc.h defines the machine state with internal linkage. See the comment
# in snake-byte-ext.c. The other sources are listed so they show up in IDEs; they
# must never be compiled separately, hence HEADER_FILE_ONLY -- doing so would
# give each object file its own private copy of s_ram and still link cleanly.
add_executable(snake-bytec1-ext
  snake-byte-ext.c
  snake-bytec1-ext.c a2rom.c a2rom.h game.c game.h
  ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  )
set_source_files_properties(snake-bytec1-ext.c a2rom.c game.c
  PROPERTIES HEADER_FILE_ONLY TRUE)
