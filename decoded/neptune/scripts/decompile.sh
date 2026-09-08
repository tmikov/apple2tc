#!/bin/bash
#
# Regenerate the Neptune disassembly listing and its coverage report:
#
#   as-generated/neptune.lst    the listing
#   testdata/coverage.txt       what is identified and what is not
#
# Usage: decompile.sh [build-dir]     (default ../../../cmake-build-debug)
#
# WHAT MAKES IT AS COMPLETE AS IT IS
#
# Three inputs, in order of how much they add:
#
#   the b33     entry at $3523 rather than the reset vector, so the trace
#               starts inside Neptune instead of inside the ROM boot
#   --run-data  the recorded play session; worth 88 code labels (997 -> 1085)
#               via 29 runtime blocks, which matters here because the collector
#               reports 21 generations -- this game rewrites its own code
#   --known-data  does not change the listing at all; it is the accounting that
#               turns "unknown" into a named region in the coverage report
#
# WHY THERE IS NO --code-at
#
# snake-byte needs one because its indirect jumps go somewhere the tracer
# cannot follow. Neptune's six in-game indirect jumps all dispatch through
# zero-page vectors, and no game code ever writes them -- the loader set them
# before the capture, so each has a single fixed value:
#
#   $11F0 JMP ($0200) -> $1D00     $20D4 JMP ($38) -> $2080
#   $1F9A JMP ($36)   -> $2000     $218F JMP ($3A) -> $2180
#   $2062 JMP ($36)   -> $2000     $2146 JMP ($4E) -> $2100
#
# All six targets are already inside identified code ranges, so hand-asserted
# edges would add nothing. If a future capture is taken at a different moment
# these values can differ, and this list is then the thing to re-derive rather
# than trust.
#
# THE SIX "invalid instruction" WARNINGS
#
# Expected, and not caused by the run data -- the count is 6 with it and 6
# without. They are the ordinary artifact of a jump-tracing disassembler
# following a computed branch into a table: e.g. $1185 is warned about, and
# $1184 is `BMI $11A8`, so $1185 is that instruction's operand byte rather than
# a new instruction. Six against 1085 code labels.

set -eu

here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
root=$(cd "$here/../../.." && pwd)
bin=${1:-$root/cmake-build-debug}

apple2tc=$bin/tools/apple2tc/apple2tc
b33=$here/../testdata/neptune.b33
rundata=$here/../testdata/neptune.json
knowndata=$here/../testdata/known-data.txt
lst=$here/../as-generated/neptune.lst
cov=$here/../testdata/coverage.txt
simplec=$here/../as-generated/neptune-simple-c.c

[ -x "$apple2tc" ] || { echo "$0: no apple2tc at $apple2tc" >&2; exit 1; }
for f in "$b33" "$rundata" "$knowndata"; do
  [ -r "$f" ] || { echo "$0: missing $f -- run make-b33.sh and collect.sh first" >&2; exit 1; }
done
mkdir -p "$(dirname "$lst")"

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

"$apple2tc" --asm \
  --run-data="$rundata" \
  --known-data="$knowndata" \
  --coverage="$cov" \
  "$b33" > "$lst" 2> "$tmp/err"

# The point of declaring data is that the claim can be wrong, and the coverage
# report is what catches it: a range declared here that the disassembler also
# reached as code means one of the two is mistaken. Never silently.
if grep -q "also decompiled as code" "$cov"; then
  echo "$0: known-data.txt declares a range that is also reached as code:" >&2
  grep -B2 -A2 "also decompiled as code" "$cov" >&2
  exit 1
fi

# Warnings are expected (see the header) but their number is not: a jump into a
# table is normal, a lot of them means the trace has gone wrong somewhere.
warnings=$(grep -c "invalid instruction" "$tmp/err" || true)
if [ "$warnings" -gt 12 ]; then
  echo "$0: $warnings 'invalid instruction' warnings, up from the 6 expected;" >&2
  echo "     the trace is following something it should not" >&2
  grep "invalid instruction" "$tmp/err" >&2
  exit 1
fi

# The --simple-c transliteration, which is built as neptune-as-generated. It is
# a separate invocation because it goes through the disassembler rather than the
# IR, so -O and --known-data mean nothing to it; only --run-data does.
#
# Generating it is not decoration. It is the only check that the disassembly is
# a correct *reading* of the game rather than a plausible-looking one: it
# compiles, it runs, and with no input it reaches $3535 with byte-identical RAM
# to the interpreter. That test found a real decompiler bug (dz 01a07eb4-5fa7,
# a truncated expression for self-modified zero-page indexed operands) that
# nothing else in the tree exercises.
"$apple2tc" --simple-c --run-data="$rundata" "$b33" > "$simplec" 2>/dev/null

# Truncated output was how that bug showed up, and it is cheap to keep looking
# for: an expression cut short leaves the line's parentheses unbalanced.
if awk '{ o = gsub(/\(/, "("); c = gsub(/\)/, ")"); if (o != c) { print NR": "$0; bad++ } }
        END { exit(bad ? 1 : 0) }' "$simplec" >&2; then
  :
else
  echo "$0: generated C has unbalanced parentheses -- an expression was truncated" >&2
  exit 1
fi

echo "wrote $lst"
echo "wrote $simplec"
echo "wrote $cov"
sed -n '/^range /,/unknown nonzero/p' "$cov"
echo "$warnings 'invalid instruction' warnings (6 expected; see the header)"
