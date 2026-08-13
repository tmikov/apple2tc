# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

add_library(d6502
  d6502.cpp ${A2TC_INC}/d6502.h opcodes.h
  a2symbols.cpp ${A2TC_INC}/a2symbols.h
  SimpleAsm.cpp ${A2TC_INC}/SimpleAsm.h
  apple2plus_rom.cpp ${A2TC_INC}/apple2plus_rom.h
  )
# d6502.cpp calls into support (PANIC_ASSERT_MSG -> panicAbort()), as
# SimpleAsm.cpp already did (vformat()). Declaring the dependency here, rather
# than leaving every consumer to list "support" alongside "d6502" by
# convention, is what lets CMake place it correctly on its own -- some
# consumers' link lines revisit d6502.a more than once to close a dependency
# cycle with another static library, and a manually-ordered list breaks when
# the revisited copy is the one that first pulls in a support symbol.
target_link_libraries(d6502 support)

add_executable(gen6502 gen.cpp)
target_link_libraries(gen6502 d6502 support)
