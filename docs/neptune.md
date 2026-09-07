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
{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"run","arguments":{"frames":1400}}}
```

Feed it on stdin: `a2mcp --root=. < prefix.jsonl`. Add `--hash-frames=out.txt`
to compare two runs. The disk is already in its title/attract sequence by
frame 1400; no BASIC command is needed.

At the end of Prefix A: frame 1400, `hgr` page 1, full screen.

## Determinism: the control

Prefix A plus 700 more frames, run twice in **separate processes**, produced
byte-identical `--hash-frames` output across every frame.

## Booting

**The disk autoboots. `PR#6` is not needed** — an earlier version of this file
said the opposite. It is harmless when present: the game ignores the keyboard
during boot, so those five keystrokes change nothing, but neither Prefix A above
nor `decoded/neptune/testdata/play2.keys` types it any more. Measured, from
`boot(disk1:...)` with no keys at all: still `text` at frame 300, `hgr` page 1
by frame 1200.

The mistake was impatience. The original test ran 600 frames, saw a text screen —
carrying the *previous* session's content, since at the time `boot` did not
clear RAM on a reboot — and concluded there was a BASIC prompt waiting. A Disk
II boot simply takes longer than that. Verified again by replaying
`decoded/neptune/testdata/play2.keys` against a control run with no keys: the
two are identical through frame 1399 and first diverge at frame 1400, exactly
where the space keypress lands.

(That "does not clear RAM" behaviour has since changed: `boot` on an
already-running `a2mcp` is now a power-on, byte-identical to a fresh process
— see dz issue 01a07cd8, filed after this same leftover-RAM effect turned out
to have a second, worse symptom: a rebooted disk session landing in whatever
the previous session left resident instead of redoing the boot at all. The
account above is what actually happened at the time and is left as it was.)

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
with `A` to fire.

**There is no "hold" — movement is a state, and the opposite key stops it.**
From the repo's owner, who has played it, and confirmed here by pressing `K`,
watching the ship travel right, then pressing `J` twice and watching it come
back:

| state | `K` | `J` |
|---|---|---|
| stopped | moves right | moves left |
| moving right | — | stops |
| moving left | stops | — |

So **reversing direction takes two presses**: the opposite key stops you, and
the same key again starts you the other way. `I` and `M` are up and down and
behave the same way by the disk's own diagram, though that has not been
separately confirmed here.

This matters more than it sounds. Sending a burst like `KKA` — the natural
"hold right and fire" a hold-to-move game would want — reads here as *start
right, keep going right, fire*, and a burst like `KJA` cancels itself. An
earlier session sent doubled direction keys throughout and concluded from the
resulting stillness that the ship was not responding at all.

## It never shows a whole picture

Neptune is single-buffered and draws by erasing and redrawing, so at a given
frame boundary part of the image is simply absent. Measured during play, eight
consecutive single frames, counting only the sky rows where sprites live and the
scrolling terrain does not:

```
ink per frame:  122  94  128  128  128   33  128  134
union of all 8: 215        pixels present in ALL 8: 0
objects/frame:   2    2    2    2    2    1    2    2
```

**Not one sky pixel survives all eight frames**, and on frame 5 an entire object
is missing — two connected components collapse to one. A single snapshot of that
frame shows no enemy, not a faint one. Roughly one capture in eight loses an
object outright.

A human never notices, because at 60 Hz the eye integrates across frames. A model
taking one snapshot does not. So **read this game with `blend: 3`**: it ORs the
last three frames' page bits, which reconstructs what the eye does. With blending
the same eight frames all show both objects.

Note what this means for `settle` and `until:"screen_update"`: frames 2, 3 and 4
above are byte-identical, so both features call them clean, and they still carry
only 59% of the ink seen across the window. Those features find moments when
*drawing has paused*, which is not the same as a moment when the picture is
*whole* — on this game there is no such moment. `settle` still earns its place
against a half-scrolled terrain; it cannot fix a missing sprite. The two are
complementary and `blend` is the one that matters more here.

## Starting a game

The disk autoboots into the title/attract sequence. **Run about 1400 frames,
then press space.** That is roughly 23 seconds from `boot(disk1:...)`; the
title screen is visible at this point. Then run a short step and check the HUD.

`SH` tells you whether it worked: a live game shows `SH:3`, the demo shows
`SH:00` and has no player ship. Pressing space too early does nothing. Reported
by the repo's owner, who has played the game, and confirmed here — every session
before this one that "looked like gameplay" was the demo.

## Playing it

The loop that works, with every argument load-bearing:

```
keys(text: "KKA", frames_between: 2)            # deliver defaults to "press"
run(frames: 4, settle: 14)                      # pace, land clean if cheap
screen(format: "image", render: "color140", scale: 2, blend: 3)
```

- `deliver` must stay `"press"`. Type-ahead never reaches this game.
- `frames` only needs to cover key delivery — `(n-1) * frames_between` — and
  everything else belongs in `settle`, which is spent only when the screen is not
  already clean.
- `blend: 3` is not optional here, for the reason above.

