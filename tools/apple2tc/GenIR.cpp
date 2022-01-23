/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "GenIR.h"

#include "PubIR.h"
#include "ir/IRDump.h"

#include "apple2tc/apple2iodefs.h"

#include <cmath>

using namespace ir;

GenIR::GenIR(const std::shared_ptr<Disas> &disas, IRContext *ctx)
    : dis_(disas),
      runData_(disas->getRunData()),
      baseStats_(runData_ ? runData_->baseStats.get() : nullptr),
      ctx_(ctx),
      builder_(ctx) {}

GenIR::~GenIR() = default;

Module *GenIR::run() {
  Module *mod = ctx_->createModule(dis_);
  Function *func = mod->createFunction();

  builder_.setInsertionBlock(func->createBasicBlock());

  auto initRegs = [this](const Regs &regs) {
    emitStoreReg8(CPURegKind::A, builder_.getLiteralU8(regs.a));
    emitStoreReg8(CPURegKind::X, builder_.getLiteralU8(regs.x));
    emitStoreReg8(CPURegKind::Y, builder_.getLiteralU8(regs.y));
    emitStoreReg8(CPURegKind::SP, builder_.getLiteralU8(regs.sp));

    emitStoreReg8(
        CPURegKind::STATUS_N, builder_.getLiteralU8((regs.status & Regs::STATUS_N) ? 0x80 : 0));
    emitStoreReg8(
        CPURegKind::STATUS_V, builder_.getLiteralU8((regs.status & Regs::STATUS_V) ? 0x80 : 0));
    emitStoreReg8(CPURegKind::STATUS_B, builder_.getLiteralU8((regs.status & Regs::STATUS_B) != 0));
    emitStoreReg8(CPURegKind::STATUS_D, builder_.getLiteralU8((regs.status & Regs::STATUS_D) != 0));
    emitStoreReg8(CPURegKind::STATUS_I, builder_.getLiteralU8((regs.status & Regs::STATUS_I) != 0));
    emitStoreReg8(
        CPURegKind::STATUS_NotZ, builder_.getLiteralU8((regs.status & Regs::STATUS_Z) == 0));
    emitStoreReg8(CPURegKind::STATUS_C, builder_.getLiteralU8((regs.status & Regs::STATUS_C) != 0));
  };

  // FIXME: use startup regs.
  uint16_t startAddr;
  const Regs *startRegs = runData_ ? runData_->getStartRegs() : nullptr;
  if (startRegs) {
    initRegs(*startRegs);
    startAddr = startRegs->pc;
  } else {
    initRegs((Regs){.sp = 0xff});
    startAddr = dis_->getStart().value();
  }

  builder_.createJmp(resolveBranch(startAddr));

  for (const auto &[_, asmBlock] : dis_->asmBlocks())
    genAsmBlock(asmBlock);

  return mod;
}

void GenIR::genAsmBlock(const AsmBlock &asmBlock) {
  builder_.setInsertionBlock(basicBlockFor(&asmBlock));
  builder_.setAddress(asmBlock.addr());
  builder_.createAddCycles(builder_.getLiteralU32(lround(asmBlock.size() * 1.7 + 0.5)));

  for (auto [addr, inst] : asmBlock.instructions(dis_.get())) {
    builder_.setAddress(addr);
    genInst(addr, inst, asmBlock);
  }

  if (!builder_.getCurBasicBlock()->getTerminator())
    builder_.createJmp(resolveFallBranch(asmBlock));
}

