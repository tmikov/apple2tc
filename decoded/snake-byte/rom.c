/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The Apple II ROM entry points Snake Byte calls, as ordinary C.
///
/// They are the machine's code rather than the game's, which is why they live
/// apart from it -- but they are decompiled on the same terms as everything
/// else here. There is no category of code in this program that belongs to
/// someone else.

#include "apple2tc/system2.h"

#include "clock.h"
#include "rom.h"

/* The monitor's own zero page. Declared in rom.h, which explains what it
   is and why the game touches it at all. */
struct Monitor s_mon;

/* ========================================================================== *
 * Apple II ROM entry points, hand-written                                  *
 * ========================================================================== */

/* COUT1, defined below. rom_cout dispatches to it, and so does the game's own
   hi-res text handler once it has drawn its glyph. */


/* ========================================================================== */
/* Private helpers.                                                           */
/*                                                                            */
/* $F847 GBASCALC and $F80E PLOT1 used to be emitted as `FUNC_GBASCALC` and    */
/* `FUNC_PLOT1`. Their only callers were $F800/$F819/$F871, so once those      */
/* became external the two helpers became unreachable and the decompiler       */
/* dropped them. They are reproduced here verbatim from the pre-externs        */
/* output.                                                                     */
/* ========================================================================== */

/// $F847 GBASCALC. The lo-res twin of BASCALC: a row 0-47 in A becomes that
/// row's base address in GBASL/GBASH.
///
/// Same shape as BASCALC and the same trick -- LSR puts the row's low bit in
/// the carry, the next three bits pick the band, and the `ADC #$7F` folds in
/// the half. It differs in the tail: BASCALC ORs the shifted value back in to
/// build a text address, and this one shifts by two and ORs, which lands on
/// the lo-res page instead.
static void rom_gbascalc(uint8_t row) {
  const uint8_t odd = row & 0x01;
  const uint8_t page = ((row >> 0x01) & 0x03) | 0x04;
  uint8_t band = row & 0x18;

  if (odd) {
    band = (band + 0x7f) + odd;
  }

  // The cast is the ASL: band reaches $98, so the shift carries out of the
  // byte, and dropping it would leak bit 9 into the page below.
  s_mon.gbas = ((uint8_t)(band << 0x02) | band) | (page << 8);
}

/// $F80E PLOT1. Store the color mask ($30) into the lo-res half-byte selected
/// by MASK ($2E) at GBASL/GBASH ($26) + Y.
static void rom_plot1(uint8_t col) {
  // One lo-res cell: replace the half of the byte MASK selects with the
  // matching half of COLOR, leaving the other half alone. `(old ^ colour) &
  // mask ^ old` is the ROM's way of saying that in three instructions.
  const uint16_t at = s_mon.gbas + col;
  const uint8_t old = peek(at);
  poke(at, ((old ^ s_mon.color) & s_mon.mask) ^ old);
}

/* ========================================================================== */
/* $F800 PLOT                                                                 */
/* ========================================================================== */

/// $F800 PLOT. Light the lo-res cell at row A, column Y, in COLOR.
///
/// Two cells share a byte, so the row's low bit picks which nibble -- MASK
/// becomes $0F for the lower half and $F0 for the upper. The ROM gets there by
/// keeping the bit on the stack across GBASCALC and then adding $E0 to $0F,
/// which is $F0 with the carry set and $EF without; only the low bit of the
/// row can make the difference, so the sum is one of the two masks.
void rom_plot(uint8_t row, uint8_t col) {
  const uint8_t half = row >> 0x01;
  const bool upper = (row & 0x01) != 0;
  // PHP/PLP across GBASCALC, to carry the LSR's bit past a call that clobbers
  // the flags. The bit is `row & 0x01`, and nothing else survived the round
  // trip, so it is simply the value.
  rom_gbascalc(half);

  const uint8_t carry = row & 0x01;
  uint8_t mask = 0x0f;

  if (upper) {
    mask = (mask + 0x00e0) + carry;
  }

  s_mon.mask = mask;
  rom_plot1(col); // JMP -- a tail call.
}

/* ========================================================================== */
/* $F819 HLINE                                                                */
/* ========================================================================== */

