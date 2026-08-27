# Handoff: converting Snake Byte into maintainable C

Read this first. It is the entry point for resuming the work on branch
`snake-byte`. Everything below is measured or committed — where something is a
guess, it says so.

**The directory was reorganised on 2026-08-27** (`023c28f`, `e6c9e9e`,
`scripts/` after them). The
artifact is `decoded/snake-byte/snake-byte.c` — it used to be
`snake-byte-cold.c`, and the name it now has was held by a 2022 fossil that is
`as-generated/snake-byte-simple-c.c`. Scaffolding moved into `reference/`
(the builds the gate compares against), `testdata/` (binary, recording, keys,
probes, baselines), `as-generated/` and `scripts/`. Each script now resolves
its `here` to the directory *above* itself, which is the only thing that made
moving them non-trivial. Run the gate from the game's directory, not from
`scripts/`: `./scripts/probe-acceptance.sh ../../cmake-build-debug`. The
`-cold` fixtures kept their names — they describe the scenario, not the build.

**Start here (2026-08-25).** **The machine is out of the game's code.** That
was the whole of step 6 and it is done — not the six cleanup steps, which
finished on 2026-08-24, but the thing those steps turned out to be a prologue
to. In four passes:

| | was | now | what stays |
| --- | --- | --- | --- |
| `ram_peek`/`ram_poke` | 314 | **3** | the entry-state loader |
| `s_status_*` | 222 | **62** | `c` on three edges, `d` for BCD |
| `s_a`/`s_x`/`s_y` | 307 | **15** | COUT's X/Y promise, the entry load |
| `branchTarget` | 122 | **0** | — |

Read **"Getting the machine out of the code"** in the playbook before doing any
of this to another binary. It is the method rather than the result, and the
method matters more than usual here because three of the four passes had an
approach that felt right and was wrong: narrowing a probe in the same step as
the move it accommodates, deriving liveness by reading a routine's body, and
sizing a table to the data you can see.

**What the artifact still is not.** Step 6's `CYCLES` question is **answered**:
the virtual clock landed on 2026-08-26, 838 charging sites became 23, and the
game was played on macOS to confirm it. See item 3. What is left is the loose
ends, and stage 4 of the design -- restructuring the sound routine with
understanding rather than mechanically -- which was always out of scope. The
game's own C is done.

**The gate compares 40,000 frames now, not 1,300** (2026-08-27, `0287e48`).
It was raised because a review measured what the old budget could see: two of
four structural mutations of `game_cold_start`'s loop nest survived a
20,000-frame comparison undetected, and only the screen and RAM checks at
40,000 caught them. Both sides get the same frame count — the cold build only
has to be at least as long as the booting one, and at equal frames it always
is, so nothing has to encode how long the boot takes. Coverage went from
371,352 block heads to 41.6M on `play`, and from 9,524 screen samples to
467,127 on `hires`. The gate now takes about two and a half minutes.

**A block-head count is not evidence about control flow.** Task 10's commit
cites 20,298,539 of them; 252,364 of the `hires` run's 260,128 hits are a
single address and `$7890` never fires at all. The screen and memory
comparisons are what actually constrain a rewrite of the game's structure.

**Before trusting a green run.** Two defects reached the user through a fully
green gate on 2026-08-26 -- the game hung on ESC, and three compiler warnings
-- so start from what the checks do not cover. The block-head trace compares
**six** addresses, not 113, so control flow inside the game is not compared at
all. The
screen and memory at 6,808 and 9,524 samples are the gate, and both have been
narrowed repeatedly by the state migration — the hole list at the top of
`ram-cold.probe` says what each hole cost and why. And **half of `a2rom` is
converted by reading rather than by running**: nothing scrolls and nothing
emits Ctrl-G, so `rom_wait`, `rom_fc68`'s scroll and `rom_coutz`'s bell and
backspace arms are green because the gate never enters them. They are on
probe-acceptance.sh's unverified list, which is the thing to check before
believing a pass.

**Last commit:** run `git log -1`; this line has been stale before. The tree is
clean and nothing is pushed.

The **standing decision that no further recordings will be made** is still in
force: unexercised code is decoded from the binary rather than held until
something runs it. See the 2026-08-22 log entry, and note that it supersedes a
playbook rule that said the opposite.

Before that, two decompiler capabilities — `--inline-str` and
`--alt-exit` — and the conversion they unblocked: **the main loop and the
auto-steer are now C** (`game_play_loop_native`, `game_auto_steer`). Before
that, a long run of infrastructure that did not touch the game at
all: the **host/engine split** (2026-08-11 log entry), the **probe compiler and
VM** (2026-08-12, 2026-08-13), **accurate cycle costs** in both engines
(2026-08-13), two apple2tc bug fixes (2026-08-13, 2026-08-14), and
**probe-stamped input** (2026-08-14, 2026-08-15), which is what finally made
the interpreter and the generated C comparable. All are summarised under
"Host, engines and probes" below; read that before assuming anything about how
verification runs, because it changed.

---

## The goal

Turn apple2tc's decompiler output for Snake Byte into C worth maintaining — real
functions, structured control flow, named variables. The wider aim is a
repeatable method for *any* Apple II game.

The decompiler is a bootstrap and an accelerator, **not the thing doing the
work**. Where it cannot reach a routine, decompile it by hand. Its rejections
and its code/data classifications are evidence, not verdicts.

## Working agreements

- **Decide routine-level questions yourself.** Do not escalate "what should I do
  about `$X`". Escalate only project-level forks: scope, architecture,
  verification strategy.
- **Never edit `snake-byte.json`.** It is a recording of what actually happened
  during a play session. Editing it to assert reachability the run never
  observed destroys its value as evidence.
- **Reachability the run never observed goes in `code-at.txt`**, one
  `ORIGIN TARGET` edge per line, each with the argument for why it is real. See
  the 2026-08-05 log entry: this supersedes the earlier "hand-decompile what the
  tool cannot reach, do not feed the decompiler more input" agreement, which was
  weighed and set aside once the cost of the alternative was measured.
- **Hand-decompilation is still the fallback**, for anything `--code-at` cannot
  reach — but note there is no general way to *call* hand-written code at an
  untraced address. `$664A` works only because `$FDED` is an externalised ROM
  vector that `rom_cout` dispatches on.
- **No new recordings.** The committed set (`play`, `play-hires`, `play-rebind`,
  and the `easy` fixture) is what there is. Code no recording reaches is decoded
  from the binary — `id` for the instructions, the opcode table for the cycle
  charges, `--ir` for the liveness — and never held back waiting for something
  to run it. Name the unexercised blocks in a comment above the routine, since
  converting takes them off the site list, and do not call the result verified.
  See the 2026-08-22 log entry; this supersedes the playbook rule that said to
  wait.
- **`tests/` is decompiler regression only** — hand-written `.s` assembled by
  `a6502`, decompiled, diffed against `.ir` baselines. Game data lives in
  `decoded/<game>/`.

---

## Read these, in this order

| File | What it holds |
| --- | --- |
| `docs/decompiling/playbook.md` | The transferable method: findings, procedure, red-flag table. Maturity is honestly marked — steps 1-4 have been executed, 5-6 have not. |
| `docs/decompiling/decision-log.md` | Append-only rationale. **Never edit existing entries**; add new ones and mark old ones `superseded`. Read it from the end: the 2026-08-15 entry's "Left open" list went stale for six days before anyone noticed, so trust the newest entry over any earlier one's forward-looking notes. |
| `decoded/snake-byte/labels.txt` | Established names, each with its evidence in a comment. |
| `decoded/snake-byte/code-at.txt` | The two hand-asserted reachability claims, each with its argument. Read before adding a third. |
| `docs/superpowers/specs/2026-08-02-snake-byte-proper-c-design.md` | The original design. Parts are superseded — the log says which. |

The two plans in `docs/superpowers/plans/` are executed and historical.

Infrastructure, only if you are touching how things run or are verified:

| File | What it holds |
| --- | --- |
| `docs/plans/2026-08-07-host-engine-split-design.md` | Why `a2run` exists. Leads with an outcome section that retracts part of its own reasoning — read that first, not the design below it. |
| `docs/plans/2026-08-11-probes-design.md` | Why probes exist: every other way of comparing the two engines failed, and all of them failed the same way. |
| `docs/probes.md` | The probe language reference. The design doc is the rationale; this is the *what*. |
| `decoded/rom/boot-state.txt` | The Apple II's boot state at the BASIC prompt, with nothing loaded — the floor any retargeted entry point starts from. Game-independent; captured with `--snapshot-at`. |
| `docs/plans/2026-08-11-probes-plan-1-compiler.md` | The executed plan. Its closing self-review is the most useful part — it names what part 2 inherits. |

**Read the log before acting on anything that seems obvious.** It deliberately
preserves the wrong turns so they are not repeated. See "Traps" below.

---

## Where things stand

### Decompiler (`tools/apple2tc/`)

- **Per-routine stack tracking.** `scanCandidate` used to balance the 6502 stack
  per *basic block*, rejecting the universal `PHA`-at-entry / `PLA`-before-`RTS`
  idiom. Depth now propagates along CFG edges, required zero only at `RTS`.
  Snake Byte: **53 → 75** routines.
- **A routine entry can also be a call's return point.** `scanCandidate` used to
  reject an `RTS` predecessor outright, losing every block that a caller falls
  into after a `JSR` — Snake Byte's `$60E7`, plus five more through the cascade.
  Accepting it was one line; the extraction then needed three further fixes. See
  the 2026-08-07 log entry. Snake Byte: **74 → 81** routines.
- **`--routines-report=<path>`** — every candidate, accepted or rejected, with
  block sets, call sites, dominator chains and natural loops. This is the input
  for hand-writing structured C. Note it reflects the CFG *before* `simplifyCFG`,
  so its blocks are more granular than `--irc1` output.
- **`--extern-routines=<file>`** — maps addresses to C function names; calls
  become calls to bodyless `Function`s and unreachable blocks are deleted.
  Note it rejects any address that is not already a known block.
- **`--inline-str=<file>`** — routines that take a NUL-terminated string after
  the `JSR` and return past it. Without it the tracer follows the fall-through
  into the text and decodes it as instructions, which is a wrong decompilation
  nothing can notice. Declared routines must also be in `--extern-routines`,
  because a *generated* one would still pop a return address the option has
  moved; both refusals are enforced and tested.
- **`--alt-exit`** — routines that return by discarding their return address and
  jumping into the caller (`PLA/PLA/JMP`). The call becomes `CallAlt`, a
  terminator with one successor per exit, and the routine returns which exit it
  took. Read the playbook entry before touching this: the obvious fix — relaxing
  the stack check — passes every behavioural oracle and produces a function that
  calls itself. Externalizing such a routine is refused unless every generated
  call site goes away too.
- **`--code-at=<file>`** — hand-asserted `ORIGIN TARGET` dynamic branch edges,
  merged into the runtime data before disassembly. `TARGET` is disassembled
  *and* becomes a successor of the branch at `ORIGIN`; the latter is what earns
  it an entry in the generated address-to-block map, without which it would be
  decompiled but unreachable. This is how `$7541` and the reference build's
  `$664A` are reached.
- **`Dominators.{h,cpp}`** — Cooper-Harvey-Kennedy, validated against a DAG, a
  single-loop routine and a nested delay loop. The codebase had none before.

### Host, engines and probes (`lib/a2host`, `lib/engine6502`, `tools/a2run`)

Added 2026-08-11/12. None of it touches the game; all of it changes how you run
and verify one.

**`system.h` is now two contracts.** `a2engine.h` is what an engine provides and
the host calls; `a2host.h` is what the host provides and an engine calls;
`a2host_api.h` is what the host offers a *front end*. There are two front ends —
`a2host_gui` (sokol) and a console `main` — and they are **separate executables
rather than one binary with a flag, because on Windows console versus GUI is a
link-time subsystem property**. So:

