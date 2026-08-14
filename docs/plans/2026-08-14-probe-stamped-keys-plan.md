# Probe-stamped key recording and replay — implementation plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development
> (recommended) or superpowers:executing-plans to implement this plan task-by-task.
> Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replay recorded keyboard input on a coordinate defined by the program
rather than by the cycle counter, so the interpreter and a generated build
receive identical input at identical program points.

**Architecture:** A new `record <expr>` statement is the recording counterpart
of the existing `key <expr>`. Both deliver keys *at the probe*, so the run that
was recorded and the run that replays take the same code path. `record` writes
`<stamp> <key>` lines in the format `--key-file` already reads; only the
coordinate changes, from cycles to a counter the script maintains. Two separate
scripts — one that records, one that replays — sharing an install site and a
counting rule.

**Tech stack:** C11 (`lib/a2host`), CMake + Ninja, bash tests
(`tests/run-tests.sh`).

## Why this exists

`drain_key_presses()` compares each key's stamp against `get_cycles()` — *each
engine's own counter*. The two engines' counters differ by up to 61 cycles,
because a generated program can only yield at basic-block boundaries while the
interpreter yields between instructions. Snake Byte polls the keyboard inside a
spin loop (`$741C`), so the iteration at which a key lands is a function of that
counter, and the two engines land on different iterations.

Measured 2026-08-14 (see the decision log entry of the same date): with
`play.keys` truncated to its first 16 keys the two engines agree on all 1,300
frames; adding key 17 splits them permanently from frame 623. Key 17's stamp is
`10,622,152`, which is *exactly* the generated engine's frame-623 boundary —
`play.keys` was recorded from the generated build, so every stamp is that
engine's own coordinate, replayed against a different one.

On a program-defined coordinate the circularity closes instead of diverging: the
key arrives when the counter reaches N, the counter advances on program events,
and both engines count the same program events. The spin loop runs the same
number of iterations on both by construction.

**The payoff is an oracle.** Once input is on a shared coordinate, any surviving
divergence between the interpreter and a generated build is a real decompiler
bug, not a sampling artefact. That is what neither frame hashes nor cycle
stamps could give us.

## Global constraints

- **Never edit `decoded/snake-byte/play.keys`, `play-hires.keys`,
  `play.frames`, `play-hires.frames`, or `snake-byte.json`.** They are
  recordings. New files are derived from them; the originals stay untouched.
- **Probe install sites must be block heads in the generated C.** A probe at any
  other address fires in the interpreter, does not exist in a generated program,
  and the report still reads as agreement. Site lists are grepped from the
  generated C with `CYCLES(0x`, which matches block heads and not
  `CYCLES_EDGE(0x`.
- **Every new rejection test must be mutation-tested**: delete the check it
  covers, confirm the suite goes red, restore. Two probe tests have already
  passed while covering nothing because a different check fired first — assert
  the specific message, never just `FATAL`.
- `.clang-format` is authoritative for C sources. `include/apple2tc/system*-inc.h`
  and `decoded/snake-byte/a2rom.c` are *already* non-clean; do not reformat them
  wholesale, just match local style.
- Build with `ninja -C cmake-build-release` and `ninja -C cmake-build-debug`;
  `decoded/*/decompile.sh` hardcodes `cmake-build-debug`.

## File structure

| File | Responsibility | Change |
|---|---|---|
| `lib/a2host/probe_internal.h` | opcode enum, limits, host-callback declarations | add `OP_RECORD`, declare `probe_record_keys()` |
| `lib/a2host/probe_parse.c` | statement parsing, reserved words | parse `record <expr>`; reserve `record` |
| `lib/a2host/probe.c` | dump tables | `opname()`, `has_operand()` entries |
| `lib/a2host/probe_vm.c` | interpreter loop | `case OP_RECORD` |
| `lib/a2host/a2host.c` | key list, pending queue, options | pending queue, `--record-keys=`, `probe_record_keys()`, drain guard, comment-tolerant key loader |
| `include/apple2tc/probe.h` | public probe API | declare `probe_uses_key()` |
| `tests/probe/record.probe`, `.expected` | compiler baseline | new |
| `tests/run-tests.sh` | suite | new dump test, new rejection tests |
| `decoded/snake-byte/rec.probe` | recording coordinate | new |
| `decoded/snake-byte/play.probe` | replay coordinate | new |
| `decoded/snake-byte/play.pkeys` | counter-stamped keys | generated, committed |
| `decoded/snake-byte/probe-acceptance.sh` | cross-engine gate for the game | new |

