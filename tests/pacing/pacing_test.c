/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The one test of a2host's wall-clock pacing.
///
/// Everything else in this repo checks a fixed-step run, because everything
/// else exists to be reproducible. This checks the other mode -- the one an
/// interactive window uses -- and its central assertion is the one that was
/// false for nine days: **a repaint that reports elapsed time must run a
/// non-zero number of cycles.**

#include "apple2tc/a2host_api.h"

#include <stdio.h>
#include <stdlib.h>

extern unsigned stub_run_calls;
extern unsigned stub_last_cycles;
extern unsigned stub_total_cycles;

static int failures;

static void check(bool ok, const char *what) {
  printf("%s: %s\n", ok ? "ok  " : "FAIL", what);
  if (!ok)
    ++failures;
}

/// One repaint: hand the host an interval and run whatever it says is due.
static unsigned repaint(double elapsed_sec) {
  const unsigned due = a2host_begin_repaint(elapsed_sec);
  for (unsigned i = 0; i != due; ++i)
    a2host_simulate_frame();
  return due;
}

int main(int argc, char *argv[]) {
  a2host_parse_args(argc, argv);
  a2host_init_emulation();

  /* --- fixed step, which is what every other test in the repo runs -------- */
  check(a2host_fixed_step(), "a front end that has not claimed a wall clock is fixed-step");
  check(a2host_begin_repaint(1.0 / 60.0) == 1, "one frame is due after one 60th of a second");
  check(a2host_begin_repaint(0.0) == 0, "no frame is due when no time has passed");
  check(a2host_begin_repaint(3.0 / 60.0) == 3, "three sixtieths are three frames");
  /* Debt carries rather than being dropped: two half-quanta make one frame. */
  check(a2host_begin_repaint(1.0 / 120.0) == 0, "half a quantum is not yet a frame");
  check(a2host_begin_repaint(1.0 / 120.0) == 1, "the other half completes it");
  /* A stall is capped instead of being paid back all at once. */
  check(a2host_begin_repaint(10.0) == 4, "a long stall catches up by at most four frames");

  /* --- wall clock, which is what a window runs and what broke ------------- */
  a2host_set_wall_clock_pacing();
  check(!a2host_fixed_step(), "claiming a wall clock leaves fixed-step mode");

  /* The first simulate_frame() is a no-op by design -- there is no previous
     frame to have taken any time -- so get past it before measuring. */
  repaint(1.0 / 60.0);
  stub_run_calls = stub_last_cycles = stub_total_cycles = 0;

  check(repaint(1.0 / 60.0) == 1, "a wall-clock repaint runs exactly one frame");
  check(stub_run_calls == 1, "and drives the engine exactly once");

  /* The assertion this whole file exists for. */
  check(stub_last_cycles > 0, "a repaint reporting elapsed time runs a non-zero cycle budget");

  /* And the budget tracks the time reported, rather than being a constant. */
  stub_last_cycles = 0;
  repaint(2.0 / 60.0);
  const unsigned two_sixtieths = stub_last_cycles;
  stub_last_cycles = 0;
  repaint(1.0 / 60.0);
  const unsigned one_sixtieth = stub_last_cycles;
  check(two_sixtieths > one_sixtieth, "twice the elapsed time is a bigger budget");

  /* A repaint that reports no time should run nothing, which is the same
     mechanism working in the other direction. */
  stub_last_cycles = 0;
  repaint(0.0);
  check(stub_last_cycles == 0, "a repaint reporting no elapsed time runs nothing");

  if (failures) {
    printf("\n%d pacing check(s) failed\n", failures);
    return 1;
  }
  printf("\npacing: all checks passed\n");
  return 0;
}
