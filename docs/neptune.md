# Neptune (Gebelli Software) — what is known

A per-game notebook, kept because **this machine is deterministic**: given the
same disk and the same input, frame N is byte-identical every time, across
separate processes. That was measured, not assumed (see [Determinism](#determinism-the-control)).
So a fact recorded here does not decay into anecdote — anyone can replay the
coordinate beside it and check.

Every claim below carries the prefix that reproduces it. Facts without a
reproducible coordinate do not belong in this file.

## Status: not playable in this emulator

**Neptune cannot currently be started.** It sits in its attract demo forever,
and no keyboard input reaches it. The causal chain, each link measured:

1. Neptune reads **`$C061` and `$C062`** — joystick/paddle button 0 and button
   1. Twice per roughly sixty seconds of emulated time, which is the shape of a
   once-per-attract-cycle "does someone want to play?" check.
2. `lib/a2io` does not emulate the game port. `a2_io_peek` dispatches on
   `addr & 0xCFF0`, so `$C060`-`$C06F` all land on the `A2_TAPEIN` arm and
   return 0 — quietly, since reporting every read buried the rest of stderr.
   The comment there says the rest: zero is not a neutral answer. Bit 7 clear
   reads as no button held, and as a paddle one-shot that has already expired,
   so a program offering joystick control believes it has one, centred nowhere
   with nothing pressed. See dz issue `01a074d9`.
3. On real hardware those addresses report the button in **bit 7**, set when
   pressed. Returning `0` therefore means *never pressed*, forever.
4. So the attract loop never exits, and the keyboard is never read: with
   `status` polled across a session, `keys_pending` only ever climbs — 2, then
   3 after a space — and never falls. Under BASIC the same queue drains
   normally, so the queue itself works.
5. **A sweep of all 97 printable keys plus Return and Escape, each against an
   identical prefix, changed nothing** — not one produced a single differing
   frame hash over 400 frames. Re-run after the keyboard-latch and speaker
   fixes landed on master, with each key pressed **360 times across a full
   62-second attract cycle** rather than at one instant, in case the game only
   samples input at some phase: still nothing, for space, Return, `A`, `1`,
   `S` or Escape.

   The harness was checked against a positive control both times — the same
   comparison with the machine sitting in BASIC does detect a keypress — so
   this is a property of Neptune, not of the experiment.

This survived the keyboard rewrite. `a0671c1` ("the keyboard is a latch, not a
queue") and `9e9f89b` ("one access per write, so the speaker moves again") fixed
two real emulator bugs, and both are confirmed working here: a keypress reaches
BASIC, `PR#6` boots the disk, and Neptune's attract demo produces 878 speaker
segments including a repeated 1.1-second tone near 334 Hz. Neptune's silence
toward the keyboard is its own, not the emulator's.

Everything previously recorded — here and in `docs/a2mcp-playing.md` — as
"gameplay" was the attract demo. That includes the belief that space starts a
game and that `J`/`K`/`I`/`M`/`A` steer a ship. The ship visibly moves, enemies
appear, shields decrease: all of it is the demo playing itself, which is exactly
the trap `docs/a2mcp-playing.md` warns about and which this author fell into
anyway. The lesson is in that document; the evidence is here.

### What would unlock it

Implement the button switches in `lib/a2io`: `$C061`/`$C062`/`$C063` returning
bit 7 set while a virtual button is held, and expose a way to press one — a
`button` tool on `a2mcp`, or a key that maps to it. `$C064`/`$C065` (paddle
position, read via the `$C070` strobe and a timing loop) would be needed only if
the game also wants analogue steering; the control legend below suggests it
accepts the keyboard once started, so buttons alone may be enough.

Until then this document records the attract demo, which is still useful: it is
a real HGR program that exercises the renderers and the stop conditions.

## Reproducing anything here

**Prefix A — boot to the attract demo.** Every measurement below starts here.

```json
{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"n","version":"1"}}}
{"jsonrpc":"2.0","id":2,"method":"tools/call","params":{"name":"boot","arguments":{"disk1":"dsk/Neptune.dsk"}}}
{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"run","arguments":{"frames":600}}}
{"jsonrpc":"2.0","id":4,"method":"tools/call","params":{"name":"keys","arguments":{"text":"PR#6\n"}}}
{"jsonrpc":"2.0","id":5,"method":"tools/call","params":{"name":"run","arguments":{"frames":1500}}}
```

Feed it on stdin: `a2mcp --root=. < prefix.jsonl`. Add `--hash-frames=out.txt`
to get a per-frame video hash, which is how two runs are compared.

At the end of Prefix A: frame 2100, `hgr` page 1, full screen, `keys_pending: 2`.

## Determinism: the control

Prefix A plus 700 more frames, run twice in **separate processes**, produced
byte-identical `--hash-frames` output across every frame. That is the premise
this whole file rests on, so it is checked rather than assumed.

## Booting

The disk does **not** autoboot. `boot(disk1:"dsk/Neptune.dsk")` lands at a BASIC
`]` prompt; `PR#6` boots the drive. Under BASIC the keyboard works normally,
which is how `PR#6` gets typed at all — the input path is fine, it is Neptune
that ignores it.

## The attract demo

- Title screen: full-screen HGR, page 1 (`A2_VC_PAGE2` set), animated. Because it
  animates, `until:"screen_change"` fires after a single frame and is useless
  here; `until:"screen_update"` is the right tool.
- The demo plays a full game by itself: ship, terrain, enemies, explosions, and a
  HUD reading `FUEL`, `SC` (score), `HI`, `LV` (level), `SH`, `BO`.
- `SH` is set when a demo life begins and counts down as the demo ship takes
  hits — observed `3 → 2 → 1`. **It is not affected by anything you press.**
- **Never flips pages during the demo**, so every completed frame it produces is
  a settle, never a flip.
- `until:"screen_update"` works well on it: across twelve consecutive
  `run(frames=120, until="screen_update")` calls, eight landed on a completed
  picture (most within 4–12 frames) and four ran to the cap with the `note`.
  The demo's animation comes in bursts with idle gaps between them.

## Reading the screen

- Render with `color140`; the per-dot `color` mode shreds the terrain into
  stripes. The terrain is **solid green**, not dithered.
- The ship is a small compact white blob. The orange-red starburst near the top
  left is an **explosion**, not the ship — a distinction `mono140` settles
  instantly, since as an occupancy mask it shows the two as clearly different
  shapes.
- Enemies render magenta.

## Controls, per the disk's own legend

The disk image contains a key-map help screen. Recover it with:

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

It prints `LEFT J---- ----K RIGHT` and `I A -- -- -FIRE M`, i.e. an I/J/K/M
diamond with `A` to fire. **Unverified in practice** — no key has ever reached
the running program, so this is the game's own documentation, not an observation.

## Open questions

- Do the buttons at `$C061`/`$C062` actually gate starting a game, or is that
  inference from the read pattern? Implementing them is the experiment.
- What does `BO` count? It read `00` in some attract phases and `20` in others.
  An earlier note claiming `BO:20` proves a real game is **withdrawn** — both
  states were the demo.
- Is `A` really fire, and does the keyboard work at all once a game starts?
- Where do the ship's coordinates, `SH`, `BO` and the score live in RAM? Nothing
  here needed the answer yet, but a RAM map is what would turn playing from a
  vision problem into a state-reading one.
