/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/apple2.h"

#include "apple2tc/apple2iodefs.h"

#include <cassert>

uint8_t EmuApple2::kbd() {
  return keys_.empty() ? lastKey_ : keys_.front() | 0x80;
}

void EmuApple2::kbdstrb() {
  if (!keys_.empty()) {
    lastKey_ = keys_.front() & 0x7F;
    keys_.erase(keys_.begin());
  }
}

uint8_t EmuApple2::ioPeek(uint16_t addr) {
  assert(addr >= IO_RANGE_START && addr <= IO_RANGE_END);

  switch (addr & 0xCFF0) {
  case A2_KBD:
    if (debug_ & DebugIO2)
      fprintf(stderr, "[%u] KBD\n", getCycles());
    return kbd();
  case A2_KBDSTRB:
    if (debug_ & DebugIO1)
      fprintf(stderr, "[%u] KBDSTRB\n", getCycles());
    kbdstrb();
    break;
  case A2_TAPEOUT:
    if (debug_ & DebugIO1)
      fprintf(stderr, "[%u] TAPEOUT\n", getCycles());
    break;
  case A2_SPKR:
    if (debug_ & DebugIO2)
      fprintf(stderr, "[%u] SPKR\n", getCycles());
    if (spkrCB_)
      spkrCB_(spkrCBCtx_, getCycles());
    break;
  case A2_STROBE:
    if (debug_ & DebugIO1)
      fprintf(stderr, "[%u] STROBE\n", getCycles());
    break;

  case A2_TXTCLR:
    switch (addr) {
    case A2_TXTCLR:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] TXTCLR\n", getCycles());
      vidControl_ &= ~VCText;
      break;
    case A2_TXTSET:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] TXTSET\n", getCycles());
      vidControl_ |= VCText;
      break;
    case A2_MIXCLR:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] MIXCLR\n", getCycles());
      vidControl_ &= ~VCMixed;
      break;
    case A2_MIXSET:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] MIXSET\n", getCycles());
      vidControl_ |= VCMixed;
      break;
    case A2_LOWSCR:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] LOWSCR\n", getCycles());
      vidControl_ &= ~VCPage2;
      break;
    case A2_HISCR:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] HISCR\n", getCycles());
      vidControl_ |= VCPage2;
      break;
    case A2_LORES:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] LORES\n", getCycles());
      vidControl_ &= ~VCHires;
      break;
    case A2_HIRES:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] HIRES\n", getCycles());
      vidControl_ |= VCHires;
      break;
    default:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] ANNUNCIATORS $%04X\n", getCycles(), addr);
      break;
    }
    break;

  default:
    fprintf(stderr, "[%u] Unsupported IO location read $%04X\n", getCycles(), addr);
  }

  return 0;
}

void EmuApple2::ioPoke(uint16_t addr, uint8_t value) {
  if (debug_ & DebugIO1)
    fprintf(stderr, "[%u] IOPOKE\n", getCycles());
  EmuApple2::ioPeek(addr);
  // According to the Apple II Reference Manual, a write is preceeded by a read.
  EmuApple2::ioPeek(addr);
}
