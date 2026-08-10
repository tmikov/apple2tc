/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// What the interpreter engine offers beyond `a2engine.h`.
///
/// A generated engine has its program fixed at decompile time; this one can be
/// handed a different program while running, which is what a2emu's F1/F2 keys
/// do.

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Load a DOS3.3 binary image into RAM and jump to it.
void engine6502_run_b33(const uint8_t *data, size_t len);

#ifdef __cplusplus
} // extern "C"
#endif
