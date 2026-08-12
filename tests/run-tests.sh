#!/bin/bash

set -e

if [ -z "$1" ]; then
  echo "Usage: $0 <build-dir>" >&2
  exit 1
fi

bin=$1

if [ ! -d "$bin" ]; then
  echo "Error: build directory '$bin' not found" >&2
  exit 1
fi

a6502=$bin/tools/a6502/a6502
apple2tc=$bin/tools/apple2tc/apple2tc
id=$bin/tools/id/id
a2run=$bin/tools/a2run/a2run
a2emu=$bin/tools/a2emu/a2emu

for tool in "$a6502" "$apple2tc" "$id" "$a2run" "$a2emu"; do
  if [ ! -x "$tool" ]; then
    echo "Error: required binary not found: $tool" >&2
    exit 1
  fi
done

$a6502 trees.s trees.b33 && $apple2tc trees.b33 -O3 --ir > trees-test.ir
diff -q trees.ir trees-test.ir
rm trees-test.ir trees.b33

$a6502 trees1.s trees1.b33 && $apple2tc trees1.b33 -O3 --ir > trees1-test.ir
diff -q trees1.ir trees1-test.ir
rm trees1-test.ir trees1.b33

$a6502 subs.s subs.b33 && $apple2tc --run-data=dclear.json -O3 --ir subs.b33 > subs-test.ir
diff -q subs.ir subs-test.ir
rm subs-test.ir subs.b33

$a6502 ands.s ands.b33 && $apple2tc -O3 --ir ands.b33 > ands-test.ir
diff -q ands.ir ands-test.ir
rm ands-test.ir ands.b33

$a6502 func.s func.b33 && $apple2tc -O3 --ir func.b33 > func-test.ir
diff -q func.ir func-test.ir
rm func-test.ir func.b33

$a6502 phapla.s phapla.b33 && $apple2tc -O3 --ir phapla.b33 > phapla-test.ir
diff -q phapla.ir phapla-test.ir
rm phapla-test.ir phapla.b33

$a6502 stackmerge.s stackmerge.b33 && $apple2tc -O3 --ir stackmerge.b33 > stackmerge-test.ir
diff -q stackmerge.ir stackmerge-test.ir
rm stackmerge-test.ir stackmerge.b33

# --code-at: hand-asserted dynamic branch edges. Two baselines, so this pins
# down what the option changes rather than merely that it runs -- without the
# edges neither continuation in codeat.s is reachable at all.
$a6502 codeat.s codeat.b33 && $apple2tc codeat.b33 --code-at=codeat.txt -O3 --ir > codeat-test.ir
diff -q codeat.ir codeat-test.ir
$apple2tc codeat.b33 -O3 --ir > codeat-noedges-test.ir
diff -q codeat-noedges.ir codeat-noedges-test.ir
rm codeat-test.ir codeat-noedges-test.ir

# Each --code-at rejection, asserted to actually reject. A check nobody has
# watched fail is not a check.
expect_reject() {
  # $1: what is wrong with it, $2: contents of the --code-at file
  printf '%s\n' "$2" > codeat-bad.txt
  if $apple2tc codeat.b33 --code-at=codeat-bad.txt -O3 --ir >/dev/null 2>codeat-bad.err; then
    echo "FAIL: --code-at accepted $1" >&2
    exit 1
  fi
  if ! grep -q 'FATAL' codeat-bad.err; then
    echo "FAIL: --code-at rejected $1, but without a FATAL diagnostic:" >&2
    cat codeat-bad.err >&2
    exit 1
  fi
  rm -f codeat-bad.txt codeat-bad.err
}

expect_reject "an origin that is not a branch"  "0300 0330"
expect_reject "an origin inside an instruction" "0301 0330"
expect_reject "a line with only one address"    "0306"
expect_reject "a malformed hex address"         "030g 0330"
expect_reject "an address wider than 16 bits"   "0306 10330"

# A target pointing at data cannot be rejected -- that untraced bytes are code
# is precisely what the option asserts -- but one that does not decode warns.
printf '0306 0328\n' > codeat-bad.txt
$apple2tc codeat.b33 --code-at=codeat-bad.txt -O3 --ir >/dev/null 2>codeat-bad.err
if ! grep -q "does not decode to a valid instruction" codeat-bad.err; then
  echo "FAIL: --code-at did not warn about a target pointing into filler bytes" >&2
  exit 1
fi
# ...and the good file must stay silent, or the warning is noise.
$apple2tc codeat.b33 --code-at=codeat.txt -O3 --ir >/dev/null 2>codeat-bad.err
if grep -q "code-at" codeat-bad.err; then
  echo "FAIL: --code-at warned about codeat.txt, which is correct:" >&2
  cat codeat-bad.err >&2
  exit 1
fi
rm -f codeat-bad.txt codeat-bad.err codeat.b33

