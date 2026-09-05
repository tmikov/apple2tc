/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_machine.h"
#include "mcp_paths.h"
#include "mcp_server.h"

// Each header brackets its own declarations in `extern "C"` under
// `#ifdef __cplusplus`, so no wrapper is needed here -- and adding one would
// be actively wrong: a2io.h pulls in soundqueue.h, which uses <atomic> in its
// C++ branch, and C++ standard headers do not tolerate C linkage.
#include "apple2tc/a2engine.h"
#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"

#include <vector>

namespace a2mcp {
namespace {

bool s_booted = false;

/// Forward one engine option, validating the path first: init_emulated() uses
/// readFileOrDie() and mountDisk() exits on a bad image, and neither may be
/// reached from inside a tool call.
void forward_path(const nlohmann::json &args, const char *key, const char *opt) {
  auto it = args.find(key);
  if (it == args.end())
    return;
  if (!it->is_string())
    throw ToolError(std::string(key) + " must be a string");
  const std::string arg = std::string(opt) + jail_path(it->get<std::string>(), false);
  if (!engine_parse_arg(arg.c_str()))
    throw ToolError(std::string("the engine rejected ") + opt);
}

} // namespace

bool machine_booted(void) {
  return s_booted;
}

void machine_boot(const nlohmann::json &args) {
  forward_path(args, "rom", "--rom=");
  forward_path(args, "disk1", "--disk1=");
  forward_path(args, "disk2", "--disk2=");
  // The engine takes the program to run as a bare argument.
  auto bin = args.find("binary");
  if (bin != args.end()) {
    if (!bin->is_string())
      throw ToolError("binary must be a string");
    const std::string path = jail_path(bin->get<std::string>(), false);
    if (!engine_parse_arg(path.c_str()))
      throw ToolError("the engine rejected the binary");
  }

  if (s_booted) {
    a2host_reboot();
  } else {
    a2host_enable_scheduled_keys();
    a2host_init_emulation();
    s_booted = true;
  }
}

nlohmann::json machine_status(void) {
  if (!s_booted)
    throw ToolError("not booted: call boot first");
  const a2_iostate_t *io = a2host_io();
  const char *mode = a2_io_get_vidmode(io) == A2_VIDMODE_TEXT ? "text"
      : a2_io_get_vidmode(io) == A2_VIDMODE_GR                ? "gr"
                                                              : "hgr";
  return {
      {"frame", a2host_frame_no()},
      {"cycles", get_cycles()},
      {"video",
       {{"mode", mode},
        {"mixed", a2_io_is_vidmode_mixed(io)},
        {"page", a2_io_get_vidpage_index(io)}}},
      {"keys_pending", a2_io_keys_count(io)},
      {"keys_scheduled", a2host_scheduled_keys_pending()},
      {"stopped", a2host_engine_stopped()}};
}

nlohmann::json machine_run(const nlohmann::json &args) {
  if (!machine_booted())
    throw ToolError("not booted: call boot first");

  auto it = args.find("frames");
  if (it == args.end() || !it->is_number_unsigned())
    throw ToolError("frames is required and must be a positive integer");
  const uint64_t frames = it->get<uint64_t>();
  // Bounded because an unbounded run hangs the server with no way for the
  // client to interrupt it. 216000 frames is an hour of emulated time.
  if (frames < 1 || frames > 216000)
    throw ToolError("frames must be between 1 and 216000");

  const std::string until = args.value("until", std::string("frames"));
  if (until != "frames" && until != "screen_change")
    throw ToolError("until must be \"frames\" or \"screen_change\"");

  const char *reason = "limit";
  uint64_t ran = 0;
  const uint64_t base_hash = until == "screen_change" ? a2host_visible_hash() : 0;
  for (; ran != frames;) {
    // Deliberately the same order as a2host_run_headless(): simulate, then
    // record (which advances the frame counter and writes --hash-frames),
    // then test the stop conditions.
    a2host_simulate_frame();
    ++ran;
    const bool at_limit = a2host_record_frame();
    if (a2host_engine_stopped()) {
      reason = "engine";
      break;
    }
    if (a2host_stop_requested()) {
      reason = "probe";
      break;
    }
    if (until == "screen_change" && a2host_visible_hash() != base_hash) {
      reason = "screen_change";
      break;
    }
    // a2mcp never passes --frames, so frame_limit_ is 0 and this cannot fire.
    // Kept because the loop is a2run's and must stay a2run's; a silent
    // divergence here is exactly what Task 6's replay diff would blame on
    // key stamping.
    if (at_limit) {
      reason = "frame_limit";
      break;
    }
  }

  nlohmann::json out = {
      {"frames_run", ran},
      {"stop_reason", reason},
      {"frame", a2host_frame_no()},
      {"cycles", get_cycles()}};
  return out;
}

nlohmann::json machine_keys(const nlohmann::json &args) {
  if (!machine_booted())
    throw ToolError("not booted: call boot first");
  auto it = args.find("text");
  if (it == args.end() || !it->is_string())
    throw ToolError("text is required and must be a string");
  const std::string text = it->get<std::string>();

  uint64_t spacing = args.value("frames_between", (uint64_t)1);
  if (spacing > 600)
    throw ToolError("frames_between must be between 0 and 600");

  unsigned n = 0;
  for (char c : text) {
    // The same translation --kbd-file uses, so that typing through the tool
    // and typing through a file behave identically.
    if (c == '\r')
      continue;
    if (c == '\n')
      c = '\r';
    if ((unsigned char)c > 0x7F)
      throw ToolError("text must be ASCII");
    a2host_schedule_key((uint8_t)c, (unsigned)(spacing * n));
    ++n;
  }
  return {
      {"scheduled", n},
      {"pending", a2host_scheduled_keys_pending()},
      {"note", "keys are delivered during a later run"}};
}

} // namespace a2mcp