void GenIR::genInst(uint16_t pc, const CPUInst &inst, const AsmBlock &asmBlock) {
  selfModOperand_ = false;
  pc_ = pc;

  for (unsigned i = 1, e = inst.size; i != e; ++i) {
    if (dis_->checkSelfModified((uint16_t)(pc + i))) {
      selfModOperand_ = true;
      break;
    }
  }

  Value *tmp1, *tmp2;
  switch (inst.kind) {
  case CPUInstKind::ADC:
    emitADC(inst);
    break;
  case CPUInstKind::SBC:
    emitSBC(inst);
    break;

  case CPUInstKind::AND:
    tmp2 = genRead(inst);
    emitStoreReg8(
        CPURegKind::A, emitUpdateNZ(builder_.createAnd8(emitLoadReg8(CPURegKind::A), tmp2)));
    break;
  case CPUInstKind::ORA:
    tmp2 = genRead(inst);
    emitStoreReg8(
        CPURegKind::A, emitUpdateNZ(builder_.createOr8(emitLoadReg8(CPURegKind::A), tmp2)));
    break;
  case CPUInstKind::EOR:
    tmp2 = genRead(inst);
    emitStoreReg8(
        CPURegKind::A, emitUpdateNZ(builder_.createXor8(emitLoadReg8(CPURegKind::A), tmp2)));
    break;

  case CPUInstKind::BCC:
    emitJCond(false, inst, emitLoadReg8(CPURegKind::STATUS_C), asmBlock);
    break;
  case CPUInstKind::BCS:
    emitJCond(true, inst, emitLoadReg8(CPURegKind::STATUS_C), asmBlock);
    break;
  case CPUInstKind::BEQ:
    emitJCond(false, inst, emitLoadReg8(CPURegKind::STATUS_NotZ), asmBlock);
    break;
  case CPUInstKind::BNE:
    emitJCond(true, inst, emitLoadReg8(CPURegKind::STATUS_NotZ), asmBlock);
    break;
  case CPUInstKind::BMI:
    emitJCond(true, inst, emitLoadReg8(CPURegKind::STATUS_N), asmBlock);
    break;
  case CPUInstKind::BPL:
    emitJCond(false, inst, emitLoadReg8(CPURegKind::STATUS_N), asmBlock);
    break;
  case CPUInstKind::BVC:
    emitJCond(false, inst, emitLoadReg8(CPURegKind::STATUS_V), asmBlock);
    break;
  case CPUInstKind::BVS:
    emitJCond(true, inst, emitLoadReg8(CPURegKind::STATUS_V), asmBlock);
    break;

  case CPUInstKind::CLC:
    emitStoreReg8(CPURegKind::STATUS_C, builder_.getLiteralU8(0));
    break;
  case CPUInstKind::CLD:
    emitStoreReg8(CPURegKind::STATUS_D, builder_.getLiteralU8(0));
    break;
  case CPUInstKind::CLI:
    emitStoreReg8(CPURegKind::STATUS_I, builder_.getLiteralU8(0));
    break;
  case CPUInstKind::CLV:
    emitStoreReg8(CPURegKind::STATUS_V, builder_.getLiteralU8(0));
    break;
  case CPUInstKind::SEC:
    emitStoreReg8(CPURegKind::STATUS_C, builder_.getLiteralU8(1));
    break;
  case CPUInstKind::SED:
    emitStoreReg8(CPURegKind::STATUS_D, builder_.getLiteralU8(1));
    break;
  case CPUInstKind::SEI:
    emitStoreReg8(CPURegKind::STATUS_I, builder_.getLiteralU8(1));
    break;

  case CPUInstKind::CMP:
    emitCmpCPUReg(inst, CPURegKind::A);
    break;
  case CPUInstKind::CPX:
    emitCmpCPUReg(inst, CPURegKind::X);
    break;
  case CPUInstKind::CPY:
    emitCmpCPUReg(inst, CPURegKind::Y);
    break;

  case CPUInstKind::DEC:
    tmp1 = genAddr(inst);
    emitAutoPoke8(
        tmp1, emitUpdateNZ(builder_.createSub8(emitAutoPeek8(tmp1), builder_.getLiteralU8(1))));
    break;
  case CPUInstKind::INC:
    tmp1 = genAddr(inst);
    emitAutoPoke8(
        tmp1, emitUpdateNZ(builder_.createAdd8(emitAutoPeek8(tmp1), builder_.getLiteralU8(1))));
    break;

  case CPUInstKind::DEX:
    emitStoreReg8(
        CPURegKind::X,
        emitUpdateNZ(builder_.createSub8(emitLoadReg8(CPURegKind::X), builder_.getLiteralU8(1))));
    break;
  case CPUInstKind::DEY:
    emitStoreReg8(
        CPURegKind::Y,
        emitUpdateNZ(builder_.createSub8(emitLoadReg8(CPURegKind::Y), builder_.getLiteralU8(1))));
    break;
  case CPUInstKind::INX:
    emitStoreReg8(
        CPURegKind::X,
        emitUpdateNZ(builder_.createAdd8(emitLoadReg8(CPURegKind::X), builder_.getLiteralU8(1))));
    break;
  case CPUInstKind::INY:
    emitStoreReg8(
        CPURegKind::Y,
        emitUpdateNZ(builder_.createAdd8(emitLoadReg8(CPURegKind::Y), builder_.getLiteralU8(1))));
    break;

  case CPUInstKind::JMP:
    tmp1 = genAddr(inst);
    if (auto *u16 = dyn_cast<LiteralU16>(tmp1))
      builder_.createJmp(resolveBranch(u16->getValue()));
    else
      builder_.createJmpInd(builder_.createCPUAddr2BB(tmp1));
    break;
  case CPUInstKind::JSR:
    tmp1 = genAddr(inst);
    tmp2 = resolveFallBranch(asmBlock);
    if (auto *u16 = dyn_cast<LiteralU16>(tmp1))
      builder_.createJSR(resolveBranch(u16->getValue()), tmp2);
    else
      builder_.createJSRInd(builder_.createCPUAddr2BB(tmp1), tmp2);
    break;
  case CPUInstKind::BRK:
    if (!brkBB_) {
      // Lazily create the break BB the first time it is used.
      auto save = builder_.saveState();
      brkBB_ = createBB(0x10100, false);
      builder_.setInsertionBlock(brkBB_);
      builder_.createPush8(emitEncodeFlags());
      builder_.createJmp(resolveBranch(dis_->peek16(A2_IRQ_VEC)));
    }
    builder_.createJSR(brkBB_, resolveFallBranch(asmBlock));
    break;

  case CPUInstKind::LDA:
    emitStoreReg8(CPURegKind::A, emitUpdateNZ(genRead(inst)));
    break;
  case CPUInstKind::LDX:
    emitStoreReg8(CPURegKind::X, emitUpdateNZ(genRead(inst)));
    break;
  case CPUInstKind::LDY:
    emitStoreReg8(CPURegKind::Y, emitUpdateNZ(genRead(inst)));
    break;

  case CPUInstKind::ASL:
    if (inst.addrMode == CPUAddrMode::A) {
      emitStoreReg8(
          CPURegKind::A,
          emitUpdateNZC(builder_.createShl16(
              builder_.createZExt8t16(emitLoadReg8(CPURegKind::A)), builder_.getLiteralU8(1))));
    } else {
      tmp1 = genAddr(inst);
      emitAutoPoke8(
          tmp1,
          emitUpdateNZC(builder_.createShl16(
              builder_.createZExt8t16(emitAutoPeek8(tmp1)), builder_.getLiteralU8(1))));
    }
    break;
  case CPUInstKind::LSR:
    if (inst.addrMode == CPUAddrMode::A) {
      tmp2 = emitLoadReg8(CPURegKind::A);
      emitStoreReg8(CPURegKind::STATUS_C, builder_.createAnd8(tmp2, builder_.getLiteralU8(0x01)));
      emitStoreReg8(
          CPURegKind::A, emitUpdateNZ(builder_.createShr8(tmp2, builder_.getLiteralU8(1))));
    } else {
      tmp1 = genAddr(inst);
      tmp2 = emitAutoPeek8(tmp1);
      emitStoreReg8(CPURegKind::STATUS_C, builder_.createAnd8(tmp2, builder_.getLiteralU8(0x01)));
      emitAutoPoke8(tmp1, emitUpdateNZ(builder_.createShr8(tmp2, builder_.getLiteralU8(1))));
    }
    break;
  case CPUInstKind::ROL:
    if (inst.addrMode == CPUAddrMode::A) {
      auto *op1 = builder_.createShl16(
          builder_.createZExt8t16(emitLoadReg8(CPURegKind::A)), builder_.getLiteralU8(1));
      auto *op2 = builder_.createZExt8t16(emitLoadReg8(CPURegKind::STATUS_C));
      emitStoreReg8(CPURegKind::A, emitUpdateNZC(builder_.createOr16(op1, op2)));
    } else {
      tmp1 = genAddr(inst);
      auto *op1 = builder_.createShl16(
          builder_.createZExt8t16(emitAutoPeek8(tmp1)), builder_.getLiteralU8(1));
      auto *op2 = builder_.createZExt8t16(emitLoadReg8(CPURegKind::STATUS_C));
      emitAutoPoke8(tmp1, emitUpdateNZC(builder_.createOr16(op1, op2)));
    }
    break;
  case CPUInstKind::ROR:
    if (inst.addrMode == CPUAddrMode::A) {
      tmp2 = emitLoadReg8(CPURegKind::A);
      auto *op1 = builder_.createZExt8t16(tmp2);
      auto *op2 =
          builder_.createMake16(builder_.getLiteralU8(0), emitLoadReg8(CPURegKind::STATUS_C));
      emitStoreReg8(
          CPURegKind::A,
          emitUpdateNZ(builder_.createTrunc16t8(
              builder_.createShr16(builder_.createOr16(op1, op2), builder_.getLiteralU8(1)))));
      emitStoreReg8(CPURegKind::STATUS_C, builder_.createAnd8(tmp2, builder_.getLiteralU8(0x01)));
    } else {
      tmp1 = genAddr(inst);
      tmp2 = emitAutoPeek8(tmp1);
      auto *op1 = builder_.createZExt8t16(tmp2);
      auto *op2 =
          builder_.createMake16(builder_.getLiteralU8(0), emitLoadReg8(CPURegKind::STATUS_C));
      emitAutoPoke8(
          tmp1,
          emitUpdateNZ(builder_.createTrunc16t8(
              builder_.createShr16(builder_.createOr16(op1, op2), builder_.getLiteralU8(1)))));
      emitStoreReg8(CPURegKind::STATUS_C, builder_.createAnd8(tmp2, builder_.getLiteralU8(0x01)));
    }
    break;
  case CPUInstKind::BIT:
    tmp2 = genRead(inst);
    emitStoreReg8(CPURegKind::STATUS_N, builder_.createAnd8(tmp2, builder_.getLiteralU8(0x80)));
    emitStoreReg8(
        CPURegKind::STATUS_V,
        builder_.createAnd8(
            builder_.createShr8(tmp2, builder_.getLiteralU8(6)), builder_.getLiteralU8(0x01)));
    emitStoreReg8(CPURegKind::STATUS_NotZ, builder_.createAnd8(emitLoadReg8(CPURegKind::A), tmp2));
    break;

  case CPUInstKind::NOP:
    break;

  case CPUInstKind::PHA:
    builder_.createPush8(emitLoadReg8(CPURegKind::A));
    break;
  case CPUInstKind::PLA:
    emitStoreReg8(CPURegKind::A, emitUpdateNZ(builder_.createPop8()));
    break;
  case CPUInstKind::PHP:
    builder_.createPush8(emitEncodeFlags());
    break;
  case CPUInstKind::PLP:
    emitDecodeFlags(builder_.createPop8());
    break;

  case CPUInstKind::RTI:
    emitDecodeFlags(builder_.createPop8());
    builder_.createRTS();
    break;
  case CPUInstKind::RTS:
    builder_.createRTS();
    break;

  case CPUInstKind::STA:
    tmp1 = genAddr(inst);
    emitWrite(inst, tmp1, emitLoadReg8(CPURegKind::A));
    break;
  case CPUInstKind::STX:
    tmp1 = genAddr(inst);
    emitWrite(inst, tmp1, emitLoadReg8(CPURegKind::X));
    break;
  case CPUInstKind::STY:
    tmp1 = genAddr(inst);
    emitWrite(inst, tmp1, emitLoadReg8(CPURegKind::Y));
    break;
  case CPUInstKind::TAX:
    emitStoreReg8(CPURegKind::X, emitUpdateNZ(emitLoadReg8(CPURegKind::A)));
    break;
  case CPUInstKind::TAY:
    emitStoreReg8(CPURegKind::Y, emitUpdateNZ(emitLoadReg8(CPURegKind::A)));
    break;
  case CPUInstKind::TSX:
    emitStoreReg8(CPURegKind::X, emitUpdateNZ(emitLoadReg8(CPURegKind::SP)));
    break;
  case CPUInstKind::TXA:
    emitStoreReg8(CPURegKind::A, emitUpdateNZ(emitLoadReg8(CPURegKind::X)));
    break;
  case CPUInstKind::TXS:
    emitStoreReg8(CPURegKind::SP, emitLoadReg8(CPURegKind::X));
    break;
  case CPUInstKind::TYA:
    emitStoreReg8(CPURegKind::A, emitUpdateNZ(emitLoadReg8(CPURegKind::Y)));
    break;

  case CPUInstKind::INVALID:
    builder_.createJmp(createAbortBlock(pc, 3));
    break;

  default:
    fprintf(stderr, "Unsupported instruction %s\n", cpuInstName(inst.kind));
    assert(false && "Unsupported instruction");
    abort();
  }
}

