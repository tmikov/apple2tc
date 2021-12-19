# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

link_libraries(a2io)

add_executable(bolo
  main.c ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  bolo.c)
