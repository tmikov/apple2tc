#!/bin/bash
#
# The acceptance test for probes: the interpreter and the decompiled ROM must
# produce byte-identical probe reports over a boot.
#
# Why this test and not a new one. 1,991 of 1,991 branch targets already match
# between these two over 120 frames of ROM boot -- measured 2026-08-08, with a
# bespoke trace comparison. Reproducing that number through probes validates
# the probe mechanism against a result we already believe. A disagreement here
# means the probes are wrong, not the decompiler.
#
# The site list is derived from the generated C rather than from apple2tc,
# because CYCLES is emitted per basic block: grepping it out of rom.c yields
# exactly the addresses the generated program *can* probe. Installing at any
# other address would fire in the interpreter and silently not exist on the
# other side -- which is the one failure mode this design cannot otherwise
# see.
#
# rom.c, not romc1.c. This test was originally switched to rom.c because the
# --irc1 back end that produces romc1.c crashed apple2tc on this input, leaving
# romc1.c stale. That crash is fixed (see decision-log.md, 2026-08-13) and
# romc1.c is regenerated, so the constraint no longer applies -- rom.c is kept
# only because the site list must come from the same back end being probed.
# Both back ends emit one CYCLES() call per basic block into an otherwise
# equivalent switch-dispatched engine, so grepping block heads out of rom.c and
# probing rom-run tests the same thing in substance: whether the interpreter and
# a generated build agree on control flow and dwell time under the shared cycle
# table.
#
# Probing romc1-run instead would need its own site list grepped from romc1.c,
# because --irc1 runs simplifyCFG and so has coarser blocks. That is a genuine
# extra check -- romc1 is the only build exercising the system2-inc.h trampoline
# engine -- and is left as future work rather than folded in here silently.

set -e

if [ -z "$1" ]; then
  echo "Usage: $0 <build-dir>" >&2
  exit 1
fi

bin=$1
a2run="$bin/tools/a2run/a2run"
romgen="$bin/decoded/rom/rom-run"
here=$(dirname "$0")

for tool in "$a2run" "$romgen"; do
  [ -x "$tool" ] || { echo "Error: not found: $tool" >&2; exit 1; }
done

frames=${FRAMES:-120}

# Block heads, straight from the artifact that defines them. blocks.txt is
# committed (see the commit message) so a failure is bisectable without
# rebuilding rom.c first, but it is still regenerated here so it never goes
# stale relative to the rom.c actually being tested.
grep -oE 'CYCLES\(0x[0-9a-f]+' "$here/rom.c" \
  | sed 's/CYCLES(0x//' | sort -u > "$here/blocks.txt"
echo "site list: $(wc -l < "$here/blocks.txt") block heads"

"$a2run" --frames="$frames" --probe="$here/trace.probe" \
  --probe-out=/tmp/probe-interp.txt > /dev/null 2>/tmp/probe-interp.err
"$romgen" --frames="$frames" --probe="$here/trace.probe" \
  --probe-out=/tmp/probe-gen.txt > /dev/null 2>/tmp/probe-gen.err

# A probe that never fired means the two sides did not cover the same set,
# even if what they did emit matches -- e.g. a site both installed but
# neither ever executed would leave two empty reports that diff clean.
for side in interp gen; do
  if grep -q "never fired" "/tmp/probe-$side.err"; then
    echo "FAIL [$side]: a probe never fired" >&2
    grep "never fired" "/tmp/probe-$side.err" >&2
    exit 1
  fi
done

if ! diff -q /tmp/probe-interp.txt /tmp/probe-gen.txt > /dev/null; then
  echo "FAIL: the two engines disagree" >&2
  diff /tmp/probe-interp.txt /tmp/probe-gen.txt | head -20 >&2
  echo "interp hits: $(wc -l < /tmp/probe-interp.txt)" >&2
  echo "gen hits:    $(wc -l < /tmp/probe-gen.txt)" >&2
  exit 1
fi

echo "PASS: $(wc -l < /tmp/probe-interp.txt) probe hits match over $frames frames"
