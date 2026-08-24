#!/bin/bash
#
# Regenerate boot-state.txt: the Apple II's machine state at the BASIC prompt,
# after boot, with nothing loaded.
#
# Usage: make-boot-state.sh [build-dir]     (default ../../cmake-build-debug)
#
# The capture is one a2run invocation; everything else here is formatting. The
# point of the formatting is that unwritten memory is not state: RAM comes up
# as $FF and the text screen as $A0, so only departures from those fills are
# listed, and the screen is shown as the 24 rows it displays rather than as
# addresses.

set -eu

here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
bin=${1:-$here/../../cmake-build-debug}
a2run=$bin/tools/a2run/a2run

[ -x "$a2run" ] || { echo "$0: no a2run at $a2run" >&2; exit 1; }

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

# $FD1B is KEYIN's wait loop: the first time the machine asks for a keystroke
# is the moment boot has finished and nothing else has happened yet.
"$a2run" --frames=200 --snapshot-at=FD1B --snapshot-out="$tmp/boot.bin" > /dev/null 2>&1

# One decimal byte value per line, in address order. -v keeps od from
# collapsing repeated lines, which would silently shift every address after
# the first run of identical bytes.
od -An -tu1 -v "$tmp/boot.bin" | tr -s ' ' '\n' | grep -v '^$' > "$tmp/bytes"

# "ADDR NAME" for everything below $0800 in the built-in symbol database.
sed -n 's/.*{0x\([0-9A-Fa-f]\{4\}\), *"\([^"]*\)".*/\1 \2/p' \
  "$here/../../lib/d6502/a2symbols.cpp" > "$tmp/syms"

