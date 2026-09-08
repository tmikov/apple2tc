# Game Port and Arrow-Key Virtual Joystick Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Emulate the Apple II game port (`$C060`-`$C07F`) in `lib/a2io` and drive it from the arrow keys in `a2emu`, so a game offering joystick control can be played with one.

**Architecture:** Three layers, each testable without the one above it. `a2io` owns the *device* — analog paddle positions, three buttons, one-shot timing against an extended 64-bit clock. `lib/a2host/joystick.c` owns the *policy* as a pure module over a plain struct — held keys become rail-or-centre paddle values, host buttons become an OR'd pushbutton mask. `a2host_gui` owns only the *bindings* — sokol keycodes to policy calls, behind an F7 mode toggle.

**Tech Stack:** C11, CMake + Ninja, sokol (GUI front end only), the repo's own `a6502` assembler for the calibration fixture.

**Spec:** `docs/superpowers/specs/2026-09-07-game-port-paddles-design.md`

## Global Constraints

- `.clang-format` is authoritative: 2-space indent, 100 column limit, attached braces, pointer right-aligned (`char *p`).
- `lib/a2host` must stay sokol-free. `include/apple2tc/a2host_api.h` says this is the property that lets the console front end skip linking a graphics stack. No sokol type or header may appear in `joystick.c`, `joystick.h` or `a2host.c`.
- Build directories are named `cmake-build-<config>`; the generator is Ninja.
- Comments in this repo say *why*, not *what*. Every non-obvious constant and every deliberate divergence from hardware gets a comment giving its reason.
- Paddle centre is `127`. Cycles per count is `11`. `A2_PADDLE_OFFSET` is `5` — the midpoint of the 0–10 interval the ROM permits.
- The port is **always emulated**, never conditional on a front end being in joystick mode.
- Replay is out of scope. No joystick state is written to `--record-keys`, and joystick input is ignored while `a2host_key_replay_active()` is true.

## Build and test commands

```bash
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
ninja -C cmake-build-debug
cd tests && ./run-tests.sh ../cmake-build-debug
```

---

### Task 1: The extended clock in `a2io`

The one-shot cannot be timed with the 32-bit counter `a2_io_peek()` receives. Both 32-bit formulations are wrong — an absolute deadline resurrects expired timers ~35 minutes on, and elapsed-from-trigger resurrects them at exactly one full wrap. This task adds the clock everything else depends on; no game-port state yet.

**Files:**
- Modify: `include/apple2tc/a2io.h` (add fields to `a2_iostate_t`, declare `a2_io_tick`)
- Modify: `lib/a2io/a2io.c` (`advance_clock`, call sites, `a2_io_tick`)
- Test: `tests/a2io/a2io_test.c`

**Interfaces:**
- Consumes: nothing.
- Produces: `io->clock` (`uint64_t`, cycles since power-on) readable by later tasks; `void a2_io_tick(a2_iostate_t *io, unsigned cycles)`.

- [ ] **Step 1: Write the failing test**

Add to `tests/a2io/a2io_test.c`, above `main()`:

```c
/// The 32-bit cycle counter a2_io_peek() receives wraps about every 71
/// minutes of emulated time. The clock behind the paddle one-shots must not,
/// or an expired timer comes back to life. Walk the counter all the way
/// around its wrap in frame-sized steps -- the way a host ticks it -- and
/// check every step was counted.
static void test_clock_extension(void) {
  a2_iostate_t io;
  a2_io_init(&io);

  // One 1/60 s frame at 1.02 MHz, and enough of them to pass 2^32 cycles.
  enum { FRAME_CYCLES = 17030, FRAMES = 300000 };
  uint64_t expected = 0;
  unsigned stamp = 0;
  for (unsigned i = 0; i < FRAMES; ++i) {
    stamp += FRAME_CYCLES;
    expected += FRAME_CYCLES;
    a2_io_tick(&io, stamp);
  }

  printf("  clock: %llu, expected %llu\n",
      (unsigned long long)io.clock, (unsigned long long)expected);
  check(io.clock == expected, "the extended clock counts across the 32-bit wrap");
  check(expected > 0x100000000ull, "and the walk really did pass a wrap");

  a2_io_done(&io);
}
```

Register it in `main()`, before `test_game_port()`:

```c
  test_clock_extension();
```

- [ ] **Step 2: Run test to verify it fails**

```bash
ninja -C cmake-build-debug
```

Expected: FAIL to compile — `a2_io_tick` is not declared, and `a2_iostate_t` has no `clock` member.

- [ ] **Step 3: Add the fields and the declaration**

In `include/apple2tc/a2io.h`, inside `a2_iostate_t`, immediately before `a2_disk2_t disk2;`:

```c
  /// Cycles since power-on, extended past the 32 bits a2_io_peek() receives.
  ///
  /// The paddle one-shots are timed against this rather than against the raw
  /// stamp because 32-bit arithmetic cannot express "already expired" for an
  /// arbitrarily old deadline. An absolute `expiry` compared with
  /// `(int32_t)(cycles - expiry) < 0` reports a one-shot as still high on a
  /// machine that never triggered one, and revives every expired timer about
  /// 35 minutes on; measuring elapsed time from a 32-bit trigger instead only
  /// moves the revival to exactly one full wrap. Widening the clock removes
  /// the class.
  uint64_t clock;
  /// The last 32-bit stamp seen, which is what `clock` is extended from.
  unsigned clock_last;
```

And with the other function declarations:

```c
/// Advance the extended clock without performing an IO access.
///
/// `clock` is exact only while consecutive stamps are **strictly less than**
/// 2^32 cycles apart -- a gap of exactly one wrap presents the same value and
/// yields a delta of zero -- and arrive in execution order, equal stamps
/// allowed. A reset of the CPU counter must be paired with a2_io_init(), so
/// the restart is not mistaken for a wrap.
///
/// Soft-switch accesses advance it on their own. This exists so a host can
/// guarantee the bound for a program that touches no soft switch for a long
/// time: a2host calls it once per frame, and a frame is ~17,000 cycles.
void a2_io_tick(a2_iostate_t *io, unsigned cycles);
```

- [ ] **Step 4: Implement the advance**

In `lib/a2io/a2io.c`, above `a2_io_peek()`:

```c
/// Extend the 32-bit stamp onto io->clock. Both operands of the subtraction
/// are already `unsigned`, so it is modular arithmetic on its own and is
/// exact for any gap shorter than 2^32 cycles.
static void advance_clock(a2_iostate_t *io, unsigned cycles) {
  io->clock += cycles - io->clock_last;
  io->clock_last = cycles;
}

void a2_io_tick(a2_iostate_t *io, unsigned cycles) {
  advance_clock(io, cycles);
}
```

Call it as the **first** statement of `a2_io_peek()`, before the Disk II boot ROM early return, so a read observes its own cycle:

```c
uint8_t a2_io_peek(a2_iostate_t *io, uint16_t addr, unsigned cycles) {
  advance_clock(io, cycles);

  // Disk II boot ROM: $C600-$C6FF (only when a disk is mounted).
```

And as the first statement of `a2_io_poke()`, before its Disk II early return:

```c
void a2_io_poke(a2_iostate_t *io, uint16_t addr, uint8_t value, unsigned cycles) {
  advance_clock(io, cycles);

  // Disk II soft switches need dedicated write handling.
```

`a2_io_poke()` tail-calls `a2_io_peek()` with the same stamp, so the second advance adds zero.

- [ ] **Step 5: Run test to verify it passes**

```bash
ninja -C cmake-build-debug && ./cmake-build-debug/tests/a2io/a2io-test
```

Expected: PASS, including the two new `ok` lines.

- [ ] **Step 6: Commit**

