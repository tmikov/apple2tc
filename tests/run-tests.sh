#!/bin/bash

set -e

bin=../cmake-build-debug
a6502=$bin/tools/a6502/a6502
apple2tc=$bin/tools/apple2tc/apple2tc

$a6502 trees.s trees.b33 && $apple2tc trees.b33 -O3 --ir > trees-test.ir
diff -q trees.ir trees-test.ir
rm trees-test.ir trees.b33

$a6502 subs.s subs.b33 && $apple2tc --run-data=dclear.json -O3 --ir subs.b33 > subs-test.ir
diff -q subs.ir subs-test.ir
rm subs-test.ir subs.b33

echo "Success!"