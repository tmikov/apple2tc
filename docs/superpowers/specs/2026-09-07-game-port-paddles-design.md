# The game port: paddles, buttons, and a virtual joystick on the arrow keys — design

`lib/a2io` implements nothing between `$C060` and `$C07F`. This adds the
device, and drives it from the arrow keys in `a2emu`, so that a game offering
joystick control can actually be played with one.

Closes the emulation half of dz issue `01a074d9`.

## Why this is worth doing, and what it is not

The motivating case is `dsk/Neptune.dsk`, whose keyboard scheme is genuinely
unpleasant: the I/J/K/M diamond does not *hold*, it *toggles*. `$1060` applies
a saturating ±1 to the direction cells, so pressing `K` when already moving
right does nothing, and reversing takes two presses — the opposite key stops
you, the same key again starts you the other way.

Neptune's joystick path does not work that way. `$36B0` reads one axis per
call and **stores an absolute value** — `#$01`, `#$00` or `#$FF` — into the
same two cells `$E3`/`$E4` that the keyboard path toggles. Both meet at the
velocity integrator at `$1180`, which knows nothing about either scheme. So a
held stick is rewritten to "moving" every pass and a centred stick is
rewritten to "stopped":

| loop count `N` | stored |
|---|---|
| 129–144 | `+1` (right / down) |
| 97–128 | `0` (centre) |
| 0–96 | `-1` (left / up) |

Giving Neptune a joystick is therefore not an approximation of better
controls. It hands the game the input model it already contains code for, and
the toggle disappears.

**This does not generalise as far as it might sound.** It buys nothing for
keyboard-only games. It helps only titles that already have a joystick path,
and Neptune happens to be one whose joystick code is better than its keyboard
code.

**Replay is explicitly out of scope.** `--key-file` is a flat
`<cycle> <keycode>` stream with no representation for analog state, and
extending it is a separate piece of work. Until then a stick-driven session is
not reproducible, and this design makes that plainly true rather than
half-true — see *Replay* below.

## Scope

- Emulate `$C060`–`$C07F` in `lib/a2io`: three pushbuttons, four paddle
  one-shots, and the trigger.
- A virtual joystick in `lib/a2host`: digital, self-centring, with a
  configurable button mapping.
- Arrow-key and button bindings in `a2host_gui`, behind an F7 mode toggle.
- Enough of a command-line surface (`--joystick`, `--joy-buttons=`,
  `--paddle=`) to start a session in joystick mode and to pin a paddle
  position in a headless run.

Not in scope: replay/recording of analog input, a real gamepad, an `a2mcp`
stick tool, and any analog policy (ramping, position-holding). The interfaces
below are shaped so each of those is additive.

## The device: `lib/a2io`

The port is **always emulated**, not only while a front end is in joystick
mode. A conditionally-present device would mean `a2run` and `a2mcp` could
never select Neptune's joystick scheme at all, since the selector at `$3584`
reads the *buttons* to decide. The default state is the honest one: a stick is
plugged in, centred, with nothing pressed.

```c
typedef struct {
  uint8_t paddle[4];    ///< True analog positions, 0-255. Centre is 127.
  uint8_t buttons;      ///< Bit n set while PBn is held (n = 0..2).
  uint64_t expiry[4];   ///< Clock value at which each one-shot drops bit 7.
} a2_gameport_t;
```

and, alongside it in `a2_iostate_t`, a monotonic clock that does not wrap:

```c
  uint64_t clock;       ///< Cycles since power-on, extended past 32 bits.
  unsigned clock_last;  ///< Last 32-bit stamp seen, for the extension.
```

embedded in `a2_iostate_t`, plus two setters in `a2io.h`:

```c
void a2_io_set_paddle(a2_iostate_t *io, unsigned n, uint8_t value);
void a2_io_set_button(a2_iostate_t *io, unsigned n, bool down);
```

Both additions are additive, so `decoded/robotron`, `decoded/snake-byte` and
`decoded/bolo` keep linking unchanged.

**They do not keep *behaving* unchanged, and that has to be said plainly.**
All three read the game port — the generated C spells the addresses in
lowercase, which is how an earlier grep for `0xC06x` missed them:

