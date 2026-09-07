/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace a2mcp {

/// Install the speaker tap. Must be called after `a2host_init_emulation()`
/// (a2_io_init() clears the callback), so `machine_boot()` calls this last --
/// on first boot and on every reboot, since a reboot re-runs init too.
void sound_install(void);

/// Everything the speaker did since the machine booted or the previous
/// `sound_report()` call, whichever is later: an array of segments, each
/// either a tone (`start_ms`, `duration_ms`, `toggles`, and -- when there is
/// more than one toggle -- `approx_frequency_hz`) or a gap between two tones
/// (`silence_ms`). The speaker is one bit and its cycle-stamped toggle list is
/// the whole signal; this is that list, summarised into what an agent that
/// cannot hear needs to tell "a tone played" from "nothing happened".
///
/// When \p wav_path is non-empty it also renders the same stamps to a WAV
/// file at that path, jailed under the server root for writing.
///
/// Draining: the stamps collected are consumed by this call, so the next
/// `sound_report()` reports only what happened since this one.
nlohmann::json sound_report(const std::string &wav_path);

} // namespace a2mcp
