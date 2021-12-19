/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/a2io.h"
#include "apple2tc/emu6502.h"

#include <cstdio>
#include <vector>

class EmuApple2 : public Emu6502 {
public:
  /// TXTTAB Applesoft Start of Program Pointer (2B).
  static constexpr uint16_t TXTTAB = 0x67;
  /// PRGEND Applesoft End of Program Pointer (2B).
  static constexpr uint16_t PRGEND = 0xAF;

  static constexpr uint16_t TXT1SCRN = 0x0400;
  static constexpr uint16_t TXT2SCRN = 0x0800;
  static constexpr uint16_t HGR1SCRN = 0x2000;
  static constexpr uint16_t HGR2SCRN = 0x4000;

  /// Enqueue a new key press.
  /// IO space range.
  ///
  /// In the future we may want to support dynamic callbacks per page, and
  /// register device behavior, etc, but for now this is sufficient.
  static constexpr uint16_t IO_RANGE_START = 0xC000;
  static constexpr uint16_t IO_RANGE_END = 0xCFFF;

  EmuApple2() : Emu6502(IO_RANGE_START, IO_RANGE_END) {
    a2_io_init(&io_);
  }
  ~EmuApple2() {
    a2_io_done(&io_);
  }

  void setSpeakerCB(void *ctx, void (*spkrCB)(void *ctx, unsigned cycles)) {
    spkrCB_ = spkrCB;
    spkrCBCtx_ = ctx;
  }

  const a2_iostate_t *io() const {
    return &io_;
  }
  a2_iostate_t *io() {
    return &io_;
  }

protected:
  /// Perform a read in the IO range.
  uint8_t ioPeek(uint16_t addr) override;
  /// Perform a write in the IO range.
  void ioPoke(uint16_t addr, uint8_t value) override;

private:
  a2_iostate_t io_;

  /// Callback invoked SPKR ($C03x) access.
  void (*spkrCB_)(void *ctx, unsigned cycles) = nullptr;
  void *spkrCBCtx_ = nullptr;
};

/// Dump the BASIC program as binary tokens.
void dumpApplesoftBasic(FILE *f, const EmuApple2 *emu);
