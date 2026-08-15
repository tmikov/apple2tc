#!/bin/bash

# --ret-addr: the emulated stack holds the return address a 6502 JSR would have
# pushed, rather than the $FFFE sentinel. A verification setting, not something
# a shipped decompilation wants -- see decoded/snake-byte/decompile.sh for the
# full argument and for what a final artifact should use instead.

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

#$apple2tc robotron2084.b33 --run-data=robotron.json --simple-c > robotron.c
#$apple2tc robotron2084.b33 --run-data=robotron.json --asm > robotron.lst
#$apple2tc robotron2084.b33 --run-data=robotron.json -O3 --ir -v2 > robotron.s
$apple2tc robotron2084.b33 --run-data=robotron.json -O3 --irc1 --ret-addr -v1 > robotronc1.c
