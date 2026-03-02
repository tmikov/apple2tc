# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

link_libraries(d6502 cpuemu a2io support)
add_executable(textemu textemu.cpp)
target_link_libraries(textemu ${CURSES_LIBRARIES})
