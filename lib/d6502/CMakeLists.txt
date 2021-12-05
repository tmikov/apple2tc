# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

add_library(d6502
  d6502.cpp ${A2TC_INC}/d6502.h
  a2symbols.cpp ${A2TC_INC}/a2symbols.h
  )
