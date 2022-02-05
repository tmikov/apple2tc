/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/system.h"

#include "c11threads/c11threads.h"

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

static uint8_t s_ram[0x10000];

static unsigned s_cycles = 0;
uint8_t g_debug = 0;

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

struct ResAndStatus {
  uint8_t result;
  uint8_t status;
};

/// Perform A + B + Carry in decimal mode and update the flags.
static struct ResAndStatus adc_decimal(uint8_t a, uint8_t b, uint8_t cf) {
  uint8_t al = (a & 15) + (b & 15) + cf;
  if (al >= 10)
    al += 6;
  uint8_t ah = (a >> 4) + (b >> 4) + (al > 15);

  uint8_t status = 0;
  if (!(uint8_t)(a + b + cf))
    status |= STATUS_Z;
  else if (ah & 8)
    status |= STATUS_N;

  if (~(a ^ b) & (a ^ (ah << 4)) & 0x80)
    status |= STATUS_V;

  if (ah >= 10)
    ah += 6;
  if (ah > 15)
    status |= STATUS_C;

  return (struct ResAndStatus){.result = (ah << 4) | (al & 15), .status = status};
}

/// Perform A - B - Carry in decimal mode and update the flags.
static struct ResAndStatus sbc_decimal(uint8_t a, uint8_t b, uint8_t cf) {
  uint8_t al = (a & 15) - (b & 15) - cf;
  if ((int8_t)(al) < 0)
    al -= 6;
  uint8_t ah = (a >> 4) - (b >> 4) - ((int8_t)(al) < 0);

  uint8_t status = 0;
  uint16_t diff = a - b - cf;
  if (!(uint8_t)diff)
    status |= STATUS_Z;
  else if (diff & 0x80)
    status |= STATUS_N;

  if ((a ^ b) & (a ^ diff) & 0x80)
    status |= STATUS_V;
  if (!(diff & 0xff00))
    status |= STATUS_C;
  if ((int8_t)ah < 0)
    ah -= 6;
  return (struct ResAndStatus){.result = (ah << 4) | (al & 15), .status = status};
}

static bool s_initialized = false;
static thrd_t s_emu_thread;
static cnd_t s_emu_cond;
static mtx_t s_emu_mutex;
static int s_emu_enabled = 0; // 0: disabled. 1: enabled. -1: stop.

static int s_remaining_cycles;

static void emulated_entry_point(void);

static int emulated_thread(void *arg) {
  mtx_lock(&s_emu_mutex);
  while (!s_emu_enabled)
    cnd_wait(&s_emu_cond, &s_emu_mutex);

  if (s_emu_enabled > 0)
    emulated_entry_point();
  mtx_unlock(&s_emu_mutex);
  return 0;
}

static void cycles_expired() {
  s_emu_enabled = false;
  cnd_signal(&s_emu_cond);
  while (!s_emu_enabled)
    cnd_wait(&s_emu_cond, &s_emu_mutex);
  if (s_emu_enabled < 0) {
    mtx_unlock(&s_emu_mutex);
    thrd_exit(1);
  }
}

#define CYCLES(pc, cycles)                      \
  do {                                          \
    s_cycles += (cycles);                       \
    if ((g_debug & DebugASM) && branchTarget) { \
      branchTarget = false;                     \
      debug_asm(pc);                            \
    }                                           \
    if ((s_remaining_cycles -= (cycles)) <= 0)  \
      cycles_expired();                         \
  } while (0)

void run_emulated(unsigned run_cycles) {
  if (!s_initialized) {
    s_initialized = true;
    s_emu_enabled = false;
    s_cycles = 0;
    s_remaining_cycles = 0;

    if (cnd_init(&s_emu_cond) != thrd_success)
      abort();
    if (mtx_init(&s_emu_mutex, mtx_plain) != thrd_success)
      abort();
    if (thrd_create(&s_emu_thread, emulated_thread, NULL) != thrd_success)
      abort();
  }
  assert((int)run_cycles >= 0 && "run_cycles must be a non-negative int");
  if (run_cycles == 0)
    return;

  mtx_lock(&s_emu_mutex);
  assert(!s_emu_enabled);

  s_remaining_cycles += (int)run_cycles;
  if (s_remaining_cycles <= 0) {
    mtx_unlock(&s_emu_mutex);
    return;
  }

  s_emu_enabled = true;
  cnd_signal(&s_emu_cond);
  while (s_emu_enabled)
    cnd_wait(&s_emu_cond, &s_emu_mutex);
  mtx_unlock(&s_emu_mutex);
}

void shutdown_emulated(void) {
  if (!s_initialized)
    return; // Nothing to do.
  mtx_lock(&s_emu_mutex);
  s_emu_enabled = -1;
  cnd_signal(&s_emu_cond);
  mtx_unlock(&s_emu_mutex);
  thrd_join(s_emu_thread, NULL);
}
