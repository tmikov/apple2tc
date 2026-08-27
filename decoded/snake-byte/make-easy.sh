#!/bin/bash
# Derive snake-byte-easy.b33 from snake-byte.b33.
#
# A test fixture, not a variant of the game. Snake Byte needs 16 apples to
# clear a level, and every level past the first draws its interior walls from
# the byte-coded display list at $8000 -- so $7113's interpreter and its 'H'
# and 'V' cases are unreachable in any recording that never finishes level 1.
# Both committed recordings stop on level 1, which left that whole subsystem
# compared against nothing.
#
# Lowering the quota to 2 makes the *existing* play-hires.pkeys reach level 3
# on its own, with no new play recorded: the snake keeps moving after the
# recorded input runs out, and that is enough. See probe-acceptance.sh's
# `easy` scenario.
#
# Two bytes, because there are two paths in:
#
#   $76BE  the immediate of `LDA #$10 / STA $7263` at $76BD, which every new
#          game runs via $76B7.
#   $7263  the initial RAM byte. $376E reaches $76C2 directly, skipping the
#          store above, so the first game of a session uses this one. Patching
#          only $76BE leaves a quota of 16 on exactly that first game -- which
#          is how this was found.
#
# The quota is BCD ($7817 does its arithmetic under SED), so $02 is 2.
set -e
cd "$(dirname "$0")"
python3 - <<'PY'
src, dst = 'testdata/snake-byte.b33', 'testdata/snake-byte-easy.b33'
d = bytearray(open(src, 'rb').read())
load = d[0] | (d[1] << 8)
off = lambda a: 4 + (a - load)
# Assert what we are patching over, so a rebuild against a different
# snake-byte.b33 fails here rather than producing a silently wrong fixture.
assert d[off(0x76bd)] == 0xA9 and d[off(0x76be)] == 0x10, 'unexpected bytes at $76BD'
assert d[off(0x7263)] == 0x10, 'unexpected byte at $7263'
d[off(0x76be)] = 0x02
d[off(0x7263)] = 0x02
open(dst, 'wb').write(bytes(d))
print('wrote %s: quota 16 -> 2 at $76BE and $7263' % dst)
PY
