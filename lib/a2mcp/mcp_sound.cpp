/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_sound.h"
#include "mcp_paths.h"
#include "mcp_server.h"

// Same reasoning as mcp_machine.cpp/mcp_screen.cpp: a2io.h pulls in
// soundqueue.h, which uses <atomic> in its C++ branch, so it must not be
// wrapped in extern "C" here.
#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"
#include "apple2tc/apple2iodefs.h"

#include <cstdint>
#include <cstdio>
#include <vector>

namespace a2mcp {
namespace {

std::vector<unsigned> s_stamps;

void spkr_cb(void *ctx, unsigned cycles) {
  (void)ctx;
  // Bounded so a long unattended run cannot grow without limit; the oldest
  // stamps are the ones the agent has already had a chance to ask about.
  if (s_stamps.size() >= 4u * 1024 * 1024)
    s_stamps.erase(s_stamps.begin(), s_stamps.begin() + s_stamps.size() / 2);
  s_stamps.push_back(cycles);
}

/// The stamps as a 44.1 kHz mono 8-bit WAV: walk them, flipping the level at
/// each one, behind a 44-byte RIFF header. Returned rather than written,
/// because writing a file under the server root is write_jailed_file()'s job
/// and there is exactly one of those. Assumes a little-endian host, as the
/// rest of the repo does.
std::string build_wav(double cpu_freq) {
  const unsigned kRate = 44100;
  std::string pcm;
  if (!s_stamps.empty()) {
    const unsigned total = (unsigned)((s_stamps.back() - s_stamps.front()) / cpu_freq * kRate) + 1;
    pcm.reserve(total);
    uint8_t level = 0x40;
    size_t next = 0;
    for (unsigned n = 0; n != total; ++n) {
      const double at = s_stamps.front() + n * cpu_freq / kRate;
      while (next != s_stamps.size() && s_stamps[next] <= at) {
        level = level == 0x40 ? 0xC0 : 0x40;
        ++next;
      }
      pcm.push_back((char)level);
    }
  }

  std::string wav;
  auto raw = [&wav](const void *p, size_t n) { wav.append((const char *)p, n); };
  auto u32 = [&raw](uint32_t v) { raw(&v, 4); };
  auto u16 = [&raw](uint16_t v) { raw(&v, 2); };
  wav.append("RIFF", 4);
  u32((uint32_t)(36 + pcm.size()));
  wav.append("WAVEfmt ", 8);
  u32(16); // PCM header size
  u16(1); // PCM
  u16(1); // mono
  u32(kRate);
  u32(kRate); // byte rate: 8-bit mono
  u16(1); // block align
  u16(8); // bits per sample
  wav.append("data", 4);
  u32((uint32_t)pcm.size());
  wav += pcm;
  return wav;
}

} // namespace

void sound_install(void) {
  s_stamps.clear();
  a2_io_set_spkr_cb(a2host_io(), nullptr, spkr_cb);
}

nlohmann::json sound_report(const std::string &wav_path) {
  // The same clock the rest of the host times cycles against (a2host_gui.c
  // uses it for real-time playback too), so a stamp's cycle count and its
  // millisecond timestamp here agree with everywhere else in the codebase.
  const double freq = A2_CLOCK_FREQ;
  auto ms = [freq](unsigned cycles) { return cycles * 1000.0 / freq; };

  // A gap longer than this ends a segment. 20 ms is below the slowest tone a
  // program can hold (50 Hz, a 10 ms half-period) and far above the gap
  // inside any real one, so the split lands on silences and not between wave
  // periods.
  const double kGapMs = 20.0;

  nlohmann::json segments = nlohmann::json::array();
  size_t i = 0;
  while (i < s_stamps.size()) {
    size_t j = i + 1;
    while (j < s_stamps.size() && ms(s_stamps[j] - s_stamps[j - 1]) < kGapMs)
      ++j;
    const unsigned span = s_stamps[j - 1] - s_stamps[i];
    const size_t toggles = j - i;
    nlohmann::json seg = {
        {"start_ms", ms(s_stamps[i] - s_stamps.front())},
        {"duration_ms", ms(span)},
        {"toggles", toggles}};
    // Two toggles make one full square-wave period, so the frequency is the
    // toggle rate halved. One lone toggle is a click with no frequency.
    if (toggles > 1 && span)
      seg["approx_frequency_hz"] = (toggles - 1) * freq / (2.0 * span);
    segments.push_back(seg);
    if (j < s_stamps.size())
      segments.push_back({{"silence_ms", ms(s_stamps[j] - s_stamps[j - 1])}});
    i = j;
  }

  if (!wav_path.empty())
    write_jailed_file(wav_path, build_wav(freq));

  nlohmann::json out = {{"segments", segments}};
  s_stamps.clear();
  return out;
}

} // namespace a2mcp
