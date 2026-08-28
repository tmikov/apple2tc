/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The emulated 6502's definitions: its storage, and every function that is not
/// a one-line accessor.
///
/// **A fragment, not a translation unit.** It does not include `system2.h`;
/// whoever includes this includes that first:
///
///     #include "apple2tc/system2.h"
///     #include "apple2tc/system2-impl.inc"
///
/// Exactly one translation unit per program may do so, or the definitions
/// appear twice and the link fails. A single-file program gets both at once by
/// including `system2-inc.h` instead.

uint16_t s_pc = 0;
uint8_t s_a = 0;
uint8_t s_x = 0;
uint8_t s_y = 0;
uint8_t s_sp = 0;
/// NEGATIVE.
uint8_t s_status_n = 0;
/// OVERFLOW.
uint8_t s_status_v = 0;
/// BREAK.
uint8_t s_status_b = 0;
/// DECIMAL.
uint8_t s_status_d = 0;
/// INTERRUPT.
uint8_t s_status_i = 0;
/// ZERO.
uint8_t s_status_not_z = 1;
/// CARRY.
uint8_t s_status_c = 0;

uint8_t s_ram[0x10000];

unsigned s_cycles = 0;
uint8_t g_debug = 0;

void reset_regs(void) {
  memset(s_ram, 0xFF, 0x10000);
  s_pc = 0;
  s_a = 0;
  s_x = 0;
  s_y = 0;
  s_status_n = 0;
  s_status_v = 0;
  s_status_b = 0;
  s_status_d = 0;
  s_status_i = 0;
  s_status_not_z = 1;
  s_status_c = 0;
  s_sp = 0xFF;
}

void set_regs(regs_t r) {
  s_pc = r.pc;
  s_a = r.a;
  s_x = r.x;
  s_y = r.y;
  s_status_n = r.status & 0x80;
  s_status_v = (r.status >> 6) & 1;
  s_status_b = (r.status >> 4) & 1;
  s_status_d = (r.status >> 3) & 1;
  s_status_i = (r.status >> 2) & 1;
  s_status_not_z = ~r.status & 2;
  s_status_c = r.status & 1;
  s_sp = r.sp;
}

regs_t get_regs(void) {
  return (regs_t){
      .pc = s_pc,
      .a = s_a,
      .x = s_x,
      .y = s_y,
      .status = s_status_n | (s_status_v << 6) | (s_status_b << 4) | (s_status_d << 3) |
          (s_status_i << 2) | ((!s_status_not_z) << 1) | s_status_c,
      .sp = s_sp};
}

unsigned get_cycles(void) {
  return s_cycles;
}

const uint8_t *get_ram(void) {
  return s_ram;
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

/// Perform A + B + Carry in decimal mode and update the flags.
struct ResAndStatus adc_decimal(uint8_t a, uint8_t b, uint8_t cf) {
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
struct ResAndStatus sbc_decimal(uint8_t a, uint8_t b, uint8_t cf) {
  cf = ~cf & STATUS_C;
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

bool s_initialized = false;
thrd_t s_emu_thread;
cnd_t s_emu_cond;
mtx_t s_emu_mutex;
int s_emu_enabled = 0; // 0: disabled. 1: enabled. -1: stop.

int s_remaining_cycles;

static int emulated_thread(void *arg) {
  mtx_lock(&s_emu_mutex);
  while (!s_emu_enabled)
    cnd_wait(&s_emu_cond, &s_emu_mutex);

  if (s_emu_enabled > 0)
    emulated_entry_point();
  mtx_unlock(&s_emu_mutex);
  return 0;
}

void cycles_expired(void) {
  s_emu_enabled = false;
  cnd_signal(&s_emu_cond);
  while (!s_emu_enabled)
    cnd_wait(&s_emu_cond, &s_emu_mutex);
  if (s_emu_enabled < 0) {
    mtx_unlock(&s_emu_mutex);
    thrd_exit(1);
  }
}

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

/* --- a2engine.h hooks. The generated engines have no options of their own and
   never stop early; they exist so the host can be written against one
   contract. --- */
a2_stop_reason_t engine_stop_reason(void) {
  return A2_STOP_CYCLES;
}
bool engine_parse_arg(const char *arg) {
  (void)arg;
  return false;
}
void engine_print_help(void) {
}