| Binary | What it is |
| --- | --- |
| `a2emu` | the windowed emulator; now 131 lines, all of it F1/F2 game loading and two gdb helpers |
| `a2run` | the same host and engine with a console `main` and no graphics |
| `<name>` / `<name>-run` | every decompiled game now builds both, via `add_a2_program` |

**Three things this bought, and they matter to the questions below:**

1. **Run data can be regenerated headlessly.** Previously impossible —
   `snake-byte.json` is a 2022 artefact of a windowed `a2emu`. Recipe in
   `decoded/snake-byte/README.md`, which is explicit that it does *not*
   reproduce that file byte for byte.
2. **`verify.sh` links no graphics at all.** It drives `snake-bytec1-run` and
   `snake-bytec1-ext-run`.
3. **`a2emu` has a test for the first time**, in `tests/run-tests.sh`: sharing
   everything but the window, `a2emu --headless` must byte-match `a2run`.

**Probes** (`lib/a2host/probe*.c`, `include/apple2tc/probe.h`) are a compiler
*and* a VM, running in every front end and in every generated program. A probe
is a small program bound to install sites, so the two engines can be compared
at points *the program* defines rather than at moments the host defines. Read
`docs/probes.md` for the language and the 2026-08-12 log entry for why.
Options: `--probe=`, `--probe-out=`, `--probe-dump`, `--record-keys=`.

`decoded/rom/probe-acceptance.sh <build-dir>` is the cross-engine gate: the
interpreter against both generated back ends over 120 frames of ROM boot,
276,255 probe hits each, byte-identical. Run it alongside `verify.sh`.

`decoded/snake-byte/probe-acceptance.sh <build-dir>` is its Snake Byte
counterpart and the stronger one. It replays probe-stamped input (`play.pkeys`,
`play-hires.pkeys` — not cycle-stamped; item 5 under "Next") through both of
`verify.sh`'s scenarios, and checks two things per scenario per back end:

| | play | hires |
|---|---|---|
| block-head trace | 2,744,938 hits identical | 2,864,242 identical |
| memory (`ram.probe`) | identical at 6,808 samples | identical at 8,465 |

The memory check is not redundant with the trace: a wrong byte written need not
change control flow within one run. The hires scenario is not redundant with
play: it reaches `$664A` and `$7541`, code the recording never took, and in the
`-ext` build `$664A` is hand-written C in `game.c` — so hires/ext is the only
cross-engine check that hand-written replacement gets.

**The hazard, stated once here because it is easy to lose:** `CYCLES` is emitted
per basic block, not per instruction, so a probe installed at a non-block-head
address fires under `a2emu`/`a2run` and *does not exist* in a generated program
— and the report still reads as agreement. Cross-engine comparison must install
from `@"file"`, using a block-head list. The list is grepped out of the
generated C, where `CYCLES(0x` names block heads and `CYCLES_EDGE(0x` names
taken-branch edges, which deliberately do not dispatch.

### Tooling (`tools/id/`)

`id` is a REPL reading stdin, so it scripts directly:

```bash
printf 'loadb33 snake-byte.b33\nlabels labels.txt\ndis $664A $66A8\n' | id
```

It resolves the built-in Apple II symbol database (`COUT1`, `BASH`, `CSWL`).
`dis` takes a count or a start/end range. `labels <file>` loads `ADDR name` pairs
that override the built-in names. **Use this instead of decoding bytes by eye.**

### Snake Byte (`decoded/snake-byte/`)

Five targets, and they are not five copies of the same thing.
`decoded/snake-byte/README.md` now carries the full file inventory; this is the
short version.

| Target | Source | Role |
| --- | --- | --- |
| `snake-byte` | `snake-byte.c` | **The artifact this work is aimed at.** One translation unit, 7,219 lines plus two memory-image includes, sharing nothing. Entered at `$3750`; no boot, no ROM code, no generated code. |
| `snake-bytec1-ext` | `snake-byte-ext.c` | The verified reference. Boots, types `CALL 14160`. ROM entry points from hand-written `a2rom.c` + `game.c`. |
| `snake-bytec1` | `snake-bytec1.c` | Self-contained control. ROM decompiled alongside the game; links alone. `play.frames` was recorded from it. |
| `snake-byte-easyc1-ext` | `snake-byte-easy-ext.c` | Test fixture, not a variant: apple quota 16 -> 2. |
| `snake-byte` | `snake-byte.c` | Historical `--simple-c` output, no longer regenerated. |

**Both are gated now.** `probe-acceptance.sh` compares cold against ext over
**two scenarios, six checks**, aligned at the first `$3750`:

| | play | hires |
| --- | --- | --- |
| block-head trace | 371,352 | 10,085 |
| screen | 6,808 samples | 9,524 |
| memory (`ram-cold.probe`) | 6,808 | 9,524 |

**A retracted claim, kept because the mistake is worth knowing.** On
2026-08-25 this paragraph said `$664A` "runs zero times again, and nobody
noticed", with counts to back it up. That was wrong. Re-measured with
`--probe` present, the hi-res hook runs **exactly 205 times** and
`game_install_cout_vector` once, as the paragraph below always said.

The cause: `--key-file` without `--probe` cannot honour a `.pkeys` stamp,
because the replay coordinate is a counter the *probe script* defines. Drop the
probe and the keys land at the wrong moments, the game never leaves its
self-playing demo, and every number you take is wrong while looking entirely
plausible. **Any ad-hoc run against a `.pkeys` file needs `--probe` too.**

`hires` was added 2026-08-23 and is not optional decoration: without it the
cold build ran `game_cout_hook_native` **zero** times under the gate. Its keys
are derived, not recorded — `scripts/make-cold-keys.sh testdata/play-hires.pkeys
testdata/play-hires-cold.pkeys 181207`.

Both probes sample at `$6217` **and `$760F`**. `$6217` is the in-game keyboard
ingest and `$760F` is the redefinition screen's; sampling only the first meant
the whole hi-res path was traced but never sampled, and a mutation that drew
205 wrong glyphs passed every check. Do not drop either site. The control is ext rather than the interpreter because there is no
interpreter that starts at `$3750`; ext earns the role by being checked against
the interpreter a few dozen lines earlier in the same script.

That pair checks only what is cold-specific — entry state, pruning, key offset.
Both builds share `game.c` and `game_native.c`, so a bug in the game's own C
changes both identically and is caught upstream, not here.

**The adapters in `game.c` are now mostly scaffolding, not an interface.**
Measured 2026-08-23: of the 42, only **14 still have a generated caller**
(`game_place_apple` has 5, `game_draw_status` and `game_plot_shape` 2 each, the
rest 1). For the other 28 every caller is hand-written C that could call the
`_native` function directly. What they still buy is verification, not
structure:

- `CYCLES(addr, 0)` keeps the entry address on the block-head list, so the
  cross-engine trace still compares it. Delete the adapter and that comparison
  point disappears silently.
- `push16(ret)`/`pop16()` keeps the emulated stack matching the 6502's, which
  `ram.probe` compares and which the inline-string idiom actually *reads*.
- Three do real work rather than marshalling — see the playbook's red flag.

So they are exactly what a shipped artifact sheds (`ret_addr == 0`, no emulated
stack, no probe sites), and exactly what Phase 1b makes redundant in bulk. Do
not delete them one at a time before then; the site count is the thing that
notices.

One was fully dead and is gone: `game_rand_byte` had no caller anywhere and no
`CYCLES` site. Its `$6C4B` mapping went with it — a mapping for a routine no
generated code calls buys nothing, and is not how a routine gets deleted.

**`./decompile.sh` reproduces the committed generated C, and this is now
gated.** `probe-acceptance.sh` regenerates into a temporary directory and
compares, before it runs anything else — because everything else in that script
runs the *committed* `.c` files, so a stale artifact would leave every PASS a
statement about the wrong file, site counts included. When it fails it says
which kind of drift it is: whitespace-stripped identical means layout only,
differing tokens mean the decompiler's output really changed.

It takes a build directory and an optional output directory
(`scripts/decompile.sh [build-dir] [out-dir]`), reads its inputs relative to the
directory above the script rather than the caller's cwd, and defaults to
regenerating in place — into `reference/` and `testdata/`, mirroring the
committed layout.

**There is no such thing as decompiler-generated code here** (2026-08-24). It
was treated as a category once, as a reason to defer moving `$0024/$0025` until
the ROM helpers were "owned"; that was wrong and the plan reads better without
it. Everything in `snake-byte.c` is a decompilation of one binary, and a
routine still emitted as a switch over block ids is a less finished one, not a
different kind. All of them are C now, and **the `bb_N:` count is 0.** There is no
decompiler-shaped code left in the file at all — not in the game, not in the
ROM. BASCALC, VTABZ, CLREOL, CLREOLZ, WAIT, PLOT1, SETCOL, SCRN, GBASCALC,
PLOT, COUT1, HLINE/VLINEZ, HOME, the line-feed tail and COUTZ went on
2026-08-24.

Decimal mode is **not** asserted away in ROM routines the way it is in game
ones. `game_bonus_screen` is entered with `D` set and it prints, so COUT can
reach BASCALC and VTABZ in decimal mode. Keep both arms.

Where a routine's control flow genuinely rejoins at several depths — COUTZ's
dispatch, HLINE falling into VLINEZ, HOME's provably-always-taken BCS — the
labels keep their addresses and get names. It is not a failure to structure
them further; it is what the code is.

`game_cold_start` looked like one of those and was not (2026-08-27). Its 21
gotos and 13 labels are now four nested `for (;;)` loops — new game, new
level, round, life — with zero labels. Twelve of the thirteen labels were
reached from a single nesting depth; the thirteenth, `$3783: JMP $76C2`, is
the cold entry three levels down, which deliberately skips $7691's and
$76B7's initialisation so the demo pass runs on the image's own level, score
and lives. That is expressed by putting the two outer loops' initialisation
at the *bottom* of their bodies, where the old `goto new_game`/`goto
new_level` already fell out to, so entering from the top skips it exactly
once with no flag. The cost is $76B7's two lines appearing twice, because
$7691 falls into $76B7. A `RoundEnd` enum carries the three ways out of the
life loop, since C cannot break three levels. Commit 1fb69a8's message carries
the full map — every label, every goto that reaches it, and its depth.

**What is decoded and never run is now recorded per routine.** Measured by
probing entry addresses across both cold scenarios, not assumed:

| never fires in either scenario | why |
| --- | --- |
| `rom_clreol` | its only caller is the scroll path; nothing scrolls |
| `rom_wait` | called only from BELL1; nothing emits Ctrl-G |
| COUTZ's Ctrl-S handshake, bell and backspace | the game prints only printable characters, returns and line feeds |

`a2rom.c` and `game.c` are **`#include`d, never compiled separately**.
`system2-inc.h` defines the machine state (`s_ram`, `s_a`, the `CYCLES` macro)
with internal linkage, so a second translation unit would get its *own* copy of
everything and link cleanly while being silently wrong. Both are marked
`HEADER_FILE_ONLY` in CMake to prevent exactly that.

- `a2rom.c` — the 9 ROM entry points, plus `GBASCALC`, `PLOT1` and the `$FB78`
  `COUTZ` subtree, which were deleted as collateral when the 9 became external.
- `game.c` — hand-decompiled *game* routines. Currently just `$664A`. Keep it
  separate from `a2rom.c`; ROM replacements and game code are different things.

---

## Build and verify

**To actually play it**, which is not something any check above does:

```bash
ninja -C cmake-build-release
cmake-build-release/decoded/snake-byte/snake-byte
```

