# Driving a2mcp to play a game

`docs/a2mcp.md` says what the six tools do. This says how to use them to boot an
unknown disk, work out its controls, and play it — the parts that are not
obvious from the tool descriptions, and the two or three that are actively
counter-intuitive.

Everything here was learned driving the finished server against
`dsk/Neptune.dsk`. Where something was observed once rather than established,
it says so.

## The shape of a session

    boot → reach a stable state → identify the controls → play in fixed steps

Only the third step is unusual, and it is the one that saves the most time: for
a game you have never seen, the control scheme is usually printed *inside the
disk image*, and reading it there costs one shell command instead of a dozen
guesses at a hundred frames each.

## Stepping: `screen_change` to arrive, fixed frames to play

`until: "screen_change"` stops on the first frame whose displayed screen differs
from the frame the call started on. That makes it exactly right for *getting
somewhere*:

- waiting out a disk boot,
- waiting for a loading screen to finish,
- advancing to the next thing worth looking at when you have no idea how long it
  takes.

It is useless the moment anything on screen animates, which in a game is
essentially always. Neptune's title screen animates, so
`run(frames=900, until="screen_change")` returned `frames_run: 1` — every time.
That is the condition working correctly and telling you nothing.

**Once the game is live, step by a fixed number of frames.** A frame is 1/60 s
of emulated time, so pick the step from how fast the game moves and how much you
are willing to miss:

| step | emulated | good for |
|---|---|---|
| 60 | 1 s | reacting to something you expect imminently |
| 150-260 | 2.5-4.5 s | ordinary play; one burst of input per step |
| 600+ | 10 s+ | attract loops, waiting out an intro |

You are blind between screenshots. A 260-frame step means four seconds of game
happened without you, so send the input for that whole window *before* the step,
not after it.

## Landing on a finished picture: `screen_update`

`screen_change` and `screen_update` answer different questions. `screen_change`
answers "has anything happened at all" — useful for *noticing*, and exactly the
wrong tool once a game is animating, per the previous section. `screen_update`
answers a narrower question: "has a complete picture been reached" — useful for
*landing a screenshot cleanly* rather than catching a frame half-drawn. Reach for
`screen_change` to know something happened; reach for `screen_update` right
before you take the screenshot you actually intend to look at.

It works by counting two things as the same "update": a page flip (a
double-buffered game presenting a finished frame — this can never be torn, since
the picture is only visible once it is entirely drawn) and a settle (the screen
changed, then held identical for a couple of frames — a single-buffered game's
idle gap between draws, if it has one). An already-static screen at the start
never counts, so a `screen_update` call always means "advance to the next
completed picture," not "return instantly because nothing is animating right
now" — that instant-return case is what `screen_change` already covers.

**Deliver your input before you ask for an update.** Keys are scheduled, so a
burst of them is still arriving over the next many frames, and an update fires at
whatever frame boundary satisfies it -- very likely one with half your keystrokes
delivered. The working order is three calls: schedule the keys, `run` enough
plain frames to deliver them (`frames_between` times the number of keys), and
only then `run` with `until: "screen_update"` to land the shot. Doing it in two
calls lands you on a picture of a half-executed intention.

**The honest limitation:** a single-buffered game that redraws every single
frame with no idle gap has no complete-frame boundary at frame resolution at
all. `screen_update` cannot find what is not there — the call runs to its frame
cap and says so in the reply (`updates` short of what was asked, plus a `note`).
That is not a wasted call: the note itself is the answer, and it means a
screenshot taken right now may be torn. For a game like that, the tool that
actually helps is a probe planted at the game's own draw-complete point in its
code (`docs/probes.md`) — `screen_update` only ever sees frame boundaries, and a
game with no idle frame boundary has nothing at that resolution to see.

## Booting a disk: it does not autoboot

`boot(disk1: "...")` mounts the image and brings the machine up, and you land at
a BASIC `]` prompt, not in the game. Type `PR#6` to boot the drive:

    keys("PR#6\n") ; run(1200)

Observed with `dsk/Neptune.dsk`: still at the prompt after 600 frames, booted
within 1200 of `PR#6`. **The cause is not established.** `reset_regs()` runs
before `init_emulated()` loads the ROM in `a2host_init_emulation()`, which is
worth looking at if this ever matters more than the one extra tool call — but
nothing here confirms that is the reason.

## `boot` does not clear RAM

