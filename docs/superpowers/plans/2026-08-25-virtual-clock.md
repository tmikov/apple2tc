# Virtual Clock Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Reduce `snake-byte-cold.c` from 839 `TICK` sites to ~10 `advance()` sites, at the places where a duration is perceptible, with the speaker's toggle timeline preserved byte for byte.

**Architecture:** One virtual clock in 6502 cycles. The coroutine machinery is untouched: `cycles_expired()` still parks the game and hands control to the host, just at rarer, justified points. The audio timeline and the pacing clock are the same clock, because the Apple II speaker's pitch *is* the interval between reads of `$C030`.

**Tech Stack:** C11, CMake+Ninja, the existing probe/gate scripts.

## Global Constraints

- Never edit `snake-byte.json`, a `.frames` file, or `.pkeys` files.
- `snake-byte-cold.c` is hand-owned; `decompile.sh` does not regenerate it.
- Every task ends green on `probe-acceptance.sh` (26 PASS) unless the task says which check it deliberately spends.
- Verify a rebuild happened before trusting a green run: `ninja` reporting "no work to do" after an edit means the edit did not compile in.
- The toggle dump must be byte-identical after every task in this plan.

---

### Task 1: The toggle dump

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`
- Create: `decoded/snake-byte/toggle-play.txt`, `decoded/snake-byte/toggle-hires.txt`
- Modify: `decoded/snake-byte/probe-acceptance.sh`

**Interfaces:**
- Produces: `static void speaker_access(uint8_t port)` — performs the `$C0xx`
  read and, when `A2_TOGGLE_DUMP` names a path, appends `"<cycles> <port>\n"`.

- [ ] **Step 1: Find every speaker access.** There are four, all spelled
      `peek((uint16_t)(0xc000 + <port>))` or `peek(0xc000 + <port>)`:
      in `game_tick_sound_native`, `game_sound_sweep_native` (two), and
      `click_speaker`. Confirm with:
      `grep -n 'peek(.*0xc000' decoded/snake-byte/snake-byte-cold.c`
      Expected: 4 lines.

- [ ] **Step 2: Add the helper**, next to `click_speaker`:

```c
/// Every access to the speaker soft switch goes through here.
///
/// The Apple II speaker is a one-bit cone and reading $C030 flips it, so the
/// waveform is entirely the sequence of these accesses and their timestamps.
/// That makes the sequence an exact oracle for any change to the game's
/// timing: dump it before and after and it must be identical.
///
/// $C020 is the cassette output, which is where a muted click goes.
static void speaker_access(uint8_t port) {
  static FILE *dump;
  static int tried;
  if (!tried) {
    const char *path = getenv("A2_TOGGLE_DUMP");
    tried = 1;
    if (path)
      dump = fopen(path, "w");
  }
  if (dump)
    fprintf(dump, "%u %u\n", (unsigned)s_cycles, (unsigned)port);
  peek((uint16_t)(0xc000 + port));
}
```

      Add `#include <stdlib.h>` beside the existing `#include <stdio.h>` if
      absent. Route all four sites through it.

- [ ] **Step 3: Verify it is deterministic.** Run each scenario twice and
      compare:

```bash
cd decoded/snake-byte
B=../../cmake-build-debug/decoded/snake-byte
for k in play-cold play-hires-cold; do
  for i in 1 2; do
    A2_TOGGLE_DUMP=/tmp/t-$k-$i.txt $B/snake-byte-cold-run \
      --frames=1300 --no-sound --key-file=$k.pkeys >/dev/null 2>&1
  done
  cmp /tmp/t-$k-1.txt /tmp/t-$k-2.txt && echo "$k deterministic"
done
```

      Expected: both print "deterministic". If not, stop — the oracle is
      unusable and the cause must be found first.

- [ ] **Step 4: Record the baselines.**

```bash
cd decoded/snake-byte
cp /tmp/t-play-cold-1.txt toggle-play.txt
cp /tmp/t-play-hires-cold-1.txt toggle-hires.txt
wc -l toggle-play.txt toggle-hires.txt
```

- [ ] **Step 5: Gate it in probe-acceptance.sh.** Add after the cold-start
      section, before the coverage summary:

```bash
# The speaker's toggle timeline: every $C0xx access and the cycle it happened
# on. The Apple II speaker is one bit, so this sequence *is* the waveform --
# which makes it an exact check on any change to the game's timing, and the
# only oracle that covers the sound at all.
for sc in play:toggle-play.txt hires:toggle-hires.txt; do
  name=${sc%%:*}; want=${sc#*:}
  case $name in play) keys=play-cold.pkeys;; hires) keys=play-hires-cold.pkeys;; esac
  A2_TOGGLE_DUMP="$tmp/toggle-$name.txt" "$bin/decoded/snake-byte/snake-byte-cold-run" \
    --frames=1300 --no-sound --key-file="$here/$keys" > /dev/null 2>&1
  if ! diff -q "$here/$want" "$tmp/toggle-$name.txt" > /dev/null; then
    echo "FAIL [toggle/$name]: the speaker timeline changed" >&2
    diff "$here/$want" "$tmp/toggle-$name.txt" | head -5 >&2
    exit 1
  fi
  echo "[toggle/$name] PASS: $(wc -l < "$here/$want") speaker accesses match"
done
```