BasicBlock *GenIR::createBB(uint32_t addr, bool real) {
  auto *res = builder_.getCurBasicBlock()->getFunction()->createBasicBlock();
  res->setAddress(addr, real);
  return res;
}

BasicBlock *GenIR::createAbortBlock(uint16_t target, uint8_t reason) {
  auto save = builder_.saveState();
  auto *abortBlock = createBB(target, false);
  builder_.setInsertionBlock(abortBlock);
  builder_.setAddress(target);
  builder_.createAbort(
      builder_.getLiteralU16(pc_), builder_.getLiteralU16(target), builder_.getLiteralU8(reason));
  return abortBlock;
}

BasicBlock *GenIR::basicBlockFor(const AsmBlock *asmBlock) {
  assert(asmBlock && "asmBlock must not be null");
  auto &entry = asmBlockMap_[asmBlock];
  if (!entry)
    entry = createBB(asmBlock->addr(), true);
  return entry;
}

BasicBlock *GenIR::resolveBranch(uint16_t addr) {
  if (auto *asmBlock = dis_->cfindAsmBlockAt(addr))
    return basicBlockFor(asmBlock);
  else
    return createAbortBlock(addr, 1);
}

BasicBlock *GenIR::resolveFallBranch(const AsmBlock &curAsmBlock) {
  if (auto *fall = curAsmBlock.getFallBlock())
    return basicBlockFor(fall);
  else
    return createAbortBlock(curAsmBlock.endAddr(), 2);
}

