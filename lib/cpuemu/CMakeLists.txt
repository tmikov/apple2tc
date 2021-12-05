# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

add_library(cpuemu
  emu6502.cpp ${A2TC_INC}/emu6502.h
  apple2.cpp applesoft.cpp ${A2TC_INC}/apple2.h
  )