| program | reads | effect of this change |
|---|---|---|
| `decoded/robotron/robotron.c:651,676` | `peek(0xc062)`, `peek(0xc061)` | none in practice: buttons still read 0 because nothing presses them |
| `decoded/robotron/robotron.c:744,794` | `JSR $FB1E`, the ROM `PREAD`, decompiled at `13511`–`13525` | **real change**, and see the limitation below |
| `decoded/snake-byte/snake-byte.c:1679,1736,1741` | `switch_pressed(0xc061..0xc063)`, behind an `s_input.joystick` gate | none, same reason |
| `decoded/bolo/bolo.c:803,809,817` | `peek(0xc070)` then a `peek(0xc064 + s_x)` timing loop | **real change**: the loop currently exits on its first pass with a count of 0 and will now spin to a reading |

Both paddle readers change from "always 0" to a real measurement, which is the
right direction — a machine with a stick plugged in reads centred, not hard
left. But "right" is a claim to verify, not to assume, so their acceptance runs
are part of the regression set below rather than something the additive-API
argument is allowed to wave away.

**And in Robotron's case the new reading will not be 127.** Its decompiled
`PREAD` charges cycles per *basic block*, up front: `CYCLES(0xfb25, 9)` for
the sample-and-branch block and `CYCLES(0xfb2a, 6)` for the increment-and-
branch block, so the generated loop advances the counter 15 cycles per
iteration where the hardware takes 11. Against a device calibrated to the
hardware, a centred paddle reads as roughly 93. This is a pre-existing
fidelity gap in the generated code's cycle accounting, not something this
design introduces — but implementing the port is what makes it observable, so
it is stated here and accepted as a limitation rather than fixed here. Filed
as dz `01a07f30-8dce`. Note the calibration test cannot catch it: that runs
under the interpreter, whose accounting is per instruction and correct.

### Decode

`a2_io_peek()` dispatches on `switch (addr & 0xCFF0)`, which collapses
`$C060`–`$C06F` into the existing `A2_TAPEIN` arm and `$C070`–`$C07F` into
`A2_PTRIG`. The `A2_TAPEIN` arm sub-dispatches on `addr & 7`:

| `addr & 7` | meaning |
|---|---|
| 0 | cassette in — unchanged, returns 0, quietly |
| 1–3 | PB0–PB2 |
| 4–7 | PADDL0–PADDL3 |

`$C068`–`$C06F` therefore mirror `$C060`–`$C067`, which is what the hardware
does: the 74LS251 decodes only A0–A2.

Any access in `$C07X` arms all four one-shots. `a2_io_poke()` already
tail-calls `a2_io_peek()`, so a *write* to `$C070` arms as well — correct,
because the hardware triggers on any access, read or write.

### The one-shot

Every `a2_io_peek()`/`a2_io_poke()` first advances the clock:

```
io->clock += cycles - io->clock_last;
io->clock_last = cycles;
```

Both operands of the subtraction are already `unsigned`, so it is modular
arithmetic on its own -- no cast needed to make it so.

Arming then sets, for each paddle:

```
expiry[n] = io->clock + CYCLES_PER_COUNT * paddle[n] + OFFSET
```

and a read of `PADDLn` returns `0x80` while `io->clock < expiry[n]`, `0x00`
otherwise. A plain 64-bit comparison, with no modular arithmetic anywhere.

**This is deliberately not a 32-bit trick, because both of the obvious ones
are wrong.** Storing an absolute 32-bit `expiry` and testing
`(int32_t)(cycles - expiry) < 0` fails immediately: that comparison is valid
only while the two stamps are within 2³¹ of each other, which holds for a
deadline in the near future but not for one arbitrarily far in the past. With
a zero-initialised `expiry`, a read at cycle `0x80000000` reports the one-shot
as still high on a machine that has never touched `$C070`, and every expired
timer resurrects about 35 minutes after it fired.