Value *GenIR::emitUpdateNZ(Value *value8) {
  assert(value8->getType()->getKind() == TypeKind::U8);
  emitStoreReg8(CPURegKind::STATUS_NotZ, value8);
  emitStoreReg8(CPURegKind::STATUS_N, builder_.createAnd8(value8, builder_.getLiteralU8(0x80)));
  return value8;
}

Value *GenIR::emitUpdateNZC(Value *value16) {
  assert(value16->getType()->getKind() == TypeKind::U16);
  emitStoreReg8(CPURegKind::STATUS_C, builder_.createHigh8(value16));
  return emitUpdateNZ(builder_.createTrunc16t8(value16));
}

Value *GenIR::emitEncodeFlags() {
  return builder_.createEncodeFlags_6op({
      emitLoadReg8(CPURegKind::STATUS_C),
      emitLoadReg8(CPURegKind::STATUS_NotZ),
      emitLoadReg8(CPURegKind::STATUS_I),
      emitLoadReg8(CPURegKind::STATUS_D),
      emitLoadReg8(CPURegKind::STATUS_V),
      emitLoadReg8(CPURegKind::STATUS_N),
  });
}

void GenIR::emitDecodeFlags(Value *value8) {
  assert(value8->getType()->getKind() == TypeKind::U8);

  emitStoreReg8(CPURegKind::STATUS_C, builder_.createDecodeFlag(value8, builder_.getLiteralU8(0)));
  emitStoreReg8(
      CPURegKind::STATUS_NotZ, builder_.createDecodeFlag(value8, builder_.getLiteralU8(1)));
  emitStoreReg8(CPURegKind::STATUS_I, builder_.createDecodeFlag(value8, builder_.getLiteralU8(2)));
  emitStoreReg8(CPURegKind::STATUS_D, builder_.createDecodeFlag(value8, builder_.getLiteralU8(3)));
  emitStoreReg8(CPURegKind::STATUS_B, builder_.getLiteralU8(0));
  emitStoreReg8(CPURegKind::STATUS_V, builder_.createDecodeFlag(value8, builder_.getLiteralU8(6)));
  emitStoreReg8(CPURegKind::STATUS_N, builder_.createDecodeFlag(value8, builder_.getLiteralU8(7)));
}

