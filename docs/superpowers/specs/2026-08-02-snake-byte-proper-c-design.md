# Converting decoded/snake-byte into proper C

Date: 2026-08-02

## Goal

Turn the Snake Byte decompiler output into hand-maintainable C: real functions,
structured control flow, named variables and structs. The result should be
source you would want to read, modify, or port — not an emulator wearing a C
costume.

## Starting point

`decoded/snake-byte/` contains:

| File | Role |
| --- | --- |
| `snake-byte.b33` | 20KB original binary, loads at `$3750–$854E`, plus a segment at `$00B1–$00C8` |
| `snake-byte.json` | Runtime trace: 500 branch targets, 565 branches, 2 generations |
| `snake-byte.c` | `--simple-c` output — one `run_emulated()` with `switch (s_pc)` |
| `snake-bytec1.c` | `-O3 --irc1` output, 18,285 lines — the good one |
| `snake-byte.lst` | Annotated disassembly, 29,311 lines |
| `play.keys` | Cycle-stamped key recording (23 events) |
| `play.kbd` | Literal text typed at the BASIC prompt: `call 14160` |

`decompile.sh` currently builds only the c1 variant.

### Shape of `snake-bytec1.c`

It compiles and runs, but remains emulator-shaped:

- **53 routines** were promoted to real C functions; everything else lives in
  `func_t001`, a `for(;;) switch (block_id)` state machine over **2,097 blocks**
  (lines 2727–16255).
- Of those 2,097 blocks, **641 are Snake Byte** and **1,442 are Apple II ROM**
  (Applesoft + Monitor at `$D000–$FFFF`), with 14 in zero page. The ROM is
  present because the entry point is the reset vector `$FA62`: the program boots
  ROM, lands in BASIC, and `play.kbd` types `CALL 14160`.
- Calls pass a fake return address (`func_6127(0xfffe)`) pushed onto an emulated
  stack.
- All state is global: `s_a`, `s_x`, `s_y`, `s_sp`, seven separate flag bytes,
  and `s_ram[0x10000]`. Zero-page variables appear only as
  `ram_peek(0x0006)` / `ram_poke(0x0006, …)`.
- `CYCLES(pc, n)` is threaded through every block. The whole thing runs on its
  own thread, handing control back to the host via condvar whenever the cycle
  budget expires (`include/apple2tc/system2-inc.h:242`).

### Findings that shape the design

**The hardware surface is tiny.** Across `$3750–$854E` the game touches only
`$C000`/`$C061` (keyboard, joystick button), `$C010` (strobe), and
`$C050/$C052/$C055/$C057` set once (graphics, full screen, page 2, hi-res),
plus `$C053` (mixed) at `$76F0`. `$C030` (speaker) appears **only** in ROM, so
all sound goes through the ROM `BELL` routine.

**Lo-res is a shadow model, not a display.** The game renders itself in both
lo-res and hi-res. Hi-res is what the player sees; the lo-res page is never
displayed and serves as the game's logical board — `SCRN` reads it back for
collision detection. Since nothing displays it and we reimplement all four
primitives that touch it, its representation is ours to choose in Phase 2 —
*provided* nothing else pokes `$400–$7FF` directly, which must be verified
before that is assumed.

**There is essentially no self-modifying code.** Only three game blocks carry
self-modification warnings. `$3752`/`$375B` are the one-shot startup relocator
(a block copy). `$736C` is a **false positive**: `$7230` is the classic inline-
string printer — it pulls its return address into ZP `$0C/$0D`, walks the
null-terminated string following the `JSR`, `COUT`s each byte, then pushes the
adjusted address back and `RTS`es past it. The "code" at `$736C` is the string
`"VALUE: "`. This makes promoting memory to real C variables far safer here
than it would normally be.

**The game blocks on input.** `$69AD: LDA $C000 / BPL $69AD` and `$7890` are
spin loops. They only terminate because the emulated CPU yields to the host when
its cycle budget expires (`cycles_expired()`). The game therefore cannot simply
become an `update()` called 60x/sec — the control flow would have to invert.

**The ROM boundary is 9 entry points.** Called from game code:

| Entry | Routine | Call sites |
| --- | --- | --- |
| `$F800` | `PLOT` — lo-res plot | 13 |
| `$F819` | `HLINE` — lo-res horizontal line | 4 |
| `$F864` | `SETCOL` — set lo-res color | 10 |
| `$F871` | `SCRN` — read lo-res pixel | 15 |
| `$FC58` | `HOME` — clear text screen | 2 |
| `$FC68` | monitor, used by the `$7230` string printer | 2 |
| `$FDED` | `COUT` — character out | 5 |
| `$FE89` | `SETKBD` | 1 |
| `$FE93` | `SETVID` | 1 |

All nine appear in the c1 output, which contains only blocks the tracer
reached, so all are live. Roughly 150 lines of C replaces all 1,442 ROM blocks.