---

### Task 1: `record <expr>` compiles

**Files:**
- Modify: `lib/a2host/probe_internal.h` (opcode enum near `OP_KEY`, ~line 149)
- Modify: `lib/a2host/probe_parse.c` (statement dispatch ~line 580, `s_reserved` ~line 922)
- Modify: `lib/a2host/probe.c` (`opname()` ~line 346, `has_operand()` ~line 397)
- Modify: `lib/a2host/probe_vm.c` (~line 410, beside `OP_KEY`)
- Create: `tests/probe/record.probe`, `tests/probe/record.expected`
- Modify: `tests/run-tests.sh`

**Interfaces:**
- Produces: `OP_RECORD` (opcode, pops one stamp operand, no operand cell);
  `void probe_record_keys(uint32_t now);` declared in `probe_internal.h`,
  defined in `a2host.c`.
- Consumes: nothing from other tasks.

Task 2 gives `probe_record_keys()` its real body. In this task it is a
definition that does nothing, so the opcode is exercisable through
`--probe-dump` without dragging the host queue in. That seam is deliberate:
this task's deliverable is "the language accepts and compiles `record`", which
is testable on its own.

- [ ] **Step 1: Add the opcode**

In `probe_internal.h`, immediately after `OP_KEY`:

```c
  OP_KEY,    // pops stamp
  OP_RECORD, // pops stamp
  OP_STOP,
```

- [ ] **Step 2: Declare the host callback**

In `probe_internal.h`, immediately after `probe_deliver_keys`'s declaration:

```c
/// `record`'s half of the same split as probe_deliver_keys above: deliver
/// every key the host has pending *now*, and write each one to the recording
/// file stamped with \p now. Defined in a2host.c beside the queue it drains.
///
/// Delivery and recording are one operation on purpose. If the host pushed
/// keys the moment they arrived and this only wrote the stamps, the recorded
/// session would have used unquantised timing while its replay uses
/// quantised, and the two would not be the same run.
void probe_record_keys(uint32_t now);
```

- [ ] **Step 3: Parse the statement**

In `probe_parse.c`, directly after the `key` case (~line 585):

```c
  if (is_kw(P, "record")) {
    probe_lex_next(&P->lx);
    parse_expr(P, 1);
    emit(P, OP_RECORD);
    return;
  }
```

`parse_expr(P, 1)` and the `&P->lx` form are copied from the `key` case
directly above; keep them identical so the two statements cannot drift.

- [ ] **Step 4: Reserve the word**

In `probe_parse.c`'s `s_reserved` (~line 922), after `"key",`:

```c
    "record",
```

- [ ] **Step 5: Dump tables**

In `probe.c`, in `opname()` after the `OP_KEY` case:

```c
  case OP_RECORD:
    return "RECORD";
```

and in `has_operand()`, add `case OP_RECORD:` to the group returning `false`
(beside `OP_KEY`). Both switches are exhaustive and `default`-less so that
adding an opcode forces a decision here — do not add a `default`.

- [ ] **Step 6: VM case**

In `probe_vm.c`, after the `OP_KEY` case:

```c
    case OP_RECORD:
      // Host state again -- see probe_record_keys in probe_internal.h.
      probe_record_keys(vm_pop());
      break;
```

- [ ] **Step 7: Stub the host callback**

In `a2host.c`, immediately after `probe_deliver_keys`:

```c
void probe_record_keys(uint32_t now) {
  // Body added when --record-keys exists; see the recording task in
  // docs/plans/2026-08-14-probe-stamped-keys-plan.md. Until then `record` is
  // a well-formed statement that records nothing.
  (void)now;
}
```

- [ ] **Step 8: Write the compiler test**

Create `tests/probe/record.probe`:

```
# `record` compiles and takes an arbitrary expression, like `key`. This is a
# dump test: it asserts the bytecode, not any recording, because the host
# side does not exist yet.
counter n

probe r() {
  inc n
  record n
  record n + 1
}

install r at $FA62
```

- [ ] **Step 9: Generate and review the baseline**

Run:

```bash
ninja -C cmake-build-release
cmake-build-release/tools/a2run/a2run --probe=tests/probe/record.probe --probe-dump \
  > tests/probe/record.expected
grep -c RECORD tests/probe/record.expected
```

