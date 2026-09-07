/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <nlohmann/json.hpp>
#include <cstdint>

namespace a2mcp {

/// Bytes in one HGR page -- the unit screen_blend_composite() combines.
constexpr unsigned kHgrPageBytes = 8192;

/// The most frames `screen`'s `blend` may ask for, and the depth of the ring
/// machine_run() keeps.
constexpr unsigned kBlendMaxFrames = 8;

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
///
/// \p args["settle"] is a different policy over the plain "frames" run above,
/// not a new detector: it paces first and lands clean only if that is cheap.
/// After the \p args["frames"] frames run out with nothing else stopping them,
/// up to \p args["settle"] more frames are simulated, stopping at the first
/// one that leaves the displayed picture complete -- a page flip, or the
/// visible hash unchanged across kSettleFrames consecutive frame boundaries.
/// `stop_reason` is "settled" on that success or "unsettled" if the window ran
/// out first (the capture may be torn). Unlike screen_update's own settle, this
/// one drops the "prior activity" requirement on purpose: an already-static
/// screen is exactly the good moment this path exists to recognize, not
/// nothing. `settle` is only meaningful with the default `until: "frames"`;
/// combined with "screen_change" or "screen_update" it throws ToolError rather
/// than raced against a stop condition that already ends the run. Absent
/// `settle`, a run still reports "limit" exactly as before.
///
/// Every frame simulated here -- by the loop above and by `settle`'s grace
/// window -- also feeds the blend ring `screen`'s `blend` reads; see
/// screen_blend_available()/screen_blend_composite() below.
nlohmann::json machine_run(const nlohmann::json &args);

/// Schedule the characters of \p args["text"] as keystrokes, spaced
/// \p args["frames_between"] frames apart (default 1). They are not delivered
/// now -- they reach the machine cycle-stamped, during a later machine_run().
/// Throws ToolError on a bad argument, including one whose last key would be
/// stamped past the end of the 32-bit cycle counter.
nlohmann::json machine_keys(const nlohmann::json &args);

/// How many consecutive frames' HGR page bytes machine_run() currently holds
/// in its blend ring, 0 to kBlendMaxFrames -- what `screen`'s `blend` draws
/// on. The ring captures the displayed page after every simulated frame and
/// is reset (emptied, then built back up frame by frame) on `boot` -- a
/// reboot included -- and whenever machine_run() sees the video mode or the
/// displayed page change, since combining frames across either would OR
/// together bytes that no longer mean the same thing. It also reads 0
/// whenever the current mode is not HGR: there is nothing to blend outside
/// HGR, so nothing is kept.
unsigned screen_blend_available(void);

/// OR the last min(\p n, screen_blend_available()) ring frames together into
/// \p dst (kHgrPageBytes bytes) and return how many frames were actually
/// combined -- which is less than \p n exactly when the ring does not yet
/// hold \p n frames, e.g. right after a reset. OR is commutative and
/// associative, so which end of the ring is read first does not matter.
/// \p n must be between 1 and kBlendMaxFrames.
unsigned screen_blend_composite(unsigned n, uint8_t *dst);

} // namespace a2mcp