# A subroutine entry that is also the return point of a call. The RTS returning
# into it used to be rejected as an invalid predecessor, which also took the
# routine calling it down through the invalid-JSR cascade.
$a6502 retpoint.s retpoint.b33 && $apple2tc retpoint.b33 --code-at=retpoint.txt -O3 --ir \
  --routines-report=retpoint-report.txt > retpoint-test.ir
diff -q retpoint.ir retpoint-test.ir
# Said directly, so a regression reads as more than an unexplained baseline diff.
if grep -q "^REJECT" retpoint-report.txt; then
  echo "FAIL: every routine in retpoint.s should be recovered, but:" >&2
  grep "^REJECT" retpoint-report.txt >&2
  exit 1
fi
rm retpoint-test.ir retpoint-report.txt retpoint.b33

# `id`'s cross-reference search, over a target reached four different ways.
# The indirect jump is deliberately *not* expected as a code hit -- it names the
# vector, not the target -- while the vector itself is what the data scan is for.
$a6502 xref.s xref.b33 && printf 'loadb33 xref.b33\nxref $310 $31F $300 $321\n' \
  | $id > xref-test.txt 2>&1
diff -q xref.expected xref-test.txt
rm xref-test.txt xref.b33

# The coverage report, with one region of each kind it classifies. The
# overlap warning is checked separately below, since a declared range that the
# disassembler also reached as code means one of the two claims is wrong.
$a6502 coverage.s coverage.b33 && $apple2tc coverage.b33 --known-data=coverage-data.txt \
  -O3 --ir --coverage=coverage-test.txt > /dev/null
diff -q coverage.expected coverage-test.txt
rm coverage-test.txt

# Declaring code as data has to be noticed, not silently believed.
printf '0300 0304 wrong\n' > coverage-bad.txt
$apple2tc coverage.b33 --known-data=coverage-bad.txt -O3 --ir --coverage=coverage-test.txt \
  > /dev/null
if ! grep -q "also decompiled as code" coverage-test.txt; then
  echo "FAIL: --coverage did not flag a data range that overlaps code" >&2
  exit 1
fi
rm coverage-test.txt coverage-bad.txt coverage.b33

# a2emu and a2run are the same host and the same engine, differing only in that
# one has a window. Headless they must agree exactly -- which is the regression
# test for a2emu, a GUI program that otherwise has none. Booting the ROM is
# enough; no game data is needed, which keeps this out of decoded/.
$a2run --frames=40 --hash-frames=frontend-run.txt > /dev/null
$a2emu --headless --frames=40 --hash-frames=frontend-emu.txt > /dev/null
if ! diff -q frontend-run.txt frontend-emu.txt > /dev/null; then
  echo "FAIL: a2emu and a2run disagree headless; they share everything but the window" >&2
  diff frontend-run.txt frontend-emu.txt | head -5 >&2
  exit 1
fi
rm frontend-run.txt frontend-emu.txt

# --- Probes -----------------------------------------------------------------
#
# The probe compiler is tested through --probe-dump, which compiles a script,
# prints the compiled form and exits without running anything. Every case below
# is a script/baseline pair; a change in generated code shows up as a diff.

mkdir -p probe-tmp

# Compile a script and diff the result against its baseline.
probe_dump_test() {
  # $1: base name under probe/
  if ! $a2run --probe="probe/$1.probe" --probe-dump > "probe-tmp/$1.txt"; then
    echo "FAIL: --probe-dump failed on probe/$1.probe" >&2
    exit 1
  fi
  diff -q "probe/$1.expected" "probe-tmp/$1.txt"
}

probe_dump_test empty

# Assert that a2run rejects something, with the specific diagnostic we expect.
# Matching the exact message rather than just "FATAL" is deliberate: an earlier
# version of these tests passed because a different check fired first.
expect_probe_reject() {
  # $1: description, $2: expected substring, $3...: a2run arguments
  local desc="$1" want="$2"
  shift 2
  if $a2run "$@" > /dev/null 2>probe-tmp/err.txt; then
    echo "FAIL: a2run accepted $desc" >&2
    exit 1
  fi
  if ! grep -q "FATAL" probe-tmp/err.txt || ! grep -q -- "$want" probe-tmp/err.txt; then
    echo "FAIL: rejected $desc, but not with the expected diagnostic '$want':" >&2
    cat probe-tmp/err.txt >&2
    exit 1
  fi
}

expect_probe_reject "a missing probe script" "cannot open probe script" \
  --probe=probe/does-not-exist.probe --probe-dump
expect_probe_reject "--probe-dump with no script" "--probe-dump requires" \
  --probe-dump
expect_probe_reject "--probe-out with no script" "--probe-out requires" \
  --probe-out=probe-tmp/out.txt

rm -rf probe-tmp

echo "Success!"