/// $F819 HLINE, and $F826 VLINEZ, which share this body.
///
/// HLINE plots along row A from column Y to H2. $F826 sits immediately below
/// its loop and walks rows from A down to V2 in column Y instead -- so one
/// entry point draws a horizontal run and the other a vertical one, out of one
/// stretch of code. Adjacent in the layout, not in the flow: the $F824 BCC
/// that closes HLINE's loop is taken every time (see below).
///
/// `down:` is present and unreached, and it stays. In the ROM $F826 is an entry
/// point in its own right, so the code has to be here; in this file it has no
/// caller, because rom_hline() is the only way in and the `across` loop cannot
/// fall out of the bottom into it -- `carry` is 0 at the `if (!carry)` re-test
/// (that is why the `if (carry)` above it did not branch away) and rom_plot1
/// cannot change it, so `across` always leaves through `done`. The labels keep
/// their addresses because that is the shape the ROM's own layout has, not
/// because this file's flow needs them.
void rom_hline(uint8_t row, uint8_t from_col) {
  uint8_t col = from_col;
  // The CMPs below leave it and the ADC at $F826 reads it back.
  uint8_t carry;

  rom_plot(row, col);

across: /* one column at a time, up to H2 */
  carry = col >= s_mon.h2;
  if (carry) {
    goto done;
  }

  col = col + 0x01;
  rom_plot1(col);
  if (!carry) {
    goto across;
  }

down: /* one row at a time, up to V2 */
  row = (row + 0x0001) + carry;

  rom_plot(row, col);
  carry = row >= s_mon.v2;
  if (!carry) {
    goto down;
  }

done:
  return;
}

/* ========================================================================== */
/* $F864 SETCOL                                                               */
/* ========================================================================== */

void rom_setcol(uint8_t ink) {
  // The lo-res colour is stored in both nibbles, so a PLOT can take whichever
  // half MASK selects without shifting. Four ASLs and an ORA get there in the
  // original; the carry they leave is read by nothing.
  const uint8_t low = ink & 0x0f;
  s_mon.color = (low << 0x04) | low;
}

/* ========================================================================== */
/* $F871 SCRN                                                                 */
/* ========================================================================== */

uint8_t rom_scrn(uint8_t row, uint8_t col) {
  // The row's low bit says which half of the byte holds this cell, and the
  // ROM keeps it across GBASCALC on the stack -- as the whole status
  // register, because LSR put it in the carry and PHP is one byte.
  const uint8_t half = row >> 0x01;
  const bool upper = (row & 0x01) != 0;

  rom_gbascalc(half);

  uint8_t cell = peek(s_mon.gbas + col);

  // The original brackets GBASCALC with PHP/PLP to keep the LSR's carry. It is
  // read by nothing here.

  if (upper) {
    cell = cell >> 0x04;
  }

  return cell & 0x0f;
}

/* ========================================================================== */
/* $FC58 HOME                                                                 */
/* ========================================================================== */

/// $FC58 HOME. Clear the text window and put the cursor at its top left.
///
/// One line at a time from WNDTOP: VTAB to it, CLREOLZ it, next. The ROM
/// carries the line number on the stack across both calls because VTABZ and
/// CLREOLZ each destroy A.
void rom_home(void) {
  uint8_t line;

home: /* $FC58 */
  line = s_mon.wndtop;
  s_mon.cv = s_mon.wndtop;
  s_mon.ch = 0x00;
  // $FC60 BEQ -- provably always taken (Y was just loaded 0), but the branch
  // instruction still executes and still pays its own cost every time. The
  // decompiler doesn't do cross-instruction flag proofs either, so it keeps
  // charging this the same way.

  for (;;) { /* CLRSC2, one line per pass */
    rom_vtabz(line);
    // $FC5C loaded Y with 0 and CLREOLZ is the only thing that moves it, so
    // every pass starts the blank at column 0. Its carry comes back out as the
    // +1 below: $FC4D's `ADC #$00` has no CLC in front of it, and this is why.
    const bool step = rom_clreolz(0x00);

    line = line + step;

    const bool past_bottom = line >= s_mon.wndbtm;
    if (!past_bottom) {
      continue;
    }

    if (!past_bottom)
      goto home; // the BCS's not-taken arm, which cannot be reached
    break;
  }

  // TABV
  rom_vtabz(s_mon.cv); // JMP -- a tail call.
}

