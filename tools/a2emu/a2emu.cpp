/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The windowed Apple II emulator.
///
/// Almost nothing lives here. The machine is `lib/a2host`, the window is
/// `lib/a2host_gui`, and executing 6502 is `lib/engine6502` -- the same three
/// pieces `a2run` is built from, differing only in that this one has a window
/// and that one does not. What remains below is genuinely a2emu's own: two
/// function keys that load a bundled game, and two debugger helpers.
///
/// It used to carry its own copy of the frame loop, video, audio, keyboard
/// handling and command line, forked from what became `a2host` and drifted
/// apart -- which is how `--headless` and `--hash-frames` came to exist on only
/// one side, and how run data ended up impossible to regenerate without a
/// display.
///
/// Because the two now share everything but the front end,
/// `a2emu --headless --key-file=K --hash-frames=F` must produce byte-identical
/// output to `a2run` given the same arguments. That is the regression test for
/// this file, and `tests/run-tests.sh` runs it.

#include "apple2tc/a2engine.h"
#include "apple2tc/a2host_api.h"
#include "apple2tc/a2host_gui.h"
#include "apple2tc/d6502.h"
#include "apple2tc/engine6502.h"

#include <cstdio>

#include "bolo.h"
#include "robotron2084.h"

/* --- Debugger helpers ------------------------------------------------------
 *
 * Never called from the program. They exist to be invoked by hand from gdb or
 * lldb while stopped -- `call disasm(0x300)` -- which is why they are free
 * functions with external linkage rather than anything tidier.
 *
 * Both read through `ram_peek` rather than the CPU's `peek`, so dumping the
 * $C0xx range no longer triggers the soft switches it lands on. Inspecting a
 * machine should not alter it.
 * ------------------------------------------------------------------------- */

/// Disassemble 20 instructions from \p pc.
void disasm(uint16_t pc) {
  for (unsigned i = 0; i != 20; ++i) {
    ThreeBytes bytes{0};
    for (unsigned j = 0; j != 3; ++j)
      bytes.d[j] = ram_peek((uint16_t)(pc + j));
    CPUInst inst = decodeInst(pc, bytes);
    FormattedInst fmt = formatInst(inst, bytes);

    printf("%04X: %-8s    %s", pc, fmt.bytes, fmt.inst);
    if (!fmt.operand.empty())
      printf("  %s", fmt.operand.c_str());
    printf("\n");

    pc += inst.size;
  }
}

/// Dump 64 bytes from \p startAddr, rounded down to a 16-byte row.
void printDB(uint16_t startAddr) {
  char asciiBuf[17];
  asciiBuf[16] = 0;
  uint16_t addr = startAddr & ~15;
  uint16_t end = addr + 64;
  while (addr != end) {
    // Offset in a 16 byte row.
    uint16_t ofs = addr & 15;
    // Address.
    if (ofs == 0) {
      printf("%04X:  ", addr);
    }

    // Spaces between values.
    if (ofs == 8)
      printf("  ");
    else if (ofs != 0)
      printf(" ");

    // Actual values.
    if (addr < startAddr) {
      printf("__");
      asciiBuf[ofs] = '.';
    } else {
      uint8_t v = ram_peek(addr);
      printf("%02X", v);
      asciiBuf[ofs] = v >= 32 && v < 128 ? v : '.';
    }

    if (ofs == 15)
      printf("  %s\n", asciiBuf);

    ++addr;
  }
}

/* --- The front end -------------------------------------------------------- */

/// F1 and F2 load a bundled game. Everything else is the host's.
static bool eventHook(const sapp_event *ev) {
  if (ev->type != SAPP_EVENTTYPE_KEY_DOWN)
    return false;
  switch (ev->key_code) {
  case SAPP_KEYCODE_F1:
    engine6502_run_b33(bolo_bin, bolo_bin_len);
    return true;
  case SAPP_KEYCODE_F2:
    engine6502_run_b33(robotron2084_bin, robotron2084_bin_len);
    return true;
  default:
    return false;
  }
}

namespace {
/// Registers the hook before main() runs, so a2host_gui needs no knowledge of
/// which front end it is serving.
struct InstallFrontEnd {
  InstallFrontEnd() {
    a2host_gui_set_event_hook(eventHook);
  }
} s_installFrontEnd;
} // namespace