```bash
git add include/apple2tc/a2io.h lib/a2io/a2io.c tests/a2io/a2io_test.c
git commit -m "a2io: extend the cycle counter past 32 bits"
```

---

### Task 2: The game-port device

**Files:**
- Modify: `include/apple2tc/a2io.h` (`a2_gameport_t`, constants, setters)
- Modify: `lib/a2io/a2io.c` (decode, arming, `a2_io_init` centring, setters)
- Test: `tests/a2io/a2io_test.c`

**Interfaces:**
- Consumes: `io->clock` and `a2_io_tick()` from Task 1.
- Produces: `void a2_io_set_paddle(a2_iostate_t *io, unsigned n, uint8_t value)`, `void a2_io_set_button(a2_iostate_t *io, unsigned n, bool down)`, and the constants `A2_PADDLE_CENTER` (127), `A2_PADDLE_CYCLES_PER_COUNT` (11), `A2_PADDLE_OFFSET` (5).

- [ ] **Step 1: Write the failing tests**

Add to `tests/a2io/a2io_test.c`:

```c
/// Arm the one-shots and return the clock value at which paddle `n` drops.
static uint64_t paddle_expiry_for(uint8_t position) {
  return (uint64_t)A2_PADDLE_CYCLES_PER_COUNT * position + A2_PADDLE_OFFSET;
}

/// Bit 7 of a PADDLn read at `cycles`, having triggered at `trigger`.
static bool paddle_high(a2_iostate_t *io, unsigned n, unsigned cycles) {
  return (a2_io_peek(io, (uint16_t)(A2_PADDL0 + n), cycles) & 0x80) != 0;
}

/// A zeroed a2_gameport_t is hard left and hard up, not centre. An unattended
/// machine must not read as a player shoving the stick into a corner, so
/// a2_io_init() sets the positions explicitly and this pins that it does.
static void test_paddle_centred_at_init(void) {
  a2_iostate_t io;
  a2_io_init(&io);

  a2_io_peek(&io, A2_PTRIG, 0);
  uint64_t centre = paddle_expiry_for(A2_PADDLE_CENTER);
  uint64_t zero = paddle_expiry_for(0);

  check(paddle_high(&io, 0, (unsigned)zero + 1),
      "a freshly initialised paddle is not at the zero rail");
  check(paddle_high(&io, 0, (unsigned)centre - 1),
      "it is still high one cycle before the centre position expires");
  check(!paddle_high(&io, 0, (unsigned)centre),
      "and low at it");

  a2_io_done(&io);
}

/// The one-shot boundaries, across the range. Set, trigger, and sample either
/// side of the computed expiry.
static void test_paddle_boundaries(void) {
  static const uint8_t kPositions[] = {0, 1, 127, 255};
  a2_iostate_t io;

  for (unsigned i = 0; i < sizeof(kPositions) / sizeof(kPositions[0]); ++i) {
    uint8_t pos = kPositions[i];
    a2_io_init(&io);
    a2_io_set_paddle(&io, 0, pos);
    a2_io_peek(&io, A2_PTRIG, 0);

    uint64_t expiry = paddle_expiry_for(pos);
    char what[80];
    snprintf(what, sizeof(what), "paddle %u is high until its expiry", pos);
    check(paddle_high(&io, 0, (unsigned)expiry - 1), what);
    snprintf(what, sizeof(what), "paddle %u is low at its expiry", pos);
    check(!paddle_high(&io, 0, (unsigned)expiry), what);

    a2_io_done(&io);
  }
}

/// A machine that has never touched $C070 has no one-shot running, at any
/// cycle. This is what an absolute 32-bit deadline gets wrong: with expiry 0,
/// (int32_t)(0x80000000 - 0) is negative and reads as "still high".
static void test_paddle_never_triggered(void) {
  a2_iostate_t io;
  a2_io_init(&io);
  check(!paddle_high(&io, 0, 0x80000000u),
      "a paddle never triggered reads low at 2^31");
  a2_io_done(&io);
}

/// ...and an expired one stays expired, however long ago it fired. Two
/// distances, because the two wrong formulations fail at different ones: the
/// absolute deadline revives at 2^31, elapsed-from-trigger at a full wrap.
static void test_paddle_stays_expired(void) {
  a2_iostate_t io;

  a2_io_init(&io);
  a2_io_set_paddle(&io, 0, A2_PADDLE_CENTER);
  a2_io_peek(&io, A2_PTRIG, 1000);
  check(!paddle_high(&io, 0, 1000u + 0x80000000u),
      "an expired paddle is still expired 2^31 cycles later");
  a2_io_done(&io);

  // The full wrap has to be walked, not jumped: a single leap of 2^32 breaks
  // a2_io_tick()'s contract and would prove nothing.
  a2_io_init(&io);
  a2_io_set_paddle(&io, 0, A2_PADDLE_CENTER);
  a2_io_peek(&io, A2_PTRIG, 1000);
  enum { FRAME_CYCLES = 17030 };
  unsigned stamp = 1000;
  for (unsigned i = 0; i < 300000; ++i) {
    stamp += FRAME_CYCLES;
    a2_io_tick(&io, stamp);
  }
  check(!paddle_high(&io, 0, 1000),
      "and still expired when the 32-bit stamp comes all the way round");
  a2_io_done(&io);
}

/// Triggering again restarts the timer rather than extending it.
static void test_paddle_rearm(void) {
  a2_iostate_t io;
  a2_io_init(&io);
  a2_io_set_paddle(&io, 0, A2_PADDLE_CENTER);

  uint64_t span = paddle_expiry_for(A2_PADDLE_CENTER);
  a2_io_peek(&io, A2_PTRIG, 0);
  a2_io_peek(&io, A2_PTRIG, 100);
  check(paddle_high(&io, 0, (unsigned)span + 99),
      "re-arming restarts the one-shot from the new trigger");
  check(!paddle_high(&io, 0, (unsigned)span + 100),
      "and it expires a span after that trigger, not the first one");

  a2_io_done(&io);
}

/// The three pushbuttons, and the $C068-$C06F mirror the 74LS251 produces by
/// decoding only A0-A2.
static void test_buttons_and_mirroring(void) {
  a2_iostate_t io;
  a2_io_init(&io);

  check((a2_io_peek(&io, A2_BUTN0, 0) & 0x80) == 0, "PB0 reads low when up");
  a2_io_set_button(&io, 0, true);
  check((a2_io_peek(&io, A2_BUTN0, 0) & 0x80) != 0, "PB0 reads high when held");
  check((a2_io_peek(&io, A2_BUTN1, 0) & 0x80) == 0, "and PB1 is unaffected");
  a2_io_set_button(&io, 0, false);
  check((a2_io_peek(&io, A2_BUTN0, 0) & 0x80) == 0, "PB0 reads low again once released");

  a2_io_set_button(&io, 2, true);
  bool mirrored = true;
  for (unsigned off = 0; off < 8; ++off) {
    uint8_t direct = a2_io_peek(&io, (uint16_t)(A2_TAPEIN + off), 0);
    uint8_t mirror = a2_io_peek(&io, (uint16_t)(A2_TAPEIN + 8 + off), 0);
    if (direct != mirror)
      mirrored = false;
  }
  check(mirrored, "$C068-$C06F mirror $C060-$C067");

  a2_io_done(&io);
}
```

Register all of them in `main()`, before `test_game_port()`:

```c
  test_paddle_centred_at_init();
  test_paddle_boundaries();
  test_paddle_never_triggered();
  test_paddle_stays_expired();
  test_paddle_rearm();
  test_buttons_and_mirroring();
```

- [ ] **Step 2: Run tests to verify they fail**

```bash
ninja -C cmake-build-debug
```