/* ========================================================================== */
/* the tail of LF: VTAB, or scroll if CV has run off the window.      */
/*                                                                            */
/*   FC68: LDA CV / CMP WNDBTM / BCC VTABZ    ; usual case: just recompute BAS */
/*   FC6E: DEC CV / LDA WNDTOP / PHA / ...    ; otherwise scroll up one line   */
/*                                                                            */
/* Transcribed from the pre-externs mega-switch blocks $FC68..$FC95 plus the   */
/* three exit blocks ($FC6C -> VTABZ, $FC22, $FC9A -> CLREOL).                 */
/*                                                                            */
/* Besides COUT1 falling through $FC66, the game calls this directly as a      */
/* VTAB: $7590 and $75D1 store CH/CV and JSR here. Their CV values are 5..13,  */
/* well under WNDBTM ($18), so they always take the BCC and never scroll.      */
/* ========================================================================== */

/// $FC68. The tail of the ROM's line feed: VTAB to CV, or scroll the window up
/// one line first if CV has run past the bottom.
///
/// The game calls it directly as a plain VTAB -- $7590 and $75D1 store CH/CV
/// and JSR here -- with a CV always well under WNDBTM, so the scroll below
/// never runs on their behalf. See a2rom.h.
///
/// The scroll copies each line over the one above it, back to front, keeping
/// the source line's base in BASL and the destination's in BAS2L.
void rom_fc68(void) {
  /* The scroll's N. It was s_status_n while every routine in this file
     maintained the whole status register; this is the only routine that ever
     read the flag, and both reads are below, inside and just after the copy
     loop, on paths the DEY that sets it dominates. So it is a local, and the
     other thirty-nine writes of s_status_n in this file were dead stores and
     are gone.

     Never executed by any test: nothing the game does scrolls, so inverting
     the test below goes unnoticed. This rests on the dominance argument
     above, not on a passing run. */
  uint8_t negative = 0;

  uint8_t line;
  uint8_t col = 0;
  bool step = false;
  /* Declared here rather than at $FC97 because `last_line` is a goto target,
     and a label may not be followed by a declaration in C11. */
  bool filled;

  if (!(s_mon.cv >= s_mon.wndbtm)) {
    // Nothing to scroll.
    rom_vtabz(s_mon.cv); // JMP -- a tail call.
    return;
  }

  s_mon.cv = s_mon.cv - 0x01;
  line = s_mon.wndtop;
  step = rom_vtabz(line);

scroll: /* one line up per pass */
  s_mon.bas2 = s_mon.bas;
  col = s_mon.wndwdth - 0x01;
  // $FC82's ADC has no CLC either; the carry is whatever VTABZ last returned.
  line = (line + 0x0001) + step;

  if (line >= s_mon.wndbtm) {
    // That was the last line.
    goto last_line;
  }

  step = rom_vtabz(line);

copy: /* one character, right to left */
{
  const uint8_t at = col;
  poke(s_mon.bas2 + at, peek(s_mon.bas + at));
  const uint8_t next = at - 0x01;
  negative = next & 0x80;
  col = next;
  if (!negative) {
    goto copy;
  }
}

  if (negative) {
    goto scroll;
  }

last_line: /* blank what the scroll left at the bottom */
  filled = rom_clreolz(0x00);

  if (!filled) {
    rom_clreol(); // JMP -- a tail call.
    return;
  }
  // Falls into the trampoline charge below before continuing; the not-taken
  // arm above jumps straight out without it.

  // TABV
  rom_vtabz(s_mon.cv);
}

