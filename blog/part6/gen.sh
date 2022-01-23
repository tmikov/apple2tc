#/bin/bash

set -e

bin=../../cmake-build-debug
a6502=$bin/tools/a6502/a6502
apple2tc=$bin/tools/apple2tc/apple2tc

#$a6502 ex.s ex.b33
#$apple2tc ex.b33 --no-ir-trees --ir -O0 > ex-1.s
#$apple2tc ex.b33 --no-ir-trees --ir -O1 > ex-2.s
#$apple2tc ex.b33 --no-ir-trees --ir -O2 > ex-3.s
#$apple2tc ex.b33 --no-ir-trees --ir -O3 > ex-4.s
#$apple2tc ex.b33 --ir -O3 > ex-5.s
$apple2tc ex.b33 --ir -O3 > ex-6.s
