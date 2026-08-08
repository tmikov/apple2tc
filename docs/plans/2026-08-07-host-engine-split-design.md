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

snake-bytec1     (GUI)     = a2host + a2host_gui + the generated C
snake-bytec1-run (console) = a2host + the generated C
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

### Every program ships as a GUI/console pair

The subsystem argument applies to a decompiled game exactly as it does to the
emulator: `snake-bytec1` is a program someone runs, but `verify.sh` drives it as
a batch tool. So each `decoded/*` target becomes two, with `-run` as the console
suffix, matching `a2run`.

Two payoffs beyond consistency. **The verification path stops linking sokol, GL
and audio** -- today `verify.sh` pulls all of that in purely to pass
`--headless`. And **`--snapshot-at` lands where Phase 1b needs it**: the capture
has to come from the decompiled build, because that is what `play.frames` was
recorded from, and `snake-bytec1-run` is precisely that build as a console tool.

There are 8 decoded targets today (1 bolo, 2 robotron, 2 rom, 3 snake-byte), so
this is 16 executables. Two things keep that from hurting:

```cmake
# Two executables from one compile: <name> is a GUI program, <name>-run a
# console one. They cannot be a single binary with a flag -- see above.
function(add_a2_program name)
  add_library(${name}-obj OBJECT ${ARGN})
  add_executable(${name}     $<TARGET_OBJECTS:${name}-obj>)
  add_executable(${name}-run $<TARGET_OBJECTS:${name}-obj>)
  target_link_libraries(${name}     PRIVATE a2host a2host_gui)
  target_link_libraries(${name}-run PRIVATE a2host)
endfunction()
```

One call per program, so the duplication never reaches the call sites; and an
OBJECT library so the generated C -- `snake-bytec1.c` alone is ~18k lines -- is
**compiled once and linked twice** rather than built twice. Build time is
essentially unchanged.

`snake-byte`'s `HEADER_FILE_ONLY` properties on `a2rom.c`, `game.c` and
`snake-bytec1-ext.c` are directory-scoped, so they keep working across both
targets unchanged.

### IO ownership

Today both sides own an `a2_iostate_t`: `decapplib.c` has `static a2_iostate_t
io_` and implements `io_peek`/`io_poke` for the generated code, while
`EmuApple2` holds its own and services `ioPeek`/`ioPoke` directly.

The host takes ownership. `Emu6502::ioPeek`/`ioPoke` are already `virtual`, so
the engine shim overrides them to call the host's `io_peek`/`io_poke`. One
`a2_iostate_t` in the process, owned by the layer that also renders it, replays
keys into it and hashes it.

### Contract changes needed

1. **`engine_stop_reason()`, queried after each `run_emulated()`.** The
   interpreter needs to report breakpoints and `--limit`; the generated engines
   always answer "cycles expired".

   Originally specified as a return value on `run_emulated`, on the grounds that
   it is hand-written in `system2-inc.h`. That was wrong: only the `--irc1`
   engine has it there — the `--simple-c` engine has `run_emulated` *emitted* by
   `PrintSimpleC.cpp`, so changing the signature would force regenerating every
   committed `--simple-c` output (`snake-byte.c`, `robotron.c`, `rom.c`,
   `bolo.c`), none of which `decompile.sh` still regenerates. A separate query
   costs one call per frame and keeps the change confined to hand-written
   headers, which is what lets stages 0-3 stay provably inert.
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
`verify.sh` scenarios are the regression test throughout, and **stages 0-3 must
not move a single frame hash** -- they are code motion and build topology, not
behaviour.

**0 — name the contract.** Split `system.h` into `a2engine.h` and `a2host.h`
with the call direction stated; `system.h` keeps including both. No code moves.

**1 — engine hooks.** Add the stop reason, `engine_parse_arg`,
`engine_print_help` and `host_io`, with the generated engine implementing them
trivially. Still one engine.

**2 — split the host.** `lib/decapplib` becomes `lib/a2host` (sokol-free) plus
`lib/a2host_gui`. `decoded/*` still build one GUI binary each, now linking both.
This absorbs the `decapplib` → `a2host` rename, so there is no separate rename
stage.

**3 — GUI/console pairs.** Add `add_a2_program`, give every `decoded/*` target
its `-run` twin, and point `verify.sh` at the console binaries. Verification
stops depending on sokol.

**4 — `engine6502` and `a2run`.** The interpreter behind `a2engine.h`, plus the
console tool. **This is what unblocks run-data regeneration.** *Verify:*
regenerate `snake-byte.json` and diff it against the committed 2022 copy.

**5 — the cross-check.** Replay `play.keys` through `a2run` and diff frame
hashes against `snake-bytec1-run`. The independent oracle the project has never
had. **Open-ended discovery, not a gate** — see Risks.

**6 — fold `a2emu` in.** Port its debug UI onto the host; delete its duplicated
frame loop, video, sound and CLI.

**7 — `--snapshot-at` / `--snapshot-out`** in `a2host`, once, available to both
engines and in particular to `snake-bytec1-run`. Then Phase 1b.

## Risks

**Cycle accounting will probably diverge (stage 5).** The generated code adds
whole-block totals via `CYCLES(pc, n)`; the interpreter adds per-instruction and
models page-crossing and branch-taken penalties as it goes. If the block totals
were computed from the same tables these agree, but that has never been tested.
A divergence here is a finding about one of the two implementations and is worth
having — but it could absorb real time, which is why stage 5 sits after the
refactor has already banked its value rather than gating it.

**Stages 0-3 must be provably inert.** They touch the only thing that currently
works. Stage 3 changes *which binary* `verify.sh` runs, so the frame hashes it
produces are exactly the evidence that the host split preserved behaviour: if
one moves, stop and understand it rather than re-recording.

**Stage 3 doubles the executable count** from 8 to 16. The OBJECT-library form
of `add_a2_program` keeps compile time flat, but link time and build-directory
size do grow. If that becomes annoying, the console twin is the one worth
keeping and the GUI target could become opt-in per game -- `bolo` is WIP and
nobody runs `rom` interactively.

## Open questions

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