/* ========================================================================== */
/* $FB78 COUTZ - the body of COUT1, reached through $FB94 -> $FBFD.           */
/*                                                                            */
/* Transcribed from the pre-externs mega-switch blocks $FB78..$FB94,          */
/* $FBD9..$FBEF, $FBF0..$FBFC, $FC01..$FC2B and $FC62..$FC66. The whole        */
/* subtree disappeared from the generated C once $FDED became external,        */
/* because $FDED was its only entry.                                          */
/*                                                                            */
/* Path coverage, measured by instrumenting this file and replaying the        */
/* 1300-frame play.keys session:                                              */
/*                                                                            */
/*   $FBF0 store char      402 hits    $FC62 CR            3 hits             */
/*   $FC68 scroll           39 hits    $FC66 LF            3 hits             */
/*   $FBD9 CMP #$87          1 hit     $FBDD bell body     1 hit              */
/*   $FBE4 speaker click   192 hits    $FB7C CR kbd poll   3 hits             */
/*   $FB85/$FB88 Ctrl-S      0 hits    $FC10 backspace     0 hits             */
/*                                                                            */
/* So the bell is live: ROM RESET reaches it via $FF3A (LDA #$87; JMP $FDED),  */
/* and the 192 clicks are the $FBE2 `LDY #$C0` loop. It is oracle-verified.    */
/*                                                                            */
/* Backspace ($FC10) is unreachable from Snake Byte, not merely untraced: no   */
/* byte the game emits is $88. The $08 inside the "CRASH" string at $7868 is   */
/* the trap for the eye - it has bit 7 clear, so $FC01 (TAY; BPL) routes it to */
/* $FBF0 as an inverse glyph long before the CMP #$88 at $FC0C.                */
/*                                                                            */
/* The Ctrl-S handshake ($FB85/$FB88) is reachable but untraced: $FB7C does    */
/* run (A == $8D three times), it just never saw a pending $93. It fires only  */
/* if the user holds Ctrl-S while a CR is being output. Transcribed but not    */
/* covered by the oracle - the one part of this file in that category.         */
/* ========================================================================== */

/// $FB78 COUTZ. The monitor's character-output dispatcher: printable
/// characters go on the screen, the four control codes it knows do their
/// thing, and everything else is dropped.
///
/// Entered from COUT1, which has already masked the character with INVFLG.
///
/// **Several of its arms never run.** Everything this game prints is
/// printable, a carriage return, or a line feed. The Ctrl-S screen hold, the
/// backspace and the bell rest on the binary alone.
///
/// The labels keep their addresses. This is a dispatcher whose arms rejoin at
/// several depths -- backspace falls into the cursor-up path, a wrapped line
/// falls into the carriage return, and a carriage return falls into the line
/// feed -- and writing it as nested ifs would need each of those spelled out
/// twice.
static void rom_coutz(uint8_t ch) {
  if (ch != 0x8d) {
    goto emit;
  }

  /* the Ctrl-S handshake, on a carriage return only. If a key is
     already waiting and it is Ctrl-S, stop here until another key arrives.
     Ctrl-C is left in the keyboard latch on the way out so that whatever is
     running next still sees it; anything else is consumed. */
  GAME_CYCLES_COORD(0xfb7c, 6);
  uint8_t key = io_peek(0xc000);
  if (!(key & 0x80)) {
    goto emit;
  }

  if (key != 0x93) {
    goto emit;
  }

  for (;;) { /* spin until a key is pressed */
    // Ctrl-S holds the display until the next keypress, and this charge is
    // what makes the hold end: it is the only suspend point in the loop, so
    // without it the host never runs and the key can never be delivered.
    // Seven cycles is one pass of the original.
    advance(7);
    key = io_peek(0xc000);
    if (!(key & 0x80)) {
      continue;
    }

    if (key == 0x83) {
      // Ctrl-C: leave it latched.
      break;
    }
    break;
  }

emit: /* $FB94 JMP $FBFD */
  if (!(ch >= 0xa0)) {
    // The not-taken arm jumps straight to the dispatch without the edge charge.
    goto dispatch;
  }
  // Falls into the trampoline charge before continuing.

store: /* put the character at the cursor */
  poke(s_mon.bas + s_mon.ch, ch);

  s_mon.ch = s_mon.ch + 0x01;
  {
    const uint8_t width = s_mon.wndwdth;
    const bool past_right_edge = s_mon.ch >= width;
    if (past_right_edge) {
      // Off the right edge, so wrap: the same thing a carriage return does.
      goto carriage_return;
    }
  }
  goto out;

dispatch: /* not printable; which control code is it? */
  if (!(ch & 0x80)) {
    // Below $80 the monitor stores it anyway, high bit and all.
    goto store;
  }

  if (ch == 0x8d) {
    goto carriage_return;
  }

  if (ch == 0x8a) {
    goto line_feed;
  }

  if (ch != 0x88) {
    goto bell;
  }

  /* backspace. Off the left edge wraps to the end of the line above,
     which is why it falls into the cursor-up path rather than returning. */
  {
    const uint8_t back = s_mon.ch - 0x01;
    s_mon.ch = back;
    if (!(back & 0x80)) {
      goto out;
    }
  }

  s_mon.ch = s_mon.wndwdth;
  s_mon.ch = s_mon.ch - 0x01;

  {
    const uint8_t top = s_mon.wndtop;
    const uint8_t cv = s_mon.cv;
    const bool at_window_top = top >= cv;
    if (at_window_top) {
      // Already on the window's top line; there is nowhere to go up to.
      goto out;
    }
  }

  s_mon.cv = s_mon.cv - 0x01;
  // TABV
  rom_vtabz(s_mon.cv);
  goto out;

bell: /* Ctrl-G, or a control code the monitor does not know */
{
  const uint8_t differs = ch != 0x87;
  if (differs) {
    // Not the bell either. Drop it.
    goto out;
  }
}

  // A tenth of a second of silence, then 192 clicks of the speaker.
  rom_wait(0x40);
  uint8_t clicks = 0xc0;

  for (;;) { /* $FBE4 */
    rom_wait(0x0c);
    io_peek(0xc030); // the click; the read is the write
    clicks = clicks - 0x01;
    if (!clicks)
      break;
  }

  goto out;

carriage_return: /* to the left edge, then down */
  s_mon.ch = 0x00;

line_feed: /* $FC66 */
  s_mon.cv = s_mon.cv + 0x01;
  rom_fc68(); // JMP -- a tail call, and where a scroll happens.

out:
  return;
}