Expected: `2`. Read the whole file before saving it — a baseline nobody has
read is not a baseline. Confirm the two `RECORD` opcodes are preceded by a
`LOAD_COUNTER` and by `LOAD_COUNTER` + `PUSH_LIT 1` + `ADD` respectively.

- [ ] **Step 10: Wire it into the suite**

In `tests/run-tests.sh`, after `probe_dump_test install`:

```bash
probe_dump_test record
```

- [ ] **Step 11: Add the reserved-word rejection test**

In `tests/run-tests.sh`, in the Declarations block after the existing
`"a reserved name as a probe"` case:

```bash
expect_bad_script "a reserved name as a counter, 'record'" "'record' is a reserved name" \
  'counter record'
```

- [ ] **Step 12: Run the suite**

Run: `cd tests && ./run-tests.sh ../cmake-build-release`
Expected: `Success!`

- [ ] **Step 13: Mutation-test the rejection**

Temporarily remove `"record",` from `s_reserved`, rebuild, run the suite.
Expected: the new rejection test fails. Restore the line, rebuild, confirm
green.

- [ ] **Step 14: Commit**

```bash
git add lib/a2host/probe_internal.h lib/a2host/probe_parse.c lib/a2host/probe.c \
        lib/a2host/probe_vm.c lib/a2host/a2host.c tests/probe/record.probe \
        tests/probe/record.expected tests/run-tests.sh
git commit -m "probe: add the record statement"
```

---

### Task 2: `--record-keys=` and the pending queue

**Files:**
- Modify: `lib/a2host/a2host.c` (`push_key` ~line 208, `push_key_if_empty` ~line 214,
  `probe_record_keys` from Task 1, option parsing ~line 510)

**Interfaces:**
- Consumes: `probe_record_keys(uint32_t)` from Task 1.
- Produces: `--record-keys=<path>`; a file of `<stamp> <key>` lines preceded by
  one `#` comment naming the script that defined the coordinate.

Gating is conditional on `--record-keys` being given. With no recording and no
`key` statement, `push_key()` behaves exactly as it does today — this must not
change `a2emu`'s interactive feel or any existing test.

- [ ] **Step 1: Add the pending queue and the record file**

Near the other file-scope state in `a2host.c` (beside `trace_keys_`, ~line 54):

```c
static FILE *record_keys_file_ = NULL;
/// Keys the host has taken but not yet handed to the machine. Non-empty only
/// while recording: `record` is what releases them, so that the moment a key
/// reaches the program is a program-defined point and not a cycle count.
/// Sized to match a2io's own key queue -- a human cannot outrun one probe
/// firing, and a full queue drops the key loudly rather than silently.
enum { PENDING_KEYS_MAX = 16 };
static uint8_t pending_keys_[PENDING_KEYS_MAX];
static unsigned pending_keys_count_ = 0;
```

- [ ] **Step 2: Gate `push_key`**

Replace `push_key`'s body:

```c
static void push_key(uint8_t ch) {
  if (record_keys_file_) {
    // Held until the next `record`, not pushed now: see probe_record_keys.
    if (pending_keys_count_ == PENDING_KEYS_MAX) {
      fprintf(stderr, "FATAL: more than %u keys pending at one probe site\n",
              (unsigned)PENDING_KEYS_MAX);
      exit(2);
    }
    pending_keys_[pending_keys_count_++] = ch;
    return;
  }
  a2_io_push_key(&io_, ch);
  if (trace_keys_)
    printf("%u %u\n", get_cycles(), ch);
}
```

- [ ] **Step 3: Make `push_key_if_empty` see pending keys**

```c
static void push_key_if_empty(uint8_t ch) {
  // Pending keys count as present, or a burst typed between two probe firings
  // would collapse to its first key while recording and not while replaying.
  if (a2_io_keys_count(&io_) == 0 && pending_keys_count_ == 0)
    push_key(ch);
}
```

- [ ] **Step 4: Implement `probe_record_keys`**

Replace the Task 1 stub:

```c
void probe_record_keys(uint32_t now) {
  for (unsigned i = 0; i != pending_keys_count_; ++i) {
    a2_io_push_key(&io_, pending_keys_[i]);
    if (record_keys_file_)
      fprintf(record_keys_file_, "%u %u\n", now, pending_keys_[i]);
  }
  pending_keys_count_ = 0;
}
```

