# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

link_libraries(decapplib)

add_executable(robotron
  robotron.c ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  )
add_executable(robotronc1
  robotronc1.c ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  )
