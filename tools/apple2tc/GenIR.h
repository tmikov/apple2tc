/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "Disas.h"
#include "ir/IR.h"

#include <unordered_map>

using namespace ir;

class GenIR {
  Disas *const dis_;
  const RuntimeData *const runData_;
  const RuntimeData::BaseStats * const baseStats_;
  IRContext *const ctx_;
  IRBuilder builder_;

  /// Current PC.
  uint16_t pc_ = 0;
  /// Whether the current operand supports self modification.
  bool selfModOperand_ = false;

  /// Map from an AsmBlock to an IR BasicBlock.
  std::unordered_map<const AsmBlock *, BasicBlock *> asmBlockMap_{};

  /// A basic block generated on demand for the BRK instruction, if encountered.
  /// It contains a PHP and a jmp to IRQ_VEC.
  BasicBlock *brkBB_ = nullptr;

public:
  GenIR(Disas *disas, IRContext *ctx);
  ~GenIR();

  Module *run();

private:
  void genAsmBlock(const AsmBlock &asmBlock);
  void genInst(uint16_t pc, const CPUInst &inst, const AsmBlock &asmBlock);

  BasicBlock *createBB(uint32_t addr);
  BasicBlock *createAbortBlock(uint16_t target, uint8_t reason);
  BasicBlock *basicBlockFor(const AsmBlock *asmBlock);
  BasicBlock *resolveBranch(uint16_t addr);
  BasicBlock *resolveFallBranch(const AsmBlock &curAsmBlock);

  Value *emitUpdateNZ(Value *value8);
  Value *emitUpdateNZC(Value *value16);

  Value *emitEncodeFlags();
  void emitDecodeFlags(Value *value8);
  void emitADC(const CPUInst &inst);
  void emitSBC(const CPUInst &inst);
  Value *emitLoadReg8(CPURegKind cpuReg);
  void emitStoreReg8(CPURegKind cpuReg, Value *value);
  void emitCmpCPUReg(const CPUInst &inst, CPURegKind cpuReg);
  void emitJCond(bool jTrue, const CPUInst &inst, Value *cond, const AsmBlock &asmBlock);
  void emitWrite(const CPUInst &inst, Value *addr, Value *value);
  Value *genRead(const CPUInst &inst);
  Value *genAddr(const CPUInst &inst);

  /// Load from non-IO.
  Value *emitRamPeek16(uint16_t addr);
  Value *emitAutoPeek16(uint16_t addr);
  Value *emitAutoPeek16(Value *addr);
  Value *emitPeek16_zpg(uint8_t addr);
  Value *emitPeek16_zpg(Value *addr);
  Value *emitAutoPeek8(Value *addr);
  void emitAutoPoke8(Value *addr, Value *value);

  /// Check if the specified address with specified width is guaranteed to be
  /// outside of the Apple2 IO range
  ///
  /// \param width offset to subtract from the IO range (to account for 16-bit
  ///     loads).
  /// \return true if the specified 16-bit address is guaranteed to be less than
  ///     A2_IO_RANGE_START.
  static bool isAddrNonIO(Value *addr, unsigned width = 1);
};