#!/bin/bash
#
# KNOWN WART, measured 2026-08-23: this script does not reproduce the committed
# generated C. Running it at a clean HEAD, with no source change at all,
# rewrites snake-bytec1-ext.c and snake-byte-easyc1-ext.c by ~12,000 lines
# each.
#
# The difference is line breaking only. Strip whitespace from both and they are
# byte-identical, so the committed files are semantically current -- what is
# stale is their layout, against an emitter whose line breaking has moved since
# they were last written. Nothing is wrong with the checked-in C.
#
# It matters because it makes any real regeneration unreadable: a one-line
# change arrives buried in a 12,000-line reformat. Until someone regenerates
# and commits the reformat on its own, prefer not to regenerate for cosmetic
# reasons, and check `git diff -w` plus a whitespace-stripped `cmp` before
# believing a large diff means a large change.

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

#$apple2tc snake-byte.b33 --run-data=snake-byte.json --asm > snake-byte.lst
#$apple2tc snake-byte.b33 --run-data=snake-byte.json --simple-c > snake-byte.c

# Both variants get --code-at=code-at.txt: hand-asserted dynamic branch edges
# for code the recorded session never reached. See that file for the argument
# behind each edge. It is deliberately a separate input rather than an edit to
# snake-byte.json, which must stay a faithful recording.
#
# The coverage accounting is regenerated alongside the C, so it cannot go stale.
# known-data.txt declares the identified non-code regions; the report flags any
# of them that the disassembler also reached as code.
# --ret-addr, on every --irc1 line below, is a VERIFICATION setting and not
# something a shipped decompilation should carry.
#
# Without it every JSR pushes the sentinel $FFFE instead of a return address,
# because a generated RTS is a C `return` and has no use for the real one. That
# is cheap and almost always harmless -- but "almost" is doing work: the
# inline-data-after-JSR idiom (see HANDOFF.md's traps table; Snake Byte uses it
# for the "VALUE: " string) finds its data by *reading* the pushed address, and
# under $FFFE it would read from the wrong place. With --ret-addr the emulated
# stack holds what the 6502 would have pushed, so that class of code is right
# rather than accidentally-unexercised.
#
# It also makes the stack comparable across engines, which is what turned the
# last unexplained residue in the interpreter-vs-generated comparison into a
# measured zero (decision log, 2026-08-15): live stack identical at all 6,808
# in-game samples, against 27,232 differing bytes before.
#
# What a final artifact wants instead is ret_addr == 0 -- no emulated stack
# maintenance at all, since native C does not need it. That is a separate
# setting from this one, and this comment exists so the distinction is not lost
# the next time someone regenerates.
$apple2tc snake-byte.b33 --run-data=snake-byte.json --code-at=code-at.txt \
  --known-data=known-data.txt -O3 --irc1 --ret-addr --coverage=coverage.txt > /dev/null

# Two variants are generated, and both are built.
#
# snake-bytec1.c is self-contained: it decompiles the Apple II ROM along with
# the game, so it compiles and links on its own. It is the reference build --
# play.frames was recorded from it, and it is the control that the extern
# variant is checked against.
$apple2tc snake-byte.b33 --run-data=snake-byte.json --code-at=code-at.txt -O3 --irc1 --ret-addr -v1 \
  > snake-bytec1.c

# snake-bytec1-ext.c emits the ROM entry points listed in rom.externs as
# declarations only; the bodies are hand-written in a2rom.c. It does NOT link on
# its own -- snake-byte-ext.c is what actually gets compiled.
#
# --alt-exit is on this line and not the two above it. It identifies routines
# that return by discarding their return address and jumping into the caller --
# $6A32 does that at $6AB3 -- and turns them into C functions returning which
# exit they took. The reference build stays without it so that the two variants
# differ by more than one flag in only one direction, and so the control this
# is checked against is not itself carrying the new transform.
$apple2tc snake-byte.b33 --run-data=snake-byte.json --code-at=code-at.txt -O3 --irc1 --ret-addr -v1 \
  --extern-routines=rom.externs --inline-str=inline-str.txt --alt-exit --prune-returns > snake-bytec1-ext.c

# The `easy` fixture. snake-byte-easy.b33 is snake-byte.b33 with the per-level
# apple quota lowered from 16 to 2 (see make-easy.sh for the two bytes and why
# there are two). It exists so the display-list interpreter at $7113 -- and the
# 'H' and 'V' cases that draw every level's interior walls -- can be compared
# across engines at all: both committed recordings stop on level 1, where the
# script is just `T $64`, so the rest of that subsystem was verified by nothing.
#
# Only the extern variant is generated. probe-acceptance.sh compares it against
# the *interpreter*, which is ground truth and a stricter control than the
# reference build; a second generated pair would cost another ~900KB of
# committed C to prove something weaker.
#
# The run-data is snake-byte.json, unchanged: the patch alters one immediate
# operand and one data byte, so every code address is where the recording says
# it is. The generated C differs from the stock build in exactly the three
# places those two bytes appear.
$apple2tc snake-byte-easy.b33 --run-data=snake-byte.json --code-at=code-at.txt -O3 --irc1 --ret-addr -v1 \
  --extern-routines=rom.externs --inline-str=inline-str.txt --alt-exit --prune-returns > snake-byte-easyc1-ext.c
