/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <memory>

namespace ir {
class IRContext;
class Module;
} // namespace ir

class Disas;

std::shared_ptr<ir::IRContext> newIRContext();
ir::Module *genIR(Disas &disas, ir::IRContext &ctx);
void dumpModule(ir::Module *mod);

bool mem2reg(ir::Module *mod);
bool dce(ir::Module *mod);
bool simplify(ir::Module *mod);