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

/// The IO state — video mode, keyboard queue, disk. Owned here because the host
/// is what renders it, replays keys into it and hashes it.
a2_iostate_t *a2host_io(void);

/* --- Driving the machine -------------------------------------------------- */

/// Run one frame: feed pending keys, run the engine for a frame's worth of
/// cycles, and let the front end finish up.
void a2host_simulate_frame(void);

/// Emit this frame's hash if asked, and advance the frame counter. Returns true
/// when the frame limit is reached.
bool a2host_record_frame(void);

/// True once the engine has asked to stop early — a breakpoint, or a collection
/// limit.
bool a2host_engine_stopped(void);

/// Run to the frame limit with no window, hashing as it goes. Does not return.
void a2host_run_headless(void);

/// Release what the host owns. The front end calls this on the way out.
void a2host_shutdown(void);

/* --- Front-end hooks ------------------------------------------------------ */

/// Seconds since the previous frame. Installed by a front end that paces
/// against a wall clock.
///
/// Consulted **only** when the run is not required to be reproducible. Replay,
/// frame hashing and tracing always use a fixed 1/60 s budget instead, which is
/// what makes those runs deterministic. When no function is installed — the
/// console front end — every frame uses the fixed budget.
typedef double (*a2host_elapsed_fn)(void);
void a2host_set_elapsed_fn(a2host_elapsed_fn fn);

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