awk -v regs="$tmp/boot.bin.regs" -v syms="$tmp/syms" '
# mawk has neither strtonum() nor and(), and does not parse 0x literals, so
# hex arrives as text and is converted here, and every constant below is
# decimal. Written for the awk that is actually installed rather than for gawk.
function hex(s,   i, c, v, d) {
  v = 0
  s = toupper(s)
  for (i = 1; i <= length(s); i++) {
    c = substr(s, i, 1)
    d = index("0123456789ABCDEF", c) - 1
    if (d < 0) continue
    v = v * 16 + d
  }
  return v
}
BEGIN {
  while ((getline line < regs) > 0) { split(line, f, " "); reg[f[1]] = f[2] }
  while ((getline line < syms) > 0) {
    split(line, f, " ")
    a = hex(f[1])
    if (a < 2048) sym[a] = f[2]
  }
}
{ b[NR-1] = $1 }
END {
  print "# Apple II+ machine state at the BASIC prompt, after boot, with no program"
  print "# loaded. Game-independent: this is the floor any program started from the"
  print "# prompt begins on."
  print "#"
  print "# WHY IT EXISTS"
  print "#"
  print "# A decompiled game starts at the reset vector and boots through the ROM,"
  print "# because that is what the recording did -- which makes every byte of ROM the"
  print "# boot path touches reachable code the decompiler must emit. For Snake Byte"
  print "# that is 1074 of 1171 tracked sites, against 97 for the game itself."
  print "#"
  print "# Retargeting the entry to the start address of the program itself deletes all"
  print "# of it (measured for Snake Byte: 1,530 blocks deleted, leaving 4). The obstacle"
  print "# is that the program does not start on a blank machine: boot set up the text"
  print "# window, the output vector, the stack and part of zero page, and the program"
  print "# reads all of that without ever writing it."
  print "#"
  print "# This is that state, recorded once."
  print "#"
  print "# REGENERATE WITH"
  print "#"
  print "#   decoded/rom/make-boot-state.sh [build-dir]"
  print "#"
  print "# which captures with:"
  print "#"
  print "#   a2run --frames=200 --snapshot-at=FD1B --snapshot-out=boot.bin"
  print "#"
  print "# $FD1B is the KEYIN wait loop: the first time the machine asks for a keystroke"
  print "# is the moment boot has finished and nothing else has happened. The snapshot"
  print "# is one-shot and fires on first arrival; it also writes boot.bin.regs."
  print "#"
  print "# WHAT IS DELIBERATELY NOT LISTED"
  print "#"
  print "#   Unwritten memory. RAM comes up as $FF and the text screen as $A0. Only"
  print "#   departures from those fills are below. The rest is not state, it is what"
  print "#   unwritten memory happens to read as."
  print "#"
  print "#   $D000-$FFFF, ROM. Identical by construction."
  print "#"
  print "# CAVEAT"
  print "#"
  print "# This is the state at the prompt, before anything is typed. A program reached"
  print "# by typing a command has had it echoed and parsed, which edits the input"
  print "# buffer and moves the cursor. Those deltas are per-program; this is the common"
  print "# floor underneath them."
  print ""

  sp = hex(reg["sp"])

  print "REGISTERS at $" toupper(reg["pc"])
  printf "  A=%s  X=%s  Y=%s  SP=%s  STATUS=%s   (cycles since reset: %s)\n\n",
         toupper(reg["a"]), toupper(reg["x"]), toupper(reg["y"]),
         toupper(reg["sp"]), toupper(reg["status"]), reg["cycles"]

  print "SOFT SWITCHES"
  print "-------------"
  print "  These are not memory -- they live in the host IO emulation, so no RAM"
  print "  image carries them. A rebased entry point must set them explicitly."
  print ""
  printf "  vid_control = $%s   %s, page %s%s\n",
         toupper(reg["vid_control"]), reg["vidmode"], reg["vidpage"],
         (reg["mixed"] == "1" ? ", mixed" : ", not mixed")
  printf "  keyboard      last key read $%s, %s queued\n",
         toupper(reg["kbd_last_key"]), reg["kbd_queued"]
  print ""
  print "  Text, page 1, not mixed is the power-on state: the ROM never touches the"
  print "  graphics switches on a boot that reaches the prompt."
  print ""

  n = 0
  for (a = 0; a < 256; a++) if (b[a] != 255) n++
  printf "ZERO PAGE -- $FF except these %d bytes\n", n
  print "------------------------------------------------------"
  for (a = 0; a < 256; a++)
    if (b[a] != 255)
      printf "  $%02X = $%02X   %s\n", a, b[a], (a in sym ? sym[a] : "")

  printf "\nSTACK -- SP=$%02X, so $%04X-$01FF is live (%d bytes). Below SP is dead:\n", sp, 257 + sp, 255 - sp
  print "popped and never read again, and its residue differs between engines."
  print "------------------------------------------------------"
  printf "  "
  for (a = 257 + sp; a <= 511; a++) printf "%s%02X", (a == 257 + sp ? "" : " "), b[a]
  print ""

  n = 0
  for (a = 512; a < 1024; a++) if (b[a] != 255) n++
  printf "\n$0200-$03FF -- $FF except these %d bytes\n", n
  print "------------------------------------------------------"
  for (a = 512; a < 1024; a++)
    if (b[a] != 255)
      printf "  $%04X = $%02X   %s\n", a, b[a], (a in sym ? sym[a] : "")

  n = 0
  for (a = 1024; a < 2048; a++) if (b[a] != 160) n++
  printf "\nTEXT SCREEN $0400-$07FF -- $A0 (space) except the banner, %d bytes.\n", n
  print "Shown as the 24 rows it displays; every character is ASCII with bit 7 set."
  print "Row r starts at $400 + (r%8)*$80 + (r/8)*$28."
  print "------------------------------------------------------"
  for (r = 0; r < 24; r++) {
    base = 1024 + (r % 8) * 128 + int(r / 8) * 40
    line = ""
    for (c = 0; c < 40; c++) {
      v = b[base + c] % 128
      line = line ((v >= 32 && v < 127) ? sprintf("%c", v) : ".")
    }
    printf "  %2d $%04X |%s|\n", r, base, line
  }

  printf "\n$0800-$0802 -- $%02X $%02X $%02X. From $0803 to $BFFF every byte is $FF.\n",
         b[2048], b[2049], b[2050]
}
' "$tmp/bytes" > "$here/boot-state.txt"

# The same state as C data, for a program that starts past the boot instead of
# performing it. boot-state.txt is for reading; this is for including.
{
  echo "/* Generated by make-boot-state.sh. Do not edit. */"
  echo "/* Apple II+ machine state at the BASIC prompt; see boot-state.txt. */"
  echo "#pragma once"
  echo "#include <stdint.h>"
  echo ""
  echo "/* RAM \$0000-\$0802. Everything from \$0803 to \$BFFF is \$FF. */"
  echo "#define A2_BOOT_RAM_LEN 0x803"
  echo "static const uint8_t kA2BootRam[A2_BOOT_RAM_LEN] = {"
  od -An -tu1 -v -N 2051 "$tmp/boot.bin" |
    awk '{ for (i = 1; i <= NF; i++) printf "%s%s,", (n++ % 16 == 0 ? "  " : " "), $i;
           print "" }'
  echo "};"
  echo ""
  echo "/* Everything outside RAM: registers, and the soft switches a RAM image"
  echo "   cannot carry. */"
  awk '{ n = toupper($1); v = $2
         if (n == "PC" || n == "A" || n == "X" || n == "Y" || n == "SP" || n == "STATUS")
           printf "#define A2_BOOT_%s 0x%s\n", n, v
         else if (n == "VID_CONTROL")
           printf "#define A2_BOOT_VID_CONTROL 0x%s\n", v }' "$tmp/boot.bin.regs"
} > "$here/boot-state-inc.h"

echo "wrote $here/boot-state.txt and $here/boot-state-inc.h"