**Clean-landing rates are phase-dependent, so calibrate where you are playing.**
With an 18-frame ceiling: 0 of 30 looks landed clean in the title-and-attract
phase — the ~1400 frames from `boot(disk1:...)` up to the space keypress that
starts a game, continuously redrawing — 18 of 30 in a later stretch of play, and
2 of 14 in a different stretch again. The same call, the same game. Re-check
when the game changes mode rather than trusting a number measured elsewhere.

**Scoring happens.** `SC` reached `00020` during a session of firing with `A`, so
`A` does damage something. `SH` has been observed going all the way to `00`.

## The screen

- Render with `color140`; the per-dot `color` mode shreds the terrain into
  stripes. The terrain is **solid green**, not dithered.
- **Your ship is the orange craft**, normally in the upper part of the screen.
  This file previously said the ship was "a small compact white blob" and that
  the orange shape was an explosion. Both were wrong. Confirmed by the only test
  that settles it: press `K`, run 30 frames, and watch which object moves right;
  then reverse and watch it come back. The white blobs on the terrain are
  something else and have never been identified.
- A large orange starburst that appears and fades *is* an explosion. It is much
  bigger than the ship and does not persist. Size and persistence tell them
  apart.
- `A` fires **horizontally to the right**; the shots are visible as magenta
  dashes travelling across the upper screen.
- Enemies render magenta.
- HUD: `FUEL`, `SC` (score), `HI`, `LV` (level), `SH`, `BO`. `SH` is a shield
  count that decreases as you are hit — observed `3 → 2` across a play session.
- Never flips pages, so every completed frame it produces is a settle, never a
  flip.
- `until:"screen_update"` finds clean moments in some phases: across twelve
  consecutive `run(frames=120, until="screen_update")` calls, eight landed on a
  completed picture, most within 4–12 frames. That was one stretch of play; see
  the phase-dependence note under Playing it before relying on the figure.

## How to actually score

`Z` drops a bomb and `BO` is the bomb count — confirmed: two presses took it from
`20` to `18`. Reported by the repo's owner; the bomb key is not on the disk's own
help screen, which lists only I/J/K/M and `A`.

**The gun fires horizontally, and enemies fly below the ship's spawn height.** So
the loop that scores is: press `M` to descend, `I` to level off at enemy
altitude, then fire. Sitting at spawn height and firing — which is what an
earlier session did for a whole game — shoots over everything and scores nothing.

That is the game's trade, and it is sharp. A run played this way went
`0 → 20 → 40 → 160 → 240 → 280 → 340` while shields went `3 → 2 → 1`. Every
shield was lost low. After a hit you respawn back at spawn height, so you must
descend again to resume scoring.

For comparison: a session spent hovering safely at spawn height and firing
scored `40` and died in about 45 s. A session descending to enemy altitude
scored `340` and was still alive past 60 s with one shield left.

## A played game, end to end

The first session that was actually controlled rather than watched was
recorded so it could be replayed:

- Started with space after ~23 s; `SH:3`.
- Held station high on the left, above the terrain, firing periodically.
- Lost a shield somewhere in the first 25 s while stationary — **standing still
  is not safe**; the scrolling brings enemies to you.
- Died at roughly 45 s with `SC:00040`, which became `HI:00040`. Short of a
  minute.
- Death returns to the Gebelli title screen, which displays `SC` and `HI` at the
  bottom — a convenient place to read a final score.

A later controlled session verified the full input loop: space at frame 1400,
`K` moved the ship right, repeated `A` bursts produced explosions, and short
`M`/`I` steps brought the ship down to enemy altitude. That run reached
`SC:00180` while still showing a live shield count. Its cycle-stamped replay is
`decoded/neptune/testdata/play2.keys`; reproduce it with:

```bash
a2run --frames=3200 --disk1=dsk/Neptune.dsk --key-file=decoded/neptune/testdata/play2.keys --hash-frames=out.txt
```

**The title screen carries the scoring table**, five enemy types worth **20, 50,
80, 10 and 500**. The 500 is the long magenta creature. That is the game telling
you what to prioritise, and it is legible in `color140` where the older per-dot
render made it mush.

**The gun fires horizontally**, so an enemy below your altitude cannot be hit
from where you are. Most enemies encountered so far sit below the ship's
default height, which means scoring requires descending toward the terrain and
therefore trading safety for points. That tension is probably the game.

Unverified but strongly suggested by the HUD: `FUEL` visibly depletes during
play, and the white blobs sitting on the terrain may be fuel or bomb targets —
which would make this a Scramble-style game where you must dive to refuel. `BO`
has never moved from `20`, so if it is bombs, the bomb key is not one of the
five keys tried.

## Sound

The attract demo produces 878 speaker segments over 3000 frames, including a
repeated 1.1-second tone near 334 Hz. It is silent from roughly 0.1 s to 19 s
after boot, which can read as "no sound" if you only listen briefly.

## Open questions

- Whether `FUEL` running out kills you, and how to replenish it. It visibly
  depletes; nothing tried so far has refilled it.
- What bombs are *for*: `Z` decrements `BO`, but no bombed ground target has yet
  been seen to score or to refuel.
- What the white blobs sitting on the terrain are: targets, scenery, or fuel.
- What `FUEL` depletion does, and whether it can be replenished.
- Where the ship's coordinates, `SH`, `BO` and the score live in RAM.
