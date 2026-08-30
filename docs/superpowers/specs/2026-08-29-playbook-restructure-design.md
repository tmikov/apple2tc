# Restructuring the playbook into one document

**Subject:** `docs/decompiling/playbook.md` · **Date:** 2026-08-29 ·
**Outcome recorded 2026-08-29**

## Outcome, and four corrections to the design below

Executed in the commit that lands this file. The playbook is seven steps, 19 techniques and a 77-row
hazard index; all 195 items were placed, 181 survive, 13 were dropped as
duplicates naming their referent, and 1 stayed in `HANDOFF.md`. Every
mechanical check passes. Four things the design got wrong.

**1. Rule 1 was not workable and was amended before any writing started.** It
said a claim used at more than one step goes into `Techniques and idioms`.
Applied literally that chapter absorbed the oracle-discipline claims — measure
what the gate covers, prove it can fail, a green gate is a claim about the
gate — which step 3 plainly owns, and `Techniques` started becoming the next
bucket. The rule as executed: **each claim has exactly one owning step;
`Techniques and idioms` holds only reusable techniques and 6502 idioms; other
steps cross-reference by name.** Still total, and it keeps the chapter to 19
entries.

**2. Step 3 needed a spine the design did not give it.** 45 items — a quarter
of the document — landed under "stand up the oracle", which is a bucket
forming under a different name. It is four things and now says so: *3a build
it · 3b prove it can fail · 3c measure what it covers · 3d what it cannot
see*. 3d is where the strongest material turned out to live.

**3. Both estimates about content were wrong, in opposite directions.** The
`HANDOFF.md` split was predicted at 10 duplicates / 11 promoted / 4
game-specific and came out **11 / 13 / 1** — more of it transferable than
credited; only `cold_compare`'s frame arithmetic is genuinely one game's. And
the design cited "same-lesson pairs inside the table" as a source of
duplication; only two are real (a liveness-by-reading pair and a table-size
pair). The rest are prose/row pairs, which under an index scheme are correct:
the row *is* the pointer to the prose.

**4. The length prediction was wrong in direction, not just magnitude.**
Predicted ~1,200 lines against 1,438; the result is **1,743**, up 21% rather
than down 17%. The growth is the per-step contract itself — seven steps and
ten substeps now each carry a "what it costs" paragraph that did not exist in
any form — plus 13 promoted hazards, 19 "used by" lines, and an index going
65 rows to 77. Nothing was traded away to get there, and the design did say
length was not a target. But a prediction stated with a number is a claim, and
this one was not met.

## And the check that actually worked was not the one specified

The design's "nothing lost" check was a distinctive-phrase test over the 195
inventory items. It passed at **181/181**, every item scoring 100%, while nine
pieces of evidence were missing from the file: `$630D`, the second `LiveOut`
example at `$6594`, the `$75B3-$75D0`/`$66A9` boundaries that prove an
asserted edge points at code, the 108→152 function-line figure,
`check_literal_sites()` and four more.

It could not have caught them. **A prose-similarity check cannot see a dropped
number, because the sentence around the number survives** — the claim is still
there, only its evidence has gone, and the words are what the check reads. The
test that found them was different in kind: extract every hex address, every
number ≥10 and every function name from the old file and require each to be
present in the new one. It reported 7 addresses, 3 numbers and 1 identifier
missing on the first run, and 60/60, 113/113, 9/9 after they were restored.

Generalise it as: **check the thing that would be lost, not the thing that
would be noticed.** Prose is what a reader notices missing; evidence is what
they do not, which is exactly why the check has to be aimed at the evidence.
This belongs with the playbook's own "count the effect, not the identifier".

---


The playbook's purpose is to make converting the *next* game easier. It has been
written as a living log instead — appended to at the end of each session — and
the appending has produced a document that declares one shape and has another.
This design replaces the shape. Nothing true in it is being thrown away.

---

## What is wrong, measured

Every number below is from the file at `6e240df`.

**It declares a shape it does not have.** The header says the document is
"written in skill shape (when to use → procedure → red flags)". The actual
order and sizes:

| section | lines | share |
| --- | ---: | ---: |
| `## When this applies` | 11 | 1% |
| `## Findings` | 1,096 | **76%** |
| `## Procedure` | 101 | 7% |
| `## Red flags` | 203 | 14% |

`Findings` is not in the declared shape. It is the bucket everything was
appended to, and it is three-quarters of the document.

**The bucket's contents show the accretion.** Seven topical subsections of
41–61 lines, then two genuine chapters — `Converting emulator-shaped C into
real C` (275) and `Getting the machine out of the code` (154) — then **ten
subsections averaging 35 lines**, each one session's lesson with nowhere else
to go:

