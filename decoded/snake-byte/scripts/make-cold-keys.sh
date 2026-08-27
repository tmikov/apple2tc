#!/bin/bash
#
# Convert a .pkeys recording for the cold-start build.
#
# Usage: make-cold-keys.sh <in.pkeys> <out.pkeys> <offset>
#
# A .pkeys stamp is a value of the replay coordinate -- a counter the probe
# language increments at the seven keyboard-read sites in rec.probe. The
# cold-start build never runs the boot, so every coordinate hit the boot
# produced does not happen, and every stamp recorded after it is that much too
# late.
#
# The correction is a constant, because no coordinate site fires between the
# end of boot and the first arrival at $3750. Measure it by installing a probe
# at $3750 alongside the coordinate and printing the counter on first arrival;
# for play.pkeys and play-hires.pkeys it is 181207, for play-rebind.pkeys
# 217543.
#
# Keys stamped below the offset are the ones typed at the BASIC prompt -- the
# eleven that spell `CALL 14160` and Return. They are dropped: this build is
# already past the point they exist to reach.

set -eu

[ $# -eq 3 ] || { echo "usage: $0 <in.pkeys> <out.pkeys> <offset>" >&2; exit 1; }
in=$1 out=$2 offset=$3

[ -f "$in" ] || { echo "$0: no such file: $in" >&2; exit 1; }
case $offset in
  ''|*[!0-9]*) echo "$0: offset must be a decimal number, got '$offset'" >&2; exit 1;;
esac

awk -v off="$offset" -v src="$in" '
  /^#/ { next }
  NF == 0 { next }
  { if ($1 < off) { dropped++; next }
    print $1 - off, $2; kept++ }
  END {
    if (kept == 0) {
      print "no keys survived the offset -- is it right?" > "/dev/stderr"
      exit 1
    }
    printf "dropped %d prompt keystrokes, kept %d\n", dropped, kept > "/dev/stderr"
  }
' "$in" > "$out.tmp"

{
  echo "# probe-stamped keys for the cold-start build, from $(basename "$in")."
  echo "# Prompt keystrokes dropped; remaining stamps shifted down by $offset."
  echo "# Regenerate with make-cold-keys.sh -- do not hand-edit."
  cat "$out.tmp"
} > "$out"
rm -f "$out.tmp"
echo "wrote $out"
