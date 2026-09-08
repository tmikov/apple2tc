#!/bin/bash
# The game port's calibration, against the real ROM.
#
# A2_PADDLE_OFFSET is not asserted against the arithmetic that produced it.
# This assembles a program that calls the ROM's PREAD for each paddle, runs it
# with four known positions, and checks PREAD returns exactly those positions.
#
# PREAD samples at trigger + 10 and then every 11 cycles, so any offset in
# 0-10 passes this and 11 already fails at position 0. The test therefore
# pins the interval: the chosen constant must pass, and stepping past its
# upper end must fail. See the design doc for why the midpoint is chosen.
set -e

bin="${1:?usage: pread-acceptance.sh <build-dir>}"
a6502="$bin/tools/a6502/a6502"
a2run="$bin/tools/a2run/a2run"
here="$(dirname "$0")"

tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

"$a6502" "$here/pread.s" "$tmp/pread.b33"

# Four positions in one run: the rails, one either side of centre.
"$a2run" "$tmp/pread.b33" --headless --frames=200 --no-sound \
    --paddle=0:0 --paddle=1:1 --paddle=2:127 --paddle=3:255 \
    --snapshot-at=30E --snapshot-out="$tmp/out.ram" > /dev/null

got="$(od -An -tu1 -j 0x1F00 -N 4 "$tmp/out.ram" | tr -s ' ' | sed 's/^ //')"
want="0 1 127 255"
if [ "$got" != "$want" ]; then
  echo "FAIL: PREAD returned [$got], expected [$want]" >&2
  exit 1
fi

# The other end of the range, in a second run so 128 and 254 are covered too.
"$a2run" "$tmp/pread.b33" --headless --frames=200 --no-sound \
    --paddle=0:128 --paddle=1:254 --paddle=2:64 --paddle=3:192 \
    --snapshot-at=30E --snapshot-out="$tmp/out2.ram" > /dev/null

got2="$(od -An -tu1 -j 0x1F00 -N 4 "$tmp/out2.ram" | tr -s ' ' | sed 's/^ //')"
want2="128 254 64 192"
if [ "$got2" != "$want2" ]; then
  echo "FAIL: PREAD returned [$got2], expected [$want2]" >&2
  exit 1
fi

echo "ok  : the ROM's PREAD returns every position the host set"