## Decisions

| Question | Decision |
| --- | --- |
| Method | Hand-written C, checked in. `snake-bytec1.c` retained as the oracle. |
| Primary source | `snake-bytec1.c` — it is a superset of the listing (flag liveness, SSA temporaries, resolved block IDs, cycle totals). `snake-byte.lst` is secondary reference. |
| Boundary | Keep `s_ram` and `a2io`. Drop the ROM behind 9 helpers. |
| Equivalence | Per-frame framebuffer hash, replayed from `play.keys`, diffed against a golden trace recorded from the c1 build. |
| Timing | Keep the emulator thread and its yield. Carry exact `CYCLES()` totals forward so the frame-hash oracle stays exact. |
| Naming | Structural conversion globally first, then reverse engineering in vertical per-subsystem slices. |
| Tooling | Improve `apple2tc` itself rather than write a throwaway script. |

The frame-hash oracle works because `lib/decapplib/decapplib.c:380` already
locks the run to exactly `1/60 * clock_freq` cycles per frame whenever
`--key-file` is active. It is insensitive to cycle counts only in the sense that
it does not compare them directly — but the game's visible behavior *does*
depend on timing, through the spin loops and delay loops. Hence the decision to
carry exact cycle totals: approximate timing would make the game drift and the
oracle would stop working.

## Why relooping needs procedure recovery first

Structuring algorithms operate on a single procedure's CFG. `func_t001` is not
one — it is 2,097 blocks from unrelated routines fused by a dispatch loop. Worse,
its edges are wrong in a way that cannot be fixed locally: inside the mega-switch
`RTS` compiles to a *computed* edge,
`block_id = find_block_id_func_t001(pop16() + 1)`, and there are **165 of them**.
Statically each is an edge from that `RTS` to every possible return site, so the
CFG is close to complete. Relooping it would produce a correct but useless
program — the same dispatch loop wearing `while`/`if`. The `block_id` variable
*is* the program counter, and a program whose PC is data cannot be structured.

So: recover procedures first, then reloop each independently.

## Phase 0 — Improve `routines.cpp`

`IdentifySimpleRoutines` in `tools/apple2tc/routines.cpp` already performs
procedure recovery. Running `apple2tc … --irc1 -v2` reports why each candidate
was rejected. Across all candidates (raw counts, summed over the two passes of
the `while (identifySimpleRoutines(mod)) {}` loop in `apple2tc.cpp` — the first
identifies 53 routines, the second finds none and terminates):

| Rejection reason | Count |
| --- | --- |
| block stack level **not zero** | 134 |
| `Pop8` stack level **underflow** | 24 |
| terminator `JmpInd` | 18 |

The `removeInvalidJSRs()` fixpoint at `routines.cpp:86-92` — which deletes any
candidate that JSRs into a rejected one — **does not fire** on this binary; the
count converges immediately at 53. All losses happen in `scanCandidate`.

Restricted to `$3750–$854E`, 18 game routines are promoted in the c1 output and
**10 distinct candidates are rejected**:

| Routine | Call sites | Rejection |
| --- | --- | --- |
| `$71F3` | 15 | stack level not zero |
| `$7230` | 10 | `Pop8` underflow |
| `$7226` | 6 | terminator `JmpInd` |
| `$6256` | — | stack level not zero |
| `$6288` | — | stack level not zero |
| `$6B3D` | — | stack level not zero |
| `$6C72` | — | stack level not zero |
| `$7000` | — | stack level not zero |
| `$72CE` | — | stack level not zero |
| `$6A32` | — | `Pop8` underflow |

The dominant rejection is weaker than it needs to be: `int stackLevel = 0;` is
declared **inside** the per-block loop (`routines.cpp:149`), so pushes and pops
must balance *within a single basic block*. The universal 6502 idiom — `PHA` at
entry, `PLA` in a different block before `RTS` — is well-behaved and rejected
anyway.

Work items:

1. **Track stack level across the candidate's block set** rather than per block.
   Expected to recover the seven "stack level not zero" routines, including
   `$71F3`, the most-called routine in the game.
2. **Add a report mode** (`--routines-report`) emitting, per candidate:
   accepted/rejected with reason, entry address, block set, call sites, and for
   accepted candidates the dominator tree and natural loops. This is the input
   to hand-writing C in Phase 1, and is reusable per binary.
3. **Leave `$7230`, `$6A32` (return-address manipulation) and `$7226`
   (`JmpInd`) rejected**, but have the report name them precisely so they are a
   known manual list rather than a surprise.

Re-running then yields a c1 output with roughly 25 of 28 game routines promoted
instead of 18, shrinking `func_t001` correspondingly.

**Risk:** the 7/10 estimate assumes "stack level not zero" means cross-block
`PHA`/`PLA`. That is consistent with the diagnostic but unverified; some of the
seven may be doing something genuinely irregular. Confirming this is the first
step of Phase 0, and the number may come down.

