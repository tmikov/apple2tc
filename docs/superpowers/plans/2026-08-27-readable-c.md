# Readable C Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make `decoded/snake-byte/snake-byte-cold.c` read like C somebody wrote on purpose, so that a person who has never heard of a 6502 can follow it and change it.

**Architecture:** Nine changes in ascending order of risk. The first six are renames, named constants and struct grouping — each provably inert or gate-checked. The last two restructure the two functions that are still shaped like assembly. Nothing here changes what the program does; every task ends green on the same gate.

**Tech Stack:** C11, CMake + Ninja, `probe-acceptance.sh`, `yield-lint.awk`.

## Global Constraints

- **Keep the `s_` prefix** on file-scope statics. It is the house convention and it stays, including on the new struct instances (`s_mon`, `s_snake`).
- **Keep `k` for const tables.** Already consistent across 52 objects.
- Never edit `snake-byte.json`, a `.frames` file, or a `.pkeys` file.
- `snake-byte-cold.c` is hand-owned; `decompile.sh` does not regenerate it.
- Every task ends with `./probe-acceptance.sh ../../cmake-build-debug` at **31 PASS**, run from `decoded/snake-byte`.
- Every task ends with `awk -f yield-lint.awk snake-byte-cold.c` exiting 0.
- Every task ends clean under all three compilers (the `[warn]` gate check does this).
- Do not split the file. `system2-inc.h` defines the machine with internal linkage, so everything touching it must share one translation unit.
- Do not restructure `rom_coutz`'s 15 `goto`s. It transcribes a ROM routine that genuinely is a jump table.

---

### Task 1: A committed way to prove a change is inert

**Files:**
- Create: `decoded/snake-byte/codegen-fingerprint.sh`

**Interfaces:**
- Produces: `codegen-fingerprint.sh [file]` — prints a 16-hex-digit hash of the emitted code. Two runs that print the same hash emitted identical instructions.

Renames and comment edits should not change a single instruction, and the cheapest way to know is to compare compiler output. Two things make a naive comparison lie, and both cost an afternoon when they were first hit.

- [ ] **Step 1: Create the script.**

```bash
cat > decoded/snake-byte/codegen-fingerprint.sh <<'EOF'
#!/bin/bash
#
# Print a hash of the code the compiler emits, for proving that an edit changed
# nothing. Run it before and after; identical output means identical
# instructions.
#
# Three things are filtered out because they move for reasons that are not code
# changes, and each one looks exactly like a real difference:
#
#   -g0        debug info carries line numbers, so any edit moves it
#   -DNDEBUG   assert() bakes __LINE__ into the text section, so deleting a
#              *comment* changes the emitted code
#   .L#        gcc renumbers its internal labels whenever a function is added
#              to or removed from the translation unit
#
set -e
cd "$(dirname "$0")"
cc -I ../../include -std=gnu11 -O2 -g0 -DNDEBUG -S -o - "${1:-snake-byte-cold.c}" |
  grep -v '^[[:space:]]*\.file\|^[[:space:]]*\.ident\|^[[:space:]]*#' |
  sed 's/\.L[A-Z]*[0-9][0-9]*/.L#/g' | sha256sum | cut -c1-16
EOF
chmod +x decoded/snake-byte/codegen-fingerprint.sh
```

- [ ] **Step 2: Verify it is deterministic.**

```bash
cd decoded/snake-byte
a=$(./codegen-fingerprint.sh); b=$(./codegen-fingerprint.sh)
[ "$a" = "$b" ] && echo "deterministic: $a" || echo "FAIL: $a vs $b"
```

Expected: `deterministic: <hash>`.

- [ ] **Step 3: Verify it detects a real change.** Prove the tool works before
      relying on it — a check nobody has seen fail is not a check.

```bash
cd decoded/snake-byte
sed 's/advance(28848)/advance(28849)/' snake-byte-cold.c > /tmp/mut.c
cp /tmp/mut.c ./zz-mut.c
./codegen-fingerprint.sh zz-mut.c   # must differ from step 2
rm -f zz-mut.c
```