Measuring elapsed time from a 32-bit trigger instead —
`(unsigned)(cycles - trigger) < duration[n]` — fixes the never-triggered case
and the 35-minute case, but only *postpones* the defect rather than removing
it. Exactly 2³² cycles after a trigger the counter presents the same value
again, the elapsed count reads as zero, and the one-shot comes back for the
length of its duration. That is a ~1.4 ms window once every ~71 minutes of
emulated time. Rare is not the same as correct, and a bug that fires twice a
day in a long session is worse than one that fires constantly, because nothing
will ever reproduce it on purpose.

Extending the counter removes the class instead of narrowing it, which is why
`a2host` also calls

```c
void a2_io_tick(a2_iostate_t *io, unsigned cycles);
```

once per frame from `a2host_simulate_frame()`, immediately after
`run_emulated()`, with the current `get_cycles()`.

**The contract the extension needs, stated exactly:** consecutive stamps must
be **strictly less than 2³² cycles apart** — a gap of exactly one full wrap
presents the same value and yields a delta of zero, which is the very failure
this replaces — and they must arrive in execution order, with equal stamps
allowed. A reset of the CPU counter must be paired with reinitialising the IO
state, so the restart is not seen as a wrap.

All three hold. A fixed-step frame is ~17,000 cycles and a wall-clock slice is
capped at 0.200 s ≈ 204,600 cycles (the slice cap in `a2host_simulate_frame()`),
both far below the limit. `a2host_reboot()` restarts the counter at zero but
goes through `a2_io_done()` and `a2_io_init()`, which clear the clock and every
deadline with it. A decoded program that never calls `a2_io_tick()` still
advances the clock on every soft-switch access; that this leaves it relying on
the same bound is documented at the function rather than assumed silently.

The advance happens **first** in `a2_io_peek()`/`a2_io_poke()`, before
`a2_io_peek()`'s Disk II boot-ROM early return and its dispatch into
`a2_disk2_peek()` for the Disk II soft switches, so a read observes its own
cycle. `a2_io_poke()`'s tail-call to `a2_io_peek()` carries the same stamp, so
the second advance adds zero.

At init everything is zero, so `0 < 0` is inactive at any clock value.

`CYCLES_PER_COUNT` is **11**. That is the figure the ROM's `PREAD` loop is
calibrated against (`LDA abs,X` 4 + `BPL` not taken 2 + `INY` 2 + `BNE` taken
3), and Neptune's hand-rolled loop independently matches it at 11.

`OFFSET` absorbs the gap between the trigger and a reader's first sample.
**Calibration constrains it to an interval rather than to a single value, and
the design says which end of that interval to take.** `PREAD` first samples at
trigger + 10 and then every 11 cycles, so every integer `OFFSET` in 0–10
reproduces the requested count exactly; 11 and above already fail at position
0. "Whatever value makes `PREAD` agree" therefore does not pick one.

The rule is: **take the midpoint of the passing interval.** The reason is
phase, not taste. A game that rolls its own read loop samples at a different
offset from the trigger than the ROM does — Neptune's prologue is 9 cycles
where `PREAD`'s is 6 — so a value sitting at either end of the interval is one
cycle of phase away from reading a different count in some *other* program,
while the ROM test still passes. The midpoint is the choice that maximises
margin for loops the calibration cannot see. The calibration test asserts the
interval; the midpoint rule picks within it, and both belong in the code as a
comment.

Three consequences worth stating:

- **The low seven bits read as 0.** Real hardware floats the bus there.
  Returning a deterministic zero is a deliberate divergence, because the
  byte-identity guarantee between `a2emu --headless` and `a2run` that
  `tests/run-tests.sh` enforces is worth more than an emulated open bus. This
  gets a comment at the site saying so.
- **The 32-bit cycle counter is extended here rather than worked around.**
  `a2_io_peek()` receives an `unsigned` that wraps roughly every 71 minutes of
  emulated time. `a2_disk2_t::motor_off_cycle` is already `uint64_t` for a
  related reason, so a wider clock is the established shape in this struct;
  what is new is `a2_io_tick()`, which is what makes the extension safe for a
  machine that goes a long time without touching a soft switch.
- **`a2_io_init()` must centre the paddles explicitly.** It currently
  `memset`s the struct to zero, and zero is hard left and hard up, not
  centre. Left as is, an unattended machine reads as a player shoving the
  stick into a corner — exactly the failure dz `01a074d9` predicts for
  Neptune today, reintroduced through the init path instead of the peek path.
  This is load-bearing and has a test of its own.