- [ ] **Step 5: Parse the option**

Beside `--trace-keys` in the option loop:

```c
    if (strncmp(arg, "--record-keys=", 14) == 0) {
      const char *path = arg + 14;
      if ((record_keys_file_ = fopen(path, "wt")) == NULL) {
        perror(path);
        exit(2);
      }
      continue;
    }
```

- [ ] **Step 6: Write the header comment and close the file**

After the option loop has finished and the probe script path is known, write
one comment line so a counter-stamped file is self-describing:

```c
  if (record_keys_file_)
    fprintf(record_keys_file_, "# probe-stamped keys; coordinate defined by %s\n",
            probe_script_path_ ? probe_script_path_ : "(no probe script)");
```

`probe_script_path_` does not exist yet — add a `static const char
*probe_script_path_ = NULL;` set where `--probe=` is handled. Close the file
next to `probe_close_output()`'s call site.

- [ ] **Step 7: Reject recording with no probe script**

Immediately after the option loop:

```c
  if (record_keys_file_ && !probe_script_path_) {
    fprintf(stderr, "FATAL: --record-keys requires --probe= to define the coordinate\n");
    exit(2);
  }
```

Without this, recording silently produces a file of zeros: nothing ever calls
`record`, so every pending key stays pending and no line is written — a file
that looks like "no keys were pressed."

- [ ] **Step 8: Make the key loader tolerate comments**

In `load_key_file` (~line 297), replace the bare `fscanf` loop with one that
skips `#` lines, so a file written by `--record-keys` can be read back:

```c
  unsigned cycles, ch;
  for (;;) {
    int c = getc(f);
    while (c == '#') { // comment to end of line
      while ((c = getc(f)) != EOF && c != '\n') {
      }
      c = getc(f);
    }
    if (c == EOF)
      break;
    ungetc(c, f);
    if ((res = fscanf(f, "%u %u\n", &cycles, &ch)) != 2) {
      fprintf(stderr, "Error parsing key file\n");
      exit(2);
    }
    ...existing capacity growth and append...
  }
```

Keep the existing `res != EOF` diagnostic behaviour: a malformed line must
still be fatal, not silently truncate the key list.

- [ ] **Step 9: Add the rejection test**

In `tests/run-tests.sh`, beside the other Options rejections:

```bash
expect_probe_reject "--record-keys with no probe script" "--record-keys requires --probe=" \
  --record-keys=probe-tmp/rec.txt
```

- [ ] **Step 10: Test recording end to end**

In `tests/run-tests.sh`, after the existing probe run tests:

```bash
# Recording: the same ROM boot, with keys arriving from --kbd-file and being
# stamped on a probe counter rather than on cycles. Two runs must produce
# byte-identical files -- a recording nobody has shown to be deterministic is
# worse than none -- and every stamp must be a counter value, so far below any
# plausible cycle count that a cycle-stamped file could not be mistaken for it.
printf 'A' > probe-tmp/one.kbd
$a2run --frames=10 --probe=probe/reckeys.probe --kbd-file=probe-tmp/one.kbd \
  --record-keys=probe-tmp/rec-a.txt --probe-out=/dev/null > /dev/null
$a2run --frames=10 --probe=probe/reckeys.probe --kbd-file=probe-tmp/one.kbd \
  --record-keys=probe-tmp/rec-b.txt --probe-out=/dev/null > /dev/null
if ! diff -q probe-tmp/rec-a.txt probe-tmp/rec-b.txt > /dev/null; then
  echo "FAIL: --record-keys is not deterministic" >&2
  diff probe-tmp/rec-a.txt probe-tmp/rec-b.txt >&2
  exit 1
fi
if ! grep -qE '^[0-9]+ 65$' probe-tmp/rec-a.txt; then
  echo "FAIL: --record-keys did not record the 'A' that was typed" >&2
  cat probe-tmp/rec-a.txt >&2
  exit 1
fi
```

Create `tests/probe/reckeys.probe`:

```
# The recording coordinate for the --record-keys test: one counter, advanced
# at the ROM's keyboard-read loop, which is where the program asks for input.
# $FD1B is a block head; see the acceptance test for why that matters.
counter n

probe kb() {
  inc n
  record n
}

install kb at $FD1B
```

- [ ] **Step 11: Run the suite**

Run: `cd tests && ./run-tests.sh ../cmake-build-release`
Expected: `Success!`