Expected: a different hash.

- [ ] **Step 4: Commit.**

```bash
git add decoded/snake-byte/codegen-fingerprint.sh
git commit -m "snake-byte: a script to prove an edit emitted the same code"
```

---

### Task 2: Drop the `_native` suffix

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

**Interfaces:**
- Produces: 31 renamed functions. Later tasks refer to them without the suffix — `game_play_loop`, `game_draw_cell`, `game_read_direction`.

The suffix once separated hand-written C from generated C. There is no generated C, so it is now 140 occurrences of noise on the most-read lines in the file.

Checked in advance: no base name collides with an existing identifier. The 17
apparent collisions are all prose inside comments that already call the routine
by its short name, so the rename fixes those too.

- [ ] **Step 1: Record the baseline fingerprint.**

```bash
cd decoded/snake-byte && ./codegen-fingerprint.sh | tee /tmp/fp-before.txt
```

- [ ] **Step 2: Rename.** Word-boundary anchored so `game_native` (prose about
      a deleted file) is not caught by the `game_` cases.

```bash
cd decoded/snake-byte
for n in $(grep -oE '\b\w+_native\b' snake-byte-cold.c | sort -u | grep -v '^game_native$'); do
  sed -i "s/\b${n}\b/${n%_native}/g" snake-byte-cold.c
done
grep -c '_native' snake-byte-cold.c   # expect only the game_native.c prose
```

- [ ] **Step 3: Remove the remaining `game_native` prose.** Seven comments
      refer to `game_native.c`, a file that no longer exists. Read each and
      delete the sentence or rewrite it to describe the code.

```bash
grep -n 'game_native' snake-byte-cold.c
```

- [ ] **Step 4: Verify nothing was emitted differently.**

The fingerprint hashes assembly *text*, which contains symbol names — so a
rename necessarily changes it. (The plan originally claimed otherwise; it was
wrong.) For a rename the check is: identical after normalising the old names to
the new ones.

```bash
cd decoded/snake-byte
gen() { cc -I ../../include -std=gnu11 -O2 -g0 -DNDEBUG -S -o - "$1" |
        grep -v '\.file\|\.ident\|^\s*#' | sed 's/\.L[A-Z]*[0-9][0-9]*/.L#/g'; }
git show HEAD:decoded/snake-byte/snake-byte-cold.c > zz-pre.c   # in-directory:
gen zz-pre.c > /tmp/pre.s                                        # the relative
gen snake-byte-cold.c > /tmp/post.s                              # #includes must
rm -f zz-pre.c                                                   # resolve
sed -E 's/\b([A-Za-z_][A-Za-z0-9_]*)_native\b/\1/g' /tmp/pre.s > /tmp/pre-renamed.s
diff /tmp/pre-renamed.s /tmp/post.s | grep -c '^[<>]'
```

Expected: `0`. Anything else is an edit that was not a rename.

Note the `zz-pre.c` dance: the old file must be compiled **in its own
directory** or its relative `#include "game-image.inc"` fails and the compiler
aborts before emitting anything — which looks exactly like "no differences".

- [ ] **Step 5: Build and gate.**

```bash
ninja -C ../../cmake-build-debug snake-byte-cold-run
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS
```

Expected: `31`.

- [ ] **Step 6: Commit.**

```bash
git add -A && git commit -m "snake-byte: drop the _native suffix, which no longer distinguishes anything"
```

---

### Task 3: Name the inks, the shapes and the cell contents

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

**Interfaces:**
- Produces: `enum Ink`, `enum Shape`, `enum CellContent`, used at every call
  site that currently passes a bare hex literal.

This is the change that decides whether somebody can tinker. Today the snake's
colour is `0x0c` in four places and nothing says so. The domains are small and
closed — nine ink values and four shapes across the whole program.

