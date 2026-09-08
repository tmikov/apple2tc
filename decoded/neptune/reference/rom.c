/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The Apple II ROM entry points Neptune calls, written by hand instead of
/// decompiled. There is exactly one.
///
/// This file is not compiled on its own -- see neptune-ext.c. It needs the
/// emulated machine that `system2-inc.h` *defines* with internal linkage
/// (`s_a`, `s_status_*`, `push8`, `pop8`, the `CYCLES` macros) and the
/// generated static helpers `ovf8()` and `sbc_dec16()`.

/// $FCA8 MON_WAIT -- the monitor's delay loop, and Neptune's only call into
/// ROM. Twelve call sites; step 1's boundary measurement found no others.
///
/// The 6502 it replaces:
///
///     FCA8: SEC
///     FCA9: PHA           <- outer loop top
///     FCAA: SBC #$01      <- inner loop top
///     FCAC: BNE $FCAA
///     FCAE: PLA
///     FCAF: SBC #$01
///     FCB1: BNE $FCA9
///     FCB3: RTS
///
/// It spins for (26 + 27*A + 5*A^2)/2 cycles and returns with A = 0.
///
/// ### This has to stay cycle-exact, and the loop is why
///
/// The obvious "readable" version computes that closed form and charges it in
/// one go. That would be wrong, and not subtly: `CYCLES` yields to the host
/// through `cycles_expired()` whenever the budget runs out, so *when* cycles
/// are charged decides where a frame boundary falls. Charging 20,000 at once
/// overshoots the boundary by a different amount than charging 4 at a time,
/// which moves every subsequent frame hash. The gate in scripts/verify.sh
/// compares cycle counts precisely so that this cannot be got wrong quietly.
///
/// So the structure below is the 6502's, with the same `CYCLES` values at the
/// same addresses. `CYCLES_EDGE` is the taken-branch penalty and deliberately
/// does not dispatch probes; `CYCLES` does. Keeping both exactly as the
/// generated code had them is what makes the two builds comparable per-site
/// and not merely per-frame.
///
/// ### Why the decimal arms are here
///
/// `SBC` behaves differently with D set, and nothing establishes that D is
/// clear at all twelve call sites -- the gate reaches only some of them. The
/// arms are transcribed rather than argued away. The inner `SBC` sets only C
/// and Z because N and V are dead before the next write; the outer one sets
/// N and V too, because they survive to the `RTS`. That asymmetry is the
/// decompiler's dead-code elimination, and it is preserved rather than
/// tidied: making the inner arm "consistent" would be a behaviour change.
void rom_mon_wait(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t t16;
  uint8_t t8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FCA8*/ CYCLES(0xfca8, 2);
  s_status_c = 0x01;

  for (;;) {
    /*$FCA9*/ CYCLES(0xfca9, 3);
    push8(s_a);

    for (;;) {
      /*$FCAA*/ CYCLES(0xfcaa, 4);
      if (!s_status_d) {
        t16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
        s_status_c = (uint8_t)(0x01 - ((uint8_t)(t16 >> 8) & 0x01));
        t8 = (uint8_t)t16;
        s_status_not_z = t8;
        s_a = t8;
      } else {
        t16 = sbc_dec16(s_a, 0x01, s_status_c);
        s_a = (uint8_t)t16;
        t8 = (uint8_t)(t16 >> 8);
        s_status_c = (t8 & 0x01);
        s_status_not_z = (~t8 & 2);
      }
      if (!s_status_not_z)
        break;
      /*$FCAC*/ CYCLES_EDGE(0xfcac, 1);
      branchTarget = true;
    }

    /*$FCAE*/ CYCLES(0xfcae, 8);
    s_a = pop8();
    if (!s_status_d) {
      uint16_t a16 = s_a;
      t16 = (a16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
      s_status_c = (uint8_t)(0x01 - ((uint8_t)(t16 >> 8) & 0x01));
      s_status_v = ovf8((uint8_t)t16, (uint8_t)a16, (uint8_t)0xfffe);
      t8 = (uint8_t)t16;
      s_status_not_z = t8;
      s_status_n = (t8 & 0x80);
      s_a = t8;
    } else {
      t16 = sbc_dec16(s_a, 0x01, s_status_c);
      s_a = (uint8_t)t16;
      t8 = (uint8_t)(t16 >> 8);
      s_status_c = (t8 & 0x01);
      s_status_not_z = (~t8 & 2);
      s_status_v = ((t8 & 0x40) != 0);
      s_status_n = (t8 & 0x80);
    }
    if (!s_status_not_z)
      break;
    /*$FCB1*/ CYCLES_EDGE(0xfcb1, 1);
    branchTarget = true;
  }

  /*$FCB3*/ CYCLES(0xfcb3, 6);
  if (ret_addr)
    pop16();
}