Expected: FAIL to compile — `A2_PADDLE_CENTER`, `a2_io_set_paddle` and `a2_io_set_button` do not exist.

- [ ] **Step 3: Declare the device**

In `include/apple2tc/a2io.h`, add to the existing anonymous `enum` that holds `A2_KBD_QUEUE_SIZE`:

```c
  /// A paddle's centre position, and what a2_io_init() sets. This is the
  /// hardware centre, not a value tuned to any one game's deadband.
  A2_PADDLE_CENTER = 127,
  /// Cycles the one-shot stays high per unit of position. The figure the
  /// ROM's PREAD loop is calibrated against: LDA abs,X 4, BPL not taken 2,
  /// INY 2, BNE taken 3. Neptune's hand-rolled loop at $36B0 matches it.
  A2_PADDLE_CYCLES_PER_COUNT = 11,
  /// The gap between the trigger and a reader's first sample.
  ///
  /// PREAD samples at trigger + 10 and then every 11 cycles, so every value
  /// in 0-10 makes it return exactly the position that was set, and 11
  /// already fails at position 0. Calibration therefore fixes an interval,
  /// not a number, and this is its midpoint: a program that rolls its own
  /// loop samples at a different phase -- Neptune's prologue is 9 cycles
  /// where PREAD's is 6 -- and the midpoint is what leaves those the most
  /// margin.
  A2_PADDLE_OFFSET = 5,
```

Above `a2_iostate_t`, add:

```c
/// The game port: three pushbuttons and four paddle one-shots.
typedef struct {
  /// True analog positions, 0-255. a2_io_init() sets these to
  /// A2_PADDLE_CENTER, because a zeroed struct is hard left and hard up.
  uint8_t paddle[4];
  /// Bit n set while PBn is held (n = 0..2).
  uint8_t buttons;
  /// Clock value at which each one-shot drops bit 7, against a2_iostate_t's
  /// extended clock. Zero means "never armed", which is also "expired".
  uint64_t expiry[4];
} a2_gameport_t;
```

Add the member to `a2_iostate_t`, next to the clock fields:

```c
  /// Game port state; see a2_gameport_t.
  a2_gameport_t gameport;
```

And the setters, next to the other public functions:

```c
/// Set paddle \p n (0-3) to an absolute position, 0-255. Takes effect at the
/// next trigger: a one-shot already in flight keeps the position it was armed
/// with, which is what the hardware does.
void a2_io_set_paddle(a2_iostate_t *io, unsigned n, uint8_t value);
/// Press or release pushbutton \p n (0-2).
void a2_io_set_button(a2_iostate_t *io, unsigned n, bool down);
```

- [ ] **Step 4: Implement the device**

In `lib/a2io/a2io.c`, centre the paddles in `a2_io_init()`:

```c
void a2_io_init(a2_iostate_t *io) {
  memset(io, 0, sizeof(*io));
  io->vid_control = A2_VC_TEXT;
  // Explicitly, because memset leaves them at 0 -- hard left and hard up.
  // A machine nobody is touching must read as a centred stick, not as one
  // shoved into a corner. See dz 01a074d9.
  for (unsigned n = 0; n < 4; ++n)
    io->gameport.paddle[n] = A2_PADDLE_CENTER;
  a2_disk2_init(&io->disk2);
}
```

Add the setters near the other public functions:

```c
void a2_io_set_paddle(a2_iostate_t *io, unsigned n, uint8_t value) {
  if (n < 4)
    io->gameport.paddle[n] = value;
}

void a2_io_set_button(a2_iostate_t *io, unsigned n, bool down) {
  if (n >= 3)
    return;
  if (down)
    io->gameport.buttons |= (uint8_t)(1u << n);
  else
    io->gameport.buttons &= (uint8_t)~(1u << n);
}
```

Replace the combined `case A2_TAPEIN: case A2_PTRIG: break;` arm — and its now-obsolete comment about the port not being emulated — with:

```c
  // Game I/O. The switch above dispatches on addr & 0xCFF0, so this arm is
  // the whole of $C060-$C06F; the 74LS251 behind it decodes only A0-A2, which
  // is why $C068-$C06F mirror $C060-$C067.
  //
  // The low seven bits read as 0. Real hardware floats the bus there; a
  // deterministic zero is a deliberate divergence, because a2emu --headless
  // and a2run are required to be byte-identical and an emulated open bus
  // would put a house on sand.
  case A2_TAPEIN:
    switch (addr & 7) {
    case 0:
      // Cassette input, which shares the page and is not emulated.
      break;
    case 1:
    case 2:
    case 3:
      return (io->gameport.buttons >> ((addr & 7) - 1)) & 1 ? 0x80 : 0x00;
    default:
      return io->clock < io->gameport.expiry[(addr & 7) - 4] ? 0x80 : 0x00;
    }
    break;

  // PTRIG: any access in $C070-$C07F fires all four one-shots. a2_io_poke()
  // tail-calls this function, so a write triggers too -- which is correct,
  // the hardware triggers on any access.
  case A2_PTRIG:
    for (unsigned n = 0; n < 4; ++n)
      io->gameport.expiry[n] = io->clock +
          (uint64_t)A2_PADDLE_CYCLES_PER_COUNT * io->gameport.paddle[n] + A2_PADDLE_OFFSET;
    break;
```

- [ ] **Step 5: Run tests to verify they pass**

```bash
ninja -C cmake-build-debug && ./cmake-build-debug/tests/a2io/a2io-test
```

Expected: PASS, and `test_game_port()` — which sweeps `$C060`-`$C07F` ascending asserting zero — must still pass **unmodified**, because it reads the paddles before it ever touches `$C070`.

- [ ] **Step 6: Commit**

```bash
git add include/apple2tc/a2io.h lib/a2io/a2io.c tests/a2io/a2io_test.c
git commit -m "a2io: emulate the game port"
```

---

### Task 3: The joystick policy module

A pure module over a plain struct: no statics, no `a2io` dependency, no sokol. That is what makes the policy testable on its own, and it is where the two subtle rules live — held-key state that `reset` can actually reach, and pushbuttons as the OR of their contributors.

**Files:**
- Create: `lib/a2host/joystick.h`
- Create: `lib/a2host/joystick.c`
- Create: `tests/a2host/joystick_test.c`
- Create: `tests/a2host/CMakeLists.txt`
- Modify: `lib/a2host/CMakeLists.txt`
- Modify: `CMakeLists.txt:55` (add the test subdirectory)
- Modify: `include/apple2tc/a2host_api.h` (the two enums, which `joystick.h` uses)

**Interfaces:**
- Consumes: nothing.
- Produces: `a2host_joy_t` and `a2host_joy_init/parse_buttons/dir/button/reset/paddle/pbmask`, plus the enums `a2host_joydir_t` and `a2host_joybtn_t`.

- [ ] **Step 1: Declare the enums in the public header**

In `include/apple2tc/a2host_api.h`, in the Setup section:

```c
/// The four direction keys of the virtual joystick.
///
/// Held-key state lives in the host, not the front end. Splitting it means
/// a reset can only reach half: centre the axis while the front end still
/// believes Left is held, and the next Right press reads as centred, then
/// rails *left* when Right is released.
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
```

- [ ] **Step 2: Write the failing tests**

Create `tests/a2host/joystick_test.c`:

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The virtual joystick's policy, which is the half of the feature that can
/// be tested. The binding layer above it is sokol events and has no harness.

#include "../../lib/a2host/joystick.h"

#include "apple2tc/a2io.h"

#include <stdio.h>
#include <string.h>

static int failures;

static void check(bool ok, const char *what) {
  printf("%s: %s\n", ok ? "ok  " : "FAIL", what);
  if (!ok)
    ++failures;
}

