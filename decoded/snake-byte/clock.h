/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The virtual clock: the only way time passes in this program, and the two
/// spellings of a cycle charge that still carry an address.
///
/// Shared by the game and by the Apple II ROM routines beside it, because both
/// have places where a duration is perceptible and both sit on the coroutine
/// that a charge suspends.

#pragma once

#include "apple2tc/system2.h"

/// Advance the virtual clock, at one of the twenty-three places in this
/// program where a duration is *perceptible*.
///
/// This is the only way time passes here, and that is the point. The 6502
/// spent cycles on every instruction it executed, and the decompiled file
/// used to charge for all of them: 838 sites, most of them paying for
/// arithmetic nobody can hear or see. Those are gone. What is left advances
/// the clock for one of two reasons, and each site says which.
///
///   - **It sets a pitch.** The Apple II speaker is a one-bit cone and
///     reading $C030 flips it, so the note *is* the interval between reads.
///     Shorten the interval and the tone goes sharp. The tone loop, the apple
///     sweep, the death buzz, the pace loop's click.
///   - **It is a duration you can see or feel.** The 1.29 s hold on a cleared
///     screen, the 7.8 s prompt timeout, the redefinition cursor's blink --
///     and the snake's own tempo, which is why redrawing the walls still
///     costs 28,848 cycles.
///
/// **Every one of them is also a suspend point, and some are the only one in
/// their loop.** `cycles_expired()` parks the game at a charge and hands the
/// host its window to draw and poll the keyboard. Delete the last charge in a
/// loop that waits for input and the window never opens: the game spins
/// forever with a frozen display and no way to answer.
///
/// That failure is invisible to the entire gate. Under probe-stamped replay
/// the key counter advances at the keyboard *read*, so a spinning loop keeps
/// incrementing it and receives its keys without a frame ever happening --
/// every check passes and the real game hangs. Playing it is a required step,
/// not a courtesy.
///
/// **Calibration.** Each value is the measured cost of the region it stands
/// for: from this site to the next charge, on real 6502 cycles. Two of them
/// are *less* than that measurement, because a region can contain a surviving
/// `GAME_CYCLES_COORD` -- the probe-carrying addresses charge for themselves,
/// and counting them here too made the game 50% slower at the death pause
/// before the block-head trace caught it.
///
/// A macro rather than a comment convention, so that the count is greppable
/// and deleting one is a visible act rather than the removal of an
/// indistinguishable line.
#define advance(n)               \
  do {                           \
    if (s_remaining_cycles <= 0) \
      cycles_expired();          \
    s_cycles += (n);             \
    s_remaining_cycles -= (n);   \
  } while (0)

/*
 * The cycle accounting, and why it is spelled two different ways.
 */

/* --- Cycle accounting ----------------------------------------------------- */

/// Charge cycles and *keep* the probe, for the addresses carrying the input
/// coordinate.
///
/// Replay does not stamp keystrokes on cycles. It stamps them on a counter the
/// probe language increments at named addresses, two of which sit inside
/// routines here -- the game's keyboard poll, and the redefinition screen's.
/// Drop the probe at one of those and the counter stops advancing there, so
/// every key stamped after it arrives at a different moment. The same two
/// addresses are where the screen and memory comparisons take their samples,
/// which is why the whole gate is independent of the clock.
///
/// The local is emulator plumbing: `CYCLES` consults a `branchTarget` flag the
/// generated dispatch used to keep, so that its trace printed one line per
/// block rather than one per instruction. A site spelled this way is a block
/// head by construction, so it answers yes and moves on.
#define GAME_CYCLES_COORD(addr, n) \
  do {                             \
    bool branchTarget = true;      \
    CYCLES((addr), (n));           \
    (void)branchTarget;            \
  } while (0)

/// Charge cycles and keep the probe, because this address is where the
/// cross-build comparison lines its two traces up.
///
/// Exactly one address uses this: the game's entry point. The build being
/// compared against boots the real machine first, and the only way to find
/// where its boot prefix ends is to look for that address in the trace. Spell
/// it as a plain GAME_CYCLES and the alignment silently finds nothing.
#define GAME_CYCLES_ANCHOR(addr, n) GAME_CYCLES_COORD((addr), (n))
