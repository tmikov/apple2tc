/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/apple2.h"

#include <cassert>

uint8_t EmuApple2::ioPeek(uint16_t addr) {
  assert(addr >= IO_RANGE_START && addr <= IO_RANGE_END);
  return a2_io_peek(&io_, addr, getCycles());
}

void EmuApple2::ioPoke(uint16_t addr, uint8_t value) {
  a2_io_poke(&io_, addr, value, getCycles());
}

void EmuApple2::ioLoadROM(const uint8_t *rom, uint16_t size) {
  a2_language_card_load_rom(&io_.language_card, rom, size);
}

void EmuApple2::refreshMainRAMView() {
  a2_language_card_sync_ram(&io_.language_card, ram_);
}
