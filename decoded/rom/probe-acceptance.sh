#!/bin/bash
#
# The acceptance test for probes: the interpreter and the decompiled ROM must
# produce byte-identical probe reports over a boot. Both C back ends are
# checked -- rom.c from --simple-c and romc1.c from --irc1.
#
# Why this test and not a new one. 1,991 of 1,991 branch targets already match
# between these two over 120 frames of ROM boot -- measured 2026-08-08, with a
# bespoke trace comparison. Reproducing that number through probes validates
# the probe mechanism against a result we already believe. A disagreement here
# means the probes are wrong, not the decompiler.
#
# Why both back ends. They are not two spellings of one thing. romc1 is the
# only build that compiles against system2-inc.h, which keeps the status flags
# unpacked and so exercises the per-flag liveness DCE that system-inc.h's
# packed byte never reaches; and only --irc1 emits the taken-branch edge
# charge as a real trampoline block, where --simple-c folds it into the
# dispatch ternary. Testing rom.c alone left that whole mechanism with no
# cross-engine coverage.
#
# The site lists are derived from the generated C rather than from apple2tc,
# because CYCLES is emitted per basic block: grepping it out yields exactly the
# addresses that generated program *can* probe. Installing at any other address
# would fire in the interpreter and silently not exist on the other side --
# which is the one failure mode this design cannot otherwise see. Note that
# "CYCLES(0x" deliberately does not match CYCLES_EDGE(0x: an edge charge
# carries the address of a branch the enclosing block already reported, and is
# not a place the program can be observed at (see AddEdgeCycles in
# tools/apple2tc/ir/Values.def).
#
# Each back end gets its own list, and the interpreter is re-run against each,
# rather than asserting the two lists are equal and sharing one run. They are
# in fact identical today -- 1,718 addresses each, which is worth knowing and
# is reported below -- but nothing requires that to stay true, and a future
# simplifyCFG change that merged blocks in --irc1 only should show up as a
# different site count, not as a spurious failure.

set -e

if [ -z "$1" ]; then
  echo "Usage: $0 <build-dir>" >&2
  exit 1
fi

bin=$1
a2run="$bin/tools/a2run/a2run"
here=$(dirname "$0")

[ -x "$a2run" ] || { echo "Error: not found: $a2run" >&2; exit 1; }

frames=${FRAMES:-120}

# Compare one generated build against the interpreter on that build's own
# block heads. $1: label, $2: generated binary, $3: its C source, $4: where to
# keep the site list.
check_backend() {
  local label=$1 prog=$2 src=$3 sites=$4
  local interp="/tmp/probe-interp-$label.txt" gen="/tmp/probe-$label.txt"

  [ -x "$prog" ] || { echo "Error: not found: $prog" >&2; exit 1; }

  # Block heads, straight from the artifact that defines them. The list is
  # committed (see the commit message) so a failure is bisectable without
  # regenerating the C first, but it is still rebuilt here so it never goes
  # stale relative to the C actually being tested.
  grep -oE 'CYCLES\(0x[0-9a-f]+' "$src" | sed 's/CYCLES(0x//' | sort -u > "$sites"
  echo "[$label] site list: $(wc -l < "$sites") block heads"

  "$a2run" --frames="$frames" --probe="$here/$label.probe" \
    --probe-out="$interp" > /dev/null 2>"/tmp/probe-interp-$label.err"
  "$prog" --frames="$frames" --probe="$here/$label.probe" \
    --probe-out="$gen" > /dev/null 2>"/tmp/probe-$label.err"

  # A probe that never fired means the two sides did not cover the same set,
  # even if what they did emit matches -- e.g. a site both installed but
  # neither ever executed would leave two empty reports that diff clean.
  local side
  for side in "interp-$label" "$label"; do
    if grep -q "never fired" "/tmp/probe-$side.err"; then
      echo "FAIL [$side]: a probe never fired" >&2
      grep "never fired" "/tmp/probe-$side.err" >&2
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

# Each back end names its probe script after itself, so the two runs cannot
# silently share one. trace.probe is --simple-c's; tracec1.probe is --irc1's.
# They differ only in which site list they install from -- checked here rather
# than trusted, since an edit to one and not the other would leave both halves
# individually valid while quietly testing two different things.
strip_probe() {
  # Drop comments, blank lines and the install line, leaving the program.
  grep -vE '^\s*(#|install\b|$)' "$1"
}
if ! diff -q <(strip_probe "$here/trace.probe") <(strip_probe "$here/tracec1.probe") \
     > /dev/null; then
  echo "FAIL: trace.probe and tracec1.probe are not the same program" >&2
  diff <(strip_probe "$here/trace.probe") <(strip_probe "$here/tracec1.probe") >&2
  exit 1
fi

check_backend trace "$bin/decoded/rom/rom-run" "$here/rom.c" "$here/blocks.txt"
check_backend tracec1 "$bin/decoded/rom/romc1-run" "$here/romc1.c" "$here/blocksc1.txt"

if diff -q "$here/blocks.txt" "$here/blocksc1.txt" > /dev/null; then
  echo "the two back ends agree on the block-head set"
else
  echo "note: the back ends emit different block-head sets ($(wc -l < "$here/blocks.txt")" \
       "vs $(wc -l < "$here/blocksc1.txt")); each was checked against its own"
fi
