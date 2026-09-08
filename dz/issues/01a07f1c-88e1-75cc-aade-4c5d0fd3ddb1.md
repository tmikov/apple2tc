---
id: 01a07f1c-88e1-75cc-aade-4c5d0fd3ddb1
title: Calls inside all-ones blocks make N and Z look live across returns
type: bug
status: open
resolution: null
component: null
assignee: null
created: 2026-09-08T03:42:51.617Z
creator: Tzvetan Mikov <tmikov@gmail.com>
---

`initBB()` gives any block whose terminator is an `IndirectBranch` a
`liveOut` of `kAll`. `RTS` is inside that range, so every RTS-terminated
block in a function still at `DecompileLevel::Low` gets it -- and after
routine identification each corpus has exactly one such function, the
un-extracted entry point, which is where most of the code that was never
turned into a routine ends up.

Robotron: 452 of 803 call sites are inside that one function, which has 211
RTS terminators. `deriveInstLiveOut()` walks back from `kAll` for a call in
such a block, so the call reports that everything its callee modifies is
still needed. `--dump-reg-liveness` marks the callees 'K' and counts the
call sites as `call-sites-blind`.

Measured with `--dump-reg-liveness` at -O3 (blind call sites / all call
sites, then functions with at least one blind caller / all functions):

    rom        45/248   37/75
    robotron   78/803   35/135
    snake-byte 30/361   28/82
    bolo       66/509   49/135

The distortion is not spread evenly -- it lands almost entirely on N and Z.
Fraction of functions reporting the flag live-out, split by whether the
function has a blind caller:

               N: blind / clean      Z: blind / clean
    rom          37/37  /  9/38        37/37  /  10/38
    robotron     20/35  /  4/100       19/35  /   4/100
    snake-byte   27/28  /  7/54        27/28  /   7/54
    bolo         36/49  / 12/86        36/49  /  14/86

So N and Z essentially never cross a call boundary that anything actually
measured, and almost always appear to when nothing did. C behaves the
opposite way: it is live-out in 38-57% of the clean rows, which is a real
result and not an artifact. Any decision about which registers become real
parameters or returns has to read the clean rows only, or it will conclude
that N and Z must be threaded through every signature.

Two directions, neither obviously right:

  - Narrow the forcing. An `RTS` whose return addresses are all known is not
    an unknown target; `--prune-returns` already reasons about exactly that
    edge set. Only a genuinely unresolved `JmpInd`/`JSRInd` needs `kAll`.
  - Extract more routines, so fewer call sites live in a Low-level function
    at all. That is the same work as improving `identifySimpleRoutines()`
    and helps for other reasons too.

Reproduce: `apple2tc <corpus> -O3 --dump-reg-liveness | grep '^SUM'`, and
`awk '$1=="FUNC" && $7 !~ /[KU]/'` for the clean population.

## Log

- 2026-09-08T03:42:51.617Z  Tzvetan Mikov <tmikov@gmail.com>  created
- 2026-09-08T04:19:23.630Z  Tzvetan Mikov <tmikov@gmail.com>  comment
    Fixed. `initBB()`/`resetBB()` now force `kAll` only when an indirect transfer
    has no recorded targets. `GenIR::addBranchTargets()` pushes the addresses the
    recording actually saw on as operands, which makes them ordinary CFG
    successors, so `updateBlockLiveInOut()` unions their `liveIn` like any other
    edge -- the `kAll` was discarding that union.

    Applied to `RTS` and `JmpInd`. `JSRInd` is left conservative: its operand 1 is
    the *return* continuation rather than a recorded target, so it always has a
    BasicBlock operand and a "has successors" test would silently always pass.
    There are three of them in Bolo and none in the other three corpora.

    Trusting the list adds no new failure mode. `IRC1::scanForDynamicBlocks()`
    already reads the same operands from the same indices to build the
    address-to-block map the generated C dispatches through, and an address
    missing from it reaches `error_handler()`. An incomplete list therefore
    already breaks the generated program loudly, at the transfer itself.

    Blind call sites, before -> after:

        rom        45 -> 9      K-rows 37 -> 7
        robotron   78 -> 44     K-rows 35 -> 24
        snake-byte 30 -> 19     K-rows 28 -> 19
        bolo       66 -> 66     K-rows 49 -> 49

    Bolo does not move because `bolo.json` predates the `Branches` field
    entirely -- it has `BranchTargets` but no origin-to-targets map, so no RTS in
    it carries a target and every one still needs `kAll`. Re-recording it would
    close that; nothing else will.

    The prediction in this issue -- that N and Z live-out would collapse -- did
    NOT hold as stated, and the reason is worth writing down. Liveness feeds
    `dce()`, so the analysis after the fix runs on a *different program*: one with
    the dead stores removed. A deleted `StoreR8` is also a deleted *kill*, so a
    genuine downstream read now reaches further back and the surviving live ranges
    are longer. Robotron's Z live-out went from 3% to 32% of the clean population
    on 14 Z reads in the whole module, entirely that effect.

    Measured directly instead, on stores deleted (loads are unchanged everywhere,
    which is the semantic check):

                   N stores        Z stores        C stores        V stores
        rom        306 -> 90       319 -> 138      453 -> 325      178 -> 158
        robotron   249 -> 135      242 -> 180      601 -> 577      251 -> 241

    That is the claim, in the form that survives: N and Z are recomputed and
    thrown away far more than C is. 71% of the ROM's N stores and 57% of its Z
    stores were dead, against 28% of its C stores.

    Verified behaviourally, not just by baseline. decoded/rom/probe-acceptance.sh
    and decoded/snake-byte/scripts/probe-acceptance.sh both pass against the
    regenerated C: 276,255 probe hits over the ROM boot, 7.8M over Snake Byte's
    longest scenario, 41.6M block heads for the cold-start build, memory and
    screen identical at every sample, speaker accesses identical. verify.sh passes
    1,300 frames on four build/scenario pairs. Robotron has no committed oracle,
    so it was checked before-against-after directly: 175,613 memory writes and
    337,926 branch-target trace lines byte-identical, plus 600 frames of video
    hashes.
