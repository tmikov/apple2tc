#!/bin/bash
#
# Regenerate decoded/neptune/testdata/neptune.b33: Neptune, already loaded and sitting on
# its title screen, as a single self-contained DOS 3.3 binary that needs no disk
# and no DOS.
#
# Usage: make-b33.sh [build-dir]     (default ../../../cmake-build-debug)
#
# WHY IT EXISTS
#
# Neptune spends about 21 seconds loading itself from disk before it will accept
# a keystroke. That is 21 seconds in front of every play session and every
# collection run, and it makes the disk a permanent dependency of both. This
# captures the machine at the end of that load and turns it into a .b33, so the
# game comes up in the time it takes to boot the ROM.
#
# WHY A .b33 AND NOT A SNAPSHOT FORMAT
#
# A .b33 already runs everywhere in this repo -- a2emu, a2run, a2mcp's
# boot(binary:), and apple2tc itself -- so a snapshot expressed as one needs no
# new loader, no new format and no new flag. What a .b33 cannot express is the
# half of the machine that is not RAM: the registers and the soft switches. So a
# short prologue is prepended that sets those and jumps to the entry point,
# which is the whole trick.
#
# WHERE IT IS CAPTURED, AND WHY THERE
#
# $3523, the top of the title screen's key-wait loop:
#
#   3523: STA KBDSTRB      ; clear the strobe
#   3526: LDX #$00         ; 256 tries
#   3528: JSR $3584        ; poll the keyboard
#   352B: BMI $3580        ; a key -> start a game
#   352D: LDA #$40
#   352F: JSR $3570        ; delay
#   3532: DEX
#   3533: BNE $3528
#   3535: ...              ; timed out -> fall through to the attract demo
#
# This is the moment the game is showing its title and is willing to start, and
# it is reached 21,823,766 cycles (~21.4s, frame ~1280) into a disk boot. Three
# things recommend it over a frame number:
#
#   - It is Neptune's own code. A frame boundary lands wherever it lands, which
#     in practice was $FCAA, inside the ROM's MON_WAIT delay loop -- resumable,
#     but it makes the ROM part of the entry path for no reason.
#   - On arrival the stack is *empty* (SP=$FF), so the resumed machine depends
#     on no stack contents at all.
#   - It is where pressing space gives a real game. Capturing later, at the main
#     loop $2000 (~27s), lands inside the attract demo instead: the game runs,
#     but space does nothing and the HUD reads SH:00 rather than SH:3.
#
# HOW THE PIECES FIT
#
#   $0000-$0002   JMP to the stub. The .b33 loader enters at the load address,
#                 and the load address is the lowest byte in the file, so the
#                 entry has to live here. The stub puts the three original bytes
#                 back before jumping.
#   $0100-...     the stub, in the stack page. Legitimate rather than a
#                 compromise: SP=$FF means nothing in $0100-$01FF is live.
#                 Placed at $0100 to stay clear of $0141-$0142, which holds
#                 bytes that may or may not be dead residue -- untested either
#                 way, so left alone.
#   $0003-$BFFF   the snapshot, verbatim. Nothing is copied at run time: the
#                 payload spans all of main RAM, so the loader's own memcpy puts
#                 zero page and everything else exactly where it belongs.
#
# DOS IS NOT IN MEMORY, AND NEITHER IS A LIVE DISK DRIVER
#
# There is disk-driver-shaped code at $BC00-$BFA6 -- IWM register reads through
# $C08C,X, phase stepping, motor-on -- and it is tempting to read that as a
# resident DOS that the b33 is quietly depending on. It is neither.
#
# Not DOS: the DOS 3.3 command table is absent (no CATALOG, BLOAD, INIT,
# MAXFILES or DISK VOLUME anywhere in the 64KB image), $03D0-$03D5 holds
# $3A $3B $3C $3D $3E $3F -- an ascending table -- rather than DOS's JMP $9DBF
# vectors, and $9D00 is data. What is up there is leftover from the boot,
# including message text at $BEC0 reading "(BRUN HELL..." and "(DOS IN
# LANGUAGE CARD...)".
#
# Not live: six probes spread across $BC57-$BF67 fired zero times in a
# 3000-frame disk boot, against positive controls at $2000 and $3523 in the
# same run that fired 630 and 2 times. Neptune loads through the $C600 boot ROM
# and its own loader; this code never executes.
#
# So those bytes ride along -- the payload spans all of $0000-$BFFF -- but they
# are ballast, not a dependency. (Note for anyone re-deriving this: an
# `xref $C0E0 $C0EF` in id proves nothing here, because a DOS-style driver
# reaches the drive as $C08C,X with X=$60 and never names $C0Ex at all.)
#
# WHAT IS NOT REPRODUCED
#
# The cycle phase. The b33 enters the game at whatever cycle the ROM boot
# happens to finish on, where the disk run entered at 21,823,766. An Apple II+
# has no interrupts and no raster register, so the 6502 cannot observe a frame
# boundary and the instruction stream is unaffected -- but the *screen is
# sampled* on frame boundaries, so --hash-frames output is not comparable
# between the two runs. This was checked rather than assumed: a capture padded
# with a delay loop, so that entry landed exactly on a frame boundary,
# reproduced 1591 consecutive frame hashes of the disk run byte for byte. The
# padding is not shipped, because it is tuned to how long the ROM boot takes and
# would rot the first time anything upstream moved.
#
# So what is verified below is the entry state itself, which is the thing this
# file actually claims to reconstruct.