## The policy: `lib/a2host`

`a2host` owns the *virtual joystick*: that an arrow key means "rail", that
centre is 127, and which PB a given host button drives. `a2io` learns none of
this.

New in `a2host_api.h` — the front-end-facing header, which is the correct one:
this is what a front end calls, not what an engine calls.

```c
/// The four direction keys. Tracked here, not in the front end, so that
/// a2host_joy_release_all() can actually clear them -- see below.
typedef enum {
  A2HOST_JOYDIR_LEFT,
  A2HOST_JOYDIR_RIGHT,
  A2HOST_JOYDIR_UP,
  A2HOST_JOYDIR_DOWN,
  A2HOST_JOYDIR_COUNT
} a2host_joydir_t;

/// Host buttons, named rather than numbered so --joy-buttons= can talk about
/// them without this library knowing anything about sokol keycodes.
typedef enum {
  A2HOST_JOYBTN_SPACE,
  A2HOST_JOYBTN_ALT,
  A2HOST_JOYBTN_CTRL,
  A2HOST_JOYBTN_COUNT
} a2host_joybtn_t;

/// Press or release a direction key. PADDL0 is derived from right/left and
/// PADDL1 from down/up.
void a2host_joy_set_dir(a2host_joydir_t dir, bool down);

/// Press or release a host button. Which PB it drives is --joy-buttons=.
void a2host_joy_set_button(a2host_joybtn_t btn, bool down);

/// Release every direction key and every button, and push the resulting
/// centred, unpressed state to the device.
void a2host_joy_release_all(void);
```

**All held-key state lives here, and that placement is the whole point.** An
earlier shape of this design had the front end hold `left_held`/`right_held`
and call a "set axis to -1/0/+1" entry point. That splits the state in two,
and `release_all()` can only reach half of it: centring the *axis* leaves the
front end's booleans set. Hold Left, lose focus, release Left outside the
window (no `KEY_UP` is delivered), come back and press Right — the stale
`left_held` makes `right - left` zero, so the stick reads centred while Right
is held, then rails *left* when Right is released. Mode toggles and reboots
break the same way. One owner, one reset.

It also removes the question of how a reboot reaches front-end state:
`a2host_reboot()` calls `a2host_joy_release_all()` directly, with no callback
into the GUI.

The mapping is digital and self-centring: `-1` → 0, `0` → 127, `+1` → 255.
Only PADDL0 and PADDL1 are reachable this way. PADDL2 and PADDL3 exist in the
device and are settable with `--paddle=`, but no key drives them — a second
stick has no binding and does not need one here.
Position 127 is the hardware centre and is what a game reading the raw value
expects. It also sits inside Neptune's 97–128 band with a unit to spare, and
since the emulation is deterministic rather than noisy there is no jitter to
absorb. Tuning the emulator's idea of "centre" to one game's asymmetric
deadband would be the wrong trade.

`a2host_joy_release_all()` is not a convenience. It is the fix for every
stuck-stick case — focus lost mid-hold, mode switched off mid-hold, machine
rebooted — and is called from all of them.

### Options

| option | meaning |
|---|---|
| `--joystick` | start in joystick mode, so a session can begin without reaching for F7 |
| `--joy-buttons=space:1,alt:0` | which PB each host button drives; unlisted buttons keep the default `space:0, alt:1, ctrl:2` |
| `--paddle=<n>:<value>` | initial position of paddle *n*, for a headless run |

All three are parsed by `a2host`, because that is where `parse_args` lives.
Only `a2host_gui` acts on `--joystick`: a front end with no keyboard has no
mode to be in, so `a2run` accepts and ignores it rather than rejecting an
option its GUI twin understands. `a2mcp` never gets that far: `mcp_main.cpp`
filters argv against its own hardcoded whitelist (`--root=`, `--probe=`,
`--probe-out=`, `--keys-out=`, `--hash-frames=`, `--help`) before
`a2host_parse_args()` is ever reached, so `--joystick` is rejected there —
the same pre-existing treatment it gives `--fast` and `--hgr-mode=`.
`--paddle=` sets a starting position only; anything that writes that paddle
afterwards — an arrow key, a future gamepad — overrides it.

