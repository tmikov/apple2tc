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

## The pacing primitive: `run(frames, settle)`

Cadence matters more than any one screenshot being untorn. An agent that wants
to look every ~100 ms of game time (`frames: 6`) needs that look to happen on
schedule; whether the frame it lands on is torn is a secondary concern. `run`'s
`settle` argument is built for exactly this: it advances `frames` frames on
schedule no matter what, then spends up to `settle` more frames looking for a
moment when the picture is complete before giving up and returning anyway.

**Use `run(frames: 6, settle: 12)` as the play loop.** That is six frames of
committed advance (100 ms of game time) plus up to twelve more spent trying to
land clean — 300 ms worst case, but usually far less, since a page flip or a
couple of quiet frames typically arrives well inside the grace window. The
reply's `stop_reason` tells you which happened: `"settled"` means the
screenshot you are about to take is a complete picture; `"unsettled"` means the
window ran out first and the shot may be torn, which is still worth knowing
before you look at it.

This is the opposite priority from `until: "screen_update"` below, and the
reason both exist. `screen_update` stops at the *first* complete picture,
however many frames that takes — it optimises for cleanliness and cannot hold
a cadence, because a game that redraws every fourth frame makes every call
take four times as long as one that redraws every frame. `run(frames, settle)`
holds the cadence and only takes cleanliness when it is cheap: it commits to
`frames` regardless, and treats anything better as a bonus within a bounded
budget. Reach for `screen_update` when landing one particular shot cleanly
matters more than when it arrives (say, a screenshot you're about to reason
about carefully); reach for `settle` for the steady perceive-act loop of
actually playing.

The same honest limitation applies as to `screen_update`, for the same
reason: a game that redraws every single frame has no idle frame boundary to
land on at frame resolution at all, so `settle` will report `"unsettled"` on
every call. That is not a wasted call — the frame count still advanced on
schedule, and the note that the shot may be torn is itself useful information
about the game you're driving.

**Spend your budget on `settle`, not on `frames`.** The two are not
interchangeable: `frames` is a floor you pay on every look whether or not it buys
anything, while `settle` is spent only when the screen is not yet clean. So the
sum of the two sets how often you land untorn, and the split between them sets
how long an average look takes. Measured on `dsk/Neptune.dsk` during play, thirty
looks each, holding the ceiling at 18 frames:

| `frames` + `settle` | landed untorn | cadence |
|---|---|---|
| 6 + 12 | 17 of 30 | ~218 ms |
| 4 + 14 | 18 of 30 | ~194 ms |
| 2 + 16 | 18 of 30 | ~183 ms |
| 1 + 17 | 18 of 30 | ~178 ms |

Shifting the same budget from the floor into the window improves both axes at
once. And the ceiling is what governs cleanliness — at a ceiling of 12 the same
game lands 9 of 30, at 10 it lands 5 of 30, whatever the split.

So: **set `frames` to the smallest value that suits your input, and put
everything else in `settle`.** The one thing `frames` must cover is key delivery,
if you want your input applied before you look — a burst of `n` keys at
`frames_between: k` takes `(n-1) * k` frames to arrive, and a run that lands
clean before then returns a picture from before your move. With a three-key burst
this never triggered in ninety looks, because a typical look outlasts the burst;
with a long burst it certainly would.

**Calibrate during play, not after boot.** An earlier version of this section
reported that small windows land cleanly *essentially never*. Those measurements
were real but were all taken in the first 2400 frames after boot, which for this
game is title and attract animation — continuously redrawing, with no clean frame
boundary anywhere in it. The identical call lands untorn 17 times in 30 during
play and 0 times in 30 over the title screen. Skip past the intro before you
calibrate, and re-check when the game changes mode.

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

**Do not assume from the genre which detector will fire -- measure.** The
reasonable guess is that a continuously scrolling action game never settles and
that only double-buffered games are servable. Both halves of that guess were
wrong on the games in this repo: a scrolling shooter settled on two thirds of
sampled windows because its animation comes in bursts with idle gaps between
them, while *neither* game here flips pages during play (the one that flips does
so only in its menu). Expect settles to do the work and flips to be rare, and
spend one cheap experiment finding out for your game: a handful of
`run(frames=120, until="screen_update")` calls during play, counting how many
stop early versus hit the cap. The worked example at the end of this document
shows that measurement and its numbers.

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