/// A held direction rails its axis; releasing centres it.
static void test_dir_rails_and_centres(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);

  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "an untouched axis is centred");

  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, true);
  check(a2host_joy_paddle(&j, 0) == 255, "holding Right rails the X axis high");
  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, false);
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "releasing it centres again");

  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, true);
  check(a2host_joy_paddle(&j, 0) == 0, "holding Left rails it low");
  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, false);

  a2host_joy_dir(&j, A2HOST_JOYDIR_DOWN, true);
  check(a2host_joy_paddle(&j, 1) == 255, "Down rails the Y axis high");
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "and leaves X alone");
  a2host_joy_dir(&j, A2HOST_JOYDIR_DOWN, false);

  a2host_joy_dir(&j, A2HOST_JOYDIR_UP, true);
  check(a2host_joy_paddle(&j, 1) == 0, "Up rails it low");
}

/// Both opposite keys held is centred, not last-press-wins. Rolling a thumb
/// from one to the other holds both briefly; last-press-wins survives that
/// but leaves the axis railed when they are released in the other order.
static void test_opposite_dirs_centre(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);

  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, true);
  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, true);
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "both horizontal keys held is centred");

  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, false);
  check(a2host_joy_paddle(&j, 0) == 255, "releasing Left leaves Right railing it");
}

/// reset() must clear the held-key booleans, not merely the derived axis.
/// The sequence is the one a lost window focus produces: the KEY_UP for a key
/// released outside the window never arrives.
static void test_reset_clears_held_keys(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);

  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, true);
  a2host_joy_reset(&j); // focus lost; Left is released outside the window
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "reset centres the axis");

  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, true);
  check(a2host_joy_paddle(&j, 0) == 255, "and a later Right rails right, not centre");
  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, false);
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "releasing it centres, not rails left");

  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  a2host_joy_reset(&j);
  check(a2host_joy_pbmask(&j) == 0, "reset releases the buttons too");
}

/// Default mapping: space->PB0, alt->PB1, ctrl->PB2.
static void test_default_button_mapping(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);

  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  check(a2host_joy_pbmask(&j) == 0x1, "Space drives PB0 by default");
  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, false);

  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, true);
  check(a2host_joy_pbmask(&j) == 0x2, "Alt drives PB1");
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, false);

  a2host_joy_button(&j, A2HOST_JOYBTN_CTRL, true);
  check(a2host_joy_pbmask(&j) == 0x4, "Ctrl drives PB2");
}

/// Neptune's override, and the aggregation it exposes. --joy-buttons=space:1
/// leaves Alt on its default of PB1, so two host buttons drive one input:
/// releasing either must not clear it while the other is held.
static void test_button_remap_and_or(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);
  check(a2host_joy_parse_buttons(&j, "space:1,alt:0"), "the Neptune mapping parses");

  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  check(a2host_joy_pbmask(&j) == 0x2, "Space now drives PB1, which is Neptune's fire");
  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, false);
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, true);
  check(a2host_joy_pbmask(&j) == 0x1, "and Alt drives PB0");
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, false);

  a2host_joy_init(&j);
  check(a2host_joy_parse_buttons(&j, "space:1"), "a partial mapping parses");
  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, true);
  check(a2host_joy_pbmask(&j) == 0x2, "Space and Alt both on PB1 is one held button");
  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, false);
  check(a2host_joy_pbmask(&j) == 0x2, "releasing one leaves PB1 held by the other");
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, false);
  check(a2host_joy_pbmask(&j) == 0, "releasing both clears it");
}

/// Malformed mappings are rejected rather than half-applied.
static void test_button_remap_rejects_junk(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);
  check(!a2host_joy_parse_buttons(&j, "space:3"), "a PB number above 2 is rejected");
  check(!a2host_joy_parse_buttons(&j, "mouse:1"), "an unknown button name is rejected");
  check(!a2host_joy_parse_buttons(&j, "space"), "a missing colon is rejected");
  check(!a2host_joy_parse_buttons(&j, ""), "an empty mapping is rejected");

  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  check(a2host_joy_pbmask(&j) == 0x1, "and a rejected mapping left the defaults alone");
}

int main(void) {
  test_dir_rails_and_centres();
  test_opposite_dirs_centre();
  test_reset_clears_held_keys();
  test_default_button_mapping();
  test_button_remap_and_or();
  test_button_remap_rejects_junk();

  if (failures) {
    printf("\n%d joystick check(s) failed\n", failures);
    return 1;
  }
  printf("\njoystick: all checks passed\n");
  return 0;
}
```

- [ ] **Step 3: Run to verify it fails**

```bash
ninja -C cmake-build-debug
```

Expected: FAIL — `lib/a2host/joystick.h` does not exist and no `joystick-test` target is defined.

- [ ] **Step 4: Write the module**

Create `lib/a2host/joystick.h`:

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The virtual joystick's policy: what a held arrow key means, and which
/// pushbutton a host button drives.
///
/// A plain struct with pure functions over it, deliberately: no statics, no
/// a2io, no sokol. The device is analog and lives in a2io; the decision that
/// an arrow key means "rail" is here; the decision that the Left arrow key is
/// the one that does it belongs to a front end. Keeping this layer free of
/// the other two is what lets it be tested at all.

#pragma once

#include "apple2tc/a2host_api.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  /// Which direction keys are down. Held state lives here rather than in the
  /// front end so a2host_joy_reset() can reach it -- see the enum's comment.
  bool dirHeld[A2HOST_JOYDIR_COUNT];
  /// Which host buttons are down.
  bool btnHeld[A2HOST_JOYBTN_COUNT];
  /// Which pushbutton (0-2) each host button drives. Two host buttons may
  /// legitimately name the same one.
  uint8_t btnPB[A2HOST_JOYBTN_COUNT];
} a2host_joy_t;

/// Nothing held, and the conventional mapping: space->PB0, alt->PB1,
/// ctrl->PB2.
void a2host_joy_init(a2host_joy_t *j);

/// Apply a `--joy-buttons=` mapping such as "space:1,alt:0". Unlisted buttons
/// keep whatever they had. Returns false and changes nothing on malformed
/// input.
bool a2host_joy_parse_buttons(a2host_joy_t *j, const char *spec);

/// Press or release a direction key.
void a2host_joy_dir(a2host_joy_t *j, a2host_joydir_t dir, bool down);

/// Press or release a host button.
void a2host_joy_button(a2host_joy_t *j, a2host_joybtn_t btn, bool down);

/// Release every direction key and every button.
void a2host_joy_reset(a2host_joy_t *j);

/// The position for paddle \p axis: 0 for axis 0 (X, from left/right) and 1
/// for axis 1 (Y, from up/down). Digital and self-centring: a rail or the
/// centre, never anything between.
uint8_t a2host_joy_paddle(const a2host_joy_t *j, unsigned axis);

/// The pushbutton mask, bit n set while PBn is held. Each bit is the OR of
/// every host button mapped to it.
uint8_t a2host_joy_pbmask(const a2host_joy_t *j);
```

Create `lib/a2host/joystick.c`:

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "joystick.h"

#include "apple2tc/a2io.h"

#include <stdlib.h>
#include <string.h>

/// The --joy-buttons= vocabulary. Names, not sokol keycodes: a front end maps
/// its own keys onto these, which is what keeps this library sokol-free.
static const char *const kButtonNames[A2HOST_JOYBTN_COUNT] = {"space", "alt", "ctrl"};

void a2host_joy_init(a2host_joy_t *j) {
  memset(j, 0, sizeof(*j));
  for (unsigned i = 0; i < A2HOST_JOYBTN_COUNT; ++i)
    j->btnPB[i] = (uint8_t)i;
}