Names are by **role, not by colour**. A reader wants "the snake's ink", not
"green"; and the same number means a lo-res colour in one call and a hi-res
shape ink in another, so a colour name would be wrong half the time.

Leave the hex inside the data tables (`kHgrPattern`, the font, the level
scripts) exactly as it is. Those are bit patterns, not quantities, and hex is
the right way to write them.

- [ ] **Step 1: Add the enums**, next to the `Cell` typedef.

```c
/// Lo-res colour, and also the ink index the hi-res cell drawers take. The
/// game uses one numbering for both. Named for what each is used to draw,
/// because that is the question somebody changing them will have.
typedef enum {
  INK_ERASE = 0x00,      ///< black, i.e. rub out whatever is there
  INK_WALL_TOP = 0x02,   ///< the side walls above the seam
  INK_GATE = 0x03,       ///< the gap the snake leaves through, and the bouncers
  INK_OCCUPIED = 0x05,   ///< the occupancy map's "something is here"
  INK_HEAD_MARK = 0x07,  ///< the head's own mark on the occupancy map
  INK_APPLE = 0x09,
  INK_SNAKE = 0x0c,
  INK_WALL_BOTTOM = 0x0d, ///< the side walls below the seam
  INK_WHITE = 0x0f,
} Ink;

/// Which of the shape table's entries a cell drawer stamps. Shapes are stored
/// four scanline masks apiece; see game_load_shape_masks.
typedef enum {
  SHAPE_APPLE = 0x01,   ///< also merged over the head so it reads as a head
  SHAPE_STEM = 0x0a,    ///< the arrow's stem on the redefinition screen
  SHAPE_SOLID = 0x15,   ///< the default: a full cell, used for walls and text
  SHAPE_BOUNCER = 0x1a,
} Shape;

/// What the lo-res occupancy map holds at a cell. Anything else is solid.
typedef enum {
  CELL_EMPTY = 0x00,
  CELL_APPLE = 0x0f,
} CellContent;
```

- [ ] **Step 2: Replace the ink literals at call sites.** These are the exact
      sites; each was read to confirm its role.

| line (approx) | current | becomes |
| --- | --- | --- |
| bouncer erase | `rom_setcol(0x00)` / `plot_at(0x00, ...)` | `INK_ERASE` |
| bouncer draw | `rom_setcol(0x03)` / `plot_shape_at(0x1a, 0x03, ...)` | `SHAPE_BOUNCER, INK_GATE` |
| `wipe_occupancy_map` | `set_ink(0x00)` / `lores_hline(at, 0x00)` | `INK_ERASE` |
| `game_bonus_screen` | the box's four ink arguments | read each; they are inks |
| the gate | `plot_hline_at(0x03, 0x12, 0x27, 0x16)` | `INK_GATE` |
| `game_set_ink` | `rom_setcol(ink ? 0x05 : 0x00)` | `INK_OCCUPIED : INK_ERASE` |
| apple | `rom_setcol(0x0f)` / `plot_shape_at(0x01, 0x09, at)` | `INK_WHITE`, `SHAPE_APPLE, INK_APPLE` |
| side walls | `plot_vline_at(0x02, ...)` / `plot_vline_at(0x0d, ...)` | `INK_WALL_TOP` / `INK_WALL_BOTTOM` |
| the head | `game_draw_head(0x0c, head)` | `INK_SNAKE` |
| — | — | **not** `draw_border`: its `0x00`s are coordinates, and it already takes ink as a parameter |
| the body | `plot_shape_at(dir, 0x0c, next)` | `INK_SNAKE` |
| head mark | `rom_setcol(0x07)` (two sites) | `INK_HEAD_MARK` |
| tail erase | `rom_setcol(0x00)` / `plot_at(0x00, tail)` | `INK_ERASE` |

- [ ] **Step 3: Replace the shape assignments.** `s_shape = 0x15` (four
      sites) becomes `SHAPE_SOLID`; `0x01` becomes `SHAPE_APPLE`; `0x0a`
      becomes `SHAPE_STEM`; `0x1a` becomes `SHAPE_BOUNCER`.

