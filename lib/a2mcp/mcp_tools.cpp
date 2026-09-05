/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_tools.h"
#include "mcp_machine.h"
#include "mcp_paths.h"
#include "mcp_screen.h"
#include "mcp_server.h"
#include "mcp_sound.h"

// Same reasoning as mcp_screen.cpp/mcp_machine.cpp: a2io.h pulls in
// soundqueue.h, which uses <atomic> in its C++ branch, so it must not be
// wrapped in extern "C" here.
#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"

namespace a2mcp {
namespace {

nlohmann::json obj(void) {
  return nlohmann::json::object();
}

} // namespace

nlohmann::json tool_schemas(void) {
  nlohmann::json tools = nlohmann::json::array();

  tools.push_back(
      {{"name", "boot"},
       {"description",
        "Boot the Apple II. Optionally mount disks, or load a ROM or a DOS 3.3 "
        "binary; with no arguments the machine boots the built-in Apple ][+ ROM "
        "into BASIC. Paths are relative to the server's root directory. Returns "
        "with the machine initialised and zero frames run -- emulated time "
        "advances only inside `run`."},
       {"inputSchema",
        {{"type", "object"},
         {"properties",
          {{"disk1", {{"type", "string"}, {"description", "disk image for drive 1"}}},
           {"disk2", {{"type", "string"}, {"description", "disk image for drive 2"}}},
           {"rom", {{"type", "string"}, {"description", "ROM image; default is built-in"}}},
           {"binary",
            {{"type", "string"}, {"description", "DOS 3.3 binary to load and run"}}}}}}}});

  tools.push_back(
      {{"name", "status"},
       {"description",
        "Current machine state: frame counter, cycle counter, video mode, keys "
        "still pending delivery, and whether the engine or a probe has stopped."},
       {"inputSchema", {{"type", "object"}, {"properties", obj()}}}});

  tools.push_back(
      {{"name", "run"},
       {"description",
        "Advance emulated time. `frames` is a required upper bound in 1/60s "
        "frames, so a run always terminates. `until` adds a stop condition: "
        "\"screen_change\" stops on the first frame where the displayed screen "
        "differs from what it was when the call started -- the way to advance to "
        "the next thing worth looking at without guessing a frame count. A probe "
        "script's `stop`, and the engine stopping itself, always end a run."},
       {"inputSchema",
        {{"type", "object"},
         {"required", {"frames"}},
         {"properties",
          {{"frames", {{"type", "integer"}, {"minimum", 1}, {"maximum", 216000}}},
           {"until", {{"type", "string"}, {"enum", {"frames", "screen_change"}}}}}}}}});

  tools.push_back(
      {{"name", "screen"},
       {"description",
        "Read the screen. \"text\" gives the cheapest representation the current "
        "mode allows: 40x24 ASCII in text mode, a 40x48 grid of hex colour digits "
        "in lo-res (GR), and in hi-res (HGR) only a note that an image is needed. "
        "\"image\" renders any mode to an inline PNG. `save_to` also writes the "
        "PNG to a path under the server's root. `render` picks how a HGR image "
        "is decoded: \"color\" (default) is the per-dot artifact approximation "
        "this tool has always drawn, where an isolated dot's colour depends on "
        "its horizontal parity; \"color140\" decodes at the true colour-clock "
        "resolution instead, so a feature comes out in one consistent colour -- "
        "use it to read a scene. \"mono140\" collapses the same colour-clock "
        "cells to white-if-any-ink/black-if-none, an occupancy mask with no "
        "colour to reason about -- use it to find shapes. `render` is accepted "
        "in every mode but only affects HGR; it is ignored in TEXT and GR. "
        "`scale` is output pixels per colour cell for \"color140\"/\"mono140\": "
        "2 (default) doubles each cell to match \"color\"'s 280x192; 1 emits "
        "the native 140x192 instead, halving the image's payload with no loss "
        "of information -- the sensible default for an agent reading a scene "
        "repeatedly. The cost: at scale 1 the image is horizontally compressed "
        "relative to how the machine displays it, so shapes are distorted (a "
        "circle reads as an ellipse) even though relative positions are "
        "preserved -- use scale 2 for anything judging shape rather than "
        "position. `scale` has no cells to describe outside \"color140\"/"
        "\"mono140\" and is accepted-and-ignored for \"color\" and for TEXT/GR, "
        "the same as `render`."},
       {"inputSchema",
        {{"type", "object"},
         {"properties",
          {{"format", {{"type", "string"}, {"enum", {"text", "image", "both"}}}},
           {"render", {{"type", "string"}, {"enum", {"color", "color140", "mono140"}}}},
           {"scale", {{"type", "integer"}, {"enum", {1, 2}}}},
           {"save_to", {{"type", "string"}}}}}}}});

  tools.push_back(
      {{"name", "keys"},
       {"description",
        "Schedule keystrokes. They are NOT delivered now: each key is stamped "
        "with a cycle count and reaches the machine during a later `run`. "
        "`frames_between` spaces them (default 1 key per frame); 0 stamps them "
        "all at the current cycle, which overruns the 32-key hardware queue past "
        "32 keys. Newline is translated to Return, as on a real Apple II."},
       {"inputSchema",
        {{"type", "object"},
         {"required", {"text"}},
         {"properties",
          {{"text", {{"type", "string"}}},
           {"frames_between", {{"type", "integer"}, {"minimum", 0}, {"maximum", 600}}}}}}}});

  tools.push_back(
      {{"name", "sound"},
       {"description",
        "What the speaker did since the previous `sound` call, as tone segments "
        "of {start_ms, duration_ms, approx_frequency_hz} with the silences "
        "between them. `save_wav` also writes the audio to a path under the "
        "server's root."},
       {"inputSchema",
        {{"type", "object"}, {"properties", {{"save_wav", {{"type", "string"}}}}}}}});

  return tools;
}

nlohmann::json call_tool(const std::string &name, const nlohmann::json &args) {
  auto text_result = [](const nlohmann::json &value) {
    return nlohmann::json{{"content", {{{"type", "text"}, {"text", value.dump(2)}}}}};
  };
  // Both preconditions up front, in this order, rather than repeated down the
  // dispatch below: an unknown name is not a machine-state problem, so it must
  // be answered as itself even before the machine is up. The known names come
  // from tool_schemas() so that this cannot drift from what the server
  // advertises.
  bool known = false;
  for (const auto &tool : tool_schemas())
    known = known || tool.at("name") == name;
  if (!known)
    throw ToolError("unknown tool: " + name);
  // `boot` is the one tool that may run on a machine that is not up yet;
  // everything else needs the same answer in the same words.
  if (name != "boot" && !machine_booted())
    throw ToolError("not booted: call boot first");

  if (name == "boot") {
    machine_boot(args);
    return text_result(machine_status());
  }
  if (name == "status")
    return text_result(machine_status());
  if (name == "run")
    return text_result(machine_run(args));
  if (name == "keys")
    return text_result(machine_keys(args));
  if (name == "screen") {
    const std::string format = args.value("format", std::string("text"));
    if (format != "text" && format != "image" && format != "both")
      throw ToolError("unknown format \"" + format + "\"");
    const std::string render = args.value("render", std::string("color"));
    a2_hgr_mode_t hgr_mode;
    if (render == "color")
      hgr_mode = A2_HGR_COLOR;
    else if (render == "color140")
      hgr_mode = A2_HGR_COLOR140;
    else if (render == "mono140")
      hgr_mode = A2_HGR_MONO140;
    else
      throw ToolError("unknown render \"" + render + "\"");
    // Checked before extraction, the same way save_to is below: args.value()
    // would silently truncate a non-integer (`scale: 1.5` becomes 1 via a
    // plain static_cast in nlohmann's number conversion, no throw) and the
    // range check below would then never see the real value.
    int scale = 2;
    if (auto it = args.find("scale"); it != args.end()) {
      if (!it->is_number_integer())
        throw ToolError("scale must be an integer (1 or 2)");
      scale = it->get<int>();
    }
    if (scale != 1 && scale != 2)
      throw ToolError("scale must be 1 or 2");
    // Only the PNG can be saved, so asking to save a text-mode reading is a
    // mistake worth naming: silently ignoring it leaves the agent believing a
    // file it will later fail to find was written.
    if (format == "text" && args.find("save_to") != args.end())
      throw ToolError("save_to needs format \"image\" or \"both\": there is no file to write");
    nlohmann::json content = nlohmann::json::array();
    if (format == "text" || format == "both") {
      const a2_vidmode_t mode = a2_io_get_vidmode(a2host_io());
      std::string text;
      if (mode == A2_VIDMODE_TEXT)
        text = screen_text();
      else if (mode == A2_VIDMODE_GR)
        text = screen_gr();
      else
        text = "hi-res: no text rendering. Call screen with format \"image\".";
      content.push_back({{"type", "text"}, {"text", text}});
    }
    if (format == "image" || format == "both") {
      const std::string png = screen_png(hgr_mode, (unsigned)scale);
      auto save = args.find("save_to");
      if (save != args.end()) {
        if (!save->is_string())
          throw ToolError("save_to must be a string");
        write_jailed_file(save->get<std::string>(), png);
      }
      content.push_back({{"type", "image"}, {"data", base64(png)}, {"mimeType", "image/png"}});
    }
    return nlohmann::json{{"content", content}};
  }
  if (name == "sound") {
    std::string wav_path;
    auto save = args.find("save_wav");
    if (save != args.end()) {
      if (!save->is_string())
        throw ToolError("save_wav must be a string");
      wav_path = save->get<std::string>();
    }
    return text_result(sound_report(wav_path));
  }
  // Only reachable if a name is advertised by tool_schemas() and dispatched
  // nowhere above.
  throw ToolError("no implementation for tool: " + name);
}

} // namespace a2mcp
