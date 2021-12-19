/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/apple2.h"

#include "apple2tc/apple2iodefs.h"

#include <cassert>

uint8_t EmuApple2::ioPeek(uint16_t addr) {
  assert(addr >= IO_RANGE_START && addr <= IO_RANGE_END);

  switch (addr & 0xCFF0) {
  case A2_SPKR:
    if (debug_ & DebugIO2)
      fprintf(stderr, "[%u] SPKR\n", getCycles());
    if (spkrCB_)
      spkrCB_(spkrCBCtx_, getCycles());
    break;
  default:
    return a2_io_peek(&io_, addr, getCycles());
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