**First check you are delivering keystrokes and not type-ahead.** `keys`
defaults to `deliver: "press"`, which overwrites the keyboard latch exactly as a
live key in `a2emu` does. The other mode, `deliver: "type"`, stages behind an
unread strobe — correct for typing text into BASIC, and invisible to a game that
polls `$C000` without acknowledging through `KBDSTRB`. Getting this backwards
made Neptune look completely unresponsive for an entire session, and produced a
confident and wholly wrong writeup about the game waiting on a joystick button.
See `docs/neptune.md`.

The lesson is that a changing counter proves the program is *running*, not that
it is *listening*. What actually proves listening is a controlled comparison:
run an identical prefix with and without the keypress and diff the per-frame
hashes (`--hash-frames`). On a deterministic machine any difference is caused by
your input and nothing else. That experiment is cheap, and it is the only thing
here that would have caught the mistake.

**A full keyboard queue means the program stopped reading it.** `status` reports
`keys_pending`, and the Apple II's hardware queue holds 32. If it sits at 32, the
program is not consuming keys at all -- it has died, paused, entered a cutscene,
or gone to a screen with its own input loop. That is a state change you would
otherwise only infer from the picture. Note what it cannot tell you on its own:
for Neptune the count climbs from the first keystroke and never falls, which
turned out to mean the program never reads the keyboard *at all*, not that it
had briefly stopped.

More generally, when you are unsure whether a keypress reached the program:
press a key that should do something visible, step, and compare — with `screen`
in `text` or GR mode if the game is in one of those, since a 40x24 grid diffs by
eye far more reliably than a hi-res picture.

## `blend`: a single frame can be missing an object entirely

Single-buffered games draw by erasing and redrawing sprites, so at any given
frame boundary part of the picture is simply missing. A human playing never
notices: at 60 Hz the eye integrates across frames and sees a stable (if
flickery) image. A model taking one snapshot does not integrate, and sees a
hole. Measured on `dsk/Neptune.dsk` during play, across eight consecutive
single frames and counting only the sky rows where sprites live: not one sky
pixel survived all eight frames, and on one of the eight an entire enemy
disappeared -- a screenshot of that frame shows no enemy at all, not a dim
one. Roughly one snapshot in eight, on the game measured, is missing an
object outright.

`blend: 2` or `blend: 3` is what to reach for when reading a scene rather than
stepping through one: it composites the last N frames' HGR page with a
bitwise OR before rendering, so a dot lit in any of them is lit in the
result -- reconstructing, in software, what a human's eye already does at
60 Hz. `settle` (above) does not substitute for this: `settle` finds a moment
when *drawing has paused*, which is the wrong question for a game that never
stops redrawing its sprites every frame. Two consecutive settled frames from
such a game are still two frames each missing part of the picture; blending
them is what actually completes it. The two are complementary, not
alternatives -- `settle` addresses a scene caught mid-scroll, `blend`
addresses a scene missing an object, and a careful read of an animating scene
may want both: settle to land on a clean frame boundary, then blend a few
frames from there.

The cost is real and worth keeping in view. A moving object smears across the
N frames it appears in -- genuine motion blur, which helps read which way
something is heading and hurts reading exactly where it is right now. And in
`color140`, which decodes at colour-clock resolution, OR-ing two frames' bit
patterns together can produce a colour neither frame actually displayed: the
composite is a bitwise fact about the underlying dots, not a resample of
either picture. Both costs grow with N, which is why `blend` tops out at 8
and why 2 or 3 is the sensible range to reach for rather than the maximum.

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

Neptune has its own notebook, `docs/neptune.md`, because a deterministic machine
makes per-game facts worth accumulating: they can be re-checked rather than
believed. It also records a mistake worth reading before you trust your own
measurements — a controlled experiment that was rigorous, had a positive
control, passed it, and still reached a false conclusion, because the control
exercised a cooperative program and the case under test did not.

## If this needs to auto-load

It is a plain document because that is how this repo distributes knowledge —
`CLAUDE.md` names the tool and points at `docs/a2mcp.md`, which points here. If
it ever needs to load itself into an agent's context on a matching task, a
Claude Code skill is this file plus a small frontmatter block, moved to
`.claude/skills/a2mcp-playing/SKILL.md`.
