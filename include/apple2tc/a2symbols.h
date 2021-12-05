/*
* Copyright (c) Tzvetan Mikov.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "apple2tc/d6502.h"

/// Find an Apple2 symbol by offset or return nullptr.
const char *findApple2Symbol(unsigned offset);

/// Resolve an operand of an instruction with an Apple2 symbol.
std::string apple2SymbolResolver(CPUInst inst);