`--joy-buttons=` resolves *names*, which is why `a2host_joybtn_t` is named
rather than numbered: `a2host` maps `"space"` to a slot, and `a2host_gui`
separately maps a sokol keycode to the same slot. That keeps `a2host`
sokol-free, which `a2host_api.h`'s own header comment identifies as the
property letting the console front end skip linking a graphics stack.

**Two host buttons may legitimately land on the same PB, so each PB is the OR
of its contributors.** `--joy-buttons=space:1` looks like a minimal override
but leaves Alt on its default of PB1, pointing both host buttons at the same
input. Forwarding each transition straight through would mean releasing either
key clears PB1 while the other is still held. `a2host` therefore keeps the
three host-button states and recomputes each PB as the OR of every host button
mapped to it. Duplicate destinations are allowed and well defined rather than
rejected — a player who wants two keys on the trigger should get two keys on
the trigger. This has a policy test of its own.

Neptune is wired the unusual way round — `$C062` (PB1) is fire and `$C061`
(PB0) is the secondary weapon, where most games put the trigger on PB0 — so
the default mapping is the conventional one and Neptune gets a documented
override, `--joy-buttons=space:1,alt:0`, recorded in `docs/neptune.md`
alongside the existing key table.

`--paddle=` exists because the calibration test needs a lever: without it
there is no way to place a stick at position 200 in a headless run, and the
timing constant could only be checked against the same arithmetic that
produced it. It is independently useful for `a2run`.

### Replay

While `a2host_key_replay_active()` is true, joystick input is ignored, exactly
as live keystrokes already are. Joystick state is **not** written to
`--record-keys`.

This is the honest position given replay is out of scope. A stick-driven
session is not reproducible, and it is better for that to be plainly true than
for a run to be half-recorded and diverge silently on playback. When the
replay format is extended, this is the seam it plugs into.

## The bindings: `a2host_gui`

A new `handle_joystick_key()`, placed **after** the replay guard in
`event_cb()`, not alongside `handle_display_key()`. That placement is
deliberate and deserves its comment: `handle_display_key()` justifies running
*ahead* of the guard on the grounds that F3–F6 never reach the machine, so a
replay cannot be perturbed. F7 and the stick do reach the machine.

While joystick mode is on:

| key | effect |
|---|---|
| arrows | `a2host_joy_set_dir()` on both `KEY_DOWN` and `KEY_UP` |
| Space / Left-Alt / Left-Ctrl | `a2host_joy_set_button()`, subject to `--joy-buttons=` |
| F7 | toggle mode; on the way *out*, `a2host_joy_release_all()` |
| anything else | reaches the machine exactly as today |

Mode defaults to off, so nothing regresses for anyone not asking for it.

`event_cb()` currently handles only `CHAR` and `KEY_DOWN`. **A `KEY_UP` path
does not exist and must be added** — it is what makes a held key a held stick.

**Opposite arrows held together read as centred.** The axis is
`right_held - left_held` rather than "last press wins". Rolling a thumb from
left to right briefly holds both, and last-press-wins is fine there — but
releasing in the other order then leaves the axis railed in a direction no
longer being pressed. Subtraction has no such state. The derivation and the
four booleans behind it live in `a2host`, not here; the GUI only translates
sokol keycodes into `a2host_joydir_t`.

**Space must be swallowed completely.** Space generates a `CHAR` event as well
as a `KEY_DOWN`. Diverting only the `KEY_DOWN` leaves the `CHAR` calling
`a2host_press_key(32)`, which sets `$C000` and asserts the strobe. The machine
then receives a keystroke the player never intended, and two things follow.

The smaller one is a race at Neptune's title screen. `$3584` tests the buttons
*before* the keyboard, so a poll landing while Space is still held does select
the joystick — the danger is the poll that lands after the button is released,
because the keyboard latch persists until acknowledged while the button does
not. A tap that falls between two polls leaves a stale key as the only
evidence, and the keyboard scheme is installed instead.