- [ ] **Step 4: Replace the cell-content tests.** `cell == 0x0f` and the
      local `kApple = 0x0f` become `CELL_APPLE`; `cell == 0x00` and
      `cell != 0x00` become `CELL_EMPTY`. There are 20 raw `0x0f` — check each,
      because some are a nibble mask (`& 0x0f`) and must not be touched.

- [ ] **Step 5: Verify nothing was emitted differently.** A named constant with
      the same value compiles to the same instruction, so this task is inert
      too.

```bash
cd decoded/snake-byte && ./codegen-fingerprint.sh
```

Expected: unchanged from Task 2's final hash.

- [ ] **Step 6: Gate and commit.**

```bash
ninja -C ../../cmake-build-debug snake-byte-cold-run
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS   # 31
git add -A && git commit -m "snake-byte: name the inks, shapes and cell contents"
```

---

### Task 4: The shape arithmetic is a two-dimensional table

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

**Interfaces:**
- Produces: `kSnakeShape[turn][dir]`, replacing three additions in
  `game_play_loop`.

Three call sites compute a shape by adding a constant to a direction:
`dir + 0x10` when the player turns clockwise, `dir + 0x04` anticlockwise,
`dir + 0x08` going straight. That is a table indexed by turn and direction,
written as arithmetic, and the two-dimensionality is what the arithmetic hides.

- [ ] **Step 1: Add the table** next to `kRowDelta` / `kColDelta`.

```c
/// Which shape the head is drawn with, by how it is turning and which way it
/// ends up facing. The original computes these by adding $10, $04 or $08 to
/// the direction, which is the same table with the rows implied.
typedef enum { TURN_CW, TURN_CCW, TURN_STRAIGHT } Turn;

static const uint8_t kSnakeShape[3][5] = {
    [TURN_CW] = {0, 0x11, 0x12, 0x13, 0x14},
    [TURN_CCW] = {0, 0x05, 0x06, 0x07, 0x08},
    [TURN_STRAIGHT] = {0, 0x09, 0x0a, 0x0b, 0x0c},
};
```

Index 0 is unused: directions are 1..4 (`DIR_RIGHT`..`DIR_DOWN`).

- [ ] **Step 2: Use it at the three sites** in `game_play_loop`.

```c
shape = kSnakeShape[TURN_CW][dir];        // was (uint8_t)(dir + 0x10)
shape = kSnakeShape[TURN_CCW][dir];       // was (uint8_t)(dir + 0x04)
shape = kSnakeShape[TURN_STRAIGHT][dir];  // was (uint8_t)(dir + 0x08)
```

- [ ] **Step 3: Gate.** This one *does* change codegen — a table lookup is not
      an addition — so the fingerprint will differ and that is expected. The
      gate is the check.

```bash
ninja -C ../../cmake-build-debug snake-byte-cold-run
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS
```

Expected: `31`. If the screen probes fail, the table is wrong: the entries must
reproduce `dir + constant` exactly for `dir` in 1..4.

- [ ] **Step 4: Commit.**

```bash
git add -A && git commit -m "snake-byte: the head's shape is a table of turn against direction"
```

---

### Task 5: The monitor's state becomes a struct

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

**Interfaces:**
- Produces: `static struct Monitor s_mon;` with fields `wndlft wndwdth wndtop
  wndbtm bas bas2 gbas v2 mask color invflg csw ksw a2l h2 ch cv`. All later
  references are `s_mon.ch`, `s_mon.wndtop`, and so on.

Seventeen variables that the file already groups with a section banner reading
"these are not the game's variables: they belong to the ROM routines it calls".
That sentence should be a type. Doing so also separates the game's state from
the emulator's `s_ram` / `s_cycles`, which today wear the same `s_` badge and
are the one thing a reader most needs told apart.

