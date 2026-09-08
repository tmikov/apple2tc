#!/bin/bash
#
# Regenerate decoded/neptune/testdata/neptune.json: runtime data for apple2tc,
# collected by replaying play2.keys against the b33 instead of against the disk.
#
# Usage: collect.sh [build-dir]     (default ../../../cmake-build-debug)
#
# WHY AGAINST THE B33
#
# A collection from a disk boot records everything the machine did, and most of
# what it did was boot. Measured, comparing the two collections of the same play
# session: the disk run produces 459 branch targets and the b33 run 305, and the
# 156 that only the disk run has are 49 in ROM, 26 in the $C600 boot slot, and
# the rest the loader at $0500/$0800 and the load machinery in $Axxx-$Bxxx.
# None of that is Neptune. The b33 run keeps 300 of the 301 game targets.
#
# So collecting here is not a convenience, it is the point: the boot code the
# decompiler would otherwise have to account for is code the shipped artifact
# never runs.
#
# WHY THE KEYS HAVE TO BE REBASED, AND WHY A CONSTANT IS ENOUGH
#
# A .keys stamp is an absolute cycle count from reset, and play2.keys was
# recorded against a disk boot, which reaches the game about 21.7 million cycles
# later than the b33 does. Every stamp is therefore that much too late.
#
# The correction is a single constant because the two runs are the same machine
# from the entry point onward -- same RAM, same registers, same soft switches,
# and an Apple II+ has no interrupts, so nothing outside the program can shift
# where a cycle lands in the instruction stream. Rebasing by
# (disk entry - b33 entry) puts every key the same number of cycles after entry
# as it was in the recording, which is the same instruction.
#
# Unlike snake-byte's make-cold-keys.sh, the offset here is measured rather than
# passed in: --snapshot-at=$entry reports the cycle of first arrival in each
# run, so the script asks both machines instead of being told.
#
# The rebased recording is committed, as testdata/play2-cold.keys, following
# snake-byte's play-cold.pkeys -- "cold" meaning a build that starts at the
# game's entry rather than booting into it. It is a generated file like
# neptune.b33 and neptune.json, and this script is the one thing that writes
# it, so it cannot drift from play2.keys without drifting from them too.
#
# Having it in the tree is what lets the b33 be replayed directly:
#
#   a2run --frames=1929 decoded/neptune/testdata/neptune.b33 \
#     --key-file=decoded/neptune/testdata/play2-cold.keys
#
# ONE CAVEAT WORTH CARRYING FORWARD
#
# Cycle stamps are an engine-dependent coordinate. This works because both runs
# use the same interpreter. If Neptune is ever decompiled and the generated
# build replays this recording, the stamps will drift, exactly as they did for
# Snake Byte -- see the "Probe-stamped key recording" entry in
# docs/decompiling/decision-log.md. The fix then is a probe-stamped .pkeys, not
# a bigger constant.

set -eu

here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
root=$(cd "$here/../../.." && pwd)
bin=${1:-$root/cmake-build-debug}

a2run=$bin/tools/a2run/a2run
disk=$root/dsk/Neptune.dsk
b33=$here/../testdata/neptune.b33
keys=$here/../testdata/play2.keys
out=$here/../testdata/neptune.json
cold=$here/../testdata/play2-cold.keys

[ -x "$a2run" ] || { echo "$0: no a2run at $a2run" >&2; exit 1; }
for f in "$disk" "$b33" "$keys"; do
  [ -r "$f" ] || { echo "$0: missing $f" >&2; exit 1; }
done

# Read from make-b33.sh rather than repeated here. The offset is only a
# constant if this is the address the b33 actually starts at, so a copy that
# could drift out of step with the generator is a bug waiting to happen.
entry=$(sed -n 's/^entry=\([0-9A-Fa-f]*\).*/\1/p' "$here/make-b33.sh")
[ -n "$entry" ] || { echo "$0: could not read entry= from make-b33.sh" >&2; exit 1; }
# The frame count play2.keys was recorded and is documented to replay under,
# against the disk. The b33 replay is shortened by the offset so that it covers
# the same span of gameplay rather than the same number of frames.
disk_frames=3200
frame_cycles=17050

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

cycles_at() {  # cycles_at <out-name> <a2run args...>
  local name=$1; shift
  "$a2run" "$@" --snapshot-at=$entry --snapshot-out="$tmp/$name" > /dev/null 2>&1
  [ -s "$tmp/$name" ] || { echo "$0: never reached \$$entry in the $name run" >&2; exit 1; }
  sed -n 's/^cycles //p' "$tmp/$name.regs"
}

disk_entry=$(cycles_at disk --frames=3000 --disk1="$disk")
b33_entry=$(cycles_at b33 --frames=200 "$b33")
offset=$((disk_entry - b33_entry))

if [ "$offset" -le 0 ]; then
  echo "$0: the b33 reaches \$$entry at cycle $b33_entry, not before the disk's $disk_entry;" >&2
  echo "     there is nothing to rebase and the recording would not line up" >&2
  exit 1
fi

# Drop keys stamped before the entry -- they belong to the boot this build skips
# -- and shift the rest. play2.keys has none today; the arithmetic is here so
# that a recording which does have them cannot silently deliver them at
# negative-turned-huge stamps.
{
  # load_key_file() skips leading `#` lines, so a generated file can say what
  # generated it. It is committed, and a committed derived file that does not
  # name its source is one someone will eventually hand-edit.
  echo "# Generated by decoded/neptune/scripts/collect.sh. Do not edit."
  echo "# play2.keys rebased from a disk boot onto neptune.b33: every stamp"
  echo "# shifted down by (disk entry - b33 entry) at \$$entry = $offset cycles,"
  echo "# so each key lands the same number of cycles after entry as recorded."
  awk -v base="$disk_entry" -v off="$offset" '
    /^#/ { next }
    $1 < base { dropped++; next }
    { printf "%d %s\n", $1 - off, $2; kept++ }
    END { printf "rebased %d keys, dropped %d below the entry\n", kept, dropped > "/dev/stderr" }
  ' "$keys"
} > "$cold"

frames=$(( (disk_frames * frame_cycles - offset) / frame_cycles + 1 ))
last=$(awk '!/^#/ { last = $1 } END { print last }' "$cold")
if [ "$last" -ge "$((frames * frame_cycles))" ]; then
  echo "$0: the replay would end at cycle $((frames * frame_cycles)), before the last key at $last" >&2
  exit 1
fi

"$a2run" --frames=$frames "$b33" --key-file="$cold" \
  --collect --out="$out" > /dev/null 2>&1
[ -s "$out" ] || { echo "$0: collection produced nothing" >&2; exit 1; }

# A collection that ran but never entered the game would still be a valid JSON
# file, so check that the two addresses this game is defined by are in it: the
# title wait it starts from and the main loop it spends its time in.
targets=$(grep -o '"BranchTargets":\[[^]]*\]' "$out" | tr ',' '\n' | tr -cd '0-9\n')
for want in $entry 2000; do
  dec=$(printf '%d' "0x$want")
  echo "$targets" | grep -qx "$dec" || {
    echo "$0: \$$want is not in the collection; the replay did not reach the game" >&2
    exit 1
  }
done

echo "wrote $cold"
echo "wrote $out"
echo "entry \$$entry: disk at cycle $disk_entry, b33 at $b33_entry, offset $offset"
echo "replayed $frames frames of play2.keys against the b33"
