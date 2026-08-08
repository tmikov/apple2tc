#!/bin/bash

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

#$apple2tc snake-byte.b33 --run-data=snake-byte.json --asm > snake-byte.lst
#$apple2tc snake-byte.b33 --run-data=snake-byte.json --simple-c > snake-byte.c

# Both variants get --code-at=code-at.txt: hand-asserted dynamic branch edges
# for code the recorded session never reached. See that file for the argument
# behind each edge. It is deliberately a separate input rather than an edit to
# snake-byte.json, which must stay a faithful recording.
#
# Two variants are generated, and both are built.
#
# snake-bytec1.c is self-contained: it decompiles the Apple II ROM along with
# the game, so it compiles and links on its own. It is the reference build --
# play.frames was recorded from it, and it is the control that the extern
# variant is checked against.
$apple2tc snake-byte.b33 --run-data=snake-byte.json --code-at=code-at.txt -O3 --irc1 -v1 \
  > snake-bytec1.c

# snake-bytec1-ext.c emits the ROM entry points listed in rom.externs as
# declarations only; the bodies are hand-written in a2rom.c. It does NOT link on
# its own -- snake-byte-ext.c is what actually gets compiled.
$apple2tc snake-byte.b33 --run-data=snake-byte.json --code-at=code-at.txt -O3 --irc1 -v1 \
  --extern-routines=rom.externs > snake-bytec1-ext.c