It opens on `APPLE ][` and `]CALL 14160` for about an eighth of a second. That
is not a hang and not a boot -- the entry snapshot was taken at `$3750` with
BASIC's text page still on screen, so the cold build inherits it and then
switches to hi-res. Confirmed working 2026-08-25.

**The wall-clock path now has a test, and did not for nine days.** Every other
check in this file is fixed-step by construction -- `--key-file`,
`--hash-frames` and `--headless` all set `a2host_fixed_step()` -- because
reproducibility is what each exists for, so the mode a person actually uses was
covered by nothing and was totally broken without anyone noticing.
`tests/pacing` covers it now: a stub engine drives a2host with no window and
asserts that a repaint reporting elapsed time runs a non-zero cycle budget.
`run-tests.sh` runs it.

Still launch the game after touching `lib/a2host`. The test covers the pacing
arithmetic, not the window, the renderer or the audio. See the 2026-08-25
"Four oracles" log entry.

```bash
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
ninja -C cmake-build-debug

cd tests && ./run-tests.sh ../cmake-build-debug     # expect: Success!
cd decoded/snake-byte && ./verify.sh ../../cmake-build-debug   # expect: 4x PASS
./probe-acceptance.sh ../../cmake-build-debug                 # the real gate: 28 PASS
./decompile.sh                                                # regenerates the c1 variants
```

`decompile.sh` reproduces the committed generated C exactly: run it at a clean
HEAD and `git status` comes back empty. `probe-acceptance.sh` checks that before
it checks anything else, because everything else it does runs the committed
`.c` files. Note `snake-byte.c` is *not* regenerated and never will be —
it is the decompilation, owned by hand.

`run-tests.sh` is no longer only decompiler regression. It also asserts that
`a2emu --headless` byte-matches `a2run`, and carries the probe compiler's 4
baseline diffs and 63 rejection assertions. Both were verified green at
`4ffb88a`, as was `verify.sh` at 4/4.

`verify.sh` replays two scenarios against both builds:

```
PASS [play/ref]:  1300 frames match     ordinary game
PASS [play/ext]:  1300 frames match
PASS [hires/ref]: 1300 frames match     C at attract -> redefine all six keys
PASS [hires/ext]: 1300 frames match
```

It replays cycle-stamped keys headless and compares per-frame FNV-1a hashes of
video memory (mode + text page + hi-res page) *and* cycle counts. `--record`
re-records both traces from the reference build, twice each, and refuses to
write unless the two runs agree.

**Never edit a `.frames` file to make a test pass.** The trace is the reference.

`[hires]` is a real cross-check, not a regression test: the reference build runs
a decompiler-generated `$664A`, while in the ext build `$FDED` is externalised,
which deletes that block and routes COUT through hand-written `game_cout_hook`.
The two builds run different code and must agree frame for frame.

---

## Coverage — what is left

**Measured, not estimated.** `decompile.sh` regenerates
`decoded/snake-byte/coverage.txt` alongside the C, so it cannot go stale:

```
range $3750-$854E  19967 bytes
  code              4244   21.3%
  declared data    13046   65.3%
  unknown zero      2327   11.7%
  unknown nonzero    350    1.8%   <- what is left to identify
```

`known-data.txt` declares the identified non-code regions, each with its
evidence; the report flags any of them the disassembler also reached as code, so
a wrong declaration cannot sit there quietly. The 2026-08-04 figure of 2,657
unknown bytes was a hand count that lumped assets, tables and buffers together.

The largest remaining gaps:

```
$799B-$7FFF  1637 bytes,  275 nonzero   84% zero -- a buffer with scattered state
$7680-$7690    17 bytes,   17 nonzero
$6633-$6640    14 bytes,   14 nonzero
$6232-$6255    36 bytes,   11 nonzero   near buf_keys
$616B-$6173     9 bytes,    8 nonzero   unreached *code*, not data: LDA $C000 /
                                        STA $C010 / STA $09 / RTS
```

then a long tail of runs under 10 bytes. Nothing left is a cluster; this is
variables and small tables, not undiscovered subsystems.

---

## `$7541` — done (2026-08-05)

Reached via `--code-at` (`7251 7541`) rather than hand-decompiled; see the
2026-08-05 log entry for why, and `code-at.txt` for the argument that the edge
is real. It is the **key-redefinition screen**:

```
        ABSOLUTE            RELATIVE
       DIRECTION           DIRECTION
           b                             b/c/d/e are font glyphs $E2-$E5,
         d + e               d   e       drawn as arrows by $664A
           c
      PRESS THE KEY FOR THIS FUNCTION
```

Six slots — absolute up/left/right/down, then relative turn-left/turn-right —
over the key table at `$6C63` (`C9 CA CB CD 88 95` = `I J K M ← →`). For each
slot it prints the current key (`$7590`), blinks the slot's arrow while polling
for a new one (`$75D1`), accepts anything `>= $A1` or the two arrows (`$761C`),
writes it back, and finally restores CSWL/CSWH to `$FDF0` at `$7587`.

`play-hires.keys` now presses six keys after `C` and `play-hires.frames` covers
1,300 frames, up from 390.

### Next

Items 1-4 below are all struck through now. What actually remains, in order:

**a. ~~Put `snake-byte` in `probe-acceptance.sh`.~~** Done 2026-08-23.

**b. ~~Convert the last 89 blocks.~~** Done 2026-08-23 — 377 lines of C in
place of 968 lines of dispatch. **The cold build contains no
decompiler-generated code at all.** What is left that is not the game is data
and runtime: `s_mem_3750` (the game's own image), `s_mem_d000` (ROM bytes the
death pause reads at `$E000` as delay lengths), five ROM helpers `a2rom.c`
calls, and the machine definition.

**And then the sharing went away.** `snake-byte.c` is a single
self-contained translation unit: the pruned body, `a2rom.c`, `game_native.c`,
`game.c` and the top level, each as this target's own copy, with the entry
state inlined. The other four targets keep the shared originals and are
unaffected.

The two memory images came back out on 2026-08-23 — `game-image.inc`
(`$3750-$854E`) and `rom-image.inc` (`$D000-$FFFF`), 2,020 lines of hex
between them, leaving 7,219 lines of actual C. They are `#include`d from
exactly one place and define statics, so they cannot become shared by
accident. The split is behaviour-preserving by construction and was checked
that way rather than only by the gate: compiling before and after gives
identical non-debug sections and two differing instructions, both `__LINE__`
immediates for assertions. The gate was run anyway and is green.

That was forced rather than chosen. While the file was shared, every change had
to stay safe for builds still running a generated dispatch over the same
addresses — the top-level conversion had already had to be split into a separate
file for exactly that reason. Cold owns its code now, and the gate is what says
whether the game still behaves.

**The gate is only as wide as its scenarios, and this was measured the hard
way.** Before 2026-08-23 the cold gate ran `play` only, which never presses
`C` — so `$664A` and the whole redefinition path ran zero times under it, and
a mutation there was caught by nothing. Adding `hires` was half the fix; the
other half was that both state probes sampled only at `$6217`, an address that
path never reaches, so the new scenario bought trace coverage and no state
coverage. **Before trusting the gate on a routine, check that a scenario runs
it *and* that a sample point fires while it does.** A probe with plenty of
output, all of it from the states you were not worried about, reads exactly
like coverage.

**Know which half of the gate a change spends.** The cold gate compares against
`snake-bytec1-ext`, which still shares the originals. Refactoring inside the
single file stays fully checked while it preserves behaviour *and* block-head
order. The moment a change deliberately breaks the second — dropping the
emulated stack, moving storage out of RAM — the trace half stops applying and
the screen half is all that is left. `screen.probe` was built for exactly that
moment; see its header.

**Dead code does not announce itself here: the build has no `-Wall`.** Three
functions were found by hand on 2026-08-23 -- `addr_to_block_id` and its
comparator, left behind when the dispatch went, and `game_hi_cmp`, which had
been dead in the shared `game.c` since its call sites moved into a converted
routine. The check is a manual compile, and it must be a real one:

```bash
gcc -c -o /dev/null -Wall -I../../include -I. snake-byte.c
```

**Count `-Wunused-but-set-variable` too.** It is a different warning from
`-Wunused-variable` and a grep for the latter misses it. It caught eight dead
locals on 2026-08-24 -- `scanline`, `dot_idx` and the hi-res pointer's low byte
in the cell drawers, which were faithful copies of `$06`/`$07`/`$04` while
those were emulated RAM and became write-only the moment the storage moved.
Expect more of these as storage keeps moving: a store that was observable
through the memory hash becomes dead code, and only this warning says so.

`-fsyntax-only` does *not* report unused functions and comes back clean, which
is how one survived a check that looked like it covered this. Turning `-Wall`
on for the generated targets is a separate decision: a generated block nothing
jumps to still gets a label, so they emit dozens of unused-label warnings.

Two conditions came out inverted and neither oracle would have caught them
without help: at `$7851` the original *branches away* when the value is not
`$FE`, so equality is the fall-through; and the paddle button at `$788E` reads
with bit 7 clear when pressed. The technique that found them is worth reusing —
temporarily respell every `GAME_CYCLES` in the converted C as
`GAME_CYCLES_ANCHOR`, which keeps the probe, and the converted code becomes
comparable block-for-block against the generated original. Take the edges out
first: the original's `CYCLES_EDGE` addresses must stay unprobed or they show as
phantom differences.

### Cleaning up the C

**Status, 2026-08-25.** Steps 0 through 5 are done, and so is everything the
plan's step 6 was standing in front of — see "Getting the machine out of the
code" below. What is left of step 6 itself is the `CYCLES` question, which is
not what the plan said it was: item 3.

**Read a count here honestly.** "Zero `ram_peek(0x...)`" was once reported as
if the emulated machine had gone. It had not — that number counted *hex
literals only*, and `ram_peek(kApplesLeft)` is the same access with a name on
it. The column below is every form, which is why it started at 314 rather than
at the 0 that was being celebrated. Pick the measure that cannot be satisfied
by a rename.

Measured 2026-08-25 over `snake-byte.c` with comments stripped, so a
mention inside an explanation cannot inflate a row.

**The memory row was wrong for a day and the way it was wrong is instructive.**
It counted `ram_peek`/`ram_poke` -- the *externally linked* spelling -- and
reported 3. The file reads memory through the `static` `peek`/`poke` as well,
which is 20 more calls. That is the trap two rows below this one, arriving
from a new direction: a measure that a *different spelling* satisfies is no
better than one a rename satisfies. Count the thing, not the identifier.

What the 19 real `s_ram` accesses are -- the remaining four `peek()` calls
address `$C0xx`, which `peek` routes to `io_peek`, so they are not memory:

