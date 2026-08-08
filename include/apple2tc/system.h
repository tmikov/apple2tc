/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Umbrella over both halves of the Apple II host/engine contract, for code
/// that is one of the two and wants the whole picture.
///
///   - `a2engine.h` — what an engine provides, and the host calls.
///   - `a2host.h`   — what the host provides, and an engine calls.
///
/// In practice most participants are one half and call the other, so they need
/// both and this is the right include. The split is documentation of
/// *direction*, not enforcement: it records which side owns each symbol, which
/// is what was previously impossible to tell by reading the header.

#pragma once

#include "apple2tc/a2engine.h"
#include "apple2tc/a2host.h"
