/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_machine.h"
#include "mcp_paths.h"
#include "mcp_server.h"
#include "mcp_sound.h"

// Each header brackets its own declarations in `extern "C"` under
// `#ifdef __cplusplus`, so no wrapper is needed here -- and adding one would
// be actively wrong: a2io.h pulls in soundqueue.h, which uses <atomic> in its
// C++ branch, and C++ standard headers do not tolerate C linkage.
#include "apple2tc/a2engine.h"
#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"

#include <cstdio>
#include <vector>

namespace a2mcp {
namespace {

bool s_booted = false;

/// What a forwarded file has to be for the engine to accept it. Existence and
/// being a regular file is what the jail checks; this is the second half,
/// because the engine's own rejection is exit(2) -- mountDisk() on a bad
/// image, loadB33() on a bad binary, loadROM()'s release_assert on an
/// oversized ROM. All three happen inside init_emulated(), i.e. inside a tool
/// call, where the process must not die.
enum class ImageKind { Disk, Rom, Binary };

/// Read \p path, or throw. Stops a little past the largest image any of the
/// three kinds can legitimately be (a 143360-byte disk), so that pointing the
/// tool at a huge file costs a read of a quarter megabyte and not of the file:
/// every check below only needs to know the size is wrong, not how wrong.
std::string read_for_check(const std::string &path, const std::string &rel) {
  FILE *f = fopen(path.c_str(), "rb");
  if (!f)
    throw ToolError("cannot read " + rel);
  std::string data;
  char buf[4096];
  size_t n;
  while (data.size() <= 0x40000 && (n = fread(buf, 1, sizeof(buf), f)) != 0)
    data.append(buf, n);
  const bool truncated = !feof(f);
  fclose(f);
  if (truncated)
    data.push_back('\0'); // Only its size past the cap matters below.
  return data;
}

void check_image(const std::string &path, const std::string &rel, ImageKind kind) {
  const std::string data = read_for_check(path, rel);
  switch (kind) {
  case ImageKind::Disk:
    // a2_disk2_mount() takes a 143360-byte DSK/DO image and nothing else.
    if (data.size() != 143360)
      throw ToolError("not a disk image: " + rel + " (a DSK/DO image is exactly 143360 bytes)");
    break;
  case ImageKind::Rom:
    // Emu6502::loadROM() maps the image at the top of the address space.
    if (data.empty() || data.size() > 0x10000)
      throw ToolError("not a ROM image: " + rel + " (1 to 65536 bytes)");
    break;
  case ImageKind::Binary: {
    // The .b33 header loadB33() reads: load address, length, then the image.
    if (data.size() <= 4)
      throw ToolError("not a DOS 3.3 binary: " + rel + " (too short for a b33 header)");
    const unsigned start = (uint8_t)data[0] + (uint8_t)data[1] * 256u;
    if (data.size() - 4 > 0x10000u - start)
      throw ToolError("not a DOS 3.3 binary: " + rel + " (does not fit at its load address)");
    break;
  }
  }
}

/// Forward one engine option, validating the path first: init_emulated() uses
/// readFileOrDie() and mountDisk() exits on a bad image, and neither may be
/// reached from inside a tool call.
void forward_path(const nlohmann::json &args, const char *key, const char *opt, ImageKind kind) {
  auto it = args.find(key);
  if (it == args.end())
    return;
  if (!it->is_string())
    throw ToolError(std::string(key) + " must be a string");
  const std::string rel = it->get<std::string>();
  const std::string path = jail_existing_file(rel);
  check_image(path, rel, kind);
  const std::string arg = std::string(opt) + path;
  if (!engine_parse_arg(arg.c_str()))
    throw ToolError(std::string("the engine rejected ") + opt);
}

} // namespace

bool machine_booted(void) {
  return s_booted;
}

void machine_boot(const nlohmann::json &args) {
  forward_path(args, "rom", "--rom=", ImageKind::Rom);
  forward_path(args, "disk1", "--disk1=", ImageKind::Disk);
  forward_path(args, "disk2", "--disk2=", ImageKind::Disk);
  // The engine takes the program to run as a bare argument.
  auto bin = args.find("binary");
  if (bin != args.end()) {
    if (!bin->is_string())
      throw ToolError("binary must be a string");
    const std::string rel = bin->get<std::string>();
    const std::string path = jail_existing_file(rel);
    check_image(path, rel, ImageKind::Binary);
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
  // Last, because a2host_init_emulation() -- reached directly above or via
  // a2host_reboot() -- calls a2_io_init(), which clears the speaker callback.
  // Installing any earlier would lose it on every boot and every reboot.
  sound_install();
}

nlohmann::json machine_status(void) {
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

  // A probe's `stop` is sticky by default -- see s_stop_requested in
  // probe_vm.c -- which is right for a2run, where the run is the process, and
  // wrong here, where a run is one tool call among many: uncleared, the first
  // `stop` would end every later run at frame 1 forever, with no tool able to
  // undo it (a2host_reboot() preserves probe state on purpose). Cleared at the
  // start of a run rather than the end of one so that `status` can still
  // report the stop that ended the previous run.
  a2host_clear_stop_request();

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
  auto it = args.find("text");
  if (it == args.end() || !it->is_string())
    throw ToolError("text is required and must be a string");
  const std::string text = it->get<std::string>();

  uint64_t spacing = args.value("frames_between", (uint64_t)1);
  if (spacing > 600)
    throw ToolError("frames_between must be between 0 and 600");

  // The stamp a2host_schedule_key() computes is `get_cycles() + frames_ahead *
  // frame_cycles` in 32-bit arithmetic, and the last key of this call is the
  // furthest ahead. Computed here in 64 bits and refused when it would not fit
  // there, because the alternative is not an error but silent corruption: a
  // wrapped stamp lands *below* the current cycle count, so the key fires
  // immediately, and the .keys file --keys-out= writes comes out
  // non-monotonic and unreplayable. Counting characters first -- '\r' is
  // dropped, so text.size() is an over-estimate.
  uint64_t count = 0;
  for (char c : text) {
    if (c != '\r')
      ++count;
  }
  if (count) {
    const uint64_t last = (uint64_t)get_cycles() + spacing * (count - 1) * a2host_frame_cycles();
    if (last > 0xFFFFFFFFu)
      throw ToolError(
          "the last key would be stamped past the end of the 32-bit cycle counter: "
          "shorten text or reduce frames_between");
  }

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
