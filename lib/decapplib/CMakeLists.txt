# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

add_library(decapplib
  decapplib.c ${A2TC_INC}/decapplib.h
  )

target_link_libraries(decapplib sokol a2io)

