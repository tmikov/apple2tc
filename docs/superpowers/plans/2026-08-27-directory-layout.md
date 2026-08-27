# Directory Layout Implementation Plan

**Goal:** Make `decoded/snake-byte/` announce its own result. A visitor runs `ls`, sees 63 files including seven near-identically named `.c` files, and has no way to tell which one is the decompilation.

**Architecture:** Two moves. The first puts scaffolding behind three directories so the artifact is the only source file at top level. The second frees the obvious name for it. Both are pure renames; `probe-acceptance.sh` at 31 PASS is the proof, because a missed path fails loudly rather than silently.

## Global Constraints

- Use `git mv`, never `mv` — the history has to follow the file.
- **Scripts stay at top level.** Every one computes `here` from its own location (`dirname "${BASH_SOURCE[0]}"`), so moving them into a `tools/` directory would break all 50+ `$here/…` references at once. They are also the things a visitor runs, so hiding them is backwards.
- Each move ends at **31 PASS** on `./probe-acceptance.sh ../../cmake-build-debug`.
- `yield-lint.awk` must still exit 0.
- Never edit the contents of `snake-byte.json`, a `.frames` or a `.pkeys` file — moving them is fine.
- Dated plans and specs under `docs/superpowers/` are records of what was true when written. Do not rewrite their prose. `README.md`, `HANDOFF.md` and `docs/decompiling/decision-log.md` are living and must be updated.

---

### Move 1: three directories

**Target layout** — 14 files and 3 directories, from 63 files:

```
README.md  CMakeLists.txt
snake-byte-cold.c  game-image.inc  rom-image.inc  entry-state-inc.h
probe-acceptance.sh  verify.sh  decompile.sh  codegen-fingerprint.sh
make-easy.sh  make-entry-state.sh  make-cold-keys.sh  yield-lint.awk
reference/     the builds the gate compares against  (11 files, 2.5 MB)
testdata/      binaries, recording, keys, probes, baselines  (36 files)
as-generated/  the raw decompiler output  (2 files, 1.7 MB)
```

- [ ] **Step 1: `as-generated/`** — `snake-byte.c`, `snake-byte.lst`.

  Named for what it is, not `history/`: this is the decompiler's own output, the
  first form the game took, and the directory's bulk is the point of keeping it.
  `history/` invites deletion.

  3 reference lines to update (`decompile.sh`, `CMakeLists.txt`).

- [ ] **Step 2: `reference/`** — `snake-bytec1.c`, `snake-bytec1-ext.c`,
      `snake-byte-easyc1-ext.c`, `snake-byte-ext.c`, `snake-byte-easy-ext.c`,
      `a2rom.c/.h`, `game.c/.h`, `game_native.c/.h`.

  These are not clutter and not alternatives to choose between — they are the
  oracle. `probe-acceptance.sh` runs `snake-bytec1-ext` as the booting build the
  cold build is compared against; `verify.sh` runs `snake-bytec1` as the control
  for that; `snake-byte-easyc1-ext` is the fixture that reaches the display-list
  interpreter. The directory name should say so.

  13 reference lines in scripts, plus `CMakeLists.txt` source paths.

- [ ] **Step 3: `testdata/`** — the 36 data files: both `.b33`, the `.json`
      recording, `code-at.txt`, `labels.txt`, `coverage.txt`, `known-data.txt`,
      `inline-str.txt`, `rom.externs`, the four `blocks*.txt`, all `.pkeys`,
      `.keys`, `.kbd`, `.frames`, the two `toggle-*.txt` baselines, and all
      nine `.probe` files.

  34 reference lines in scripts.

- [ ] **Step 4: gate.** `./probe-acceptance.sh ../../cmake-build-debug 2>&1 | grep -c PASS` → 31, and `awk -f yield-lint.awk snake-byte-cold.c` → exit 0.

- [ ] **Step 5: also check the paths the gate does not exercise** — `decompile.sh`, `make-easy.sh`, `make-entry-state.sh`, `make-cold-keys.sh` are not run by the gate, so their paths must be read rather than tested. Confirm each names its inputs and outputs under the new directories.

- [ ] **Step 6: commit.**

---

### Move 2: free the obvious name

The artifact is `snake-byte-cold.c`. "Cold" means "starts at the game's entry
instead of booting the machine" — a distinction meaningful only to someone
already inside the project. The best name in the directory is held by a 2022
fossil that nothing runs.

- [ ] **Step 1: rename the fossil first**, or the next step collides:
      `as-generated/snake-byte.c` → `as-generated/snake-byte-simple-c.c`, named
      for the `--simple-c` flag that produced it. Rename its CMake target
      `snake-byte` → `snake-byte-as-generated`.

- [ ] **Step 2: rename the artifact.** `snake-byte-cold.c` → `snake-byte.c`,
      and the targets `snake-byte-cold` → `snake-byte`, which makes the runner
      `snake-byte-run`.

      Do this with a plain textual substitution of `snake-byte-cold` →
      `snake-byte` across `CMakeLists.txt` and the scripts. It is safe: the
      fixtures that share the word are `play-cold.pkeys`, `play-hires-cold.pkeys`,
      `trace-cold.probe`, `ram-cold.probe` and `blocks-cold.txt`, none of which
      contain the string `snake-byte-cold`. `snake-byte-cold-run` correctly
      becomes `snake-byte-run`.

      Leave those five fixture names alone. They describe the *scenario* — a run
      that starts cold — which is still exactly what they are.

- [ ] **Step 3: gate**, then the four ungated scripts as in Move 1 Step 5.

- [ ] **Step 4: commit.**

---

### Move 3: say so in the README

- [ ] **Step 1:** rewrite the README's opening so the first thing a reader
      learns is which file is the result and which directory is scaffolding.
      Update its file tables to the new layout. It currently claims
      `snake-byte-cold.c` is 7,219 lines; it is 5,322.

- [ ] **Step 2:** update the living docs — `HANDOFF.md` and
      `docs/decompiling/decision-log.md` — for the new file and target names.
      Leave dated plans and specs alone.

- [ ] **Step 3:** commit.