void GenIR::emitADC(const CPUInst &inst) {
  const bool simpleADC = baseStats_ && !baseStats_->decimalSet;

  BasicBlock *normalBB = nullptr;
  BasicBlock *decimalBB = nullptr;
  BasicBlock *nextBB = nullptr;

  if (!simpleADC) {
    normalBB = createBB(pc_, false);
    decimalBB = createBB(pc_, false);
    nextBB = createBB(pc_ + inst.size, false);

    builder_.createJTrue(emitLoadReg8(CPURegKind::STATUS_D), decimalBB, normalBB);
    builder_.setInsertionBlock(normalBB);
  }
  {
    auto *op1 = builder_.createZExt8t16(emitLoadReg8(CPURegKind::A));
    auto *op2 = builder_.createZExt8t16(genRead(inst));
    auto *add1 = builder_.createAdd16(op1, op2);
    auto *op3 = builder_.createZExt8t16(emitLoadReg8(CPURegKind::STATUS_C));
    auto *add2 = builder_.createAdd16(add1, op3);
    emitStoreReg8(CPURegKind::STATUS_C, builder_.createHigh8(add2));
    emitStoreReg8(CPURegKind::STATUS_V, builder_.createOvf8(add2, op1, op2));
    auto *res = builder_.createTrunc16t8(add2);
    emitUpdateNZ(res);
    emitStoreReg8(CPURegKind::A, res);
  }
  if (simpleADC)
    return;
  builder_.createJmp(nextBB);

  builder_.setInsertionBlock(decimalBB);
  {
    auto *op1 = emitLoadReg8(CPURegKind::A);
    auto *op2 = genRead(inst);
    auto *op3 = emitLoadReg8(CPURegKind::STATUS_C);
    auto *res16 = builder_.createAdcDec16(op1, op2, op3);
    emitStoreReg8(CPURegKind::A, builder_.createTrunc16t8(res16));
    auto *flags = builder_.createHigh8(res16);
    emitStoreReg8(CPURegKind::STATUS_C, builder_.createDecodeFlag(flags, builder_.getLiteralU8(1)));
    emitStoreReg8(
        CPURegKind::STATUS_NotZ, builder_.createDecodeFlag(flags, builder_.getLiteralU8(2)));
    emitStoreReg8(CPURegKind::STATUS_V, builder_.createDecodeFlag(flags, builder_.getLiteralU8(6)));
    emitStoreReg8(CPURegKind::STATUS_N, builder_.createDecodeFlag(flags, builder_.getLiteralU8(7)));
  }
  builder_.createJmp(nextBB);

  builder_.setInsertionBlock(nextBB);
}

void GenIR::emitSBC(const CPUInst &inst) {
  const bool simpleSBC = baseStats_ && !baseStats_->decimalSet;

  BasicBlock *normalBB = nullptr;
  BasicBlock *decimalBB = nullptr;
  BasicBlock *nextBB = nullptr;

  if (!simpleSBC) {
    normalBB = createBB(pc_, false);
    decimalBB = createBB(pc_, false);
    nextBB = createBB(pc_ + inst.size, false);

    builder_.createJTrue(emitLoadReg8(CPURegKind::STATUS_D), decimalBB, normalBB);
    builder_.setInsertionBlock(normalBB);
  }
  {
    auto *op1 = builder_.createZExt8t16(emitLoadReg8(CPURegKind::A));
    auto *op2 = builder_.createZExt8t16(genRead(inst));
    auto *sub1 = builder_.createSub16(op1, op2);
    auto *op3 = builder_.createZExt8t16(
        builder_.createSub8(builder_.getLiteralU8(1), emitLoadReg8(CPURegKind::STATUS_C)));
    // a - b - (1 - c).
    auto *sub2 = builder_.createSub16(sub1, op3);
    // This is the borrow flag that results from subtraction.
    auto *borrow = builder_.createAnd8(builder_.createHigh8(sub2), builder_.getLiteralU8(0x01));
    // However, 6502 needs a carry, which is inverted: 1 - borrow.
    emitStoreReg8(CPURegKind::STATUS_C, builder_.createSub8(builder_.getLiteralU8(1), borrow));
    // Note that a subsequent SBC a2, b2 will evaluate like this:
    //    a2 - b2 - (1 - c) <=> a2 - b2 - (1 - (1 - borrow)) <=> a2 - b2 - borrow.
    emitStoreReg8(CPURegKind::STATUS_V, builder_.createOvf8(sub2, op1, builder_.createNot16(op2)));
    auto *res = builder_.createTrunc16t8(sub2);
    emitUpdateNZ(res);
    emitStoreReg8(CPURegKind::A, res);
  }
  if (simpleSBC)
    return;
  builder_.createJmp(nextBB);

  builder_.setInsertionBlock(decimalBB);
  {
    auto *op1 = emitLoadReg8(CPURegKind::A);
    auto *op2 = genRead(inst);
    auto *op3 = emitLoadReg8(CPURegKind::STATUS_C);
    auto *res16 = builder_.createSbcDec16(op1, op2, op3);
    emitStoreReg8(CPURegKind::A, builder_.createTrunc16t8(res16));
    auto *flags = builder_.createHigh8(res16);
    emitStoreReg8(CPURegKind::STATUS_C, builder_.createAnd8(flags, builder_.getLiteralU8(0x01)));
    emitStoreReg8(CPURegKind::STATUS_NotZ, builder_.createAnd8(flags, builder_.getLiteralU8(0x02)));
    emitStoreReg8(CPURegKind::STATUS_V, builder_.createAnd8(flags, builder_.getLiteralU8(0x40)));
    emitStoreReg8(CPURegKind::STATUS_N, builder_.createAnd8(flags, builder_.getLiteralU8(0x80)));
  }
  builder_.createJmp(nextBB);

  builder_.setInsertionBlock(nextBB);
}

Value *GenIR::emitLoadReg8(CPURegKind cpuReg) {
  return builder_.createLoadR8(builder_.getCPUReg(cpuReg));
}

void GenIR::emitStoreReg8(CPURegKind cpuReg, Value *value) {
  builder_.createStoreR8(builder_.getCPUReg(cpuReg), value);
}

