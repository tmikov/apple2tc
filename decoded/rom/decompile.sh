#!/bin/bash

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

#$apple2tc --rom apple2plus.rom --run-data=run.json > rom.lst
#$apple2tc --rom apple2plus.rom --run-data=run.json --simple-c > rom.c
#$apple2tc --rom apple2plus.rom --run-data=run.json --ir -O3 -v1 > rom.s
$apple2tc --rom apple2plus.rom --run-data=run.json --irc1 -O3 -v1 > romc1.c