The larger one outlives the title screen: the joystick forms the installer
writes at `$1300` and `$138C` branch *past* the `STA $C010` that the keyboard
forms execute, so in joystick mode nothing ever clears the strobe. A spurious
keystroke latched there stays latched.

**The existing `ignoreNextCh_` idiom is not strong enough to do this, and
reaching for it would be a bug.** `event_cb()` copies and clears that flag at
the top of *every* event, so it only survives if the `CHAR` is the very next
event. Two things break that. Bundled sokol emits `KEY_DOWN`, a synthetic
`KEY_UP`, and only then `CHAR` for Super-modified keys on macOS
(`include/apple2tc/sokol/sokol_app.h:3516-3539`) — and this design *adds* a
`KEY_UP` path, so the intervening event now runs through `event_cb()` and
clears the flag before the `CHAR` arrives. A `CHAR` with no preceding
`KEY_DOWN` at all leaks for the same reason.

So the suppression is stated positively instead: while joystick mode is on,
`handle_joystick_key()` drops any `CHAR` event whose character is bound to a
host button — Space, today. That is a condition on the current mode rather
than a one-event flag, so no interleaved event can defeat it, and
`ignoreNextCh_` is left alone to keep doing its existing job for the arrow
keys outside joystick mode.

**Focus loss releases everything.** `SAPP_EVENTTYPE_UNFOCUSED`, `ICONIFIED`
and `SUSPENDED` all call `a2host_joy_release_all()`. A key released outside
the window never delivers `KEY_UP`, so without this an alt-tab mid-hold leaves
the axis railed with nothing able to clear it.

**Status.** The window title is this front end's only status readout, so the
mode belongs there: `a2emu -- HGR:color Persist:off Joy:on`. This renames
`report_display_state()` to `report_status()`, since it no longer reports only
display options — a static function in one file, renamed rather than left
with a misleading name.

## Tests

### Unit, `tests/a2io/a2io_test.c`

- **Centred at init.** After `a2_io_init()`, arm and read `PADDL0`: bit 7 set,
  and still set well past where a zeroed paddle would have dropped it. This is
  the test that fails if someone later trusts the `memset`.
- **Buttons.** PB*n* set reads `0x80` at `$C061+n`, clear reads `0x00`.
- **Mirroring.** `$C068`–`$C06F` read identically to `$C060`–`$C067`.
- **One-shot boundaries.** For 0, 1, 127 and 255: bit 7 set one cycle before
  expiry, clear at it.
- **Re-arming mid-flight** restarts the timer rather than extending it.
- **A machine that has never triggered never reads armed.** Read `PADDL0` at
  cycle `0x80000000` on a freshly initialised device. Catches the
  absolute-deadline formulation.
- **An expired one-shot stays expired at 2³¹.** Arm, let it expire, read again
  more than 2³¹ cycles later. Catches the same formulation by its other door.
- **An expired one-shot stays expired at a full wrap.** Arm, let it expire,
  then advance the 32-bit stamp all the way around to *exactly* the trigger
  value again and read. This is the one that only the extended clock passes —
  both 32-bit formulations report the one-shot as freshly armed here. The
  advance must be made in frame-sized steps that honour the tick contract
  above, not as one jump: a single leap of 2³² would violate the precondition
  and test nothing.
- **The clock extension survives a wrap.** Drive `a2_io_tick()` across the
  `0xFFFFFFFF → 0` boundary in frame-sized steps and assert the one-shot
  expires at the correct absolute time, not 2³² cycles early or late.
- The existing `test_game_port()` stays **unmodified and passing**. It sweeps
  `$C060`–`$C07F` ascending asserting every read is zero, and that remains
  correct: `$C064`–`$C067` are read before `$C070` is ever touched, so the
  one-shots have never been armed.

### Policy, sokol-free and therefore testable

- **Button OR aggregation.** With `--joy-buttons=space:1`, press both Space
  and Alt, release Space, and assert PB1 is still held. This is the case the
  naive pass-through gets wrong.
- **`release_all()` clears the direction keys, not just the axes.** Press
  Left, call `a2host_joy_release_all()`, press Right, assert the axis rails
  right — then release Right and assert it centres. Written against the
  focus-loss sequence it exists to fix.
