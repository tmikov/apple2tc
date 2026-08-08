# Splitting the Apple II host from the execution engine

Date: 2026-08-07

## Problem

There are two Apple II hosts in the tree, and they are the same program written
twice:

| | `tools/a2emu` | `lib/decapplib` |
| --- | --- | --- |
| execution | `EmuApple2`, an interpreting 6502 | the generated C (`s_ram`, `run_emulated`) |
| links | `d6502 cpuemu a2io support sokol` | `sokol a2io` |
| used by | itself | `decoded/{bolo,rom,robotron,snake-byte}` |
| run-data collection | `DebugState6502` | — |
| `--headless`, `--hash-frames`, `--frames` | — | yes |
| `--kbd-file`, `--no-sound`, `--fast` | own copy | own copy |
| frame loop, video, sound, key replay | own copy | own copy |

`a2emu::simulateFrame` and `decapplib::simulate_frame` are recognisably the same
function, forked and then evolved apart — one grew deterministic replay and
frame hashing, the other kept `StopReason` handling for `--limit`.

Three consequences, in increasing order of importance:

1. **Run data cannot be regenerated.** `decoded/snake-byte/snake-byte.json` was
   produced by a windowed `a2emu` in commit `04ee0b8`, dated **2022-01-02**.
   Nothing in the current tree can reproduce it, and on a machine without a
   display nothing can produce run data for a *new* game either. For a project
   whose aim is a repeatable method for any Apple II binary, "step zero needs a
   GUI and is not reproducible" is a worse hole than any amount of ROM left in
   the output.
2. **There is no independent oracle.** `play.frames` is recorded from
   `snake-bytec1` and compared against `snake-bytec1` and its extern variant.
   The interpreter — a genuinely separate implementation of the same machine —
   has never been compared against the generated code.
3. **Anything new must be written twice**, or put where it does not belong.
   Start-state snapshot capture for Phase 1b is the immediate example.

## The finding: the contract already exists

`include/apple2tc/system.h` is not "the decompiled runtime's header". It is an
**engine contract**, and it already lines up with `Emu6502` almost one to one:

| `system.h` | `Emu6502` |
| --- | --- |
| `reset_regs()` | `reset()` |
| `get_regs()` / `set_regs()` | `getRegs()` / `setRegs()` |
| `get_cycles()` | `getCycles()` |
| `get_ram()` | `getMainRAM()` |
| `ram_peek` / `ram_poke` | `ram_peek` / `ram_poke` |
| `run_emulated(cycles)` | `runFor(cycles)` |
| `init_emulated()` | `loadROM()` + `reset()` |
| `debug_asm(pc)` — engine calls host | `setDebugStateCB` — engine calls host |

It is also already a *two-way* contract, just not labelled as one:

- **Engine provides, host calls:** `reset_regs`, `set_regs`, `get_regs`,
  `get_cycles`, `get_ram`, `ram_peek`, `ram_poke`, `ram_peek16`,
  `init_emulated`, `run_emulated`, `shutdown_emulated`.
- **Host provides, engine calls:** `io_peek`, `io_poke`, `debug_asm`,
  `error_handler`, `g_debug`.

### The boundary is already in the right place

This is the part worth being explicit about, because it looked like the main
risk and turned out to be the main reassurance.

The two engines have completely different execution models. The generated code
is straight-line C with no way to return from the middle of a routine, so
`system2-inc.h` runs it **on its own thread**: `run_emulated()` hands over via a
condition variable and blocks, and `CYCLES()` calls `cycles_expired()` to hand
back when the budget runs out. `Emu6502::runFor()` is an ordinary interpreter
loop that just returns.

Both nevertheless present the host with the same synchronous call: *run N
cycles, return when done*. The threading is entirely inside the engine and never
leaks. That means the split does not require inventing a boundary — it requires
naming the one that is already there.

It also yields an invariant worth writing down: **the host is blocked for the
entire time the engine runs**, so `debug_asm` and `error_handler` may touch host
state without synchronisation.

## Design

**Make the contract explicit, and use link-time polymorphism.**

No vtable and no indirection: no executable ever needs two engines at once, so
each one links the engine it wants and the linker resolves `system.h`. This is
both the simplest possible mechanism and the fastest.

