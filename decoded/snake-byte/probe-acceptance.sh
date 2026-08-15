#!/bin/bash
#
# The cross-engine gate for probe-stamped input: the interpreter and each
# generated build must produce byte-identical block-head traces, and identical
# memory, when replayed from the .pkeys files -- keystrokes stamped on the probe
# counter that play.probe/rec.probe define, rather than on raw cycles. Both of
# verify.sh's scenarios are run: play.pkeys (23 keys) and play-hires.pkeys (19).
# Modeled directly on decoded/rom/probe-acceptance.sh; read that header first.
#
# Why this is stricter than verify.sh. verify.sh (play.keys, cycle-stamped)
# diffs one video hash per frame -- 1,300 numbers. This diffs the address of
# every basic block dispatched over the same run, which is on the order of
# 2.7 million lines. Two programs can hash the same screen at frame boundaries
# while taking different paths to get there; they cannot produce the same
# 2.7-million-line PC trace by accident. And ram.probe then compares memory at
# every in-game sample, because a wrong byte written need not change control
# flow within one run. trace.probe carries play.probe's own `kb` delivery (same
# sites, same counter, `key` instead of `record`) so the replay lands on the
# coordinate the .pkeys file was stamped on, not on wall cycles -- see
# rec.probe for why the coordinate needs exactly those sites, and for the
# measurements behind $6217 and $760F.
#
# Why two site lists. snake-bytec1.c decompiles the ROM alongside the game and
# is self-contained. snake-bytec1-ext.c has the ROM entry points listed in
# rom.externs, and the game's own $664A COUT hook, replaced by hand-written
# a2rom.c/game.c (stitched into one translation unit by snake-byte-ext.c --
# see that file's header for why they must share a TU). The two therefore
# expose different sets of block heads: every CYCLES(0x... site the compiled
# binary can actually be probed at, which for the -ext build spans all three
# source files, not just the generated one. Installing a probe at an address
# that isn't a block head in the binary under test would fire in the
# interpreter, not exist on the generated side, and still diff clean -- the
# one failure mode this design cannot otherwise see (see AddEdgeCycles in
# tools/apple2tc/ir/Values.def for why CYCLES_EDGE(0x is deliberately excluded
# from the grep).
#
# Each backend gets its own site list and its own interpreter run, rather than
# asserting the two lists are equal and sharing one run -- the ext list is in
# fact a strict subset of the ref list today (see the note this script prints
# at the end), but nothing requires that to stay true.

set -e
# The site-list pipelines below (`grep | sed | sort -u`) run under `set -e`
# alone, `$?` is `sort`'s, not `grep`'s: a `grep` that hard-fails on a
# missing/renamed source file (exit 2, plus a warning on stderr) would still
# leave `sort` exiting 0, so the script would sail on with a silently
# shrunken site list. pipefail makes the pipeline's status the worst of its
# parts, so that failure now aborts here instead.
set -o pipefail

if [ -z "$1" ]; then
  echo "Usage: $0 <build-dir>" >&2
  exit 1
fi

bin=$1
a2run="$bin/tools/a2run/a2run"
here=$(dirname "$0")

[ -x "$a2run" ] || { echo "Error: not found: $a2run" >&2; exit 1; }

frames=${FRAMES:-1300}
b33="$here/snake-byte.b33"
keys="${KEYS:-$here/play.pkeys}"

# Every script that delivers keys carries its own copy of the coordinate --
# the probe language has no include, so the `install kb at ...` line is
# duplicated five ways. They must all name the same sites in the same order or
# the stamps in a .pkeys file mean different things to different scripts, and
# each script stays individually valid while the set quietly disagrees. That is
# the same drift hazard the trace.probe/trace-ext.probe check covers, so it is
# checked the same way: asserted, not trusted.
coord=$(grep -h '^install kb at ' "$here/rec.probe")
for p in play.probe trace.probe trace-ext.probe ram.probe; do
  if [ "$(grep -h '^install kb at ' "$here/$p")" != "$coord" ]; then
    echo "FAIL: $p installs the coordinate differently from rec.probe" >&2
    echo "  rec.probe: $coord" >&2
    echo "  $p: $(grep -h '^install kb at ' "$here/$p")" >&2
    exit 1
  fi
done
echo "coordinate: ${coord#install kb at }"