set -eu

here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
root=$(cd "$here/../../.." && pwd)
bin=${1:-$root/cmake-build-debug}

a2run=$bin/tools/a2run/a2run
id=$bin/tools/id/id
disk=$root/dsk/Neptune.dsk
out=$here/../testdata/neptune.b33

for f in "$a2run" "$id"; do
  [ -x "$f" ] || { echo "$0: no executable at $f" >&2; exit 1; }
done
[ -r "$disk" ] || { echo "$0: no disk image at $disk" >&2; exit 1; }

# The title screen's key wait. See the header for why this address and not a
# frame number.
entry=3523
# Where the stub goes, and the highest byte of the payload.
stub=256      # $0100
top=49151     # $BFFF

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

# --- capture ---------------------------------------------------------------
# --frames is a ceiling, not a target: the snapshot is one-shot and fires on
# first arrival, and 3000 frames is comfortably past the ~1280 it takes.
"$a2run" --frames=3000 --disk1="$disk" \
  --snapshot-at=$entry --snapshot-out="$tmp/snap.bin" > /dev/null 2>&1
[ -s "$tmp/snap.bin" ] || { echo "$0: capture produced no snapshot" >&2; exit 1; }

reg() { sed -n "s/^$1 //p" "$tmp/snap.bin.regs"; }
a=$(reg a); x=$(reg x); y=$(reg y); sp=$(reg sp)
status=$(reg status); vc=$(reg vid_control)

# The three bytes the trampoline at $0000 overwrites, which the stub restores.
b0=$(od -An -tx1 -N3 -v "$tmp/snap.bin" | tr -d ' \n')
z0=${b0:0:2}; z1=${b0:2:2}; z2=${b0:4:2}

# The keyboard is the one piece of non-RAM state 6502 code cannot recreate:
# reading $C010 clears the strobe and nothing sets it. Capture points with a key
# still pending are therefore not expressible, so refuse rather than emit a b33
# that quietly starts from a different machine. ($3523 clears the strobe itself
# on the first instruction, so this is belt and braces here -- but the check
# belongs to the technique, not to this address.)
if [ "$(reg kbd_queued)" != "0" ]; then
  echo "$0: a key is pending at \$$entry; that state cannot be rebuilt by a prologue" >&2
  exit 1
fi

# The stub writes the status byte through PHA/PLP, which touches exactly one
# byte: $0100+SP, the slot the next push would use. That is dead only because
# the stack is empty here. If a capture ever has live stack, this needs saying
# again rather than assuming.
if [ "$sp" != "FF" ]; then
  echo "$0: SP is \$$sp, not \$FF; the stub assumes an empty stack page" >&2
  exit 1
fi
scratch=$((stub + 0xFF))

# --- soft switches ---------------------------------------------------------
# vid_control is exactly the four classic bits, so each is one absolute read.
# A2_VC_TEXT=1, A2_VC_MIXED=2, A2_VC_PAGE2=4, A2_VC_HIRES=8.
vcd=$((0x$vc))
sw_text=$(( (vcd & 1) ? 0xC051 : 0xC050 ))
sw_mixed=$(( (vcd & 2) ? 0xC053 : 0xC052 ))
sw_page=$(( (vcd & 4) ? 0xC055 : 0xC054 ))
sw_hires=$(( (vcd & 8) ? 0xC057 : 0xC056 ))

