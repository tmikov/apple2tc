# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# The soft switch test: a2io alone, no CPU and no window.
#
# See a2io_test.c for the bug that had gone unnoticed for five years because
# nothing tested a2io directly.
add_executable(a2io-test a2io_test.c)
target_link_libraries(a2io-test a2io)