- [ ] **Step 12: Mutation-test both new checks**

Remove the Step 7 guard, rebuild, run: the "--record-keys with no probe script"
test must fail. Restore. Then make `probe_record_keys` skip the
`fprintf`, rebuild, run: the "did not record the 'A'" check must fail. Restore
and confirm green.

- [ ] **Step 13: Commit**

```bash
git add lib/a2host/a2host.c tests/run-tests.sh tests/probe/reckeys.probe
git commit -m "probe: record keys on a probe-defined coordinate"
```

---

### Task 3: stop the cycle drain from racing the probe

**Files:**
- Modify: `lib/a2host/a2host.c` (`drain_key_presses` ~line 240, `a2host_simulate_frame` ~line 349/360)
- Modify: `include/apple2tc/probe.h`, `lib/a2host/probe.c` (new `probe_uses_key()`)

**Interfaces:**
- Produces: `bool probe_uses_key(void);` — true when the loaded script contains
  at least one `OP_KEY`.

This is a live bug the moment a counter-stamped file exists.
`drain_key_presses()` runs every frame whenever `key_presses_` is loaded, and
`probe_deliver_keys()` shares its cursor. Counter stamps are small integers, so
against `get_cycles()` every one of them is already in the past — the frame
drain would deliver the entire key file during the first simulated frame and
the probe would find nothing left.

- [ ] **Step 1: Write the failing test first**

In `tests/run-tests.sh`, after the recording test from Task 2:

A differential test, so it needs no baseline and no way to observe the
keyboard latch directly. `peek8` is a side-effect-free array read (see
`tests/probe/mem.probe`), so it cannot be used to see what the keyboard
hardware delivered — the two scripts below are distinguished by what the ROM
*does* instead.

In `tests/run-tests.sh`, after the recording test from Task 2:

```bash
# Replay of a counter-stamped file must be driven by `key`, not by the frame
# drain. Counter stamps are small integers, so against get_cycles() they are
# all already in the past and an unguarded drain delivers the whole file
# during the first simulated frame -- leaving `key` nothing to do and making
# the two scripts below behave identically.
#
# replay-key.probe releases only the key stamped 1; replay-nokey.probe
# releases nothing at all. With the drain guarded, the ROM gets one key in the
# first case and none in the second, so the two reports differ. With the drain
# unguarded, the drain delivers both keys in both cases and the reports are
# the same -- which is the failure this asserts against.
printf '1 65\n999999 66\n' > probe-tmp/two.pkeys
$a2run --frames=10 --probe=probe/replay-key.probe --key-file=probe-tmp/two.pkeys \
  --probe-out=probe-tmp/replay-key.txt > /dev/null
$a2run --frames=10 --probe=probe/replay-nokey.probe --key-file=probe-tmp/two.pkeys \
  --probe-out=probe-tmp/replay-nokey.txt > /dev/null
if diff -q probe-tmp/replay-key.txt probe-tmp/replay-nokey.txt > /dev/null; then
  echo "FAIL: delivering a key changed nothing -- the frame drain delivered it anyway" >&2
  exit 1
fi
```

Create `tests/probe/replay-key.probe`:

```
# Releases exactly the key stamped 1: `n` reaches 1 and stops, so the key
# stamped 999999 is never due. The report is the ROM's keyboard-loop trace,
# which changes shape once a key actually arrives.
counter n

probe kb() {
  if (n == 0) {
    inc n
  }
  key n
  printf("%u\n", n)
}

install kb at $FD1B
```

Create `tests/probe/replay-nokey.probe` — identical but for the missing `key`,
so the only variable between the two runs is whether the script delivers:

```
# The control: same sites, same counter, same report, no delivery at all.
counter n

probe kb() {
  if (n == 0) {
    inc n
  }
  printf("%u\n", n)
}

install kb at $FD1B
```

- [ ] **Step 2: Run it and watch it fail**

Run: `cd tests && ./run-tests.sh ../cmake-build-release`
Expected: FAIL with "the frame drain delivered a key the probe never released".

If it passes, stop: the premise is wrong and the rest of this task is
unnecessary. Re-read `a2host_simulate_frame` before continuing.

- [ ] **Step 3: Expose whether the script uses `key`**

In `probe.c`, during or after compilation, set a file-scope flag by scanning
the compiled code for `OP_KEY`, and expose:

```c
bool probe_uses_key(void) {
  return s_uses_key;
}
```

