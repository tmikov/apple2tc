# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# The virtual joystick's policy: pure functions over a struct, so this links
# the one translation unit and nothing else. The bindings above it are sokol
# events and have no harness; see joystick_test.c.
add_executable(joystick-test joystick_test.c ../../lib/a2host/joystick.c)
target_link_libraries(joystick-test a2io)
