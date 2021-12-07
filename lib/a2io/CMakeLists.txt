# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

add_library(a2io
  a2io.c ${A2TC_INC}/a2io.h
  font.cpp font.h
  )