- **`--joy-buttons=` parsing**, including unlisted buttons keeping defaults
  and rejection of malformed input.

### Calibration, `tests/gameport/pread-acceptance.sh`

`include/apple2tc/apple2plus_rom.h` bundles the real Apple II+ ROM, so
`$FB1E` is genuine `PREAD`. A short `a6502`-assembled program sets `X`, calls
`PREAD` and stores `Y`, for paddles 0–3. Run under `a2run` with four different
`--paddle=` values and assert the four returned counts equal the four
positions set. A handful of runs covers 0, 1, 127, 128, 254 and 255.

Capture needs **both** snapshot options — `a2host.c` rejects `--snapshot-out`
without `--snapshot-at` — so the assembled program stores its four results to
a known address and then reaches a fixed capture address that `--snapshot-at`
names, after the last store. The test reads the four bytes out of the dumped
RAM image.

This is what constrains `OFFSET`: the ROM decides the interval, not the model
that produced it. As noted above the interval has width, so the test asserts
the *ends* — that the chosen constant passes and that stepping outside the
interval fails — rather than pretending the ROM names a single value.

### Mutation

A green test is a claim about the test until something breaks it. Two
deliberate breakages, each confirmed to go red, then reverted:

- `CYCLES_PER_COUNT` 11 → 10 must fail the calibration test.
- Removing the explicit centring in `a2_io_init()` must fail the
  init-centring test.
- Replacing the extended clock with the absolute-deadline form
  `(int32_t)(cycles - expiry) < 0` must fail the never-triggered test.
- Replacing it with the 32-bit elapsed form
  `(unsigned)(cycles - trigger) < duration[n]` must fail the full-wrap test
  and *pass* the other two. This pair matters most: both 32-bit formulations
  looked right in turn, and the mutations are the proof that each test
  rejects the specific thing it was written to reject rather than passing by
  accident.

### Regression

`tests/run-tests.sh` passes unchanged, and the `a2emu --headless` ≡ `a2run`
byte-identity check still holds. That check compares two interpreter front
ends booting the ROM, so it is necessary but not sufficient here: the port is
now always emulated, and the programs that read it are the decoded ones.

So the regression set also includes `decoded/rom/probe-acceptance.sh` and
`decoded/snake-byte/scripts/probe-acceptance.sh`, plus before/after comparisons of
**both** paddle readers — `decoded/bolo` and `decoded/robotron`. A diff in
those two is expected, and Robotron's is expected to be a reading near 93
rather than 127 for the reason given above; an *unexplained* diff anywhere
else is a failure.

### Not tested, stated plainly

The sokol event translation — F7, `KEY_UP`, focus-loss release, the Space
`CHAR` suppression — has no test harness in this repo and does not get one
here. The policy underneath it (`--joy-buttons=` parsing, the rail/centre
mapping, `release_all`) is sokol-free and is unit-tested. The binding layer is
verified by hand against Neptune, and this document records that it is
unverified by machine rather than implying otherwise.

## Known limitation: snapshots do not carry game-port state

`probe_take_snapshot()` (`lib/a2host/probe.c:107`) writes RAM, the registers,
the cycle counter and — deliberately — the soft switches, on the stated
grounds that they are "the half of the machine a RAM image cannot carry":
`vid_control` and the keyboard latch. Paddle positions, button state and any
in-flight one-shot fall in exactly that category, and this design does **not**
add them.

The consequence is bounded and worth naming rather than discovering. A
snapshot taken at a joystick-dependent entry point cannot reconstruct the
stick from RAM, so a run rebased on it resumes centred with nothing pressed.
That costs the calibration test nothing, since it reads result bytes out of
the RAM image rather than restoring a machine.

Adding those fields changes the snapshot register-file format, which is a
separate concern with its own compatibility question. Filed as dz
`01a07f30-c3d9` rather than riding along on this one.

## Acceptance

Booting `dsk/Neptune.dsk` under `a2emu --joystick
--joy-buttons=space:1,alt:0`, pressing Space at the title screen selects the
joystick scheme (`$3584` → `$3700`), and holding an arrow moves the ship for
as long as it is held and stops when released — with no second press needed to
reverse.
