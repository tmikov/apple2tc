#!/bin/bash

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

#$apple2tc --rom apple2plus.rom --run-data=run.json > rom.lst
$apple2tc --rom apple2plus.rom --run-data=run.json --simple-c -O3 -v1 > rom.c
#$apple2tc --rom apple2plus.rom --run-data=run.json --ir -O3 -v1 --ret-addr > rom.s
#
# romc1.c (the --irc1 back end) cannot currently be regenerated: apple2tc
# crashes on this input -- "CPURegLiveness.h:89: Assertion 'it !=
# funcData_.end()' failed" -- reproducibly, and pre-existing (it reproduces
# identically before the cycle-accounting work of 2026-08-12/13). Left
# commented out and romc1.c left stale until that crash is fixed elsewhere;
# see docs/decompiling/decision-log.md.
#$apple2tc --rom apple2plus.rom --run-data=run.json --irc1 -O3 -v1 --ret-addr > romc1.c