bool a2host_joy_parse_buttons(a2host_joy_t *j, const char *spec) {
  // Parsed into a copy so a malformed entry half way through leaves the
  // caller's mapping untouched rather than partly rewritten.
  uint8_t parsed[A2HOST_JOYBTN_COUNT];
  memcpy(parsed, j->btnPB, sizeof(parsed));

  if (!spec || !*spec)
    return false;

  for (const char *p = spec; *p;) {
    const char *colon = strchr(p, ':');
    if (!colon)
      return false;

    size_t nameLen = (size_t)(colon - p);
    unsigned btn = A2HOST_JOYBTN_COUNT;
    for (unsigned i = 0; i < A2HOST_JOYBTN_COUNT; ++i) {
      if (strlen(kButtonNames[i]) == nameLen && memcmp(p, kButtonNames[i], nameLen) == 0) {
        btn = i;
        break;
      }
    }
    if (btn == A2HOST_JOYBTN_COUNT)
      return false;

    char *end = NULL;
    unsigned long pb = strtoul(colon + 1, &end, 10);
    if (end == colon + 1 || pb > 2)
      return false;
    parsed[btn] = (uint8_t)pb;

    if (*end == 0)
      break;
    if (*end != ',')
      return false;
    p = end + 1;
  }

  memcpy(j->btnPB, parsed, sizeof(parsed));
  return true;
}

void a2host_joy_dir(a2host_joy_t *j, a2host_joydir_t dir, bool down) {
  if (dir < A2HOST_JOYDIR_COUNT)
    j->dirHeld[dir] = down;
}

void a2host_joy_button(a2host_joy_t *j, a2host_joybtn_t btn, bool down) {
  if (btn < A2HOST_JOYBTN_COUNT)
    j->btnHeld[btn] = down;
}

void a2host_joy_reset(a2host_joy_t *j) {
  memset(j->dirHeld, 0, sizeof(j->dirHeld));
  memset(j->btnHeld, 0, sizeof(j->btnHeld));
}

uint8_t a2host_joy_paddle(const a2host_joy_t *j, unsigned axis) {
  bool low = axis == 0 ? j->dirHeld[A2HOST_JOYDIR_LEFT] : j->dirHeld[A2HOST_JOYDIR_UP];
  bool high = axis == 0 ? j->dirHeld[A2HOST_JOYDIR_RIGHT] : j->dirHeld[A2HOST_JOYDIR_DOWN];

  // Subtraction rather than last-press-wins: rolling a thumb from one key to
  // the other holds both for a moment, and last-press-wins then leaves the
  // axis railed in a direction nobody is pressing once they are released in
  // the other order.
  int dir = (high ? 1 : 0) - (low ? 1 : 0);
  return dir > 0 ? 255 : dir < 0 ? 0 : (uint8_t)A2_PADDLE_CENTER;
}

uint8_t a2host_joy_pbmask(const a2host_joy_t *j) {
  // OR, not assignment: --joy-buttons=space:1 leaves alt on its default of
  // PB1, so two host buttons drive one input and releasing either must not
  // clear it while the other is held.
  uint8_t mask = 0;
  for (unsigned i = 0; i < A2HOST_JOYBTN_COUNT; ++i)
    if (j->btnHeld[i])
      mask |= (uint8_t)(1u << j->btnPB[i]);
  return mask;
}
```

- [ ] **Step 5: Wire the build**

In `lib/a2host/CMakeLists.txt`, add the module to the `a2host` library sources:

```cmake
add_library(a2host
  a2host.c ${A2TC_INC}/a2host_api.h
  joystick.c joystick.h
  probe.c probe_lex.c probe_parse.c probe_vm.c probe_internal.h ${A2TC_INC}/probe.h
  ${A2TC_INC}/a2engine.h ${A2TC_INC}/a2host.h ${A2TC_INC}/system.h
  )
```

Create `tests/a2host/CMakeLists.txt`:

```cmake
# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# The virtual joystick's policy: pure functions over a struct, so this links
# the one translation unit and nothing else. The bindings above it are sokol
# events and have no harness; see joystick_test.c.
add_executable(joystick-test joystick_test.c ../../lib/a2host/joystick.c)
target_link_libraries(joystick-test a2io)
```

In the root `CMakeLists.txt`, after the `tests/a2io` line:

```cmake
add_subdirectory(tests/a2host)
```

- [ ] **Step 6: Run tests to verify they pass**

```bash
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
ninja -C cmake-build-debug && ./cmake-build-debug/tests/a2host/joystick-test
```

Expected: PASS, `joystick: all checks passed`.

- [ ] **Step 7: Hook it into the regression harness**

In `tests/run-tests.sh`, immediately after the `a2io-test` block:

```bash
# The virtual joystick's policy -- rails, centring, and the pushbutton OR that
# two host buttons on one PB depend on. See tests/a2host/joystick_test.c.
if ! "$bin/tests/a2host/joystick-test" > joystick-out.txt; then
  echo "FAIL: virtual joystick policy" >&2
  cat joystick-out.txt >&2
  exit 1
fi
rm joystick-out.txt
```

- [ ] **Step 8: Commit**

```bash
git add lib/a2host/joystick.c lib/a2host/joystick.h lib/a2host/CMakeLists.txt \
        include/apple2tc/a2host_api.h tests/a2host CMakeLists.txt tests/run-tests.sh
git commit -m "a2host: add the virtual joystick policy"
```

---

### Task 4: Wire the policy into `a2host`

**Files:**
- Modify: `include/apple2tc/a2host_api.h` (the three front-end entry points)
- Modify: `lib/a2host/a2host.c` (the instance, options, tick, reboot)

**Interfaces:**
- Consumes: `a2host_joy_*` from Task 3; `a2_io_set_paddle`, `a2_io_set_button`, `a2_io_tick` from Tasks 1–2.
- Produces: `a2host_joy_set_dir`, `a2host_joy_set_button`, `a2host_joy_release_all`, `bool a2host_joystick_at_start(void)`; the options `--joystick`, `--joy-buttons=`, `--paddle=`.

- [ ] **Step 1: Declare the front-end API**

In `include/apple2tc/a2host_api.h`, after the two enums from Task 3:

```c
/// Press or release a direction key on the virtual joystick. PADDL0 is
/// derived from right/left and PADDL1 from down/up; only those two axes are
/// reachable from the keyboard.
void a2host_joy_set_dir(a2host_joydir_t dir, bool down);

/// Press or release a host button. Which pushbutton it drives is the
/// --joy-buttons= mapping, and two host buttons may share one.
void a2host_joy_set_button(a2host_joybtn_t btn, bool down);

/// Release every direction key and every button, and push the resulting
/// centred, unpressed state to the machine.
///
/// Not a convenience. It is the fix for the front end's own stuck-stick
/// cases -- focus lost mid-hold, joystick mode switched off mid-hold -- and
/// in particular for the fact that a key released outside the window never
/// delivers a KEY_UP at all. (A reboot needs the same reset but cannot call
/// this wrapper, since the device is already torn down by the time
/// a2host_reboot() gets there -- see Step 4, which calls a2host_joy_reset()
/// on the policy state directly instead.)
void a2host_joy_release_all(void);

/// Whether --joystick asked the front end to start in joystick mode. Only a
/// front end with a keyboard acts on it; a2run accepts and ignores the
/// option rather than rejecting one its GUI twin understands. a2mcp never
/// reaches this parser at all -- mcp_main.cpp filters argv against its own
/// hardcoded whitelist first and rejects --joystick there, the same
/// pre-existing treatment it gives --fast and --hgr-mode=.
bool a2host_joystick_at_start(void);
```

- [ ] **Step 2: Hold the instance and push to the device**

In `lib/a2host/a2host.c`, with the other file-scope statics:

```c
#include "joystick.h"

