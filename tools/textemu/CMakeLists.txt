# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# cpuemu already carries d6502, which already carries support. Naming them
# ahead of cpuemu would only make the linker see them twice.
link_libraries(cpuemu a2io)
add_executable(textemu textemu.cpp)
target_link_libraries(textemu ${CURSES_LIBRARIES})

add_executable(disk_test disk_test.cpp)

