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
