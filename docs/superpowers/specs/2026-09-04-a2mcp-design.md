# a2mcp: an MCP server for driving the Apple II headless — design

An agent boots a disk, watches the screen, presses keys, and hears what the
speaker did — and leaves behind a `.keys` file that replays the whole session
under `a2run`.

## Why this shape

`a2host_api.h` already describes a machine a front end drives one frame at a
time: `a2host_init_emulation()`, `a2host_simulate_frame()`,
`a2host_record_frame()`. `a2host_run_headless()` is the only part that assumes
"run to a frame limit and exit". An MCP server is therefore a **third front
end** — the same machine, driven by JSON-RPC instead of by a frame count.

It is in-process, not a wrapper around `a2run`. `a2run` runs to completion and
exits, so "pause and inspect" through it would mean re-running from frame 0 on
every call, and it could only report what the CLI already prints. In-process,
pausing is *not calling* `a2host_simulate_frame()`, and a screenshot is a struct
read.

**The pause model falls out of that and needs no state.** Emulated time advances
only inside a `run` call. Between calls the machine is frozen, so an agent
thinking for eight seconds costs zero emulated time. `boot()` leaves the machine
initialised with zero frames run; that is "start it paused".

That also settles what "can an agent play games?" means. Wall-clock latency is
irrelevant; the budget is *decisions per game*. A few hundred perceive-act
cycles is comfortable. A game demanding thousands of inputs per minute of play
is not, and no amount of protocol design changes that.

## Scope

One machine per process. `lib/a2host` is built on file-scope statics — one
`parse_args`, one `iostate`, one engine — and honouring that keeps every tool
free of a session id and leaves both existing front ends untouched. An agent
that wants two games launches two servers.

Six tools, and no debugger surface in v1: no memory reads, no disassembly, no
register pokes. Those are additive later and nothing here has to change to
admit them.

## The tools

Video mode is not a tool. It rides on `status()` and on every `screen()` reply,
so "check the mode, then screenshot" is one call.

| tool | arguments | result |
|---|---|---|
| `boot` | `disk1?`, `disk2?`, `rom?`, `binary?` | machine initialised, zero frames run. Called again, reboots. |
| `run` | `frames` (required cap), `until: "frames" \| "screen_change"` | `{frames_run, stop_reason, probe?, frame, cycles}` |
| `status` | — | `{frame, cycles, video: {mode, mixed, page}, keys_pending, stopped}` |
| `screen` | `format: "text" \| "image" \| "both"`, `save_to?` | mode-appropriate text and/or inline PNG; always carries `video` |
| `keys` | `text`, `frames_between?` | keys *scheduled*, not injected |
| `sound` | `save_wav?` | tone segments since the previous `sound` call |

### `run` and stopping

`frames` is a mandatory cap, so a run always terminates. It is bounded at
216,000 — an hour of emulated time — because an unbounded value would hang the
server with no way for the client to interrupt it.

