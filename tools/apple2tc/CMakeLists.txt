# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

include_directories(${APPLE2TC_ROOT_DIR}/external/json/include)

link_libraries(d6502 support)
add_executable(apple2tc
  apple2tc.cpp
  Disas.cpp RuntimeData.cpp Disas.h
  GenIR.cpp GenIR.h PubIR.h
  SSA.cpp dce.cpp simplify.cpp
  routines.cpp
  IRC1.cpp IRC1.h
  PrintSimpleC.cpp
  ir/IR.cpp ir/ValueList.h ir/IR.h ir/IRUtil.cpp ir/IRUtil.h
  ir/IRDump.cpp ir/IRDump.h
  )
