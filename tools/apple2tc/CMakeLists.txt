# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

link_libraries(d6502)
add_executable(apple2tc
  apple2tc.cpp
  Disas.cpp Disas.h
  PrintSimpleC.cpp
  )
