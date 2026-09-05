/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The host library's own API, for a **front end** — the thing that owns
/// `main` and decides how the machine is presented.
///
/// Not to be confused with the two contract headers:
///
///   - `a2engine.h`   what an engine provides, and the host calls.
///   - `a2host.h`     what the host provides, and an *engine* calls.
///   - `a2host_api.h` (this) what the host provides, and a *front end* calls.
///
/// There are two front ends, and they are separate executables rather than one
/// binary with a flag, because on Windows console versus GUI is a link-time
/// subsystem property that no runtime flag can bridge:
///
///   - `a2host_gui`, a sokol window;
///   - a console `main` that calls `a2host_parse_args` then
///     `a2host_run_headless`.
///
/// Everything here is sokol-free. That is what allows the console front end to
/// avoid linking a graphics stack at all.

#pragma once

#include "apple2tc/a2io.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- Setup ---------------------------------------------------------------- */

/// Parse the options this library owns. Anything unrecognised is offered to the
/// engine via `engine_parse_arg`, then rejected.
void a2host_parse_args(int argc, char *argv[]);

/// Bring up the emulated machine: IO, registers, and the engine's own
/// initialisation. No graphics or audio.
void a2host_init_emulation(void);

/// Declare that this front end supplies keys programmatically, the way
/// --key-file= does. Must be called before a2host_init_emulation(): it is part
/// of the condition for the dummy keypress that init pushes, and a front end
/// whose session is meant to be replayable with --key-file= must take that
/// keypress too, or the replay sees one keystroke the original did not.
void a2host_enable_scheduled_keys(void);

/// Tear the emulated machine down and bring it back up, as if the process had
/// just started. Frame and cycle counters restart at zero. Probe counters and
/// any open report file deliberately survive, because a probe script's state is
/// the run's, not the machine's.
void a2host_reboot(void);

/// The IO state — video mode, keyboard queue, disk. Owned here because the host
/// is what renders it, replays keys into it and hashes it.
a2_iostate_t *a2host_io(void);

/* --- Driving the machine -------------------------------------------------- */

/// Run one frame: feed pending keys, run the engine for a frame's worth of
/// cycles, and let the front end finish up.
void a2host_simulate_frame(void);

/// True when a2host_simulate_frame() advances the machine by a fixed 1/60 s of
/// emulated time rather than by however much wall clock has passed.
///
/// A front end does not need to consult this: a2host_begin_repaint() already
/// does, and returns the right number of frames for either mode. It is exposed
/// because tests assert on it.
bool a2host_fixed_step(void);

/// Declare that this front end paces against a wall clock, i.e. that it has a
/// real display and no obligation to be reproducible. The console front end
/// never calls this, so a headless run is always fixed-step.
void a2host_set_wall_clock_pacing(void);

/// Start a repaint. \p elapsed_sec is the wall-clock time since the previous
/// call. Returns how many times to call a2host_simulate_frame() now.
///
/// The elapsed time is *pushed*, once, at the moment the front end knows it --
/// it used to be a callback the host invoked from inside simulate_frame, and
/// that cost nine days: the front end advanced its own "last repaint" marker
/// before the loop, so the callback measured zero, every frame ran zero cycles
/// and every windowed build was frozen. Nothing the caller does after this
/// returns can affect the value now.
///
/// In wall-clock mode the answer is always 1, because simulate_frame sizes the
/// frame from \p elapsed_sec and a faster monitor just means smaller slices.
/// In fixed-step mode -- replay, frame hashing, tracing -- each frame is a
/// fixed 1/60 s of emulated time, so calling once per repaint would tie the
/// machine's speed to the refresh rate (2.4x on a 144 Hz display). There the
/// answer is however many whole quanta the clock says are due.
unsigned a2host_begin_repaint(double elapsed_sec);

/// Emit this frame's hash if asked, and advance the frame counter. Returns true
/// when the frame limit is reached.
bool a2host_record_frame(void);

/// Frames simulated so far.
unsigned a2host_frame_no(void);

/// True once the engine has asked to stop early — a breakpoint, or a collection
/// limit.
bool a2host_engine_stopped(void);

/// Run to the frame limit with no window, hashing as it goes. Does not return.
void a2host_run_headless(void);

/// Release what the host owns. The front end calls this on the way out.
void a2host_shutdown(void);

/* --- Front-end hooks ------------------------------------------------------ */

/// Called at the end of each simulated frame, after the engine has run.
/// The GUI uses it to submit audio.
typedef void (*a2host_frame_done_fn)(void);
void a2host_set_frame_done_fn(a2host_frame_done_fn fn);

/* --- Queries a front end needs -------------------------------------------- */

/// argv[0] with any directory stripped; the GUI titles its window with it.
const char *a2host_argv0(void);

bool a2host_headless(void);
bool a2host_sound_enabled(void);
/// True while a cycle-stamped key recording is driving input, in which case the
/// front end must ignore real keyboard events.
bool a2host_key_replay_active(void);
/// Top up the keyboard queue from `--kbd-file`, if one is open.
void a2host_drain_kbd_file(void);
/// Queue a key, but only if nothing is pending.
void a2host_push_key_if_empty(uint8_t ch);

/* --- Debug plumbing, called by generated programs -------------------------- */

void add_watch(uint16_t addr, uint8_t size);
void add_nondebug(uint16_t from, uint16_t to);
void add_default_nondebug(void);

#ifdef __cplusplus
} // extern "C"
#endif
