/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_tools.h"
#include "mcp_machine.h"
#include "mcp_screen.h"
#include "mcp_server.h"

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
        "PNG to a path under the server's root."},
       {"inputSchema",
        {{"type", "object"},
         {"properties",
          {{"format", {{"type", "string"}, {"enum", {"text", "image", "both"}}}},
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
    if (!machine_booted())
      throw ToolError("not booted: call boot first");
    const std::string format = args.value("format", std::string("text"));
    if (format != "text")
      throw ToolError("format \"" + format + "\" is not implemented yet");
    nlohmann::json content = nlohmann::json::array();
    content.push_back({{"type", "text"}, {"text", screen_text()}});
    return nlohmann::json{{"content", content}};
  }
  throw ToolError("unknown tool: " + name);
}

} // namespace a2mcp