/// The virtual joystick. One per process, like everything else here.
static a2host_joy_t joy_;
/// Whether --joystick was given.
static bool joystickAtStart_ = false;
```

And the push, plus the three entry points:

```c
/// Push the policy's current opinion to the device. Called after every change
/// rather than polled, so the machine sees a keypress at the cycle it landed.
static void joy_push(void) {
  for (unsigned axis = 0; axis < 2; ++axis)
    a2_io_set_paddle(&io_, axis, a2host_joy_paddle(&joy_, axis));
  uint8_t mask = a2host_joy_pbmask(&joy_);
  for (unsigned n = 0; n < 3; ++n)
    a2_io_set_button(&io_, n, (mask >> n) & 1);
}

void a2host_joy_set_dir(a2host_joydir_t dir, bool down) {
  a2host_joy_dir(&joy_, dir, down);
  joy_push();
}

void a2host_joy_set_button(a2host_joybtn_t btn, bool down) {
  a2host_joy_button(&joy_, btn, down);
  joy_push();
}

void a2host_joy_release_all(void) {
  a2host_joy_reset(&joy_);
  joy_push();
}

bool a2host_joystick_at_start(void) {
  return joystickAtStart_;
}
```

Initialise it in `a2host_parse_args()`, before the argument loop, so `--joy-buttons=` has defaults to override:

```c
  a2host_joy_init(&joy_);
```

- [ ] **Step 3: Add the three options**

In `a2host_parse_args()`'s loop, next to the other option handlers:

```c
    if (strcmp(arg, "--joystick") == 0) {
      joystickAtStart_ = true;
      continue;
    }
    if (strncmp(arg, "--joy-buttons=", 14) == 0) {
      if (!a2host_joy_parse_buttons(&joy_, arg + 14)) {
        fprintf(
            stderr,
            "--joy-buttons: expected a list like \"space:1,alt:0\" over "
            "space/alt/ctrl and pushbuttons 0-2, got \"%s\"\n",
            arg + 14);
        exit(1);
      }
      continue;
    }
    if (strncmp(arg, "--paddle=", 9) == 0) {
      // <n>:<value>. A starting position only: anything that writes that
      // paddle afterwards overrides it.
      char *end = NULL;
      unsigned long n = strtoul(arg + 9, &end, 10);
      if (end == arg + 9 || *end != ':' || n > 3) {
        fprintf(stderr, "--paddle: expected <0-3>:<0-255>, got \"%s\"\n", arg + 9);
        exit(1);
      }
      char *vend = NULL;
      unsigned long v = strtoul(end + 1, &vend, 10);
      if (vend == end + 1 || *vend != 0 || v > 255) {
        fprintf(stderr, "--paddle: expected <0-3>:<0-255>, got \"%s\"\n", arg + 9);
        exit(1);
      }
      pendingPaddle_[n] = (int)v;
      continue;
    }
```

With, among the statics:

```c
/// --paddle= positions, applied once the machine exists. -1 is "not given".
static int pendingPaddle_[4] = {-1, -1, -1, -1};
```

Applied at the end of `a2host_init_emulation()`, after `a2_io_init(&io_)`:

```c
  for (unsigned n = 0; n < 4; ++n)
    if (pendingPaddle_[n] >= 0)
      a2_io_set_paddle(&io_, n, (uint8_t)pendingPaddle_[n]);
```

Add all three to `print_help()`, alongside the existing options:

```c
  printf(" --joystick       Start in joystick mode (windowed front end only)\n");
  printf(" --joy-buttons=M  Which pushbutton each host button drives, e.g.\n");
  printf("                  space:1,alt:0 -- Neptune's fire is PB1, not PB0\n");
  printf(" --paddle=N:V     Start paddle N (0-3) at position V (0-255)\n");
```

- [ ] **Step 4: Tick the clock and reset on reboot**

In `a2host_simulate_frame()`, immediately after `run_emulated(runCycles);`:

```c
    // Keep a2io's extended clock honest even across a frame in which the
    // program touched no soft switch: its 32-bit source wraps every ~71
    // minutes, and a missed wrap would revive an expired paddle one-shot.
    a2_io_tick(&io_, get_cycles());
```

In `a2host_reboot()`, before `a2host_init_emulation();`:

```c
  // The machine is about to be rebuilt with a centred stick, so the policy's
  // idea of what is held has to go with it -- otherwise a key held across the
  // reboot leaves the two disagreeing until it is next pressed.
  a2host_joy_reset(&joy_);
```

- [ ] **Step 5: Verify it builds and nothing regressed**

```bash
ninja -C cmake-build-debug
cd tests && ./run-tests.sh ../cmake-build-debug; cd ..
```

Expected: build clean, all existing tests pass.

- [ ] **Step 6: Check the options are real**

```bash
./cmake-build-debug/tools/a2run/a2run --help | grep -E "joystick|joy-buttons|paddle"
./cmake-build-debug/tools/a2run/a2run --paddle=9:1 2>&1 | head -1
./cmake-build-debug/tools/a2run/a2run --joy-buttons=space:9 2>&1 | head -1
```

Expected: three help lines; both bad options rejected with the message above.

- [ ] **Step 7: Commit**

```bash
git add include/apple2tc/a2host_api.h lib/a2host/a2host.c
git commit -m "a2host: drive the game port from the virtual joystick"
```

---

### Task 5: The GUI bindings

**Files:**
- Modify: `lib/a2host/a2host_gui.c`

**Interfaces:**
- Consumes: `a2host_joy_set_dir`, `a2host_joy_set_button`, `a2host_joy_release_all`, `a2host_joystick_at_start` from Task 4.
- Produces: nothing other tasks depend on.

- [ ] **Step 1: Rename the status reporter**

`report_display_state()` will report more than display options. Rename it and its four call sites to `report_status()`, and update its doc comment's first line to "The window title is the only status display this front end has, so every key that changes an option re-derives it and calls this."

- [ ] **Step 2: Add the mode and put it in the title**

With the other file-scope statics in `a2host_gui.c`:

```c
/// Whether the arrow keys drive the virtual joystick rather than sending the
/// Apple II's own arrow characters. Off unless --joystick asked otherwise, so
/// nothing regresses for anyone not looking for it.
static bool joyMode_ = false;
```

In `report_status()`, extend both `snprintf` calls with the mode, e.g.:

```c
    snprintf(
        title,
        sizeof(title),
        "%s -- HGR:%s Persist:on(%.2f) Joy:%s",
        a2host_argv0(),
        hgr_mode_name(hgrMode_),
        (double)persistDecay_,
        joyMode_ ? "on" : "off");
```

and the matching `Persist:off Joy:%s` form in the other branch.

In `init_cb()`, adopt the option:

```c
  joyMode_ = a2host_joystick_at_start();
```

- [ ] **Step 3: Write the joystick key handler**

Add above `event_cb()`:

```c
/// Map a sokol keycode to a virtual-joystick direction. Returns false if this
/// key is not one.
static bool joy_dir_of_key(sapp_keycode code, a2host_joydir_t *out) {
  switch (code) {
  case SAPP_KEYCODE_LEFT:
    *out = A2HOST_JOYDIR_LEFT;
    return true;
  case SAPP_KEYCODE_RIGHT:
    *out = A2HOST_JOYDIR_RIGHT;
    return true;
  case SAPP_KEYCODE_UP:
    *out = A2HOST_JOYDIR_UP;
    return true;
  case SAPP_KEYCODE_DOWN:
    *out = A2HOST_JOYDIR_DOWN;
    return true;
  default:
    return false;
  }
}

