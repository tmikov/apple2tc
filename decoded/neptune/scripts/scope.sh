#!/bin/bash
#
# Step 1 of docs/decompiling/playbook.md, "Scope the boundary", for Neptune.
#
# Usage: scope.sh [build-dir]     (default ../../../cmake-build-debug)
#
# Prints the three numbers that step asks for, all derived from
# as-generated/neptune.lst rather than remembered:
#
#   - instructions inside the game's own range against the total
#   - the distinct ROM entry points the game calls
#   - the hardware surface: which $C0xx addresses it touches
#
# WHY IT IS A SCRIPT
#
# The playbook's own rule is to derive the set rather than trust a list written
# earlier, and the boundary is a decision made from these numbers. Anything
# that changes the capture point, the recording or the asserted edges changes
# them, and a claim in a README that cannot be re-derived rots silently.
#
# One thing this deliberately does not do is count `Code range` markers. The
# playbook is explicit that --asm prints one only at a discontinuity, so they
# measure how fragmented the code is and not how much of it there is; the
# counts below come from the instruction stream.

set -eu

here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
root=$(cd "$here/../../.." && pwd)
lst=$here/../as-generated/neptune.lst
rundata=$here/../testdata/neptune.json

[ -r "$lst" ] || { echo "$0: no listing at $lst -- run decompile.sh first" >&2; exit 1; }

# mawk has neither strtonum() nor 0x literals, so hex is converted here. Same
# reason decoded/rom/make-boot-state.sh carries its own.
awk '
function hex(s,   i, d, v) {
  v = 0; s = toupper(s)
  for (i = 1; i <= length(s); i++) {
    d = index("0123456789ABCDEF", substr(s, i, 1)) - 1
    if (d >= 0) v = v * 16 + d
  }
  return v
}

# Instruction lines look like:   /*3802*/   LDA   M_B0,X    ; $00B0
# Data lines carry DFB/DW and are skipped; so are org/label-only lines.
/^\/\*[0-9A-F][0-9A-F][0-9A-F][0-9A-F]\*\// {
  addr = hex(substr($0, 3, 4))
  mn = ""
  for (i = 1; i <= NF; i++)
    if ($i ~ /^[A-Z][A-Z][A-Z]$/) { mn = $i; break }
  if (mn == "" || mn == "DFB" || mn == "DW") next

  total++
  if (addr < 49152) game++; else if (addr >= 53248) rom++

  # The trailing "; $XXXX" is the resolved operand, which is what we want:
  # it is already past any symbol the listing substituted.
  tgt = -1
  if (match($0, /; \$[0-9A-F][0-9A-F][0-9A-F][0-9A-F] *$/))
    tgt = hex(substr($0, RSTART + 3, 4))
  if (tgt < 0) next

  # A call or jump from the game into ROM crosses the boundary.
  if (addr < 49152 && tgt >= 53248 && (mn == "JSR" || mn == "JMP")) {
    if (!(tgt in romsites)) romentry[++nrom] = tgt
    romsites[tgt]++
  }
  # Anything the game addresses in $C000-$CFFF is hardware.
  if (addr < 49152 && tgt >= 49152 && tgt < 53248) {
    if (!(tgt in iosites)) ioaddr[++nio] = tgt
    iosites[tgt]++
  }
}

END {
  printf "instructions disassembled: %d\n", total
  printf "  in Neptune  $0000-$BFFF : %5d  (%.1f%%)\n", game, 100 * game / total
  printf "  in ROM      $D000-$FFFF : %5d  (%.1f%%)\n", rom, 100 * rom / total

  printf "\ndistinct ROM entry points called from the game: %d\n", nrom
  for (i = 1; i <= nrom; i++) {
    a = romentry[i]
    printf "  $%04X   %d call site(s)\n", a, romsites[a]
  }

  printf "\nhardware surface: %d distinct $C0xx addresses\n", nio
  for (i = 1; i <= nio; i++) {
    a = ioaddr[i]
    printf "  $%04X   %d site(s)\n", a, iosites[a]
  }
}
' "$lst"

# The hardware surface above is a LOWER BOUND, and the difference matters.
#
# It comes from the disassembly, and the disassembly only covers code the
# tracer reached. Neptune decides its input device once at the title screen and
# patches three sites accordingly; the paddle arm is never taken here, so the
# paddle reader at $36B0 is never decoded, and a surface read off the listing
# silently omits $C070 and $C064 -- which is exactly what happened, and got
# committed, before this section existed.
#
# So scan the image as well. This over-reports: any three bytes that look like
# an absolute-addressed opcode count, including data. That is the right
# direction for a bound to err in, and anything it finds that the listing did
# not is a pointer at untraced code rather than a conclusion.
b33=$here/../testdata/neptune.b33
if [ -r "$b33" ]; then
  echo
  echo "hardware surface from the image, including untraced code (upper bound):"
  od -An -tu1 -v -j4 "$b33" | tr -s ' ' '\n' | grep -v '^$' | awk '
    { b[NR - 1] = $1 }
    END {
      # Opcodes taking an absolute operand that could touch $C0xx. Missing one
      # here is a silent hole, so this errs wide: every absolute load, store,
      # compare and ALU op, indexed or not. AND abs ($2D) is the cautionary
      # case -- it is how $3587 reads the second button, and leaving it out
      # dropped $C062 from a scan that otherwise looked complete.
      split("173 141 44 205 236 204 172 174 " \
            "13 45 77 109 237 " \
            "189 157 185 153 190 188 " \
            "29 61 93 125 253 25 57 89 121 249 " \
            "14 78 46 110 238 206", o, " ")
      for (k in o) op[o[k]] = 1
      for (i = 0; i + 2 < NR; i++) {
        if (!(b[i] in op)) continue
        t = b[i + 1] + b[i + 2] * 256
        if (t >= 49152 && t < 49408 && !(t in seen)) { seen[t] = 1; list[++n] = t }
      }
      for (i = 1; i <= n; i++)
        for (j = i + 1; j <= n; j++)
          if (list[j] < list[i]) { tmp = list[i]; list[i] = list[j]; list[j] = tmp }
      for (i = 1; i <= n; i++) printf "  $%04X\n", list[i]
      printf "  (%d distinct)\n", n
    }'
fi

# The listing is a static trace, so it reaches ROM the recording never did --
# following a BRK into the interrupt handler, for instance. The recording says
# which of it is real.
if [ -r "$rundata" ]; then
  echo
  echo "ROM addresses the recording actually reached:"
  tr ',' '\n' < "$rundata" | tr -cd '0-9\n' | awk '
    $1 >= 53248 && $1 <= 65535 { seen[$1] = 1 }
    END {
      n = 0
      for (a in seen) { printf "  $%04X\n", a; n++ }
      if (n == 0) print "  none"
    }' | sort
fi
