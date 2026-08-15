/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The **host** half of the Apple II host/engine contract: what the machine
/// around the CPU provides, and an engine calls.
///
/// The other half is `a2engine.h` — what an engine provides and the host calls.
/// `system.h` includes both.
///
/// The host owns everything that is not the CPU: the `a2_iostate_t` behind
/// `io_peek`/`io_poke`, the keyboard queue and its replay, the video memory
/// these read and write, frame timing, and the frame-hash oracle. An engine
/// executes 6502 and reaches the machine only through here.
///
/// These are called *from* the engine while the host is blocked inside
/// `run_emulated()`, so they may touch host state without synchronisation —
/// including when the engine runs on its own thread, as the generated C does.
///
/// Note this is the engine-facing surface only. `a2host_api.h` separately
/// declares what the host offers to a *program* embedding it.

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Read the $C000-$CFFF soft-switch range.
uint8_t io_peek(uint16_t addr);
/// Write the $C000-$CFFF soft-switch range.
void io_poke(uint16_t addr, uint8_t value);

/// Called at each basic block when `g_debug` asks for it. See `a2engine.h`.
void debug_asm(uint16_t pc);

/// The engine has reached something it cannot execute — an unknown address, an
/// unimplemented vector target. Reports and does not return.
void error_handler(uint16_t pc);

/// Queue `keys` to be typed into the machine, exactly as `--kbd-file` would
/// feed a file: fed a few at a time as the keyboard queue drains, `\n`
/// translated to Return, `\r` ignored.
///
/// For a program that cannot be used without typing something first. A
/// decompilation traced from the reset vector reproduces the whole boot, so
/// it comes up at whatever prompt the original did, and the recorded session's
/// first keystrokes were the command that started the program. Registering
/// those here makes the built artifact start on its own.
///
/// Call it from `init_emulated()`; `a2host_init_emulation()` looks for queued
/// keys immediately afterwards.
///
/// `--key-file` and `--kbd-file` both take precedence, so replaying a
/// recording is unaffected — the recording already contains those keystrokes,
/// and typing them twice would desynchronise it.
///
/// The string is not copied. Pass a literal or something that outlives the run.
void a2host_buffer_keys(const char *keys);

#ifdef __cplusplus
} // extern "C"
#endif
