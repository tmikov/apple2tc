/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/system.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint16_t s_pc = 0;
static uint8_t s_a = 0;
static uint8_t s_x = 0;
static uint8_t s_y = 0;
static uint8_t s_status = 0;
static uint8_t s_sp = 0;

static int s_cycles = 0;
static int s_remaining_cycles = 0;

static uint8_t s_ram[0x10000];

uint8_t g_debug = 0;

#define CYCLES(pc, cycles)                                                 \
  do {                                                                     \
    s_cycles += (cycles);                                                  \
    s_remaining_cycles -= (cycles);                                        \
    if ((g_debug & DebugASM) && (!(g_debug & DebugEmu) || branchTarget)) { \
      branchTarget = false;                                                \
      debug_asm(pc);                                                       \
    }                                                                      \
  } while (0)

void reset_regs(void) {
  memset(s_ram, 0xFF, 0x10000);
  s_pc = 0;
  s_a = 0;
  s_x = 0;
  s_y = 0;
  s_status = STATUS_IGNORED;
  s_sp = 0xFF;
}

void set_regs(regs_t r) {
  s_pc = r.pc;
  s_a = r.a;
  s_x = r.x;
  s_y = r.y;
  s_status = r.status;
  s_sp = r.sp;
}

regs_t get_regs(void) {
  return (regs_t){.pc = s_pc, .a = s_a, .x = s_x, .y = s_y, .status = s_status, .sp = s_sp};
}

unsigned get_cycles(void) {
  return s_cycles;
}

const uint8_t *get_ram(void) {
  return s_ram;
}
static inline void ram_poke_impl(uint16_t addr, uint8_t value) {
  if (g_debug & DebugMem)
    printf("$%04x: $%04x=$%02x\n", s_pc, addr, value);
  s_ram[addr] = value;
}
void ram_poke(uint16_t addr, uint8_t value) {
  ram_poke_impl(addr, value);
}
uint8_t ram_peek(uint16_t addr) {
  return s_ram[addr];
}
uint16_t ram_peek16(uint16_t addr) {
  return ram_peek(addr) + ram_peek(addr + 1) * 256;
}
uint16_t ram_peek16al(uint16_t addr) {
  assert(!(addr & 1) && "ram_peek16al() address must be word aligned");
  return *(uint16_t *)(s_ram + addr);
}

static inline uint8_t peek_zpg(uint8_t addr) {
  return s_ram[addr];
}
static inline void poke_zpg(uint8_t addr, uint8_t value) {
  ram_poke_impl(addr, value);
}
static uint16_t peek16_zpg(uint8_t addr) {
  return peek_zpg(addr) + (peek_zpg(addr + 1) << 8);
}

static uint8_t peek(uint16_t addr) {
  if (addr >= 0xC000 && addr <= 0xCFFF)
    return io_peek(addr);
  return s_ram[addr];
}
static inline void poke(uint16_t addr, uint8_t value) {
  if (addr < 0xC000) {
    ram_poke_impl(addr, value);
  } else if (addr <= 0xCFFF) {
    io_poke(addr, value);
  } else {
    // Ignore writes to ROM.
  }
}
static uint16_t peek16(uint16_t addr) {
  return peek(addr) + (peek(addr + 1) << 8);
}
static uint16_t peek16al(uint16_t addr) {
  assert(!(addr & 1) && "peek16al() address must be word aligned");
  return peek(addr) + (peek(addr + 1) << 8);
}

static inline void push8(uint8_t v) {
  ram_poke_impl(STACK_PAGE_ADDR + s_sp--, v);
}
static inline uint8_t pop8() {
  return s_ram[STACK_PAGE_ADDR + ++s_sp];
}
static void push16(uint16_t v) {
  push8(v >> 8);
  push8(v);
}
static uint16_t pop16() {
  uint8_t tmp = pop8();
  return tmp + (pop8() << 8);
}

/// Update the N and Z flags based on the passed value and return the passed
/// value for convenience.
static uint8_t update_nz(uint8_t v) {
  static_assert(STATUS_N == 0x80, "The sign flag must be 0x80");
  s_status = (s_status & ~(STATUS_N | STATUS_Z)) | (v & STATUS_N) | (v == 0 ? STATUS_Z : 0);
  return v;
}

