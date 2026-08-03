# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

link_libraries(decapplib)

add_executable(snake-byte
  snake-byte.c ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  )
# snake-byte-ext.c is the only file compiled here: it #includes both
# snake-bytec1.c (generated) and a2rom.c (hand-written ROM entry points), which
# must share a translation unit. See the comment in snake-byte-ext.c. The other
# sources are listed so they show up in IDEs; they must not be compiled
# separately, hence HEADER_FILE_ONLY.
add_executable(snake-bytec1
  snake-byte-ext.c
  snake-bytec1.c a2rom.c a2rom.h
  ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  )
set_source_files_properties(snake-bytec1.c a2rom.c PROPERTIES HEADER_FILE_ONLY TRUE)
