/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

enum {
  /// Negative.
  STATUS_N = 0x80,
  /// Overflow.
  STATUS_V = 0x40,
  /// Ignored..
  STATUS_IGNORED = 0x20,
  /// Break.
  STATUS_B = 0x10,
  /// Decimal.
  STATUS_D = 0x08,
  /// Interrupt.
  STATUS_I = 0x04,
  /// Zero.
  STATUS_Z = 0x02,
  /// Carry.
  STATUS_C = 0x01,
};

enum {
  STACK_PAGE_ADDR = 0x100,
};

typedef struct {
  uint16_t pc;
  uint8_t a;
  uint8_t x;
  uint8_t y;
  uint8_t status;
  uint8_t sp;
} regs_t;

enum  {
  DebugASM = 1,
  DebugIO1 = 2,
  DebugIO2 = 4,
  DebugKbdin = 8,
  DebugStdout = 16,
  DebugCountBB = 32,
};

extern uint8_t g_debug;

void reset_regs(void);
void set_regs(regs_t r);
regs_t get_regs(void);
unsigned get_cycles(void);
uint8_t ram_peek(uint16_t addr);
uint16_t ram_peek16(uint16_t addr);

void init_emulated(void);
void run_emulated(void);

uint8_t io_peek(uint16_t addr);
void io_poke(uint16_t addr, uint8_t value);
void debug_asm(uint16_t pc);
void error_handler(uint16_t pc);
