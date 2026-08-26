# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# The wall-clock pacing test: a2host, a stub engine, and no window.
#
# a2host is deliberately sokol-free, which is what makes this possible at all.
# See pacing_test.c for why the mode it checks had no coverage.
add_executable(pacing-test pacing_test.c stub_engine.c)
target_link_libraries(pacing-test a2host)