# The number of keys a correct run must load and deliver, derived from the
# .pkeys file itself rather than hardcoded: an empty or truncated key file (a
# regression in the file, or a mis-parse) would otherwise still make the rest
# of this script pass -- see the "Loaded N keys" check below for why an empty
# key file is exactly the failure mode this exists to catch. `|| true`: grep
# exits 1 on zero matches (an empty/all-comment file), which would otherwise
# trip `set -e` and abort with no explanation before the check below runs.
set_scenario() {
  # $1: the .pkeys file for this scenario. Sets $keys and $expected_keys, which
  # check_backend and check_memory both read.
  keys=$1
  if [ ! -f "$keys" ]; then
    echo "FAIL: $keys does not exist" >&2
    exit 1
  fi
  expected_keys=$(grep -vc '^#' "$keys" || true)
  if [ "$expected_keys" -eq 0 ]; then
    echo "FAIL: $keys has no keys to replay -- empty or all-comment" >&2
    exit 1
  fi
  echo "--- scenario $(basename "$keys"): $expected_keys keys ---"
}

# Compare one generated build against the interpreter on that build's own
# block heads. $1: label, $2: generated binary, $3: probe script, $4: where to
# keep the site list, $5: minimum acceptable site count, $6..: the C
# source(s) that make up the binary.
check_backend() {
  local label=$1 prog=$2 probe=$3 sites=$4 floor=$5
  shift 5
  local srcs=("$@")
  local tag="$label-$(basename "$keys" .pkeys)"
  local interp="/tmp/pkeys-probe-interp-$tag.txt" gen="/tmp/pkeys-probe-$tag.txt"

  [ -x "$prog" ] || { echo "Error: not found: $prog" >&2; exit 1; }

  # Block heads, straight from the artifact(s) that define them. The list is
  # committed so a failure is bisectable without regenerating the C first, but
  # it is still rebuilt here so it never goes stale relative to the C actually
  # being tested.
  grep -ohE 'CYCLES\(0x[0-9a-f]+' "${srcs[@]}" | sed 's/CYCLES(0x//' | sort -u > "$sites"
  local nsites
  nsites=$(wc -l < "$sites")
  echo "[$label] site list: $nsites block heads"
  # An empty list is already caught downstream (the probe compiler rejects
  # it), but a partial one is not: e.g. one of several source files present
  # but truncated to empty would still let grep/sort exit 0. $floor is a
  # generous margin below the count committed in $sites (not a tight pin --
  # see the call sites below for the actual numbers), just enough to catch a
  # source file's worth of sites silently going missing.
  if [ "$nsites" -lt "$floor" ]; then
    echo "FAIL [$label]: site list has only $nsites block heads, expected at least $floor" >&2
    exit 1
  fi

  "$a2run" --preload "$b33" --key-file="$keys" --probe="$probe" \
    --probe-out="$interp" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-probe-interp-$tag.err"
  "$prog" --key-file="$keys" --probe="$probe" \
    --probe-out="$gen" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-probe-$tag.err"

  # A probe that never fired means the two sides did not cover the same set,
  # even if what they did emit matches -- e.g. a site both installed but
  # neither ever executed would leave two empty reports that diff clean.
  #
  # Neither check below is enough on its own to prove keys were actually
  # delivered: an empty --key-file= makes both engines produce identical,
  # "never fired"-free traces too -- the reports agree vacuously because
  # nothing input-dependent ran. "Loaded $expected_keys keys" is a2host's own
  # confirmation (lib/a2host/a2host.c's load_key_file()) that the file was
  # read and parsed, not skipped, empty, or truncated; asserting it in BOTH
  # runs is what makes this gate mean "the two engines agree with real input
  # in play," not just "the two engines agree."
  local side
  for side in "interp-$tag" "$tag"; do
    if grep -q "never fired" "/tmp/pkeys-probe-$side.err"; then
      echo "FAIL [$side]: a probe never fired" >&2
      grep "never fired" "/tmp/pkeys-probe-$side.err" >&2
      exit 1
    fi
    if ! grep -qx "Loaded $expected_keys keys" "/tmp/pkeys-probe-$side.err"; then
      echo "FAIL [$side]: expected to see 'Loaded $expected_keys keys' on stderr" \
           "(from $keys) -- keys were not actually delivered" >&2
      cat "/tmp/pkeys-probe-$side.err" >&2
      exit 1
    fi
  done

  if ! diff -q "$interp" "$gen" > /dev/null; then
    echo "FAIL [$label]: the interpreter and the generated build disagree" >&2
    diff "$interp" "$gen" | head -20 >&2
    echo "interp hits: $(wc -l < "$interp")" >&2
    echo "gen hits:    $(wc -l < "$gen")" >&2
    exit 1
  fi

  echo "[$label] PASS: $(wc -l < "$interp") probe hits match over $frames frames"
}