- [ ] **Step 6: Run the gate.** Expect 26 PASS plus 2 new toggle PASS.

- [ ] **Step 7: Commit.**

```bash
git add -A && git commit -m "snake-byte: the speaker's toggle timeline is an oracle"
```

---

### Task 2: Collapse a leaf drawing routine, to prove the method

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

**Interfaces:**
- Consumes: `speaker_access` from Task 1 (only via the gate).
- Produces: the pattern every later collapse follows.

`game_clear_hgr_native` is the right first target: it contains no speaker
access, no input poll and no call, its cost is a constant, and it is 105 ms —
big enough that getting it wrong shows.

- [ ] **Step 1: Measure its exact cost.** Temporarily wrap the call site:

```c
{ unsigned c0_ = s_cycles; game_clear_hgr_native();
  fprintf(stderr, "CLEARHGR %u\n", (unsigned)(s_cycles - c0_)); }
```

      Run `play-cold` and record the number. Expected: 106,897.

- [ ] **Step 2: Replace the body's ticks with one advance.** Remove the
      `TICK` calls inside `game_clear_hgr_native` and put a single
      `TICK(106897);` at the top, with a comment giving the measurement and
      the routine's duration in ms.

- [ ] **Step 3: Verify the toggle timeline is unchanged.**
      Run the gate. Expected: both toggle checks PASS.
      If they fail, the measured constant is wrong — do not adjust it by
      trial; re-measure.

- [ ] **Step 4: Verify the screen and memory probes.** Expect 6 cold PASS.

- [ ] **Step 5: Commit.**

```bash
git add -A && git commit -m "snake-byte: game_clear_hgr charges once, not 512 times"
```

---

### Task 3: Collapse the remaining constant-cost drawing routines

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

- [ ] **Step 1:** For each of `wipe_occupancy_map`, `draw_border`,
      `game_draw_cell_native`, `game_merge_cell_native`,
      `game_plot_hline_native`, `game_plot_vline_native`,
      `game_lores_vline_native`, `game_plot_shape_native`,
      `game_draw_head_native`, `game_load_shape_masks`, `rom_plot`,
      `rom_plot1`, `rom_scrn`, `rom_gbascalc`, `rom_bascalc`, `rom_vtabz`,
      `rom_clreolz`, `rom_hline`: measure the per-call cost the same way as
      Task 2, one routine at a time.

      Routines whose cost is **data-dependent** (a loop whose trip count
      varies: `rom_hline`, `rom_clreolz`, `game_plot_hline_native`,
      `game_plot_vline_native`, `game_lores_vline_native`) keep a per-iteration
      `TICK` inside the loop and collapse only the straight-line remainder.

- [ ] **Step 2:** After each routine, run the gate. The toggle timeline must
      stay identical. Commit per routine or per small group.

- [ ] **Step 3:** Confirm the count fell:
      `grep -c 'TICK(' decoded/snake-byte/snake-byte-cold.c`

---

### Task 4: The perceptible sites keep their fine structure

**Files:**
- Modify: `decoded/snake-byte/snake-byte-cold.c`

- [ ] **Step 1: Do not collapse these**, and mark each with a comment saying
      why it survives:
      the tone loop's pass (`game_tick_sound_native`), the pace loop's key
      read and speaker click (`game_play_loop_native`), `spin`, the setup
      prompt loop (`game_setup_screen`), the death pause, the blink loops
      (`edit_key_blank`, `edit_key_prompt`), `rom_wait`, and
      `game_sound_sweep_native`.

- [ ] **Step 2: Rename `TICK` to `advance` at the survivors** so the two are
      distinguishable, with `#define advance(n) TICK(n)`.

- [ ] **Step 3:** Gate; commit.

---

### Task 5: Play it

**Files:** none.

- [ ] **Step 1:** `ninja -C cmake-build-release && cmake-build-release/decoded/snake-byte/snake-byte-cold`

- [ ] **Step 2:** Confirm: the game reaches the difficulty prompt, accepts a
      digit, plays, the snake moves at a sane speed, keys steer it, and the
      sound is recognisably the same. **A hang here is the expected failure
      mode** — a polling loop that lost its last advance. The gate cannot see
      it.

- [ ] **Step 3:** Record the result in `HANDOFF.md`.