```
include/apple2tc/
  a2engine.h     what an engine must provide   (host → engine calls)
  a2host.h       what the host provides        (engine → host calls)
  system.h       umbrella that includes both, for existing users

lib/a2host/      NO sokol. CLI (common args), key replay, deterministic frame
                 loop, hash_video_state, record_frame, the headless loop, owns
                 the single a2_iostate_t, debug_asm / error_handler.
                 (today's decapplib, minus the GUI, made engine-agnostic)

lib/a2host_gui/  sokol. Window, audio, events, rendering, wall-clock pacing,
                 sokol_main.

lib/engine6502/  an engine implementing a2engine.h over Emu6502, plus b33/ROM/
                 disk loading and DebugState6502 run-data collection.

a2emu  (GUI)     = a2host + a2host_gui + engine6502 + its debug UI
a2run  (console) = a2host + engine6502
decoded/*/       = a2host + a2host_gui + the generated C
```

### Why two executables and not one with a flag

On Windows the console/GUI distinction is a **link-time** property --
`/SUBSYSTEM:CONSOLE` versus `/SUBSYSTEM:WINDOWS` -- not something a runtime flag
can bridge. A GUI binary has no stdout to write frame hashes to unless it
allocates a console; a console binary pops up a console window when launched
from Explorer. So `a2run` is a first-class, permanent tool rather than
scaffolding for stage 2a, and `a2emu` does not grow a `--headless` flag.

This is also what forces `lib/a2host` to be sokol-free, which turns out to cost
nothing: every sokol reference in today's `decapplib` already falls on the GUI
side of the line. `simulate_frame` consults the wall clock *only* in the
non-deterministic branch -- replay and hashing take the fixed
`1/60 * clock_freq` path; `a2_sound_submit` is guarded by `sound_enabled_`,
which is off headless; and the `stm_setup`/`stm_now` calls in `run_headless` are
vestigial, feeding a `curFrameTick_` that the deterministic branch never reads.
The host is linked against sokol today only because there was one library.

### IO ownership

Today both sides own an `a2_iostate_t`: `decapplib.c` has `static a2_iostate_t
io_` and implements `io_peek`/`io_poke` for the generated code, while
`EmuApple2` holds its own and services `ioPeek`/`ioPoke` directly.

The host takes ownership. `Emu6502::ioPeek`/`ioPoke` are already `virtual`, so
the engine shim overrides them to call the host's `io_peek`/`io_poke`. One
`a2_iostate_t` in the process, owned by the layer that also renders it, replays
keys into it and hashes it.

### Contract changes needed

1. **`run_emulated` returns a stop reason.** The interpreter needs to report
   breakpoints and `--limit`; the generated engine always returns
   "cycles expired". `run_emulated` is hand-written in `system2-inc.h`, not
   generated, so this costs nothing on the decompiled side.
2. **`engine_parse_arg(const char *) -> bool` and `engine_print_help()`.** The
   host owns the common options (`--headless`, `--frames`, `--key-file`,
   `--hash-frames`, `--no-sound`, `--fast`); engine-specific ones (`--rom`,
   `--disk1`, `--disk2`, the input binary) belong to the engine. Unrecognised
   arguments are offered to the engine before erroring.
3. **`a2_iostate_t *host_io(void)`**, so an engine shim can reach the one
   instance.
4. **Later, for Phase 1b:** a snapshot hook, implemented once in the host.

## What moves

From `lib/decapplib/decapplib.c` (723 lines):

| stays in the host | becomes engine-facing |
| --- | --- |
| CLI parsing, `print_help` | `engine_parse_arg` / `engine_print_help` hooks |
| `load_key_file`, `drain_key_presses`, `drain_kbd_file`, `push_key` | — |
| `simulate_frame`, `frame_cb`, `record_frame`, `run_headless` | calls `run_emulated`, now checking the stop reason |
| `hash_video_state`, `update_screen`, `update_screen_image` | — |
| `init_window`, `speaker_cb`, `stream_userdata_cb`, sound | — |
| `io_peek`, `io_poke`, owns `a2_iostate_t` | provided *to* the engine |
| `debug_asm`, `error_handler`, `add_watch`, `add_nondebug` | provided *to* the engine |
| `init_emulation` | calls `init_emulated` |

