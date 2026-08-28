/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The emulated 6502, declarations and definitions together: the
/// single-translation-unit form, for a program that is one file and wants to
/// stay that way.
///
/// The two lines below are the whole of it, and they are also exactly what a
/// program writes into its own .c when it wants more than one translation unit
/// -- every other unit then includes only `system2.h`. Do both and the
/// definitions appear twice; the linker says so.

#pragma once

#include "apple2tc/system2.h"
#include "apple2tc/system2-impl.inc"
