/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The emulated 6502 the --irc1 back end targets: its state, its accessors, and
/// declarations of everything else.
///
/// Any number of translation units may include this. The other half --
/// `system2-impl.inc` -- holds the definitions and must be compiled exactly
/// once per program, by a .c that includes this header and then that fragment.
/// `system2-inc.h` is exactly such a .c, for programs that are a single
/// translation unit and want to stay that way.
///
/// The accessors live here, and inline, because they are accessors: one to
/// three lines over the state below. Each translation unit gets its own copy of
/// the code and they all share one machine.

#pragma once

#include "apple2tc/probe.h"
#include "apple2tc/system.h"

#include "c11threads/c11threads.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint16_t s_pc;
extern uint8_t s_a;
extern uint8_t s_x;
extern uint8_t s_y;
extern uint8_t s_sp;
/// NEGATIVE.
extern uint8_t s_status_n;
/// OVERFLOW.
extern uint8_t s_status_v;
/// BREAK.
extern uint8_t s_status_b;
/// DECIMAL.
extern uint8_t s_status_d;
/// INTERRUPT.
extern uint8_t s_status_i;
/// ZERO.
extern uint8_t s_status_not_z;
/// CARRY.
extern uint8_t s_status_c;

extern uint8_t s_ram[0x10000];

extern unsigned s_cycles;

extern bool s_initialized;
extern thrd_t s_emu_thread;
extern cnd_t s_emu_cond;
extern mtx_t s_emu_mutex;
extern int s_emu_enabled; ///< 0: disabled. 1: enabled. -1: stop.

extern int s_remaining_cycles;

struct ResAndStatus {
  uint8_t result;
  uint8_t status;
};

static inline void ram_poke_impl(uint16_t addr, uint8_t value) {
  if (g_debug & DebugMem)
    printf("%8u $%04x: $%04x=$%02x\n", s_cycles, s_pc, addr, value);
  s_ram[addr] = value;
}
static inline uint8_t peek_zpg(uint8_t addr) {
  return s_ram[addr];
}
static inline void poke_zpg(uint8_t addr, uint8_t value) {
  ram_poke_impl(addr, value);
}
static inline uint16_t peek16_zpg(uint8_t addr) {
  return peek_zpg(addr) + (peek_zpg(addr + 1) << 8);
}

static inline uint8_t peek(uint16_t addr) {
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
static inline uint16_t peek16(uint16_t addr) {
  return peek(addr) + (peek(addr + 1) << 8);
}
static inline uint16_t peek16al(uint16_t addr) {
  assert(!(addr & 1) && "peek16al() address must be word aligned");
  return peek(addr) + (peek(addr + 1) << 8);
}

static inline void push8(uint8_t v) {
  ram_poke_impl(STACK_PAGE_ADDR + s_sp--, v);
}
static inline uint8_t pop8() {
  return s_ram[STACK_PAGE_ADDR + ++s_sp];
}
static inline void push16(uint16_t v) {
  push8(v >> 8);
  push8(v);
}
static inline uint16_t pop16() {
  uint8_t tmp = pop8();
  return tmp + (pop8() << 8);
}

/* Defined in system2-impl.inc. */
void reset_regs(void);
void set_regs(regs_t r);
regs_t get_regs(void);
unsigned get_cycles(void);
const uint8_t *get_ram(void);
void ram_poke(uint16_t addr, uint8_t value);
uint8_t ram_peek(uint16_t addr);
uint16_t ram_peek16(uint16_t addr);
uint16_t ram_peek16al(uint16_t addr);
struct ResAndStatus adc_decimal(uint8_t a, uint8_t b, uint8_t cf);
struct ResAndStatus sbc_decimal(uint8_t a, uint8_t b, uint8_t cf);
void cycles_expired(void);
void run_emulated(unsigned run_cycles);
void shutdown_emulated(void);
a2_stop_reason_t engine_stop_reason(void);
bool engine_parse_arg(const char *arg);
void engine_print_help(void);

/* Supplied by the decompiled program; called by the emulation thread. */
void emulated_entry_point(void);

#define CYCLES(pc, cycles)                                                 \
  do {                                                                     \
    s_pc = (pc);                                                           \
    if (s_remaining_cycles <= 0)                                           \
      cycles_expired();                                                    \
    s_cycles += (cycles);                                                  \
    s_remaining_cycles -= (cycles);                                        \
    if ((g_debug & DebugASM) && (!(g_debug & DebugEmu) || branchTarget)) { \
      branchTarget = false;                                                \
      debug_asm(pc);                                                       \
    }                                                                      \
    /* At the end, not beside debug_asm above, and not gated by g_debug:   \
       the probe must observe the block's entry state, which is fixed by   \
       now, and probes are deliberately not a debug feature -- they must   \
       fire even when tracing is off, since add_default_nondebug() blanks  \
       exactly the ranges (e.g. $FCA8-$FCB3, $FD0C-$FD3C) a keyboard       \
       probe wants to sit in. Placing this after cycles_expired() (above)  \
       is safe, not "on the wrong side of a yield": cycles_expired() only  \
       returns once the host has re-armed s_remaining_cycles and handed    \
       control back, at which point this thread again holds s_emu_mutex    \
       exclusively -- the host is parked in cnd_wait, touching nothing --  \
       until the *next* cycles_expired() or shutdown_emulated(). */        \
    if (g_probe_sites)                                                     \
      probe_dispatch(pc);                                                  \
  } while (0)

/* The taken-branch penalty, charged on the CFG edge that owes it rather than
   at a program location -- see AddEdgeCycles in tools/apple2tc/ir/Values.def.
   Deliberately neither traces nor dispatches probes: pc here is the address of
   a branch that the block ending in it already reported, so doing either would
   observe a single execution of that branch twice.

   It does keep s_pc and the cycles_expired() yield, unlike the system-inc.h
   version, which has neither to keep. Dropping the yield here would be a
   behaviour change beyond the two observation facilities -- the host would
   regain control one edge later -- and this macro exists to remove exactly
   those two things and nothing else. */
#define CYCLES_EDGE(pc, cycles)     \
  do {                              \
    s_pc = (pc);                    \
    if (s_remaining_cycles <= 0)    \
      cycles_expired();             \
    s_cycles += (cycles);           \
    s_remaining_cycles -= (cycles); \
  } while (0)