- [ ] **Step 1: Replace the seventeen declarations with one struct**, keeping
      every existing per-field comment as a field comment.

```c
/// The Apple II monitor's own zero page. Not the game's variables: they belong
/// to the ROM routines it calls, and the game writes them only to pass
/// arguments -- a cursor position before COUT, a colour before PLOT, a
/// right-hand end before HLINE. The names are the Apple II's own, because
/// every reference to this hardware uses them.
static struct {
  uint8_t wndlft, wndwdth, wndtop, wndbtm; ///< the text window
  uint16_t gbas;    ///< lo-res line base, from GBASCALC
  uint16_t bas;     ///< text line base, from BASCALC
  uint16_t bas2;    ///< the scroll's destination line
  uint8_t v2;       ///< VLINE's bottom row
  uint8_t mask;     ///< which nibble of a lo-res byte a PLOT touches
  uint8_t color;    ///< the lo-res colour, both nibbles
  uint8_t invflg;   ///< COUT1 ANDs the character with this
  uint16_t csw;     ///< the character output vector the game repoints
  uint16_t ksw;     ///< the character input vector; nothing reads it
  uint8_t a2l;      ///< SETKBD/SETVID scratch
  uint8_t h2;       ///< HLINE's right-hand end; see the note on its second job
  uint8_t ch, cv;   ///< the text cursor
} s_mon;
```

- [ ] **Step 2: Rewrite the references.** Mechanical, and a missed one is a
      compile error rather than a silent change.

```bash
cd decoded/snake-byte
for f in wndlft wndwdth wndtop wndbtm gbas bas2 bas v2 mask color invflg csw ksw a2l h2 ch cv; do
  sed -i "s/\bs_${f}\b/s_mon.${f}/g" snake-byte-cold.c
done
```

Order matters: `bas2` is substituted before `bas`, or `s_bas2` becomes
`s_mon.bas2` twice over.

- [ ] **Step 3: Fix the initialiser.** `init_emulated` sets several of these
      from the entry snapshot; check every assignment still compiles and reads
      naturally.

- [ ] **Step 4: Build, gate.**

```bash
ninja -C ../../cmake-build-debug snake-byte-cold-run
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS   # 31
```

- [ ] **Step 5: Commit.**

```bash
git add -A && git commit -m "snake-byte: the monitor's zero page is a struct, not seventeen globals"
```

---

### Task 6: The game's own state becomes four structs

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

**Interfaces:**
- Produces: `s_snake`, `s_progress`, `s_sound`, `s_input`.

Forty-nine globals left after Task 5, and no function signature says which of
them it touches. Four groups fall out of the names themselves.

- [ ] **Step 1: The snake.** Keep each existing declaration's comment as a
      field comment; they are the most carefully written in the file.

```c
/// Everything belonging to the snake currently on the playfield.
static struct {
  Cell head, tail;
  uint8_t direction;     ///< DIR_RIGHT..DIR_DOWN, 1..4
  uint8_t growth;        ///< cells still to grow before the tail starts moving
  bool head_moved;       ///< head is on a new cell, so merge the head shape
  uint8_t shape;         ///< which shape the cell drawers stamp
  uint8_t shape_mask[4]; ///< the loaded shape's four scanline masks
  uint8_t steer_dir;     ///< the direction snake_move_verdict is asked about
} s_snake;
```

```bash
cd decoded/snake-byte
for f in head_moved head tail direction growth shape_mask shape steer_dir; do
  sed -i "s/\bs_${f}\b/s_snake.${f}/g" snake-byte-cold.c
done
```

`head_moved` before `head`, and `shape_mask` before `shape`, or the shorter
name eats the longer one.

- [ ] **Step 2: Sound.** The `tone_` and `sound_` prefixes are what the struct
      now says, so the fields drop them.

