# [WIP] Decoding Bolo

This directory contains the work in progress of "decoding" BOLO into C. The
contents here evolve simultaneously with the work on the decompiler.

## Files

`bolo.b33` is the Apple II binary from ftp.apple.asimov.net/images/games/file_based/.
When loaded part of it overlaps the first Hires graphics page, so it moves its second
part from address $4000 to address $6000, essentially leaving a "hole" in the middle
of the image.

`bolo-relocated.b33` is the larger image saved after the copy. This image was manually
constructed in `Id` using `memcpy` and `saveb33`. This image cannot be directly
executed because it still has code in the beginning performing the "relocation"
and part of the code to be relocated has been overwritten.

[preamble.s](preamble.s) is a stub that jumps over the relocation code at the start of
`bolo-relocated.b33` and sets up the graphics mode, and sets the memory and
registers to look as if the relocation just occurred. That last pat may not be
necessary but we are playing it safe. It has to be assembled with `a6502` into
`preamble.b33`, which can be loaded just before the original image.

`bolo-patched.b33` is concatenation of `preamble.b33` and `bolo-relocated.b33`
performed using `id`. This image can be executed and seems to work fine in `a2emu`.