/* ========================================================================== */
/* $FDED COUT                                                                 */
/* ========================================================================== */

/// $FDED COUT is a single instruction: `JMP ($36)`. It dispatches through the
/// output vector CSWL/CSWH, so the *behaviour* of COUT is whatever the vector
/// points at, and it must not be short-circuited to ROM COUT1.
///
/// Snake Byte can hook the vector: $6641 does
///     LDA #$4A / STA $36 / LDA #$66 / STA $37 / RTS
/// pointing it at $664A, the game's own hi-res text renderer. ($6641 is called
/// from $7485 and $793F.) $664A maps the text cursor (BASL/BASH + CH) to a
/// hi-res address by adding $1C to the high byte ($04xx -> $20xx) and draws an
/// 8-byte glyph from the font at $66A9; control characters below $20 fall
/// through to ROM COUT1 at $FDF0.
///
/// Only those two targets are implemented, and any other is a hard failure
/// rather than a quiet fallback to COUT1. Nothing in this program can tell the
/// difference between the right answer and a wrong one here -- both render --
/// so a wrong guess would diverge invisibly, while an abort fires at the
/// moment the question first matters.
void rom_cout(uint8_t ch) {
  uint16_t vector;

  vector = s_mon.csw; // JMP ($36)
  switch (vector) {
  case 0xfdf0:
    rom_cout1(ch);
    break;
  case 0x664a:
    game_cout_hook(ch);
    break;
  default:
    fprintf(
        stderr,
        "rom_cout: output vector CSWL/CSWH ($36/$37) points at $%04X, "
        "which is not implemented.\n"
        "  Known targets are $FDF0 (ROM COUT1) and $664A (the game's "
        "hi-res text renderer).\n",
        vector);
    error_handler(0xfded);
    abort();
  }

  return;
}

/// $FDF0 COUT1. Put a character on the text screen, then restore Y.
///
/// Printable characters ($A0 and up) are masked with INVFLG, which is how the
/// monitor does inverse and flashing -- $FF leaves them alone. Control codes
/// are let through unmasked, since mangling them would change what they mean.
void rom_cout1(uint8_t ch) {
  const bool printable = ch >= 0xa0;

  if (printable) {
    ch = ch & s_mon.invflg;
  }

  // The original saves Y in YSAV1 across the call and puts it back, because
  // COUT promises its callers it preserves Y. Nothing in this file ever reads
  // Y for a value, so the promise has nobody to keep it to: saving and
  // restoring were the only two things that touched it.
  rom_coutz(ch); // JSR $FB78
}

/* ========================================================================== */
/* $FE89 SETKBD / $FE93 SETVID                                                */
/*                                                                            */
/* Both fall into the shared tail at $FE9B (SETPWRC/"install IO hooks"), which */
/* is duplicated into each function exactly as the decompiler emitted it.      */
/* ========================================================================== */

