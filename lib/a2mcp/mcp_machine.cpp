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
      {"stopped", a2host_engine_stopped()}};
}

} // namespace a2mcp
