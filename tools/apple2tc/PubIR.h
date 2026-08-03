/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ir {
class IRContext;
class Module;
} // namespace ir

class Disas;
class CPURegLiveness;

std::shared_ptr<ir::IRContext> newIRContext(unsigned verbosity, bool preserveRetAddr);
ir::Module *genIR(const std::shared_ptr<Disas> &disas, ir::IRContext &ctx);
void dumpModule(ir::Module *mod, bool irTrees, CPURegLiveness *liveness);

/// SSA conversion of LoadR8/StoreR8 within individual basic blocks.
bool localCPURegSSA(ir::Module *mod);
bool dce(ir::Module *mod);
bool simplify(ir::Module *mod);
bool simplifyCFG(ir::Module *mod);
/// \param report if non-null, write a human-readable analysis of every routine
///     candidate — accepted and rejected — to this stream.
bool identifySimpleRoutines(ir::Module *mod, FILE *report = nullptr);
/// Replace calls to the specified addresses with calls to bodyless external
/// functions, then delete everything that becomes unreachable.
/// \param externs (address, name) pairs.
bool externRoutines(
    ir::Module *mod,
    const std::vector<std::pair<uint16_t, std::string>> &externs);
std::shared_ptr<CPURegLiveness> liveness(ir::Module *mod);

void printIRC1(ir::Module *mod, FILE *os, bool trees);