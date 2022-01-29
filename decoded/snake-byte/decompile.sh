#!/bin/bash

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

#$apple2tc snake-byte.b33 --run-data=snake-byte.json --asm > snake-byte.lst
#$apple2tc snake-byte.b33 --run-data=snake-byte.json --simple-c > snake-byte.c
$apple2tc snake-byte.b33 --run-data=snake-byte.json -O3 --irc1 -v1 > snake-bytec1.c