```c
/// The one-bit speaker. `port` is $30 for the speaker and $20 for the cassette
/// output, which is where a muted click goes.
static struct {
  uint8_t period;      ///< the falling tone's current half-period
  uint8_t countdown;   ///< passes remaining before the next click
  uint8_t passes;      ///< passes left in this call of game_tick_sound
  uint8_t click_count; ///< steering clicks still owed
  uint8_t port;
  bool muted;
} s_sound = {.port = 0x20};
```

```bash
sed -i 's/\bs_tone_period\b/s_sound.period/g; s/\bs_tone_countdown\b/s_sound.countdown/g;
        s/\bs_tone_passes\b/s_sound.passes/g; s/\bs_click_count\b/s_sound.click_count/g;
        s/\bs_click_port\b/s_sound.port/g;    s/\bs_sound_muted\b/s_sound.muted/g' snake-byte-cold.c
```

- [ ] **Step 3: Input.**

```c
/// The keyboard ring the poll fills and the play loop drains, and the six
/// bindings the redefinition screen edits.
static struct {
  uint8_t ring[16];    ///< only a byte with bit 7 set counts as present
  uint8_t read, write; ///< indices into ring, masked to its size
  uint8_t bindings[6]; ///< the six actions' keys, in kKeyDefaults order
  bool joystick;       ///< a joystick was chosen at the setup prompt
} s_input;
```

```bash
sed -i 's/\bs_key_ring\b/s_input.ring/g;  s/\bs_key_table\b/s_input.bindings/g;
        s/\bs_ring_read\b/s_input.read/g; s/\bs_ring_write\b/s_input.write/g;
        s/\bs_joystick_selected\b/s_input.joystick/g' snake-byte-cold.c
```

- [ ] **Step 4: Progress.** Everything that outlives a single life.

```c
/// The score, and how far the player has got. The counters are BCD, because
/// the status panel prints them a nibble at a time.
static struct {
  uint8_t score[4];             ///< four BCD bytes, little-endian, eight digits
  uint8_t best[4];              ///< the high score, same shape
  uint8_t level;
  uint8_t lives;
  uint8_t lives_at_level_start; ///< restored if the level is failed
  uint8_t bonus[2];             ///< the round-cleared bonus, BCD
  uint8_t apple_value[2];       ///< what one apple is worth at this level
  uint8_t afield[2];            ///< apples currently on the playfield
  uint8_t eaten[2];
  uint8_t left[2];
  uint8_t quota[2];             ///< apples needed to clear the round
} s_progress;
```

```bash
sed -i 's/\bs_hi_score\b/s_progress.best/g;   s/\bs_score\b/s_progress.score/g;
        s/\bs_lives_at_level_start\b/s_progress.lives_at_level_start/g;
        s/\bs_lives\b/s_progress.lives/g;
        s/\bs_bonus_amount\b/s_progress.bonus/g;
        s/\bs_apple_value\b/s_progress.apple_value/g;
        s/\bs_apples_afield\b/s_progress.afield/g;
        s/\bs_apples_eaten\b/s_progress.eaten/g;
        s/\bs_apples_left\b/s_progress.left/g;
        s/\bs_apples_quota\b/s_progress.quota/g' snake-byte-cold.c
sed -i 's/\bs_level\b/s_progress.level/g' snake-byte-cold.c
```

`s_hi_score` before `s_score`, and `s_lives_at_level_start` before `s_lives`.
`s_level` is done last and on its own line, because `s_level_time` stays a
plain global and a careless `\bs_level\b` in the same pass can reach it:

```bash
grep -n 's_progress.level_time' snake-byte-cold.c   # expect no output
```

- [ ] **Step 5: Leave the rest as they are.** `s_apple`, `s_bouncers`,
      `s_demo_mode`, `s_difficulty`, `s_setup_seen`, `s_step_delay`,
      `s_script_*`, `s_level_time`, `s_life_*`, `s_rand_ptr`, `s_str_ptr`,
      `s_page2`, `s_cursor`, `s_plot`, `s_hline`, `s_vline_at`, `s_mem_00b1`.
      Grouping for its own sake is worse than not grouping; these do not form
      an obvious noun.

