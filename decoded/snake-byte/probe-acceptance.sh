#!/bin/bash
#
# The cross-engine gate for probe-stamped input: the interpreter and each
# generated build must produce byte-identical block-head traces when replayed
# from play.pkeys -- the 23 keystrokes stamped on the probe counter that
# play.probe/rec.probe define, rather than on raw cycles. Modeled directly on
# decoded/rom/probe-acceptance.sh; read that file's header first.
#
# Why this is stricter than verify.sh. verify.sh (play.keys, cycle-stamped)
# diffs one video hash per frame -- 1,300 numbers. This diffs the address of
# every basic block dispatched over the same run, which is on the order of
# 2.7 million lines. Two programs can hash the same screen at frame boundaries
# while taking different paths to get there; they cannot produce the same
# 2.7-million-line PC trace by accident. trace.probe carries play.probe's own
# `kb` delivery (same four sites, same counter, `key` instead of `record`) so
# the replay lands on the coordinate play.pkeys was stamped on, not on wall
# cycles -- see rec.probe for why the coordinate needs exactly those four
# sites, including $6217.
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
keys="$here/play.pkeys"

# Compare one generated build against the interpreter on that build's own
# block heads. $1: label, $2: generated binary, $3: probe script, $4: where to
# keep the site list, $5..: the C source(s) that make up the binary.
check_backend() {
  local label=$1 prog=$2 probe=$3 sites=$4
  shift 4
  local srcs=("$@")
  local interp="/tmp/pkeys-probe-interp-$label.txt" gen="/tmp/pkeys-probe-$label.txt"

  [ -x "$prog" ] || { echo "Error: not found: $prog" >&2; exit 1; }

  # Block heads, straight from the artifact(s) that define them. The list is
  # committed so a failure is bisectable without regenerating the C first, but
  # it is still rebuilt here so it never goes stale relative to the C actually
  # being tested.
  grep -ohE 'CYCLES\(0x[0-9a-f]+' "${srcs[@]}" | sed 's/CYCLES(0x//' | sort -u > "$sites"
  echo "[$label] site list: $(wc -l < "$sites") block heads"

  "$a2run" --preload "$b33" --key-file="$keys" --probe="$probe" \
    --probe-out="$interp" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-probe-interp-$label.err"
  "$prog" --key-file="$keys" --probe="$probe" \
    --probe-out="$gen" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-probe-$label.err"

  # A probe that never fired means the two sides did not cover the same set,
  # even if what they did emit matches -- e.g. a site both installed but
  # neither ever executed would leave two empty reports that diff clean.
  local side
  for side in "interp-$label" "$label"; do
    if grep -q "never fired" "/tmp/pkeys-probe-$side.err"; then
      echo "FAIL [$side]: a probe never fired" >&2
      grep "never fired" "/tmp/pkeys-probe-$side.err" >&2
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

check_backend trace "$bin/decoded/snake-byte/snake-bytec1-run" "$here/trace.probe" \
  "$here/blocks.txt" "$here/snake-bytec1.c"
check_backend trace-ext "$bin/decoded/snake-byte/snake-bytec1-ext-run" "$here/trace-ext.probe" \
  "$here/blocks-ext.txt" "$here/snake-bytec1-ext.c" "$here/a2rom.c" "$here/game.c"

if diff -q "$here/blocks.txt" "$here/blocks-ext.txt" > /dev/null; then
  echo "the two back ends agree on the block-head set"
else
  echo "note: the back ends emit different block-head sets ($(wc -l < "$here/blocks.txt")" \
       "vs $(wc -l < "$here/blocks-ext.txt")); each was checked against its own"
fi
