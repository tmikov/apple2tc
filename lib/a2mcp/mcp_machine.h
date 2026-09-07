/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <nlohmann/json.hpp>

namespace a2mcp {

/// Boot the emulated machine, or reboot it if it is already running. Every
/// path in \p args is jailed to the server root and confirmed to exist before
/// it reaches the engine -- see mcp_paths.h. Throws ToolError on a bad path or
/// argument without ever letting the engine exit() the process.
void machine_boot(const nlohmann::json &args);

/// True once machine_boot() has succeeded at least once.
bool machine_booted(void);

/// The machine's current state. Requires a booted machine -- call_tool() is
/// what enforces that, once, for every tool but `boot`.
nlohmann::json machine_status(void);

/// Advance emulated time by \p args["frames"] frames, or until a probe's
/// `stop` or the engine itself ends the run early, and clearing any `stop` a
/// previous run left set. Throws ToolError on a bad argument.
///
/// \p args["until"] == "screen_update" stops on the \p args["updates"]-th
/// (default 1) moment the displayed picture is *complete*, counting two kinds
/// of moment as the same thing: a page-2 flip (a double-buffered program just
/// presented -- a2host_page_flips() changed during the frame), or a settle (
/// the visible hash changed at some point, then held identical across
/// kSettleFrames consecutive frame boundaries). A settle requires prior
/// activity, so an already-static screen counts as nothing -- the call always
/// means "advance to the next completed picture", never "return immediately
/// because nothing is animating". When both fire on the same frame, only the
/// flip is counted and the settle state is reset, because a flip already is
/// the completion event. If the frame cap is reached with fewer updates than
/// asked for, the reply says so: the screen changed on every frame and no
/// complete-frame boundary was ever found.
nlohmann::json machine_run(const nlohmann::json &args);

/// Schedule the characters of \p args["text"] as keystrokes, spaced
/// \p args["frames_between"] frames apart (default 1). They are not delivered
/// now -- they reach the machine cycle-stamped, during a later machine_run().
/// Throws ToolError on a bad argument, including one whose last key would be
/// stamped past the end of the 32-bit cycle counter.
nlohmann::json machine_keys(const nlohmann::json &args);

} // namespace a2mcp