- [ ] **Step 6: Build, gate after each struct** — five separate compiles is
      cheaper than one debugging session.

```bash
ninja -C ../../cmake-build-debug snake-byte-cold-run
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS   # 31
```

- [ ] **Step 7: Commit, one per struct.**

```bash
git add -A && git commit -m "snake-byte: group the snake's state into a struct"
```

---

### Task 7: Name the screen writes

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

There are 61 `peek`/`poke` sites into the 64K array. Most should stay: writing
to `$2000` for hi-res is what the machine does, and hiding it would make the
code less honest. Two patterns read badly enough to be worth naming.

- [ ] **Step 1: Add two helpers** next to the cell drawers.

```c
/// A byte of hi-res page 1. The page is $2000-$3FFF, and successive scanlines
/// of a row are $400 apart, which is why walking down a cell is +4 on the high
/// byte and nothing else.
static void hgr_poke(uint16_t addr, uint8_t v) {
  poke(addr, v);
}
static uint8_t hgr_peek(uint16_t addr) {
  return peek(addr);
}
```

- [ ] **Step 2: Use them** in `game_draw_cell`, `game_merge_cell`,
      `game_clear_hgr` and `game_cout_hook` — the four routines that write
      hi-res. (The plan first said three and was wrong; the glyph blitter in
      `game_cout_hook` writes the same page.) In that fourth one wrap only the
      *destination*: the `peek` beside it reads the font out of the game image,
      which is not the screen.

      Leave the text screen writes (`poke(s_mon.bas + s_mon.ch, ch)`) alone —
      `bas` already says what it is — and leave `game_cold_start`'s
      $3800→$1800 copy alone, which reads the hi-res range as level data and
      writes outside the page.

- [ ] **Step 3: Gate.** At `-O2` these inline away, so the fingerprint will
      very likely be unchanged; if it is not, the gate is what matters.

```bash
ninja -C ../../cmake-build-debug snake-byte-cold-run
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS   # 31
```

- [ ] **Step 4: Commit.**

```bash
git add -A && git commit -m "snake-byte: name the hi-res screen accesses"
```

---

### Task 8: Prune the comments that narrate the 6502

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

The file is about 42% comment. The excess is comments that describe the
original's instruction sequence rather than what the C does — "$624E is left
one below range here and normalised at $62B8, which is the order the samples
see".

**The distinction to apply, and it matters more than the volume:**

- A comment that states a **constraint** stays. "Computing the wrap early would
  be tidier and would not match" tells the next person not to make a change
  that looks obviously correct. That is the most valuable kind of comment in
  this file and there are dozens of them.
- A comment that **narrates the original's instructions** goes. The reader
  cannot check it, cannot use it, and it is between them and the code.
- A comment that says a path is **unverified** stays. Several routines are
  transcribed from the binary and never executed by any test; that is a fact
  about the code's trustworthiness and nothing else records it.

- [ ] **Step 1: Work through the file top to bottom**, applying the three rules
      above. Do not batch this with a regex — the categories are not
      syntactically distinguishable.

- [ ] **Step 2: Verify the edit was comment-only.**

```bash
cd decoded/snake-byte && ./codegen-fingerprint.sh
```

Expected: unchanged from Task 7.

- [ ] **Step 3: Gate and commit.**

```bash
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS   # 31
git add -A && git commit -m "snake-byte: comments state constraints, not instruction sequences"
```

---

### Task 9: Extract the draw block from the play loop

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

**Interfaces:**
- Produces: `static LifeEnd snake_step(uint8_t dir, uint8_t shape, uint8_t *cell_out);`

`game_play_loop` is 232 lines with 8 `goto`s and 7 labels. The dispatch half is
an honest state machine and its `goto`s stay — the auto-steer really does
re-enter below the high-bit test, and writing that as anything else would be a
lie. The `draw:` block underneath is about 70 self-contained lines: draw the
head, step it one cell, and decide what it landed on.