Declare it in `include/apple2tc/probe.h` beside `probe_installed()`:

```c
/// True when the loaded script contains at least one `key` statement, i.e.
/// when key delivery is the script's job. The host's per-frame cycle drain
/// must stand down in that case -- the two would otherwise both deliver from
/// the same list, and since a counter stamp is a small integer, the cycle
/// drain would win every time.
bool probe_uses_key(void);
```

Scan the code array rather than setting a flag in the parser: the parser has
several exits and a flag set in one of them is a thing to keep in sync, while
the compiled code is the artefact that actually determines behaviour.

- [ ] **Step 4: Guard the drain**

In `drain_key_presses()`, at the top:

```c
  if (!key_presses_)
    return;
  // The script delivers; see probe_uses_key(). Returning here rather than
  // skipping the call site keeps the one rule in one place.
  if (probe_uses_key())
    return;
```

- [ ] **Step 5: Run the suite**

Run: `cd tests && ./run-tests.sh ../cmake-build-release`
Expected: `Success!`

- [ ] **Step 6: Confirm the old path is untouched**

Run: `cd decoded/snake-byte && BIN=../../cmake-build-release ./verify.sh`
Expected: 4/4 PASS. `play.keys` is cycle-stamped and its scripts use no probe,
so nothing about it may change.

- [ ] **Step 7: Commit**

```bash
git add lib/a2host/a2host.c lib/a2host/probe.c include/apple2tc/probe.h \
        tests/run-tests.sh tests/probe/replay-key.probe tests/probe/replay-nokey.probe
git commit -m "probe: the frame drain stands down when a script delivers keys"
```

---

### Task 4: choose the coordinate for Snake Byte

**Files:**
- Create: `decoded/snake-byte/rec.probe`, `decoded/snake-byte/play.probe`

**Interfaces:**
- Produces: an install-site set, identical in both scripts.

The coordinate must advance whenever the program asks for input, across *both*
phases of the recording: keys 1–11 of `play.keys` are typed at the BASIC prompt
and consumed by the ROM, and keys 12 onward are consumed by the game's own
polling loops. A coordinate that only covers one phase leaves the other
cycle-quantised.

- [ ] **Step 1: Find every site that reads the keyboard**

```bash
cd decoded/snake-byte
grep -nE 'LDA +\$C000|BIT +\$C000' snake-byte.lst
grep -nE 'LDA +\$C000|BIT +\$C000' ../rom/rom.lst
```

The grep finds the *reads*. The site to install at is the head of the loop
containing each read, which is what fires once per poll:

| read | loop head | what it is |
|---|---|---|
| `$FD21 BIT $C000` | **`$FD1B`** | the ROM's KEYIN wait loop. `$FD1B` is `INC $4E`, the loop head, which is also where the ROM's random seed advances |
| `$741F LDA $C000` | **`$741F`** | the game's delay loop poll — the loop the 2026-08-14 investigation traced the divergence to |
| `$7890 LDA $C000` | **`$7890`** | the game's wait-for-space loop |

Verified 2026-08-14: all three are block heads in `snake-bytec1.c`. Step 2
re-checks that rather than trusting it, because it is the assumption that
fails silently.

- [ ] **Step 2: Confirm each is a block head in the generated C**

```bash
grep -oE 'CYCLES\(0x[0-9a-f]+' snake-bytec1.c | sed 's/CYCLES(0x//' | sort -u > /tmp/sb-blocks.txt
for a in 741f 7890 fd1b; do
  grep -qx "$a" /tmp/sb-blocks.txt && echo "$a ok" || echo "$a NOT A BLOCK HEAD"
done
```

Any address reported as not a block head must be replaced by the head of the
block containing it, or the probe fires only in the interpreter and the reports
still diff clean. Do not skip this check.

- [ ] **Step 3: Write the recording script**

Create `decoded/snake-byte/rec.probe`:

```
# The recording half. One counter, advanced wherever the program reads the
# keyboard -- $FD1B is the ROM's KEYIN loop (keys typed at the BASIC prompt),
# $741F and $7890 the game's own polls. Both phases must be covered or half
# the recording stays cycle-quantised.
#
# play.probe must install at exactly these sites and count exactly this way.
# Nothing enforces that; a mismatch shows up as a failed replay.
counter n

probe kb() {
  inc n
  record n
}

install kb at $FD1B, $741F, $7890
```

