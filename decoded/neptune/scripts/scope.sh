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