```
 25  What "converted" is worth, and how to report it
 15  Merging an adapter beats deleting one
 33  `CYCLES` is two mechanisms, and a plan that says otherwise will mislead you
 56  Most of the clock is not the clock
 22  The failure a gate of this kind cannot see
 41  Calibrate the oracle against the change, by mutating what you just changed
 40  A runtime header that *defines* the machine costs you separate compilation
 88  Making it readable, after it is correct
 16  Derive the set; a list written before the work is a starting point
 16  Deriving a scenario instead of recording one
```

The last two have near-identical titles and sit adjacent. There are also two
separate sections named `Hand-decompiling`.

**The scope-tag legend is wrong.** It declares three tags. The body uses five:

| tag | uses | declared? |
| --- | ---: | --- |
| `[process]` | 29 | **no** |
| `[6502]` | 28 | yes |
| `[apple2tc]` | 20 | yes |
| `[tool]` | 3 | **no** |
| `[game]` | 2 (one in the legend itself) | yes |

The most-used tag in the document is defined nowhere.

**The Procedure preamble contradicts its own steps.** "Step 4 is deliberately
two steps, and only the first has been run" sits directly above step 4.2,
marked `*(executed 2026-08-23)*`. "Untested ones are hypothesis, so correct
them from experience" describes a document in which all six steps now read
`(executed)`.

**`Red flags` is two parallel lists in two formats.** Ten prose entries, then a
65-row table which is not an index of them: `DEY`/`BNE` counts a zero as 256,
`A warning you cannot see is still a warning` and `An empty diagnostic list is
not a clean one` have no row. Inside the table there are same-lesson pairs —
*"Reading a routine's body to find its parameters"* / *"A register-liveness
claim you produced by reading"*, and *"A table's size stated rather than
derived"* / *"A table sized to the data it visibly holds"*.

**It defers outward for material it should own.** One reference to
`HANDOFF.md`, sending the reader there for the six cleanup steps. Meanwhile
`HANDOFF.md`'s own 25-row `Traps` table holds **11 hazards that were never
promoted** — the lessons the next game needs, filed under the last game.

---

## The purpose this serves

One document that makes converting the next game easier, organized by the order
in which the work is done. Snake Byte's evidence stays, as evidence for the
claims; Snake Byte's chronology does not.

`decision-log.md` remains the append-only record of *why*, unchanged and
unedited — that is its charter. The playbook states conclusions; the log states
how they were arrived at, including the wrong turns.

---

## The shape

```
# Playbook: converting apple2tc output into maintainable C
    front matter — what this is, when it applies, preconditions,
    maturity, how to read the tags

## The procedure
    1. Scope the boundary
    2. Recover procedures
    3. Stand up the oracle
    4. Cut the library boundary
         4a. Externalize the entry points
         4b. Retarget the entry point
    5. Structural conversion
    6. Get the machine out of the code
         names → storage → flags and registers → tables → byte pairs → the clock
    7. Make it an artifact
         readability, file boundaries, warnings, what ships vs what verifies

## Techniques and idioms
## Hazard index
```

`## Findings` ceases to exist. That is the change that matters: a document with
a bucket refills the bucket.

### Step 7 is new as a step, not as material

The current procedure ends at step 6 and the work does not. `Making it
readable, after it is correct` (88 lines) and `A runtime header that *defines*
the machine costs you separate compilation` (40 lines) are already in the
document with no home, and the most recent Snake Byte work — removing 161 of
165 casts, splitting `rom.c` out of the game, gating on every compiler rather
than the configured one — is the same phase. It is the phase that runs after
the artifact is correct and before it is worth reading.

### Step 6 absorbs the forwarded cleanup steps

The Procedure preamble currently says the six cleanup steps "live in
`HANDOFF.md` rather than here because their order turned out to be
game-independent but their *costs* were not." Game-independent order is exactly
what belongs in a procedure. They come in, and the per-step cost line (below)
is where the game-dependence is stated rather than used as a reason to file
them elsewhere. After this, the playbook contains no reference to `HANDOFF.md`.

---

## The per-step contract

Every step carries the same four things, in this order.

1. **What you do.** The actions, in order.
2. **What it costs.** Which oracle this step spends, and what stops being
   checkable once it has. This is the through-line. The document's own sharpest
   claim is *"the oracles decide the conversion order, not taste"*, and stating
   the cost at each step is what makes this a procedure rather than a list of
   things that are true.