| what | n | can it go? |
| --- | --- | --- |
| video memory: text, lo-res and hi-res screen | 8 | no -- that *is* the screen |
| the loaded image, read through a pointer: display lists, inline strings, `game_rand_byte`'s walk | 4 | maybe, as const arrays |
| ROM bytes read as data (`$E000`, the death pause's delay lengths) | 1 | no |
| the cold start's `$3800`->`$1800` relocation | 1 | no -- the game moves its own data |
| loading the images and the entry state into `s_ram` | 5 | no |


```
                                  now     was    note
memory accesses into s_ram          19    314   see below -- and note that the
                                                "3" this row said until
                                                2026-08-25 counted one spelling
s_status_* references                0    222   both survivors went 2026-08-25
s_a / s_x / s_y references           0    307
s_sp, push8/pop8                     0     42   the emulated stack
branchTarget                         0    122   never read; 2 left inside one macro
ram_peek(0x...) with a hex literal    0    115
bb_N: labels                          0    141
tmpN_U8 temporaries                   0
dead `ret` / `ret_addr` parameters    0    121   one real `ret_addr` remains, and it
                                                is an argument: the inline string's
                                                address, which the original popped
TICK, GAME_CYCLES               (gone)          838 charging sites before the
                                                virtual clock; both macros are
                                                gone, folded away or deleted
advance statements                  23          the whole of the clock: every site
                                                where a duration is perceptible,
                                                plus two that only yield
compiler warnings of its own          0          under cc, clang and gcc; gated by
                                                [warn]. One dead `ovf8` remains
                                                at -Wall and predates the file
adapters left                         0     42
lines                            5,806  16,195   the generated ext build, for scale
```

What remains, in order of how well-defined it is:

0. ~~**What the plan's six steps did not cover.**~~ **Done 2026-08-24/25.** The
   game's variables are C variables, the plotting state is threaded, the status
   flags and registers are audited, and the lookup tables are arrays. See
   "Getting the machine out of the code" below, which is the method rather than
   the fact that it is finished.

1. ~~**Step 4's adapters.**~~ **Done 2026-08-24 — there are none left.** The
   last six merged into their natives or inlined at their single call site;
   `game_draw_cell` had three callers so it merged. Not one comparison point
   was spent doing it: the pin held at 113 throughout and the block-head counts
   are identical to the digit, 421,698 and 95,140.

   The lesson, if more marshalling ever appears: an adapter *deleted* costs the
   trace its site and moves the pin; an adapter *merged* — charge, `CYCLES`
   site and write-back all moving inside the routine it describes — costs
   nothing.
2. ~~**Step 4's remainder: `s_a`/`s_x`/`s_y`.**~~ **Done.** 307 references are
   **15**, and all 15 are load-bearing. See "The registers" below.
3. **Step 6 -- done, and confirmed by playing it.** `TICK` turned out
   to be doing three jobs, and the one that matters is not obvious: **it is the
   coroutine's suspend point.** The game runs on its own thread;
   `cycles_expired()` parks it wherever it is and hands control to the host,
   which draws and polls the keyboard in that window. Delete all 839 and
   nothing hands control back -- no drawing, no input, `--frames` never ends.

   The design is `docs/superpowers/specs/2026-08-25-snake-byte-virtual-clock-design.md`
   and the plan is `docs/superpowers/plans/2026-08-25-virtual-clock.md`. In
   short: one virtual clock in 6502 cycles, advanced only where a duration is
   *perceptible* -- audible, or visible as a pause -- which is about ten sites.
   The other ~830 go. The coroutine machinery is untouched, so there is no
   yield API to build and no control inversion.

   Why cycles stay the unit: the Apple II speaker is one bit, reading `$C030`
   flips it, so pitch *is* the interval between reads. A 1 kHz tone is a toggle
   every ~510 cycles against a 17,030-cycle frame. Sound needs cycle
   resolution, and `a2_sound_spkr()` already takes cycles -- so the audio
   timeline and the pacing clock are the same clock.

   **Task 1 is done** (commit `992878e`): `speaker_access()` records every
   `$C0xx` access and the cycle it happened on, `toggle-play.txt` and
   `toggle-hires.txt` are the baselines, and probe-acceptance.sh checks them.
   **The gate is 28 checks now, not 26.**

   **The toggle baselines must be recorded with `--probe`, and this cost most
   of an afternoon.** The replay coordinate is a counter the *probe script*
   defines, so `--key-file` without `--probe` cannot honour a `.pkeys` stamp:
   the keys land at the wrong moments, the game never leaves its self-playing
   demo, and every measurement taken that way is wrong while looking entirely
   plausible. Two "findings" of mine on 2026-08-26 were nothing but this, and
   both are retracted:

   - "`$664A` runs 0 times against the claimed 205." It runs **exactly 205**.
     The number in this file was right.
   - "Neither scenario produces audible sound." `play` produces 114,800
     audible passes and its toggle dump spans ports `$30` and `$20`.

   What is actually true: `play` plays, with sound. `hires` presses `C` at the
   attract screen and edits keys instead of playing, so it stays in demo mode
   and all its clicks go to `$20`, the cassette output. That is the scenario
   behaving as designed, not a gap.

   **Task 2 is done** (commit `3aa669e`), and it is the method the rest
   follows. `game_clear_hgr_native` charged 8,192 times inside its page loop
   and now charges once, `TICK(106897)`, at the top. The constant is measured
   rather than summed by hand: instrument the entry and exit, run both cold
   scenarios, and confirm the number is the same at *every* call -- it was, at
   all five, which is what licenses treating the cost as a constant.

   Two results from it worth carrying forward:

   - **The gate did not move.** 28 PASS, both toggle timelines byte-identical,
     both cold screen probes still matching at 6,808 and 9,524 samples. So no
     screen sample lands inside the fill, and this is the first evidence for
     the design's hypothesis that the game's logic does not depend on the
     clock. It is evidence, not proof -- the routine has no input poll.
   - **What a collapse spends is the animation, not the duration.** The host
     draws about six frames while the single charge is absorbed, and all six
     now show a cleared page rather than one wiping downward. That was agreed
     in the design. The 105 ms itself survives, which is what matters, since
     `$7056` follows with a 1.29 s hold and the two read as one pause.

   **Tasks 3 and 4 are done too** (`31a40a3`, `c5f219b`, `f021230`,
   `f0ef213`). The drawing routines were measured and folded, and the
   seventeen charges that are load bearing are now spelled `advance(n)`
   instead of `TICK(n)` -- a macro that expands to the same thing, so that
   deleting one is a visible act. `advance` marks two different failures:
   a charge that sets a pitch (the tone loop, the apple sweep, the death
   buzz, the pace loop's click) and a charge that is the only yield in a loop
   that waits for input (the setup prompt, the death pause, both halves of
   the redefinition blink). See the macro's comment in the file.

   **Measure self cost, not total.** This is the trap in task 3 and it is
   easy to fall into: `draw_border`'s 73,939 cycles are almost all the seven
   line drawers it calls, and those still charge. Collapsing to the total
   would have counted them twice. The instrumentation that gets this right is
   an accumulator that only counts the routine's *own* charges, saved and
   zeroed on entry and restored on exit, with `__attribute__((cleanup))` so
   that early returns still report -- four of these routines have them.

   **Do not trust a cost that is constant only in the recordings.**
   `rom_clreolz` measures 567 at all 32 calls and `rom_hline` 642 at all 168,
   but both are loops whose trip count depends on their arguments; the
   scenarios simply never vary them. They were left alone. The same reasoning
   is why the three line drawers carry a formula (22n + 5, 28n + 6) in their
   comments rather than the number that was measured.

   **What the collapses cost, measured rather than assumed.** Over both cold
   scenarios and over a 4,000-frame free run with no keys at all: the run's
   total cycle count and its *final* frame hash are byte-identical to before
   -- 22,147,950 and 68,182,955 -- so nothing drifts and nothing accumulates.
   What moves is where the clock crosses a frame boundary: 170 and 287 frames
   of 1,300 changed their hash, the longest stretch with the game parked went
   from 1 frame to 7, and the free run shows 13 fewer distinct images out of
   1,065. All of that is `game_clear_hgr_native`'s 106,897-cycle charge and
   its siblings, and all of it is the fills no longer being drawn
   progressively. It is the frame-hash oracle the design already gave up, now
   with a number on it. Compare the 2026-08-25 note below on merging adjacent
   `TICK`s, which was rejected for a *fraction* of this -- the difference is
   that this buys the design and that bought 49 statements.

   **The collapse is done: 838 charging sites, and now 23** (commits
   `7451af0`, `3d8c85c`, `a886caf`). There is no `TICK` in the file at all --
   not the macro either, which was only ever `advance` under an older name --
   and no `GAME_CYCLES`.

   **The last fifteen were found late, and only because they were spelled
   differently.** `GAME_CYCLES` survived the collapse untouched because every
   grep was for `TICK`, and they turned out to be the expensive ones: not for
   their cycles, which nothing perceives, but for the *addresses* they needed.
   Fifteen charges were holding up four tables, six of `kNeighbour`'s eight
   fields, and eight parameters across ten call sites. Removing them turned
   `game_promote_high_score` from three parallel address tables into a loop
   that compares two numbers, and the absolute-key dispatch from a struct of
   five address fields wrapped around one key into an array of four keys.
   Address literals in code went 82 to 18, and the 18 left are load-bearing.

   Nothing perceptible moved: the snake step is 113,974 cycles either way and
   the audible pitch is identical to four decimal places, because every charge
   removed sits outside the pace loop and so shifts the silence between notes
   rather than the notes.

   The plan's tasks 2-4 only fold the drawing routines, which is 48 sites, and
   for a while this file said the plan therefore could not reach its goal.
   That was wrong, and worth recording as a mistake rather than quietly
   fixing: the spec expresses an *idea* -- most charges are arithmetic that
   happens to consume time, a few actively advance it, remove the first group
   -- and how to get there was for the work to figure out, not something the
   plan had to enumerate. Reading a plan's task list as the boundary of the
   task is how you end up delivering 6% of a design and calling it blocked.

   **Why deleting the clock is safe, which is not obvious.** The cold gate is
   keyed on *code events*, not on time. `screen.probe` and `ram-cold.probe`
   sample at `$6217` and `$760F` -- the game's own keyboard reads -- and the
   replay stamps ride a counter incremented at those same addresses.
   `screen.probe` has a section headed "Why not frame hashes". So the design's
   hypothesis is not merely plausible, it is what the probe language was built
   for: all six cold checks pass unchanged, and `$6217` fires 6,808 times
   before and after.

   **Calibration is per site: each `advance` takes the measured cost of the
   region from itself to the next charge.** Three ways that goes wrong, all
   found by measurement after a check went red:

   - **A region can contain a surviving `GAME_CYCLES_COORD`, and those charge
     for themselves.** Counting them in the `advance` too made the death pause
     cost 24 cycles a poll instead of 16. Two of the 21 sites are deliberately
     *below* their measured region for this reason. Nothing caught it but the
     block-head trace running 22% short -- and that check nearly did not catch
     it either, because its margin was 5%.
   - **The tone loop needs three numbers.** A silent pass is 38 cycles, one
     counting toward a click is 50, and while the game is audible it is 61.
     38 against 50 is most of a semitone.
   - **Redrawing the walls is the snake's tempo.** 28,848 cycles, once per
     step, constant at all 82 calls. Delete it and a step falls from 115 ms to
     84 and the game plays a quarter faster. The design's "drawing becomes
     instant, agreed" was about giving up the *sight* of a fill sweeping down
     the screen; it is a different question when the main loop is paced by the
     redraw. This is the one site the spec's own survivor list does not have.

   **What it cost, measured against the build this work started from.** The
   play scenario makes 573 speaker accesses before and after, in an identical
   port sequence; the audible tone is 0.3% flat at the median and 4.3% at the
   worst, inside a semitone. A snake step is 111.7 ms against 114.9. The
   toggle baselines are **re-recorded**, because the waveform really does move
   by a few percent -- they still catch the next regression, but they no
   longer certify this change, and the ear is what certifies it.

   **Playing it found a hang, and the hang was the predicted one** (commit
   `3d8c85c`, 2026-08-26). The collapse deleted the charge inside two loops
   that spin on `$C000` -- the ESC pause at `$69AD` and the ROM's Ctrl-S
   screen hold at `$FB88`. A charge is the coroutine's only suspend point, so
   both waited for a key the host was never given a turn to deliver: ESC froze
   the game and a second ESC could not reach it. Each got back the seven
   cycles a pass cost.

   **The cause was trusting the design's survivor table.** It lists the tone
   loop, the pace loop, `spin`, the setup prompt, the death pause, the blink
   loops and `rom_wait`, and neither of these -- the same way it does not
   mention the walls. A list written before the work is a starting point; the
   set has to be derived from the code.

   It is derived now. **`yield-lint.awk`** brace-matches every loop in the file
   and fails if one reads the keyboard, its strobe or a button without
   containing an `advance()` or a `GAME_CYCLES`. From a clean checkout it finds
   exactly those two and nothing else. Beside it, `esc-pause.keys` presses ESC
   78 times across a run and the run must terminate; both are mutation-tested.
   The live one degrades silently if a timing change stops any ESC landing in
   the play loop -- the lint is the one that does not, which is why it is the
   guarantee and the other is a smoke test. **The gate is 30 checks.**

   Two things worth keeping from how it was found. The toggle baselines did
   not move at all, because no scenario presses ESC or Ctrl-S -- the hang was
   in code the gate has never once entered. And the watchdog for the live
   check polls with `kill -0` rather than a `sleep N; kill` subshell: that
   subshell outlives a check finishing in a second and then fires into a
   recycled pid, which killed `probe-acceptance.sh` during its own first run.
   macOS has no `timeout(1)`, so neither uses one.

   **And three C23-extension warnings** (commit `e6b74bf`). Deleting the charge
   that followed a label left `done:` and `out:` ending their functions with no
   statement, and `last_line:` followed by a declaration. Two were mine; `out:`
   was already bare.

   The lesson is not the labels, it is **which compiler you look at**: gcc
   diagnoses none of these and clang diagnoses all three, so a Linux box stays
   green while the Mac prints three warnings. The `[warn]` check therefore runs
   *every* C compiler it can find rather than the one CMake configured, because
   checking the configured one would have reproduced the original failure. It
   uses default warnings, not `-Wall`, to match the real build; `-Wall` adds one
   pre-existing dead `ovf8`, left alone deliberately.

   A second thing worth carrying: I first reported all three as a regression on
   the strength of a comparison **that had never compiled**. The old file was
   built from a scratch directory, a relative `#include` failed, and the run
   aborted before producing any diagnostic. Zero warnings out of a compiler that
   never ran looks exactly like zero warnings out of a clean file. Check that a
   comparison actually did the work before believing what it says.

   **Task 5 is done: the game plays correctly on macOS** (2026-08-26), with
   the sound right and ESC pausing and resuming. **The virtual-clock arc is
   complete.**

   It is worth being exact about what that confirmation is worth, because it
   is the only evidence for most of this work. The toggle oracle was
   re-recorded, so it does not certify the collapse; the measurements say the
   tone is inside a semitone and the tempo inside 3%, but those are arguments.
   Someone playing it is the verdict, and both defects that reached the user
   -- the ESC hang and three compiler warnings -- were found that way and by
   nothing else. The mechanical checks that ran alongside (release target
   links, 6,000-frame free run terminates, longest parked stretch 2 frames)
   were all green while the game hung.

   Two of the three checks added since exist to close that gap where it can be
   closed: `[yield]` derives the set of loops that must be able to suspend, and
   `[warn]` runs every compiler rather than the configured one. Neither would
   have caught the *sound* being wrong. That still needs ears.

4. **Step 6's old entry below is superseded** by item 3. Its `CYCLES` table
   of 130 probing / 721 charge-only sites predates the conversions and is
   stale. As of 2026-08-27 the file has **no** `TICK` and no `GAME_CYCLES` --
   both names are gone -- **23** `advance` (an addressless charge; 838 `TICK`
   before the virtual-clock work), 8
   `GAME_CYCLES` (a charge on an edge, deliberately not probing) and 11
   probing sites, of which the cold trace installs 6.
4. **Loose ends:** **350** unknown nonzero bytes in `coverage.txt` (was 451;
   three table extents were derived and declared on 2026-08-25, and
   `$616B-$61FF` resolved into 140 bytes of table plus 9 bytes of unreached
   *code*), probe phase 3, and `robotron`/`bolo`, which have never been
   regenerated against any of this.


## Getting the machine out of the code

The four sections below are what step 6 actually turned out to be, in the order
they were done: **storage** out of emulated RAM, then the **status flags**, then
the **registers**, then the **lookup tables**. Each made the next legible, which
is why the order is worth keeping.

They are kept here at length because they are this game's record. **The
transferable copy is the playbook's "Getting the machine out of the code"** —
read that one first if the next binary is not Snake Byte; this is the evidence
behind it.

Three of the four had an approach that felt right and was wrong, and each
section says which:

| pass | the wrong instinct |
| --- | --- |
| storage | narrow the probe in the same step as the move it accommodates |
| flags | quote "in `rom_*`, registers are the algorithm" instead of checking it |
| registers | derive liveness by reading a routine's body |
| tables | size a table to the data you can see, or to what a run reaches |

### The registers, and the tables

Done 2026-08-24 and 08-25, same method as the flags: classify, do not inspect.

**Compute which registers a routine reads on entry; do not read it off.** A
fixpoint over read-before-write, following calls, is twelve lines of Python and
it is the only reason `rom_cout`'s parameter list came out right: it reads X
and Y as well as A, because it dispatches through `JMP ($36)` into the COUT
hook and the hook reads them. Reading `rom_cout`'s own body would have said
"A". The five entry points callers use are `rom_plot(row, col)`,
`rom_scrn(row, col)` returning the cell, `rom_setcol(ink)`,
`rom_hline(row, from_col)` and `rom_cout(ch)`; the bodies are untouched and
assign to `s_a`/`s_y` at the top.

**Poison, do not delete, to test whether a store is dead.** 42 residue writes
were suspected dead. Assigning each `0x5A` instead of its intended value and
running the gate is strictly stronger than deleting it: a deleted store leaves
the *previous* value in the register, which can coincidentally be the right
one, whereas a poisoned one cannot. Both batches passed all six cold checks.

**An `apple2tc --ir` live-out claim expires.** Three of those writes carried a
comment sourcing them to the IR's liveness analysis, and they were true --  of
the *generated* program, where the reader was generated code. That reader
became C and the claim quietly stopped holding. Re-derive; do not inherit.

**A value the code cannot state can still be measured.** Two delay loops count
down from whatever Y happens to hold, because the original never initialises
it. Asserting `s_y == 0` at both sites over full runs of both scenarios never
fired, so `spin()` takes it as an argument now and says where the number came
from. That is weaker than a proof and stronger than a guess, and the comment
says which.

**`branchTarget` was never read.** 121 of its 122 occurrences were the
generated dispatch's assembly-trace plumbing. The five that remain are inside
`GAME_CYCLES_COORD`, which declares one in its own block -- and that is why the
file's one bare `CYCLES` is spelled that way now.

**The `rom_*` bodies went the same way** (2026-08-25), once their entry points
had signatures: every routine computes in locals. `rom_bascalc(line)` returns
BASL's low byte, `rom_scrn(row, col)` returns the cell, `rom_wait(n)` takes its
count, `rom_coutz(ch)` carries the character through eleven branches as a
parameter instead of in A, and SETIO's `X`/`Y` turn out to be the built-in
device's vector -- `$FD1B` for KEYIN, `$FDF0` for COUT1 -- which is the first
time reading it says so.

**Adjacent `TICK`s are not merged, and that is deliberate** (2026-08-25). 47
runs of consecutive `TICK` statements could collapse into 47 single ones, 839
sites down to 790. It was tried, measured, and not kept. `TICK` is not an
accumulator: it tests `s_remaining_cycles <= 0` and may call `cycles_expired()`
before charging, so merging two removes a test and lets a frame boundary land
later. Measured over both scenarios: 4 and 3 frame video hashes of 1,300
changed, frame cycle counts moved by up to 56, and the run drifted 4 cycles in
22.1 million.

Nothing about the program changed -- the program-defined probes are byte
identical at all 16,332 samples -- but 49 fewer statements does not buy a
permanent difference in a real oracle. Anyone gating the cold build on frame
hashes later would inherit the drift and have no way to tell it from a bug.

`TICK` no longer exists as a name; the reasoning transfers to `advance`, which
is the same macro. It matters more now, not less: with 23 charges left, each
one is a yield some loop may be the only user of.

**The lookup tables are arrays** (2026-08-25). Thirteen of them, and the last
`ram_peek` of game data went with them -- the three that remain are the
entry-state loader filling `s_ram`. They had been left as image reads on the
grounds that they are "the image, not variables that happen to live in it",
which was an assertion rather than a reason: the real obstacle was that the
extents had not been derived, which is the playbook's own red flag.

**Deriving one of them was the whole point.** `$6174`'s shape masks hold data
to `$61DF` -- 27 shapes, the last `$1A`, the largest the code names. But the
tail is drawn with shape `ahead + $0C` where `ahead` is a lo-res cell, so 0-15,
which reaches shape `$1B` and index 111: past the data, into the zero padding
that runs to the code at `$6200`. An array sized to the data -- or to the 107
the two scenarios actually reach, which was measured -- would have been an
out-of-bounds read the first time a tail walked into a cell holding 15, on a
path no recording takes.

**The gate cannot check a table entry nothing reads**, so all 13 arrays were
compared against `snake-byte.b33` byte for byte instead. That check is worth
repeating after any edit to them.

**~~The 15 that remain are the floor.~~ They were not** (2026-08-25). The claim
was that COUT is reached through `JMP ($36)` and *cannot* take arguments, so the
hook must read its slot out of X. Checked:

- **Y was a closed loop.** COUT1 saved it and put it back; the hook did the
  same. Nothing ever read it for a value, so the promise had nobody to keep it
  to.
- **X was the slot, and the comment was wrong.** `cout_left_x()` said the value
  was "whatever COUT left behind ... an amount that depends on the ROM". COUT1
  never touches X and the hook saves and restores it without looking, so what
  COUT leaves is what its caller put there. The delay's first pass is `slot`
  long, and the callers already have `slot` as a parameter.

Calling something the floor is a claim like any other. This one was made from
the same kind of reading that produced the two wrong liveness claims above.

### The carry

`C` survived the flag audit as "genuinely crosses a call" and did not survive
being checked with a tool. No `rom_*` routine reads it on entry: every read is
preceded either by a local write or by a call whose callee writes it. All ten
game-side carry writes were dead -- inverting every one passes all six cold
checks -- and eight more inside `rom_*` were dead because the branch after them
tests the value rather than the flag. 36 writes are 12.

What the carry actually is, is one 6502 idiom on three edges: a routine ends on
a `CMP` and its caller reaches an `ADC` with no `CLC` in front of it, so the
compare's carry *is* the +1. `rom_clreolz` and `rom_vtabz` return it and
`rom_bascalc` hands back its second `ASL`'s. A global flag was hiding a real
data dependency behind something that looked like residue, which is exactly why
the wrong conclusion was easy to reach.

**Both wrong liveness claims this file has produced were produced by reading.**
The other was `rom_cout`'s parameter list. Use the read-before-write fixpoint
over the call graph; it is a dozen lines and it does not miss callees.

`D` stays entirely: 17 reads, every one choosing between a binary and a BCD
arm, plus the `assert_binary_mode` gates.

**The flags went the same day.** `C` was already local in every routine that
read it. `D` was not deleted because both scenarios take the binary arm 3,942
times out of 3,942 -- that is two scenarios, not a proof -- but because every
SED/CLD region in the file is *closed*: checked mechanically, all six contain
only `adc_dec16`, `sbc_dec16` and the `bcd_*` helpers, which take their
decimal-ness as an argument, and not one ROM call. The monitor is only ever
entered in binary. `game_bonus_screen` looked like the counter-example and is
not: its `$78C7` CLD comes before it prints.

**And the emulated stack.** Every PHA/PLA bracketed a call that could not reach
the C local it was protecting -- a 6502 pushes because JSR clobbers registers,
and a C call does not touch a caller's local. `rom_fc68` looked unbalanced and
was not: the scroll pops at the top of each pass and pushes at the bottom.

**Half of a2rom is converted by reading, not by running**, and this has not
changed: nothing scrolls and nothing emits Ctrl-G, so `rom_wait`, `rom_fc68`'s
scroll and `rom_coutz`'s bell and backspace arms are on probe-acceptance.sh's
unverified list. The gate is green on them because it never enters them.

### The game's state, and the flags

Done 2026-08-24, and first of the four. The interesting part is the method and
what it turned up, not the counts.

**Storage moves are done in the order that produces evidence.** Move the
storage with `ram-cold.probe` untouched and read the gate: it must say *trace
PASS, screen PASS, memory FAIL*. That failure is the proof the oracle was
watching those bytes. Only then narrow the probe, and then mutate each new
edge address and watch memory alone catch it. Every one of the 34 edges added
across the four slices was mutation-tested; all 34 were caught.

The `game` field is XOR-folded over its surviving pieces rather than given a
parameter each, because `PROBE_MAX_PARAMS` is 16 and there are more holes than
that. One wrong byte changes exactly one sub-hash, which is all this checks.

**Seven stores turned out to be dead the moment their bytes left RAM.**
`$6637`/`$6638` (where a bouncer step would have landed before the walls got a
say), `$6C4A` (how many axes blocked it), `$6B39`/`$6B3A` (the apple sweep's
cursor) and `$3754`/`$3757` (the relocation loop's patched LDA/STA operands).
Every one was written and read by nothing: the routines work in locals and
commit through the struct or the counter they were handed, so the stores
existed only because the memory oracle hashed the addresses. **Expect this at
every slice** -- and note that `-Wall` will *not* tell you, because these were
writes to `s_ram`, not to a variable.

**Initialisers are load-bearing.** Every moved variable is initialised to the
byte the shipped image holds at its address -- or, for zero page, from
`entry-state-inc.h`. A new game seeds itself from `s_apples_quota`, the status
panel is drawn before anything writes `s_lives` or `s_level`, and
`s_setup_seen` is already set in the image, which is why the setup screen's
first pass is skipped on a cold start.

**The flag audit is a classification, not an inspection.** Split every
`s_status_*` site into reads and writes, then ask, per flag, which reads a
write can reach:

| flag | writes | reads | verdict |
| --- | --- | --- | --- |
| `b` | 3 | 0 | never read at all -- the PHP bytes use the `STATUS_B` constant |
| `i` | 3 | 2 | both reads feed a PHP byte whose only reader is the PLP restoring `i` |
| `v` | 26 | 2 | the same, for bit 6 |
| `n` | 40 | 2 | both reads in `rom_fc68`, dominated by a local write |
| `not_z` | 43 | 3 | `rom_wait`'s two loops and `steer_try`'s return, all dominated |
| `c` | 36 | 26 | *this line was wrong -- see below* |
| `d` | 18 | 18 | **stays** -- every BCD region and every `assert_binary_mode` |

A value pushed only so that it can be popped back into itself is not a value.
That is what killed `i` and `v`: bits 2 and 6 leave the pushed byte and the two
restores go with them, while bit 3 (`d`) and bit 0 (the LSR's carry) stay
because those are genuinely read.

"In `rom_*`, registers are the algorithm" is true of `d` and was not true of
the other six. **The `c` row above was wrong**, and the way it was wrong is the
most useful thing on this page: the reads it cited are preceded by a *call*
whose callee writes the carry. Reading the routine's own body missed that.
Corrected 2026-08-25 -- see "The carry" below.

**Two of the localisations are not checked by running them**, and say so above
themselves: nothing scrolls and nothing emits Ctrl-G, so inverting `rom_fc68`'s
or `rom_wait`'s test passes all six cold checks. `$FC8C` and `$FC93` are
already on probe-acceptance.sh's unverified list. `steer_try`'s is checked --
inverting it fails the play screen.

**The ink threaded; the shape did not.** `s_ink` was read in exactly two
places, and every writer's value was readable at its call site, so the run
helpers take it as an argument and `draw_border(0x0d)` says where the border's
colour comes from. `s_shape` stays a global because `plot_at`'s two callers --
the bouncer erase and the tail erase -- genuinely erase with whatever shape was
last used. Both inferred inherited inks were mutation-tested; the redefinition
screen's is caught by the hires screen and by nothing else.

**`-Wall` does not flag a file-scope static that is written and never read.**
This is a different blind spot from the `-Wunused-but-set-variable` one already
recorded, and it is why threading and deleting must be separate steps: the gate
was run green with `s_ink` written but unread before any of its writes were
removed.

**~~Step 0 — strengthen the cold gate first.~~** Done 2026-08-23. It compared
trace and screen but not memory, and most of what follows moves values without
moving pixels. `ram-cold.probe` adds the memory comparison. It deliberately
drops the stack — the cold build is heading for no emulated stack at all, so
comparing it would pin the scaffolding and go red on the first adapter deleted
rather than on the first bug. It also skips `$4E/$4F`: RNDL/RNDH record how
long the ROM's KEYIN spun, so they differ between two boots of the same build,
and they are measurably the only two bytes below `$C000` that do.

**~~Step 1 — the free deletions.~~** Done 2026-08-23. `func_t001` and the 15
adapters that were nothing but `push16`/call/`pop16`. No cycle sites, no
marshalling; the site count stayed at 120 and all three checks stayed green.
`plot_shape_at` lost a `ret` parameter that existed only to feed one of them,
which is the cascade to expect from the rest.

**Step 2 — names.** All 115 addresses, `ram_peek(0x6253)` -> `life_outcome()`.
Pure rename: same cycles, same blocks, so the trace checks it at full strength.
Do it while the trace is strongest, sliced by subsystem, a commit each. Biggest
readability win at the lowest risk, and it is where the remaining "what *is*
`$725A`?" knowledge stops being re-derived.

**Done, 2026-08-23.** All 115 addresses are named, in six slices — the
scoreboard, the snake, the plotter's argument block, the monitor's zero page,
the settings block and pointers, and the tables. `ram_peek(0x...)` no longer
appears in `snake-byte.c`. The names are in `labels.txt` too, so `id`
shows them.

Three things to carry forward:

- **The oracle for a rename is a compile, not the gate.** A real rename must
  produce the same object. Build the file at `-O2` before and after (the
  one-line accessors inline away) and diff the disassembly; the only permitted
  difference is `__LINE__` immediates, displaced by whatever lines you added.
  Normalise the symbol name out first if you renamed a function. This was run
  on all six slices and caught nothing, which is what it is for. **It does not
  survive step 3** — moving storage changes the code by design, and that is
  where `ram-cold.probe` takes over.
  ```bash
  git show HEAD:decoded/snake-byte/snake-byte.c > /tmp/head-cold.c
  for f in /tmp/head-cold.c decoded/snake-byte/snake-byte.c; do
    cc -I include -I decoded/snake-byte -O2 -std=gnu11 -c $f -o /tmp/$(basename $f).o
    objdump -d --no-show-raw-insn /tmp/$(basename $f).o | sed 's/^ *[0-9a-f]*:\t//' | tail -n +3
  done  # diff the two outputs
  ```
- **Two blocks are hand-written unions. Do not tidy them into one name.**
  `$0000-$0008` is the plotter's arguments *and*, inside
  `game_cout_hook_native` only, a font pointer plus the caller's saved X/Y plus
  the character. `$002C` is HLINE's H2 *and* `game_print_bcd`'s digit flag.
  Both have two enums over the same addresses, and the substitutions went per
  routine.
- **Naming is what audits the names.** Four were wrong, all in converted C that
  passes every gate: `$725F/$7260` is apples on the field, not a countdown;
  `$0304` is the level's time allowance, not the apple value;
  `poll_and_discard()` was a speaker click; and `game_eat_apple` awards an
  extra life. The last came from `rom.externs`, so the generated C carried it
  too. See the two 2026-08-23 decision-log entries.

**Step 3 — real parameters.** `$0000-$0003` and `$0024/$0025` are the plotter's
argument block and the cursor, and the five most-touched addresses in the file
(63, 55, 26, 25, 23 uses). They are parameters passed through fixed memory
because the 6502 had no other way. This is the step that moves storage, so it
is the one that spends Step 0's memory oracle — retire it explicitly here.

**3a done, 2026-08-23: `$0000-$0008` left RAM.** It is `s_plot[9]`, a C object,
196 accesses moved, initialised from the entry snapshot so the first read before
any write still sees what the booting build sees.

- **3b done: the glyph blitter is out of the block.** Its five values — the
  glyph, the caller's X and Y, the source and the destination — are C locals in
  `game_cout_hook_native`, and `$0000` is `s_shape`, a named global rather than
  a slot. It is a global and not a parameter because the original makes it one:
  `bouncer_step` erases with whatever was last left there.
  The argument that the split is safe is written above the block, and rests on
  the hook being installed in exactly two places, both of which restore CSWL
  and both of whose exits reach `game_draw_playfield` — which writes `$15`
  unconditionally — before the play loop can run. **Do not weaken that comment
  without redoing the argument.**
- **The memory oracle is now narrowed and must not be widened back.**
  `ram-cold.probe`'s `zp_lo` starts at `$0009`. The retirement was done in the
  order that produces evidence: the move landed first with the probe untouched
  and the gate read *trace PASS, screen PASS, memory FAIL* — which is what
  proves the oracle was watching those bytes. Then the narrowing, then a
  mutation (`ram_poke(0x0040, 0x99)`) that **memory alone** catches, which is
  what proves the rest of the range still works.
- **What is no longer checked:** a value that is wrong but never drawn. Wrong
  shape/ink/column/row still moves pixels, and the screen compares 6,808
  samples. Probe phase 3 would restore the rest and is unbuilt.

**3c: the delay loop.** `$7061`'s two outer counters were `kCol`/`kRow`,
borrowed because zero page was the only place to put them. They are parameters.
Safe because nothing reads the residue — `wipe_occupancy_map` overwrites the
row on its first line and the column is not read until `draw_border` writes it.

**3e-3h, 2026-08-24.** The monitor's whole zero page (`$0020-$003E`) followed
the cursor out of RAM; `s_plot[9]` became named variables; the hi-res cell
drawers' four scratch bytes became locals, with the destination's high byte a
return value instead of a global the adapters read after the call; and every
plot and run call site states its arguments (`plot_at`, `plot_shape_at`,
`plot_hline_at`, `plot_vline_at`, `lores_vline_at`).

**`ram_peek(0x...)` and `ram_poke(0x...)` appear zero times in the file.** Every
address that stood in for a variable is a variable. The ~290 remaining
`ram_peek`/`ram_poke` calls all go through named constants for storage that is
genuinely the game's memory image.

Writing the inherited values out is where this stopped being cosmetic. Three
were load-bearing and none of them looked it:

- the bonus screen's box reads as four edges at column `$0D` and is not — an
  hline leaves `s_col` at its own endpoint, so the first vline ran down `$1A`,
  the box's right edge.
- `game_draw_side_walls` computed its seam from `s_run_end`, still holding the
  wall top from two calls earlier. That is a local named `wall_top` now.
- the redefinition screen's stem and third glyph both inherit column `$1E` from
  the arrow plotted above them.

### ~~What is left of step 3~~ — done 2026-08-24

`s_col`, `s_row` and `s_run_end` had already gone with the adapters; the
helpers take a `Cell` and an endpoint. What was actually left was `s_ink` and
`s_shape`, and the warning that stood here — *do not assume the other four are
like `s_shape`, and do not assume they are not* — was worth heeding, because
they were not alike.

`s_ink` threaded completely. It was read in exactly two lines, and the value
every writer set was readable at its call site, so the three run helpers take
it and `draw_border(0x0d)` names where the border's colour comes from.
`s_shape` did not thread and stays: `plot_at`'s two callers, the bouncer erase
and the tail erase, genuinely erase with whatever shape was last used, so
passing one would invent a value the original does not have.

The order that worked: readers first — there are far fewer than writers — then
ask which reader gets its value from a caller and which from whatever ran last.

**3d: the cursor.** `$0024/$0025` — CH and CV — are `s_ch` and `s_cv`, 43
accesses moved. That they belong to the ROM rather than to the game was raised
as a reason to defer and is not one: **"decompiler-generated" is not a category
that means anything here.** The ROM is being decompiled too, a routine still
emitted as a switch over block ids is a less finished one rather than a
different kind, and there is no version of this artifact in which the monitor
keeps its state in a 64K array. Everything gets decompiled — ROM, game,
whatever — and the plan should be read that way throughout.

`ram-cold.probe`'s zero page is three pieces now: `$0009-$0023`, `$0026-$004D`,
`$0050-$00FF`. Both new edges were mutation-tested. `$0023` is caught; so is
`$0040`. `$0026` is *not*, and that is not a hole — GBASL is recomputed by
GBASCALC before every use, so a stray write there is overwritten before the
next sample can see it. Do not "fix" that by widening the hash.

**Step 4 — return values instead of `s_a` and the flags.** Half done
2026-08-24: the seven adapters with no `CYCLES` site of their own are gone, and
their callers pass values (`game_print_bcd_native(byte)` rather than `s_a =
byte` then a call). `game_play_loop` became `game_play_one_life()`. Three
decimal-mode gates became one `assert_binary_mode()`.

The flag half is finished (see "The game's state, and the flags"), which
leaves 288 `s_a`/`s_x`/`s_y` references. Most are inside `rom_*`, where the
registers are the monitor's own argument-passing convention. The game side is
already clean: it goes through `lores_plot(row, col)`, `scrn_cell(c)`,
`plot_shape_at(...)`.

**Do not take "in `rom_*`, registers are the algorithm" on trust for these.**
It was the same claim made about the flags, and when it was checked it held for
`c` and `d` and was false for the other five: `b` was never read at all, and
`i` and `v` were only ever pushed so that they could be popped back into
themselves. Classify every `s_a`/`s_x`/`s_y` site as a read or a write and ask
which reads each write can reach, the same way.

**~~Step 5 — drop `ret_addr` and the emulated stack.~~** Done 2026-08-24. 36
signatures, and `push16`/`pop16` are unused in this build. Free, because
`ram-cold.probe` never hashed `$0100-$01FF`. The documented trap did not bite:
`game_print_inline_str` takes its address as an ordinary parameter and reads
the game's own image, not the stack. `push8`/`pop8` stay — 21 of them, PHA/PHP
inside a routine that the routine pops itself.

**Step 6 — `CYCLES`. Read this before starting: the step is not what this line
used to say.** It said "it is what the trace oracle is made of, so after this
the screen is the only check left". True, and incomplete — **`CYCLES` is also
the clock.** The macro adds to `s_cycles` and calls `cycles_expired()`, which
is what advances the host's frames. Delete the sites and time stops: `--frames`
never terminates and the delay loops cost nothing.

Measured 2026-08-24, the 851 sites are two populations:

| | count | what it is |
| --- | --- | --- |
| probing `CYCLES` / `GAME_CYCLES_*` | 130 (120 distinct) | trace oracle *and* clock |
| charge-only `CYCLES_EDGE` / `GAME_CYCLES` | 721 | clock alone |

- **6a — retire the trace.** Stop the 130 probing. This is the trade the plan
  described, but on its own it spends the strongest oracle and improves no
  code: the sites stay, they just stop being watched. **Do not do it to unblock
  the adapters — it does not.**
- **6b — remove the clock.** Needs the artifact to pace itself some other way,
  which nothing has designed, and it puts the cold build beyond comparison with
  the booting one, since the gate's whole method is running the same emulated
  program twice. Do this when the artifact stops being checked by comparison.

**The adapters are not blocked on either.** They hold `CYCLES` sites, and
deleting one drops a comparison point — but not silently:
`probe-acceptance.sh` pins the count at 120 and fails until the number is
changed on purpose. Delete them one at a time, lower the pin with a reason, and
the rest of the trace is untouched. Four are already charged `CYCLES(addr, 0)`
— `game_clear_hgr`, `game_draw_playfield`, `game_update_high_score`,
`game_find_apple` — so deleting those costs no cycles at all, only the site.
Check each one's A/flags write-back is unread first; that is the only real
work in it.

**d. Loose ends:** 451 unknown nonzero bytes in the coverage report; probe phase
3 (apple2tc emitting `PROBE_x(...)` sites so a probe can read the generated C's
own variables); and `robotron`/`bolo`, which have never been regenerated against
any of this.

1. ~~**The two remaining rejection roots.**~~ — **both done.** `$7230` became
   `--inline-str` (2026-08-17) and `$6A32` became `--alt-exit` (2026-08-18), the
   latter after one wrong attempt that passed every oracle and produced a
   self-recursive function; see the playbook entry on structural checks.

   **No game routine is rejected any more.** The report now reads 53 accepted /
   64 rejected, and every one of the 64 is Applesoft ROM — the dominant class
   being `stack level 2 at RTS`, the push-address-then-`RTS` computed jump.
   Nothing there is on the path to readable game C. Re-measure with
   `--routines-report` before treating any rejection as work.
2. ~~**Teach the disassembler the inline-string idiom.**~~ — **done**,
   `--inline-str=<file>` (`tools/apple2tc/Disas.cpp`). One declaration file per
   game; the routine must also be in `--extern-routines`, which is enforced.

2a. ~~**Every identified game routine is converted.**~~ — **done, and now
   complete.** `$6256`, `$6288`, `$6A32`, `$72CE`, `$78B3` and `$7980` went on
   2026-08-21; `$69A9`, the last emulator-shaped one, on 2026-08-22. `game.c`
   is adapters only, and the sole generated function left in the whole
   translation unit is `func_t001`, the top-level dispatch.

   The pinned site count is **1171**, of which **97 are in the game range
   ($3750-$854E) and 1074 are Applesoft ROM** — measured against
   `blocks-ext.txt`, so it is re-derivable:

   ```bash
   python3 -c "
   import io
   a=[int(l,16) for l in io.open('decoded/snake-byte/blocks-ext.txt') if l.strip()]
   g=[x for x in a if 0x3750<=x<=0x854e]
   print(len(a), len(g), len(a)-len(g))"
   ```

   **92% of what is left is ROM**, and no amount of further game work touches
   it. That is what makes retargeting the entry (item 4) the dominant item
   rather than a someday one — and it is why "convert another routine" is no
   longer available as the next step.

   Three things a new conversion should know, all of them learned here:

   - ~~**`game_print_inline_str` leaves orphans.**~~ **Fixed by
     `--prune-returns`** (2026-08-22). The orphans were never about the
     inline-string printer: `$7239` is where `$7230`'s own `JSR $FC68` returned,
     `$FC68` is external, and a ROM RTS still listed `$7239` among its returns.
     That one edge held the printer's body, its RTS, and the ten blocks its
     callers resume at. Pruning return edges no surviving call can produce
     removed 64 edges and 298 more blocks, and 74 `GAME_CYCLES_SHARED`
     spellings became plain `GAME_CYCLES` -- each one named by the lint, which
     checks the claim in both directions.
   - **`$741F` is a coordinate address** and lives inside `$7980`. It is
     spelled `GAME_CYCLES_COORD` there. Converting a coordinate site without
     that spelling stops the replay counter advancing and is caught only as a
     several-hundred-thousand-line trace diff.
   - **`$78B3` is entered with decimal mode set**, so its adapter omits the
     assertion every other adapter carries.

   Nineteen blocks are now converted without ever having been run: seven in
   `$6A32`, nine in `$7980` and three in `$69A9`, listed in the comments above
   `game_auto_steer()`, `game_setup_screen()` and
   `game_pause_or_toggle_sound_native()`. Those comments are the only record,
   because converting takes the addresses off the site list. The unverified
   count in probe-acceptance.sh is **20, and every one of them is ROM** — the
   number can no longer say anything about the game.

   **Read a routine's coverage per property, not as one figure.** `$69A9`
   measured it: inverting its Ctrl-S test fails `trace-ext`, but mis-charging
   its cycles by one passes every gate, because verify.sh compares cycles and
   never enters the routine while probe-acceptance.sh enters it and is
   deliberately cycle-blind. Control flow checked, timing not, one number for
   both.

3. ~~**`$8000-$84A4`**~~ — **answered 2026-08-07: it is data**, 29 vector
   display lists read as a byte stream by `$7019`. Nothing reaches it because
   nothing jumps into it. See the log entry for the command grammar, which
   parses the region exactly.
4. ~~**Phase 1b: retarget the entry to `$3750`.**~~ — **done 2026-08-23**, as
   the `snake-byte` target. See `decoded/snake-byte/snake-byte.c`.

   `snake-byte-body.c` is a hand-owned fork of the generated
   `snake-bytec1-ext.c`: the dispatch is rooted at `$3750` instead of the reset
   vector, and everything unreachable from there is deleted. **16,195 lines ->
   3,046.** `func_t001` went from 1,775 cases to 89 — every one of them game
   code — its address map from 136 entries to 1, and 42 of the 47 generated ROM
   helpers went with it. The five that stayed are the ones `a2rom.c` calls.

   What could not go: `s_mem_d000`, the ROM image. The death pause reads its
   delay lengths out of ROM *as data* at `$E000`, so the bytes are still needed
   even though none of the code is.

   The reachable set came from `apple2tc --ir`, which prints `Succ(...)` per
   block and every `Call` — walk it from `$3750` and 1,776 blocks reduce to 89.
   Doing the same walk on the emitted C instead is a trap: `pop16`, `tmp3_U16`
   and `sbc_dec16` all contain "16", so a naive scan for `block_id = N`
   manufactures edges to block 16 and reports 104 live cases instead of 89.

   Verified against the booting build, from `$3750` on: **block-head trace
   identical, screen identical at all 6,808 in-game samples.** The block count
   was 788,097 when this was written; it is 421,789 as of 2026-08-23, because
   converting the top level and deleting the pure-marshalling adapters took
   those addresses off the site list. A falling count here is the conversion
   working, not a regression — but re-measure rather than trusting either
   figure. Nothing about the entry state was guessed -- `--snapshot-at`
   captured it, and `make-entry-state.sh` regenerates it.

   **`--snapshot-at` now exists, as capture only** (2026-08-23). Every front
   end takes `--snapshot-at=<hex> --snapshot-out=<path>` and writes 64KB of RAM
   plus a `.regs` sidecar the first time the PC reaches that address — one shot,
   because the interesting address is usually re-entered (Snake Byte reaches
   `$3750` eight times a run) and what is wanted is the first arrival. It fails
   loudly if the address is never reached.

   That is the measuring half. **The restoring half does not exist**: nothing
   consumes a snapshot, and the generated engine still starts at the reset
   vector. That is what Phase 1b still needs.

   The soft switches are captured too, into the `.regs` sidecar, because they
   are the half of the machine a RAM image cannot carry — they are host IO
   state, not memory. At the prompt they are the power-on values: text, page 1,
   not mixed, `vid_control = $01`. A rebased entry has to set them explicitly.

   What the capture already settled is the size of the problem, which was being
   guessed at. See `decoded/rom/boot-state.txt`: the machine's entire
   boot-created RAM state is **`$0000-$0802`** — zero page, the stack, the input
   buffer, and the text screen. Everything from `$0803` to `$BFFF` is `$FF`,
   i.e. uninitialised and not boot's doing at all. It is two kilobytes, recorded
   once, and game-independent.

   **The re-based trace does not need a human — measured 2026-08-23.** This
   was the obvious way for Phase 1b to collide with the no-new-recordings
   agreement, so it was checked before any design work. It does not.

   Both halves are mechanical:

   - The `.frames` files always were. `verify.sh --record` re-runs the
     reference build twice and refuses to write unless the two agree.
   - The **key files re-stamp by a constant**. Instrument the coordinate
     (`rec.probe`'s seven sites) with a `printf` alongside its `key n`, add a
     probe at `$3750`, and replay each scenario: the first `$3750` entry lands
     at **n=181,207** for `play` and `play-hires` and **n=217,543** for
     `play-rebind`, and no coordinate site fires between the end of boot and
     that entry. `$FB7C` is the one ROM site that also fires *after* the game
     starts — 17 times, in the two hi-res scenarios — and it fires from
     `a2rom.c`, which a retargeted build still has, so the post-entry sequence
     is unchanged.

   So re-stamping is: **drop the keys stamped below the entry constant, and
   subtract it from the rest.** The dropped keys are the same 11 in all three
   scenarios and they are literally `CALL 14160` + CR — 14160 is `$3750`, so
   they exist only to reach the address the retargeted build starts at. What
   is left is 12 / 8 / 34 keys.

   Two things this does *not* say. The snapshot format is still the real work
   and still unbuilt. And `$3750` is entered **8 times** in a single play run
   — the game restarts through it — so a cold start there is something the
   game already does to itself; what has to be captured is the state at the
   *first* entry, which is the one arriving from BASIC.

   The probe used is five lines; reproduce it rather than trusting these
   numbers if anything upstream has moved.
5. **Frame 623 is resolved; a narrower divergence remains, not yet
   root-caused.** Frame 623 was the old cycle-stamped-input artefact: `play.keys`
   was recorded from the generated build, so its stamps were that engine's own
   cycle coordinate, replayed against the interpreter's different one (see the
   2026-08-14 "Probe-stamped key recording" decision-log entry). Probe phase 2
   closes that gap — `record <expr>` (the recording counterpart of `key
   <expr>`) and `--record-keys=` let a script stamp keys on a coordinate the
   *program* defines, not the host's cycle counter (`docs/probes.md`).
   `decoded/snake-byte/play.keys` was converted to `play.pkeys` on that
   coordinate, and `decoded/snake-byte/probe-acceptance.sh` is the resulting
   cross-engine gate: replaying `play.pkeys` against the interpreter and both
   generated builds produces a byte-identical 2,744,938-hit block-head trace
   over all 1,300 frames, and frame 623 itself now matches.

   **The 8 residual frames are now accounted for too** (2026-08-15 entry).
   8 of 1,300 frame hashes still differ, but nothing in the machine does:
   sampling state at program-defined instants instead of at the host's frame
   timer shows all memory outside the stack page identical at 6,808 in-game
   samples, SP identical, and — once `--ret-addr` was turned on — the live
   stack identical too. The frame hashes differ only because the host samples
   video on a fixed cycle interval while the two engines sit up to one basic
   block apart in *phase* (not rate: 22,147,953 vs 22,147,957 cycles over the
   whole run). It is a property of the measuring apparatus.

   Both scenarios are converted (`play.pkeys`, `play-hires.pkeys`) and both are
   gated. What is genuinely left is `robotron` and `bolo`: the machinery is
   generic, but neither has a coordinate script, and `bolo`'s `--irc1` line is
   commented out in its `decompile.sh`.

   Probe phase 3 is still unbuilt: apple2tc does not yet emit `PROBE_x(...)`
   placeholder sites, so a probe can observe machine state but not the
   generated C's own variables.

**No longer deferred:** the headless-vs-windowed trace comparison was deferred
for want of a display. `a2run` removes the need for one, and the comparison is
now a test that runs on every invocation of `tests/run-tests.sh`.

**Still deferred:** regenerating `decoded/robotron/**` and `decoded/bolo/**`,
which would pick up both the recovery fix and `--extern-routines`.

---

## Traps

Mistakes already made here. The log has the full accounts.

| Trap | Reality |
| --- | --- |
| "A changed frame trace means my code ran" | It only proves *something* changed. Pressing `C` at nine different cycles changed the trace every time; a call counter showed the target ran **zero** times. Instrument and count. |
| "Absent from `BranchTargets`" = unreachable | The list is capped at 500 entries and records only *targets*, so fall-through blocks read as absent either way. Different claims; only "unreachable" is worth relying on, and it needs an argument. |
| `--asm` `Code range` markers as a coverage measure | They appear only at discontinuities — 260 bytes against 3,931 bytes of real instructions. Count instructions. |
| Externalizing entry points will shrink the output a lot | It removed 112 blocks, not ~1,430. Library code is reachable both via its entry points *and* via the start PC, non-additively (112 and 48 alone, 1,530 together). |
| A self-modification warning in game code | Check for the inline-string-after-`JSR` idiom first. Snake Byte's only steady-state "self-modifying" block was the string `"VALUE: "`. |
| Replacing a routine that dispatches through a vector | `COUT` is `JMP ($36)` and the game repoints it. `rom_cout` honours the vector and aborts loudly on an unknown target — the trace cannot catch a wrong guess there. |
| clangd/IDE diagnostics in this repo | It lacks the include paths and reports cascading phantom errors (`CircularList.h file not found`, `s_a` undeclared in `a2rom.c`). Trust `ninja` and the test suite. |
| A bodyless `ir::Function` | `getAddress()` derives from `getEntryBlock()`, which asserts on an empty block list — silent UB under `NDEBUG`. Use `Function::isExternal()`. |
| Adding a `--code-at` target is enough to reach it | It also needs the `ORIGIN` half. Only a block that is a successor of a *dynamic* branch gets an address-to-block map entry; without one the code is decompiled but unreachable, and you still get `Unknown address`. |
| `--code-at` will tell you if you point it at data | It cannot. That untraced bytes are code is exactly what you are asserting. It only checks the target starts a block and the origin is a branch, and warns if the target decodes to an invalid instruction. The real check is that the surrounding data boundaries come out right. |
| The compiler will DCE unreachable blocks | It will not. `dce()` removes only instructions, and every `Void`-typed instruction counts as having side effects. A pass that orphans blocks must delete them itself. |
| Two frame-hash traces that disagree mean one engine is wrong | Not between *different* engines. Frame hashing presumes a shared sampling clock; the generated engine yields only at block boundaries and an interpreter between instructions, so two perfectly equivalent implementations still disagree in 1-3 frame bursts around transitions. Same-engine comparison is fine, and is what `verify.sh` does. |
| Comparing register traces across engines | `CPURegLiveness` and `dce` drop stores to dead registers by design, so the generated code does not maintain `Y` or the flags where nothing reads them. Traces diverge on line 2. `--compat` makes the format diffable, not the content. |
| A rejection test that greps for `FATAL` | Twice during the probe work a test passed while covering nothing, because a *different* check fired first and satisfied the grep. Assert the specific message, and prove the test can fail by deleting the check it covers and watching the suite go red. |
| A probe that produces no output | Says nothing about agreement. A probe on a non-block-head address fires in the interpreter and does not exist in the generated program; the report then reads as agreement while covering less than you think. |
| `snake-byte.lst` shows you every keyboard read | It cannot. `$7541` and `$664A` are reached only through `--code-at` control flow, so the disassembler never traced them and the listing prints them as `DFB` data. `$760F`, the read that takes 6 of the 19 keys in the hires scenario, is visible *only* in the generated C. Grep the C, not the listing, for anything on a `--code-at` path. |
| `--ret-addr` is a cosmetic verification flag | It is on for a correctness reason. Without it a `JSR` pushes the sentinel `$FFFE`, and the inline-data-after-`JSR` idiom — which Snake Byte uses — finds its data by *reading* that address. It costs nothing (same `push16`, different compile-time constant). But it is **not** for a shipped artifact: that wants `ret_addr == 0`, no emulated stack at all. |
| Every `CYCLES`-shaped call site is a program location | It is not. The taken-branch penalty is charged on the *edge*, in a block carrying the branch's address that the program is never actually *at* — so it must not trace or dispatch probes, or one execution of that branch gets reported twice. Hence `AddEdgeCycles`/`CYCLES_EDGE`. 698 ROM addresses are edges and 121 of them are also real block heads, so the two are not distinguishable by address. |
| A plan's list of keyboard-read sites is complete because it was grepped once | Snake Byte's coordinate plan named three sites (`$FD1B`, `$741F`, `$7890`); a fourth, `$6217` — the in-game ingest that clears the strobe and fills the ring buffer at `$623C` — was missing. Recording with only the three captured 11 of 23 keys, not 11 cycle-quantised ones: once a script delivers via `key` at all, the host's per-frame drain stands down entirely (`probe_uses_key()`), so an uncovered site's keys are never delivered, not merely mis-timed. Found by recording and counting, not by reading the disassembly harder. |
| A green gate proves the code you just wrote | Only if the binary was rebuilt. Twice on 2026-08-24 `probe-acceptance.sh` reported six green cold checks against a stale executable — once because ninja saw no work for a file it had just been handed (same-second mtime), once because the build had failed and the old binary was still there. The script now refuses to run if any program is older than its sources; before that check existed, the only defence was reading the `Linking` line above the result. |
| `-Wall` will tell you when a store goes dead | Not for a file-scope static. `s_ink` was written at fourteen sites and read at none for a whole build, and `-Wall`, `-Wextra` and the gate were all green. It only warns about *locals* (`-Wunused-but-set-variable`, itself a separate warning from `-Wunused-variable`). And it says nothing at all about a dead `ram_poke`, which is what seven of the moved stores turned out to be. Classify the sites; do not wait to be told. |
| "In `rom_*`, registers are the algorithm", so leave them | True of `c` and `d`. False of `b`, `i`, `v`, `n` and `not_z`, which is 115 of the 222 flag references. The phrase was being quoted rather than checked. |
| A green gate proves a routine you just rewrote | Only if a scenario runs it. Inverting `rom_fc68`'s scroll test or `rom_wait`'s countdown test passes all six cold checks, because nothing scrolls and nothing emits Ctrl-G. `probe-acceptance.sh`'s unverified list already names those addresses; read it before believing a pass. |
| A test that fails proves the check it names | Task 3's own drain-guard regression test specified `--frames=10`. At that frame count the buggy (unguarded) and the fixed (guarded) build produce byte-identical output — the installed keyboard site isn't even reached until roughly frame 8.3, and the one key that would distinguish the two builds is stamped for a point roughly 59 frames further out — so the test failed identically before and after the fix and proved nothing either way. Needed `--frames=100`. A red result is only evidence once you have also seen it turn green for the right reason. |
| `cold_compare`'s ext/cold `--frames` pair scales proportionally | It does not. The gap between them is the fixed number of frames the booting build spends in ROM boot before its first `$3750` — roughly 150 frames, independent of the total budget. The shipped 1300/1150 pair happens to have a ~150-frame gap; raising both by the same *ratio* (or the same round number) does not preserve it. Tried 40000/39000 (a 1000-frame gap): cold fell ~800-900 frames short of the booting build's trace length and the gate failed on "cold produced N blocks, fewer than needed" — not a content mismatch. Confirmed by re-running cold alone at `--frames=39900`: byte-identical to the booting build's aligned trace. Compute the cold-side number from the fixed offset, not from the ext-side number scaled down. |

---

## Open questions

- **The `$93`/`$83` Ctrl-S handshake** in `rom_coutz`: reachable at runtime
  (needs a key pending when a CR is output) but not on any trace. Transcribed
  faithfully, unvalidated by execution — and now measured rather than assumed:
  probed at `$FB85` it fires 0 times in both cold scenarios, as do the bell and
  the backspace beside it. The routine says so above itself.
- **`$66A6`**: `JMP $6655`, an alternate entry to the glyph blitter that skips
  the control-character filter. Nothing references it. Left unimplemented.