/// Update the N, Z, C flags.
static uint8_t update_nzc(unsigned v) {
  static_assert(STATUS_N == 0x80, "The sign flag must be 0x80");
  static_assert(STATUS_C == 0x01, "The carry flag must be 0x01");
  static_assert(STATUS_V == 0x40, "The overflow flag must be 0x40");

  s_status = (s_status & ~(STATUS_N | STATUS_Z | STATUS_C)) | (v & STATUS_N) |
      ((uint8_t)v == 0 ? STATUS_Z : 0) | ((v >> 8) & STATUS_C);
  return (uint8_t)v;
}

/// Same as updateNZC(), but C is inverted. This is used after subtractions
/// to reflect the nature of 6502's C flag as true carry and not "borrow".
static uint8_t update_nz_inv_c(unsigned v) {
  return update_nzc(v ^ 0x100);
}

/// Update the N, Z, V and C flags as a result from addition. op1 and op2 are
/// the addition operands, but only their sign bits are examined.
static uint8_t update_nzvc(unsigned v, uint8_t op1, uint8_t op2) {
  static_assert(STATUS_N == 0x80, "The sign flag must be 0x80");
  static_assert(STATUS_C == 0x01, "The carry flag must be 0x01");
  static_assert(STATUS_V == 0x40, "The overflow flag must be 0x40");

  s_status = (s_status & ~(STATUS_N | STATUS_Z | STATUS_C | STATUS_V)) | (v & STATUS_N) |
      ((uint8_t)v == 0 ? STATUS_Z : 0) | ((v >> 8) & STATUS_C) |
      (((~(op1 ^ op2) & (op1 ^ v)) >> 1) & STATUS_V);
  return (uint8_t)v;
}

/// Same as updateNZVC(), but C is inverted. This is used after subtractions
/// to reflect the nature of 6502's C flag as true carry and not "borrow".
static inline uint8_t update_nzv_inv_c(unsigned v, uint8_t op1, uint8_t op2) {
  return update_nzvc(v ^ 0x100, op1, op2);
}

/// Set the carry flag to bit 0 of the specified value.
static inline void set_c_to_bit0(uint8_t value) {
  static_assert(STATUS_C == 1, "Carry flag should be 0x01");
  s_status = (s_status & ~STATUS_C) | (value & STATUS_C);
}

/// Perform A + B + Carry in decimal mode and update the flags.
static uint8_t adc_decimal(uint8_t a, uint8_t b) {
  uint8_t c = s_status & STATUS_C;
  uint8_t al = (a & 15) + (b & 15) + c;
  if (al >= 10)
    al += 6;
  uint8_t ah = (a >> 4) + (b >> 4) + (al > 15);

  s_status &= ~(STATUS_N | STATUS_V | STATUS_Z | STATUS_C);
  if (!(uint8_t)(a + b + c))
    s_status |= STATUS_Z;
  else if (ah & 8)
    s_status |= STATUS_N;

  if (~(a ^ b) & (a ^ (ah << 4)) & 0x80)
    s_status |= STATUS_V;

  if (ah >= 10)
    ah += 6;
  if (ah > 15)
    s_status |= STATUS_C;

  return (ah << 4) | (al & 15);
}

/// Perform A - B - Carry in decimal mode and update the flags.
static uint8_t sbc_decimal(uint8_t a, uint8_t b) {
  uint8_t c = ~s_status & STATUS_C;
  uint8_t al = (a & 15) - (b & 15) - c;
  if ((int8_t)(al) < 0)
    al -= 6;
  uint8_t ah = (a >> 4) - (b >> 4) - ((int8_t)(al) < 0);

  s_status &= ~(STATUS_N | STATUS_V | STATUS_Z | STATUS_C);
  uint16_t diff = a - b - c;
  if (!(uint8_t)diff)
    s_status |= STATUS_Z;
  else if (diff & 0x80)
    s_status |= STATUS_N;

  if ((a ^ b) & (a ^ diff) & 0x80)
    s_status |= STATUS_V;
  if (!(diff & 0xff00))
    s_status |= STATUS_C;
  if ((int8_t)ah < 0)
    ah -= 6;
  return (ah << 4) | (al & 15);
}

void shutdown_emulated(void) {}