/// $FE89 SETKBD. Point the character *input* vector at the built-in keyboard.
///
/// The vector it writes, KSWL/KSWH, is not read by anything in this build --
/// nothing dispatches input through it -- but the routine runs at startup and
/// its cycles count, so it is here in full.
void rom_setkbd(void) {
  s_mon.a2l = 0x00;
  // X and Y are the built-in device's vector, $FD1B (KEYIN). SETIO below keeps
  // the low half and replaces the page.
  const uint8_t entry_low = 0x1b;
  // $FE91 BNE -- provably always taken (Y was just loaded #$1B, nonzero),
  // same reasoning as $FC60 in rom_home: the decompiler doesn't do
  // cross-instruction flag proofs, so the branch still executes and still
  // pays its own cost every time.

  // $FE9B SETIO. A2L is the slot; slot 0 means the built-in device and the ROM
  // answers page $FD, where its own KEYIN and COUT1 live. A real slot would
  // give $Cn00 instead -- decoded, never taken, because the game never sets
  // one.
  const uint8_t slot = s_mon.a2l & 0x0f;
  uint8_t page, low;
  if (slot) {
    page = slot | 0xc0;
    low = 0x00;
    // $FEA5 BEQ -- provably always taken (Y was just loaded 0).
  } else {
    page = 0xfd;
    low = entry_low;
  }

  s_mon.ksw = low | (page << 8);
}

/// $FE93 SETVID. Point COUT's vector back at the ROM's own COUT1.
///
/// This is how the game gets out of its hi-res text hook -- or would be. It
/// writes CSWL/CSWH, which up to 2026-08-24 meant zero page $36/$37, and now
/// means the two variables those became. Between the move and this commit the
/// store went to RAM and was read by nobody, which nothing caught: the entry
/// snapshot already holds $FDF0 there, and the game only calls SETVID once, at
/// startup, before it has repointed anything. Had it ever called it after
/// installing the hook, COUT would have stayed hooked.
void rom_setvid(void) {
  s_mon.a2l = 0x00;
  // The same, for $FDF0 (COUT1).
  const uint8_t entry_low = 0xf0;

  // $FE9B SETIO. A2L is the slot; slot 0 means the built-in device and the ROM
  // answers page $FD, where its own KEYIN and COUT1 live. A real slot would
  // give $Cn00 instead -- decoded, never taken, because the game never sets
  // one.
  const uint8_t slot = s_mon.a2l & 0x0f;
  uint8_t page, low;
  if (slot) {
    page = slot | 0xc0;
    low = 0x00;
    // $FEA5 BEQ -- provably always taken (Y was just loaded 0).
  } else {
    page = 0xfd;
    low = entry_low;
  }

  s_mon.csw = low | (page << 8);
}

/// $FBC1 BASCALC. Turn a text line number in A into that line's base address
/// in BASL/BASH.
///
/// The Apple II text screen is three interleaved bands of eight lines. The ROM
/// does not use a table: bit 0 of the line picks the band's second half, bits
/// 1-2 pick the band, and bits 3-4 pick the line within it, folded in by the
/// `ADC #$7F` and the two shifts below.
///
/// A and the flags are left as the original leaves them, because COUT1 and
/// CLREOLZ both read BASL straight afterwards and the monitor's callers are
/// not all in this file's control.
/// Compute BASH and BASL for line \p line, returning BASL's low byte and, in
/// \p carry_out, the bit shifted out of the second ASL -- which VTABZ adds
/// straight back in.
uint8_t rom_bascalc(uint8_t line, bool *carry_out) {
  // LSR: the carry is the line's low bit, and it is what decides the ADC below.
  const uint8_t odd = line & 0x01;
  const uint8_t page = ((line >> 1) & 0x03) | 0x04;
  uint8_t band = line & 0x18;

  if (odd) {
    // ADC #$7F with the carry the LSR just set, i.e. +$80: the second half of
    // the band.
    band = (band + 0x007f) + odd;
  }

  // ASL twice, then OR the original back in -- the original does this in BASL
  // itself, which is why the band is stored there first. The second shift's
  // carry out is what VTABZ adds straight back in, which is why it is returned
  // rather than left in a flag.
  const uint16_t shifted = band << 0x02;
  *carry_out = ((shifted & 0x01ff) >> 8) != 0;
  const uint8_t addr_lo = shifted | band;
  s_mon.bas = addr_lo | (page << 8);
  return addr_lo;
}

