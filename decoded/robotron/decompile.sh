#!/bin/bash

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

#$apple2tc robotron2084.b33 --run-data=robotron.json --simple-c > robotron.c
#$apple2tc robotron2084.b33 --run-data=robotron.json --asm > robotron.lst
#$apple2tc robotron2084.b33 --run-data=robotron.json -O3 --ir -v2 > robotron.s
$apple2tc robotron2084.b33 --run-data=robotron.json -O3 --irc1 -v1 > /dev/null