- [ ] **Step 1: Extract it**, taking the direction and shape and returning the
      outcome. The three `return LIFE_*` inside it become the function's
      returns; the one path that falls through to `tail:` needs a distinct
      value — add `LIFE_CONTINUE` to `LifeEnd` for it.

- [ ] **Step 2: Call it from the loop.**

```c
draw: {
  uint8_t cell = 0;
  const LifeEnd outcome = snake_step(dir, shape, &cell);
  if (outcome != LIFE_CONTINUE) {
    *cell_out = cell;
    return outcome;
  }
  goto tail;
}
```

- [ ] **Step 3: Gate.**

```bash
ninja -C ../../cmake-build-debug snake-byte-cold-run
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS   # 31
```

- [ ] **Step 4: Play it.** This is the first task that changes control flow, so
      the gate is no longer sufficient on its own.

```bash
ninja -C ../../cmake-build-release snake-byte-cold
../../cmake-build-release/decoded/snake-byte/snake-byte-cold
```

Confirm: the snake moves, steers, eats, and dies with the right sound.

- [ ] **Step 5: Commit.**

```bash
git add -A && git commit -m "snake-byte: one step of the snake is a function"
```

---

### Task 10: Re-nest the top level

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

`game_cold_start` is 218 lines with **21 `goto`s and 13 labels**, and it is the
worst thing in the file. It is a four-level nested state machine — new game →
new level → round → life — flattened into a label list, so the nesting that
gives the game its shape is invisible and has to be reconstructed by tracing
jumps.

**This is the riskiest task in the plan.** Flattened state machines often have
entry edges that do not survive re-nesting: a label reached from two different
depths, or a jump that skips an initialisation. Do it last, and do it slowly.

- [ ] **Step 1: Map the jumps before changing anything.** For each of the 13
      labels, list every `goto` that reaches it and the nesting depth it is at.
      Write the map into the commit message; it is the evidence that the
      re-nesting is faithful.

```bash
cd decoded/snake-byte
awk '/^void game_cold_start/,/^}/' snake-byte-cold.c | grep -nE '^\s*[a-z_]+:|goto '
```

- [ ] **Step 2: Only if the map is a tree, re-nest it** as loops:

```c
for (;;) {            // new game
  ...
  for (;;) {          // new level
    ...
    for (;;) {        // round
      ...
      for (;;) {      // life
```

If any label is reached from two different depths, **stop and report it** —
that edge is a real property of the game's structure and flattening it back out
may be the honest answer.

- [ ] **Step 3: Gate.**

```bash
ninja -C ../../cmake-build-debug snake-byte-cold-run
./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS   # 31
```

- [ ] **Step 4: Play it, for longer than the other tasks.** The gate's
      scenarios are 1,300 frames and cover one game. This function is the only
      thing that runs *between* games, so play through a death, a level change
      and a game over.

- [ ] **Step 5: Commit.**

```bash
git add -A && git commit -m "snake-byte: the top level is four nested loops, not thirteen labels"
```

---

## Verification summary

| task | proof |
| --- | --- |
| 1 tool | deterministic, and detects a one-digit mutation |
| 2 `_native` | assembly identical after normalising the renamed symbols |
| 3 constants | fingerprint identical |
| 4 shape table | gate 31 PASS |
| 5 monitor struct | gate 31 PASS |
| 6 state structs | gate 31 PASS, per struct |
| 7 screen writes | gate 31 PASS |
| 8 comments | fingerprint identical |
| 9 extract step | gate 31 PASS + play |
| 10 re-nest | gate 31 PASS + play through a game over |

## Out of scope

- Splitting the file. `system2-inc.h` forces one translation unit.
- Removing the `s_` prefix. It is the house convention and it stays.
- Restructuring `rom_coutz`.
- Giving the emulator's machine state external linkage, which is what would
  make a file split possible. That is a change to shared infrastructure.