`until` defaults to `"frames"`. `until` names an
additional stop condition; whichever fires first names itself in `stop_reason`,
one of `"limit"`, `"screen_change"`, `"probe"` (with the probe's name), or
`"engine"`.

Conditions richer than `screen_change` come from a probe script supplied at
launch via the existing `--probe=`. The probe language already has `stop`, and
already compiles in every front end, so it is the condition language and no new
one is invented. Dynamic probe installation is out of scope: it would make the
machine's behaviour depend on tool-call order, which is exactly what the
reproducibility artifact must not do.

`screen_change` exists because an agent exploring an unknown disk has no memory
map to write a probe against, and advancing by guessed frame counts burns the
one resource that limits play. It is the primitive for "advance to the next
thing worth looking at".

**A rejected condition, recorded so it is not re-proposed:** "stop when the
program reads `$C000`". Games poll the keyboard in spin loops thousands of times
per frame, so it fires instantly and always.

### `keys` schedules, it does not inject

`keys` appends `<cycle> <key>` entries to the same pending list `--key-file`
fills, stamped from the current cycle plus `frames_between`. Three things follow:
`drain_key_presses()` delivers them on its existing path; a game polling between
frames sees realistic spacing; and the file the server writes **is** the
recording.

`frames_between` defaults to 1, one key per frame, which is fast enough to be
convenient and slow enough that a game polling once per frame sees every key.
`0` schedules them all at the current cycle, which overruns the 32-entry
hardware queue past that many keys and is only for short bursts.

The consequence the tool description must state plainly: **keys take effect only
during a subsequent `run`.** `keys()` followed by `screen()` shows nothing.

### Reading the screen

Format follows the mode, because the goal is a representation an agent can
reason about, not a faithful picture.

`format` defaults to `"text"`, the cheap answer, which in HGR means the reply
carries the video mode and a note that only `"image"` can show it.

- **Text** — 24 lines of ASCII via the existing `apple2_decode_text_screen`,
  about 1 KB. Inverse and flashing render as plain ASCII in v1; an attribute map
  is a later addition if it proves to matter.
- **GR** — a 40x48 grid of hex colour digits read straight from the page bytes
  (each byte is two 4-bit blocks), plus a colour legend, plus the bottom four
  lines as ASCII when mixed. Lo-res *is* text, and a colour grid tells an agent
  where things are in a way a picture of the same screen does not.
- **HGR** — PNG only. 280x192 monochrome-with-artifact-colour is genuinely hard
  to read, and this design does not pretend otherwise.

`format: "image"` renders any mode through the existing
`apple2_render_{text,gr,hgr}_screen` into `a2_screen` RGBA and encodes with a
vendored `stb_image_write.h` — the one new third-party file. `save_to` writes
the same PNG to a path under `--root`, so a session's visuals persist as
artifacts.

### Sound

The Apple II speaker is one bit, so its entire output is a list of toggle
cycle-stamps. The server installs its own `a2_io_set_spkr_cb` and appends
stamps to a growable buffer that `sound()` drains.

`sound()` returns segments of `{start_ms, duration_ms, approx_frequency_hz}`
with the silences between them — which is how Apple II games actually make
sound, and something an agent can reason about. `save_wav` synthesises a WAV
from the same stamps. No inline audio: nothing plays it, and base64 PCM is
enormous.

## Configuration and the path jail

`boot`'s `binary` is the engine's bare-argument run path — the DOS 3.3 binary
`a2run` takes positionally — and `rom` its `--rom=`.

`a2mcp` launches with `--root=<dir>`, plus `--probe=`/`--probe-out=` passed
through to `a2host_parse_args`. No machine exists until `boot()`.

Every path an agent supplies — disks, ROM, binary, `save_to`, `save_wav` — is
`realpath`'d and prefix-compared against the `realpath` of `--root`, which
settles symlink escapes too. This is a local dev tool, but a server that opens
any path the process can read is worth being deliberate about.

## Reproducibility

The artifact is a cycle-stamped `.keys` file in the format `--key-file` already
reads. Headless is fixed-step, so a key scheduled at a frame boundary has a
well-defined cycle, and `a2run --key-file=play.keys --frames=N` replays the
agent's whole session under the interpreter with no new machinery.

Probe-stamped recording stays available for cross-engine work by supplying a
`--probe=` containing `record`, unchanged from today.

A full JSON-lines transcript of tool calls was considered and rejected for v1:
it is a format only `a2mcp` understands, and it feeds neither `a2run` nor a
decompiled build.

## Changes to `lib/a2host`

**Disks need no new API.** `engine_parse_arg` is already public in `a2engine.h`,
returns `bool`, and never exits, so `boot()` sets drive paths through the
existing host/engine contract and then calls `a2host_init_emulation()`.

1. **`a2host_stop_requested()`** — export the probe stop flag.
   `a2host_run_headless` reads it internally; a front end driving its own loop
   cannot.

2. **`a2host_visible_hash()`** — new, and deliberately *not* the existing
   `hash_video_state()`. That one hashes both text pages and both HGR pages
   regardless of mode, on purpose, so the oracle catches a divergence the
   display is hiding. Reusing it for `screen_change` would be a bug: a
   double-buffered game writes its hidden page every frame, so the condition
   would fire immediately and forever. The new one hashes only what is
   displayed, per the current mode and page bits.

3. **`a2host_schedule_key(ch, at_cycle)`** — the largest piece. `key_presses_`
   today is a fixed array slurped from `--key-file`, walked by a cursor, and
   `free()`d when exhausted, with `a2host_key_replay_active()` reading
   `key_presses_ != NULL` as "replay still live". It must become growable, and
   must distinguish a server-fed list that is merely *empty* from a replay that
   is *over*. Each scheduled key is also written to the session `.keys` file as
   it is queued.

4. **`a2host_reboot()`** — the one unverified assumption, and the first thing
   to settle in implementation. `init_emulated()` is written to be called once:
   it mounts, installs the disk ROM, and registers debug callbacks on statics
   that accumulate. If a second call is not clean, v1 makes `boot()`
   once-per-process and rebooting means restarting the server.

## Testing

**The test that must exist** is the reproducibility claim, because the `.keys`
file is worthless if it does not hold. An MCP session boots, schedules keys,
runs frames and reports the visible hash at each stop; `a2run --key-file=` with
the emitted file and `--hash-frames=` must reproduce those hashes frame for
frame. Same shape as the existing cross-engine acceptance checks, and it fails
loudly if key stamping is off by one cycle.

Everything else is golden transcripts: JSON-lines requests on stdin, responses
diffed against a baseline — the repo's existing idiom, deterministic because
headless is fixed-step.

**No game data is needed.** The built-in `apple2plus_rom` boots to BASIC with no
disk, so one transcript — `boot` → `run(until="screen_change")` →
`keys("PRINT 2+2\n")` → `run` → `screen(format="text")` — covers boot, stepping,
key scheduling, the text renderer and the reproducibility artifact using only
what is committed. Disk boot gets one transcript against a disk already in the
tree. These live in `tests/` beside the a2emu/a2run equivalence check, which is
where host-level tests already are.

Negative tests, each mutation-checked by deleting the guard and confirming the
test goes red: a `disk1` and a `save_to` escaping `--root`, a symlink escaping
it, `run` before `boot`, an unknown `format`, `frames` past the cap.

## The error model

**The tool layer never calls `exit()`.** Every failure `mcp_tools.cpp` and the
`machine_*`/`sound_*`/`screen_*` functions under it can reach is a JSON-RPC
error with a message, and the server goes on serving.

That holds only because `a2mcp` validates ahead of the library's `*OrDie`
paths, which are not so careful: `init_emulated` reads through
`readFileOrDie`, `mountDisk` exits on an image `a2_disk2_mount` will not take,
`loadB33` exits on a bad header, and `Emu6502::loadROM` asserts on an
oversized ROM. So `boot` jails every path, confirms it is a regular file, and
checks its size and header against the kind it was passed as, before the
engine sees it; and every numeric argument is bounded before it is used.

A second, cheaper failure never reaches a per-argument check at all: a
well-formed request whose argument has the wrong JSON type — `render: 42`, a
number where the schema says string. Every typed accessor in
`mcp_tools.cpp`/`mcp_machine.cpp` (`args.value(...)`, `it->get<T>()`) throws
`nlohmann::json::type_error` on that, and no individual tool catches it.
`serve()`'s `tools/call` dispatch does, in one place, alongside `ToolError`:
`catch (const nlohmann::json::exception &e)` turns it into the same
`isError` result, naming the tool and saying its arguments were malformed.
The same dispatch also pulls the top-level `method`/`params.name` fields out
without `.value()`, since a bare non-object message (`42` instead of `{...}`)
makes `.value()` throw regardless of the key it's asked for — a request that
malformed gets a JSON-RPC error, not a crash, for the same reason. This is
also why a numeric argument is checked for the right JSON subtype *before*
extraction (`scale`, `frames_between`) rather than only range-checked after:
nlohmann's own number conversion does not throw when the JSON value is the
wrong number subtype — a `number_float` narrowed to `int` is a silent
`static_cast`, not an error — so `scale: 1.5` truncated to `1` would sail
past both the exception path and the range check if extracted first.

**The library beneath the tool layer can still exit, in these cases**, all of
them known and none of them reachable through a well-formed call:

  - `a2host_schedule_key()` exits if its `realloc` fails.
  - `probe_fatal()` exits on a probe runtime fault — a stack overflow or
    corrupt bytecode in a script that compiled — from inside the frame loop a
    `run` is driving.
  - Anything a future forwarded option or engine path adds that dies rather
    than returning a failure. The pattern is the library's, not this front
    end's, so the guard has to be re-established for each thing forwarded.

The exits that are *by design* all happen before the first request is served:
bad launch arguments, and probe compilation, where dying is correct and the
MCP client shows the stderr.

## Layout

`main()` goes in `lib/a2host/mcp_main.c`, mirroring `console_main.c`;
`tools/a2mcp/` holds only a `CMakeLists.txt`, as `tools/a2run/` does. Links
`a2host`, `engine6502`, `a2io`, `support` — no sokol. Transport is MCP stdio:
newline-delimited JSON-RPC 2.0, parsed with the vendored `nlohmann/json` in
`external/json`.