## Phase 1 — Structural conversion, global

Mechanical in intent, with a bounded judgment tail. Address-derived names
throughout (`func_60e4`, `var_0003`, `tbl_6174`). Exact `CYCLES()` totals
carried forward.

1. **Replace the ROM.** Implement the 9 entry points in `a2rom.c`. Remove the
   `$D000–$FFFF` blocks.
2. **Change the entry point** from the reset vector `$FA62` to the game's
   `$3750`, collapsing the startup relocator to its post-copy state.
3. **Recover the remaining routines by hand** — the three Phase 0 leaves behind,
   plus any that surface. `$7230` is not recovered but *rewritten*: it becomes
   `print_str(const char *)`, with the inline string at each of its 10 call sites
   lifted out of the code stream into a real string literal.
4. **Handle tail calls.** The listing shows 30 distinct intra-game `JMP` targets
   across 50 sites. Those landing on another routine's entry are tail calls and
   become `f(); return;`; the rest are ordinary intra-procedural jumps.
5. **Reloop each function** from the Phase 0 report: dominator tree, back edges
   (`n→h` where `h` dominates `n`) for natural loops, then iterative region
   matching against sequence / if-then / if-then-else / while / do-while,
   collapsing each match until one node remains. Irreducible leftovers get node
   duplication, or a function-local dispatch as a last resort. At 10–40 blocks
   per function, few or none are expected — 1982 hand-written 6502 is mostly
   structured.
6. **Delete dead flag writes** (`s_status_n = …` where nothing reads it). Pure
   dataflow; c1 already carries the liveness information.

**Risk:** starting cold at `$3750` means reproducing whatever machine state
Applesoft left behind when `CALL 14160` transferred control — principally zero
page, and the stack pointer (`decapplib.c` sets `SP = 0xF0`, "SP is 0xF0 in
BASIC"). State the game establishes for itself is not a concern: `$376E` writes
`$52/$01/$01/$01/$64` to `$0300–$0304` on its own, and `$3750` relocates
`$3800–$3FFF` to `$1800–$1FFF` before jumping to `$76C2`. If the inherited state
is not reproduced exactly, frame hashes will not match from frame one. This must
be pinned down before Phase 1 can be verified.

## Phase 2 — Vertical reverse-engineering slices

One subsystem at a time, each taken to finished named C, verified against the
oracle, and committed independently. The rest of the game keeps running out of
the remaining structural code while this proceeds. Per slice: promote zero page
to locals or parameters (needs interprocedural liveness), extract structs from
data tables, apply meaningful names.

Candidate first slice: the hi-res drawing at `$60E4–$6174` — self-contained,
with its tables already visible (`$6000`/`$6030` address bytes, `$6060` masks,
`$6064` patterns). Strong second: the lo-res shadow grid, where owning all four
primitives means the representation can change.

Vertical rather than horizontal because the oracle reports *that* something
broke, not *where*. One subsystem per slice keeps every failure attributable.

## Deliverables

| Path | Contents |
| --- | --- |
| `tools/apple2tc/routines.cpp` | Stack-tracking fix, report mode |
| `tests/` | One `.s` + `.ir` baseline covering cross-block `PHA`/`PLA` |
| `decoded/snake-byte/snake-byte-src.c` | The hand-written game |
| `decoded/snake-byte/a2rom.c`, `a2rom.h` | The 9 ROM replacements |
| `decoded/snake-byte/snake-bytec1.c` | Unchanged — the oracle build |
| `decoded/snake-byte/play.frames` | Golden per-frame hash trace |
| `decoded/snake-byte/verify.sh` | Replays `play.keys` against both builds, diffs |
| `lib/decapplib/decapplib.c` | Per-frame framebuffer hashing behind a flag |

Both binaries stay in `decoded/snake-byte/CMakeLists.txt` throughout, so the
oracle is always buildable.

The frame trace and `verify.sh` live beside `play.keys` and `decompile.sh`, not
in `tests/` — `tests/` is decompiler regression (hand-written `.s` assembled by
`a6502`, decompiled, diffed against `.ir` baselines), and a game's golden trace
is project data, not a test of the decompiler. The one genuine `tests/` addition
is the Phase 0 guard, since changing which routines get promoted changes `.ir`
output for robotron and bolo too.

## Open questions

1. Does anything besides `PLOT`/`HLINE`/`SETCOL`/`SCRN` access `$400–$7FF`? If
   so, the lo-res grid representation is not free to change in Phase 2.
2. What exact machine state does the game rely on at `$3750` entry?
3. Do any computed-`RTS` jump tables (push address, then `RTS`) exist in the
   game range? `$7226`'s `JmpInd` rejection suggests at least one indirect
   dispatch that will need a `switch`.
