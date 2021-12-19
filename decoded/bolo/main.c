/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/system.h"

#include "apple2tc/a2io.h"

#include <stdio.h>
#include <stdlib.h>

static a2_iostate_t io_;

uint8_t io_peek(uint16_t addr) {
  return a2_io_peek(&io_, addr, get_cycles());
}

void io_poke(uint16_t addr, uint8_t value) {
  a2_io_poke(&io_, addr, value, get_cycles());
}

typedef struct {
  uint16_t addr;
  uint8_t size;
} watch_t;

enum { MAX_WATCHES = 32u };
static watch_t s_watches[MAX_WATCHES];
static unsigned s_num_watches = 0;

static void add_watch(uint16_t addr, uint8_t size) {
  if (s_num_watches < MAX_WATCHES)
    s_watches[s_num_watches++] = (watch_t){.addr = addr, .size = size};
}

static unsigned s_numDebugLines = 0;
enum { MAX_DEBUG_LINES = 2000000 };

void debug_asm(uint16_t pc) {
  if (g_debug & DebugCountBB) {
    if (++s_numDebugLines == MAX_DEBUG_LINES) {
      printf("Reached %u basic blocks\n", s_numDebugLines);
      exit(0);
    }
    return;
  }

  regs_t r = get_regs();
  r.pc = pc;
  printf("%04X: %-8s  ", r.pc, "");

  // Dump the registers.
  printf("A=%02X X=%02X Y=%02X SP=%02X SR=", r.a, r.x, r.y, r.sp);
  // Dump the flags.
  static const char names[9] = "NV.BDIZC";
  for (unsigned i = 0; i != 8; ++i)
    putchar((r.status & (0x80 >> i)) ? names[i] : '.');

  if (s_num_watches == 0) {
    // The PC again for convenience.
    printf(" PC=%04X", r.pc);
  }

  for (unsigned i = 0; i != s_num_watches; ++i) {
    watch_t watch = s_watches[i];
    putchar(' ');
    if (watch.addr < 256)
      printf("($%02X)=", watch.addr);
    else
      printf("($%04X)=", watch.addr);
    if (watch.size == 1)
      printf("$%02X", ram_peek(watch.addr));
    else
      printf("$%04X", ram_peek16(watch.addr));
  }
  putchar('\n');

  if (++s_numDebugLines == MAX_DEBUG_LINES) {
    fflush(stdout);
    exit(0);
  }
}

void error_handler(uint16_t pc) {
  printf("BB count=%u\n", s_numDebugLines);
  exit(1);
}

int main(int argc, char **argv) {
  a2_io_init(&io_);
  g_debug = DebugASM | DebugCountBB;
  reset_regs();
  set_regs((regs_t){.pc = 0, .a = 0xa0, .x = 0, .y = 1, .sp = 0xf0, .status = STATUS_IGNORED});

  init_emulated();
  for (;;)
    run_emulated(~0u);

  a2_io_done(&io_);
  return 0;
}