/// ...and to a host button.
static bool joy_btn_of_key(sapp_keycode code, a2host_joybtn_t *out) {
  switch (code) {
  case SAPP_KEYCODE_SPACE:
    *out = A2HOST_JOYBTN_SPACE;
    return true;
  case SAPP_KEYCODE_LEFT_ALT:
    *out = A2HOST_JOYBTN_ALT;
    return true;
  case SAPP_KEYCODE_LEFT_CONTROL:
    *out = A2HOST_JOYBTN_CTRL;
    return true;
  default:
    return false;
  }
}

/// F7, the joystick keys, and the events that must release a held stick.
///
/// Unlike handle_display_key() this runs *after* event_cb()'s replay guard.
/// That function may run ahead of the guard because F3-F6 never reach the
/// machine; F7 and the stick do, so a cycle-stamped replay must not see them.
///
/// Returns whether the event was consumed.
static bool handle_joystick_key(const sapp_event *ev) {
  // A key released outside the window delivers no KEY_UP, so anything that
  // takes focus away has to release the stick or it stays railed.
  if (ev->type == SAPP_EVENTTYPE_UNFOCUSED || ev->type == SAPP_EVENTTYPE_ICONIFIED ||
      ev->type == SAPP_EVENTTYPE_SUSPENDED) {
    a2host_joy_release_all();
    return false; // not ours to consume; other handlers may want it
  }

  if (ev->type == SAPP_EVENTTYPE_KEY_DOWN && ev->key_code == SAPP_KEYCODE_F7) {
    joyMode_ = !joyMode_;
    // Leaving the mode with a key held would strand the stick at a rail.
    if (!joyMode_)
      a2host_joy_release_all();
    report_status();
    return true;
  }

  if (!joyMode_)
    return false;

  // Space produces a CHAR event as well as a KEY_DOWN, and letting it through
  // would put a keystroke the player never intended into $C000. At Neptune's
  // title screen that risks selecting the keyboard scheme -- the buttons are
  // tested first, but the latch outlives the button, so a tap landing between
  // two polls leaves the stale key as the only evidence. In joystick mode the
  // strobe is never cleared either, because the joystick forms Neptune
  // installs at $1300 and $138C branch past the STA $C010 the keyboard forms
  // execute.
  //
  // Suppressed as a condition on the mode rather than through ignoreNextCh_:
  // that flag is cleared at the top of every event, and the KEY_UP handling
  // below -- plus sokol's own synthetic KEY_UP for Super-modified keys on
  // macOS -- can land between the KEY_DOWN and the CHAR.
  if (ev->type == SAPP_EVENTTYPE_CHAR)
    return ev->char_code == ' ';

  if (ev->type == SAPP_EVENTTYPE_KEY_DOWN || ev->type == SAPP_EVENTTYPE_KEY_UP) {
    bool down = ev->type == SAPP_EVENTTYPE_KEY_DOWN;
    a2host_joydir_t dir;
    if (joy_dir_of_key(ev->key_code, &dir)) {
      a2host_joy_set_dir(dir, down);
      return true;
    }
    a2host_joybtn_t btn;
    if (joy_btn_of_key(ev->key_code, &btn)) {
      a2host_joy_set_button(btn, down);
      return true;
    }
  }

  return false;
}
```

- [ ] **Step 4: Call it from `event_cb()`**

Immediately after the replay guard:

```c
  // A cycle-stamped recording is driving input; real keystrokes must not
  // perturb it.
  if (a2host_key_replay_active())
    return;

  // F7 and the stick. After the guard, unlike handle_display_key() -- see
  // that function's comment, and this one's.
  if (handle_joystick_key(ev))
    return;
```

- [ ] **Step 5: Build and drive it by hand**

```bash
ninja -C cmake-build-debug
./cmake-build-debug/tools/a2emu/a2emu --disk1=dsk/Neptune.dsk --joystick \
    --joy-buttons=space:1,alt:0