Rebooting re-initialises IO and the engine; it does not zero memory. In a
long-lived server the first `screen` after a `boot` can show the *previous*
session's screen. Neptune's boot showed a BASIC listing left over from an
earlier session in the same process.

Take it as a rule: after `boot`, do not trust a screenshot until you have run
enough frames for the new program to have drawn something.

## Finding the controls without guessing

Games of this era print their key map on a help screen, so the text is sitting
in the disk image in high-bit ASCII. Search for it:

```bash
python3 - <<'EOF'
d = open("dsk/Neptune.dsk", "rb").read()
cur, out = "", []
for b in d:
    c = b & 0x7F                      # Apple II text has the high bit set
    if 32 <= c < 127:
        cur += chr(c)
    else:
        if len(cur) >= 8: out.append(" ".join(cur.split()))
        cur = ""
for s in out:
    u = s.upper()
    if any(k in u for k in ("KEY", "ARROW", "FIRE", "LEFT", "RIGHT", "PRESS")):
        print(repr(s))
EOF
```

For Neptune that prints the key diagram directly:

    'LEFT J---- ----K RIGHT . W #'
    'I A -- -- -FIRE M M'

— an I/J/K/M diamond with `A` to fire. Worth trying before anything else.

**There are no paddles or buttons.** `lib/a2io` implements no `$C061`-`$C06F`
handling at all, so a game that wants a joystick or a paddle button cannot be
played through this server, and a game that *starts* on a button press cannot be
started. The keyboard is the whole input surface.

## Confirming your input actually landed

The trap is attract mode: a demo that plays itself looks exactly like a game you
are controlling, and you can spend a long time "playing" a movie. Do not judge
from the picture — judge from the status line.

For Neptune, the game showed `BO:20` (bombs) where the attract loop showed
`BO:00`, and `SH` moved `00 → 04 → 03` across a firing burst and a hit. Counters
that only a real game maintains are the cheap proof.

**A full keyboard queue means the program stopped reading it.** `status` reports
`keys_pending`, and the Apple II's hardware queue holds 32. If it sits at 32, the
program is not consuming keys at all -- it has died, paused, entered a cutscene,
or gone to a screen with its own input loop. That is a state change you would
otherwise only infer from the picture, and it is how a mid-session death was
spotted here: shields had been dropping, then input stopped being read entirely
while the death animation played.

More generally, when you are unsure whether a keypress reached the program:
press a key that should do something visible, step, and compare — with `screen`
in `text` or GR mode if the game is in one of those, since a 40x24 grid diffs by
eye far more reliably than a hi-res picture.

## Reading a hi-res picture: pick the render mode

`screen`'s default `render:"color"` is the per-dot artifact approximation the
GUI has always drawn, and it is the wrong choice for looking *at* the picture:
an isolated white feature can render violet in one column and green in the
next, purely from which column it happens to fall on. Two modes fix that for
different purposes:

- `render:"color140"` decodes at the Apple II's true colour-clock resolution,
  so a feature comes out in one consistent colour instead of a dotted
  necklace. Use it when you need to read what a hi-res scene actually shows.
- `render:"mono140"` collapses those same colour-clock cells to
  white-if-either-dot-is-set, black-if-neither -- an ink/no-ink occupancy
  mask with no colour to reason about. Use it for finding shapes (a sprite,
  a wall, a bullet), the same job you might expect a plain 1-bit rendering
  to do.

Reach for `mono140` when the question is "where is it", and `color140` when
the question is "what does it look like".

There is no `render:"mono"` here, and that is deliberate rather than an
omission. A colour fill on this hardware *is physically* an alternating dot
pattern -- HCOLOR green, for instance, is every other dot lit, not a solid
run of dots that happens to look green. A faithful bit-for-bit monochrome
rendering of that is therefore not a solid region at all; it is a
one-pixel-pitch comb. Measured across a solid-green terrain row:

    per-dot:      . # . # . # . # . # . # . # . # . # . #
    colour-clock: # # # # # # # # # # # # # # # # # # # #

Run a connected-components pass over the left row and a single landmass
turns into 140 one-pixel slivers. `mono140` is built at colour-clock
resolution specifically so a colour fill lands as the solid region it
visually is -- that is what makes it the tool for finding shapes, and why a
truthful per-dot monochrome view is the wrong one for that job even though
it sounds like it should be the simplest choice.

## `scale`: read `color140`/`mono140` at their native width

