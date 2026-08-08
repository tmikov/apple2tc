# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# An a2engine.h engine backed by the Emu6502 interpreter -- the alternative to
# the C that apple2tc generates. Which one a program gets is a link-time choice.
# An OBJECT library, like the other halves of a program. That is what the
# generated engine effectively is -- compiled straight into the executable --
# and it matters here for a second reason: the host/engine contract is two-way,
# so as two archives they would be mutually dependent and a single left-to-right
# link scan could not resolve both. Objects are present before any archive is
# reached, so the cycle never arises.
add_library(engine6502 OBJECT engine6502.cpp)
target_link_libraries(engine6502 PUBLIC a2host cpuemu d6502 a2io support)