void GenIR::emitCmpCPUReg(const CPUInst &inst, CPURegKind cpuReg) {
  Value *regValue = emitLoadReg8(cpuReg);
  Value *data = genRead(inst);
  emitStoreReg8(CPURegKind::STATUS_NotZ, builder_.createCmp8ne(regValue, data));
  emitStoreReg8(CPURegKind::STATUS_C, builder_.createCmp8ae(regValue, data));
  emitStoreReg8(
      CPURegKind::STATUS_N,
      builder_.createAnd8(builder_.createSub8(regValue, data), builder_.getLiteralU8(0x80)));
}

void GenIR::emitJCond(bool jTrue, const CPUInst &inst, Value *cond, const AsmBlock &asmBlock) {
  if (!selfModOperand_) {
    auto *target = resolveBranch(inst.operand);
    auto *fall = resolveFallBranch(asmBlock);
    if (jTrue)
      builder_.createJTrue(cond, target, fall);
    else
      builder_.createJFalse(cond, target, fall);
    return;
  }

  // A self modified conditional branch target.
  BasicBlock *indBranchBlock = createBB(pc_, false);

  // Jmp over the indirect branch to the fall block if the condition is false.
  if (jTrue)
    builder_.createJFalse(cond, resolveFallBranch(asmBlock), indBranchBlock);
  else
    builder_.createJTrue(cond, resolveFallBranch(asmBlock), indBranchBlock);

  builder_.setInsertionBlock(indBranchBlock);
  // Instruction byte 1, sign extended to 16-bit.
  auto *brOffset =
      builder_.createSExt8t16(builder_.createRamPeek8(builder_.getLiteralU16(pc_ + 1)));
  // Next instruction address.
  auto *nextAddr = builder_.getLiteralU16(pc_ + 2);
  // Indirect jump to brOffset + nextAddr.
  builder_.createJmpInd(builder_.createCPUAddr2BB(builder_.createAdd16(nextAddr, brOffset)));
}

void GenIR::emitWrite(const CPUInst &inst, Value *addr, Value *value) {
  switch (inst.addrMode) {
  case CPUAddrMode::A:
    emitStoreReg8(CPURegKind::A, value);
    return;

  case CPUAddrMode::Abs:
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Ind:
  case CPUAddrMode::X_Ind:
  case CPUAddrMode::Ind_Y:
  case CPUAddrMode::Zpg:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
    emitAutoPoke8(addr, value);
    return;

  case CPUAddrMode::Imm:
  case CPUAddrMode::Rel:
  case CPUAddrMode::Implied:
  case CPUAddrMode::_last:
  case CPUAddrMode::_invalid:
  default:
    assert(false);
    abort();
  }
}

Value *GenIR::genRead(const CPUInst &inst) {
  Value *addr;
  switch (inst.addrMode) {
  case CPUAddrMode::A:
    return emitLoadReg8(CPURegKind::A);
  case CPUAddrMode::Imm:
    return !selfModOperand_ ? (Value *)builder_.getLiteralU8(inst.operand)
                            : (Value *)builder_.createRamPeek8(builder_.getLiteralU16(pc_ + 1));

  case CPUAddrMode::Abs:
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Ind:
  case CPUAddrMode::X_Ind:
  case CPUAddrMode::Ind_Y:
  case CPUAddrMode::Zpg:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
    return emitAutoPeek8(genAddr(inst));

  case CPUAddrMode::Rel:
  case CPUAddrMode::Implied:
  case CPUAddrMode::_last:
  case CPUAddrMode::_invalid:
  default:
    assert(false);
    abort();
  }
}