/// $FC24 VTABZ. BASCALC for the line in A, then shift the base right by the
/// window's left edge, so BASL points at the first column of the window rather
/// than of the screen.
/// Point BASL/BASH at line \p line of the text screen.
///
/// Returns the carry out of `ADC WNDLFT`, which the scroll at $FC81 reads as
/// the +1 that steps to the next line -- the same trick HOME plays with
/// CLREOLZ's. The carry going *in* is BASCALC's, from its second ASL.
bool rom_vtabz(uint8_t line) {
  bool carry;
  const uint8_t base = rom_bascalc(line, &carry);

  const uint16_t r = (base + s_mon.wndlft) + carry;
  // VTABZ adds WNDLFT to BASL and leaves BASH exactly as BASCALC set it, so
  // this really is a write of one half -- and the cast is what makes it one:
  // r carries the ADC's ninth bit, which must not reach BASH.
  s_mon.bas = (s_mon.bas & 0xff00) | (uint8_t)r;

  return ((r >> 8) & 0x01) != 0;
}

/// $FC9C CLREOL. Blank from the cursor to the right edge of the window.
///
/// **Transcribed from the binary and never executed by any test.** Its only
/// caller is on the scroll path, and nothing the game does scrolls. The body
/// is three instructions and its tail call *is* exercised, so what is
/// unverified is the starting column and the jump.
void rom_clreol(void) {
  rom_clreolz(s_mon.ch); // JMP -- a tail call; nobody reads its carry.
}

/// $FC9E CLREOLZ. The same, from column Y rather than from the cursor. Writes
/// spaces up to but not including the window's width.
///
/// Y is left at the width and the carry set, which is how the original exits
/// the loop; both are still written because the monitor's callers read them.
/// Blank from \p col to the window's right edge.
///
/// Returns the carry its final `CPY WNDWDTH` leaves, which is not decoration:
/// HOME's CLRSC2 loop reaches its `ADC #$00` with no CLC of its own and uses
/// this as the +1 that steps to the next line, and $FC9A branches on it.
bool rom_clreolz(uint8_t col) {
  const uint8_t space = 0xa0; // a space, high bit set

  for (;;) {
    poke(s_mon.bas + col, space);

    const uint8_t next = col + 1;
    col = next;

    const uint8_t width = s_mon.wndwdth;
    if (next >= width) {
      return true;
    }
  }
}

/// $FCA8 WAIT. The monitor's delay: two nested `SBC #$01 / BNE` loops around
/// the value in A, which BELL uses to time its tone.
///
/// **Transcribed from the binary and never executed by any test.** Its only
/// callers are inside BELL1, and nothing the game does emits a Ctrl-G. An
/// error in the transcription would not be caught.
///
/// A comes back as 0 and the carry set. The inner loop counts A down to zero
/// from a copy on the stack, and the outer one counts the original down, so
/// the total is quadratic in A rather than linear.
void rom_wait(uint8_t n) {
  /* The SBC's Z, which both loops branch on. A local for the same reason the
     scroll's N is one: nothing outside this routine ever read the flag, and
     each read here follows the SBC that sets it.

     Never executed by any test -- WAIT's only caller is BELL1 and nothing
     emits a Ctrl-G -- so inverting either test below goes unnoticed. This
     rests on the dominance argument above and not on a passing run. */
  uint8_t not_zero = 0;

  // SEC: the borrow chain both loops run on.
  uint8_t carry = 0x01;

  for (;;) {

    // The inner loop: A down to zero, one SBC per pass.
    uint8_t inner = n;
    for (;;) {
      // WAIT is called only by BELL1, where it is the silence between the
      // bell's clicks, so this charge is the tone. Nothing exercises it, so
      // the number rests on the 6502's timings rather than on a measurement.
      advance(5);
      const uint16_t r = (inner - 0x0001) - (0x01 - carry);
      carry = 0x01 - ((r >> 8) & 0x01);
      inner = r;
      not_zero = inner;
      if (!not_zero)
        break;
    }

    // The outer one: the copy off the stack, down by one.
    const uint16_t r = (n - 0x0001) - (0x01 - carry);
    carry = 0x01 - ((r >> 8) & 0x01);
    n = r;
    not_zero = n;
    if (!not_zero)
      break;
  }
}
