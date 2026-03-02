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

if [ ! -x "$a6502" ] || [ ! -x "$apple2tc" ]; then
  echo "Error: required binaries not found in '$bin'" >&2
  echo "  Expected: $a6502" >&2
  echo "  Expected: $apple2tc" >&2
  exit 1
fi

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

echo "Success!"
