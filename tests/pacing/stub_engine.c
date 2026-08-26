/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The smallest thing a2host will accept as an engine.
///
/// It exists so that the *pacing* can be tested without a window. That is the
/// one part of the host no other test reaches: every check in this repo is
/// fixed-step by construction -- run-tests.sh compares a2emu --headless
/// against a2run, verify.sh and probe-acceptance.sh replay key files -- because
/// reproducibility is what each of them is for. The wall-clock path a person
/// actually plays on had nothing, and was totally broken for nine days before a
/// bug report found it.
///
/// It records the cycle counts it is asked to run and nothing else.

#include "apple2tc/a2engine.h"

#include <string.h>

/// Part of the engine contract, not the host's: a2engine.h declares it extern
/// and every engine defines it.
uint8_t g_debug;

static uint8_t s_ram[0x10000];
static unsigned s_cycles;
static regs_t s_regs;

/// What the test asserts on: every run_emulated() budget, in order.
unsigned stub_run_calls;
unsigned stub_last_cycles;
unsigned stub_total_cycles;

void reset_regs(void) {
  memset(&s_regs, 0, sizeof(s_regs));
}
void set_regs(regs_t r) {
  s_regs = r;
}
regs_t get_regs(void) {
  return s_regs;
}
unsigned get_cycles(void) {
  return s_cycles;
}
const uint8_t *get_ram(void) {
  return s_ram;
}
void ram_poke(uint16_t addr, uint8_t value) {
  s_ram[addr] = value;
}
uint8_t ram_peek(uint16_t addr) {
  return s_ram[addr];
}
uint16_t ram_peek16(uint16_t addr) {
  return (uint16_t)(s_ram[addr] | (s_ram[(uint16_t)(addr + 1)] << 8));
}

void init_emulated(void) {
}

void run_emulated(unsigned run_cycles) {
  ++stub_run_calls;
  stub_last_cycles = run_cycles;
  stub_total_cycles += run_cycles;
  s_cycles += run_cycles;
}

void shutdown_emulated(void) {
}

a2_stop_reason_t engine_stop_reason(void) {
  return A2_STOP_CYCLES;
}

bool engine_parse_arg(const char *arg) {
  (void)arg;
  return false;
}

void engine_print_help(void) {
}
