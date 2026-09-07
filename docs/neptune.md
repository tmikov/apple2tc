# Neptune (Gebelli Software) — what is known

A per-game notebook, kept because **this machine is deterministic**: given the
same disk and the same input, frame N is byte-identical every time, across
separate processes. That was measured, not assumed. So a fact recorded here does
not decay into anecdote — anyone can replay the coordinate beside it and check.

Every claim below carries the prefix that reproduces it.

## Status: playable

Neptune responds to its own control keys. Confirmed by running an identical
prefix with and without a key and diffing `--hash-frames`: `A`, `J` and `K` each
change the run, and space does not — consistent with the key map the disk itself
carries. It boots to an attract demo that plays itself; the keys work from
there.

**A `keys` call must deliver keystrokes, not type-ahead.** That is the default
(`deliver: "press"`), and it matters for this game specifically: Neptune polls
`$C000` and never acknowledges through `KBDSTRB`, so a key delivered as
type-ahead stages behind the unread strobe and never reaches the latch. Use
`deliver: "type"` only for entering text, such as `PR#6` at the BASIC prompt or
a BASIC program.

### The mistake this file previously recorded

An earlier version of this document stated, at length and with evidence, that
Neptune could not be started at all and was waiting on a joystick button that
`lib/a2io` does not emulate. That was wrong, and the way it was wrong is worth
keeping.

The reasoning was: sweep all 97 printable keys against an identical prefix, diff
the frame hashes, observe that nothing changes; separately observe that the game
reads `$C061`/`$C062`; conclude it wants a button. The sweep was real and the
button reads are real. The conclusion was not, because **`a2mcp`'s `keys` tool
was delivering through the type-ahead path**, so no key after the first ever
reached the machine. The game had been unreachable by the instrument, not
unresponsive.

The experiment even had a positive control — the same comparison with the
machine sitting in BASIC *does* detect a keypress — and the control passed while
the experiment was invalid. BASIC reads each character and clears the strobe, so
the latch is always free and type-ahead reaches it immediately; Neptune never
frees the latch, so it does not. **A control has to exercise the same conditions
as the case under test.** One that only proves the harness works against a
cooperative program proves nothing about an uncooperative one.

The buttons are still not emulated and Neptune still reads them, roughly once
per attract cycle; see dz issue `01a074d9`. That is a real gap. It is simply not
what was stopping the keyboard.

## Reproducing anything here

**Prefix A — boot to the attract demo.**

```json
{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"n","version":"1"}}}
{"jsonrpc":"2.0","id":2,"method":"tools/call","params":{"name":"boot","arguments":{"disk1":"dsk/Neptune.dsk"}}}
{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"run","arguments":{"frames":600}}}
{"jsonrpc":"2.0","id":4,"method":"tools/call","params":{"name":"keys","arguments":{"text":"PR#6\n","deliver":"type"}}}
{"jsonrpc":"2.0","id":5,"method":"tools/call","params":{"name":"run","arguments":{"frames":1500}}}
```

Feed it on stdin: `a2mcp --root=. < prefix.jsonl`. Add `--hash-frames=out.txt`
to compare two runs. Note `deliver:"type"` on the `PR#6` line: that is text for
BASIC, not a game keystroke.

At the end of Prefix A: frame 2100, `hgr` page 1, full screen.

## Determinism: the control

Prefix A plus 700 more frames, run twice in **separate processes**, produced
byte-identical `--hash-frames` output across every frame.

## Booting

The disk does not autoboot. `boot(disk1:"dsk/Neptune.dsk")` lands at a BASIC `]`
prompt; `PR#6` boots the drive.

## Controls

From the disk's own key-map help screen, recovered with:

```bash
python3 - <<'EOF'
d = open("dsk/Neptune.dsk","rb").read()
cur, out = "", []
for b in d:
    c = b & 0x7F
    if 32 <= c < 127: cur += chr(c)
    else:
        if len(cur) >= 8: out.append(" ".join(cur.split()))
        cur = ""
for s in out:
    if any(k in s.upper() for k in ("FIRE","LEFT","RIGHT","KEY")): print(repr(s))
EOF
```

It prints `LEFT J---- ----K RIGHT` and `I A -- -- -FIRE M` — an I/J/K/M diamond
with `A` to fire. `A`, `J` and `K` are confirmed to affect the run; the exact
effect of each, and of `I`/`M`, is not yet pinned to a coordinate here.

## The screen

- Render with `color140`; the per-dot `color` mode shreds the terrain into
  stripes. The terrain is **solid green**, not dithered.
- The ship is a small compact white blob. A persistent orange-red shape sits
  near the top left. `mono140` separates shapes from colour when in doubt.
- Enemies render magenta.
- HUD: `FUEL`, `SC` (score), `HI`, `LV` (level), `SH`, `BO`. `SH` is a shield
  count that decreases as you are hit — observed `3 → 2` across a play session.
- Never flips pages, so every completed frame it produces is a settle, never a
  flip.
- `until:"screen_update"` works well: across twelve consecutive
  `run(frames=120, until="screen_update")` calls, eight landed on a completed
  picture, most within 4–12 frames.

## Sound

The attract demo produces 878 speaker segments over 3000 frames, including a
repeated 1.1-second tone near 334 Hz. It is silent from roughly 0.1 s to 19 s
after boot, which can read as "no sound" if you only listen briefly.

## Open questions

- What each of `A`, `J`, `K`, `I`, `M` actually does on screen, pinned to
  coordinates.
- What `BO` counts.
- How a game is started from the attract demo, if it must be.
- Where the ship's coordinates, `SH`, `BO` and the score live in RAM.