- [ ] **Step 4: Write the replay script**

Create `decoded/snake-byte/play.probe`:

```
# The replay half of rec.probe -- same sites, same counter, `key` instead of
# `record`. See rec.probe for why these sites.
counter n

probe kb() {
  inc n
  key n
}

install kb at $FD1B, $741F, $7890
```

- [ ] **Step 5: Verify both compile**

```bash
../../cmake-build-release/tools/a2run/a2run --probe=rec.probe --probe-dump | head -20
../../cmake-build-release/tools/a2run/a2run --probe=play.probe --probe-dump | head -20
```

Expected: no errors; `RECORD` in the first, `KEY` in the second, three install
sites in each.

- [ ] **Step 6: Commit**

```bash
git add decoded/snake-byte/rec.probe decoded/snake-byte/play.probe
git commit -m "snake-byte: the recording and replay coordinate"
```

---

### Task 5: convert `play.keys` and prove the replay reproduces it

**Files:**
- Create: `decoded/snake-byte/play.pkeys` (generated, committed)

**Interfaces:**
- Consumes: `rec.probe`, `play.probe` from Task 4; `--record-keys` from Task 2.

`play.keys` drives the conversion run and is never modified. If the replay
reproduces the conversion run's frame hashes exactly, the conversion is
faithful; if it does not, the coordinate is wrong and Task 4 needs revisiting
before anything downstream is believable.

- [ ] **Step 1: Convert**

```bash
cd decoded/snake-byte
A=../../cmake-build-release
$A/decoded/snake-byte/snake-bytec1-run --key-file=play.keys --probe=rec.probe \
  --record-keys=play.pkeys --probe-out=/dev/null --frames=1300 --hash-frames=/tmp/conv.frames
wc -l play.pkeys
```

Expected: 24 lines — one `#` header plus 23 keys. If fewer, a key was pressed
where no site fires and the coordinate does not cover the whole session; go
back to Task 4.

- [ ] **Step 2: Check the stamps are counter values**

```bash
awk '!/^#/{print $1}' play.pkeys | sort -n | tail -1
```

Expected: a number far below `play.keys`'s largest cycle stamp of 20,033,808.
A stamp of the same order as a cycle count means `record` was handed the wrong
expression.

- [ ] **Step 3: Replay and compare against the conversion run**

```bash
$A/decoded/snake-byte/snake-bytec1-run --key-file=play.pkeys --probe=play.probe \
  --probe-out=/dev/null --frames=1300 --hash-frames=/tmp/replay.frames
diff <(awk '{print $3}' /tmp/conv.frames) <(awk '{print $3}' /tmp/replay.frames) | head
```

Expected: no output. The replayed run must be the recorded run, frame for
frame. Cycle counts (column 2) may differ trivially; the hashes must not.

- [ ] **Step 4: Confirm `play.keys` is untouched**

```bash
git status --short decoded/snake-byte/play.keys
```

Expected: no output.

- [ ] **Step 5: Commit**

```bash
git add decoded/snake-byte/play.pkeys
git commit -m "snake-byte: play.keys converted to probe-stamped play.pkeys"
```

---

### Task 6: the cross-engine gate, and what frame 623 does

**Files:**
- Create: `decoded/snake-byte/probe-acceptance.sh`

**Interfaces:**
- Consumes: everything above.

This is the measurement the whole plan is for. Model the script on
`decoded/rom/probe-acceptance.sh`, which already checks the interpreter against
both generated back ends and asserts no probe went unfired.

- [ ] **Step 1: Measure frame hashes across engines**

```bash
cd decoded/snake-byte
A=../../cmake-build-release
$A/tools/a2run/a2run --preload snake-byte.b33 --key-file=play.pkeys --probe=play.probe \
  --probe-out=/dev/null --frames=1300 --hash-frames=/tmp/pk-i.frames
$A/decoded/snake-byte/snake-bytec1-run --key-file=play.pkeys --probe=play.probe \
  --probe-out=/dev/null --frames=1300 --hash-frames=/tmp/pk-g.frames
diff <(awk '{print $3}' /tmp/pk-i.frames) <(awk '{print $3}' /tmp/pk-g.frames) | head
```

Record the result whatever it is. Two outcomes, both worth having:

- **They agree on all 1,300 frames.** Frame 623 was the cycle-stamped input
  coordinate, and Question 1 has its oracle.
