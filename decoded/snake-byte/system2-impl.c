/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The emulated machine, compiled once for this program.
///
/// snake-byte.c includes only `system2.h` -- the declarations -- so it is no
/// longer the whole program by construction. Anything else that needs the
/// machine includes that same header and links against this.

#include "apple2tc/system2.h"
#include "apple2tc/system2-impl.inc"