From `tools/a2emu/a2emu.cpp` (748 lines), `simulateFrame`, `updateScreen`,
`updateScreenImage`, `drainKBDFile`, `initWindow`, `openKBDFile` and the
overlapping half of `parseCLI` are deleted in favour of the host. What is
genuinely a2emu's own — `disasm`, `printDB`, `onWarmRestartBP`,
`initTraceCollect`, F1/F2 program loading, the debug key bindings — stays, and
needs a hook for engine-specific interactive keys.

## Staging

Every stage ends somewhere verifiable. `tests/run-tests.sh` and all four
`verify.sh` scenarios are the regression test throughout.

**Stage 0 — name the contract.** Split `system.h` into `a2engine.h` and
`a2host.h` with the call direction stated, keep `system.h` including both. No
code moves. *Verify:* everything builds, `verify.sh` 4/4 unchanged.

**Stage 1 — engine hooks.** Add the stop reason and the `engine_parse_arg` /
`engine_print_help` / `host_io` hooks, with the generated engine implementing
them trivially. The host still hosts one engine. *Verify:* `verify.sh` 4/4, and
**not one frame hash may change** — this stage is pure motion.

**Stage 2a — a headless interpreter tool.** `lib/engine6502` plus a small
executable that is host + engine6502, with no windowed UI: `--key-file`,
`--kbd-file`, `--hash-frames`, `--frames`, `--headless`, `--collect`.
Deliberately a *new* tool rather than a flag on `a2emu`, because `a2emu` has no
`main()` — sokol supplies it and calls `sokol_main` — so making it headless
means restructuring its entry point, which is a separate concern from proving
the engine split works. **This stage alone unblocks run-data regeneration.**
*Verify:* replay `play.keys`, get frame hashes, and regenerate `snake-byte.json`
and diff it against the committed 2022 copy.

**Stage 3 — the cross-check.** Replay `play.keys` through the interpreter and
diff frame hashes against `snake-bytec1`. This is the independent oracle the
project has never had. **Open-ended discovery, not a gate** — see Risks.

**Stage 2b — fold `a2emu` in.** Port its debug UI onto the host and delete its
duplicated frame loop, video, sound and CLI.

**Stage 4 — `--snapshot-at` / `--snapshot-out`** in the host, once, working for
both engines. Then Phase 1b.

**Stage 5 — rename `decapplib` → `a2host`.** Mechanical, and deliberately last
so the functional diffs stay readable.

## Risks

**Cycle accounting will probably diverge (stage 3).** The generated code adds
whole-block totals via `CYCLES(pc, n)`; the interpreter adds per-instruction and
models page-crossing and branch-taken penalties as it goes. If the block totals
were computed from the same tables these agree, but that has never been tested.
A divergence here is a finding about one of the two implementations and is worth
having — but it could absorb real time, which is why stage 3 sits after the
refactor has already banked its value rather than gating it.

**Stage 0 and 1 must be provably inert.** They touch the only thing that
currently works. If a frame hash moves, stop and understand it rather than
re-recording.

**The rename is churn.** Four `CMakeLists.txt` and the include paths. Harmless,
but it would obscure the real diffs if done early.

## Open questions

- **Should `decoded/*` also build two binaries each?** The same argument
  applies: a decompiled game is a GUI program, but `verify.sh` drives it as a
  batch tool. The symmetric end state is `snake-bytec1` (GUI) plus
  `snake-bytec1-run` (console), with `verify.sh` using the latter -- which
  would also stop the verification path linking sokol, GL and audio purely to
  run headless. Deliberately **not** part of this refactor: it doubles the six
  `decoded/` targets and mixes a build-topology change into what stages 0 and 1
  need to keep as provably inert code motion. `a2host` being sokol-free is what
  makes it possible afterwards.
- **`textemu` is a third host** (ncurses, headless-capable, `EmuApple2`-based).
  It should probably fold onto `engine6502` + host too, but it is out of scope
  here and does not block anything.
- **Does the interpreter need language-card or aux-memory parity** with the
  generated engine for the cross-check to be meaningful? Snake Byte uses
  neither, so stage 3 can proceed without answering this.
- **Where should run-data collection live** once there are two engines? It is
  `DebugState6502`, driven by the interpreter's per-instruction callback, and
  has no meaning for the generated engine. Probably stays engine-side rather
  than becoming a host feature.