```

Expected, and this is the acceptance criterion for the whole feature: the title bar reads `Joy:on`; pressing Space at the title screen starts the game via the joystick path; holding an arrow moves the ship for as long as it is held and stops when released, with **no second press needed to reverse**. F7 returns the arrows to their Apple II characters. Alt-tabbing mid-hold does not leave the ship moving.

- [ ] **Step 6: Commit**

```bash
git add lib/a2host/a2host_gui.c
git commit -m "a2emu: drive the virtual joystick from the arrow keys"
```

---

### Task 6: The `PREAD` calibration acceptance test

The ROM decides `A2_PADDLE_OFFSET`, not the model that produced it. `include/apple2tc/apple2plus_rom.h` bundles the real Apple II+ ROM, so `$FB1E` is genuine `PREAD`.

**Files:**
- Create: `tests/gameport/pread.s`
- Create: `tests/gameport/pread-acceptance.sh`
- Modify: `tests/run-tests.sh`

**Interfaces:**
- Consumes: `--paddle=` from Task 4; the device from Task 2.
- Produces: nothing other tasks depend on.

- [ ] **Step 1: Write the fixture**

Create `tests/gameport/pread.s`:

```
; Call the ROM's PREAD once per paddle and store the four counts.
;
; PREAD takes the paddle index in X, returns the count in Y, and leaves X
; alone -- it reads $C064,X -- so X doubles as the store index.
;
; One `org`, and the capture address is counted rather than forced with a
; second one: a forward org leaves a gap in the emitted image, and the
; acceptance script would then depend on how a6502 fills it. The bytes are
;
;   $300 ldx #0      2      $306 sta $1F00,x  3      $30C bne loop  2
;   $302 jsr $FB1E   3      $309 inx          1      $30E again     3
;   $305 tya         1      $30A cpx #4       2
;
; so `again` is at $30E, which is what --snapshot-at= names. The snapshot
; fires on the FIRST time the address is reached (probe.c's s_snap_fired), so
; the capture point must be one the program reaches only after all four
; stores -- which is why it is the halt and not the entry.

        org     $300

        ldx     #0
loop:   jsr     $FB1E
        tya
        sta     $1F00,x
        inx
        cpx     #4
        bne     loop
again:  jmp     again
```

- [ ] **Step 2: Write the acceptance script**

Create `tests/gameport/pread-acceptance.sh`:

```bash
#!/bin/bash
# The game port's calibration, against the real ROM.
#
# A2_PADDLE_OFFSET is not asserted against the arithmetic that produced it.
# This assembles a program that calls the ROM's PREAD for each paddle, runs it
# with four known positions, and checks PREAD returns exactly those positions.
#
# PREAD samples at trigger + 10 and then every 11 cycles, so any offset in
# 0-10 passes this and 11 already fails at position 0. The test therefore
# pins the interval: the chosen constant must pass, and stepping past its
# upper end must fail. See the design doc for why the midpoint is chosen.
set -e

bin="${1:?usage: pread-acceptance.sh <build-dir>}"
a6502="$bin/tools/a6502/a6502"
a2run="$bin/tools/a2run/a2run"
here="$(dirname "$0")"

tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

"$a6502" "$here/pread.s" "$tmp/pread.b33"

# Four positions in one run: the rails, one either side of centre.
"$a2run" "$tmp/pread.b33" --headless --frames=200 --no-sound \
    --paddle=0:0 --paddle=1:1 --paddle=2:127 --paddle=3:255 \
    --snapshot-at=30E --snapshot-out="$tmp/out.ram" > /dev/null

got="$(od -An -tu1 -j 0x1F00 -N 4 "$tmp/out.ram" | tr -s ' ' | sed 's/^ //')"
want="0 1 127 255"
if [ "$got" != "$want" ]; then
  echo "FAIL: PREAD returned [$got], expected [$want]" >&2
  exit 1
fi

# The other end of the range, in a second run so 128 and 254 are covered too.
"$a2run" "$tmp/pread.b33" --headless --frames=200 --no-sound \
    --paddle=0:128 --paddle=1:254 --paddle=2:64 --paddle=3:192 \
    --snapshot-at=30E --snapshot-out="$tmp/out2.ram" > /dev/null

got2="$(od -An -tu1 -j 0x1F00 -N 4 "$tmp/out2.ram" | tr -s ' ' | sed 's/^ //')"
want2="128 254 64 192"
if [ "$got2" != "$want2" ]; then
  echo "FAIL: PREAD returned [$got2], expected [$want2]" >&2
  exit 1
fi

echo "ok  : the ROM's PREAD returns every position the host set"
```

Make it executable:

```bash
chmod +x tests/gameport/pread-acceptance.sh
```

- [ ] **Step 3: Run it**

```bash
./tests/gameport/pread-acceptance.sh cmake-build-debug
```

Expected: `ok  : the ROM's PREAD returns every position the host set`.

If it fails, the reported counts say which way `A2_PADDLE_OFFSET` is wrong: counts one high mean the offset is too large. Find the passing interval by bisection, then set the constant to its midpoint and update the comment in `a2io.h` with the interval you measured.

- [ ] **Step 4: Hook it into the harness**

In `tests/run-tests.sh`, after the `joystick-test` block:

```bash
# The game port's timing, against the real ROM's PREAD rather than against
# the arithmetic that produced the constant. See tests/gameport/.
if ! ./gameport/pread-acceptance.sh "$bin"; then
  echo "FAIL: game port PREAD calibration" >&2
  exit 1
fi
```

- [ ] **Step 5: Run the whole suite**

```bash
cd tests && ./run-tests.sh ../cmake-build-debug; cd ..
```

Expected: everything passes.

- [ ] **Step 6: Commit**

```bash
git add tests/gameport tests/run-tests.sh
git commit -m "tests: calibrate the paddle one-shot against the ROM's PREAD"
```

---

### Task 7: Mutation, regression sweep and docs

A green test is a claim about the test until something breaks it. This task proves each new test rejects the specific thing it was written to reject, confirms the decoded programs behave as the design predicts, and records the Neptune invocation.

**Files:**
- Modify: `docs/neptune.md`
- Modify: `dz/issues/01a074d9-*.md` (via `dz comment`)

**Interfaces:**
- Consumes: everything above.
- Produces: nothing.

- [ ] **Step 1: Mutate the timing constant**

Change `A2_PADDLE_CYCLES_PER_COUNT` from 11 to 10, rebuild, and run:

```bash
ninja -C cmake-build-debug && ./tests/gameport/pread-acceptance.sh cmake-build-debug
```

Expected: **FAIL**. Revert.

- [ ] **Step 2: Mutate the init centring**

Delete the centring loop in `a2_io_init()`, rebuild, run `a2io-test`.

Expected: **FAIL** on `test_paddle_centred_at_init`. Revert.

- [ ] **Step 3: Mutate the clock back to an absolute 32-bit deadline**

Change `a2_gameport_t::expiry` to `unsigned`, store `(unsigned)io->clock + ...` at the trigger, and compare with `(int32_t)((unsigned)io->clock - io->gameport.expiry[n]) < 0`. Rebuild, run `a2io-test`.

Expected: **FAIL** on `test_paddle_never_triggered`. Revert.

- [ ] **Step 4: Mutate it to the 32-bit elapsed form**

Store a 32-bit `trigger` and `duration[4]`, and compare `(unsigned)((unsigned)io->clock - trigger) < duration[n]`. Rebuild, run `a2io-test`.

Expected: `test_paddle_never_triggered` **passes**, and `test_paddle_stays_expired` **fails** on its full-wrap check. This pair is the point: each test rejects its own formulation rather than passing by accident. Revert.

- [ ] **Step 5: Sweep the decoded programs**

Robotron and Bolo both read paddles, so both change. Build them before and after and compare:

```bash
./decoded/rom/probe-acceptance.sh cmake-build-debug
./decoded/snake-byte/scripts/probe-acceptance.sh cmake-build-debug
```

Expected: both pass — they read only buttons, which stay unpressed.

For Bolo and Robotron, confirm the change is the predicted one and nothing else. Robotron's decompiled `PREAD` charges 15 cycles per iteration against the hardware's 11 (dz `01a07f30-8dce`), so a centred paddle reads about **93** there, not 127. Record what you observe. An unexplained diff anywhere else is a failure.

- [ ] **Step 6: Confirm the front ends still agree**

```bash
cd tests && ./run-tests.sh ../cmake-build-debug; cd ..
```

Expected: pass, including the `a2emu --headless` ≡ `a2run` byte-identity check.

- [ ] **Step 7: Document the Neptune invocation**

In `docs/neptune.md`, in the `## Controls` section after the I/J/K/M table:

```markdown
**The joystick path avoids all of this.** Neptune supports a stick as well as
the keyboard and picks whichever you touch first at the title screen
(`$3584`). Where the keyboard toggles a direction, `$36B0` stores an absolute
one every pass, so a held stick means "keep moving" and a centred one means
"stop" — no second press to reverse.

Neptune is wired the unusual way round: `$C062` (PB1) is fire and `$C061`
(PB0) is the secondary weapon, where most games put the trigger on PB0. So it
wants the button override:

```bash
a2emu --disk1=dsk/Neptune.dsk --joystick --joy-buttons=space:1,alt:0
```

Space fires, Left-Alt drops the secondary weapon, the arrow keys are the
stick, and F7 toggles the mode off again.
```

- [ ] **Step 8: Update the issue**

```bash
dz comment 01a074d9 -m "The game port is implemented: three pushbuttons, four paddle one-shots timed at 11 cycles per count against an extended 64-bit clock, and the \$C068-\$C06F mirror. a2emu drives PADDL0/PADDL1 and the buttons from the arrow keys behind an F7 mode toggle. Calibrated against the ROM's PREAD rather than against its own arithmetic; see tests/gameport/. Replay is still out of scope -- joystick input is ignored during a --key-file replay and is not recorded."
```

Leave the issue open or close it as the maintainer prefers; the replay half named in its "Wanted" section is deliberately not delivered here.

- [ ] **Step 9: Commit**

```bash
git add docs/neptune.md dz/issues
git commit -m "docs: record Neptune's joystick invocation"
```

---

## Self-review

**Spec coverage.** Device and decode → Task 2. Extended clock and `a2_io_tick` → Task 1. Init centring → Task 2. Policy, rails, OR aggregation, `release_all` → Task 3. Options and reboot → Task 4. GUI bindings, `KEY_UP`, focus loss, Space suppression, `report_status` rename → Task 5. Calibration and the `OFFSET` interval → Task 6. Mutation, decoded-program sweep, `docs/neptune.md` → Task 7. The spec's two known limitations are filed as dz `01a07f30-8dce` and `01a07f30-c3d9` and are deliberately not implemented.

**Type consistency.** `a2host_joy_dir`/`a2host_joy_button`/`a2host_joy_reset` are the module's names (Task 3); `a2host_joy_set_dir`/`a2host_joy_set_button`/`a2host_joy_release_all` are the front-end wrappers (Task 4). The distinction is deliberate — the first take an `a2host_joy_t *`, the second act on the process-wide instance and push to the device. `a2host_joy_paddle` returns `uint8_t`; `a2host_joy_pbmask` returns a `uint8_t` mask, not a count.

**Known gap, stated rather than hidden.** Task 5 has no automated test. The sokol event translation — F7, `KEY_UP`, focus-loss release, the Space `CHAR` filter — has no harness in this repo, and Step 5 of that task is a manual check against Neptune. Everything under it is covered.
