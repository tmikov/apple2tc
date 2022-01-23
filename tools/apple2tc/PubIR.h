/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdio>
#include <memory>

namespace ir {
class IRContext;
class Module;
} // namespace ir

class Disas;

std::shared_ptr<ir::IRContext> newIRContext();
ir::Module *genIR(const std::shared_ptr<Disas> &disas, ir::IRContext &ctx);
void dumpModule(ir::Module *mod, bool irTrees);

/// SSA conversion of LoadR8/StoreR8 within individual basic blocks.
bool localCPURegSSA(ir::Module *mod);
bool dce(ir::Module *mod);
bool simplify(ir::Module *mod);

void printIRC1(ir::Module *mod, FILE *os, bool trees);