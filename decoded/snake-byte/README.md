
To run the decompiled binary, after starting type `CALL 14160` in BASIC.

## Regenerating the run data

`snake-byte.json` is the recorded trace apple2tc decompiles against. It was
produced by a windowed `a2emu` in January 2022 and, until `a2run` existed,
nothing in the tree could reproduce it. The command is now:

```shell
a2run snake-byte.b33 --from-rom --collect --limit=3600348 \
      --key-file=play.keys --frames=1300 --out=new.json
```

**This does not reproduce the committed file byte for byte, and cannot.** The
2022 session's keystrokes and duration were never recorded, so its coverage
cannot be replayed. What the above produces is a *different session of the same
game*: 478 of the 500 branch targets match, the self-modification generations
are byte-identical, and the differences are all "this session went somewhere
else" — `func_64c8`'s region reached by one and not the other, a decimal `SBC`
executed by one and not the other.

So do not overwrite `snake-byte.json` casually. Every committed artefact
downstream — the generated C, `play.frames`, the coverage report — is keyed to
it, and replacing it invalidates all of them at once. Regenerate deliberately,
into a scratch file, and re-verify everything before adopting it.
