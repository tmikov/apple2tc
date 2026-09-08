#!/bin/bash
#
# Seed the hand-maintained artifact from the generated extern build:
#
#   neptune.c         the game -- hand-maintained from here on
#   game-image.inc    $0000-$BFFF as hex
#   rom-image.inc     $D000-$FFFF as hex
#
# Usage: seed-artifact.sh [build-dir]        (refuses to overwrite neptune.c)
#
# THIS IS RUN ONCE, AND THEN NEVER AGAIN
#
# It is committed for provenance, not for repetition: it records exactly what
# the artifact started as, so "did someone hand-edit this into existence?" has
# an answer. After the first run neptune.c is a source file like any other and
# re-running this would throw away every conversion made since -- hence the
# refusal to overwrite.
#
# scripts/decompile.sh keeps regenerating as-generated/ and reference/ from the
# binary, and those stay machine-owned. They are the control the artifact is
# checked against by scripts/verify.sh; that comparison is the whole reason the
# artifact may be edited at all.
#
# WHY THE IMAGES MOVE OUT
#
# 3,844 of the generated file's 22,728 lines are two hex arrays. They are data,
# they will never be edited, and leaving them inline means every search through
# the game's code walks past 16% of noise.

set -eu

here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
root=$(cd "$here/../../.." && pwd)

src=$here/../reference/neptunec1-ext.c
out=$here/../neptune.c
gameimg=$here/../game-image.inc
romimg=$here/../rom-image.inc

[ -r "$src" ] || { echo "$0: no $src -- run decompile.sh first" >&2; exit 1; }
if [ -e "$out" ]; then
  echo "$0: $out already exists." >&2
  echo "     This script seeds the artifact once; it does not regenerate it." >&2
  echo "     Delete the file deliberately if you really mean to start over." >&2
  exit 1
fi

awk -v gameimg="$gameimg" -v romimg="$romimg" -v out="$out" '
  # The two image arrays are diverted to their own files and replaced by an
  # #include, so the game code is what is left.
  /^static const uint8_t s_mem_0000\[0x[0-9a-f]+\] = \{/ { dest = gameimg; started = 1 }
  /^static const uint8_t s_mem_d000\[0x[0-9a-f]+\] = \{/ { dest = romimg;  started = 1 }
  started {
    print > dest
    if ($0 ~ /^};/) {
      close(dest)
      printf "#include \"%s\"\n", (dest == gameimg ? "game-image.inc" : "rom-image.inc") > out
      started = 0; dest = ""
    }
    next
  }
  { print > out }
' "$src"

echo "wrote $out            $(wc -l < "$out") lines"
echo "wrote $gameimg   $(wc -l < "$gameimg") lines"
echo "wrote $romimg    $(wc -l < "$romimg") lines"