`color140` and `mono140` decode at 140-wide colour-clock resolution and then,
by default, double each cell back out to 280x192 so the picture lines up with
`color`. That doubling adds nothing to look at -- it is the same information
twice -- and an image's token cost scales with its area, so the default
`scale: 2` makes every one of these screenshots cost twice what it needs to.

Pass `scale: 1` to get the 140x192 image instead. It is the sensible default
for an agent reading a scene repeatedly: half the payload, with no
information lost -- column *k* of the `scale: 1` image is exactly column
*2k* (and *2k+1*) of the `scale: 2` image, not a resample or a crop.

The cost is honest, not hidden: at `scale: 1` the image is horizontally
compressed relative to how the machine actually displays it, so shapes read
distorted -- a circle comes out as an ellipse -- even though relative
positions are preserved. Reach for `scale: 2` (or leave it at its default)
whenever the question is about a shape rather than a position; `scale: 1` is
the right default when it isn't.

## Spacing keys is how you emulate holding one down

`keys` schedules; `frames_between` sets the gap. There is no key-repeat and no
key-down state — a real Apple II keyboard hands the program one character at a
time, and so does this.

- `frames_between: 4-8` reads like a held direction key: `keys("KKKKKK", 5)`
  moves steadily right for half a second.
- `frames_between: 0` stamps everything at the current cycle and overruns the
  32-entry hardware queue past 32 keys. Only for short bursts.
- Interleave to act within one step: `keys("KKKKAAAAKKKKAAAA", 5)` moves and
  fires across about 80 frames, which is one ordinary step's worth of play.

Schedule the whole burst, then `run` once. Two `keys` calls with a `run` between
them cost twice the turns for the same emulated time.

## What the session actually costs

Emulated time is free — the machine is frozen between calls, so thinking is free
too. What you spend is **turns**: one perceive-act cycle is a `keys`, a `run`
and a `screen`, and a few hundred of those is a comfortable budget.

That rules some things in and some out. A game that pauses for input, or one
where four seconds of drift is survivable, plays fine. A twitch game does not:
you will get through a few seconds of it and learn little. Prefer the cheapest
representation that answers your question — `screen(format:"text")` is about a
kilobyte and an HGR PNG is far more, so read the text screen when the game is in
one, and only pay for an image when the game is in hi-res.

## Worked example: Neptune (Gebelli Software)

Verified in one session:

| | |
|---|---|
| boot | `boot(disk1:"dsk/Neptune.dsk")`, then `keys("PR#6\n")`, `run(1200)` |
| title | full-screen HGR, page 2, animated — `screen_change` fires every frame |
| start | space, from the title |
| in game? | `BO:20` in a real game, `BO:00` in the attract loop |
| controls | `J` left, `K` right, `I` up, `M` down, `A` fire (from the disk's own legend) |

Measured while actually playing, which corrects two guesses in an earlier draft
of this table:

- **`SH` is a shield count that decreases as you are hit** -- observed `3 -> 2 ->
  1` across one session. The earlier note here, that pressing `A` moved `SH` from
  `00` to `04` and so could not be a fire button, was a misreading: that
  transition was the game *starting* and granting shields, not the keypress.
- **`screen_update` works on this game during real gameplay.** Across twelve
  consecutive `run(frames=120, until="screen_update")` calls in live play, eight
  landed on a completed picture -- most within 4 to 12 frames -- and four ran to
  the cap with the `note`. So Neptune does have idle gaps between its animation
  bursts, contradicting a reasonable prior guess that a continuously scrolling
  game would never settle. Do not assume; measure the game in front of you.
- **Neptune never flips pages during play**, so every update it produces is a
  settle. Bolo does flip, but only in its attract and configuration menu, not in
  gameplay. Between the two games in this repo, the page-flip detector currently
  only ever fires on a menu.
- **Your ship is a small compact white blob**; the orange-red starburst that
  appears near the top left is an explosion, not you. Confirm with `mono140` if
  in doubt -- as an occupancy mask it separates the two shapes immediately -- or
  use the perturbation trick above.

Still not established: what `BO` counts, and whether space is the only way to
start.

## If this needs to auto-load

It is a plain document because that is how this repo distributes knowledge —
`CLAUDE.md` names the tool and points at `docs/a2mcp.md`, which points here. If
it ever needs to load itself into an agent's context on a matching task, a
Claude Code skill is this file plus a small frontmatter block, moved to
`.claude/skills/a2mcp-playing/SKILL.md`.