- **They still diverge.** The input coordinate is no longer a candidate
  explanation, and what remains is a real difference between the two engines —
  which is exactly what this instrument was built to isolate. Note the first
  differing frame and stop; root-causing it is separate work.

- [ ] **Step 2: Compare probe reports, not just frame hashes**

```bash
grep -oE 'CYCLES\(0x[0-9a-f]+' snake-bytec1.c | sed 's/CYCLES(0x//' | sort -u > blocks.txt
```

Write `trace.probe` installing at `@"blocks.txt"` and printing `%04X` of `pc`,
exactly as `decoded/rom/trace.probe` does — but it must *also* carry the
`key n` delivery, or the replay has no coordinate:

```
counter n

probe kb() {
  inc n
  key n
}

probe trace(site = pc) {
  printf("%04X\n", site)
}

install kb at $FD1B, $741F, $7890
install trace at @"blocks.txt"
```

Run both engines with it and diff the reports. This is far stricter than frame
hashes: it compares the program's control flow at every block head.

- [ ] **Step 3: Write the acceptance script**

Create `decoded/snake-byte/probe-acceptance.sh` following
`decoded/rom/probe-acceptance.sh`'s structure: derive the site list from the
generated C, run the interpreter and each generated build, check for
"never fired" on both sides, and diff. Check `snake-bytec1-run` and
`snake-bytec1-ext-run` — the ext build substitutes hand-written `a2rom.c` and
`game.c`, so it is a genuinely different program and worth its own comparison.

- [ ] **Step 4: Mutation-test the new gate**

Perturb one `CYCLES` constant in `snake-bytec1.c` by 1, rebuild, run the
script. Expected: FAIL. Restore, rebuild, confirm PASS. A gate nobody has
watched fail is not a gate.

- [ ] **Step 5: Run every gate**

```bash
cd tests && ./run-tests.sh ../cmake-build-release
cd ../decoded/snake-byte && BIN=../../cmake-build-release ./verify.sh
./probe-acceptance.sh ../../cmake-build-release
cd ../rom && ./probe-acceptance.sh ../../cmake-build-release
```

Expected: `Success!`, 4/4, and both acceptance scripts passing.

- [ ] **Step 6: Commit**

```bash
git add decoded/snake-byte/probe-acceptance.sh decoded/snake-byte/blocks.txt \
        decoded/snake-byte/trace.probe
git commit -m "snake-byte: cross-engine probe acceptance on probe-stamped input"
```

---

### Task 7: documentation

**Files:**
- Modify: `docs/probes.md`, `HANDOFF.md`, `docs/decompiling/decision-log.md`

- [ ] **Step 1: Document `record` in the language reference**

Add `record <expr>` to `docs/probes.md` beside `key <expr>`, and update the
"Execution status" section: counter-stamped recording is no longer phase 2
work. State the one thing the language does not enforce — that a recording
script and its replay script agree on the coordinate.

- [ ] **Step 2: Update HANDOFF.md**

The "Next" list item 5 currently names cycle-denominated key stamps as
outstanding. Replace it with whatever Task 6 measured.

- [ ] **Step 3: Write the decision-log entry**

Cover: why cycle stamps could not work (the `$741C` measurement), what the
coordinate change buys, the conversion method, and Task 6's result — including
if the divergence survived. An entry that only records the good outcome is
worth less than one that records what was measured.

- [ ] **Step 4: Commit**

```bash
git add docs/probes.md HANDOFF.md docs/decompiling/decision-log.md
git commit -m "docs: probe-stamped key recording"
```

---

## Self-review

**Spec coverage.** `record` statement (Task 1), `--record-keys` (Task 2),
delivery gated at the probe per the "same run" requirement (Task 2, steps 2–4),
separate record and replay scripts (Task 4), the generated key file standing
alone (Task 5 — `play.pkeys` replays with no reference to `play.keys`).

**Known gaps, deliberate.** `play-hires.keys` is not converted; it should follow
once `play.keys` works, and needs its own site check because the hi-res
scenario reaches `$664A`. `--trace-keys` still writes cycle stamps and is left
alone — it is the recording path for a session that has no probe.

**Risk to watch.** Task 4 is the one that can silently be wrong: if a keyboard
read happens at a site the coordinate does not cover, that key stays
cycle-quantised and the divergence survives for a reason that looks like a
decompiler bug. Task 5 step 1's line count is the check that catches it — do
not skip it or wave it through with "close enough".