3. **What bites.** The findings, unchanged in substance: a bolded claim, then
   its evidence.
4. **Status.** One line — executed or not, and on which game.

A step with nothing under "what bites" is a step nobody has written up yet, and
saying so is more useful than the current arrangement, where the absence is
invisible.

---

## The rules that keep it one document

Five rules. Each is mechanically checkable, which is what distinguishes them
from an intention.

1. **Every claim lives under the step where you would act on it.** A claim used
   at more than one step goes into `Techniques and idioms` instead, and every
   step that uses it must *name* it. An entry nothing names is deleted. The
   rule is total on purpose: exactly one step means it lives there, two or more
   means it is centralized and named, so no claim is ever duplicated and none
   is homeless.
2. **No new top-level section.** New material amends or displaces existing
   material. This is precisely what the ten orphan subsections did not do, and
   the rule exists to make that failure mode require a deliberate act.
3. **Every hazard-index row names its step.** A hazard with no step is a hazard
   nobody knows when to worry about.
4. **The narrative stays in `decision-log.md`.** The playbook states the
   conclusion. Where a wrong turn is itself the lesson, state the lesson, not
   the history of the paragraph: *"this entry used to say the opposite"*
   becomes a claim about the trap.
5. **The legend and the body agree on tags.** Four tags:

   | tag | means |
   | --- | --- |
   | `[6502]` | true of 6502 / Apple II work generally |
   | `[apple2tc]` | true of this decompiler and its tools |
   | `[process]` | true of how the work is verified and sequenced |
   | `[game]` | observed once, may not generalize |

   `[tool]` retires into `[apple2tc]` — all three uses are `apple2tc --ir` or
   `id`. `[game]` stays despite having one user in 1,438 lines: it marks the
   claims that admit they are one game's, and that admission is worth being
   able to make.

---

## The method: an inventory, not a rewrite from memory

The atomic unit is the bolded lead-in claim and the table row. There are
**~196** of them:

| source | count |
| --- | ---: |
| bolded claims in `playbook.md` | 106 |
| hazard-table rows in `playbook.md` | 65 |
| `Traps` rows in `HANDOFF.md` | 25 |

Each gets one row in a placement table: *item → destination*, where the
destination is a step, a `Techniques and idioms` entry, a hazard-index row, or
`dropped as duplicate of #N`. Two constraints:

- **Nothing may be unplaced.**
- **Anything dropped must name the item it duplicates.** A drop with no
  referent is a deletion pretending to be a merge.

The placement table is reviewed before the file is touched. It is where a fact
would get silently lost, and it is cheaper to read than the diff.

### The 25 HANDOFF traps

Ten already have a playbook counterpart and merge into it. Eleven are promoted
with their evidence, tagged. The remainder are genuinely Snake Byte-specific —
`snake-byte.lst` and the keyboard-read sites, `cold_compare`'s `--frames` pair
— and stay in `HANDOFF.md` rather than being generalized falsely. Exact
membership of each group is decided in the placement table, not here; these
counts are the current estimate and the table is what settles them.

---

## Verification

Run against the result:

| check | passes when |
| --- | --- |
| no bucket | no section titled `Findings` |
| tags | every tag used in the body is in the legend, and every legend tag has ≥1 user |
| hazards | every hazard-index row names a step |
| techniques | every `Techniques and idioms` entry is named from ≥1 step |
| no dangling refs | no "see X above/below" whose target does not exist |
| no outward defer | no reference to `HANDOFF.md` |
| nothing lost | a distinctive phrase from each of the ~196 inventory items is present, or the item is marked dropped with its referent |

The last one is the real check and the others are cheap. It is run from the
placement table, not from memory.

---

## Scope

**In:** restructuring `playbook.md`; promoting the 11 orphan hazards; reducing
`HANDOFF.md`'s `Traps` table to its game-specific rows plus a pointer.

**Out:** `decision-log.md`, which is append-only and correct as it stands.
`HANDOFF.md`'s other sections — whether that document should survive Snake Byte
being finished is a separate question and is not answered here. The plans and
specs under `docs/superpowers/`, which are historical.

**Not attempted:** promoting the playbook to `.claude/skills/`. Its own header
says that should wait until it has survived a second game, and it has not.
Doing it now would be asserting the thing the restructure is meant to make
testable.

---

## What this deliberately does not do

It does not shorten the document as a goal. The estimate is ~1,200 lines
against today's 1,438 — reorganization, plus ~80 lines promoted in, minus
deduplication and the scaffolding around retractions. That is a prediction, not
a target. A claim is not removed for being long, and the check above is
designed to make removal-by-attrition visible.
