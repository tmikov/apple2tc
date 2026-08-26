# Snake Byte: one virtual clock, at the sites where duration is perceptible

**Status:** design, approved 2026-08-25 · **Supersedes:** nothing; extends the
"Cleaning up the C" arc in `HANDOFF.md`

## The problem

`snake-byte-cold.c` charges 6502 cycles at 839 `TICK` sites. Two costs:

- **The game is not hand-editable.** Change a routine and its cycle charges are
  wrong, in units that only mean anything on Apple II hardware.
- **It is hard to read.** Most lines of some routines are cycle bookkeeping.

`TICK` is doing three jobs and only the first is obvious:

1. **Accounting** — `s_cycles += n`.
2. **Yielding.** The game runs on its own thread as a coroutine.
   `run_emulated(n)` tops up a budget, wakes it, and blocks; when the budget is
   spent `cycles_expired()` parks the game *wherever it is* and hands control
   back. The host draws and polls the keyboard in that window. **Every `TICK` is
   a potential suspend point.** Delete them all and nothing hands control back:
   no drawing, no input, `--frames` never terminates.
3. **Oracles** — cycle counts feed `verify.sh` and the frame hashes.

## What the measurements say

Where 22,007,564 cycles of a `play` run go:

| | cycles | share |
| --- | --- | --- |
| `game_setup_screen` (prompt timeout) | 7.9M | 36% |
| `game_tick_sound_native` (the tone) | 6.5M | 30% |
| `game_draw_cell_native` (drawing work) | 3.1M | 14% |
| `spin` (pause after clearing hi-res) | 2.6M | 12% |
| ~830 other sites | 1.9M | 8% |

Durations, measured per call:

| | cycles | frames | ms |
| --- | --- | --- | --- |
| pace loop, one snake step | 103,874 mean | 6.1 | **102** |
| `spin(0,0,4)` | ~1,317,915 | ~77 | **1,290** |
| `clear_hgr` | 106,897 | 6.3 | 105 |
| `draw_border` | 73,939 | 4.3 | 72 |
| `wipe_occupancy_map` | 74,043 | 4.3 | 73 |
| level-1 display list | ~375 | 0 | 0 |

Two conclusions:

- **The fills are not worth preserving.** ~140 ms of visible hi-res sweep per
  level, once. `wipe_occupancy_map` draws to the lo-res page while the display
  is already on hi-res page 2, so a third of it shows nothing at all. Drawing
  becomes instant; no yields inside it.
- **The pace loop is the main loop, not a delay.** Its 82 passes each tick the
  sound, read the keyboard and maybe click the speaker. It already polls input
  every ~1.2 ms.

## The design

**One virtual clock, in 6502 cycles, advanced only where a duration is
perceptible -- audible, or visible as a pause.**

```c
advance(40);                 // one tone-loop pass -- sets the pitch
advance(89);                 // the pace loop's key read -- spaces the clicks
audio_toggle(s_click_port);
```

The ~830 sites that charge for arithmetic nobody can hear or see are deleted.

### Why this needs no new yield API

The coroutine machinery is untouched. `cycles_expired()` still parks the game
when the budget is spent, the host still draws and polls there. The clock just
advances in bigger, rarer, justified steps. No frame-yield placement judgement,
no restructuring, no control inversion.

### Why the audio timeline and the pacing clock are the same clock

Sound is the one thing that genuinely needs sub-frame resolution: the Apple II
speaker is a one-bit cone, reading `$C030` flips it, and the pitch *is* the
interval between reads. A 1 kHz square wave is a toggle every ~510 cycles
against a 17,030-cycle frame -- 30x too coarse to carry in frames.

`a2_sound_spkr()` already takes a cycle timestamp and a clock rate. So the audio
timeline is in cycles; deliberate pauses are in cycles; they are one clock. No
separate wall-clock mechanism is needed.

### The surviving sites

| site | why it is perceptible |
| --- | --- |
| tone loop pass | sets the pitch |
| pace loop: key read, speaker click | spaces the clicks, and makes the waveform irregular the way the original is |
| `spin` | the 1.29 s hold on the cleared screen |
| setup prompt loop | the 7.8 s timeout into demo mode |
| death pause | the buzz after dying |
| blink loops (`edit_key_blank`, `edit_key_prompt`) | the redefinition cursor |
| `rom_wait` | the bell's silence |

Roughly ten. Every number is justifiable by pointing at something a person can
hear or see, which is the property the current 839 lack.

### Calibration: the step that makes it exact

Deleting the arithmetic ticks **shortens the intervals between toggles**,
because those intervals currently include the arithmetic. Delete them naively
and the pitch rises.

So each surviving `advance(n)` is set to the **measured total cycles of the
region it stands for** -- from that site to the next advance -- not to whatever
its old `TICK` charged. The tone loop's pass does not advance 6; it advances
everything the pass costs.

## Verification

**The toggle timeline is the strong oracle, and it is new.** The whole sound
path reduces to a sequence of `(cycle, port)` speaker accesses. Dumped from the
current build and from the converted one, they must be **byte-identical**: not
"close", but the same waveform. This is the best possible check on the part of
the work that cannot be mechanical.

| oracle | fate |
| --- | --- |
| toggle timeline | **new**, and must stay identical |
| `ram-cold.probe`, `screen.probe` | expected to survive -- see the hypothesis below |
| cold block-head trace (6 addresses) | expected to survive |
| frame hashes for the cold build | **given up** |
| `verify.sh`, the ext builds' gates | unaffected -- different source files |

**Hypothesis, to be tested rather than assumed: the game's logic is
cycle-independent.** The pace loop runs `s_step_delay` passes whatever they
cost; the prompt counts iterations; replay delivers keys on a counter that
advances at keyboard *reads*. If that holds, the screen and memory probes pass
unchanged. If it does not, that is a finding worth having.

## The hazard

**Every loop that can block on input must keep an advance**, or the host never
gets a window to poll and the game hangs. The list is the surviving-sites table
above.

This failure is invisible to every automated check: under probe-stamped replay
the key counter advances at the keyboard *read* site, so a spinning loop keeps
incrementing it and receives its key without a frame ever happening. Green gate,
frozen window -- the same signature, and the same underlying reason, as the
`a2host` pacing regression found on 2026-08-25.

**Playing the game is therefore a required verification step, not a courtesy.**

## Staging

0. **Toggle dump + baselines.** Record twice, refuse to write unless they agree.
1. **Measure each region's cycle total**, to calibrate the surviving advances.
2. **The collapse.** Delete the ~830, install the calibrated advances. Toggle
   dump byte-identical; screen and memory probes pass.
3. **Play it.** The hang risk lives here and nowhere else.
4. **Later, separately: the sound routine's structure**, converted with
   understanding rather than mechanically, still checked against the same dump.

Stage 4 is out of scope for this spec.

## Out of scope

- Restructuring the sound routine (stage 4).
- Any change to `game.c` / `game_native.c` / `a2rom.c` or the booting builds.
- Making the fills visible; drawing becomes instant, agreed.
