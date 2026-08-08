/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The **engine** half of the Apple II host/engine contract: what something
/// that executes 6502 must provide, and the host calls.
///
/// The other half is `a2host.h` — what the host provides and the engine calls.
/// `system.h` includes both.
///
/// There are two engines:
///
///   - the C emitted by apple2tc, whose implementation of this interface lives
///     in `system-inc.h` / `system2-inc.h`;
///   - an interpreter over `Emu6502`.
///
/// **Selection is at link time.** No program needs two engines at once, so each
/// executable links the one it wants and the linker resolves these symbols.
/// There is no vtable and no indirection.
///
/// ### The engine may block the host
///
/// `run_emulated()` is synchronous — it returns when the cycle budget is spent
/// — but *how* an engine achieves that is entirely its own business, and the
/// two differ completely. An interpreter simply loops. The generated C cannot,
/// because straight-line C has no way to return from the middle of a routine,
/// so it runs on its own thread and hands control back and forth through a
/// condition variable.
///
/// The host must not care, and must not assume the engine is on its thread. It
/// gets one guarantee, which is enough: **the host is blocked for the entire
/// time the engine runs**, so the `a2host.h` callbacks may touch host state
/// without synchronisation.

#pragma once

#include <stdbool.h>
#include <stdint.h>

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
  DebugCountBB = 2,
  DebugMem = 4,
  // Debug info compatible with the emulator.
  DebugEmu = 8,
};

/// Set by the host from its command line, read by the engine on every block.
/// Defined by the engine rather than the host because the generated code's
/// `CYCLES()` macro tests it in its hottest path.
extern uint8_t g_debug;

void reset_regs(void);
void set_regs(regs_t r);
regs_t get_regs(void);
unsigned get_cycles(void);
const uint8_t * get_ram(void);
void ram_poke(uint16_t addr, uint8_t value);
uint8_t ram_peek(uint16_t addr);
uint16_t ram_peek16(uint16_t addr);

void init_emulated(void);
void run_emulated(unsigned run_cycles);
void shutdown_emulated(void);

/// Why the last run_emulated() returned.
typedef enum {
  /// The cycle budget was spent. The ordinary case, and all the generated
  /// engines ever report.
  A2_STOP_CYCLES,
  /// The engine chose to stop early -- a breakpoint, or a collection limit
  /// reached. The host should stop driving frames.
  A2_STOP_REQUESTED,
} a2_stop_reason_t;

/// Queried by the host after each run_emulated().
///
/// Deliberately a query rather than run_emulated()'s return value: the --irc1
/// engine defines run_emulated() in system2-inc.h, but the --simple-c engine
/// has it *emitted* by the decompiler, so changing its signature would mean
/// regenerating every committed --simple-c output. This costs one call per
/// frame and keeps the contract confined to hand-written headers.
a2_stop_reason_t engine_stop_reason(void);

/// Offered every command-line argument the host does not recognise, in order.
/// Return true if it was consumed.
///
/// This is what lets an engine own its own options -- which ROM, which disk
/// image, which binary to run -- without the host knowing they exist.
bool engine_parse_arg(const char *arg);

/// Print the engine's own options, for --help.
void engine_print_help(void);