Value *GenIR::genAddr(const CPUInst &inst) {
  Value *tmp;
  if (!selfModOperand_) {
    switch (inst.addrMode) {
    case CPUAddrMode::Zpg:
    case CPUAddrMode::Abs:
      // snprintf(buf, sizeof(buf), "0x%04x", inst.operand);
      return builder_.getLiteralU16(inst.operand);

    case CPUAddrMode::Abs_X:
      // snprintf(buf, sizeof(buf), "0x%04x + s_x", inst.operand);
      return builder_.createAdd16(
          builder_.getLiteralU16(inst.operand),
          builder_.createZExt8t16(emitLoadReg8(CPURegKind::X)));

    case CPUAddrMode::Abs_Y:
      // snprintf(buf, sizeof(buf), "0x%04x + s_y", inst.operand);
      return builder_.createAdd16(
          builder_.getLiteralU16(inst.operand),
          builder_.createZExt8t16(emitLoadReg8(CPURegKind::Y)));

    case CPUAddrMode::Ind:
      // snprintf(buf, sizeof(buf), "peek16(0x%04x)", inst.operand);
      return emitAutoPeek16(inst.operand);

    case CPUAddrMode::X_Ind: {
      // snprintf(buf, sizeof(buf), "peek16_zpg(0x%02x + s_x)", inst.operand);
      //  %1 = add8 const, X
      //  %2 = ZExt8t16 %1
      //  %3 = peek8 %2
      //  %4 = add8 const+1, X
      //  %5 = ZExt816 %4
      //  %6 = peek8 %5
      //  %7 = make16 %3, %6
      auto *X = emitLoadReg8(CPURegKind::X);
      auto *op1 = builder_.createRamPeek8(
          builder_.createZExt8t16(builder_.createAdd8(builder_.getLiteralU8(inst.operand), X)));
      return builder_.createMake16(
          op1,
          builder_.createRamPeek8(builder_.createZExt8t16(
              builder_.createAdd8(builder_.getLiteralU8(inst.operand + 1), X))));
    }

    case CPUAddrMode::Ind_Y:
      // snprintf(buf, sizeof(buf), "peek16_zpg(0x%02x) + s_y", inst.operand);
      tmp = emitPeek16_zpg(inst.operand);
      return builder_.createAdd16(tmp, builder_.createZExt8t16(emitLoadReg8(CPURegKind::Y)));

    case CPUAddrMode::Zpg_X:
      // snprintf(buf, sizeof(buf), "(uint8_t)(0x%02x + s_x)", inst.operand);
      return builder_.createZExt8t16(
          builder_.createAdd8(builder_.getLiteralU8(inst.operand), emitLoadReg8(CPURegKind::X)));

    case CPUAddrMode::Zpg_Y:
      // snprintf(buf, sizeof(buf), "(uint8_t)(0x%02x + s_y)", inst.operand);
      return builder_.createZExt8t16(
          builder_.createAdd8(builder_.getLiteralU8(inst.operand), emitLoadReg8(CPURegKind::Y)));

    case CPUAddrMode::A:
    case CPUAddrMode::Imm:
    case CPUAddrMode::Implied:
    case CPUAddrMode::Rel:
    case CPUAddrMode::_last:
    case CPUAddrMode::_invalid:
    default:
      assert(false);
      abort();
    }
  } else {
    switch (inst.addrMode) {
    case CPUAddrMode::Abs:
      // snprintf(buf, sizeof(buf), "ram_peek16(0x%04x)", scPC_ + 1);
      return emitRamPeek16(pc_ + 1);

    case CPUAddrMode::Abs_X:
      // snprintf(buf, sizeof(buf), "ram_peek16(0x%04x) + s_x", scPC_ + 1);
      tmp = emitRamPeek16(pc_ + 1);
      return builder_.createAdd16(tmp, builder_.createZExt8t16(emitLoadReg8(CPURegKind::X)));

    case CPUAddrMode::Abs_Y:
      // snprintf(buf, sizeof(buf), "ram_peek16(0x%04x) + s_y", scPC_ + 1);
      tmp = emitRamPeek16(pc_ + 1);
      return builder_.createAdd16(tmp, builder_.createZExt8t16(emitLoadReg8(CPURegKind::Y)));

    case CPUAddrMode::Ind:
      // snprintf(buf, sizeof(buf), "peek16(ram_peek16(0x%04x))", scPC_ + 1);
      return emitAutoPeek16(emitRamPeek16(pc_ + 1));

    case CPUAddrMode::X_Ind: {
      // snprintf(buf, sizeof(buf), "peek16_zpg(ram_peek(0x%04x) + s_x)", scPC_ + 1);
      //   %1 = add8 const, X
      //   %2 = ZExt8t16 %1
      //   %3 = peek8 %2
      //   %4 = add8 1, %1
      //   %5 = ZExt816 %4
      //   %6 = peek8 %5
      //   %7 = make16 %3, %6
      tmp = builder_.createRamPeek8(builder_.getLiteralU16(pc_ + 1));
      auto *X = emitLoadReg8(CPURegKind::X);
      auto *addr = builder_.createAdd8(tmp, X);
      auto *op1 = builder_.createRamPeek8(builder_.createZExt8t16(addr));
      return builder_.createMake16(
          op1,
          builder_.createRamPeek8(
              builder_.createZExt8t16(builder_.createAdd8(builder_.getLiteralU8(1), addr))));
    }

    case CPUAddrMode::Ind_Y:
      // snprintf(buf, sizeof(buf), "peek16_zpg(ram_peek(0x%04x)) + s_y", scPC_ + 1);
      tmp = emitRamPeek16(pc_ + 1);
      tmp = emitPeek16_zpg(tmp);
      return builder_.createAdd16(tmp, builder_.createZExt8t16(emitLoadReg8(CPURegKind::Y)));

    case CPUAddrMode::Zpg:
      // snprintf(buf, sizeof(buf), "ram_peek(0x%04x)", scPC_ + 1);
      return builder_.createZExt8t16(builder_.createRamPeek8(builder_.getLiteralU16(pc_ + 1)));

    case CPUAddrMode::Zpg_X:
      // snprintf(buf, sizeof(buf), "(uint8_t)(ram_peek(0x%04x) + s_x)", scPC_ + 1);
      tmp = builder_.createRamPeek8(builder_.getLiteralU16(pc_ + 1));
      return builder_.createZExt8t16(builder_.createAdd8(tmp, emitLoadReg8(CPURegKind::X)));

    case CPUAddrMode::Zpg_Y:
      // snprintf(buf, sizeof(buf), "(uint8_t)(ram_peek(0x%04x) + s_y)", scPC_ + 1);
      tmp = builder_.createRamPeek8(builder_.getLiteralU16(pc_ + 1));
      return builder_.createZExt8t16(builder_.createAdd8(tmp, emitLoadReg8(CPURegKind::Y)));

    case CPUAddrMode::A:
    case CPUAddrMode::Imm:
    case CPUAddrMode::Implied:
    case CPUAddrMode::Rel:
      // snprintf(
      //     buf,
      //     sizeof(buf),
      //     "(uint16_t)(0x%04x + (int8_t)ram_peek(0x%04x))",
      //     (uint16_t)(scPC_ + 2),
      //     scPC_ + 1);
      // break;
    case CPUAddrMode::_last:
    case CPUAddrMode::_invalid:
    default:
      assert(false);
      abort();
    }
  }
}

