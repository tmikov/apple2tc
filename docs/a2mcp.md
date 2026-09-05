# a2mcp: an MCP front end for the emulator

`a2mcp` is a third front end over `lib/a2host`, next to `a2emu` (windowed) and
`a2run` (console, driven by `--frames`). Like `a2run` it has no source of its
own — `tools/a2mcp/CMakeLists.txt` links `lib/a2mcp` and `engine6502` and
nothing else — and like `a2run` it never opens a window. What is different is
the drive train: instead of a frame count on the command line, `a2mcp` reads
newline-delimited JSON-RPC from stdin and writes replies to stdout, so an MCP
client can boot the machine, look at it, act, and look again, one tool call at
a time (`lib/a2mcp/mcp_server.cpp`'s `serve()`). Everything else — the engine,
the IO chip emulation, the keyboard queue, disk images, probe scripts — is the
same code the other two front ends run.

**Status.** All six tools are built and tested: `boot`, `status`, `run`,
`screen`, `keys`, `sound`. `tests/run-tests.sh`'s a2mcp block replays a
transcript for each against a committed baseline (`tests/mcp/*.jsonl` /
`*.expected`) and separately checks that `a2mcp`'s own frame loop tracks
`a2run`'s bit for bit, that a `keys`-recorded session replays under `a2run
--key-file=`, and that a symlink cannot be used to read or write outside
`--root` -- one test for each direction, because the read and write halves of
the jail resolve differently and only the read half used to be covered.

## Launching it

`a2mcp` takes no frame count and never exits on its own — it serves until
stdin closes. An MCP client starts it as a subprocess and talks JSON-RPC over
its stdin/stdout, so the client's config just needs the binary and its
options:

```json
{
  "mcpServers": {
    "apple2": {
      "command": "/home/tmikov/work/apple2tc/cmake-build-debug/tools/a2mcp/a2mcp",
      "args": ["--root=/home/tmikov/work/apple2tc/tests"]
    }
  }
}
```

Options (`lib/a2mcp/mcp_main.cpp`):

| option | effect |
|---|---|
| `--root=<dir>` | **Required.** Every path an agent supplies to `boot` or `screen`/`sound`'s `save_to`/`save_wav` is resolved against this directory and rejected if it resolves outside it — `realpath()` on both sides, so a symlink that points out of the jail is caught the same as a literal `../`. Reading and writing are separate entry points (`jail_existing_file()` / `jail_new_file()` in `lib/a2mcp/mcp_paths.cpp`), because a file being written need not exist yet: only its directory has to. A name that *does* exist is resolved either way, so an existing symlink at the final component of a `save_to` is judged by where it points. |
| `--probe=<path>` | A probe script (`docs/probes.md`), compiled once at startup, before the first request is served. A bad script exits the process with a diagnostic on stderr rather than turning into a tool error, because at that point there is no client connected yet to hand an error to. |
| `--probe-out=<path>` | Where the probe's `printf` output goes. Requires `--probe=`. Without it, a probe's `printf` goes to **stderr** — `a2host_probe_output_to_stderr()`, called before `a2host_parse_args`. The library's own default is stdout, which is right for `a2run` (whose stdout is a report) and would put script output into the middle of the JSON-RPC stream here. Fixed at the sink rather than by requiring this option, so a probe opcode that prints can be added later without rediscovering the problem. |
| `--keys-out=<path>` | Opened before the server starts serving, so the first key any `keys` call schedules lands in the file. Every scheduled key is appended as a `<cycles> <key>` line — the same format `a2run --key-file=` reads — turning the whole session into a replayable recording. |
| `--hash-frames=<path>` | Per-frame video-state hashes, written exactly as `a2run --hash-frames=` writes them. This is what lets `run` be diffed frame-for-frame against a plain `a2run` session — it is the test `tests/run-tests.sh` runs to prove the two frame loops agree. |

`--help` lists the same five; `--trace`, `--trace-keys`, `--probe-dump` and
any other `a2host` option that writes to stdout is deliberately not forwarded
(`mcp_main.cpp`'s comment on the whitelist) — stdout carries JSON-RPC and
nothing else, so an option that could put other text there is simply not on
the list. The whitelist alone is not enough for `--probe=`, which *is*
forwarded: see `--probe-out=` above.

## Playing a game with it

`docs/a2mcp-playing.md` covers driving the machine against a real game: that a
disk does not autoboot (type `PR#6`), that `until: "screen_change"` is for
*reaching* a stable state and is useless once anything animates, how to read a
game's control scheme out of its own disk image, and how to tell a game you are
controlling from an attract loop that ignores you.

## The three things an agent cannot guess

**Emulated time only advances inside `run`.** Between tool calls the machine
is frozen — `boot`, `status`, `keys` and `screen` never call
`a2host_simulate_frame()`. An agent that spends ten turns deciding what to do
next costs itself ten turns, not ten seconds of game time; nothing on the
Apple II side ages while it thinks.

**`keys` schedules, it does not inject.** Calling `keys` stamps each
character on the cycle counter and appends it to a private list
(`a2host_schedule_key()`); the keys reach the emulated keyboard queue only
when a later `run` advances far enough past their stamps to drain them.
Calling `keys` and then `screen` with no `run` in between shows the machine
exactly as it was before the call — this is what `tests/mcp/keys.expected`
checks (request 5's screen still reads a bare `]` prompt; the `PRINT 2+2` typed
in request 4 shows up only after request 6's `run`).

**Hi-res has no text rendering.** `screen` with `format: "text"` gives 40x24
ASCII in text mode and a 40x48 grid of hex colour digits in lo-res (GR); in
HGR it can only say so (`"hi-res: no text rendering. Call screen with format
\"image\"."`, `lib/a2mcp/mcp_tools.cpp`). There is no ASCII approximation of a
bitmap. `format: "image"` — an inline PNG — is the only way to see HGR
content at all.

## The six tools

### `boot`

Boots the machine. With no arguments: the built-in Apple ][+ ROM, into BASIC.
`disk1` / `disk2` mount disk images; `rom` loads a different ROM image;
`binary` loads and runs a DOS 3.3 binary. All four are paths under `--root`.
Calling `boot` again on an already-running machine reboots it —
`a2host_reboot()` tears the engine down (including unloading the ROM,
`Emu6502::unloadROM()`) and brings it back up with frame and cycle counters
at zero. Returns the same object `status` does.

### `status`

No arguments. Frame counter, cycle counter, video mode (`text`/`gr`/`hgr`,
plus whether it's mixed and which page is active), keys still waiting in the
hardware queue (`keys_pending`) versus keys scheduled but not yet drained
into it (`keys_scheduled`), and whether the engine or a probe's `stop` has
ended the run.

### `run`

```json
{"frames": 120, "until": "screen_change"}
```

`frames` is required — an upper bound in 1/60 s frames (1 to 216000, an hour
of emulated time), so a call always terminates even if nothing else stops it.
`until: "screen_change"` (default is plain `"frames"`) adds an early exit: the
run stops on the first frame where `a2host_visible_hash()` — a hash of only
the mode bits and the page(s) actually on screen — differs from its value
when the call started. That is the way to advance to "the next thing worth
looking at" without guessing a frame count. A probe script's `stop`, and the
engine stopping itself, always end a run early regardless of `until`. The
reply reports `frames_run`, `stop_reason` (`"limit"`, `"screen_change"`,
`"probe"`, or `"engine"`), and the frame/cycle counters after the run.

The loop itself (`machine_run()` in `lib/a2mcp/mcp_machine.cpp`) is a copy of
`a2host_run_headless()`'s: simulate, then record the frame (which is what
advances the frame counter and writes `--hash-frames=`), then test the stop
conditions, in that order. `tests/run-tests.sh` diffs 40 frames of `a2run`
against 40 frames of `a2mcp` to hold the two to the same behaviour.

### `screen`

```json
{"format": "image", "save_to": "shot.png"}
```

`format` is `"text"` (default), `"image"`, or `"both"`. `"text"` gives the
cheapest representation the current mode allows — see above for what that
means in HGR. `"image"` renders any mode (including HGR) to a PNG, returned
inline as base64; `save_to` additionally writes it to a path under `--root`.

### `keys`

```json
{"text": "PRINT 2+2\n", "frames_between": 1}
```

Schedules keystrokes — see [above](#the-three-things-an-agent-cannot-guess)
for what "schedules" means. `frames_between` spaces consecutive keys that
many frames apart (default 1); `\n` is translated to Return, the same
translation `--kbd-file=` uses. The reply reports how many keys were
scheduled and how many (across every `keys` call so far) are still waiting to
be drained by a `run`.

`format: "text"` has no file to write, so passing `save_to` with it is
rejected rather than ignored.

### `sound`

No required arguments; `save_wav` writes a WAV to a path under `--root`.
Reports what the speaker did since the previous `sound` call (or since
`boot`, the first time) as a list of tone segments —
`{start_ms, duration_ms, toggles, approx_frequency_hz}` — separated by
`{silence_ms}` gaps wherever the speaker went quiet for at least 20 ms. Every
call clears the recorded stamps, so segments never repeat across calls.

## A worked session

This is `tests/mcp/keys.jsonl` run for real against
`cmake-build-debug/tools/a2mcp/a2mcp --root=tests` (reformatted onto one line
per exchange; the actual protocol is newline-delimited JSON, one object per
line, no pretty-printing):

```
-> {"method":"initialize", ...}
<- {"result":{"protocolVersion":"2025-06-18","serverInfo":{"name":"a2mcp","version":"1"}, ...}}

-> {"method":"tools/call","params":{"name":"boot","arguments":{}}}
<- cycles:0 frame:0 keys_pending:1 keys_scheduled:0 mode:text

-> {"method":"tools/call","params":{"name":"run","arguments":{"frames":120}}}
<- cycles:2028950 frame:120 frames_run:120 stop_reason:limit

-> {"method":"tools/call","params":{"name":"keys","arguments":{"text":"PRINT 2+2\n"}}}
<- scheduled:10 pending:10 note:"keys are delivered during a later run"

-> {"method":"tools/call","params":{"name":"screen","arguments":{"format":"text"}}}
<- "               APPLE ][                 "
   "                                        "
   "]                                       "
   ... (blank prompt -- the keys have not been drained yet)

-> {"method":"tools/call","params":{"name":"run","arguments":{"frames":180}}}
<- cycles:5097951 frame:300 frames_run:180 stop_reason:limit

-> {"method":"tools/call","params":{"name":"screen","arguments":{"format":"text"}}}
<- "               APPLE ][                 "
   "                                        "
   "]PRINT 2+2                              "
   "4                                       "
   "]                                       "
   ...
```

`4` on the screen after the second `run` is BASIC having actually evaluated
what `keys` typed — the ten scheduled keystrokes (`PRINT 2+2` plus the
Return) were stamped starting at cycle 2028950 (`run`'s cycle count after the
first 120 frames) and one frame apart, so the last one lands at cycle
2182400, comfortably inside the second `run`'s 180-frame budget. This
transcript, byte for byte, is `tests/mcp/keys.jsonl` against
`tests/mcp/keys.expected` — one of the baselines `tests/run-tests.sh` diffs
on every run.

## Replaying a session

`--keys-out=<path>` turns a session into a `.keys` file — one `<cycles>
<key>` line per scheduled key, in the exact format `--key-file=` reads. It
can be replayed with:

```shell
a2run --frames=300 --key-file=session.keys --hash-frames=replay.txt
```

`300` because that is how many frames the recording session actually ran
(120 + 180 above); `a2run` needs `--frames=` because, unlike `a2mcp`, it has
no client telling it when to stop.

This works because the headless front ends are fixed-step: every
`a2host_simulate_frame()` advances exactly 1/60 s of emulated time regardless
of wall clock, so a key stamped at a given cycle count lands at a
well-defined point in a well-defined frame no matter which process produced
or is replaying the recording. Recorded from the session above:

```shell
$ cat session.keys
2028950 80
2046000 82
2063050 73
2080100 78
2097150 84
2114200 32
2131250 50
2148300 43
2165350 50
2182400 13
$ a2run --frames=300 --key-file=session.keys --hash-frames=replay-a2run.txt
$ a2mcp --root=tests --hash-frames=replay-a2mcp.txt < session.jsonl
$ diff replay-a2run.txt replay-a2mcp.txt   # empty
```

`80`, `82`, `73`... are the ASCII codes of `PRINT 2+2` and `13` is Return.
The 17050-cycle spacing between consecutive keys is one frame's worth of
cycles at the emulated 6502's clock rate — `frames_between: 1`, the `keys`
default.

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

## Known limitations

These are properties of the shipped code, not open bugs waiting on a fix —
each is listed so an agent (or a person) does not spend time rediscovering
it.

1. **The cycle counter wraps.** `get_cycles()` returns a plain 32-bit
   `unsigned`, which overflows after roughly 70 minutes of emulated time at
   the Apple II's ~1.02 MHz clock. `--key-file=` stamps, and therefore every
   `.keys` file `--keys-out=` writes, have always inherited this — nothing in
   `a2mcp` makes it worse or better.

2. **`STA` to a soft switch can net-cancel.** `a2_io_poke()` implements every
   non-disk soft-switch *write* by calling `a2_io_peek()` twice
   (`lib/a2io/a2io.c`) — reads and writes have the same side effect on real
   hardware, and the emulator has no separate write path for most of them.
   For the speaker toggle at `$C030`, two peeks at the same cycle toggle it
   twice, netting to no change: a program that does `STA $C030` produces two
   `sound` stamps at one cycle instead of one. Every fixture and decoded game
   in this repository uses `LDA $C030` instead, so nothing currently
   exercises the `STA` case — but `sound` would under-report a program that
   did. This is `a2io.c`'s pre-existing behaviour, not something `a2mcp`
   introduced.

3. **A long `keys` call at a wide spacing is refused, not wrapped.** `keys`
   stamps its *n*-th character `spacing * n` frames ahead, and the stamp the
   machine stores is a 32-bit cycle count. The call computes where its last
   key would land in 64-bit arithmetic first and raises a tool error if that
   is past 2^32 — at `frames_between: 600` (the maximum) that is somewhere
   around the 420th character of one call. The refusal is the limitation:
   there is no way to schedule that far ahead, because the counter the stamp
   lives in cannot hold it. Wrapping instead would put the key *below* the
   current cycle count, firing the whole tail at once and writing a
   `--keys-out=` file that no replay can read.

4. **`frames_between: 0` past 32 keys drops keys.** The hardware keyboard
   queue holds 32 entries (`A2_KBD_QUEUE_SIZE`); stamping every key at the
   same cycle means a `run` that reaches that cycle tries to drain all of
   them into the queue in one frame, and anything past the 32nd is silently
   dropped (`a2_io_push_key()` returns `false` and the caller ignores it).
   The tool description says so. This is not a divergence risk: `a2run`
   replaying the same `.keys` file drops the same keys at the same point, so
   reproducibility survives even though the input was lossy.

5. **A reboot mid-session breaks `.keys` replay.** `a2host_reboot()` resets
   the cycle counter to zero but leaves the `--keys-out=` file open, so any
   key scheduled after a `boot` call that reboots an already-running machine
   is stamped starting near zero again — after entries from before the
   reboot that were stamped against the old, higher counter. The resulting
   file is non-monotonic and `a2run --key-file=` will not replay it
   correctly. Recording per *session* rather than per *boot* was a deliberate
   choice; this consequence of it was not designed around, and a session that
   needs to reboot and still be replayable has no answer here yet. What the
   reboot does do is say so, twice: a warning on stderr, and a `# reboot:`
   line in the recording itself, which `load_key_file()` skips as a comment —
   so whoever finds the broken replay finds the reason in the file.

6. **`screen_change` can fire on pixels nobody can see.** In mixed HGR mode,
   `a2host_visible_hash()` hashes the *entire* hi-res page, including the
   bottom rows the text window is currently covering — it does not carve out
   the hidden region (`lib/a2host/a2host.c`). A program that keeps drawing
   into those hidden rows while the visible ones are static will end a
   `screen_change` run on a change nobody watching the screen would see.

7. **A probe's `stop` ends the run, not the server.** `a2run` exits on a
   probe `stop`, so the library's flag is sticky by design. Here a run is one
   tool call among many, so `run` clears the flag before each call. Two
   consequences: a `stop` cannot wedge the session (the next `run` advances
   again), and `status` reports `stopped` for the gap between the run that
   stopped and the next one, not forever. A script that wants to stop the
   machine for good has to stop it on every frame it is asked to run.

8. **Buffered keys and scheduled keys both drain.** `a2host_simulate_frame()`
   drains scheduled keys unconditionally, then falls into the chain that
   drains a `--key-file=` replay, a `--kbd-file=`, or the program's own
   buffered keys — and a `--key-file=` replay suppresses buffered keys
   (`key_presses_ != NULL`), where `a2mcp`'s scheduled keys do not. So a
   program that registers buffered keys at `init_emulated()` gets both its own
   keys and the agent's under `a2mcp`, and only the agent's under an `a2run`
   replay of the same recording. Nothing bootable in this repository does
   that, so no test covers it; it would be a real divergence for one that did.

9. **`tests/mcp/image.expected` pins exact PNG bytes.** The baseline is the
   literal base64 of an stb-compressed PNG. This is not fragile to platform
   or compiler — `stb_image_write.h`'s PNG encoder is integer arithmetic
   throughout, unlike its HDR and JPEG paths — but a deliberate upgrade of
   the vendored `stb_image_write.h` would still change those bytes and need
   the baseline regenerated.