# --- stub ------------------------------------------------------------------
# Order matters and is not obvious. Every load writes N and Z, and TXS is the
# only way to set SP, so: SP first (through X), then Y and X to their final
# values, then the status byte pushed and pulled back with the accumulator
# loaded in between. PLP is last because nothing after it may touch a flag, and
# JMP does not.
{
  echo "loadbin $tmp/snap.bin 0"
  printf 's $%04X\n' $stub
  echo "asm lda #\$$z0"
  echo 'asm sta $00'
  echo "asm lda #\$$z1"
  echo 'asm sta $01'
  echo "asm lda #\$$z2"
  echo 'asm sta $02'
  printf 'asm lda $%04X\n' $sw_text $sw_mixed $sw_page $sw_hires
  echo "asm ldx #\$$sp"
  echo 'asm txs'
  echo "asm ldy #\$$y"
  echo "asm ldx #\$$x"
  echo "asm lda #\$$status"
  echo 'asm pha'
  echo "asm lda #\$$a"
  echo 'asm plp'
  echo "asm jmp \$$entry"
  echo 's $0000'
  printf 'asm jmp $%04X\n' $stub
  printf 'saveb33 %s $0000 $%04X\n' "$out" $top
} > "$tmp/build.id"

# id is an interactive tool, so it prefixes each reply with its "> " prompt even
# on a pipe. Every pattern below has to survive that.
"$id" < "$tmp/build.id" > "$tmp/build.log" 2>&1
sed 's/^> //' "$tmp/build.log" > "$tmp/build.out"
grep -q "^saved:" "$tmp/build.out" || { cat "$tmp/build.log" >&2; exit 1; }

# How far the stub actually reached, so the check below knows what to excuse.
# id echoes each assembled instruction as "ADDR: BYTES  MNEMONIC", so the last
# line in the stack page is the closing JMP; its length is counted from the
# bytes id printed rather than assumed.
#
# mawk has no strtonum(), so hex arrives as text and is converted here -- the
# same reason decoded/rom/make-boot-state.sh carries its own hex().
stub_end=$(awk 'function hex(s,   i, v, d) {
                  v = 0; s = toupper(s)
                  for (i = 1; i <= length(s); i++) {
                    d = index("0123456789ABCDEF", substr(s, i, 1)) - 1
                    if (d >= 0) v = v * 16 + d
                  }
                  return v
                }
                /^01[0-9A-Fa-f][0-9A-Fa-f]: / {
                  addr = hex(substr($1, 1, 4))
                  n = 0
                  for (i = 2; i <= NF; i++) {
                    if ($i !~ /^[0-9A-Fa-f][0-9A-Fa-f]$/) break
                    n++
                  }
                  last = addr + n - 1
                }
                END { print last }' "$tmp/build.out")
[ -n "$stub_end" ] || { echo "$0: could not find the stub in id output" >&2; exit 1; }

# --- verify ----------------------------------------------------------------
# The claim is that running the b33 rebuilds the captured machine. So run it,
# capture again at the same address -- which the b33 reaches immediately -- and
# require the two to agree.
"$a2run" --frames=200 "$out" \
  --snapshot-at=$entry --snapshot-out="$tmp/back.bin" > /dev/null 2>&1
[ -s "$tmp/back.bin" ] || { echo "$0: the generated b33 never reached \$$entry" >&2; exit 1; }

# Registers and soft switches must match exactly. `cycles` is excluded and only
# `cycles`: it counts from reset, and the two runs reach the entry by different
# routes, which is the one difference this file is allowed to have.
if ! diff <(grep -v '^cycles ' "$tmp/snap.bin.regs") \
          <(grep -v '^cycles ' "$tmp/back.bin.regs") > /dev/null; then
  echo "$0: the b33 does not reconstruct the captured registers/soft switches" >&2
  diff <(grep -v '^cycles ' "$tmp/snap.bin.regs") \
       <(grep -v '^cycles ' "$tmp/back.bin.regs") >&2
  exit 1
fi

# RAM must match across the whole payload except the bytes the stub occupies and
# the one slot PHA/PLP borrowed. Only $0000-$BFFF is compared: above it are the
# soft switches and the ROM, and $C600 in particular differs by design -- the
# disk run has a Disk II ROM installed there and the b33 run, having no disk,
# does not.
bad=$(cmp -l -n $((top + 1)) "$tmp/snap.bin" "$tmp/back.bin" 2>/dev/null |
      awk -v lo="$stub" -v hi="$stub_end" -v scratch="$scratch" \
          '{ a = $1 - 1; if ((a < lo || a > hi) && a != scratch) print a }' |
      head -20)
if [ -n "$bad" ]; then
  echo "$0: the b33 changes RAM outside the stub:" >&2
  echo "$bad" | while read -r addr; do printf '  $%04X\n' "$addr" >&2; done
  exit 1
fi

echo "wrote $out"
printf 'entry $%s, stub $%04X-$%04X, payload $0000-$%04X (%u bytes)\n' \
  "$entry" "$stub" "$stub_end" "$top" "$((top + 1))"
echo "verified: re-entering \$$entry from the b33 reproduces the captured"
printf 'registers, soft switches, and all of $0000-$%04X outside the stub.\n' "$top"