# trace.probe and trace-ext.probe name themselves after the backend they
# install over, so the two runs cannot silently share one. They must otherwise
# be the same program -- checked here rather than trusted, since an edit to
# one and not the other would leave both halves individually valid while
# quietly testing two different things.
strip_probe() {
  # Drop comments, blank lines and the install lines, leaving the program.
  grep -vE '^\s*(#|install\b|$)' "$1"
}
if ! diff -q <(strip_probe "$here/trace.probe") <(strip_probe "$here/trace-ext.probe") \
     > /dev/null; then
  echo "FAIL: trace.probe and trace-ext.probe are not the same program" >&2
  diff <(strip_probe "$here/trace.probe") <(strip_probe "$here/trace-ext.probe") >&2
  exit 1
fi

# Floors: measured 1,694 (trace) and 1,669 (trace-ext) block heads as of this
# writing. 1,600 leaves headroom for legitimate drift (a simplifyCFG change
# that merges a few blocks) while still catching the failure this guards
# against -- a whole source file (a2rom.c alone contributes 75 sites, game.c
# 5, and losing snake-bytec1-ext.c itself would collapse the -ext list to
# under 100) going missing or empty out of the grep.
# Both scenarios verify.sh replays, now against the interpreter as well.
# play-hires is not a duplicate of play: it reaches $664A (the game's own hi-res
# COUT hook) and $7541, code the recording never took and that exists only via
# --code-at -- and in the -ext build $664A is hand-written C in game.c, so the
# hires/ext pair is the only cross-engine check that hand-written replacement
# gets. KEYS= overrides the list for a one-off run.
for keyfile in ${KEYS:-"$here/play.pkeys" "$here/play-hires.pkeys"}; do
  set_scenario "$keyfile"

check_backend trace "$bin/decoded/snake-byte/snake-bytec1-run" "$here/trace.probe" \
  "$here/blocks.txt" 1600 "$here/snake-bytec1.c"
check_backend trace-ext "$bin/decoded/snake-byte/snake-bytec1-ext-run" "$here/trace-ext.probe" \
  "$here/blocks-ext.txt" 1600 "$here/snake-bytec1-ext.c" "$here/a2rom.c" "$here/game.c"

if diff -q "$here/blocks.txt" "$here/blocks-ext.txt" > /dev/null; then
  echo "the two back ends agree on the block-head set"
else
  echo "note: the back ends emit different block-head sets ($(wc -l < "$here/blocks.txt")" \
       "vs $(wc -l < "$here/blocks-ext.txt")); each was checked against its own"
fi

# Memory equivalence. The checks above prove the engines take the same path;
# this proves they compute the same values along it. A wrong byte written to
# memory need not change control flow within 1300 frames, so neither check
# subsumes the other. ram.probe documents why the stack page is excluded and
# why that exclusion costs nothing (return-address slots are synthetic in a
# generated build by construction; bytes below SP are dead).
#
# No site list: ram.probe installs at fixed addresses, so there is nothing to
# derive or to go stale. The "Loaded N keys" assertion still applies -- without
# input, memory would agree vacuously the same way the traces would.
check_memory() {
  local label=$1 prog=$2
  local tag="$label-$(basename "$keys" .pkeys)"
  local interp="/tmp/pkeys-ram-interp-$tag.txt" gen="/tmp/pkeys-ram-$tag.txt"

  [ -x "$prog" ] || { echo "Error: not found: $prog" >&2; exit 1; }

  "$a2run" --preload "$b33" --key-file="$keys" --probe="$here/ram.probe" \
    --probe-out="$interp" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-ram-interp-$tag.err"
  "$prog" --key-file="$keys" --probe="$here/ram.probe" \
    --probe-out="$gen" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-ram-$tag.err"

  local side
  for side in "interp-$tag" "$tag"; do
    if grep -q "never fired" "/tmp/pkeys-ram-$side.err"; then
      echo "FAIL [ram/$side]: a probe never fired" >&2
      exit 1
    fi
    if ! grep -qx "Loaded $expected_keys keys" "/tmp/pkeys-ram-$side.err"; then
      echo "FAIL [ram/$side]: expected 'Loaded $expected_keys keys' on stderr" >&2
      exit 1
    fi
  done

  if ! diff -q "$interp" "$gen" > /dev/null; then
    echo "FAIL [ram/$label]: the engines disagree on memory contents" >&2
    diff "$interp" "$gen" | head -10 >&2
    exit 1
  fi

  echo "[ram/$label] PASS: memory identical at $(wc -l < "$interp") in-game samples"
}

check_memory ref "$bin/decoded/snake-byte/snake-bytec1-run"
check_memory ext "$bin/decoded/snake-byte/snake-bytec1-ext-run"
done