Value *GenIR::emitRamPeek16(uint16_t addr) {
  if ((addr & 1) == 0) {
    // Aligned address.
    return builder_.createRamPeek16al(builder_.getLiteralU16(addr));
  } else if (addr < 0xFFFF) {
    // Unaligned address without wraparound.
    return builder_.createRamPeek16un(builder_.getLiteralU16(addr));
  } else {
    // The very unlikely case of reading 16 bits from 0xFFFF. Read two separate
    // bytes and combine them.
    return builder_.createMake16(
        builder_.createRamPeek8(builder_.getLiteralU16(0xFFFF)),
        builder_.createRamPeek8(builder_.getLiteralU16(0)));
  }
}

Value *GenIR::emitAutoPeek16(uint16_t addr) {
  auto *addrValue = builder_.getLiteralU16(addr);
  if ((addr & 1) == 0) {
    // Aligned address.
    return addr < A2_IO_RANGE_START - 1 ? builder_.createRamPeek16al(addrValue)
                                        : builder_.createPeek16al(addrValue);
  } else if (addr < 0xFFFF) {
    // Unaligned address without wraparound.
    return addr < A2_IO_RANGE_START - 1 ? builder_.createRamPeek16un(addrValue)
                                        : builder_.createPeek16un(addrValue);
  } else {
    // The very unlikely case of reading 16 bits from 0xFFFF. Read two separate
    // bytes and combine them.
    return builder_.createMake16(
        builder_.createRamPeek8(builder_.getLiteralU16(0xFFFF)),
        builder_.createRamPeek8(builder_.getLiteralU16(0)));
  }
}

Value *GenIR::emitAutoPeek16(Value *addr) {
  if (auto *u16 = dyn_cast<LiteralU16>(addr))
    return emitAutoPeek16(u16->getValue());

  if (isAddrNonIO(addr, 2)) {
    return builder_.createRamPeek16un(addr);
  } else {
    auto *op1 = builder_.createPeek8(addr);
    return builder_.createMake16(
        op1, builder_.createPeek8(builder_.createAdd16(addr, builder_.getLiteralU16(1))));
  }
}

Value *GenIR::emitPeek16_zpg(uint8_t addr) {
  auto *addrValue = builder_.getLiteralU16(addr);
  if ((addr & 1) == 0) {
    // Aligned address.
    return builder_.createRamPeek16al(addrValue);
  } else if (addr < 0xFF) {
    return builder_.createRamPeek16un(addrValue);
  } else {
    // The unlikely case of reading 16 bits from 0xFF..0x00. Read two separate
    // bytes and combine them.
    auto *op1 = builder_.createRamPeek8(builder_.getLiteralU16(0xFF));
    return builder_.createMake16(op1, builder_.createRamPeek8(builder_.getLiteralU16(0)));
  }
}

Value *GenIR::emitPeek16_zpg(Value *addr) {
  if (auto *u8 = dyn_cast<LiteralU8>(addr))
    return emitPeek16_zpg(u8->getValue());

  auto *op1 = builder_.createRamPeek8(addr);
  return builder_.createMake16(
      op1,
      builder_.createRamPeek8(
          builder_.createZExt8t16(builder_.createAdd8(addr, builder_.getLiteralU8(1)))));
}

Value *GenIR::emitAutoPeek8(Value *addr) {
  return isAddrNonIO(addr) ? builder_.createRamPeek8(addr) : builder_.createPeek8(addr);
}

void GenIR::emitAutoPoke8(Value *addr, Value *value) {
  if (isAddrNonIO(addr))
    builder_.createRamPoke8(addr, value);
  else
    builder_.createPoke8(addr, value);
}

bool GenIR::isAddrNonIO(Value *addr, unsigned width) {
  --width;
  assert(addr->getType()->getKind() == TypeKind::U16 && "Address must be 16-bit");
  if (auto *u16 = dyn_cast<LiteralU16>(addr)) {
    return u16->getValue() < A2_IO_RANGE_START - width;
  } else if (addr->getKind() == ValueKind::ZExt8t16) {
    // 8-bit values are always smaller than the IO range.
    return true;
  } else if (addr->getKind() == ValueKind::Add16) {
    auto *inst = cast<Instruction>(addr);
    auto *op1 = inst->getOperand(0);
    auto *op2 = inst->getOperand(1);
    // If we have a zext, it will be in operand2.
    if (op1->getKind() == ValueKind::ZExt8t16)
      std::swap(op1, op2);
    // zext + zext.
    if (op2->getKind() == ValueKind::ZExt8t16) {
      // Zext + Zext is <= 255*2, which is always less.
      if (op1->getKind() == ValueKind::ZExt8t16)
        return true;
      if (auto *op1u16 = dyn_cast<LiteralU16>(op1)) {
        // literal16 + [0..255] + ofs < A2_IO_RANGE_START.
        // literal16 < A2_IO_RANGE_START - 255 - ofs.
        if (op1u16->getValue() < A2_IO_RANGE_START - 255 - width)
          return true;

        // literal16 + [0..255] > A2_IO_RANGE_END
        // literal16 > A2_IO_RANGE_END
        if (op1u16->getValue() > A2_IO_RANGE_END)
          return true;
      }
    }
  }
  return false;
}

std::shared_ptr<ir::IRContext> newIRContext() {
  return std::make_shared<IRContext>();
}

Module *genIR(const std::shared_ptr<Disas> &disas, ir::IRContext &ctx) {
  GenIR genIR(disas, &ctx);
  return genIR.run();
}

void dumpModule(ir::Module *mod, bool irTrees) {
  ir::IRDumper(stdout, irTrees).dump(mod);
}
