/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte, decompiled: the whole program, in one translation unit,
/// sharing nothing.
///
/// This is the artifact the exercise is for. It enters at the game's own entry
/// point, $3750, with the machine state the Apple II boot would have produced;
/// it contains no Applesoft ROM code and no decompiler-generated code.
///
/// Why one file
/// ------------
/// Two reasons, and the second is the important one.
///
/// The mechanical reason is that `apple2tc/system2-inc.h` does not merely
/// declare the emulated machine, it *defines* it, and nearly all of it has
/// internal linkage -- the registers, the flags, `s_ram`, the cycle counters,
/// the access helpers. Anything implementing a 6502 routine has to be in the
/// same translation unit as that.
///
/// The real reason is ownership. The other four targets in this directory are
/// scaffolding: `snake-bytec1-ext` boots the machine and is the control this
/// one is checked against, `snake-bytec1` is the control for *that*,
/// `snake-byte-easyc1-ext` is a fixture, and `snake-byte` is history. While
/// this file shared game.c and game_native.c with them, every change here had
/// to be safe for builds that still run a generated dispatch over the same
/// addresses -- which is a real constraint and cost a file split already. It
/// does not share anything now. Change it freely; the gate says whether the
/// game still behaves.
///
/// What is still in here that is not the game
/// ------------------------------------------
///   - `s_mem_3750`, the game's own binary image. Its data -- level scripts,
///     the font, the tables -- and it stays.
///   - `s_mem_d000`, the Apple II ROM image. Not for its code: the death pause
///     reads delay lengths out of ROM *as data* at $E000.
///   - the Apple II ROM's own entry points, as C. They are the machine's, not
///     the game's, but they are decompiled on the same terms as everything
///     else here: there is no category of code in this file that belongs to
///     someone else. The last five still shaped like the decompiler left them
///     -- BASCALC, VTABZ, CLREOL, CLREOLZ and WAIT -- were converted on
///     2026-08-24; what is still `bb_N:` and `goto` is the remaining entry
///     points, which is the same job unfinished.
///   - the entry state, $0000-$0802 plus registers and one soft switch,
///     captured with --snapshot-at. See make-entry-state.sh.
///
/// The first two are 2,020 lines of hex between them, so they live in
/// game-image.inc and rom-image.inc and are `#include`d below. That is a
/// concession to reading the file, not a retreat from owning it: nothing else
/// includes them, they define statics and so can be included only once, and
/// the split was verified by compiling before and after -- every non-debug
/// section identical, and the emitted code differing in exactly the two
/// `__LINE__` immediates the shift moved.
///
/// How it is checked
/// -----------------
/// probe-acceptance.sh runs this against snake-bytec1-ext and requires the two
/// to agree from $3750 onward: every block head that is still probed, and the
/// screen at every in-game sample. The screen check is the one that survives
/// as more of this file turns into ordinary C.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"


/* ========================================================================== *
 * The machine, the images, and what is left of the ROM                     *
 * ========================================================================== */

// Loaded binary at [$3750..$854E]
// --code-at: 3 asserted edges applied
// Loaded segment [$00B1..$00C8]
// 293 new runtime blocks added
// code labels: 1673
// data labels: 284

#include "apple2tc/system2-inc.h"

/// Defined next to click_speaker; declared here because the sound routines
/// above it also toggle the speaker.
static void speaker_access(uint8_t port);

/// Charge the cycles the original spent, without naming the address.
///
/// Step 6b. The clock has to keep running -- `cycles_expired()` is what
/// advances the host's frames, so with no charges at all `--frames` never
/// terminates -- but nothing needs the *address* once the block-head trace is
/// retired. This is the same arithmetic CYCLES does, minus the `s_pc` store
/// and the probe dispatch that a charge-only site never used.
///
/// Timing is bit-identical, which is what makes the change checkable: the
/// screen and memory comparisons run against the booting build unchanged.
#define TICK(n)                            \
  do {                                     \
    if (s_remaining_cycles <= 0)           \
      cycles_expired();                    \
    s_cycles += (n);                       \
    s_remaining_cycles -= (n);             \
  } while (0)


/// The zero-page fragment $00B1-$00C8 the run data carried: the six-byte
/// CHRGET routine Applesoft assembles there at boot. Small enough to read,
/// so unlike the other two images it stays in the file.
static const uint8_t s_mem_00b1[0x0018] = {
  0xE6, 0xB8, 0xD0, 0x02, 0xE6, 0xB9, 0xAD, 0x06, 0x02, 0xC9, 0x3A, 0xB0, 0x0A, 0xC9, 0x20, 0xF0,
  0xEF, 0x38, 0xE9, 0x30, 0x38, 0xE9, 0xD0, 0x60
};

#include "game-image.inc"
#include "rom-image.inc"

/// Put the three images into RAM: the zero-page fragment the run data carried,
/// the game itself, and the Apple II ROM. Named for what it does -- it used to
/// be the whole of init_emulated(), which now has state to install as well.
static void load_images(void) {
  memcpy(s_ram + 0x00b1, s_mem_00b1, 0x0018);
  memcpy(s_ram + 0x3750, s_mem_3750, 0x4dff);
  memcpy(s_ram + 0xd000, s_mem_d000, 0x3000);
}

static inline uint8_t ovf8(uint8_t res, uint8_t a, uint8_t b) {
  return (~(a ^ b) & (a ^ res)) >> 7;
}
static uint16_t adc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = adc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}
static uint16_t sbc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = sbc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}

/* --- The monitor's zero page ---------------------------------------------- */
/*
 * $0020-$003E, with the Apple II's own names. These are not the game's
 * variables: they belong to the ROM routines it calls, and the game writes
 * them only to pass arguments -- a cursor position before COUT, a colour
 * before PLOT, a right-hand end before HLINE. Keeping the monitor's spelling
 * is deliberate; every Apple II reference uses it, and a2rom.h's prose
 * already does.
 */
/* The monitor's own state, out of emulated RAM on the same terms as the game's.
   Names are the Apple II's, because every reference uses them and a2rom.h's
   prose already did. */
static uint8_t s_wndlft;  ///< text window: left, width, top, bottom
static uint8_t s_wndwdth;
static uint8_t s_wndtop;
static uint8_t s_wndbtm;
/* Addresses, not byte pairs. The 6502 splits a pointer into two zero-page
   bytes because that is all it can address; nothing here has two halves that
   mean different things. Where a routine really does write one half -- VTABZ
   rewrites only the low byte, and the glyph blitter does 8-bit arithmetic on
   each -- the code says so with a mask or a shift, which is what the split
   used to be hiding. */
static uint16_t s_gbas;  ///< lo-res line base, from GBASCALC and V2
static uint16_t s_bas;   ///< text line base, from BASCALC and CV
static uint16_t s_bas2;  ///< the scroll's destination line
static uint8_t s_v2;      ///< VLINE's bottom row
static uint8_t s_mask;    ///< which nibble of a lo-res byte a PLOT touches
static uint8_t s_color;   ///< the lo-res colour, both nibbles
static uint8_t s_invflg;  ///< COUT1 ANDs the character with this: $FF normal
static uint16_t s_csw; ///< the character output vector the game repoints
static uint16_t s_ksw; ///< the character input vector; nothing reads it
static uint8_t s_a2l;     ///< SETKBD/SETVID scratch

/// $002C, and still one byte doing two jobs.
///
/// To the ROM it is H2, the right-hand end of a lo-res HLINE. To
/// game_print_bcd it is the flag saying a digit has been printed, so
/// game_print_zero_if_blank knows whether the field came out empty. The two
/// never overlap -- nothing draws while a number is being printed -- and
/// draw_border actually *relies* on the H2 half persisting from
/// wipe_occupancy_map ("$2C is still $27 from the wipe"), so the storage stays
/// shared. Splitting it would need that argument made in both directions; the
/// accessors below carry the meaning instead.
static uint8_t s_h2;

/// The three pointer pairs, low byte first, as ram_peek16al read them.

/// $0024/$0025 -- CH and CV, the text cursor, out of emulated RAM.
///
/// The last two addresses in this block that anything still *computes* with.
/// The ROM's BASCALC turns CV into BASL/BASH, CLREOL and COUT1 step CH along a
/// line, and the game sets both before every field it prints. All of that is
/// C now; the bytes at $24/$25 are no longer read by anybody.
///
/// Moved even though the ROM owns them, because the ROM is being decompiled
/// too -- a routine that is still emitted as a switch over block ids is not a
/// different kind of code, only a less finished one. There is no version of
/// this artifact in which the monitor keeps its state in a 64K array.
///
/// Initialised from the entry snapshot, like the plotter's block.
static uint8_t s_ch, s_cv;

/// The converted game routines are entered with decimal mode clear. Rather
/// than carry the decompiler's dead decimal arms through them, say so and fail


/// A cell on the 40x48 playfield grid.
typedef struct {
  uint8_t col;
  uint8_t row;
} Cell;

/// One of the two objects that ricochet around the playfield. The original
/// keeps them as four parallel pairs of bytes and copies one set into
/// $6633-$6636 before stepping it -- a hand-rolled calling convention, which
/// is why a struct fits so exactly, and which step_bouncer_slot now spells as
/// passing one.
typedef struct {
  uint8_t col;
  uint8_t row;
  /// Always +1 or -1. The original stores them as bytes and reflects with
  /// `EOR #$FE`, which swaps $01 and $FF.
  int8_t dx;
  int8_t dy;
} Bouncer;

void game_cold_start(void);
void rom_plot(uint8_t row, uint8_t col);
void rom_hline(uint8_t row, uint8_t from_col);
void rom_setcol(uint8_t ink);
uint8_t rom_scrn(uint8_t row, uint8_t col);
void rom_home(void);
void rom_fc68(void);
void rom_cout(uint8_t ch);
void rom_setkbd(void);
void rom_setvid(void);
void game_move_bouncer(Bouncer *b);
void game_print_inline_str(uint16_t ret_addr);
uint8_t rom_bascalc(uint8_t line, bool *carry_out);
bool rom_vtabz(uint8_t line);
void rom_clreol(void);
bool rom_clreolz(uint8_t col);
void rom_wait(uint8_t n);

/// The program starts here. There used to be a func_t001 in between -- the
/// generated dispatch -- and by the end it was a stub that called this.
static void emulated_entry_point(void) {
  game_cold_start();
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
  /*$FBC1*/ TICK(20);
  // LSR: the carry is the line's low bit, and it is what decides the ADC below.
  const uint8_t odd = line & 0x01;
  const uint8_t page = (uint8_t)(((line >> 1) & 0x03) | 0x04);
  uint8_t band = line & 0x18;

  if (!odd) {
    /*$FBCC*/ TICK(1);
  } else {
    /*$FBCE*/ TICK(2);
    // ADC #$7F with the carry the LSR just set, i.e. +$80: the second half of
    // the band.
    band = (uint8_t)((uint16_t)(band + 0x007f) + odd);
  }

  /*$FBD0*/ TICK(19);
  // ASL twice, then OR the original back in -- the original does this in BASL
  // itself, which is why the band is stored there first. The second shift's
  // carry out is what VTABZ adds straight back in, which is why it is returned
  // rather than left in a flag.
  const uint16_t shifted = (uint16_t)(band << 0x02);
  *carry_out = ((shifted & 0x01ff) >> 8) != 0;
  const uint8_t addr_lo = (uint8_t)shifted | band;
  s_bas = (uint16_t)(addr_lo | (page << 8));
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
  /*$FC24*/ TICK(6);
  bool carry;
  const uint8_t base = rom_bascalc(line, &carry);

  /*$FC27*/ TICK(6);
  const uint16_t r = ((uint16_t)base + s_wndlft) + carry;
  // VTABZ adds WNDLFT to BASL and leaves BASH exactly as BASCALC set it, so
  // this really is a write of one half.
  s_bas = (uint16_t)((s_bas & 0xff00) | (uint8_t)r);

  /*$FC2B*/ TICK(6);
  return ((r >> 8) & 0x01) != 0;
}

/// $FC9C CLREOL. Blank from the cursor to the right edge of the window.
///
/// **Decoded from the binary and not verified.** Neither cold scenario reaches
/// it: probed at $FC9C, it fires 0 times in play and 0 in hires, because its
/// only caller is $FC9A on the scroll path and nothing in the recordings
/// scrolls. The body is three instructions and its tail call is the routine
/// below, which *is* exercised, so what is unchecked is the LDY and the jump.
void rom_clreol(void) {
  /*$FC9C*/ TICK(3);
  rom_clreolz(s_ch); // JMP -- a tail call; nobody reads its carry.
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
  /*$FC9E*/ TICK(2);
  const uint8_t space = 0xa0; // a space, high bit set

  for (;;) {
    /*$FCA0*/ TICK(13);
    poke((uint16_t)(s_bas + col), space);

    const uint8_t next = (uint8_t)(col + 1);
    col = next;

    const uint8_t width = s_wndwdth;
    if (next >= width) {
      /*$FCA7*/ TICK(6);
      return true;
    }

    /*$FCA5*/ TICK(1);
  }
}

/// $FCA8 WAIT. The monitor's delay: two nested `SBC #$01 / BNE` loops around
/// the value in A, which BELL uses to time its tone.
///
/// **Decoded from the binary and not verified.** Probed at $FCA8, it fires 0
/// times in both cold scenarios. Its only callers are $FBDF and $FBE6 inside
/// BELL1, and nothing in the recordings outputs a Ctrl-G. That was already
/// true of the version the decompiler emitted, so no coverage was lost here --
/// but an error in the transcription would not be caught either.
///
/// A comes back as 0 and the carry set. The inner loop counts A down to zero
/// from a copy on the stack, and the outer one counts the original down, so
/// the total is quadratic in A rather than linear.
void rom_wait(uint8_t n) {

  /* The SBC's Z, which both loops branch on. A local for the same reason the
     scroll's N is one: nothing outside this routine ever read the flag, and
     each read here follows the SBC that sets it.

     Also not checked by running it -- rom_wait's only caller is BELL1 and
     nothing emits Ctrl-G, so it fires zero times in both scenarios. Inverting
     either test below passes all six cold checks. */
  uint8_t not_zero = 0;

  /*$FCA8*/ TICK(2);
  // SEC: the borrow chain both loops run on.
  uint8_t carry = 0x01;

  for (;;) {
    /*$FCA9*/ TICK(3);

    // The inner loop: A down to zero, one SBC per pass.
    uint8_t inner = n;
    for (;;) {
      /*$FCAA*/ TICK(4);
      const uint16_t r = (uint16_t)(inner - 0x0001) - (uint8_t)(0x01 - carry);
      carry = (uint8_t)(0x01 - ((uint8_t)(r >> 8) & 0x01));
      inner = (uint8_t)r;
      not_zero = inner;
      if (!not_zero)
        break;
      /*$FCAC*/ TICK(1);
    }

    // The outer one: the copy off the stack, down by one.
    /*$FCAE*/ TICK(8);
    const uint16_t r = (uint16_t)(n - 0x0001) - (uint8_t)(0x01 - carry);
    carry = (uint8_t)(0x01 - ((uint8_t)(r >> 8) & 0x01));
    n = (uint8_t)r;
    not_zero = n;
    if (!not_zero)
      break;
    /*$FCB1*/ TICK(1);
  }

  /*$FCB3*/ TICK(6);
}

/* ========================================================================== *
 * Apple II ROM entry points -- declarations                                *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Hand-written replacements for the Apple II ROM entry points that Snake Byte
/// calls. The addresses listed here are also listed in `rom.externs`, which is
/// passed to the decompiler via `--extern-routines=`. That makes the decompiler
/// emit a declaration but no body for each of them, so the definitions in
/// `a2rom.c` take over.
///
/// They followed the generated code's calling convention until 2026-08-24,
/// taking a `ret_addr` they pushed on entry to mimic a JSR. That is gone --
/// nothing in this build reads the emulated call stack, so `push16` and
/// `pop16` are unused here and the routines are ordinary C functions.
///
/// The CPU state that used to be passed in those globals is gone: the ROM
/// entry points take parameters and return values, `rom_plot` wants a row and
/// a column rather than A and Y, and nothing in this file names A, X or Y.
///
/// `push8`/`pop8` are still here and are not the same thing: PHA/PHP inside a
/// routine, which the routine pops itself before it returns.
///
/// IMPORTANT: `a2rom.c` is *not* a standalone translation unit. See the comment
/// at the top of that file.



/// $F800 PLOT. Plot a lo-res block at column Y, row A. Trashes A, preserves Y.
void rom_plot(uint8_t row, uint8_t col);

/// $F819 HLINE. Draw a horizontal lo-res line at row A from column Y to the
/// column in $2C.
void rom_hline(uint8_t row, uint8_t from_col);

/// $F864 SETCOL. Set the lo-res color to A (low nibble), replicated into both
/// nibbles of $30.
void rom_setcol(uint8_t ink);

/// $F871 SCRN. Read the lo-res block at column Y, row A; returns the color in A.
uint8_t rom_scrn(uint8_t row, uint8_t col);

/// $FC58 HOME. Clear the text window and move the cursor to its top left.
void rom_home(void);

/// $FC68. The tail of the ROM's LF handling, not a documented entry point under
/// a familiar name:
///
///     FC68: LDA CV / CMP WNDBTM / BCC VTABZ
///     FC6E: DEC CV / ... scroll the window up one line, clear the bottom
///
/// So it recomputes BASL/BASH from CV, and only scrolls when CV has run past the
/// bottom of the window. COUT1 reaches it by falling through $FC66, but the game
/// also calls it directly as a VTAB -- $7590 and $75D1 store CH/CV and JSR here,
/// with CV always well under WNDBTM ($18), so the scroll half never runs.
void rom_fc68(void);

/// $FDED COUT. Output the character in A through the output vector CSWL/CSWH
/// at $36/$37. See the extensive comment in `a2rom.c`: only the ROM COUT1
/// ($FDF0) target is implemented; any other target aborts loudly.
void rom_cout(uint8_t ch);

/// $FE89 SETKBD. Reset the input vector KSWL/KSWH ($38/$39) to the keyboard.
void rom_setkbd(void);

/// $FE93 SETVID. Reset the output vector CSWL/CSWH ($36/$37) to the screen,
/// i.e. to COUT1 at $FDF0.
void rom_setvid(void);

/* ========================================================================== *
 * The game as ordinary C -- declarations                                   *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */



/* ========================================================================== *
 * The game, as ordinary C                                                    *
 * ========================================================================== *
 *
 * This section arrived as game_native.c, which was one half of a split: game.c
 * held each routine in the shape the 6502 left it -- arguments in fixed
 * zero-page addresses, results in A and the flags, an emulated stack, CYCLES
 * at every block head -- because generated code called it and read that state,
 * while game_native.c held the same routines as C behind an adapter that
 * marshalled between the two.
 *
 * None of that is true here. There is no generated code left to keep an ABI
 * for, the 42 adapters are gone, the arguments are arguments, and the storage
 * is C variables rather than addresses reached through accessors. The two
 * files are one, and the frontier the split described has been crossed.
 *
 * What survives from that arrangement is the cycle accounting below, and the
 * reason it is spelled three different ways.
 */


/* --- Cycle accounting ----------------------------------------------------- */

/// Charge the cycles the original spent in a block, without registering a
/// probe site.
///
/// Timing cannot be dropped along with the block structure. A frame is a cycle
/// budget, so a converted routine that runs at a different speed moves every
/// later frame boundary and therefore every later frame hash -- the one oracle
/// meant to survive to the end. What is given up is only the *observability*
/// of the address: `CYCLES_EDGE` does not dispatch a probe, so the site leaves
/// the block-head trace. That is the middle rung of the ladder in this file's
/// header, and it is deliberate.
///
/// Spelled differently from `CYCLES` so the two cannot be confused, and so
/// probe-acceptance.sh can reject a plain `CYCLES` in this file -- one written
/// here would charge cycles and never be probed, which is a silent hole rather
/// than a declared trade.
#define GAME_CYCLES(addr, n) CYCLES_EDGE((addr), (n))

/// Charge the cycles and *keep* the probe, for the handful of addresses that
/// carry the input coordinate.
///
/// Replay does not stamp keystrokes on cycles -- it stamps them on a counter
/// the probe language increments at seven named addresses (see rec.probe).
/// Two of those, $6217 and $760F, sit inside routines this file is converting.
/// A converted site charges its cycles and drops its probe, so spelling one of
/// those GAME_CYCLES would stop the counter advancing there, and every key
/// stamped after that point would arrive at a different instant.
///
/// That is caught, but badly. The interpreter side of the comparison always
/// runs the original binary, so its counter never drifts; only the generated
/// side does, and the block-head trace then diverges. Measured: converting
/// $760F fails trace-ext on the play-hires scenario at line 640,983 of the
/// diff, with four addresses in the redefinition screen appearing on one side
/// and not the other. Nothing about that output says "the input coordinate
/// moved". verify.sh does not notice at all -- it replays the cycle-stamped
/// .keys files, which do not use the counter.
///
/// So the site keeps its probe, and the spelling says why at the call site.
/// probe-acceptance.sh asserts that every address written this way is one of
/// the coordinate's, which turns a 640,983-line diff into one line naming the
/// address.
///
/// The local inside the macro is the emulator's assembly-trace plumbing:
/// `CYCLES` consults a `branchTarget` flag the generated dispatch keeps, to
/// print one line per block rather than one per instruction. Converted code
/// has no block structure for that question to be about, and a site spelled
/// this way is a block head by construction, so it answers yes and moves on.
///
/// That local is the only `branchTarget` left in this file. The generated code
/// declared one per function and assigned it at every block head; nothing here
/// ever read one, so all 121 went.
#define GAME_CYCLES_COORD(addr, n) \
  do {                             \
    bool branchTarget = true;      \
    CYCLES((addr), (n));           \
    (void)branchTarget;            \
  } while (0)

/// Charge the cycles and keep the probe, because some *other* source still
/// names this address.
///
/// A converted routine normally takes its block heads out of the comparison
/// altogether: nothing probes them, on either engine, and the two agree by
/// saying nothing. That argument fails the moment another file still emits a
/// `CYCLES` for the same address, because then the interpreter reports it and
/// the generated build does not.
///
/// It has happened twice, for two different reasons. $6216 is an RTS shared by
/// two routines, one converted and one not. $720E is stranger: the low half of
/// $71F3 survives in the generated C as an orphan -- its only predecessors were
/// in the extern-replaced region, so nothing can reach it -- but it is still
/// text in the file, so it is still on the site list. Neither is visible in the
/// pinned count, which was right both times.
///
/// probe-acceptance.sh checks all three spellings against the site list built
/// from the other sources: an address here must be in it, an address on plain
/// GAME_CYCLES must not be, and GAME_CYCLES_COORD must be on the coordinate.
#define GAME_CYCLES_SHARED(addr, n) GAME_CYCLES_COORD((addr), (n))

/// Charge the cycles and keep the probe, because the address is where a
/// cross-build comparison lines the two traces up.
///
/// Exactly one address uses this, and it is $3750. probe-acceptance.sh checks
/// the cold-start build against the booting one, and the booting one's trace
/// has to have its boot prefix removed before the two are comparable. The only
/// way to find where that prefix ends is to look for the entry address, so the
/// entry address has to still be in the trace. Converting it to a plain
/// GAME_CYCLES takes it out and the alignment silently finds nothing.
///
/// Spelled apart from GAME_CYCLES_COORD, which it expands to, because the
/// reason is different and the two lints check different things.
#define GAME_CYCLES_ANCHOR(addr, n) GAME_CYCLES_COORD((addr), (n))

/* --- Converted routines --------------------------------------------------- */

/// $660F -- start a life: put the snake's head in \p head_col and set both
/// bouncers going from opposite corners. Returns the value the original left
/// in A, which its one caller stores as the tail column.
uint8_t game_start_life(uint8_t head_col);

/// $6127 -- copy shape \p shape's four scanline masks into $6060, and return
/// the last one.
void game_load_shape_masks(uint8_t shape);

/// $6641 -- point the ROM's character-output vector at the game's own hi-res
/// handler, so every later COUT reaches game_cout_hook.
void game_install_cout_vector(void);

/// $64C8 -- step \p b one cell along its deltas and redraw it, reflecting off
/// whatever it hits.
void bouncer_step(Bouncer *b);

/// What $6AB8 decided about a candidate move. The original says all of this
/// in A and the Z flag; the adapter puts it back.
typedef enum {
  /// The target cell holds something other than empty or an apple.
  MOVE_TARGET_TAKEN,
  /// The target row is 0, the top border. Accepted, oddly -- but the border
  /// is occupied, so the check above normally rejects it first.
  MOVE_ROW_ZERO,
  /// Safe: at least one of the target's four neighbours is free.
  MOVE_OK,
  /// All four neighbours are occupied -- legal to enter, fatal next move.
  MOVE_DEAD_END,
} MoveVerdict;

/// $6AB8 -- judge a step in direction \p dir. \p cell_out receives what the
/// occupancy map held at the target, which the original leaves in A.
MoveVerdict snake_move_verdict(uint8_t dir, uint8_t *cell_out);

/// $728D -- copy the score at $7252 over the high score at $7256 if it beats
/// it, comparing BCD bytes most significant first.
void game_promote_high_score(void);

/// $6148 / $615A -- runs of hi-res cells along a row or down a column.
void game_plot_hline_native(uint8_t ink, Cell c, uint8_t to_col);
void game_plot_vline_native(uint8_t ink, Cell c, uint8_t to_row);

/// $7000 -- the lo-res half of a vertical run, restoring $03.
void game_lores_vline_native(Cell c, uint8_t to_row);

/// $6594 -- step the bouncers the difficulty calls for, then return the next
/// queued key.
uint8_t game_step_bouncers_native(void);

/// $60E7 -- draw the loaded shape into cell \p c in ink \p ink, replacing.
void game_draw_cell_native(uint8_t ink, Cell c);

/// $6B93 -- the same, merged into what is already there.
void game_merge_cell_native(uint8_t ink, Cell c);

/// $702B -- zero hi-res page 1.
void game_clear_hgr_native(void);

/// $7045 -- clear the screen, draw the border, and run the current level's
/// display list from $8000.
void game_draw_playfield_native(void);

/// $69C3 -- sweep columns outward from the snake for an apple, leaving the
/// answer at $6B3B/$6B3C.
void game_find_nearest_apple(void);

/// $6C75 -- turn \p key, the byte just taken off the ring, into the code the
/// game acts on: a direction, a joystick setting applied on the spot, or $00
/// for nothing. Reads the joystick itself when one is selected and the key
/// was not a direction.
uint8_t game_read_direction_native(uint8_t key);

/// $75D1 -- blink slot \p slot on the key-redefinition screen until the player
/// presses something it will accept, and return that key.
uint8_t game_edit_key_native(uint8_t slot);

/// $3750 -- the program's entry, and the outermost loop: relocate the level
/// data, initialise $0300-$0304, then new game -> level -> round -> life
/// forever. Never returns; the game has no way out.
void game_cold_start(void);

/// $69A9 -- ESC pauses until any key is pressed; Ctrl-S toggles the sound.
/// Every key the dispatch chain did not recognise arrives here and is ignored.
///
/// Returns the key the Ctrl-S test actually saw, which is \p key unless ESC
/// paused: the keypress that ends the pause replaces it and is tested in turn.
void game_pause_or_toggle_sound_native(uint8_t key);

/// $6BFB -- twenty passes of the falling tone that plays while the head moves.
void game_tick_sound_native(void);

/// $71F3 -- print \p byte as two decimal digits, dropping leading zeros.
void game_print_bcd_native(uint8_t byte);

/// $7226 -- print a single "0" if the number just printed was all zeros.
void game_print_zero_if_blank_native(void);

/// $7267 -- add the two-byte BCD value at $71CB to the four-byte score.
void game_add_score_native(void);

/// $7024 -- set the lo-res plot colour from an ink byte: 0 erases, anything
/// else draws.
void game_set_ink_native(uint8_t ink);

/// $7019 -- read the byte at the $000A pointer into A and advance it.
uint8_t game_next_byte_native(void);

/// $6C4B -- the game's pseudo-random byte, always $00-$7F.
uint8_t game_rand_byte_native(void);

/// $7642 -- put an apple on a free cell, by rejection sampling.
Cell game_place_apple_native(void);

/// $71CD -- recompute what one apple is worth for the current level.
void game_set_apple_value_native(void);

/// $6BEF -- plot the head on the occupancy map and flag it as newly there.
void game_mark_head_native(uint8_t row, uint8_t col);

/// $6BDA -- draw a cell, merging the head shape over it if the head is on it.
void game_draw_head_native(uint8_t ink, Cell c);

/// $7633 -- count one apple eaten, and make the noise for it.
void game_award_extra_life_native(void);

/// $60E4 -- load a shape and draw it into the current cell.
void game_plot_shape_native(uint8_t ink, Cell c);

/// $64A9 -- the rising-then-falling sweep an eaten apple makes.
void game_sound_sweep_native(void);

/// $7590 -- show \p key as slot \p slot's binding on the redefinition screen.
void game_show_key_native(uint8_t slot, uint8_t key);

/// $6B3D -- draw both side walls, and leave SCRN of the bottom-centre cell.
uint8_t game_draw_side_walls_native(void);

/// $6217 -- poll the keyboard and push what it finds into the ring at $623C.
void game_read_key_native(void);

/// $664A -- draw \p ch through the game's own hi-res font, then hand it on to
/// the ROM's COUT1 so the cursor still moves.
void game_cout_hook_native(uint8_t ch);

/// Why a life ended. The original says all of this in one byte at $6253, which
/// its caller at $7739 reads the moment the routine returns; game_play_loop's
/// adapter is what puts the byte back.
typedef enum {
  /// Reached the gate: column $14 of row 0. $6253 stays 0.
  LIFE_GATE,
  /// Moved onto an apple. $6253 is $0F.
  LIFE_APPLE,
  /// The quit key. $6253 is $FF.
  LIFE_QUIT,
  /// The level timer at $6255 ran out. $6253 is $FE.
  LIFE_TIMEOUT,
  /// Moved onto something solid. $6253 is that cell, which is what
  /// \p cell_out receives -- the caller distinguishes the cases by it.
  LIFE_CRASH,
} LifeEnd;

/// $6288 -- play one life: steer, move, draw and pace the snake until
/// something ends it. \p cell_out receives the occupancy byte the head landed
/// on, which is only meaningful for LIFE_CRASH.
LifeEnd game_play_loop_native(uint8_t *cell_out);

/// What the auto-steer decided. $6A32 answers in A, and $6288 re-examines the
/// answer as though it had been typed, so the two spellings are a key and a
/// direction number -- telling them apart is the high bit.
typedef enum {
  /// Carry on in the direction the snake is already going.
  STEER_STRAIGHT,
  /// Turn. \p key_out holds the absolute-direction key that achieves it.
  STEER_TURN,
  /// Nothing is safe. The original says this by discarding its own return
  /// address and jumping to $6315 in its caller, which is "carry on" reached
  /// without the caller's consent; see the alternate-exit transform in
  /// tools/apple2tc/routines.cpp.
  STEER_BOXED_IN,
} SteerChoice;

/// $6A32 -- steer toward the apple at $6B3B/$6B3C, trying candidate directions
/// in order of usefulness and taking the first that snake_move_verdict()
/// allows. Leaves the direction it settled on in $6B38, as the original does.
SteerChoice game_auto_steer(uint8_t *key_out);

/// $72CE -- draw the status panel: six labelled BCD fields in a 2x3 grid
/// across the bottom three text rows, then home the cursor.
void game_status_panel(void);

/// $78B3 -- the bonus screen: award twice the apple's value, draw a box over
/// the playfield, print BONUS and the amount, and hold it there.
void game_bonus_screen(void);

/// $6256 -- set up a life and hand over to the main loop: the snake as a
/// single cell at the bottom centre facing up, ten segments of growth owed,
/// the timer full, and the key ring empty.
void game_begin_life(void);

/// $7980 -- the setup screen: seed the random pointer, then either ask for a
/// difficulty (falling back to the demo if nobody answers) or run the key
/// redefinition screen.
void game_setup_screen(void);

/* ========================================================================== *
 * The 6502-shaped adapters -- declarations                                 *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */



/// \file
/// Snake Byte routines decompiled by hand, because apple2tc could not reach
/// them. See game.c for why each one is here.
///
/// Like a2rom.c, game.c is textually included into the generated translation
/// unit and is never compiled on its own.

/// $664A -- the game's own COUT handler, a hi-res text renderer.
/// $7230 -- print the NUL-terminated string that follows the call.
void game_print_inline_str(uint16_t ret_addr);


/* --- $60E4/$60E7/$6127: the hi-res cell plotter -------------------------- */

/// $6127 -- load the four scanline masks for shape $00 into $6060.

/// $60E7 -- draw the currently-loaded shape into the cell at row $03,
/// column $02, in ink $01.

/// $60E4 -- load the shape for $00, then draw it. The form nearly every
/// caller uses.

/// $6148 -- plot a horizontal run of cells, from column $02 through column
/// $08 inclusive, along row $03.

/// $615A -- plot a vertical run of cells, from row $03 through row $08
/// inclusive, down column $02.

/* --- $7000/$7019/$7024: the screen-script primitives ---------------------- */

/// $7019 -- fetch the next byte of the display list into A and advance the
/// $0A/$0B pointer.

/// $7024 -- set the lo-res colour from the ink flag in Z: black if zero,
/// grey otherwise. Tail-calls the ROM's SETCOL.

/// $7000 -- plot a vertical run on the lo-res occupancy map, from row $03
/// through row $08 inclusive at column $02, leaving $03 unchanged.

/* --- $702B/$71F3/$7226/$7267: the score ---------------------------------- */

/// $71F3 -- print one BCD byte as two digits, suppressing leading zeros via
/// the $002C flag.

/// $7226 -- print '0' if $002C shows no digit was printed. Called once at the
/// end of a multi-byte number.

/// $7267 -- add $71CC:$71CB to the four-byte BCD score at $7252.

/// $702B -- zero hi-res page 1, $2000 through $3FFF.

/* --- $6B93 --------------------------------------------------------------- */

/// $6B93 -- load the shape for $00 and merge it into the cell at row $03,
/// column $02, setting bits rather than replacing the byte.

/// $7045 -- clear the screen, draw the border, then interpret the current
/// level's display list at $8000. See game.c for the opcodes.

/* --- snake state and scoring setup --------------------------------------- */

/// $6641 -- point CSWL/CSWH at $664A so COUT reaches game_cout_hook.

/// $660F -- adapter for game_start_life(): head column in A, both bouncers
/// placed at opposite corners.

/// $6BEF -- PLOT the head onto the lo-res map and raise $0305 and $6C46.

/// $6BDA -- draw the caller's cell, merging shape 1 over it when $0305 is set.

/// $71CD -- set the per-apple score at $71CB/$71CC to $71C8[difficulty] times
/// the level, in BCD.

/* --- apples and sound ---------------------------------------------------- */

/// $7642 -- place a new apple on a free cell, found by rejection sampling.

/// $64A9 -- a rising then falling pitch sweep, clicked through $6C49.

/// $7633 -- count one apple eaten and play the sweep.

/// $6217 -- poll the keyboard into the 16-entry ring buffer at $623C.

/// $7590 -- show the character in A at slot X of the key-redefinition screen.

/// $6B3D -- draw both side walls in two inks, with a randomly placed seam.

/// $6AB8 -- can the snake step in direction $6B38? Returns A = 0 / Z set for
/// yes, and refuses dead ends one move early.

/// $64C8 -- step the bouncer at $6633/$6634 by its deltas, reflecting off
/// whatever it hits.
void game_move_bouncer(Bouncer *b);

/// $728D -- copy the score at $7252 over the high score at $7256 if it beats
/// it, comparing BCD bytes most significant first.

/// $6BFB -- twenty passes of a falling tone, driven by the period at $6C46.

/// $6594 -- step the bouncers the difficulty calls for, then return the next
/// queued key in A.

/// $69C3 -- find an apple by sweeping columns outward from the snake, leaving
/// the result at $6B3B/$6B3C.

/// $69A9 -- adapter for game_pause_or_toggle_sound_native(): ESC pauses until
/// a key, Ctrl-S toggles the sound flag at $69C2.

/// $75D1 -- blink slot X of the key-redefinition screen and wait for a
/// replacement key.

/// $6C72 -- turn the next key, or the joystick, into a direction.

/// $6288 -- play one life, and leave the reason it ended in $6253, which is
/// what the caller at $7739 reads.
static void game_play_one_life(void);





/* ========================================================================== *
 * Apple II ROM entry points, hand-written                                  *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Hand-written replacements for the Apple II ROM entry points listed in
/// `rom.externs`. See `a2rom.h` for the per-routine documentation.
///
/// ============================================================================
/// THIS FILE IS NOT A STANDALONE TRANSLATION UNIT. IT MUST BE #include-d.
/// ============================================================================
///
/// The emulated machine state lives in `apple2tc/system2-inc.h`, and almost all
/// of it has *internal linkage*:
///
///   - `s_a`, `s_x`, `s_y`, `s_sp`, `s_status_*`, `s_ram`, `s_pc`, `s_cycles`,
///     `s_remaining_cycles` are all `static`. This file no longer uses the
///     registers at all, but the header still defines them.
///   - `peek`, `poke`, `peek16`, `ram_peek16al`, `push8`, `pop8`, `push16`,
///     `pop16`, `adc_decimal`, `sbc_decimal` are all `static`.
///   - The `CYCLES()` macro expands to references to `s_pc`, `s_cycles`,
///     `s_remaining_cycles` and a *local* variable `branchTarget`, which is
///     why the only use of `CYCLES` here goes through GAME_CYCLES_COORD --
///     that macro declares one inside its own block.
///
/// Only `ram_peek`, `ram_poke`, `ram_peek16`, `io_peek`, `io_poke` and
/// `error_handler` have external linkage (declared in `apple2tc/system.h`).
///
/// A separate `.c` file therefore cannot see the CPU state at all. In addition,
/// this file calls the still-generated helpers `rom_vtabz`, `rom_clreolz`,
/// `rom_clreol` and `rom_wait` and uses `ovf8()` / `adc_dec16()`, which
/// the decompiler emits as `static` in the generated file.
///
/// Consequently this file must be textually included into the same translation
/// unit as the generated C, *after* it. That is what `snake-byte-ext.c` does,
/// and `snake-byte-ext.c` is the file CMake compiles for the `snake-bytec1-ext`
/// target:
///
///     #include "snake-bytec1-ext.c"
///     #include "a2rom.c"
///
/// DO NOT add `a2rom.c` to CMake as a source file of its own. Compiling it
/// alone yields a wall of undefined identifiers, because everything listed
/// above is invisible outside the generated file's translation unit. Worse, if
/// it ever did compile separately it would get its *own* copy of `s_ram` and
/// the registers -- `system2-inc.h` declares them `static` -- and link cleanly
/// while reading and writing a machine state nothing else can see.
///
/// This file is used only by the `snake-bytec1-ext` build. The plain
/// `snake-bytec1` target is the self-contained reference build, which
/// decompiles the ROM instead of calling into here.


/* Helpers that remain in the generated code. Redeclared here so that this file
   reads standalone; C permits identical redeclarations. */
bool rom_vtabz(uint8_t line);
bool rom_clreolz(uint8_t col);
void rom_clreol(void);
void rom_wait(uint8_t n);

/* $FDF0 COUT1, defined below. `rom_cout` dispatches to it, and so does the
   game's own $664A handler in game.c once it has drawn its glyph. */
static void rom_cout1(uint8_t ch);

/* ========================================================================== */
/* Private helpers.                                                           */
/*                                                                            */
/* $F847 GBASCALC and $F80E PLOT1 used to be emitted as `FUNC_GBASCALC` and    */
/* `FUNC_PLOT1`. Their only callers were $F800/$F819/$F871, so once those      */
/* became external the two helpers became unreachable and the decompiler       */
/* dropped them. They are reproduced here verbatim from the pre-externs        */
/* output.                                                                    */
/* ========================================================================== */

/// $F847 GBASCALC. Compute the lo-res base address for row A into GBASL/GBASH
/// ($26/$27).
/// $F847 GBASCALC. The lo-res twin of BASCALC: a row 0-47 in A becomes that
/// row's base address in GBASL/GBASH.
///
/// Same shape as BASCALC and the same trick -- LSR puts the row's low bit in
/// the carry, the next three bits pick the band, and the `ADC #$7F` folds in
/// the half. It differs in the tail: BASCALC ORs the shifted value back in to
/// build a text address, and this one shifts by two and ORs, which lands on
/// the lo-res page instead.
static void rom_gbascalc(uint8_t row) {
  /*$F847*/ TICK(20);
  const uint8_t odd = (uint8_t)(row & 0x01);
  /*$F84D*/ const uint8_t page = (uint8_t)(((row >> 0x01) & 0x03) | 0x04);
  /*$F850*/ uint8_t band = (uint8_t)(row & 0x18);

  if (odd) {
    /*$F854*/ TICK(2);
    band = (uint8_t)((band + 0x7f) + odd);
  } else {
    // $F852 BCC -- the branch itself, taken here (not modelled by the block
    // above's own cost, which is the not-taken total; see the design doc on
    // edge costs).
    /*$F852*/ TICK(1);
  }

  /*$F856*/ TICK(19);
  /*$F85C*/ s_gbas = (uint16_t)((uint8_t)((uint8_t)(band << 0x02) | band) | (page << 8));

  /*$F85E*/
}

/// $F80E PLOT1. Store the color mask ($30) into the lo-res half-byte selected
/// by MASK ($2E) at GBASL/GBASH ($26) + Y.
static void rom_plot1(uint8_t col) {
  // One lo-res cell: replace the half of the byte MASK selects with the
  // matching half of COLOR, leaving the other half alone. `(old ^ colour) &
  // mask ^ old` is the ROM's way of saying that in three instructions.
  /*$F80E*/ TICK(28);
  const uint16_t at = (uint16_t)(s_gbas + col);
  const uint8_t old = peek(at);
  /*$F816*/ poke(at, (uint8_t)(((old ^ s_color) & s_mask) ^ old));

  /*$F818*/
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
  /*$F800*/ TICK(11);
  const uint8_t half = (uint8_t)(row >> 0x01);
  const bool upper = (row & 0x01) != 0;
  // PHP/PLP across GBASCALC, to carry the LSR's bit past a call that clobbers
  // the flags. The bit is `row & 0x01`, and nothing else survived the round
  // trip, so it is simply the value.
  /*$F802*/ rom_gbascalc(half);

  /*$F805*/ TICK(8);
  const uint8_t carry = (uint8_t)(row & 0x01);
  /*$F806*/ uint8_t mask = 0x0f;

  if (upper) {
    /*$F80A*/ TICK(2);
    mask = (uint8_t)(((uint16_t)mask + 0x00e0) + carry);
  } else {
    // $F808 BCC -- the branch itself, taken here.
    /*$F808*/ TICK(1);
  }

  /*$F80C*/ TICK(3);
  s_mask = mask;
  rom_plot1(col); // JMP -- a tail call.
}

/* ========================================================================== */
/* $F819 HLINE                                                                */
/* ========================================================================== */

/// $F819 HLINE, and $F826 VLINEZ, which share this body.
///
/// HLINE plots along row A from column Y to H2. Its loop ends by falling out
/// of the bottom into VLINEZ's, which walks rows from A down to V2 in column
/// Y -- so one entry point draws a horizontal run and the other a vertical
/// one, out of the same six instructions.
///
/// The labels keep their addresses rather than becoming nested loops, for the
/// same reason game_cold_start's do: the two loops are entered at different
/// depths and leave through each other, which C's loop forms cannot say
/// without a flag that the original does not have.
void rom_hline(uint8_t row, uint8_t from_col) {
  uint8_t col = from_col;
  // The CMPs below leave it and the ADC at $F826 reads it back.
  uint8_t carry;

  /*$F819*/ TICK(6);
  rom_plot(row, col);

across: /* $F81C -- one column at a time, up to H2 */
  TICK(5);
  carry = (uint8_t)(col >= s_h2);
  if (carry) {
    // $F81E BCS -- the branch itself, taken here.
    /*$F81E*/ TICK(1);
    goto done;
  }

  /*$F820*/ TICK(8);
  col = (uint8_t)(col + 0x01);
  /*$F821*/ rom_plot1(col);
  /*$F824*/ TICK(2);
  if (!carry) {
    // $F824 BCC -- the branch itself, taken here.
    /*$F824*/ TICK(1);
    goto across;
  }

down: /* $F826 -- one row at a time, up to V2 */
  TICK(2);
  row = (uint8_t)(((uint16_t)row + 0x0001) + carry);

  /*$F828*/ TICK(9);
  /*$F829*/ rom_plot(row, col);
  /*$F82C*/ TICK(9);
  /*$F82D*/ carry = (uint8_t)(row >= s_v2);
  if (!carry) {
    // $F82F BCC -- the branch itself, taken here.
    /*$F82F*/ TICK(1);
    goto down;
  }

done:
  /*$F831*/ TICK(6);
}

/* ========================================================================== */
/* $F864 SETCOL                                                               */
/* ========================================================================== */

void rom_setcol(uint8_t ink) {
  // The lo-res colour is stored in both nibbles, so a PLOT can take whichever
  // half MASK selects without shifting. Four ASLs and an ORA get there in the
  // original; the carry they leave is read by nothing.
  /*$F864*/ TICK(25);
  const uint8_t low = (uint8_t)(ink & 0x0f);
  s_color = (uint8_t)((low << 0x04) | low);

  /*$F870*/
}

/* ========================================================================== */
/* $F871 SCRN                                                                 */
/* ========================================================================== */

uint8_t rom_scrn(uint8_t row, uint8_t col) {
  // The row's low bit says which half of the byte holds this cell, and the
  // ROM keeps it across GBASCALC on the stack -- as the whole status
  // register, because LSR put it in the carry and PHP is one byte.
  /*$F871*/ TICK(11);
  const uint8_t half = (uint8_t)(row >> 0x01);
  const bool upper = (row & 0x01) != 0;

  /*$F873*/ rom_gbascalc(half);

  /*$F876*/ TICK(11);
  uint8_t cell = peek((uint16_t)(s_gbas + col));

  // The original brackets GBASCALC with PHP/PLP to keep the LSR's carry. It is
  // read by nothing here.

  if (upper) {
    /*$F87B*/ TICK(8);
    cell = (uint8_t)(cell >> 0x04);
  } else {
    // $F879 BCC -- the branch itself, taken here.
    /*$F879*/ TICK(1);
  }

  /*$F87F*/ TICK(8);
  /*$F881*/
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
  TICK(13);
  line = s_wndtop;
  /*$FC5A*/ s_cv = s_wndtop;
  /*$FC5E*/ s_ch = 0x00;
  // $FC60 BEQ -- provably always taken (Y was just loaded 0), but the branch
  // instruction still executes and still pays its own cost every time. The
  // decompiler doesn't do cross-instruction flag proofs either, so it keeps
  // charging this the same way.
  /*$FC60*/ TICK(1);

  for (;;) { /* $FC46 -- CLRSC2, one line per pass */
    TICK(9);
    /*$FC47*/ rom_vtabz(line);
    /*$FC4A*/ TICK(6);
    // $FC5C loaded Y with 0 and CLREOLZ is the only thing that moves it, so
    // every pass starts the blank at column 0. Its carry comes back out as the
    // +1 below: $FC4D's `ADC #$00` has no CLC in front of it, and this is why.
    const bool step = rom_clreolz(0x00);

    /*$FC4D*/ TICK(13);
    line = (uint8_t)(line + step);

    /*$FC52*/ const bool past_bottom = line >= s_wndbtm;
    if (!past_bottom) {
      // $FC54 BCC -- the branch itself, taken here.
      /*$FC54*/ TICK(1);
      continue;
    }

    /*$FC56*/ TICK(2);
    if (!past_bottom)
      goto home; // the BCS's not-taken arm, which cannot be reached
    // $FC56 BCS -- the branch itself, same address as the block above because
    // this is a singleton one-instruction block.
    /*$FC56*/ TICK(1);
    break;
  }

  /*$FC22*/ TICK(3); // TABV
  rom_vtabz(s_cv); // JMP -- a tail call.
}

/* ========================================================================== */
/* $FC68 -- the tail of LF: VTAB, or scroll if CV has run off the window.      */
/*                                                                            */
/*   FC68: LDA CV / CMP WNDBTM / BCC VTABZ    ; usual case: just recompute BAS */
/*   FC6E: DEC CV / LDA WNDTOP / PHA / ...    ; otherwise scroll up one line   */
/*                                                                            */
/* Transcribed from the pre-externs mega-switch blocks $FC68..$FC95 plus the   */
/* three exit blocks ($FC6C -> VTABZ, $FC22, $FC9A -> CLREOL).                */
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

     Not checked by running it: nothing in either scenario scrolls, and $FC8C
     and $FC93 are on probe-acceptance.sh's unverified list to say so.
     Inverting the test below passes all six cold checks. This rests on the
     dominance argument above, not on a green gate. */
  uint8_t negative = 0;

  uint8_t line;
  uint8_t col = 0;
  bool step = false;

  /*$FC68*/ TICK(8);
  if (!(s_cv >= s_wndbtm)) {
    // $FC6C BCC -- the branch itself, taken here. Nothing to scroll.
    /*$FC6C*/ TICK(1);
    rom_vtabz(s_cv); // JMP -- a tail call.
      return;
  }

  /*$FC6E*/ TICK(17);
  s_cv = (uint8_t)(s_cv - 0x01);
  /*$FC70*/ line = s_wndtop;
  /*$FC73*/ step = rom_vtabz(line);

scroll: /* $FC76 -- one line up per pass */
  TICK(28);
  /*$FC78*/ s_bas2 = s_bas;
  /*$FC80*/ col = (uint8_t)(s_wndwdth - 0x01);
  // $FC82's ADC has no CLC either; the carry is whatever VTABZ last returned.
  line = (uint8_t)(((uint16_t)line + 0x0001) + step);

  if (line >= s_wndbtm) {
    // $FC86 BCS -- the branch itself, taken here. That was the last line.
    /*$FC86*/ TICK(1);
    goto last_line;
  }

  /*$FC88*/ TICK(9);
  /*$FC89*/ step = rom_vtabz(line);

copy: /* $FC8C -- one character, right to left */
  TICK(15);
  {
    const uint8_t at = col;
    /*$FC8E*/ poke((uint16_t)(s_bas2 + at), peek((uint16_t)(s_bas + at)));
    /*$FC90*/ const uint8_t next = (uint8_t)(at - 0x01);
    negative = (uint8_t)(next & 0x80);
    col = next;
    if (!negative) {
      // $FC91 BPL -- the branch itself, taken here (loop back).
      /*$FC91*/ TICK(1);
      goto copy;
    }
  }

  /*$FC93*/ TICK(2);
  if (negative) {
    // $FC93 BMI -- the branch itself, taken here (outer loop back).
    /*$FC93*/ TICK(1);
    goto scroll;
  }

last_line: /* $FC95 -- blank what the scroll left at the bottom */
  TICK(8);
  /*$FC97*/ const bool filled = rom_clreolz(0x00);

  /*$FC9A*/ TICK(2);
  if (!filled) {
    rom_clreol(); // JMP -- a tail call.
      return;
  }
  // $FC9A BCS -- taken here, and it falls into the trampoline charge below
  // before continuing; the not-taken arm above jumps straight out without it.
  /*$FC9A*/ TICK(1);

  /*$FC22*/ TICK(3); // TABV
  rom_vtabz(s_cv);
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
/// **Three of its arms are decoded and never run.** Probed at their entry
/// addresses across both cold scenarios: the character store fires 204 and 651
/// times and the carriage return and line feed 0 and 17, but the Ctrl-S
/// handshake ($FB85), its spin ($FB88), the backspace ($FC10) and the bell
/// ($FBD9 and $FBDD) fire **zero** times in either. Everything this game
/// prints is printable, a return, or a line feed; it never emits a Ctrl-G or a
/// backspace, and no recording holds a key down across a return. Those arms
/// rest on the binary alone.
///
/// The labels keep their addresses. This is a dispatcher whose arms rejoin at
/// several depths -- backspace falls into the cursor-up path, a wrapped line
/// falls into the carriage return, and a carriage return falls into the line
/// feed -- and writing it as nested ifs would need each of those spelled out
/// twice.
static void rom_coutz(uint8_t ch) {
  /*$FB78*/ TICK(4);
  if (ch != 0x8d) {
    /*$FB7A*/ TICK(1);
    goto emit;
  }

  /* $FB7C -- the Ctrl-S handshake, on a carriage return only. If a key is
     already waiting and it is Ctrl-S, stop here until another key arrives.
     Ctrl-C is left in the keyboard latch on the way out so that whatever is
     running next still sees it; anything else is consumed. */
  /*$FB7C*/ GAME_CYCLES_COORD(0xfb7c, 6);
  uint8_t key = io_peek(0xc000);
  if (!(key & 0x80)) {
    /*$FB7F*/ TICK(1);
    goto emit;
  }

  /*$FB81*/ TICK(4);
  if (key != 0x93) {
    /*$FB83*/ TICK(1);
    goto emit;
  }

  /*$FB85*/ TICK(4);

  for (;;) { /* $FB88 -- spin until a key is pressed */
    TICK(6);
    key = io_peek(0xc000);
    if (!(key & 0x80)) {
      /*$FB8B*/ TICK(1);
      continue;
    }

    /*$FB8D*/ TICK(4);
    if (key == 0x83) {
      // Ctrl-C: leave it latched.
      /*$FB8F*/ TICK(1);
      break;
    }
    /*$FB91*/ TICK(4);
    break;
  }

emit: /* $FB94 JMP $FBFD */
  TICK(3);
  /*$FBFD*/ TICK(4);
  if (!(ch >= 0xa0)) {
    // The not-taken arm jumps straight to the dispatch without the edge charge.
    goto dispatch;
  }
  // $FBFF BCS -- taken here, and it falls into the trampoline charge before
  // continuing.
  /*$FBFF*/ TICK(1);

store: /* $FBF0 -- put the character at the cursor */
  TICK(9);
  /*$FBF2*/ poke((uint16_t)(s_bas + s_ch), ch);

  /*$FBF4*/ TICK(13);
  s_ch = (uint8_t)(s_ch + 0x01);
  {
    const uint8_t width = s_wndwdth;
    const bool past_right_edge = s_ch >= width;
    if (past_right_edge) {
      // Off the right edge, so wrap: the same thing a carriage return does.
      /*$FBFA*/ TICK(1);
      goto carriage_return;
    }
  }
  /*$FBFC*/ TICK(6);
  goto out;

dispatch: /* $FC01 -- not printable; which control code is it? */
  TICK(4);
  if (!(ch & 0x80)) {
    // Below $80 the monitor stores it anyway, high bit and all.
    /*$FC02*/ TICK(1);
    goto store;
  }

  /*$FC04*/ TICK(4);
  if (ch == 0x8d) {
    /*$FC06*/ TICK(1);
    goto carriage_return;
  }

  /*$FC08*/ TICK(4);
  if (ch == 0x8a) {
    /*$FC0A*/ TICK(1);
    goto line_feed;
  }

  /*$FC0C*/ TICK(4);
  if (ch != 0x88) {
    /*$FC0E*/ TICK(1);
    goto bell;
  }

  /* $FC10 -- backspace. Off the left edge wraps to the end of the line above,
     which is why it falls into the cursor-up path rather than returning. */
  /*$FC10*/ TICK(7);
  {
    const uint8_t back = (uint8_t)(s_ch - 0x01);
    s_ch = back;
    if (!(back & 0x80)) {
      /*$FC12*/ TICK(1);
      /*$FBFC*/ TICK(6);
      goto out;
    }
  }

  /*$FC14*/ TICK(11);
  /*$FC16*/ s_ch = s_wndwdth;
  /*$FC18*/ s_ch = (uint8_t)(s_ch - 0x01);

  /*$FC1A*/ TICK(8);
  {
    const uint8_t top = s_wndtop;
    const uint8_t cv = s_cv;
    const bool at_window_top = top >= cv;
    if (at_window_top) {
      // Already on the window's top line; there is nowhere to go up to.
      /*$FC1E*/ TICK(1);
      /*$FC2B*/ TICK(6);
      goto out;
    }
  }

  /*$FC20*/ TICK(5);
  s_cv = (uint8_t)(s_cv - 0x01);
  /*$FC22*/ TICK(3); // TABV
  rom_vtabz(s_cv);
  goto out;

bell: /* $FBD9 -- Ctrl-G, or a control code the monitor does not know */
  TICK(4);
  {
    const uint8_t differs = (uint8_t)(ch != 0x87);
    if (differs) {
      // Not the bell either. Drop it.
      /*$FBDB*/ TICK(1);
      /*$FBEF*/ TICK(6);
      goto out;
    }
  }

  // A tenth of a second of silence, then 192 clicks of the speaker.
  /*$FBDD*/ TICK(8);
  /*$FBDF*/ rom_wait(0x40);
  /*$FBE2*/ TICK(2);
  uint8_t clicks = 0xc0;

  for (;;) { /* $FBE4 */
    TICK(8);
    /*$FBE6*/ rom_wait(0x0c);
    /*$FBE9*/ TICK(8);
    io_peek(0xc030); // the click; the read is the write
    /*$FBEC*/ clicks = (uint8_t)(clicks - 0x01);
    if (!clicks)
      break;
    /*$FBED*/ TICK(1);
  }

  /*$FBEF*/ TICK(6);
  goto out;

carriage_return: /* $FC62 -- to the left edge, then down */
  TICK(5);
  /*$FC64*/ s_ch = 0x00;

line_feed: /* $FC66 */
  TICK(5);
  s_cv = (uint8_t)(s_cv + 0x01);
  /*$FC68*/ rom_fc68(); // JMP -- a tail call, and where a scroll happens.

out:
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
/// In the entire recorded session, however, the vector always points at ROM:
/// the run data records $FDED -> $FDF0 only, and $6641 was never executed
/// (neither it nor its callers $7485/$793F appear in BranchTargets). $664A is
/// therefore classified as data and is not present in the generated C at all.
///
/// Because the recorded session never dispatches anywhere but $FDF0, the
/// verification oracle structurally *cannot* detect a wrong choice here. So a
/// non-ROM vector target is a hard, loud failure rather than a silent fallback
/// to COUT1: a wrong-but-quiet answer would diverge invisibly, whereas an abort
/// fires at exactly the moment the question first matters.
///
/// KNOWN GAP for a later phase: implement $664A (the hi-res text renderer) and
/// dispatch to it here.
/// $FDED COUT -- `JMP ($36)`. Dispatches through the output vector CSWL/CSWH
/// rather than reimplementing COUT1, because Snake Byte repoints it: $6641
/// installs the game's own hi-res text renderer at $664A.
///
/// Any target other than the two we implement is a hard failure rather than a
/// fallback. The recorded session never leaves $FDF0, so `verify.sh` cannot
/// catch a wrong guess here -- a silent fallback would render with the wrong
/// font onto the wrong page, undetectably.
void rom_cout(uint8_t ch) {
  uint16_t vector;

  /*$FDED*/ TICK(5);
            vector = s_csw; // JMP ($36)
            switch (vector) {
            case 0xfdf0:
              rom_cout1(ch);
              break;
            case 0x664a:
              game_cout_hook_native(ch);
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

/// $FDF0 COUT1 -- the ROM's own character output: mask to the current text
/// mode, then COUTZ for the actual placement and cursor bookkeeping.
/// $FDF0 COUT1. Put a character on the text screen, then restore Y.
///
/// Printable characters ($A0 and up) are masked with INVFLG, which is how the
/// monitor does inverse and flashing -- $FF leaves them alone. Control codes
/// are let through unmasked, since mangling them would change what they mean.
static void rom_cout1(uint8_t ch) {
  /*$FDF0*/ TICK(4);
  const bool printable = ch >= 0xa0;

  if (printable) {
    /*$FDF4*/ TICK(3);
    ch = (uint8_t)(ch & s_invflg);
  } else {
    // $FDF2 BCC -- the branch itself, taken here.
    /*$FDF2*/ TICK(1);
  }

  // The original saves Y in YSAV1 across the call and puts it back, because
  // COUT promises its callers it preserves Y. Nothing in this file ever reads
  // Y for a value, so the promise has nobody to keep it to: saving and
  // restoring were the only two things that touched it.
  /*$FDF6*/ TICK(12);
  rom_coutz(ch); // JSR $FB78

  /*$FDFC*/ TICK(13);

  /*$FDFF*/
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

  /*$FE89*/ TICK(11);
  s_a2l = 0x00;
  // X and Y are the built-in device's vector, $FD1B (KEYIN). SETIO below keeps
  // the low half and replaces the page.
  const uint8_t entry_low = 0x1b;
  // $FE91 BNE -- provably always taken (Y was just loaded #$1B, nonzero),
  // same reasoning as $FC60 in rom_home: the decompiler doesn't do
  // cross-instruction flag proofs, so the branch still executes and still
  // pays its own cost every time.
  /*$FE91*/ TICK(1);

  // $FE9B SETIO. A2L is the slot; slot 0 means the built-in device and the ROM
  // answers page $FD, where its own KEYIN and COUT1 live. A real slot would
  // give $Cn00 instead -- decoded, never taken, because the game never sets
  // one.
  /*$FE9B*/ TICK(7);
  /*$FE9D*/ const uint8_t slot = (uint8_t)(s_a2l & 0x0f);
  uint8_t page, low;
  if (slot) {
    /*$FEA1*/ TICK(6);
    page = (uint8_t)(slot | 0xc0);
    low = 0x00;
    // $FEA5 BEQ -- provably always taken (Y was just loaded 0).
    /*$FEA5*/ TICK(1);
  } else {
    // $FE9F BEQ -- the branch itself, taken here.
    /*$FE9F*/ TICK(1);
    /*$FEA7*/ TICK(2);
    page = 0xfd;
    low = entry_low;
  }

  /*$FEA9*/ TICK(14);
  s_ksw = (uint16_t)(low | (page << 8));

  /*$FEAD*/
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

  /*$FE93*/ TICK(9);
  s_a2l = 0x00;
  // The same, for $FDF0 (COUT1).
  const uint8_t entry_low = 0xf0;

  // $FE9B SETIO. A2L is the slot; slot 0 means the built-in device and the ROM
  // answers page $FD, where its own KEYIN and COUT1 live. A real slot would
  // give $Cn00 instead -- decoded, never taken, because the game never sets
  // one.
  /*$FE9B*/ TICK(7);
  /*$FE9D*/ const uint8_t slot = (uint8_t)(s_a2l & 0x0f);
  uint8_t page, low;
  if (slot) {
    /*$FEA1*/ TICK(6);
    page = (uint8_t)(slot | 0xc0);
    low = 0x00;
    // $FEA5 BEQ -- provably always taken (Y was just loaded 0).
    /*$FEA5*/ TICK(1);
  } else {
    // $FE9F BEQ -- the branch itself, taken here.
    /*$FE9F*/ TICK(1);
    /*$FEA7*/ TICK(2);
    page = 0xfd;
    low = entry_low;
  }

  /*$FEA9*/ TICK(14);
  s_csw = (uint16_t)(low | (page << 8));

  /*$FEAD*/
}

/* ========================================================================== *
 * The game, as ordinary C                                                  *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte as ordinary C. See game_native.h for what this file is for and
/// what it costs to move a routine into it.
///
/// Like a2rom.c and game.c this is textually included into the generated
/// translation unit, and it must come *before* game.c, whose adapters call
/// into it.


/* ========================================================================== */
/* Storage                                                                    */
/* ========================================================================== */

/// The two bouncers. They lived at $6639-$6640, and not as two structs: the
/// fields were interleaved in pairs -- both columns, then both rows, then both
/// dx, both dy -- so nothing could be overlaid on them and every access was a
/// load or a store of four separate bytes.
///
/// Out of RAM the interleaving has nothing left to say, and this is an array
/// of two. The initialisers are the shipped image's bytes de-interleaved;
/// game_start_life overwrites both before the first life.
static Bouncer s_bouncers[2] = {
    {.col = 0x04, .row = 0x04, .dx = +1, .dy = +1},
    {.col = 0x26, .row = 0x01, .dx = -1, .dy = +1},
};

/// $6C63 -- the six bound keys, in slot order. The key-redefinition screen
/// writes them; kKeyDefaults is the same six as shipped and is never written.
static uint8_t s_key_table[6] = {0xc9, 0xca, 0xcb, 0xcd, 0x88, 0x95};

/// $6060 -- the loaded shape's four scanline masks, which game_load_shape
/// copies out of kShapeMaskTable and the two hi-res cell drawers read.
/// $FF FF FF FF in the shipped image, which is what a shape of all dots
/// would leave there.
static uint8_t s_shape_mask[4] = {0xff, 0xff, 0xff, 0xff};

/* --- The game's own tables ------------------------------------------------ */
/*
 * Read-only data inside the loaded image, and const arrays here. They were
 * ram_peek(kTable + i) until 2026-08-25, which is what a 6502 has to write and
 * not what the data is: every one of these is a lookup indexed by a small
 * integer.
 *
 * Every extent below is *derived*, not assumed -- the playbook's own red flag,
 * because the listing does not delimit a table and a 16-byte one that is
 * really 128 is invisible until something indexes past 16. Each says where its
 * end comes from. The bytes are the shipped image's, transcribed from
 * snake-byte.b33; a wrong byte in any of them moves pixels, and the screen
 * oracle compares 6,808 and 9,524 samples.
 */

/// $6000/$6030 -- each hi-res cell row's base address. The original splits the
/// 48 addresses into parallel low and high tables because a 6502 indexes bytes;
/// one table of addresses is the same data said once. kHgrLineHi ended at
/// $605F, where the shape masks began.
static const uint16_t kHgrLineBase[48] = {
    0x2000, 0x3000, 0x2080, 0x3080, 0x2100, 0x3100,
    0x2180, 0x3180, 0x2200, 0x3200, 0x2280, 0x3280,
    0x2300, 0x3300, 0x2380, 0x3380, 0x2028, 0x3028,
    0x20a8, 0x30a8, 0x2128, 0x3128, 0x21a8, 0x31a8,
    0x2228, 0x3228, 0x22a8, 0x32a8, 0x2328, 0x3328,
    0x23a8, 0x33a8, 0x2050, 0x3050, 0x20d0, 0x30d0,
    0x2150, 0x3150, 0x21d0, 0x31d0, 0x2250, 0x3250,
    0x22d0, 0x32d0, 0x2350, 0x3350, 0x23d0, 0x33d0,
};

/// $6064 -- dot patterns, indexed by dot_index(): ink 0-15, scanline parity,
/// column mod 4, so ((15*2+1)<<2)|3 == 127 is the largest index and the table
/// is exactly 128. known-data.txt says the same: "ends exactly at hgr_draw".
static const uint8_t kHgrPattern[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x91, 0xa2, 0xc4,
    0xa2, 0xc4, 0x88, 0x91, 0xc4, 0x88, 0x91, 0xa2, 0x91, 0xa2, 0xc4, 0x88,
    0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x08, 0x11, 0x22, 0x44,
    0x22, 0x44, 0x08, 0x11, 0x2a, 0x55, 0x2a, 0x55, 0x55, 0x2a, 0x55, 0x2a,
    0xd5, 0xaa, 0xd5, 0xaa, 0xd5, 0xaa, 0xd5, 0xaa, 0xf7, 0xee, 0xdd, 0xbb,
    0xdd, 0xbb, 0xf7, 0xee, 0x11, 0x22, 0x44, 0x08, 0x44, 0x08, 0x11, 0x22,
    0xaa, 0xd5, 0xaa, 0xd5, 0xaa, 0xd5, 0xaa, 0xd5, 0x33, 0x66, 0x4c, 0x19,
    0x4c, 0x19, 0x33, 0x66, 0xaa, 0xd5, 0xaa, 0xd5, 0x55, 0x2a, 0x55, 0x2a,
    0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0xaa, 0xd5, 0xaa, 0xd5,
    0x2a, 0x55, 0x2a, 0x55, 0x3b, 0x77, 0x6e, 0x5d, 0x6e, 0x5d, 0x3b, 0x77,
    0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
};

/// $6174 -- four AND masks per shape, indexed (shape << 2) + line.
///
/// The extent is the one that had to be worked out rather than looked up. The
/// data ends at $61DF -- 27 shapes, the last of them $1A, which is the largest
/// the code names. But game_play_loop draws the tail with shape `ahead + $0C`,
/// where `ahead` is a lo-res cell and so 0-15, which reaches shape $1B and
/// index 111. $61E0-$61FF is zero, and $6200 is code (dequeue_key). So the
/// table runs to the code boundary, indices 108-139 are the padding the
/// original reads in that case, and nothing can index past it: the byte
/// arithmetic wraps at 256, but shape is bounded by $1B.
static const uint8_t kShapeMaskTable[140] = {
    0x00, 0x00, 0x00, 0x00, 0x8c, 0xbf, 0x8c, 0x00, 0x00, 0x8c, 0xbf, 0x8c,
    0x98, 0xfe, 0x98, 0x00, 0x8c, 0xbf, 0x8c, 0x00, 0xb3, 0xb0, 0x8f, 0x00,
    0x8f, 0xb0, 0xb3, 0xb3, 0xfc, 0x83, 0xf3, 0xb3, 0xf3, 0x83, 0xfc, 0x00,
    0xff, 0x00, 0xff, 0x00, 0xb3, 0xb3, 0xb3, 0xb3, 0xff, 0x00, 0xff, 0x00,
    0xb3, 0xb3, 0xb3, 0xb3, 0xe0, 0xff, 0xe0, 0x00, 0xbf, 0x0c, 0x0c, 0x0c,
    0x83, 0xff, 0x83, 0x00, 0x0c, 0x0c, 0x0c, 0xbf, 0x8f, 0xb0, 0xb3, 0xb3,
    0xfc, 0x83, 0xf3, 0xb3, 0xf3, 0x83, 0xfc, 0x00, 0xb3, 0xb0, 0x8f, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0xf8, 0xfe, 0xff, 0xff, 0xbf, 0x0f, 0x03,
    0x03, 0x0f, 0xbf, 0xff, 0xff, 0xfe, 0xf8, 0xe0, 0x9c, 0xe3, 0xe3, 0x9c,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/// $6387/$638C/$6391/$6396 -- one five-entry table per absolute-direction key,
/// giving the relative turn that achieves it from the current direction, or
/// $00 for "nothing to do". Five bytes each because they are indexed by
/// direction 1-4 with entry 0 unused; $639B is code, which is where the fourth
/// one ends.
static const uint8_t kTurnForKey[4][5] = {
    {0x00, 0x88, 0x00, 0x95, 0x00,},
    {0x00, 0x00, 0x88, 0x00, 0x95,},
    {0x00, 0x00, 0x95, 0x00, 0x88,},
    {0x00, 0x95, 0x00, 0x88, 0x00,},
};

/// $6A55 -- direction to the key that turns to it. known-data.txt derives the
/// five bytes from the LDA $6A55,X that reads them.
static const uint8_t kSteerKey[5] = {
    0x00, 0xcb, 0xc9, 0xca, 0xcd,
};

/// $6C6A -- the six keys as shipped. The live six at $6C63 are s_key_table,
/// which the redefinition screen writes; these are never written.
static const uint8_t kKeyDefaults[6] = {
    0xc9, 0xca, 0xcb, 0xcd, 0x88, 0x95,
};

/// $71C8 -- what one apple is worth, indexed by difficulty 0-2. Three bytes:
/// $71CB is s_apple_value, which is where the table stops.
static const uint8_t kAppleValueTable[3] = {
    0x10, 0x15, 0x20,
};

/// $75B3-$75D0 -- the redefinition screen's layout, five six-entry tables, one
/// per slot: where the key is printed, where its arrow is, and which glyph the
/// arrow uses. known-data.txt derives the span.
static const uint8_t kKeyCH[6] = {
    0x0a, 0x06, 0x0e, 0x0a, 0x1a, 0x22,
};
static const uint8_t kKeyCV[6] = {
    0x05, 0x09, 0x09, 0x0d, 0x09, 0x09,
};
static const uint8_t kArrowCH[6] = {
    0x0a, 0x08, 0x0c, 0x0a, 0x1c, 0x20,
};
static const uint8_t kArrowCV[6] = {
    0x07, 0x09, 0x09, 0x0b, 0x09, 0x09,
};
static const uint8_t kArrowGlyph[6] = {
    0xe2, 0xe4, 0xe5, 0xe3, 0xe4, 0xe5,
};

/* --- What the bouncer step left behind ------------------------------------ */
/*
 * $6637, $6638 and $6C4A held where the step would have landed before the
 * walls got a say, and whether that cell was occupied. Nothing in this file
 * ever read any of the three -- bouncer_step works in locals and commits into
 * the Bouncer it was handed -- so the three stores existed only because the
 * memory oracle hashed the bytes. Out of RAM they are dead code, and they are
 * gone. The addresses are recorded here because that is the only place the
 * knowledge now lives.
 */

/* --- The auto-steer's answer ---------------------------------------------- */
/*
 * $6B38-$6B3C. $6B39/$6B3A were the apple sweep's cursor, left wherever the
 * search stopped, and went the same way as the three above: written at the end
 * of the sweep, read by nothing. $6B3B/$6B3C are the answer and are real.
 */

/// The direction $6A32 settled on. game_move_ok and key_for_direction both
/// read it back rather than being passed it.
static uint8_t s_steer_dir = 0x02;

/// The cell the sweep decided to steer towards.
static Cell s_apple = {.col = 0x13, .row = 0x1d};

/* --- Sound: $6C46-$6C49 and $69C2 ----------------------------------------- */

/// The tone's period, and its on/off switch: 0 is silent.
static uint8_t s_tone_period;
/// Passes left before the next click.
static uint8_t s_tone_countdown;
/// How many passes one tick of the tone runs.
static uint8_t s_tone_passes = 0x0f;
/// Where the click goes, as the low byte of the soft switch: $30 the speaker,
/// $20 the cassette, which is to say muted.
static uint8_t s_click_port = 0x20;
/// Toggled by Ctrl-S.
static bool s_sound_muted;

/// $6C71 -- the player chose the joystick at the setup prompt.
static bool s_joystick_selected;

/* --- The plotter's arguments ---------------------------------------------- */
/*
 * Nine bytes that lived at $0000-$0008 because the 6502 had nowhere else to
 * pass arguments. Every plot goes through them: set the shape, the ink and the
 * cell, then call.
 *
 * They are C variables now, and no longer aliased with anything -- the glyph
 * blitter, which used to borrow all nine for a font pointer and the caller's
 * registers, has its own locals. Making them actual parameters is the step
 * after this one, and each needs its own argument first: s_shape turned out to
 * be a genuine global, because bouncer_step erases with whatever was last left
 * in it.
 */
/*
 * Nine bytes of zero page that the drawing routines pass arguments in, because
 * the 6502 has nowhere else to put them. Every plot goes through here: set the
 * shape, the ink and the cell, then call.
 *
 * game_cout_hook_native is the exception and reuses all nine for something
 * unrelated -- see the second enum. That is not a naming problem to be tidied
 * away; it is a union the original wrote by hand, and the names say so.
 */
/// $0000 -- which shape to draw. game_load_shape turns it into the four
/// scanline masks at $6060.
///
/// A variable and not a parameter, which is what the original makes it:
/// bouncer_step erases where the bouncer was using whatever was last left
/// here, so the value outlives any one call. Every path that could have made
/// that a hazard was checked -- see the note below.
static uint8_t s_shape;


/// The block itself, moved out of emulated RAM and split into variables now
/// that the glyph blitter no longer shares it.
///
/// They are still variables rather than parameters, which is the next step and
/// not this one. What changed is that nothing can reach them by address, and
/// each one has a type and a name instead of an index.
/// The lo-res colour is gone from here. It was a global for the reason
/// s_shape still is -- the original inherits it: draw_border drew six runs in
/// whatever ink game_draw_playfield had set, and the redefinition screen's
/// stem took the ink of the arrow plotted above it. But every one of those
/// inheritances turned out to be readable at the call site, so the run helpers
/// take the ink as an argument and the inheritance is written down instead of
/// implied.
///
/// s_shape did not thread and is still below. plot_at's two callers -- the
/// bouncer erase and the tail erase -- genuinely erase with whatever shape was
/// last used, so passing one would invent a value the original does not have.


/// SCRN one cell. Defined further down, next to the plot helpers.
static uint8_t scrn_cell(Cell c);

/// Plot the loaded shape, or a named one, into a cell. Defined further down,
/// next to the run helpers; declared here because the bouncers and the snake
/// use them well before that.
static void plot_at(uint8_t ink, Cell c);
static void plot_shape_at(uint8_t shape, uint8_t ink, Cell c);

/// The glyph blitter at $664A used to park five values in this block -- the
/// glyph, the caller's X and Y, and a source and destination pointer. They are
/// locals in game_cout_hook_native now.
///
/// Splitting them out needed an argument, because bouncer_step erases with
/// whatever was last left in s_shape: a COUT through the hi-res hook between a
/// shape write and that erase used to change which cells got erased. The
/// argument is that it cannot happen. The hook is installed in exactly two
/// places, game_bonus_screen and game_setup_screen, and each restores CSWL
/// before returning. Inside both windows every plot writes the shape
/// immediately before its call. And every path out of either window reaches
/// game_draw_playfield, which writes $15 unconditionally before it draws
/// anything, before the play loop can run -- game_bonus_screen returns into
/// round_cleared and game_setup_screen into new_game, and both fall through
/// start_round.
///
/// The gate could not have caught this being wrong until 2026-08-23: the cold
/// build ran $664A zero times under it. It now runs it 205 times per run, and
/// samples state at $760F while it does.

/* --- The snake, the key ring, and the click counter ----------------------- */
/*
 * $6232-$6255 in the original, one contiguous run holding everything a life
 * consists of. The two delta tables are read-only image data and stay where
 * they are, as const arrays; everything after them is state and is C
 * variables. The initialisers are the shipped image's bytes, which for this
 * block are the leftovers of whoever last ran the game before it was
 * assembled: game_begin_life overwrites all of them, but game_draw_status and
 * the first pace read some of them first.
 */

/// The four directions, as $624E numbered them.
enum { DIR_RIGHT = 1, DIR_UP = 2, DIR_LEFT = 3, DIR_DOWN = 4 };

/// Column and row deltas, indexed by direction 1..4. Entry 0 is $00 in both
/// and is never reached, since a direction of 0 means "no key".
/// Measured against DIR_RIGHT..DIR_DOWN: columns are +1/0/-1/0 and rows are
/// 0/-1/0/+1, so right is +column and up is -row, screen order.
static const uint8_t kColDelta[5] = {0x00, 0x01, 0x00, 0xff, 0x00};
static const uint8_t kRowDelta[5] = {0x00, 0x00, 0xff, 0x00, 0x01};

/// The sixteen-entry ring the keyboard scan fills and the play loop drains.
static uint8_t s_key_ring[16];

/// Where the play loop reads next, and where the scan writes next. The scan
/// refuses to advance the write cursor onto the read one, which is the ring's
/// only full test. Both wrap with `& $0F`.
static uint8_t s_ring_read;
static uint8_t s_ring_write;

/// The direction the snake is travelling, DIR_RIGHT..DIR_DOWN.
static uint8_t s_direction = DIR_UP;

/// The head's cell, and the tail's. The snake itself is not stored -- the
/// lo-res screen is the occupancy map, and the tail walks it by reading the
/// colour it finds to work out which way the body went.
static Cell s_head = {.col = 0x14, .row = 0x24};
static Cell s_tail = {.col = 0x14, .row = 0x27};

/// How the life ended, which $7739 reads the moment the play loop returns:
/// $00 the gate, $0F an apple, $FF the quit key, $FE the timer, anything
/// else the occupancy byte the head ran into. See LifeEnd, which is this
/// byte with names on.
static uint8_t s_life_outcome;

/// Segments still owed. While it is nonzero the tail is not trimmed, so the
/// snake grows; a life starts with ten and each apple adds ten more.
static uint8_t s_growth = 0x07;

/// The life timer -- see s_life_time for what seeds it and why $FF stops it.
static uint8_t s_life_timer = 0x61;

/// How many more clicks the current sound effect owes. The pace loop spends
/// one per pass while it is nonzero, so the value is a duration: $20 for
/// eating an apple, $10 for a steering input, $07 for growing.
static uint8_t s_click_count;

/* ========================================================================== */
/* Converted routines                                                         */
/* ========================================================================== */

uint8_t game_start_life(uint8_t head_col) {
  /*$660F*/ TICK(50);
  s_head.col = head_col;

  // Opposite corners, converging. The original's nine stores are these two.
  const Bouncer a = {.col = 0x01, .row = 0x01, .dx = +1, .dy = +1};
  const Bouncer b = {.col = 0x26, .row = 0x01, .dx = -1, .dy = +1};
  s_bouncers[0] = a;
  s_bouncers[1] = b;

  // $6630 `LDA #$14`. Its one caller, $6256, stores this as the tail column;
  // it is not related to head_col, which happens to be $14 as well.
  return 0x14;
}

void game_load_shape_masks(uint8_t shape) {
  /*$6127*/ TICK(53);
  // Four masks per shape at $6174, into the four the plotter reads.
  uint8_t last = 0;
  for (unsigned line = 0; line < 4; ++line) {
    last = kShapeMaskTable[(uint8_t)((uint8_t)(shape << 2) + line)];
    s_shape_mask[line] = last;
  }
}

void game_install_cout_vector(void) {
  /*$6641*/ TICK(16);
  // CSWL/CSWH at $36/$37, pointed at $664A.
  s_csw = 0x664a;
}

/* ========================================================================== */
/* $64C8 -- the bouncer                                                       */
/*                                                                            */
/* What the original spends bytes on and this does not: $6633-$6636 are a      */
/* parameter block the caller copies in and out, $6637/$6638 are the candidate */
/* cell, and $6C4A counts how many axes were blocked. All five are locals in   */
/* any language with a stack, and all five are locals here -- the parameter    */
/* block is the argument, and the other three were mirrored back only for as   */
/* long as the memory oracle hashed the bytes.                                */
/*                                                                            */
/* The plots go through plot_at and plot_shape_at. Which one a call uses is    */
/* the point: the erase at $654C deliberately does *not* name a shape. It      */
/* reuses whatever s_shape holds, and the mask that shape selects decides      */
/* which pixels get cleared. Tidying that away would change the screen.        */
/* ========================================================================== */

/// Reflect a delta.
///
/// The original is `EOR #$FE`, which swaps $01 and $FF -- a bounce with no
/// compare. Written here as negation, which is the same thing over the only
/// values a delta ever holds and says what it means.
///
/// The two differ for anything but +/-1 (EOR would turn 2 into $FC, negation
/// into $FE), so the equivalence rests on the invariant, and the invariant is
/// checkable: $6635/$6636 are written only by this reflection and by the
/// copy-in from $663D-$6640, which in turn are written only by game_start_life
/// -- with +/-1 -- and the matching copy-back. A closed set. Swapping the two
/// spellings also passes every oracle, which is corroboration rather than
/// proof.
static int8_t reflect(int8_t d) {
  assert(d == 1 || d == -1);
  return (int8_t)-d;
}

/// True if the cell at (\p col, \p row) is occupied, asked of the lo-res
/// occupancy map through the ROM's SCRN.
///
/// Charges nothing: the CMP that follows each SCRN is its own block in the
/// original and the cycles differ per call site, so each caller charges its
/// own. Folding them in here is what broke the first attempt -- 4 cycles
/// missing three times over, and every oracle diverged at once.
static bool cell_taken(uint8_t col, uint8_t row) {
  return rom_scrn(row, col) != 0x00;
}

void bouncer_step(Bouncer *b) {
  unsigned blocked = 0;

  if (b->row == 0) {
    TICK(6);
    return;
  }
  TICK(1);

  TICK(36);
  uint8_t want_col = (uint8_t)(b->col + b->dx);
  uint8_t want_row = (uint8_t)(b->row + b->dy);

  const bool diagonal_taken = cell_taken(want_col, want_row);
  TICK(4);
  if (diagonal_taken) {
    // Which axis actually stopped it? Ask the two cells either side.
    TICK(14);
    const bool across_taken = cell_taken(want_col, b->row);
    TICK(4);
    if (across_taken) {
      TICK(24);
      want_col = b->col;
      b->dx = reflect(b->dx);
      ++blocked;
    } else {
      TICK(1);
    }

    TICK(14);
    const bool down_taken = cell_taken(b->col, want_row);
    TICK(4);
    if (down_taken) {
      TICK(24);
      want_row = b->row;
      b->dy = reflect(b->dy);
      ++blocked;
    } else {
      TICK(1);
    }

    TICK(6);
    if (blocked == 0) {
      // An inside corner: only the diagonal is blocked, so go back the way
      // it came.
      TICK(36);
      want_col = b->col;
      want_row = b->row;
      b->dx = reflect(b->dx);
      b->dy = reflect(b->dy);
    } else {
      TICK(1);
    }
  } else {
    TICK(1);
  }

  // Erase where it was. Ink 0 is black, and the shape is whatever the caller
  // last left in $00 -- see the header.
  TICK(11);
  rom_setcol(0x00);

  TICK(20);
  plot_at(0x00, (Cell){.col = b->col, .row = b->row});

  TICK(14);
  rom_plot(b->row, b->col);

  TICK(11);
  s_shape = 0x1a;

  if (want_row == 0) {
    // Off the board: not redrawn, and the position is not committed.
    TICK(6);
    return;
  }
  TICK(1);

  TICK(29);
  b->row = want_row;
  b->col = want_col;
  rom_setcol(0x03);

  TICK(6);
  plot_shape_at(0x1a, 0x03, (Cell){.col = b->col, .row = b->row});

  TICK(14);
  rom_plot(b->row, b->col);

  TICK(6);
}

/* --- The scoreboard: $7252-$7266 ------------------------------------------ */
/*
 * Eleven fields, every one of them BCD, and every multi-byte one least
 * significant first. Six are what game_status_panel prints, and the labels it
 * prints beside them are where those six names come from -- see the $72CE
 * header for the layout.
 *
 * They are C variables now. The multi-byte ones stay arrays rather than
 * becoming a single integer, because the game's arithmetic on them is BCD and
 * byte-at-a-time: the carry from one half to the next is the original's own
 * `ADC #$00`, and folding a pair into one number would have to invent a BCD
 * meaning for the whole that the game never uses.
 *
 * Every initialiser is the byte the shipped image holds at that address, and
 * none of them is decoration: a new game seeds itself from s_apples_quota and
 * the status panel is drawn before anything writes s_lives or s_level, so
 * several of these are read before they are written.
 */

/// SCORE, four bytes. game_add_score adds an apple's worth into it;
/// game_promote_high_score copies it over the high score at the end of a game.
static uint8_t s_score[4] = {0x00, 0x00, 0x00, 0x00};

/// HI SCORE, four bytes, the same shape.
static uint8_t s_hi_score[4] = {0x00, 0x00, 0x00, 0x00};

/// APPLES LEFT -- what remains of this round's quota. start_round loads it
/// from s_apples_quota and each apple eaten takes one off; both bytes zero is
/// what ends the round.
static uint8_t s_apples_left[2] = {0x10, 0x00};

/// SNAKES LEFT -- lives. $7691 sets it to 2 for a new game.
static uint8_t s_lives = 0x02;

/// Apples on the playfield *right now* -- not, as this pair was commented
/// before it was named, a countdown to the next one. game_place_apple ends
/// by BCD-incrementing it ($766C) and $7743 decrements it when one is
/// eaten; $77D0 places a replacement exactly when it reaches zero, which is
/// why the field normally holds one apple and the timeout path's three
/// arrive together. Measured on play-hires: it is $01 at every apple eaten,
/// and $77D0 fires and places one every time.
static uint8_t s_apples_afield[2] = {0x00, 0x00};

/// Apples eaten this round. Only the first $11 of them score -- $777B tests
/// the high byte and $7780 compares the low against $11.
static uint8_t s_apples_eaten[2] = {0x00, 0x00};

/// The round's quota, copied into s_apples_left by start_round. $10 for a
/// fresh level, and the timeout path at $7817 adds three to both.
static uint8_t s_apples_quota[2] = {0x10, 0x00};

/// LEVEL, one BCD byte.
static uint8_t s_level = 0x01;

/// What s_life_timer is loaded with when a life begins. Comes from s_level_time,
/// the level's own allowance.
///
/// $77BC sets it to $FF once the round's last apple is gone, which stops the
/// timer rather than lengthening it: s_life_timer counts down one per pace,
/// but game_draw_side_walls reads any value with bit 7 set as out of range,
/// clamps the wall height and writes $FF back ($6B55), so the count never
/// reaches zero and the run to the gate is untimed. Decoded from $641C and
/// $6B55, not observed -- all three recordings seed it $64 and never reach
/// the clamp.
static uint8_t s_life_time = 0x64;

/* --- The settings block: $0300-$0305 -------------------------------------- */
/*
 * Six bytes that outlive a life, set once at $376E and then kept up to date as
 * the game goes on. Everything else about a life is torn down and rebuilt.
 * game_cold_start writes all six before anything reads them, so unlike the
 * scoreboard these need no initialisers.
 */

/// How long the pace loop dawdles between steps, i.e. the snake's speed.
/// $52 at startup and at the top of every round; $772E takes two off at the
/// start of each life and stops at 3, so the snake speeds up as lives are
/// lost and never goes faster than that.
static uint8_t s_step_delay;

/// 0..2, chosen at the setup prompt. Indexes the per-apple value table at
/// $71C8, and decides how many bouncers step per pass.
static uint8_t s_difficulty;

/// The game is playing itself, because nobody answered the difficulty
/// prompt before it timed out. Any input at all clears it, and while it is
/// set the death pause does not wait to be told to carry on.
static bool s_demo_mode;

/// Which of the 29 display lists at $8000 this level draws, 1-based.
/// select_script skips that many '*'-terminated scripts to find it.
static uint8_t s_script_index;

/// The level's time allowance, which seeds s_life_time at the start of every
/// life. Set by the display list's 'T' command (see run_script), and $64 until
/// one says otherwise. It is *not* the apple value, which is s_apple_value and
/// is computed by game_set_apple_value from the difficulty and the level
/// number; that routine never reads this byte.
static uint8_t s_level_time;

/// The head moved this step, so the next draw merges the head shape over
/// the cell. game_mark_head raises it, game_draw_head reads it and clears it.
static bool s_head_moved;

/* --- The three zero-page pointers: $000A-$000F ---------------------------- */
/*
 * Pointer pairs in the original, low byte first, and 16-bit variables here.
 * Their byte-at-a-time increments -- bump the low byte, and if it wrapped bump
 * the high one -- are what a 6502 writes for ++p, so that is what they are;
 * the branch survives only because the two arms cost different cycles.
 *
 * The initialisers are what the machine holds at $3750, out of
 * entry-state-inc.h rather than out of the game image: this is zero page, and
 * the game never loaded over it.
 */

/// Into the current display list. game_next_byte reads through it and bumps
/// it; select_script points it at the right script first.
static uint16_t s_script_ptr = 0x994c;

/// Into the string that follows a JSR to game_print_inline_str -- which is
/// where the printer finds it, by reading the return address off the stack.
static uint16_t s_str_ptr = 0xffe1;

/// game_rand_byte's cursor. It is not a generator: it walks $1800-$1FFF and
/// returns the first byte it finds with bit 7 clear, so the "random" numbers
/// are the game's own level data, relocated there by the cold start.
/// game_setup_screen clamps the high byte into [$18,$1F) on the way in, which
/// is what keeps it inside that window.
static uint16_t s_rand_ptr = 0x6b00;

/// $73D7 -- the setup screen has run once. The first time through it asks
/// nothing, takes difficulty 1 and demo mode, and only sets this. The shipped
/// image already has it set, so that first pass is skipped on a cold start and
/// the prompt appears immediately.
static bool s_setup_seen = true;

/// VALUE -- what one apple is worth on this level, two BCD bytes.
/// game_set_apple_value recomputes it per level.
static uint8_t s_apple_value[2] = {0x00, 0x00};

/// The bonus screen's own two-byte BCD scratch, and the lives count it
/// compares against to decide whether the round earned a bonus at all.
static uint8_t s_bonus_amount[2] = {0x00, 0x00};
static uint8_t s_lives_at_level_start = 0x02;

/* ========================================================================== */
/* $728D -- the high score                                                    */
/*                                                                            */
/* Four BCD bytes at $7252 against four at $7256, most significant first.      */
/* Below at any byte and it stops; above and it copies; equal and it moves on. */
/*                                                                            */
/* The four compares are a loop here, over the byte indices. In game.c that    */
/* would be a bug -- the site list is built by grepping for literal CYCLES     */
/* addresses, and a computed one silently leaves the trace. This file is       */
/* deliberately outside that grep, so the constraint does not apply and the    */
/* code can be shaped by what it means instead.                                */
/* ========================================================================== */

/// Most significant first, which is the order the compare runs in.
static const uint8_t kMsbFirst[4] = {3, 2, 1, 0};
static const uint16_t kCmpBlock[4] = {0x728d, 0x7297, 0x72a1, 0x72ab};
static const uint16_t kBelowEdge[4] = {0x7293, 0x729d, 0x72a7, 0x72b1};
static const uint16_t kEqualBlock[4] = {0x7295, 0x729f, 0x72a9, 0x72b3};

void game_promote_high_score(void) {
  bool beats_it = true;

  for (unsigned i = 0; i < 4; ++i) {
    GAME_CYCLES(kCmpBlock[i], 10);
    const uint8_t mine = s_score[kMsbFirst[i]];
    const uint8_t best = s_hi_score[kMsbFirst[i]];
    if (mine < best) {
      GAME_CYCLES(kBelowEdge[i], 1);
      beats_it = false;
      break;
    }
    GAME_CYCLES(kEqualBlock[i], 2);
    if (mine != best) {
      GAME_CYCLES(kEqualBlock[i], 1);
      break;
    }
    // Equal: fall through to the next byte. All four equal reaches the copy
    // below, which is a harmless self-assignment.
  }

  if (beats_it) {
    TICK(32);
    for (unsigned i = 0; i < 4; ++i)
      s_hi_score[i] = s_score[i];
  }

  TICK(6);
}

/* ========================================================================== */
/* $69C3 -- find an apple                                                     */
/*                                                                            */
/* Sweep whole columns looking for $0F on the occupancy map: from the snake's  */
/* own column leftwards, then from it again rightwards. First hit wins, so the */
/* result leans left. Nothing found parks the answer at row 0, column $14.     */
/*                                                                            */
/* $6B39/$6B3A were the cursor and $6B3B/$6B3C the answer. The cursor is a    */
/* pair of locals here: nothing ever read the two bytes back, so once the      */
/* storage left RAM the stores that mirrored them were dead code.              */
/* ========================================================================== */

/// The lo-res occupancy map's value at \p c. $0F is an apple.
static uint8_t cell_at(Cell c) {
  return rom_scrn(c.row, c.col);
}

void game_find_nearest_apple(void) {
  static const uint8_t kApple = 0x0f;
  static const uint8_t kLastRow = 0x27;

  Cell c = {.col = s_head.col, .row = 1};
  bool found = false;

  TICK(14);
  for (;;) { // leftwards
    TICK(14);
    const uint8_t v = cell_at(c);
    TICK(4);
    if (v == kApple) {
      TICK(1);
      found = true;
      break;
    }
    TICK(14);
    if (++c.row != kLastRow) {
      TICK(1);
      continue;
    }
    TICK(14);
    c.row = 1;
    if (--c.col == 0)
      break;
    TICK(1);
  }

  if (!found) {
    TICK(8);
    c.col = s_head.col;

    for (;;) { // rightwards
      TICK(14);
      const uint8_t v = cell_at(c);
      TICK(2);
      TICK(2);
      if (v == kApple) {
        TICK(1);
        break;
      }
      TICK(14);
      if (++c.row != kLastRow) {
        TICK(1);
        continue;
      }
      TICK(20);
      c.row = 1;
      if (++c.col == kLastRow) {
        TICK(12);
        c.row = 0;
        c.col = 0x14;
        break;
      }
      TICK(1);
    }
  }

  TICK(22);
  s_apple = c;
}

/* ========================================================================== */
/* $6AB8 -- is a move safe?                                                   */
/*                                                                            */
/* The four neighbour probes are the same shape four times over, so they are  */
/* a table here. In the original they are 60 bytes of straight-line code with */
/* the offsets and the block addresses interleaved.                           */
/*                                                                            */
/* The verdict comes back as an enum and the adapter turns it into A and the  */
/* flags, which is the one thing that cannot move into this file while        */
/* generated callers still branch on them.                                    */
/* ========================================================================== */

/// Where each neighbour is, and the four block addresses the original spends
/// on it: the SCRN block, the CMP after it, the not-taken edge, and the
/// increment.
static const struct {
  int8_t dcol, drow;
  uint16_t scrn_block, scrn_cycles, scrn_ret;
  uint16_t cmp_block, edge, inc_block;
} kNeighbour[4] = {
    {+1, 0, 0x6ae8, 12, 0x6aee, 0x6aef, 0x6af1, 0x6af3},
    {-1, 0, 0x6af6, 16, 0x6aff, 0x6b00, 0x6b02, 0x6b04},
    {0, +1, 0x6b07, 18, 0x6b12, 0x6b13, 0x6b15, 0x6b17},
    {0, -1, 0x6b1a, 18, 0x6b25, 0x6b26, 0x6b28, 0x6b2a},
};

MoveVerdict snake_move_verdict(uint8_t dir, uint8_t *cell_out) {
  TICK(42);

  // The head plus this direction's deltas.
  const Cell target = {
      .col = (uint8_t)(kColDelta[dir] + s_head.col),
      .row = (uint8_t)(kRowDelta[dir] + s_head.row),
  };

  const uint8_t cell = cell_at(target);
  *cell_out = cell;

  // Empty or an apple, and nothing else, may be stepped into.
  TICK(4);
  if (cell != 0x00)
    TICK(2);
  else
    TICK(1);
  TICK(2);
  if (cell != 0x00 && cell != 0x0f) {
    TICK(6);
    return MOVE_TARGET_TAKEN;
  }
  TICK(1);

  TICK(12);
  if (target.row == 0) {
    // Row 0 is the top border; there is nothing above it to look at.
    TICK(1);
    TICK(6);
    return MOVE_ROW_ZERO;
  }

  // A target whose four neighbours are all occupied is a dead end: legal to
  // enter, fatal on the move after, so it is refused a step early.
  unsigned free_neighbours = 0;
  for (unsigned i = 0; i < 4; ++i) {
    GAME_CYCLES(kNeighbour[i].scrn_block, kNeighbour[i].scrn_cycles);
    const Cell n = {
        .col = (uint8_t)(target.col + kNeighbour[i].dcol),
        .row = (uint8_t)(target.row + kNeighbour[i].drow),
    };

    const uint8_t v = cell_at(n);
    GAME_CYCLES(kNeighbour[i].cmp_block, 4);
    if (v == 0x00) {
      GAME_CYCLES(kNeighbour[i].inc_block, 6);
      ++free_neighbours;
    } else {
      GAME_CYCLES(kNeighbour[i].edge, 1);
    }
  }

  TICK(6);
  if (free_neighbours) {
    TICK(8);
    return MOVE_OK;
  }
  TICK(1);
  TICK(8);
  return MOVE_DEAD_END;
}

/* ========================================================================== */
/* $7045 -- draw the playfield                                                */
/*                                                                            */
/* 62 blocks in one routine, and six things: set the video mode, spin, wipe    */
/* the occupancy map, open the wall gaps the difficulty calls for, draw the    */
/* border in both representations, and run the level's display list.           */
/*                                                                            */
/* The display list is the reason this one was worth the trace it cost. $8000  */
/* holds 29 scripts, one per level, each ending in '*'; the interpreter skips  */
/* level-1 scripts to find the current one and then dispatches single-letter   */
/* opcodes. As a chain of CMP/BNE with the operand loads interleaved it is 180 */
/* bytes that read as noise.                                                  */
/* ========================================================================== */

/// The display list's opcodes. Single letters, in Apple II ASCII.
typedef enum {
  OP_RESTART = 0x45, ///< 'E' -- back to level 1 and start over. Level 30 only.
  OP_HLINE = 0x48,   ///< 'H' -- ink, column, last column, row
  OP_PLOT = 0x50,    ///< 'P' -- ink, column, row. No script uses it.
  OP_STORE = 0x54,   ///< 'T' -- one byte, into $0304
  OP_VLINE = 0x56,   ///< 'V' -- ink, row, last row, column
  OP_END = 0x2a,     ///< '*' -- end of this level's script
} ScriptOp;

/// $7024 -- the lo-res plot colour. Zero erases, anything else draws.
static void set_ink(uint8_t ink) {
  game_set_ink_native(ink);
}

/// The ROM's HLINE, which takes its right-hand end from $2C.
static void lores_hline(uint8_t row, uint8_t from_col) {
  rom_hline(row, from_col);
}

/// Plot at a cell in a given ink, keeping whatever shape s_shape holds.
///
/// Separate from plot_shape_at because for these callers the shape genuinely
/// is inherited -- see s_shape -- and passing one would be inventing a value
/// the original does not have.
static void plot_at(uint8_t ink, Cell c) {
  game_plot_shape_native(ink, c);
}

/// The three runs, with their ink and both endpoints as arguments instead of
/// as assignments before the call. What they buy is that a caller states the
/// whole run in one place, including the two things it used to inherit from
/// whatever ran before it.
static void plot_hline_at(uint8_t ink, uint8_t col, uint8_t row, uint8_t to_col) {
  game_plot_hline_native(ink, (Cell){.col = col, .row = row}, to_col);
}

static void plot_vline_at(uint8_t ink, uint8_t col, uint8_t row, uint8_t to_row) {
  game_plot_vline_native(ink, (Cell){.col = col, .row = row}, to_row);
}

static void lores_vline_at(uint8_t col, uint8_t row, uint8_t to_row) {
  /*$7000*/ TICK(6);
  // The row it restores was this routine's result in A. Nothing reads it; the
  // adapter that did has gone.
  game_lores_vline_native((Cell){.col = col, .row = row}, to_row);
}

/// The ROM's PLOT.
static void lores_plot(uint8_t row, uint8_t col) {
  rom_plot(row, col);
}

/// $7019 through its adapter: the next display-list byte.
static uint8_t script_byte(void) {
  return game_next_byte_native();
}

/// Graphics, hi-res, page 2, full screen. The reads are the writes.
static void select_hires_page2(void) {
  TICK(32);
  s_tone_period = 0x00;
  io_peek(0xc050);
  io_peek(0xc057);
  io_peek(0xc055);
  io_peek(0xc052);
}

/// A plain three-deep delay, all three counters as arguments.
///
/// \p inner is the innermost, and the original does not initialise it -- it
/// counts down from whatever Y happened to hold. That is not a value the code
/// can state, so it was measured instead: game_clear_hgr_native's page loop is
/// what runs immediately before, and it exits with Y wrapped to zero. Asserted
/// over full runs of both scenarios, where it is zero every time. It is
/// therefore passed as zero, which counts a full 256.
///
/// The other two were the plotter's column and row, borrowed because zero page
/// was the only place to put them; $7056 loaded them and $7061 counted them
/// down. Nothing reads what the loop leaves behind: wipe_occupancy_map
/// overwrites the row on its first line, open_wall_gaps touches neither, and
/// the column is not read until draw_border writes it.
static void spin(uint8_t inner, uint8_t middle, uint8_t outer) {
  for (;;) {
    TICK(4);
    if (--inner) {
      TICK(1);
      continue;
    }
    TICK(7);
    middle = (uint8_t)(middle - 1);
    if (middle) {
      TICK(1);
      continue;
    }
    TICK(7);
    outer = (uint8_t)(outer - 1);
    if (!outer)
      break;
    TICK(1);
  }
}

/// Clear the lo-res occupancy map, one full-width row at a time from the
/// bottom up. Ink 0 is black, so this erases.
static void wipe_occupancy_map(void) {
  // 972 cycles of its own, and a constant: the loop runs from the literal
  // $27 down to zero whatever the game is doing. Measured at every call in
  // both scenarios. The 74,043 cycles the routine takes in total are almost
  // all lores_hline's, which still charges for each of its 40 rows.
  TICK(972);
  uint8_t at = 0x27;
  set_ink(0x00);

  for (;;) {
    s_h2 = 0x27;
    lores_hline(at, 0x00);

    const uint8_t row = (uint8_t)(at - 1);
    at = row;
    // BPL: row 0 is drawn, and the loop ends one step later.
    if (row & 0x80)
      break;
  }
}

/// One gap per bouncer, which is what the difficulty counts.
static void open_wall_gaps(void) {
  TICK(6);
  const uint8_t difficulty = s_difficulty;
  if (!difficulty) {
    TICK(1);
    return;
  }

  TICK(10);
  lores_plot(0x01, 0x01);

  TICK(8);
  if (s_difficulty == 0x01) {
    TICK(1);
    return;
  }
  TICK(10);
  lores_plot(0x01, 0x26);
}

/// The border, in both representations, plus the gap the snake leaves through.
///
/// \p ink is the six sides' colour, which the original inherits from
/// game_draw_playfield rather than setting.
static void draw_border(uint8_t ink) {
  // 148 cycles of its own -- nine straight-line charges, no loop and no
  // branch, so it cannot be anything else. The 73,939 the routine takes in
  // total belong to the seven line drawers it calls, which still charge.
  TICK(148);
  lores_hline(0x00, 0x00); // $2C is still $27 from the wipe

  lores_hline(0x27, 0x00);

  // The four sides, twice: once on the lo-res occupancy map and once in
  // hi-res. Four of these seven used to leave the endpoint out and inherit
  // $27 from the call above; it is written at each of them now.
  lores_vline_at(0x00, 0x00, 0x27);
  lores_vline_at(0x27, 0x00, 0x27);
  plot_hline_at(ink, 0x00, 0x00, 0x27);
  plot_hline_at(ink, 0x00, 0x27, 0x27);
  plot_vline_at(ink, 0x00, 0x00, 0x27);
  plot_vline_at(ink, 0x27, 0x00, 0x27);

  // Ink 3 over columns $12-$16 of the bottom row, on top of the border just
  // laid down: the gap the snake leaves through.
  plot_hline_at(0x03, 0x12, 0x27, 0x16);
}

/// Walk the pointer to the current level's script, skipping one whole script
/// per level below it. DEX first, so level 1 skips nothing.
static void seek_script(void) {
  TICK(14);
  uint8_t left = s_script_index;
  s_script_ptr = 0x8000;

  for (;;) {
    TICK(4);
    if (!--left) {
      TICK(1);
      return;
    }
    for (;;) {
      TICK(6);
      const uint8_t b = script_byte();
      TICK(4);
      if (b == OP_END) {
        TICK(1);
        break;
      }
      TICK(3);
    }
  }
}

void game_draw_playfield_native(void) {
  TICK(6);
  game_clear_hgr_native();
  select_hires_page2();
  spin(0x00, 0x00, 0x04); // the counts $7056 used to store into $02/$03
  wipe_occupancy_map();

  TICK(21);
  s_wndtop = 0x14;
  s_shape = 0x15;
  set_ink(0x0d);

  open_wall_gaps();
  draw_border(0x0d);

restart:
  seek_script();

  for (;;) {
    TICK(6);
    const uint8_t op = script_byte();

    TICK(4);
    if (op == OP_RESTART) {
      TICK(9);
      s_script_index = 0x01;
      goto restart;
    }
    TICK(1);

    TICK(4);
    if (op == OP_HLINE) {
      TICK(6);
      const uint8_t ink = script_byte();
      TICK(9);
      const uint8_t col = script_byte();
      TICK(9);
      const uint8_t last = script_byte();
      TICK(9);
      const uint8_t row = script_byte();
      TICK(12);
      set_ink(ink);

      TICK(18);
      s_h2 = last;
      lores_hline(row, col);

      TICK(6);
      plot_hline_at(ink, col, row, last);
      TICK(3);
      continue;
    }
    TICK(1);

    TICK(4);
    if (op == OP_VLINE) {
      TICK(6);
      const uint8_t ink = script_byte();
      TICK(9);
      const uint8_t row = script_byte();
      TICK(9);
      const uint8_t last = script_byte();
      TICK(9);
      const uint8_t col = script_byte();
      TICK(12);
      set_ink(ink);

      // The lo-res half puts $03 back where it found it, which is what let
      // the original's hi-res half run the same span without restating it.
      // Both spans are stated here.
      TICK(6);
      lores_vline_at(col, row, last);
      TICK(6);
      plot_vline_at(ink, col, row, last);
      TICK(3);
      continue;
    }
    TICK(1);

    TICK(4);
    if (op == OP_PLOT) {
      TICK(6);
      const uint8_t ink = script_byte();
      TICK(9);
      const uint8_t col = script_byte();
      TICK(9);
      const uint8_t row = script_byte();
      TICK(12);
      set_ink(ink);

      TICK(12);
      lores_plot(row, col);
      TICK(6);
      game_plot_shape_native(ink, (Cell){.col = col, .row = row});
      TICK(3);
      continue;
    }
    TICK(1);

    TICK(4);
    if (op == OP_STORE) {
      TICK(6);
      const uint8_t v = script_byte();
      TICK(7);
      s_level_time = v;
      continue;
    }
    TICK(1);

    TICK(4);
    if (op == OP_END) {
      TICK(1);
      TICK(6);
      return;
    }
    // Anything unrecognised is skipped. No script contains one.
    TICK(3);
  }
}

/* ========================================================================== */
/* $60E7, $6B93, $702B -- the hi-res plotter                                  */
/*                                                                            */
/* Hi-res page 1 is a grid of 48 rows, four scanlines tall and one byte wide.  */
/* $6000/$6030 hold each row's base address split into low and high halves,    */
/* and successive scanlines are $400 apart -- which is why walking down a cell */
/* is +4 on the high byte and nothing else.                                    */
/*                                                                            */
/* $04/$05 (the destination pointer), $06 (the pattern index) and $07 (the     */
/* scanline counter) are locals.                                              */
/* ========================================================================== */

/// The address of a cell row's first scanline, from the split table.
static uint16_t cell_row_base(uint8_t row) {
  return kHgrLineBase[row];
}

/// Index into the 128-byte dot table at $6064: 16 inks of 8, four column
/// phases in each of two scanline parities.
static uint8_t dot_index(uint8_t ink, uint8_t scanline, uint8_t col) {
  return (uint8_t)((uint8_t)(((ink << 1) | (scanline & 1)) << 2) | (col & 3));
}

/// $60E7 -- draw the loaded shape into one cell, replacing what was there.
void game_draw_cell_native(uint8_t ink, Cell c) {
  // 343 cycles, constant: four scanlines every time, no data-dependent
  // branch. Measured at all 18,739 calls across both scenarios, where it is
  // this number without exception. The busiest routine in the game -- it is
  // 3.1M of the 22M a play run spends -- so it is also the one where the
  // charge-per-line bookkeeping cost the most reading.
  /*$60E7*/ TICK(343);
  uint16_t dest = cell_row_base(c.row);

  for (unsigned line = 0; line < 4; ++line) {
    // Built in $06 in two steps, and written out between them because it is
    // zero page and a probe may sample there.
    const uint8_t idx = dot_index(ink, (uint8_t)line, c.col);

    poke(dest + c.col, (uint8_t)(kHgrPattern[idx] & s_shape_mask[line]));
    dest += 0x0400; // one scanline down, i.e. +4 on the high byte
  }
  // The carry is what the loop's CPX #4 leaves.
}

/// $6B93 -- the same cell, merged instead of replaced: only bits are set, and
/// the pattern is inverted first. $7F and not $FF because bit 7 is the byte's
/// hi-res palette bit and flipping it would shift the whole byte's colour.
///
/// It also builds the index differently -- ROR where $60F7 has ROL, so the
/// scanline parity lands in bit 7 and the two following ASLs shift it out.
/// The index degenerates to (ink >> 1) * 4 + (col & 3). Changing it to match
/// $60F7 fails the screen check, so whatever the author meant, it is load
/// bearing.
void game_merge_cell_native(uint8_t ink, Cell c) {
  // 371 cycles, constant, for the same reason as $60E7: four scanlines and
  // no branch. Measured at every call.
  TICK(371);
  uint16_t dest = cell_row_base(c.row);

  for (unsigned line = 0; line < 4; ++line) {
    const uint8_t parity = (uint8_t)(line & 1);
    const uint8_t idx =
        (uint8_t)((uint8_t)(((uint8_t)((parity << 7) | (ink >> 1))) << 2) | (c.col & 3));

    const uint16_t at = dest + c.col;
    poke(at,
         (uint8_t)(((kHgrPattern[idx] ^ 0x7f) & s_shape_mask[line]) | peek(at)));
    dest += 0x0400;
  }
}

/// $702B -- zero hi-res page 1, $2000 through $3FFF. The inner loop runs a
/// full 256 bytes because Y wraps, so the terminating test is on the page.
///
/// The whole routine costs one charge, not 8,192. 106,897 cycles is the sum of
/// what the 6502 spent here, measured at every call in both scenarios, where
/// it is that number every time -- there is no data dependence, the loop
/// always clears the same 32 pages. It is 6.3 frames, 105 ms, and what it
/// buys the viewer is a hi-res page wiping downward. That is the one thing
/// this charge does *not* preserve: the clearing is now instantaneous and the
/// six frames the host draws through the charge already show it finished.
/// Agreed in the design -- the fills are not worth preserving -- and the
/// duration is, because $7056 follows immediately with a 1.29 s hold and the
/// two read as one pause.
///
/// The charge sits at the top so it covers the region up to the next one,
/// which is select_hires_page2's. Every TICK is a yield point, so collapsing
/// them moves where the game can be parked; here that is safe because nothing
/// in the loop reads input or the clock.
void game_clear_hgr_native(void) {
  TICK(106897);

  for (uint8_t page = 0x20;;) {
    uint8_t y = 0;
    do {
      poke((uint16_t)(page << 8) + y, 0x00);
      ++y;
    } while (y);

    ++page;
    if (page == 0x40)
      break;
  }
}

/* ========================================================================== */
/* $6148, $615A, $7000 -- runs of cells                                       */
/*                                                                            */
/* Each loads the shape once and repeats a draw along one axis until the       */
/* moving coordinate reaches $08. The end is tested after drawing, so it is    */
/* inclusive and a degenerate run still plots one cell. A start past the end   */
/* wraps through 255; nothing guards against it and nothing needs to.          */
/* ========================================================================== */

/// $6148 -- a horizontal run of hi-res cells, from \p c along row c.row to
/// \p to_col. The original walked $02/$03 and left $02 on the endpoint, which
/// its adapter returned in A; that is the return value here.
void game_plot_hline_native(uint8_t ink, Cell c, uint8_t to_col) {
  /*$6148*/ TICK(6);
  // Loads the four scanline masks the cell drawers read. The mask it
  // returns was the original\'s result in A and nothing reads it.
  game_load_shape_masks(s_shape);
  for (;;) {
    TICK(6);
    game_draw_cell_native(ink, c);

    TICK(8);
    if (c.col == to_col)
      break;

    TICK(8);
    c.col = (uint8_t)(c.col + 1);
  }
  TICK(1);
  TICK(6);
}

/// $615A -- the same down a column: rows $03 through $08 in column $02.
void game_plot_vline_native(uint8_t ink, Cell c, uint8_t to_row) {
  /*$615A*/ TICK(6);
  // Loads the four scanline masks the cell drawers read. The mask it
  // returns was the original\'s result in A and nothing reads it.
  game_load_shape_masks(s_shape);
  for (;;) {
    TICK(6);
    game_draw_cell_native(ink, c);

    TICK(8);
    if (c.row == to_row)
      break;

    TICK(8);
    c.row = (uint8_t)(c.row + 1);
  }
  TICK(1);
  TICK(6);
}

/// $7000 -- the lo-res half of a vertical run. Unlike the hi-res one it puts
/// $03 back where it found it, because the caller draws the hi-res run over
/// the same coordinates next.
void game_lores_vline_native(Cell c, uint8_t to_row) {
  // The original saves the starting row on the stack, because the hi-res half
  // of a display list's 'V' runs the same span next and the loop below walks
  // c.row to the end of it. Every caller states both ends now.

  for (;;) {
    TICK(12);
    lores_plot(c.row, c.col);

    TICK(8);
    if (c.row == to_row)
      break;

    TICK(8);
    c.row = (uint8_t)(c.row + 1);
  }
  TICK(1);
  TICK(13);
}

/* ========================================================================== */
/* $6594 -- step the bouncers, then take a key                                */
/* ========================================================================== */

/// Move one bouncer. The original copies it into the parameter block at
/// $6633-$6636, calls $64C8, and copies it back out; those eight ram_pokes
/// were what a struct copy looks like without structs, and the block is the
/// argument now.
static void step_bouncer_slot(int slot, uint16_t block, uint16_t cycles,
                              uint16_t back_block, uint16_t back_cycles) {
  GAME_CYCLES(block, cycles);
  Bouncer b = s_bouncers[slot];
  game_move_bouncer(&b);

  GAME_CYCLES(back_block, back_cycles);
  s_bouncers[slot] = b;
}

/// $6200 -- take the next key out of the ring buffer game_read_key fills.
/// Only a byte with bit 7 set counts; the slot is cleared and the read index
/// advances. Returns what the original leaves in A.
///
/// $6216, the RTS all paths share, is charged by the adapter and not here.
/// game_read_key also ends there and is not converted, so $6216 is still a
/// probe site -- and converting one of a shared block's two paths, while the
/// other still reports, is exactly how the two engines stop agreeing. The
/// trace caught it; the pinned site count could not, because the count was
/// right.
static uint8_t dequeue_key(void) {
  TICK(10);
  const uint8_t at = s_ring_read;
  const uint8_t key = s_key_ring[at];
  if (!(key & 0x80)) {
    TICK(1);
    return key;
  }

  TICK(24);
  s_key_ring[at] = 0x00;
  s_ring_read = (uint8_t)((at + 1) & 0x0f);
  // X *is* live out of $6594 -- `apple2tc --ir` says so -- unlike X out of
  // $6C72, where the same check let the write go. So it is maintained.
  return key;
}

/// $6594 -- step as many bouncers as the difficulty calls for, then fall into
/// the key dequeue whose byte is the return value.
uint8_t game_step_bouncers_native(void) {
  TICK(6);
  const uint8_t difficulty = s_difficulty;

  if (!difficulty) {
    TICK(3);
    return dequeue_key();
  }
  TICK(1);

  step_bouncer_slot(0, 0x659c, 38, 0x65b7, 40);

  if (s_difficulty == 0x01) {
    TICK(3);
    return dequeue_key();
  }
  TICK(1);

  step_bouncer_slot(1, 0x65d9, 38, 0x65f4, 35);
  return dequeue_key();
}

/* ========================================================================== */
/* $6C72 -- turn the next input into a direction                              */
/* ========================================================================== */

/// The six inputs the game understands. $6C63 holds the key that produces
/// each one and $6C6A the code the game acts on; the two start out identical,
/// which is why the substitution is invisible until the player rebinds a key
/// from the title screen. Both tables are part of the loaded image, and the
/// first is written at $757C, from the redefinition screen.
///
/// The substitution went unchecked for a while, and coverage could not say so.
/// Both blocks that perform it run constantly -- play.pkeys presses I, J, K
/// and M all through the round -- but those are the *default* bindings, where
/// the two tables hold identical bytes and reading the wrong one is invisible.
/// play-hires.pkeys does rebind, and then the recording ends without playing.
/// Swapping input_code for input_key passed verify.sh 4/4, all three traces,
/// memory and screen.
///
/// play-rebind.pkeys closes it: bound to W A S Z Q E and then played with
/// them, so the tables differ across all 8 substitutions it performs. The same
/// swap now fails trace-ext.
enum { kInputCount = 6 };

static uint8_t input_key(int i) {
  return s_key_table[i];
}

static uint8_t input_code(int i) {
  return kKeyDefaults[i];
}

/// The two codes that are settings rather than directions.
enum { kCodeJoystickOn = 0x80, kCodeJoystickOff = 0x8b };

/// The code every input collapses to in attract mode. Its caller stores $FF
/// at $6253, which ends the game in progress.
enum { kCodeStop = 0x92 };

/// $0302 -- attract mode: nobody answered the difficulty prompt before it
/// timed out, so the game is playing itself. Any input at all ends it, which
/// is why the whole key table is skipped below.
static bool attract_mode(void) {
  return s_demo_mode;
}

/// A switch input. Every one of the game's three read sites -- here, the
/// difficulty prompt at $7428, and the pause check at $788B -- takes bit 7
/// *clear* to mean active, which is the opposite of a stock Apple II paddle
/// button. Consistent enough to be the convention of whatever switch box the
/// game was written for, so it is transcribed and not corrected.
static bool switch_pressed(uint16_t sw) {
  return !(io_peek(sw) & 0x80);
}

uint8_t game_read_direction_native(uint8_t key) {
  TICK(9);

  if (attract_mode()) {
    TICK(6);
    if (s_joystick_selected) {
      TICK(6);
      if (switch_pressed(0xc061)) {
        TICK(12);
        return kCodeStop;
      }
      TICK(1);
    } else {
      TICK(1);
    }

    TICK(6);
    if (key & 0x80) {
      TICK(8);
      return kCodeStop;
    }
    TICK(1);
    TICK(6);
    // Not a keypress, so nothing happened -- $00 out of an empty ring.
    return key;
  }
  TICK(1);

  // Search the bindings from the last slot down, so that if the player has
  // bound the same key twice the higher slot wins.
  TICK(6);
  uint8_t code = key;
  int slot = kInputCount - 1;
  for (;;) {
    TICK(6);
    if (key == input_key(slot)) {
      TICK(1);
      TICK(4);
      code = input_code(slot);
      break;
    }

    TICK(4);
    if (--slot < 0) {
      TICK(3);
      break;
    }
    TICK(1);
  }
  // X is not written back. The original leaves it on the matching slot, or
  // $FF, but nothing reads it: `apple2tc --ir` prints per-function register
  // liveness, and func_6c72's LiveOut is A, Y and the flags. Y is in that set,
  // so the joystick block below does maintain it.

  TICK(4);
  if (code == kCodeJoystickOn) {
    TICK(12);
    s_joystick_selected = true;
    return 0x01;
  }
  TICK(1);

  TICK(4);
  if (code == kCodeJoystickOff) {
    TICK(12);
    s_joystick_selected = false;
    return 0x00;
  }
  TICK(1);

  TICK(4);
  if (code & 0x80) {
    // A direction. Hand it straight back.
    TICK(6);
    return code;
  }
  TICK(1);

  TICK(6);
  const bool joystick = s_joystick_selected;
  if (!joystick) {
    TICK(8);
    return code;
  }
  TICK(1);

  // The joystick is two switch inputs read twice, with annunciator 2
  // selecting the pair -- four directions on two pins. Exactly one has to be
  // active: none or several is ambiguous and rejected.
  TICK(12);
  int pressed = 0;
  uint8_t chosen = 0; // only read when exactly one input turned out active
  io_peek(0xc05b); // annunciator 2 on
  if (switch_pressed(0xc062)) {
    TICK(4);
    chosen = 0;
    ++pressed;
  } else {
    TICK(1);
  }

  TICK(6);
  if (switch_pressed(0xc063)) {
    TICK(4);
    chosen = 3;
    ++pressed;
  } else {
    TICK(1);
  }

  TICK(10);
  io_peek(0xc05a); // annunciator 2 off
  if (switch_pressed(0xc062)) {
    TICK(4);
    chosen = 1;
    ++pressed;
  } else {
    TICK(1);
  }

  TICK(6);
  if (switch_pressed(0xc063)) {
    TICK(4);
    chosen = 2;
    ++pressed;
  } else {
    TICK(1);
  }
  // Y, unlike X, is live out of here -- see above.

  TICK(4);
  if (pressed != 1) {
    TICK(8);
    return 0x00;
  }
  TICK(1);
  TICK(10);
  return input_code(chosen);
}

/* ========================================================================== */
/* $75D1 -- read a replacement key for one slot                               */
/* ========================================================================== */

/// Where slot \p slot sits on the redefinition screen, and what blinks there.
///
/// $75CB is not the slot's current binding, which is the obvious reading. Its
/// six bytes are $E2 $E4 $E5 $E3 $E4 $E5, and the repeats settle it: six
/// distinct keys cannot have four distinct glyphs. It is the *direction* the
/// slot stands for -- up, left, right, down, left, right -- drawn through the
/// game's own hi-res COUT, which is why the player knows which key is being
/// asked for.
static uint8_t slot_col(int slot) {
  return kArrowCH[slot];
}

static uint8_t slot_row(int slot) {
  return kArrowCV[slot];
}

static uint8_t slot_glyph(int slot) {
  return kArrowGlyph[slot];
}

/// $75D1 -- the dark half: erase the glyph and wait, polling nothing.
static void edit_key_blank(uint8_t slot) {
  // The original parks the slot at $0002 for the whole routine, because COUT
  // clobbers X and every step below needs it again. Here it is the parameter,
  // and the glyph blitter no longer has anything to clobber it with.
  TICK(23);
  s_ch = slot_col(slot);
  s_cv = slot_row(slot);
  rom_fc68();

  TICK(11);
  rom_cout(0xa0);

  // Both halves of the blink count X down to zero 256 times, and the X they
  // start from is whatever COUT left behind -- the original never initialises
  // it. What COUT leaves is what its caller put there, which is the slot: the
  // ROM's COUT1 does not touch X at all, and the hi-res hook saves and
  // restores it. So the first pass of the delay is `slot` long and the other
  // 255 are a full 256. Transcribed rather than tidied: it is the delay's
  // actual duration.
  TICK(2);
  uint8_t x = slot;
  uint8_t y = 0;
  for (;;) {
    TICK(4);
    if (--x) {
      TICK(1);
      continue;
    }
    // $75EC is `LDA #$41 / BEQ`, a branch that cannot be taken and a value
    // nothing reads. Four cycles of the delay and nothing else.
    TICK(4);
    TICK(4);
    if (!--y)
      break;
    TICK(1);
  }
}

/// $75F3 -- the lit half: draw the glyph and wait, reading the keyboard each
/// time the inner counter wraps. Returns the accepted key, or 0 if the wait
/// ran out or the key was rejected -- either way the blink starts again, and
/// no acceptable key is 0.
static uint8_t edit_key_prompt(uint8_t slot) {
  TICK(23);
  s_ch = slot_col(slot);
  s_cv = slot_row(slot);
  rom_fc68();

  TICK(13);
  const uint8_t glyph = slot_glyph(slot);
  rom_cout(glyph);

  // The slot again -- see edit_key_blank for why COUT leaves it in X.
  TICK(2);
  uint8_t x = slot;
  uint8_t y = 0;
  for (;;) {
    TICK(4);
    if (--x) {
      TICK(1);
      continue;
    }

    GAME_CYCLES_COORD(0x760f, 6);
    const uint8_t key = io_peek(0xc000);
    if (key & 0x80) {
      TICK(1);
      TICK(8);
      io_poke(0xc010, key); // clear the strobe

      // Anything from $A1 up -- every printable key -- plus the two arrows.
      // The carry the three compares leave is not written back: the caller's
      // next act on it is $7582's `CPX #$06`, which sets it.
      if (key >= 0xa1) {
        TICK(1);
        return key;
      }
      TICK(4);
      if (key == 0x88) { // left arrow
        TICK(1);
        return key;
      }
      TICK(4);
      if (key == 0x95) { // right arrow
        TICK(1);
        return key;
      }
      TICK(6);
      return 0;
    }

    TICK(4);
    if (--y) {
      TICK(1);
      continue;
    }
    TICK(6);
    return 0;
  }
}

uint8_t game_edit_key_native(uint8_t slot) {
  uint8_t key;
  do {
    edit_key_blank(slot);
    key = edit_key_prompt(slot);
  } while (!key);

  TICK(9);
  return key;
}

/* ========================================================================== */
/* $6BFB -- the falling tone                                                  */
/* ========================================================================== */

/* The whole tone is four bytes, declared up with the rest of the state.       */
/* s_tone_period doubles as the on/off switch: game_mark_head raises it to 1   */
/* when the head moves and game_draw_playfield clears it, so the sound follows */
/* the snake and stops with it. s_click_port is where the click goes, as the   */
/* low byte of the soft switch: $C030 is the speaker and $C020 the cassette    */
/* output, which nobody can hear -- so muting is a store rather than a branch, */
/* and the click itself is one indexed read. See the $7642 header for why that */
/* shape was chosen.                                                          */

void game_tick_sound_native(void) {
  TICK(6);
  s_tone_passes = 0x14; // twenty

  for (;;) {
    TICK(6);
    const uint8_t period = s_tone_period;
    if (period) {
      TICK(4);
      if (period < 0x80) {
        TICK(8);
        const uint8_t left = (uint8_t)(s_tone_countdown - 1);
        s_tone_countdown = left;
        if (!left) {
          TICK(28);
          const uint8_t port = s_click_port;
          speaker_access(port);

          // Two INC $6C46: every click lengthens the period, so the pitch
          // falls for as long as the head keeps moving.
          s_tone_period = (uint8_t)(s_tone_period + 2);
          s_tone_countdown = s_tone_period;
        } else {
          TICK(1);
        }
      } else {
        TICK(1);
      }
    } else {
      TICK(1);
    }

    TICK(8);
    if (s_tone_period >= 0x80) {
      // Fallen off the bottom of the range: silence until something restarts
      // it. $80 is reached from below in steps of two, so this is the end of
      // one slide rather than a wrap.
      TICK(6);
      s_tone_period = 0x00;
    } else {
      TICK(1);
    }

    // Chosen afresh every pass, and defaulting to inaudible.
    TICK(12);
    s_click_port = 0x20;
    if (!attract_mode()) {
      TICK(6);
      if (!s_sound_muted) {
        TICK(6);
        s_click_port = 0x30;
      } else {
        TICK(1);
      }
    } else {
      TICK(1);
    }

    TICK(8);
    const uint8_t left = (uint8_t)(s_tone_passes - 1);
    s_tone_passes = left;
    if (!left)
      break;
    TICK(1);
  }
  TICK(6);
}

/* ========================================================================== */
/* $71F3 -- a BCD byte, with leading zeros suppressed                         */
/* ========================================================================== */

/// $002C -- the significance flag, holding the last significant digit rather
/// than a plain 1. Its caller clears it before the first byte of a number, so
/// leading zeros print nothing and interior ones print; $7226 consults it
/// after the last byte, and prints a single "0" if the whole number was.
static bool digit_seen(void) {
  return s_h2 != 0;
}

static void note_digit(uint8_t digit) {
  s_h2 = digit;
}

enum { kCharZero = 0xb0 };

/// `CLC / ADC #$B0`, which is what turns 0-9 into the character for it.
///
/// The original's ADC honours the D flag; this addition does not. Digits are
/// printed with D clear -- $7267 is the only thing in the game that sets it,
/// and it clears it again before returning -- so say so loudly rather than
/// carry a decimal path that cannot be reached.
static void cout_digit(uint8_t digit) {
  rom_cout((uint8_t)(kCharZero + digit));
}

void game_print_bcd_native(uint8_t byte) {
  const uint8_t high = (uint8_t)(byte >> 4);

  TICK(15);
  if (!high) {
    TICK(1);
  } else {
    TICK(3);
    note_digit(high);
  }

  TICK(8);
  if (digit_seen()) {
    TICK(1);
    TICK(14);
    cout_digit(high);
  } else {
    // A leading zero: dropped, and nothing is printed.
    TICK(7);
  }

  const uint8_t low = (uint8_t)(byte & 0x0f);

  TICK(10);
  if (!low) {
    TICK(1);
  } else {
    TICK(3);
    note_digit(low);
  }

  TICK(8);
  if (digit_seen()) {
    TICK(1);
    TICK(14);
    cout_digit(low);
    TICK(6);
  } else {
    TICK(10);
  }
}

/// $7226 -- called after the last byte of a number: if nothing significant was
/// printed, the number was zero, and one "0" is printed for the whole of it.
void game_print_zero_if_blank_native(void) {
  TICK(5);
  if (digit_seen()) {
    TICK(1);
    TICK(6);
    return;
  }

  TICK(5);
  rom_cout(kCharZero); // JMP $FDED -- a tail call, so no return address.
}

/* ========================================================================== */
/* $7267 -- add to the score                                                  */
/* ========================================================================== */

void game_add_score_native(void) {
  TICK(56);

  // Decimal mode for the whole run, and adc_dec16 rather than a second
  // hand-written BCD adder: it is the one the emulator and the generated code
  // both use, so it cannot disagree with them about the undefined corners of
  // BCD ADC. It returns the sum in the low byte and the flags in the high one.

  // Four bytes at $7252, least significant first, plus a two-byte value at
  // $71CB. The original adds the value into the low half and then propagates
  // the carry through the top half with `ADC #$00`, which flips the operand
  // order halfway -- kept, because adc_dec16 need not be symmetric over BCD
  // that is not valid BCD.
  unsigned carry = 0;
  uint8_t flags = 0;
  for (int i = 0; i < 4; ++i) {
    const uint8_t a = i < 2 ? s_apple_value[i] : s_score[i];
    const uint8_t m = i < 2 ? s_score[i] : 0x00;
    const uint16_t r = adc_dec16(a, m, carry);
    s_score[i] = (uint8_t)r;
    flags = (uint8_t)(r >> 8);
    carry = flags & 0x01;
  }

  // Only the `easy` fixture checks the carry. Breaking the propagation between
  // bytes -- so the score never carries past $99 -- passes verify.sh 4/4, both
  // 1300-frame traces, memory and screen, and fails only the 3000-frame run
  // against snake-byte-easy.b33. Neither committed recording ever scores
  // enough to cross a byte boundary.
  //
  // D is the only flag live out of $7267 that survives here, and the
  // original's CLD is what makes it false. `apple2tc --ir` also called C and V
  // live out; that was true of the generated program, whose caller read them.
  // Nothing does now.
}

/* ========================================================================== */
/* $7024, $7019 -- two small ones                                             */
/* ========================================================================== */

/// $7024 -- tell the ROM's lo-res plotter which colour to draw the occupancy
/// map in. Zero erases, anything else draws, so the map only ever holds colour
/// 0 or colour 5: it is a two-state map, not a picture.
///
/// The original takes its argument in the Z flag rather than in A, because
/// every caller reaches it with `LDA $01 / JSR $7024` and $01 is the same ink
/// byte the hi-res plotter takes. Here it is the byte, and the adapter asserts
/// that the flag agreed with it.
void game_set_ink_native(uint8_t ink) {
  TICK(2);
  if (!ink) {
    TICK(1);
  } else {
    TICK(2);
  }

  TICK(3);
  rom_setcol(ink ? 0x05 : 0x00); // JMP $F864 -- a tail call.
}

/// $7019 -- read the byte the $000A pointer addresses and advance it. The
/// display-list interpreter's only way of reading its script.
uint8_t game_next_byte_native(void) {
  TICK(14);
  const uint8_t b = peek(s_script_ptr);

  ++s_script_ptr;
  if (s_script_ptr & 0xff) {
    TICK(1);
  } else {
    // The low byte wrapped, so the original had to bump the high one too.
    TICK(5);
  }
  TICK(6);
  return b;
}

/* ========================================================================== */
/* $6C4B, $7642, $71CD -- apples                                              */
/* ========================================================================== */

/// $6C4B -- the game's random number. A pointer at $000E walks memory from
/// $1800 upward and the first byte with bit 7 clear is the answer; the pointer
/// is reset to $1800 whenever it finds one that is not. Not random, but
/// unpredictable enough to place an apple and it costs nothing to keep.
///
/// There is no generator at all: program text and data are the entropy. The
/// result is therefore always $00-$7F, which is what makes it usable directly
/// as a coordinate.
///
/// The restart does not advance the pointer -- it stores $1800 and jumps
/// straight back to the load -- so a byte at $1800 with bit 7 set would hang
/// the game. Nothing enforces that; the original simply relies on it.
uint8_t game_rand_byte_native(void) {
  TICK(7);
  ++s_rand_ptr;
  if (s_rand_ptr & 0xff) {
    TICK(1);
  } else {
    TICK(5);
  }

  for (;;) {
    TICK(9);
    const uint8_t b = peek(s_rand_ptr);
    if (!(b & 0x80)) {
      TICK(1);
      TICK(6);
      return b;
    }

    TICK(13);
    s_rand_ptr = 0x1800;
  }
}

/// Add one, in BCD, to the two-byte counter at \p at.
static void bcd_inc16(uint8_t at[2]) {
  uint16_t r = adc_dec16(at[0], 0x01, 0x00);
  at[0] = (uint8_t)r;

  r = adc_dec16(at[1], 0x00, (uint8_t)(r >> 8) & 0x01);
  at[1] = (uint8_t)r;
}

Cell game_place_apple_native(void) {
  // Rejection sampling: two pseudo-random bytes as column and row, ask the
  // lo-res map whether that cell is free, and start over if it is not.
  // game_rand_byte returns $00-$7F while the field is 40x40, so most draws
  // land outside it and hit the border or garbage -- the retry loop does far
  // more work than it looks like.
  Cell at = {0, 0};
  for (;;) {
    TICK(6);
    at.col = game_rand_byte_native();

    TICK(9);
    at.row = game_rand_byte_native();

    TICK(15);
    const bool taken = cell_taken(at.col, at.row);

    TICK(2);
    if (!taken)
      break;
    TICK(1);
  }

  // White on the occupancy map, so the snake's collision test sees it.
  TICK(8);
  rom_setcol(0x0f);

  TICK(12);
  rom_plot(at.row, at.col);

  TICK(16);
  plot_shape_at(0x01, 0x09, at);

  // One more apple on screen. $77D0 watches this pair and calls back here when
  // it reaches zero.
  TICK(32);
  bcd_inc16(s_apples_afield);
  return at;
}

/// $71CD -- what one apple is worth: the difficulty's entry in the $71C8 table
/// added to itself once per level, in BCD, into $71CB. X is never touched in
/// the original's loop, which is what makes it the same entry every time.
void game_set_apple_value_native(void) {
  TICK(20);
  s_apple_value[0] = 0x00;
  s_apple_value[1] = 0x00;
  const uint8_t per_apple = kAppleValueTable[s_difficulty];
  uint8_t levels = s_script_index;

  for (;;) {
    TICK(28);
    uint16_t r = adc_dec16(per_apple, s_apple_value[0], 0x00);
    s_apple_value[0] = (uint8_t)r;

    r = adc_dec16(s_apple_value[1], 0x00, (uint8_t)(r >> 8) & 0x01);
    s_apple_value[1] = (uint8_t)r;

    if (!--levels)
      break;
    TICK(1);
  }

  TICK(8);
  // D is the whole of this routine's live-out set now that V has gone.
}

/* ========================================================================== */
/* $6BEF, $6BDA, $7633, $60E4 -- the head and the apple it eats               */
/* ========================================================================== */

/// $6BEF -- mark the head on the lo-res occupancy map, at the row and column
/// the caller has already loaded, and raise the two flags that say it is
/// there: $0305 for the next draw and $6C46 to start the tone.
void game_mark_head_native(uint8_t row, uint8_t col) {
  TICK(6);
  rom_plot(row, col);

  TICK(16);
  s_head_moved = true;
  s_tone_period = 0x01;

  // A and its flags are live out of $6BEF, unlike almost everything else here.
}

/// $6BDA -- draw the cell the caller set up, and if $0305 says the head is on
/// it, merge shape 1 over the top so the head reads as a head rather than
/// replacing the body cell underneath. $0305 is consumed here.
void game_draw_head_native(uint8_t ink, Cell c) {
  TICK(6);
  game_plot_shape_native(ink, c);

  TICK(6);
  if (s_head_moved) {
    TICK(11);
    s_shape = 0x01;
    { // was game_plot_shape_merge()

      /*$6B93*/ TICK(6);
      game_load_shape_masks(s_shape);


      // The high byte it returns was the original's result in A; nothing
      // reads it now.
      game_merge_cell_native(ink, c);

    }
  } else {
    TICK(1);
  }

  TICK(12);
  s_head_moved = false;
  // Only V and D are live out, and neither is touched here.
}

/// $7633 -- an extra snake for clearing the round, and the noise that says so.
///
/// Not "eat an apple", which is what this was called: it BCD-increments $725E,
/// which the status panel prints as SNAKES LEFT, and its one caller is $7803 --
/// reached from $77EA, the round-cleared path, straight after the bonus
/// screen. Nothing about it runs when an apple is eaten; that path is $7743,
/// and it touches four other counters and not this one.
void game_award_extra_life_native(void) {
  TICK(22);
  s_lives = (uint8_t)adc_dec16(s_lives, 0x01, 0x00);
  game_sound_sweep_native();

  TICK(6);
}

/// $60E4 -- load a shape and draw it, which is the pair every caller wants.
void game_plot_shape_native(uint8_t ink, Cell c) {
  TICK(6);
  game_load_shape_masks(s_shape);
  game_draw_cell_native(ink, c); // JMP -- a tail call.
}

/* ========================================================================== */
/* $64A9, $7590, $6B3D                                                        */
/* ========================================================================== */

/// $64A9 -- the noise an apple makes. Two sweeps: X starts at 0, so the first
/// DEX wraps to 255 and the delay between clicks runs 256, 255, ... 1 and the
/// pitch rises; the second counts X up from 0, so the delay runs 256, 1, 2,
/// ... 255 and it falls again.
void game_sound_sweep_native(void) {
  TICK(2);
  uint8_t x = 0x00;

  do {
    TICK(4);
    uint8_t y = x;
    do {
      TICK(4);
      if (--y)
        TICK(1);
    } while (y);

    // The click, at whichever port $6C2C last chose. Neither the Y it loads
    // nor the byte it reads outlives the next pass.
    //
    // Nothing checks the port. Reading $C001+port instead of $C000+port
    // passes every oracle -- correctly, as it happens, since $C020-$C02F all
    // mirror the cassette toggle and $C030-$C03F the speaker. But hardcoding
    // $30 here would pass too, and that would be a real bug: the mute would
    // stop working and no oracle in this repo looks at sound.
    TICK(12);
    speaker_access(s_click_port);
    if (--x)
      TICK(1);
  } while (x);

  do {
    TICK(4);
    uint8_t y = x;
    do {
      TICK(4);
      if (--y)
        TICK(1);
    } while (y);

    TICK(12);
    // The click itself. The read *is* the write -- see the note above.
    speaker_access(s_click_port);
    if (++x)
      TICK(1);
  } while (x);

  TICK(6);
}

/// $7590 -- show \p key as the binding of slot \p slot on the redefinition
/// screen. The two arrow keys have no printable glyph, so they are shown as
/// 'f' and 'g', which is where the arrow shapes live in the game's own font at
/// $66A9.
void game_show_key_native(uint8_t slot, uint8_t key) {
  TICK(7);

  uint8_t glyph = key;
  if (key == 0x88) { // left arrow
    TICK(2);
    glyph = 0xe6;
  } else {
    TICK(1);
  }

  TICK(4);
  if (glyph == 0x95) { // right arrow
    TICK(2);
    glyph = 0xe7;
  } else {
    TICK(1);
  }

  // The original parks the slot in X here because COUT would otherwise be the
  // last thing to touch it. Nothing reads it: the hi-res hook saves and
  // restores X without looking at it, and every use of the slot below is the
  // parameter.
  TICK(23);
  s_ch = kKeyCH[slot];
  s_cv = kKeyCV[slot];
  rom_fc68();

  TICK(10);
  rom_cout(glyph);

  TICK(9);
}

/// $6B3D -- both side walls, each in two segments of different ink, with the
/// seam at a row derived from $6255. The seam is what the player aims for.
uint8_t game_draw_side_walls_native(void) {
  // The random byte is thrown away. The call is not: $6C4B advances the
  // pointer at $000E, so this is what keeps apple placement from repeating
  // level to level.
  TICK(6);
  (void)game_rand_byte_native();

  TICK(26);
  s_shape = 0x15;

  uint8_t seed = s_life_timer;
  if (seed & 0x80) {
    // A negative seed is clamped, and $6255 reset so the next call starts from
    // a known place.
    TICK(8);
    s_life_timer = 0xff;
    seed = 0x70;
  } else {
    TICK(1);
  }

  // How far down the upper segment reaches, from the timer: the walls close in
  // as a life runs out.
  TICK(18);
  const uint8_t wall_top = (uint8_t)((seed >> 2) + 1);
  plot_vline_at(0x02, 0x00, 0x01, wall_top);

  TICK(16);
  plot_vline_at(0x02, 0x27, 0x01, wall_top);

  TICK(30);
  const uint8_t seam = (uint8_t)(wall_top + 1);
  // The lower segment.
  plot_vline_at(0x0d, 0x27, seam, 0x27);

  TICK(18);
  plot_vline_at(0x0d, 0x00, seam, 0x27);

  // Tail call: SCRN of the bottom-centre cell, which is this routine's second
  // result -- the caller uses it to decide whether to draw the gate.
  TICK(7);
  return scrn_cell((Cell){.col = 0x14, .row = 0x27});
}

/* ========================================================================== */
/* $6217 -- the keyboard, into the ring                                       */
/* ========================================================================== */

/// A 16-entry ring at $623C, with $624D the write index and $624C the read
/// index; dequeue_key above is the other end of it.
///
/// If advancing the write index would land on the read index the buffer is
/// full and the key is dropped -- but note it has already been *stored* by
/// then. The byte is written and then disowned by not committing the index,
/// which is a byte of work saved and a slot of the ring left holding a key
/// nobody will read until it is overwritten.
void game_read_key_native(void) {
  // $6217 is on the replay coordinate, and is also where ram.probe and
  // screen.probe take their samples. It keeps its probe for both reasons.
  GAME_CYCLES_COORD(0x6217, 10);
  const uint8_t at = s_ring_write;
  const uint8_t key = io_peek(0xc000);

  if (key & 0x80) {
    TICK(21);
    io_poke(0xc010, key); // clear the strobe
    s_key_ring[at] = key;

    // The $0F is the ring's size and nothing checks it: widening it to $1F
    // passes every oracle, because no recording ever presses sixteen keys
    // faster than the game reads them. Do not tidy it.
    const uint8_t next = (uint8_t)((at + 1) & 0x0f);
    if (next != s_ring_read) {
      TICK(10);
      s_ring_write = next;
      return;
    }
    TICK(1);
  } else {
    TICK(1);
  }

  // The RTS belongs to the routine before this one, and both early exits
  // share it -- as does the key dequeue, whose adapter still emits it.
  TICK(6);
  // A and X are the live-out set, and both are set above on every path.
}

/* ========================================================================== */
/* $664A -- the hi-res COUT hook                                              */
/*                                                                            */
/* Installed by $6641 over the ROM's character-output vector, so that every    */
/* COUT in the game and in the ROM draws through the game's own 8x8 font at    */
/* $66A9 instead of the text screen. Control characters are passed straight    */
/* on to $FDF0, which is what keeps the cursor, scrolling and the rest of      */
/* COUT's behaviour working underneath.                                       */
/*                                                                            */
/* Everything the original parked below $0009 is a local here. It used to have */
/* to stay in emulated RAM, because ram.probe hashed $0000-$00FF and the       */
/* residue had to match the original's down to the loop counter's last value;  */
/* that range left the hash when the storage moved, on 2026-08-23.             */
/* ========================================================================== */

/// Where glyph \p glyph's eight rows live. The font starts at $66A9 and the
/// first glyph in it is the space, $20.
static uint16_t glyph_rows(uint8_t glyph) {
  return (uint16_t)(0x66a9 + (uint16_t)(glyph - 0x20) * 8);
}

/// The hi-res address matching the text cursor. BASL/BASH at $28 point at the
/// text line, CH at $24 is the column; `- 4 + $20` on the high byte is
/// `+ $1C`, which maps $04xx (text page 1) onto $20xx (hi-res page 1).
static uint16_t hires_cursor(void) {
  // Each half separately, and that is not an accident of the split: the low
  // byte wraps at 8 bits without carrying into the high one, so a cursor near
  // the end of a line addresses the start of the same hi-res row.
  const uint8_t hi = (uint8_t)((s_bas >> 8) - 0x04 + 0x20);
  const uint8_t lo = (uint8_t)((s_bas & 0xff) + s_ch);
  return (uint16_t)(lo | (hi << 8));
}

void game_cout_hook_native(uint8_t ch) {
  TICK(9);
  const uint8_t glyph = (uint8_t)(ch & 0x7f);

  if (glyph >= 0x20) {
    TICK(1);
    TICK(82);

    // The original's SBC/ADC pairs honour the D flag. COUT is never reached in
    // decimal mode -- the ROM clears D at reset and neither BASIC nor the game
    // sets it around output -- so rather than carry dead decimal paths, fail
    // loudly if that assumption ever breaks.

    // The original parks all of this in the plotter's zero-page block, which
    // it is not otherwise using. They are locals: the glyph, the caller's X
    // and Y, the source and the destination all die at the closing brace, and
    // nothing outside reads them. See the block's header for why the aliasing
    // had to be shown to be unobservable before they could be split out.
    // The original saves the caller's X and Y here and puts them back at the
    // end, because it uses X as the scanline counter. That counter is the loop
    // variable below, so there is nothing left to save from.
    const uint16_t src = glyph_rows(glyph);
    uint16_t dest = hires_cursor();

    for (unsigned row = 0; row < 8; ++row) {
      TICK(33);
      poke(dest, peek((uint16_t)(src + row)));

      // One hi-res scanline down within the character cell, which is +$400.
      dest = (uint16_t)(dest + 0x0400);

      // `INX / CPX #8 / BNE`: the branch is taken on every pass but the last.
      if (row != 7)
        TICK(1);
    }

    TICK(9);
  }

  TICK(7);
  rom_cout1(ch); // JMP $FDF0 -- the PLA, high bit still on it
}

/* ========================================================================== */
/* $6288 -- one life                                                          */
/*                                                                            */
/* The game's main loop: steer the snake, move it one cell, redraw the head    */
/* and the tail, and pace the whole thing with a delay loop that also ticks    */
/* the sound and polls the keyboard. It returns when the life ends, and the    */
/* five ways that can happen are LifeEnd.                                      */
/*                                                                            */
/* The direction lives at $624E and is 1..4. It is a cycle, so a turn is       */
/* ±1 and a wrap: $6232 and $6237 are the column and row deltas indexed by     */
/* it, and they read right, up, left, down -- decrementing turns clockwise.    */
/*                                                                            */
/* The shape drawn for a step is the direction plus $10, $08 or $04 depending  */
/* on which way the snake turned to get there, which is how the corner pieces  */
/* pick themselves. The original computes it before normalising the direction  */
/* back into range, so it is the *old* direction that selects the shape.       */
/*                                                                            */
/* Steering has two spellings. The arrow keys are relative -- $95 turns        */
/* clockwise, $88 anticlockwise -- and I/J/K/M are absolute, translated into   */
/* whichever turn would achieve them from where the snake is already going by  */
/* the four tables at $6387. Asking for the direction it is already going, or  */
/* for a reversal, yields $00 from those tables and is ignored.                */
/*                                                                            */
/* Nothing here is in emulated RAM any more; the four tables at $6387 are the  */
/* loaded image, which is a different thing.                                  */
/* ========================================================================== */

/// What the player can press, after game_read_direction_native() has had its
/// say. The arrows turn; the letters are the I/J/K/M diamond; $92 quits. The
/// last two are read by $69A9 alone, at the far end of the dispatch chain.
enum {
  KEY_TURN_CW = 0x95,
  KEY_TURN_CCW = 0x88,
  KEY_QUIT = 0x92,
  KEY_UP = 0xc9,
  KEY_LEFT = 0xca,
  KEY_RIGHT = 0xcb,
  KEY_DOWN = 0xcd,
  KEY_CTRL_S = 0x93,
  KEY_ESC = 0x9b,
};

/// $6387/$638C/$6391/$6396 -- turn an absolute-direction key into the relative
/// turn that achieves it from \p dir, or $00 for "nothing to do", which covers
/// both "already going that way" and "that would be a reversal".
static uint8_t turn_for_key(uint8_t key, uint8_t dir) {
  switch (key) {
  case KEY_UP:
    return kTurnForKey[0][dir];
  case KEY_LEFT:
    return kTurnForKey[1][dir];
  case KEY_RIGHT:
    return kTurnForKey[2][dir];
  case KEY_DOWN:
    return kTurnForKey[3][dir];
  default:
    return 0;
  }
}

/// Draw \p shape into \p c with ink \p ink, through the plotter's zero-page
/// argument block.
static void plot_shape_at(uint8_t shape, uint8_t ink, Cell c) {
  s_shape = shape;
  game_plot_shape_native(ink, c);
}

/// SCRN one cell.
static uint8_t scrn_cell(Cell c) {
  return rom_scrn(c.row, c.col);
}

/// $649F -- one click of the speaker. $6C49 holds the port offset, $30 for
/// the speaker and $20 for the cassette output that nobody can hear, which is
/// how muting works; game_sound_sweep does the same thing at $64B0.
///
/// Not a keyboard read, which is what this was called until the scoreboard
/// pass went looking: the address is $C000 + $6C49, and the built-in symbol
/// database resolves the $C000 to KBD, so the disassembly reads `LDA KBD,Y`
/// and the index is what makes it the speaker.
/// Every access to the speaker soft switch goes through here.
///
/// The Apple II speaker is a one-bit cone and reading $C030 flips it, so the
/// waveform is entirely the sequence of these accesses and the cycles they
/// happen on. That makes the sequence an exact oracle for any change to this
/// program's timing: dump it before and after, and it must be identical. It is
/// also the only check the sound has ever had.
///
/// $C020 is the cassette output, which is where a muted click goes.
static void speaker_access(uint8_t port) {
  static FILE *dump;
  static int tried;
  if (!tried) {
    const char *path = getenv("A2_TOGGLE_DUMP");
    tried = 1;
    if (path)
      dump = fopen(path, "w");
  }
  if (dump)
    fprintf(dump, "%u %u\n", (unsigned)s_cycles, (unsigned)port);
  peek((uint16_t)(0xc000 + port));
}

static void click_speaker(void) {
  speaker_access(s_click_port);
}

/* ========================================================================== */
/* $69A9 -- pause and mute                                                    */
/*                                                                            */
/* The dispatch chain above ends in a JSR here, so this is not only the pause */
/* key: it is where every key the game does not recognise comes to be         */
/* ignored. Both tests simply fail and it returns.                            */
/*                                                                            */
/* ESC spins on the keyboard until something is pressed, and the key that     */
/* ends the wait is left in A and falls through into the Ctrl-S test -- so    */
/* ESC and then Ctrl-S resumes and toggles the sound in a single pass. The    */
/* strobe is cleared after the wait rather than before it, so a key already   */
/* pending when ESC arrives ends the pause immediately.                       */
/*                                                                            */
/* Three of the six blocks are decoded from the binary rather than checked by */
/* a recording: $69AD and $69B2, the spin and the strobe clear, and $69B9,    */
/* the toggle. No committed recording presses either key. Converting takes    */
/* those addresses off the site list, so this comment is now the only record  */
/* that they are unverified.                                                  */
/*                                                                            */
/* What the other three do get is thinner than it looks, and it was measured  */
/* rather than assumed. Only play-rebind reaches this routine at all, and it  */
/* reaches it once. Inverting the Ctrl-S test is caught, by trace-ext on that */
/* scenario. Mis-charging the entry block is not: 4 cycles written as 5       */
/* passes verify.sh 4/4 and every probe scenario, because the one oracle that */
/* compares cycles (verify.sh) never executes this code, and the oracle that  */
/* does (probe-acceptance.sh) stamps its input on a probe counter precisely   */
/* so that it does not depend on cycles. 4 written as 4000 is caught, by      */
/* frame-boundary drift. So the cycle charges here are checked only to a      */
/* resolution of roughly a frame, and the numbers below rest on the opcode    */
/* timings, not on a passing test.                                            */
/* ========================================================================== */

void game_pause_or_toggle_sound_native(uint8_t key) {
  TICK(4);
  if (key == KEY_ESC) {
    for (;;) {
      TICK(6);
      key = io_peek(0xc000);
      if (key & 0x80)
        break;
      TICK(1);
    }
    TICK(4);
    io_poke(0xc010, key);
  } else {
    TICK(1);
  }

  TICK(4);
  if (key == KEY_CTRL_S) {
    TICK(10);
    s_sound_muted = !s_sound_muted;
  } else {
    TICK(1);
  }

  TICK(6);
}

LifeEnd game_play_loop_native(uint8_t *cell_out) {
  TICK(6);
  game_find_nearest_apple();

  for (;;) {
    /* --- $628B: a key, and what the game makes of it -------------------- */
    TICK(6);
    game_read_key_native();
    TICK(6);
    uint8_t code;
    { // was game_read_direction()

      // The JSR stays here rather than moving into the native routine, because
      // game_step_bouncers's own adapter is what keeps $6216 -- the RTS it shares
      // with the unconverted game_read_key -- a probe site.
      /*$6C72*/ TICK(6);
      const uint8_t key = game_step_bouncers_native();
      // $6216 is an RTS shared with game_read_key, so it stays a probe site even
      // though the routine that used to hold it is gone. Spelled with plain CYCLES
      // rather than GAME_CYCLES_SHARED because site_addrs() does not grep for that
      // form -- it would have kept probing and left the list, which is the silent
      // half of a hole rather than the loud one.
      TICK(6);

      // The original saves the key on the stack across the $0302 test; here it is
      // an argument. The pushed byte is never observed: nothing between the PHA
      // and the PLA samples memory, and ram.probe compares only the live stack.
      code = game_read_direction_native(key);
    }

    uint8_t dir = s_direction;
    uint8_t shape;

  dispatch: /* $6291 */
    TICK(2);
    if (!(code & 0x80)) {
      TICK(1);
      goto autopilot;
    }

  steer: /* $6293 -- a key with the high bit on, so the player is steering */
    TICK(10);
    s_click_count = 0x10;
    if (code == KEY_TURN_CW) {
      TICK(14);
      shape = (uint8_t)(dir + 0x10);
      // $624E is left one below range here and normalised at $62B8, which is
      // the order the samples see; computing the wrap early would be tidier
      // and would not match.
      s_direction = (uint8_t)(dir - 1);
      goto draw;
    }

    TICK(1);
    TICK(2);
    TICK(1);
    TICK(4);
    if (code == KEY_TURN_CCW) {
      TICK(17);
      shape = (uint8_t)(dir + 0x04);
      s_direction = (uint8_t)(dir + 1);
      goto draw;
    }

    TICK(1);
    TICK(4);
    if (code == KEY_QUIT) {
      TICK(12);
      return LIFE_QUIT;
    }

    // The four absolute keys, each a compare and a table lookup. Whatever the
    // table gives is examined as if the player had pressed it, which is why
    // this goes back to the top rather than falling through.
    {
      static const struct {
        uint8_t key;
        uint16_t cmp_addr, cmp_cycles, hit_addr, hit_cycles, edge_addr;
      } kAbsolute[] = {
          {KEY_UP, 0x6353, 4, 0x6357, 11, 0x634b},
          {KEY_LEFT, 0x6360, 4, 0x6364, 11, 0x6355},
          {KEY_RIGHT, 0x636d, 4, 0x6371, 11, 0x6362},
          {KEY_DOWN, 0x637a, 4, 0x637e, 11, 0x636f},
      };
      for (unsigned i = 0; i < 4; ++i) {
        GAME_CYCLES(kAbsolute[i].edge_addr, 1);
        GAME_CYCLES(kAbsolute[i].cmp_addr, kAbsolute[i].cmp_cycles);
        if (code == kAbsolute[i].key) {
          GAME_CYCLES(kAbsolute[i].hit_addr, kAbsolute[i].hit_cycles);
          code = turn_for_key(kAbsolute[i].key, dir);
          goto dispatch;
        }
      }
    }

    // $639B -- anything else is the pause/mute key.
    TICK(1);
    TICK(6);
    game_pause_or_toggle_sound_native(code);
    TICK(3);
    goto pace;

  autopilot: /* $6308 -- no steering this step */
    TICK(6);
    if (s_demo_mode) {
      uint8_t proposal = 0;
      TICK(6);
      const SteerChoice choice = game_auto_steer(&proposal);
      if (choice == STEER_BOXED_IN) {
        // $6AB3 -- the auto-steer found nothing safe and gave up by jumping
        // here over its own return address. From this side that is simply
        // "carry straight on and take what comes".
        goto straight;
      }
      TICK(2);
      if (choice == STEER_TURN) {
        // $6312 -- it proposed a turn. Act on it as though it had been typed,
        // re-entering below the high-bit test the way $6312 does.
        TICK(3);
        code = proposal;
        goto steer;
      }
      TICK(1);
    } else {
      TICK(1);
    }

  straight: /* $6315 */
    TICK(11);
    shape = (uint8_t)(dir + 0x08);

  draw: /* $62A5 -- draw the head, then step it one cell */
  {
    TICK(28);
    const Cell head = s_head;
    s_shape = shape;
    game_draw_head_native(0x0c, head);

    // $62B8 -- the direction back into 1..4, and the ink is the direction.
    TICK(26);
    dir = (uint8_t)((((uint8_t)(s_direction - 1)) & 3) + 1);
    s_direction = dir;
    rom_setcol(dir);

    TICK(14);
    rom_plot(head.row, head.col);

    // $62D1 -- advance the head, and see what is there.
    TICK(42);
    const Cell next = {
        .col = (uint8_t)(head.col + kColDelta[dir]),
        .row = (uint8_t)(head.row + kRowDelta[dir]),
    };
    s_head = next;
    const uint8_t cell = scrn_cell(next);

    TICK(25);
    s_life_outcome = cell;
    TICK(6);
    plot_shape_at(dir, 0x0c, next);
    TICK(3);

    /* --- $6474: what did it move onto? ------------------------------- */
    TICK(6);
    if (cell == 0) {
      TICK(3);
      TICK(8);
      rom_setcol(0x07);
      TICK(14);
      rom_plot(next.row, next.col);

      // $633C -- the gate is column $14 of row 0.
      TICK(8);
      if (next.col == 0x14) {
        TICK(6);
        if (next.row == 0) {
          TICK(6);
          return LIFE_GATE;
        }
        TICK(1);
      } else {
        TICK(1);
      }
      goto tail;
    }

    TICK(1);
    TICK(4);
    if (cell == 0x0f) {
      // $6480 -- an apple. Marked here; the caller does the scoring.
      TICK(14);
      s_click_count = 0x20;
      rom_setcol(0x07);
      TICK(14);
      game_mark_head_native(next.row, next.col);
      TICK(6);
      *cell_out = cell;
      return LIFE_APPLE;
    }

    // $6494 -- solid. Pause, buzzing, for a length taken byte by byte out
    // of ROM at $E000: nobody chose those numbers, they were simply there.
    TICK(1);
    TICK(6);
    // Both loops are DEY/BNE and DEX/BNE, which test *after* decrementing,
    // so a count of zero means 256 and not none. Ten of the bytes this reads
    // out of $E000 are zero, so that is the common case here rather than a
    // corner: getting it wrong costs 12,790 cycles of the pause, which is
    // three quarters of a frame and shifts everything after it.
    uint8_t x = 0xff;
    do {
      TICK(6);
      uint8_t y = peek(0xe000 + x);
      do {
        TICK(4);
        --y;
        if (y != 0)
          TICK(1);
      } while (y != 0);
      TICK(12);
      click_speaker();
      --x;
      if (x != 0)
        TICK(1);
    } while (x != 0);
    TICK(6);
    *cell_out = cell;
    return LIFE_CRASH;
  }

  tail: /* $63A1 -- trim the tail, unless the snake is still growing */
    TICK(6);
    if (s_growth) {
      TICK(15);
      s_growth = (uint8_t)(s_growth - 1);
      s_click_count = 0x07;
    } else {
      TICK(1);
      TICK(14);
      const Cell tail = s_tail;
      const uint8_t under = scrn_cell(tail);

      // The original keeps `under` on the stack across the erase. It stays on
      // the emulated stack here too: ram.probe hashes the live stack, and a
      // sample taken inside the plotter would otherwise see a byte on one
      // engine and not the other.
      TICK(11);
      rom_setcol(0x00);
      TICK(14);
      rom_plot(tail.row, tail.col);
      TICK(25);
      plot_at(0x00, tail);

      // $63DA -- the byte that was under the tail is the direction the tail
      // must follow, so the same delta tables move it on.
      TICK(44);
      const uint8_t tail_dir = under;
      const Cell tail_next = {
          .col = (uint8_t)(tail.col + kColDelta[tail_dir]),
          .row = (uint8_t)(tail.row + kRowDelta[tail_dir]),
      };
      s_tail = tail_next;
      const uint8_t ahead = scrn_cell(tail_next);

      TICK(32);
      plot_shape_at((uint8_t)(ahead + 0x0c), 0x0c, tail_next);
      TICK(3);
    }

  pace: /* $640F -- the timer, the walls, and the delay that sets the speed */
    TICK(20);
    click_speaker();
    {
      const uint8_t left = (uint8_t)(s_life_timer - 1);
      s_life_timer = left;
      if (left == 0) {
        TICK(12);
        return LIFE_TIMEOUT;
      }
    }

    TICK(1);
    TICK(10);
    const uint8_t gate_cell = game_draw_side_walls_native();
    TICK(4);
    if (gate_cell == 0) {
      // $642D -- the gate at the bottom is clear, so draw it. No edge charge
      // here: $6429's branch falls through to this and is only *taken* when
      // the cell is occupied.
      TICK(31);
      s_shape = 0x15;
      plot_hline_at(0x0d, 0x12, 0x27, 0x16);
      TICK(8);
      rom_setcol(0x0d);
      TICK(10);
      rom_plot(0x27, 0x14);
    } else {
      TICK(1);
    }

    // $6450 -- the delay that sets the speed. $0300 iterations, each one
    // ticking the falling tone and taking a key, and counting $6473 down for
    // as long as the last move gave it something to say.
    // DEX/BNE again: $0300 of zero would mean 256 passes, not none.
    TICK(4);
    uint8_t n = s_step_delay;
    do {
      TICK(6);
      game_tick_sound_native();
      TICK(11);
      game_read_key_native();
      TICK(6);
      if (s_click_count) {
        TICK(18);
        click_speaker();
        s_click_count = (uint8_t)(s_click_count - 1);
      } else {
        TICK(1);
      }
      TICK(10);
      --n;
      if (n != 0)
        TICK(1);
    } while (n != 0);
    TICK(3);
  }
}

/* ========================================================================== */
/* $6A32 -- the auto-steer                                                    */
/*                                                                            */
/* Chase the apple that $69C3 left at $6B3B/$6B3C. Candidate directions are    */
/* tried in order of how much they help, and the first one game_move_ok        */
/* accepts wins: the row is closed before the column, and each axis is tried   */
/* toward the apple before away from it.                                       */
/*                                                                            */
/* Two of the eight attempts repeat earlier ones, and the whole second half    */
/* re-tries the row after the column has failed. It is a search someone        */
/* unrolled by hand, and the order *is* the algorithm, so this keeps the order */
/* rather than folding the repeats away.                                       */
/*                                                                            */
/* None of the three committed recordings reaches the last three attempts or   */
/* the give-up. The auto-steer only runs when $0302 is set, which is a mode    */
/* chosen at the setup screen ($73DD, $7457, $7478) that none of them picks.   */
/* The blocks nothing exercises are $6A83, $6A8B, $6A9F, $6AA7, $6AA9, $6AB1   */
/* and $6AB3, decoded from the binary the way $664A was, and this comment is   */
/* now the only record of it -- converting the routine takes those addresses   */
/* off the site list, so probe-acceptance.sh no longer counts them among the   */
/* unverified.                                                                 */
/* ========================================================================== */

/// $6A55 -- the absolute-direction key that turns the snake to face \p dir.
/// Index 0 is unused; the four that matter are the I/J/K/M diamond.
static uint8_t key_for_direction(uint8_t dir) {
  return kSteerKey[dir];
}

/// Propose \p dir and report whether the move is allowed.
///
/// $6B38 holds the proposal because snake_move_verdict() reads it there, and
/// because the accept path at $6A48 reads it back out. The cycle charges are
/// arguments so that they land either side of the call, as the original's
/// STA/JSR and the BEQ after it do.
///
/// "Allowed" is whatever leaves Z set in the original, which is the two safe
/// verdicts -- and also a taken cell that happens to be the apple, since
/// eating it is the point.
static bool steer_try(
    uint8_t dir,
    uint16_t before_addr,
    unsigned before_cycles,
    uint16_t after_addr,
    unsigned after_cycles) {
  /* What the original's BEQ tests. It was s_status_not_z; nothing outside this
     routine ever read that flag, so it is a local. This one the gate does
     run: inverting the sense of the return below fails the play screen. */
  uint8_t move_taken = 0;

  GAME_CYCLES(before_addr, before_cycles);
  s_steer_dir = dir;
  { // was game_move_ok()

    /*$6AB8*/ TICK(0);

    uint8_t cell = 0;
    const MoveVerdict v = snake_move_verdict(s_steer_dir, &cell);

    // Turn the verdict back into what the callers at $6A40 branch on. The
    // switch covers every MoveVerdict, so move_taken is always assigned; the
    // initialiser above is there because the compiler cannot see that.
    switch (v) {
    case MOVE_TARGET_TAKEN:
    // $6AD9 CMP #$0F left these.
    move_taken = (cell != 0x0f);
    break;
    case MOVE_ROW_ZERO:
    case MOVE_OK:
    move_taken = 0x00;
    break;
    case MOVE_DEAD_END:
    move_taken = 0x01;
    break;
    }
  }
  GAME_CYCLES(after_addr, after_cycles);
  // The original branches on Z, which game_move_ok leaves set for exactly the
  // verdicts that permit the move -- including a target holding the apple.
  return move_taken == 0;
}

SteerChoice game_auto_steer(uint8_t *key_out) {
  TICK(10);
  const uint8_t apple_row = s_apple.row;
  const uint8_t apple_col = s_apple.col;
  const uint8_t head_row = s_head.row;
  const uint8_t head_col = s_head.col;

  bool settled = false;

  // $6A32 -- if the apple is on another row, close that first. This one test
  // is a BNE, so its edge is charged when the move is *refused*; every later
  // test is a BEQ to the accept path and charges its edge the other way round.
  if (apple_row != head_row) {
    TICK(4);
    uint8_t dir;
    if (apple_row >= head_row) {
      TICK(1);
      dir = DIR_DOWN;
    } else {
      TICK(2);
      dir = DIR_UP;
    }
    settled = steer_try(dir, 0x6a40, 10, 0x6a46, 2);
    if (!settled)
      TICK(1);
  } else {
    TICK(1);
  }

  // $6A5A -- the column, toward the apple and then away from it.
  if (!settled) {
    TICK(10);
    if (apple_col >= head_col) {
      settled = steer_try(DIR_RIGHT, 0x6a62, 12, 0x6a6a, 2);
      if (settled) {
        TICK(1);
      } else {
        settled = steer_try(DIR_LEFT, 0x6a6c, 12, 0x6a74, 2);
        if (settled)
          TICK(1);
        else
          TICK(3);
      }
    } else {
      TICK(1);
      settled = steer_try(DIR_LEFT, 0x6a79, 12, 0x6a81, 2);
      if (settled) {
        TICK(1);
      } else {
        settled = steer_try(DIR_RIGHT, 0x6a83, 12, 0x6a8b, 2);
        if (settled)
          TICK(1);
        // Refused: falls straight into $6A8D, where the other branch had to
        // spend a JMP to get.
      }
    }
  }

  // $6A8D -- the row again, now as an escape rather than as progress.
  if (!settled) {
    TICK(10);
    if (apple_row >= head_row) {
      settled = steer_try(DIR_DOWN, 0x6a95, 12, 0x6a9d, 2);
      if (settled)
        TICK(1);
    } else {
      TICK(1);
    }
    if (!settled) {
      settled = steer_try(DIR_UP, 0x6a9f, 12, 0x6aa7, 2);
      if (settled) {
        TICK(1);
      } else {
        settled = steer_try(DIR_DOWN, 0x6aa9, 12, 0x6ab1, 2);
        if (settled) {
          TICK(1);
        } else {
          // $6AB3 -- nothing is safe.
          TICK(11);
          return STEER_BOXED_IN;
        }
      }
    }
  }

  // $6A48 -- a direction was accepted. Already going that way means there is
  // nothing to say; otherwise name the key that turns to it.
  TICK(10);
  const uint8_t dir = s_steer_dir;
  if (dir == s_direction) {
    TICK(1);
    TICK(6);
    *key_out = dir;
    return STEER_STRAIGHT;
  }
  TICK(6);
  TICK(6);
  *key_out = key_for_direction(dir);
  return STEER_TURN;
}

/* ========================================================================== */
/* $72CE -- the status panel                                                  */
/*                                                                            */
/* Six labelled numbers in a 2x3 grid on text rows $14-$16:                   */
/*                                                                            */
/*     SCORE:  ....        HI SCORE:  ....                                    */
/*     APPLES LEFT:  ..    VALUE:  ..                                         */
/*     SNAKES LEFT:  ..    LEVEL:  .                                          */
/*                                                                            */
/* Every field is the same four moves: place the cursor, print a label, clear  */
/* the leading-zero flag, then print BCD bytes most significant first and a    */
/* '0' if suppression ate all of them. The labels are inline strings after the */
/* JSR -- see game_print_inline_str -- so the "argument" each call passes is   */
/* the address the original pushed, and the text follows it.                   */
/*                                                                            */
/* $24 and $25 are the ROM's CH and CV. Only the last write to CV is followed  */
/* by VTAB, because COUT recomputes the line base itself on the way past.      */
/* Twenty-nine of the thirty block heads here keep their probes, spelled            */
/* GAME_CYCLES_SHARED. game_print_inline_str returns to an address it computes */
/* at run time, so every one of its return points is a dynamic block -- and    */
/* the generated C therefore still carries this routine's tail as cases        */
/* nothing can reach but that are still text in the file, and still on the     */
/* site list. The interpreter reports those addresses; so must this. Only      */
/* $72CE itself leaves, because that is the one the call now replaces.         */
/*                                                                            */
/* Every charge below is written out with a literal address rather than passed */
/* to a helper. A helper reads better and is wrong: the site-list lint finds   */
/* these by grepping for the literal, so an address reaching GAME_CYCLES       */
/* through a parameter is invisible to it, and nine of these were.            */
/* ========================================================================== */

/// $002C -- the flag game_print_bcd raises when it prints a digit, so that
/// game_print_zero_if_blank knows whether the field came out empty.
static void clear_leading_zero_flag(void) {
  s_h2 = 0x00;
}

void game_status_panel(void) {
  // SCORE, row $14 column $00. Four BCD bytes at $7252, little-endian.
  TICK(16);
  s_cv = 0x14;
  s_ch = 0x00;
  game_print_inline_str(0x72d8);
  TICK(15);
  clear_leading_zero_flag();
  game_print_bcd_native(s_score[3]);
  TICK(10);
  game_print_bcd_native(s_score[2]);
  TICK(10);
  game_print_bcd_native(s_score[1]);
  TICK(10);
  game_print_bcd_native(s_score[0]);
  TICK(6);
  game_print_zero_if_blank_native();

  // HI SCORE, same row, column $14. Four bytes at $7256.
  TICK(11);
  s_ch = 0x14;
  game_print_inline_str(0x7307);
  TICK(15);
  clear_leading_zero_flag();
  game_print_bcd_native(s_hi_score[3]);
  TICK(10);
  game_print_bcd_native(s_hi_score[2]);
  TICK(10);
  game_print_bcd_native(s_hi_score[1]);
  TICK(10);
  game_print_bcd_native(s_hi_score[0]);
  TICK(6);
  game_print_zero_if_blank_native();

  // APPLES LEFT, row $15 column $00. Two bytes at $725A.
  TICK(16);
  s_ch = 0x00;
  s_cv = 0x15;
  game_print_inline_str(0x733d);
  TICK(15);
  clear_leading_zero_flag();
  game_print_bcd_native(s_apples_left[1]);
  TICK(10);
  game_print_bcd_native(s_apples_left[0]);
  TICK(6);
  game_print_zero_if_blank_native();

  // A space, which the next field's cursor move immediately overrides. It is
  // there to wipe the character one place past this field, left over from a
  // longer count earlier in the game.
  TICK(8);
  rom_cout(0xa0);

  // VALUE, same row, column $14. Two bytes at $71CB -- the current worth of an
  // apple, which game_set_apple_value computes per level.
  TICK(11);
  s_ch = 0x14;
  game_print_inline_str(0x736b);
  TICK(15);
  clear_leading_zero_flag();
  game_print_bcd_native(s_apple_value[1]);
  TICK(10);
  game_print_bcd_native(s_apple_value[0]);
  TICK(6);
  game_print_zero_if_blank_native();

  // SNAKES LEFT, row $16 column $00. One byte at $725E, printed as though it
  // were the low half of a two-byte field: the high half is the literal 0
  // below, which prints nothing at all once leading zeros are suppressed. It
  // costs a call to keep the shape of every other field.
  TICK(16);
  s_cv = 0x16;
  s_ch = 0x00;
  game_print_inline_str(0x7392);
  TICK(11);
  clear_leading_zero_flag();
  game_print_bcd_native(0x00);
  TICK(10);
  game_print_bcd_native(s_lives);
  TICK(6);
  game_print_zero_if_blank_native();

  // LEVEL, same row, column $14. One byte at $7265.
  TICK(11);
  s_ch = 0x14;
  game_print_inline_str(0x73b8);
  TICK(15);
  clear_leading_zero_flag();
  game_print_bcd_native(s_level);
  TICK(6);
  game_print_zero_if_blank_native();

  // Home the cursor. This CV write is the one that needs VTAB, because nothing
  // prints after it to recompute the line base.
  TICK(11);
  s_cv = 0x00;
  rom_fc68();
  TICK(6);
}

/* ========================================================================== */
/* $78B3 -- the bonus screen                                                  */
/*                                                                            */
/* Awarded when a level is finished. The bonus is twice whatever an apple was  */
/* worth on that level, which the original says twice over: once as BCD        */
/* arithmetic into $78B0/$78B1 so the number can be printed, and once as two   */
/* consecutive calls to game_add_score, which adds $71CB/$71CC each time.      */
/* Neither reads the other's answer.                                          */
/*                                                                            */
/* This routine is *entered with decimal mode set* -- unusual here, and the    */
/* reason the adapter cannot assert against it the way the others do. The      */
/* generated C makes the same claim: its binary-mode path is dead, folded away */
/* because D is known set on entry. $78C7 clears it before anything else runs. */
/*                                                                            */
/* Then a box: a frame in ink 9 and its interior wiped in ink 0, four rows at  */
/* a time. The text goes through the game's own hi-res font, which is what     */
/* $6641 installs and what the two pokes at $795F take back out again.         */
/* ========================================================================== */

void game_bonus_screen(void) {
  // $78B3 -- double the apple's value into $78B0/$78B1, in BCD.
  TICK(36);
  const uint16_t lo = adc_dec16(s_apple_value[0], s_apple_value[0], 0x00);
  s_bonus_amount[0] = (uint8_t)lo;
  const uint16_t hi = adc_dec16(s_apple_value[1], s_apple_value[1], (uint8_t)(lo >> 8) & 0x01);
  s_bonus_amount[1] = (uint8_t)hi;

  // Twice, because the bonus is twice the apple value and game_add_score adds
  // it once.
  game_add_score_native();
  TICK(6);
  game_add_score_native();
  TICK(6);
  game_status_panel();

  // $78D1 -- the frame, in ink 9: top and bottom edges, then both sides.
  TICK(31);
  s_shape = 0x01;
  // Columns $0D-$1A, rows $10-$15. The two sides used to inherit their column
  // from the edge above: an hline left $02 at its own endpoint, so the
  // first vline ran down $1A, the right edge, and not the $0D it looks like.
  plot_hline_at(0x09, 0x0d, 0x10, 0x1a);
  TICK(16);
  plot_hline_at(0x09, 0x0d, 0x15, 0x1a);
  TICK(16);
  plot_vline_at(0x09, 0x1a, 0x10, 0x15);
  TICK(16);
  plot_vline_at(0x09, 0x0d, 0x10, 0x15);

  // $7909 -- the interior, in ink 0, one row at a time from $11 to $14. The
  // original re-loads $02 each time and increments $03 in place, which is why
  // the rows are not written out as constants.
  TICK(26);
  plot_hline_at(0x00, 0x0e, 0x11, 0x19);
  TICK(16);
  plot_hline_at(0x00, 0x0e, 0x12, 0x19);
  TICK(16);
  plot_hline_at(0x00, 0x0e, 0x13, 0x19);
  TICK(16);
  plot_hline_at(0x00, 0x0e, 0x14, 0x19);

  // $7937 -- "BONUS: " and the amount, through the hi-res font.
  TICK(16);
  s_ch = 0x0f;
  s_cv = 0x09;
  game_install_cout_vector();
  // The LDA #$4A flags are overwritten by the second load; only these outlive.
  TICK(6);
  game_print_inline_str(0x7944);
  TICK(15);
  s_h2 = 0x00;
  game_print_bcd_native(s_bonus_amount[1]);
  TICK(10);
  game_print_bcd_native(s_bonus_amount[0]);

  // $795D -- COUT back to the ROM's, and $02 becomes the outermost counter of
  // the pause below.
  TICK(15);
  s_csw = 0xfdf0;
  uint8_t passes = 0x20; // $02 was the outermost counter

  // $7969 -- hold the screen. Everything from $794D on keeps its probe: the
  // inline-string printer returns to an address it computes, so this whole tail
  // survives in the generated C as dynamic blocks nothing reaches, and stays on
  // the site list. The edges are exempt -- edges are never probed.
  // Three nested counters, the innermost taking its
  // length from the middle one, so the delay shortens as it goes; the keyboard
  // is read and discarded each time round to keep the strobe clear. All three
  // are DEX/DEY loops, which test after decrementing -- a count of zero would
  // mean 256, and none of these start at zero.
  do {
    TICK(2);
    uint8_t x = 0x80;
    do {
      TICK(4);
      uint8_t y = x;
      do {
        TICK(4);
        --y;
        if (y != 0)
          TICK(1);
      } while (y != 0);
      TICK(12);
      click_speaker();
      --x;
      if (x != 0)
        TICK(1);
    } while (x != 0);
    TICK(7);
    passes = (uint8_t)(passes - 1);
    if (passes != 0)
      TICK(1);
  } while (passes != 0);
  TICK(6);
}

/* ========================================================================== */
/* $6256 -- start a life                                                      */
/*                                                                            */
/* Head and tail both at row $27 column $14 -- the snake begins as one cell,   */
/* in the gate at the bottom centre, facing up. $6254 owes it ten segments of  */
/* growth, which is what makes it appear to emerge from the gate rather than   */
/* start full length.                                                          */
/*                                                                            */
/* $6258 supplies the tail column rather than it being written here: the       */
/* original loads $14 into A, and game_start_life hands back $14 from its own  */
/* constant at $6630. The two are unrelated and happen to agree.               */
/*                                                                            */
/* Ends by falling into the main loop, which the original does with a JSR and  */
/* an RTS it never reaches -- the generated call passes a return address of 0  */
/* for the same reason.                                                        */
/* ========================================================================== */

void game_begin_life(void) {
  TICK(8);
  const uint8_t tail_col = game_start_life(0x14);

  TICK(36);
  s_tail.col = tail_col; // from $6630, by way of $660F
  s_head.row = 0x27; // the bottom edge
  s_tail.row = 0x27; // the same cell
  s_direction = DIR_UP;
  s_growth = 0x0a; // ten
  s_life_timer = 0x64;

  // $6279 -- empty the sixteen-entry key ring at $623C. DEX/BPL, so it runs
  // down through 0 and stops when X wraps negative, one more pass than a
  // count of $0F suggests.
  uint8_t x = 0x0f;
  do {
    TICK(9);
    s_key_ring[x] = 0x00;
    --x;
    if (!(x & 0x80))
      TICK(1);
  } while (!(x & 0x80));

  TICK(11);
  s_ring_read = 0x00;
  s_ring_write = 0x00;
  game_play_one_life();
}

/* ========================================================================== */
/* $7980 -- the setup screen                                                  */
/*                                                                            */
/* Three jobs, in the order the original does them: clamp the pointer          */
/* game_rand_byte reads its numbers through, offer the difficulty prompt, and  */
/* -- if the player presses C rather than a digit -- run the key redefinition  */
/* screen instead.                                                             */
/*                                                                            */
/* $0301 is the difficulty, 0 to 2. $0302 is the flag that decides whether the */
/* game plays itself: set here when nobody answers the prompt in time, and     */
/* cleared when they do. That is the only place it is ever set, which is why   */
/* the auto-steer at $6A32 runs in some recordings and not others.             */
/*                                                                            */
/* $73D7 makes the first pass different. It starts zero, so the very first     */
/* call sets the demo going and returns without asking anything; every call    */
/* after that reaches the prompt.                                              */
/*                                                                            */
/* Nine blocks here are decoded from the binary rather than checked by a       */
/* recording: $73DD (that first pass), $742D through $744C (the joystick, only */
/* read when $6C71 selects it), $7455 (the prompt timing out) and $798A (one   */
/* arm of the clamp). Converting them takes their addresses off the site list, */
/* so this comment is the only record that they are unverified.                */
/* ========================================================================== */

void game_setup_screen(void) {
  // $7980 -- keep $0E/$0F inside the window game_rand_byte expects.
  TICK(7);
  const uint8_t hi = (uint8_t)(s_rand_ptr >> 8);
  bool clamp_lo = hi >= 0x1f;
  if (!clamp_lo) {
    TICK(4);
    clamp_lo = hi < 0x18;
    if (!clamp_lo)
      TICK(1);
  } else {
    TICK(1);
  }
  if (clamp_lo) {
    TICK(8);
    s_rand_ptr &= 0xffde;
  }
  TICK(13);
  s_rand_ptr = (uint16_t)((s_rand_ptr & 0x1fff) | 0x1800);

  // $73D8 -- the first call through here never asks anything.
  TICK(6);
  if (!s_setup_seen) {
    TICK(20);
    s_demo_mode = true;
    s_difficulty = 0x01;
    s_setup_seen = true;
    return;
  }

  // $73E9 -- the prompt, and the two counters that time it out. $02 is the
  // outer one and $03 the inner; both count *up* to zero.
  TICK(1);
  TICK(16);
  s_cv = 0x17;
  s_ch = 0x00;
  game_print_inline_str(0x73f3);
  TICK(10);
  // $02 and $03 time the prompt out: the inner one wraps 256 times per tick of
  // the outer, and when the outer wraps nobody has answered.
  uint8_t outer_count = 0xe8;
  uint8_t inner_count = 0x00;

  uint8_t key;
wait: /* $741C */
  for (;;) {
    // $741C -- spin Y round once, then look at the keyboard. This site keeps
    // its probe: it is one of the addresses the replay coordinate counts.
    // Y again, and again not initialised by the original. Measured the same
    // way as spin's: zero on every pass of both scenarios, and the loop itself
    // leaves it zero, so only the first arrival could ever differ.
    uint8_t ticks = 0;
    do {
      TICK(4);
      ++ticks;
      if (ticks != 0)
        TICK(1);
    } while (ticks != 0);

    GAME_CYCLES_COORD(0x741f, 6);
    key = io_peek(0xc000);
    if (key & 0x80) {
      TICK(1);
      break;
    }

    TICK(7);
    const uint8_t inner = (uint8_t)(inner_count + 1);
    inner_count = inner;
    if (inner != 0) {
      TICK(1);
      continue;
    }

    // $7428 -- once the inner counter wraps, try the joystick, if one is
    // selected. Each button stands in for a digit.
    TICK(6);
    if (s_joystick_selected) {
      TICK(10);
      io_peek(0xc05b);
      if (!(io_peek(0xc062) & 0x80)) {
        TICK(5);
        key = 0xb1;
        break;
      }
      TICK(1);
      TICK(10);
      io_peek(0xc05a);
      if (!(io_peek(0xc062) & 0x80)) {
        TICK(5);
        key = 0xb0;
        break;
      }
      TICK(1);
      TICK(6);
      if (!(io_peek(0xc063) & 0x80)) {
        TICK(5);
        key = 0xb2;
        break;
      }
      TICK(1);
    } else {
      TICK(1);
    }

    // $7451 -- the outer counter. When it wraps too, nobody is answering.
    TICK(7);
    const uint8_t outer = (uint8_t)(outer_count + 1);
    outer_count = outer;
    if (outer == 0) {
      TICK(20);
      s_demo_mode = true;
      s_difficulty = 0x01;
      io_poke(0xc010, 0x01);
      return;
    }
    TICK(1);
  }

  // $7461 -- something was pressed. Clear the strobe with it still in A, the
  // way the original does.
  TICK(8);
  io_poke(0xc010, key);
  if (key != 0xc3) {
    TICK(4);
    if (key < 0xb0) {
      TICK(1);
      goto wait;
    }
    TICK(4);
    if (key >= 0xb3) {
      TICK(1);
      goto wait;
    }
    // $7470 -- a digit. The subtract is a plain SBC with carry set.
    TICK(24);
    s_difficulty = (uint8_t)(key - 0xb0);
    s_demo_mode = false;
    io_poke(0xc010, 0x00);
    return;
  }

  // $747F -- C, so redefine the keys instead. Show the six current bindings,
  // draw the highlight, then walk them again asking for replacements.
  TICK(1);
  TICK(6);
  game_clear_hgr_native();
  TICK(10);
  io_peek(0xc052);
  game_install_cout_vector();
  // The LDA #$4A flags are overwritten by the second load; only these outlive.
  TICK(11);
  s_cv = 0x01;
  game_print_inline_str(0x748e);

  TICK(2);
  for (uint8_t i = 0; i != 6; ++i) {
    TICK(10);
    game_show_key_native(i, s_key_table[i]);
    TICK(6);
    if (i != 5)
      TICK(1);
  }

  TICK(26);
  plot_shape_at(0x02, 0x0c, (Cell){.col = 0x1e, .row = 0x12});
  // The stem below it, down the same column -- which the original inherited
  // from the plot above rather than restating.
  TICK(21);
  s_shape = 0x0a;
  plot_vline_at(0x0c, 0x1e, 0x13, 0x1d);
  // At the stem's far end -- the vline above left $03 on $1D.
  TICK(11);
  plot_shape_at(0x0e, 0x0c, (Cell){.col = 0x1e, .row = 0x1d});

  TICK(2);
  for (uint8_t i = 0; i != 6; ++i) {
    TICK(6);
    const uint8_t chosen = game_edit_key_native(i);
    TICK(11);
    s_key_table[i] = chosen;
    game_show_key_native(i, chosen);
    TICK(6);
    if (i != 5)
      TICK(1);
  }

  // $7587 -- COUT back to the ROM's.
  TICK(16);
  s_csw = 0xfdf0;
}

/* ========================================================================== *
 * Adapters: 6502 machine state in, real C out                              *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte routines decompiled by hand.
///
/// Like a2rom.c, this file is textually included into the generated translation
/// unit -- it uses the machine state that `apple2tc/system2-inc.h` defines with
/// internal linkage, plus generated helpers. It must come *after* a2rom.c,
/// since it calls the static `rom_cout1`. See snake-byte-ext.c.
///
/// Why hand-decompiled
/// -------------------
/// apple2tc drives disassembly from a recorded run. A routine only reached
/// through an indirect jump that the recording never took is invisible to it:
/// the bytes get classified as data and never appear in the output. That is the
/// case here, so these routines are decoded from the binary directly.


/* ========================================================================== */
/* $664A -- the game's own COUT handler.                                      */
/*                                                                            */
/* $6641 installs it:                                                         */
/*     LDA #$4A / STA $36 / LDA #$66 / STA $37 / RTS                          */
/* pointing CSWL/CSWH at $664A, so every subsequent COUT ($FDED = JMP ($36))   */
/* lands here. $6641 is called from $7485 and $793F.                          */
/*                                                                            */
/* The recorded session never runs $6641 -- neither it nor its callers appear  */
/* in snake-byte.json's BranchTargets, and the run-data records $FDED -> $FDF0 */
/* only. So the tracer saw $664A as data and apple2tc emitted nothing for it.  */
/*                                                                            */
/* Decoded from snake-byte.b33 by hand. The decode was cross-checked against   */
/* apple2tc using a scratch run-data file with $664A added as a branch target; */
/* the committed snake-byte.json is deliberately unmodified, because it is a   */
/* recording of what actually happened and not a place to assert reachability. */
/* The CYCLES() constants below come from that cross-check.                    */
/*                                                                            */
/* Original:                                                                  */
/*                                                                            */
/*   664A: PHA / AND #$7F / CMP #$20 / BCS $6655                              */
/*   6651: PLA / JMP $FDF0                 ; control chars -> ROM COUT1        */
/*   6655: STA $08 / STX $02 / STY $03     ; save char, X, Y                   */
/*   665B: LDA #0 / STA $00 / STA $01                                          */
/*   6661: LDA $29 / SEC / SBC #4 / CLC / ADC #$20 / STA $05                   */
/*   666B: LDA $28 / CLC / ADC $24 / STA $04                                   */
/*   6672: LDA $08 / SEC / SBC #$20                                            */
/*   6677: ASL / ASL / ROL $01 / ASL / ROL $01     ; (c - $20) * 8, 16-bit      */
/*   667E: CLC / ADC #$A9 / STA $00 / LDA $01 / ADC #$66 / STA $01             */
/*   6689: LDX #0                                                              */
/*   668B: TXA / TAY / LDA ($00),Y / LDY #0 / STA ($04),Y                      */
/*   6693: LDA $05 / CLC / ADC #4 / STA $05        ; next hi-res scanline       */
/*   669A: INX / CPX #8 / BNE $668B                                            */
/*   669F: LDX $02 / LDY $03 / JMP $6651           ; restore, then COUT1        */
/*   66A6: JMP $6655                       ; dead: nothing references $66A6     */
/*   66A9: font, 8 bytes per glyph                                             */
/*                                                                            */
/* Two things worth noting. The destination high byte is BASH - 4 + $20, i.e.  */
/* BASH + $1C, which maps text page 1 ($04xx) onto hi-res page 1 ($20xx): the  */
/* text cursor drives where the glyph lands. And after blitting, control falls  */
/* into $6651, so ROM COUT1 still runs and does the cursor advance, wrap and   */
/* scroll -- the game draws the glyph and lets the ROM keep the bookkeeping.   */
/* ========================================================================== */

/// $7230 -- print the NUL-terminated string that follows the call.
///
/// The original takes its argument by popping its own return address, walking
/// the bytes after the JSR, and then pushing the address of the terminator so
/// that its RTS lands past them. Declaring it in inline-str.txt is what lets
/// apple2tc point each call site's fall-through past the string instead; the
/// call still passes the *original* return address, because ExternRoutines
/// derives that from the call site and not from the fall-through. So \p
/// ret_addr is the same pointer the original popped, and this reads its
/// argument from the same place for the same reason.
///
/// Nothing is pushed or popped. The original's PLA/PLA and PHA/PHA cancel, so
/// the emulated stack pointer ends where the caller left it either way, and
/// ram.probe compares only the live stack.
///
void game_print_inline_str(uint16_t ret_addr) {

  /*$7230*/ TICK(20);
  s_str_ptr = ret_addr;
  rom_fc68(); // VTAB to the current CV

  for (;;) {
    // The pointer is stepped before the read, which is why the caller passes
    // the address of the JSR's last byte rather than of the string.
    /*$7239*/ TICK(7);
    ++s_str_ptr;
    if (!(s_str_ptr & 0xff)) {
      /*$723D*/ TICK(5);
    } else {
      /*$723B*/ TICK(1);
    }

    /*$723F*/ TICK(9);
    const uint8_t ch = peek(s_str_ptr);
    if (!ch) {
      /*$7243*/ TICK(1);
      break;
    }

    /*$7245*/ TICK(6);
    rom_cout(ch);
    /*$7248*/ TICK(3);
  }

  /*$724B*/ TICK(18);
}


/* ========================================================================== */
/* $6127, $60E7, $60E4 -- the hi-res cell plotter.                            */
/*                                                                            */
/* The game treats hi-res page 1 as a grid of 48 cell rows, each four         */
/* scanlines tall and one byte (seven pixels) wide. $6000 and $6030 hold the  */
/* low and high bytes of each row's base address -- $2000, $3000, $2080,      */
/* $3080, ... -- and successive scanlines within a cell are $400 apart, which */
/* is why walking down a cell is just +4 on the high byte.                    */
/*                                                                            */
/* Arguments. The original passed all three in zero page; the cell is a        */
/* parameter here and only the shape is still a variable:                     */
/*   s_shape  picks four AND masks from the table at $6174 ($00)              */
/*   c.col    the byte offset within the cell row ($02)                       */
/*   c.row    cell row, 0-47 ($03)                                            */
/* Scratch, which was $04-$07 and is now four locals:                          */
/*   the destination pointer, advanced one scanline per iteration             */
/*   the index into the dot-pattern table at $6064                            */
/*   the scanline counter, 0-3                                                */
/*                                                                            */
/* $6064 is a 128-byte dot-pattern table, $6064-$60E3, immediately before the */
/* plotter code. It is 16 inks of 8 bytes, and the index the original builds  */
/*                                                                            */
/*     (($01 << 1 | scanline & 1) << 2) | ($02 & 3)                           */
/*                                                                            */
/* is just ink * 8 + parity * 4 + (column & 3): each ink carries four dot     */
/* phases for the column's position within the hi-res byte, in two sets so    */
/* that odd and even scanlines can differ. Ink 0 is all zeros and erases; ink */
/* 15 is $7F throughout and fills solid; the rest are dithers, several of     */
/* them the same pattern at opposite phase, which on a hi-res display is how  */
/* you get the complementary colour. Whatever comes out is ANDed with that    */
/* scanline's mask.                                                           */
/*                                                                            */
/* Decoded by hand from snake-byte.lst and the tables read out of             */
/* snake-byte.b33. The CYCLES constants and the block boundaries they sit on  */
/* deliberately mirror what apple2tc generates for the same addresses, so the */
/* block-head trace stays identical and probe-acceptance.sh can compare this  */
/* against the interpreter instruction for instruction.                       */
/* ========================================================================== */




/* ========================================================================== */
/* $6148, $615A -- runs of cells.                                             */
/*                                                                            */
/* Both load the shape once and then repeat game_draw_cell along one axis:    */
/* $6148 walks the column ($02), $615A walks the row ($03). The run ends when */
/* the moving coordinate reaches $08, tested after the cell is drawn, so the  */
/* endpoint is inclusive and a degenerate run still plots one cell. A start   */
/* past the end wraps through 255 rather than drawing nothing -- faithfully   */
/* reproduced below, since nothing in the original guards against it.         */
/*                                                                            */
/* Arguments are game_draw_cell's, plus:                                      */
/*   $08  last column ($6148) or last row ($615A), inclusive                  */
/*                                                                            */
/* $70D4 shows what they are for: with $08 = $27 it draws the four borders of */
/* the 40x40 cell playfield -- top and bottom with $6148, left and right with */
/* $615A -- which is where the grid's dimensions come from.                   */
/*                                                                            */
/* The register and flag assignments are not decoration. Callers are still    */
/* generated code that reads this machine state, so a hand-written routine    */
/* has to leave behind what the 6502 would. Where apple2tc's own output drops */
/* some of it, that is its whole-program DCE proving a particular flag dead;  */
/* setting it anyway is always safe, and cheaper than re-deriving the proof   */
/* every time a caller changes.                                               */
/* ========================================================================== */



/* ========================================================================== */
/* $7019, $7024, $7000 -- the screen-script primitives.                       */
/*                                                                            */
/* $7113 runs a little byte-coded display list: it fetches an opcode, then    */
/* that opcode's operands, then draws. 'H' ($48) is a horizontal run, 'V'     */
/* ($56) a vertical one, 'P' ($50) a single cell, 'T' ($54) sets $0304. The   */
/* three routines here are what every one of those cases is built from, which */
/* is why $7019 alone has fourteen call sites.                                */
/*                                                                            */
/* Why every draw happens twice                                               */
/* ---------------------------                                                */
/* The 'V' case is the clearest: $7185 calls $7000, then $7188 calls $615A.   */
/* The first draws the run with the ROM's lo-res PLOT, the second draws the   */
/* same run as hi-res cells. The hi-res page is what the player sees; the     */
/* lo-res page is the game's own 40x48 occupancy map, and it reads it back    */
/* with the ROM's SCRN. $62D7 is the payoff -- it adds the current direction  */
/* deltas from $6232/$6237 to the snake's head, SCRNs the result, and finds   */
/* out what it is about to run into. Collision detection for free, at the     */
/* cost of drawing everything twice.                                          */
/* ========================================================================== */




/* ========================================================================== */
/* $71F3, $7226, $7267, $702B -- the score.                                   */
/*                                                                            */
/* The score is BCD: four bytes at $7252-$7255, little-endian, eight digits.  */
/* $7267 adds to it with the 6502's decimal mode, and $71F3 prints one byte   */
/* of it as two digits by nibble, since in BCD a nibble is already a digit.   */
/*                                                                            */
/* $002C is the leading-zero flag, and it is the reason these two are a pair. */
/* $71F3 prints a digit only once $2C is non-zero, and sets $2C from the      */
/* first non-zero digit it sees, so leading zeros are suppressed across a     */
/* whole multi-byte number rather than per byte. A caller clears $2C, prints  */
/* the bytes most-significant first, then calls $7226 -- which prints a       */
/* single '0' if $2C is still clear, so that a score of zero is not printed   */
/* as nothing at all. $72E4 is the canonical example.                         */
/*                                                                            */
/* $2C is the ROM's H2, the right-hand endpoint of HLINE. The game uses the   */
/* same byte for both ($7159 stores a column into it), which is safe only     */
/* because drawing and score printing never interleave.                       */
/*                                                                            */
/* $B0 is '0' in Apple II ASCII (high bit set). Digits above 9 would print as */
/* punctuation, which is another way of saying the arithmetic must be BCD.    */
/* ========================================================================== */

/// $7209 and $7220: CLC / ADC #$B0 / JSR COUT. The two are byte-identical
/// apart from the return address the JSR pushes, so they share one body here.
/// No CYCLES of its own -- both call sites are already inside a counted block.





/* ========================================================================== */
/* $6B93 -- the merging cell plotter.                                         */
/*                                                                            */
/* Same shape as $60E4: load the shape's masks, then walk the cell's four     */
/* scanlines. Two things differ.                                              */
/*                                                                            */
/* It merges instead of replacing. $60E7 stores `pattern & mask`, wiping      */
/* whatever else shared the byte; this one stores `(pattern ^ $7F) & mask |   */
/* existing`, so it only ever sets bits. $7F and not $FF: bit 7 is the hi-res */
/* palette bit for the byte, and inverting that would shift the whole byte's  */
/* colour.                                                                    */
/*                                                                            */
/* And it builds the table index differently. $60F7 does ROL $06, giving      */
/*                                                                            */
/*     ink * 8 + parity * 4 + (column & 3)                                    */
/*                                                                            */
/* while $6BAD does ROR $06 on the same operands, giving                      */
/*                                                                            */
/*     (ink >> 1) * 4 + (column & 3)                                          */
/*                                                                            */
/* -- the parity bit lands in bit 7 and is shifted straight back out by the   */
/* two ASLs that follow. Whether that was intended is not something the       */
/* binary can tell us. What it means in practice is that the caller's ink 1   */
/* selects row 0, all zeros, which the EOR then turns into a solid $7F: the   */
/* routine fills the shape rather than dithering it. Reproduced exactly,      */
/* including the lost bit, because the screen the game draws depends on it.   */
/* ========================================================================== */


/* ========================================================================== */
/* $7045 -- draw the playfield, and run the level's display list.             */
/*                                                                            */
/* One routine, but two halves. The first draws what every level has: the     */
/* border, in both representations (see the $7019 header for why everything   */
/* is drawn twice). The second, from $7113, interprets the level's own script */
/* for whatever else it needs.                                                */
/*                                                                            */
/* The script                                                                 */
/* ----------                                                                 */
/* $8000 holds 29 of them, one per level, each ending in '*'. $7113 sets the  */
/* pointer to $8000 and skips $0303 - 1 whole scripts to find the current     */
/* level's. The opcodes, all operands fetched with game_next_byte:            */
/*                                                                            */
/*   'H' $48  ink, column, last column, row  -- horizontal run                */
/*   'V' $56  ink, row, last row, column     -- vertical run                  */
/*   'P' $50  ink, column, row               -- one cell                      */
/*   'T' $54  value                          -- store to $0304                */
/*   'E' $45  (none)  -- reset $0303 to 1 and start over, i.e. wrap to level 1 */
/*   '*' $2A  end of this level's script                                      */
/*                                                                            */
/* Anything else falls through $71C4 back to the dispatch and is ignored.     */
/* Decoded from the data: 'H' appears in 26 of the 29 scripts and 'V' in 26,  */
/* 'T' only in level 1's (which is otherwise empty -- the border is all there */
/* is), 'E' only in level 30's, and 'P' in none of them at all.               */
/*                                                                            */
/* Verification                                                               */
/* ------------                                                               */
/* Neither committed recording finishes level 1, so none of this ran in the   */
/* gate until snake-byte-easy.b33 (make-easy.sh) lowered the apple quota and  */
/* let play-hires reach level 3. 'H', 'V' and the skip loop are covered by    */
/* that scenario. 'E', 'P' and the $71C4 fallthrough are not, and cannot be   */
/* by replay: 'E' needs level 30, and no script in the game uses 'P'.         */
/*                                                                            */
/* On flags: as elsewhere in this file the routine leaves what the 6502 would */
/* leave, since generated callers can observe it. The exception is a load     */
/* immediately before a call that overwrites the flags anyway -- LDA/LDY into */
/* the ROM's HLINE and PLOT -- where only the register is set, matching what  */
/* game_cout_hook already does at $669F.                                      */
/* ========================================================================== */


/* ========================================================================== */
/* $6641, $660F, $6BEF, $6BDA, $71CD -- snake state and scoring setup.        */
/*                                                                            */
/* $624F/$6250 are the snake's head column and row; $6251/$6252 are the tail. */
/* $62D7 walks the head forward by adding the current direction's deltas from */
/* $6232/$6237 and SCRNs the result off the lo-res map (see the $7019 header).*/
/*                                                                            */
/* $0301 is the difficulty, 0-2, and $71CD is what gives it away: it indexes  */
/* the three-byte table at $71C8 -- $10, $15, $20 -- with $0301 and adds that */
/* entry to $71CB/$71CC once per level, in BCD. So an apple is worth          */
/* base[difficulty] * level, which is why $71CB read $15 throughout the       */
/* recordings: difficulty 1, level 1. It is also what decides the two         */
/* optional wall gaps in game_draw_playfield.                                 */
/* ========================================================================== */






/* ========================================================================== */
/* $7642, $64A9, $7633 -- apples, and the sound trick.                        */
/*                                                                            */
/* $6C49 is the game's mute switch, and it is a nice piece of work: every     */
/* sound routine reads `LDA $C000,Y` with Y = $6C49, and $6C2C picks either   */
/* $30 or $20 for it. $C030 is the speaker; $C020 is the cassette output. So  */
/* turning the sound off routes the identical click to a port nobody is       */
/* listening to, and the timing loops do not change at all -- no branch in    */
/* the hot path, and muting cannot alter the game's speed.                    */
/* ========================================================================== */




/* ========================================================================== */
/* $6217, $7590, $6B3D                                                        */
/* ========================================================================== */




/* ========================================================================== */
/* $6AB8 -- is the next cell in direction $6B38 worth moving into?            */
/*                                                                            */
/* The move validator behind the direction-choosing chain at $6A40, which     */
/* calls it once per candidate direction and branches on the answer. Two      */
/* separate rejections:                                                       */
/*                                                                            */
/*   the target cell itself must be empty or an apple ($0F). Anything else is */
/*   wall or snake, and it returns immediately.                               */
/*                                                                            */
/*   at least one of the target's four orthogonal neighbours must be empty.   */
/*   $6C4A counts them, and a target with none is a dead end -- legal to      */
/*   enter and fatal on the next move, so it is refused a step early.         */
/*                                                                            */
/* Returns A = 0 with Z set when the move is good, A = 1 when it is a dead    */
/* end, and simply Z clear on the early rejection. apple2tc drops the A       */
/* entirely -- its whole-program DCE can see that every caller branches on Z  */
/* alone -- but the 6502 sets it, so it is set here.                          */
/*                                                                            */
/* $6637/$6638 hold the target column and row: $6232[dir] + $624F and         */
/* $6237[dir] + $6250, the direction deltas added to the snake's head.        */
/* ========================================================================== */


/* ========================================================================== */
/* $64C8 -- move the bouncer one step.                                        */
/*                                                                            */
/* $6633/$6634 are its column and row, $6635/$6636 its per-axis deltas, and   */
/* $6637/$6638 the candidate cell it is trying to move to. A row of 0 means   */
/* it is not on the board, and both the entry test and the exit test use      */
/* that.                                                                      */
/*                                                                            */
/* The bounce is the neat part. A delta is +1 or -1, and `EOR #$FE` swaps     */
/* them: $01 ^ $FE = $FF, $FF ^ $FE = $01. So reflecting off a wall is one    */
/* instruction with no compare.                                               */
/*                                                                            */
/* Three cases when the diagonal target is occupied, distinguished by SCRNing */
/* the two cells either side of it:                                           */
/*                                                                            */
/*   the horizontal neighbour is blocked -- keep the column, flip $6635       */
/*   the vertical neighbour is blocked   -- keep the row, flip $6636          */
/*   neither is blocked, yet the diagonal is -- an inside corner. $6C4A is    */
/*   still zero, so $6530 undoes both axes and flips both deltas, sending it  */
/*   back the way it came.                                                    */
/* ========================================================================== */

void game_move_bouncer(Bouncer *b) {
  // Adapter. The body is bouncer_step() in game_native.c.
  //
  // Cost: the trace gives up every block head in here except $64C8's, which
  // stays below. The cycles do not move -- bouncer_step charges each block
  // with GAME_CYCLES -- so the frame hashes and the memory samples are
  // unaffected.
  //

  /*$64C8*/ TICK(12);

  // The state the original leaves behind: A holds the row it loaded first, and
  // the flags come from that load.

  bouncer_step(b);
}

/* ========================================================================== */
/* $728D, $6BFB                                                               */
/* ========================================================================== */



/* ========================================================================== */
/* $6594, $69C3                                                               */
/* ========================================================================== */



/* ========================================================================== */
/* $69A9, $75D1, $6C72 -- the rest of the input path.                         */
/*                                                                            */
/* All three are adapters now; the decodes live in game_native.c. They carry  */
/* most of what no recording exercises -- ESC and Ctrl-S, the arrow keys on   */
/* the redefinition screen, and the whole joystick branch -- so the comments  */
/* above the native routines, rather than probe-acceptance.sh's baseline      */
/* list, are what now record which blocks rest on the binary alone.           */
/* ========================================================================== */




/* ========================================================================== */
/* $6288 -- one life. See game_native.c for what it does.                     */
/* ========================================================================== */

/// $6288 -- play one life and record how it ended at $6253, which is where
/// $7739 reads it. What used to be an adapter is just this write-back now: no
/// return address, and A is not left holding the reason because nothing reads
/// it there any more.
static void game_play_one_life(void) {

  uint8_t cell = 0;
  switch (game_play_loop_native(&cell)) {
  case LIFE_GATE:
    s_life_outcome = 0x00;
    break;
  case LIFE_APPLE:
    s_life_outcome = 0x0f;
    break;
  case LIFE_QUIT:
    s_life_outcome = 0xff;
    break;
  case LIFE_TIMEOUT:
    s_life_outcome = 0xfe;
    break;
  case LIFE_CRASH:
    s_life_outcome = cell;
    break;
  }
}

/* ========================================================================== */
/* $72CE -- the status panel. See game_native.c.                              */
/* ========================================================================== */


/* ========================================================================== */
/* $78B3 -- the bonus screen. See game_native.c.                              */
/* ========================================================================== */


/* ========================================================================== */
/* $6256 -- start a life. See game_native.c.                                  */
/* ========================================================================== */


/* ========================================================================== */
/* $7980 -- the setup screen. See game_native.c.                              */
/* ========================================================================== */


/* ========================================================================== *
 * The top level                                                            *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte's top level, as C. Included only by snake-byte-cold.c.
///
/// Why this is not in game_native.c, where the rest of the converted game
/// lives: game_native.c is shared with snake-bytec1-ext, and that build still
/// has the generated func_t001 emitting every address in here. Two sources
/// claiming the same block head is exactly what probe-acceptance.sh's lint
/// rejects, and rightly -- in the ext build these addresses must keep their
/// probes, and they do, in the generated dispatch. Only the cold build has
/// replaced that dispatch, so only the cold build compiles this.
///
/// It goes away when snake-bytec1-ext does.


/* ========================================================================== */
/* $3750 and $7691 -- the top level                                           */
/*                                                                            */
/* The last of the decompiler's output, and the outermost loop in the game:   */
/* new game -> new level -> new round -> one life -> what happened -> repeat.  */
/* Everything it calls was converted long before it was.                      */
/*                                                                            */
/* The labels keep their addresses because this is a state machine, not a     */
/* nest of loops: a life ends by clearing the round, by dying, or by the      */
/* player quitting, and each re-enters at a different depth. As while-loops   */
/* it would need flags to say which level to break out to.                    */
/*                                                                            */
/* Decimal mode is explicit. The generated code emitted both a binary and a   */
/* decimal arm for every add and subtract because it could not prove which    */
/* way the D flag went; the assembly settles it, since the original brackets  */
/* each BCD run with SED/CLD. $7743-$7777 and $7817-$783A are decimal and     */
/* everything else is binary, so each operation is written the one way it     */
/* runs, and there is no D flag left to consult.                             */
/*                                                                            */
/* The ROM's own arms went with it, on an argument rather than a measurement:  */
/* every SED/CLD region in this file contains only adc_dec16, sbc_dec16 and    */
/* the bcd_* helpers -- which take their decimal-ness explicitly -- and no ROM */
/* call at all, and each clears D before it returns. So the monitor was only   */
/* ever entered in binary. game_bonus_screen looked like the counter-example   */
/* and is not: it is entered with D set, and its $78C7 CLD comes before it     */
/* prints anything. Measured as well: 3,942 D tests across both scenarios,     */
/* every one of them binary.                                                   */
/* ========================================================================== */

/// BCD add across a low/high pair, as SED/CLC/ADC/ADC leaves it.
static void bcd_add16(uint8_t at[2], uint8_t by) {
  uint16_t r = adc_dec16(at[0], by, 0);
  at[0] = (uint8_t)r;
  r = adc_dec16(at[1], 0x00, (uint8_t)(r >> 8) & 0x01);
  at[1] = (uint8_t)r;
}

/// BCD subtract across a low/high pair, as SED/SEC/SBC/SBC leaves it.
static void bcd_sub16(uint8_t at[2], uint8_t by) {
  uint16_t r = sbc_dec16(at[0], by, 1);
  at[0] = (uint8_t)r;
  r = sbc_dec16(at[1], 0x00, (uint8_t)(r >> 8) & 0x01);
  at[1] = (uint8_t)r;
}

void game_cold_start(void) {
  /* $3750 -- copy eight pages of level data from $3800 down to $1800. The
     original walks them by incrementing the operands of its own LDA and STA
     ($3754 and $3757), which is why it re-enters $3750 eight times rather
     than looping inside. The page counter is this loop's own, so the two
     patched bytes are written by nothing and read by nothing; they were
     mirrored back only for as long as the memory oracle hashed them. */
  for (unsigned page = 0; page != 8; ++page) {
    // Probed, not because anything needs to observe it, but because it is what
    // probe-acceptance.sh aligns the two builds on -- see GAME_CYCLES_ANCHOR.
    GAME_CYCLES_ANCHOR(0x3750, 2);
    for (unsigned i = 0; i != 256; ++i) {
      TICK(13);
      poke((uint16_t)(0x1800 + page * 256 + i), peek((uint16_t)(0x3800 + page * 256 + i)));
      if (i != 255)
        TICK(1);
    }
    TICK(20);
    if (page != 7)
      TICK(1);
  }

  TICK(6);
  rom_setvid();
  TICK(6);
  rom_setkbd();
  TICK(29);
  s_step_delay = 0x52;
  s_difficulty = 0x01;
  s_demo_mode = true; // so the first pass plays itself
  s_script_index = 0x01;
  s_level_time = 0x64;
  goto round;             // $3783: JMP $76C2

new_game: /* $7691 */
  TICK(6);
  game_setup_screen();
  TICK(6);
  game_promote_high_score();
  TICK(40);
  s_script_index = 0x01;
  s_level = 0x01;
  s_score[0] = 0x00;
  s_score[1] = 0x00;
  s_score[2] = 0x00;
  s_score[3] = 0x00;
  s_lives = 0x02;
  s_apples_afield[0] = 0x00;

new_level: /* $76B7 */
  TICK(14);
  s_lives_at_level_start = s_lives;
  s_apples_quota[0] = 0x10;

round: /* $76C2 */
  TICK(6);
  s_apples_quota[1] = 0x00;

start_round: /* $76C7 */
  TICK(40);
  s_apples_afield[0] = 0x00;
  s_apples_afield[1] = 0x00;
  s_apples_eaten[0] = 0x00;
  s_apples_eaten[1] = 0x00;
  s_apples_left[0] = s_apples_quota[0];
  s_apples_left[1] = s_apples_quota[1];
  game_draw_playfield_native();
  TICK(14);
  s_life_time = s_level_time;
  game_set_apple_value_native();
  TICK(14);
  io_peek(0xc054);       // page 1
  io_peek(0xc053);       // mixed text/graphics
  const Cell apple = game_place_apple_native();
  TICK(6);
  // $76F6 redraws it, at the cell game_place_apple just chose.
  game_plot_shape_native(0x09, apple);
  TICK(8);
  s_step_delay = 0x52;
  TICK(23);
  s_head_moved = false;
  s_life_timer = s_life_time;
  s_wndtop = 0x14; // window top, so HOME clears only the status panel
  rom_home();
  TICK(6);
  game_status_panel();
  TICK(6);
  game_begin_life();
  TICK(3);
  goto verdict; // $7716: JMP $7739 -- a fresh round asks the same question

life: /* $7719 */
  TICK(19);
  s_life_timer = s_life_time;
  s_wndtop = 0x14;
  rom_home();
  TICK(6);
  game_status_panel();
  TICK(8);
  if (s_step_delay >= 0x03) {
    // $7730 -- two steps faster each life, but never past 3.
    TICK(8);
    s_step_delay = (uint8_t)(s_step_delay - 2);
  } else {
    TICK(1);
  }
  TICK(6);
  game_play_one_life();

verdict: /* $7739 -- $6253 says how the life ended */
  TICK(8);
  if (s_life_outcome == 0x0f)
    goto ate_apple;
  TICK(3);
  TICK(4);
  if (s_life_outcome != 0x00)
    goto not_apple;
  goto round_cleared;

ate_apple: /* $773E */
  TICK(1);
  TICK(76);
  bcd_sub16(s_apples_afield, 0x01);
  bcd_sub16(s_apples_left, 0x01);
  bcd_add16(s_apples_eaten, 0x01);

  // $777B -- points only for the first $11 apples of the round. The high
  // byte must be zero and the low one below $11, both BCD.
  if (s_apples_eaten[1]) {
    TICK(1);
  } else {
    TICK(8);
    if (s_apples_eaten[0] >= 0x11) {
      TICK(1);
    } else {
      TICK(6);
      game_add_score_native();
    }
  }

  TICK(18);
  s_growth = (uint8_t)(s_growth + 0x0a); // ten more cells of snake

  // $7793 -- anything left in the round?
  if (s_apples_left[0]) {
    TICK(1);
    goto next_apple;
  }
  TICK(6);
  if (s_apples_left[1]) {
    TICK(1);
    goto next_apple;
  }

  /* $779A -- that was the last one. Draw the bar across the bottom, put the
     marker on it, and stop the clock for the run to the gate -- see
     s_life_time for why $FF stops it rather than lengthening it. */
  TICK(31);
  s_shape = 0x15;
  plot_hline_at(0x06, 0x12, 0x00, 0x16);
  TICK(16);
  plot_shape_at(0x15, 0x00, (Cell){.col = 0x14, .row = 0x00});
  TICK(14);
  s_life_time = 0xff;
  rom_setcol(0x00);
  TICK(10);
  rom_plot(0x00, 0x14);
  TICK(3);
  goto life;

next_apple: /* $77D0 -- place one only when both countdown bytes are zero */
  TICK(6);
  if (s_apples_afield[0]) {
    TICK(3);
    goto life;
  }
  TICK(1);
  TICK(6);
  if (s_apples_afield[1]) {
    TICK(3);
    goto life;
  }
  TICK(1);
  TICK(6);
  game_place_apple_native();
  TICK(3);
  goto life;

round_cleared: /* $77EA */
  TICK(32);
  {
    const uint16_t r = adc_dec16(s_level, 0x01, 0x00);
    s_level = (uint8_t)r;
  }
  s_script_index = (uint8_t)(s_script_index + 1);
  // $77F8 -- no life was lost this round, so it earns a bonus.
  if (s_lives == s_lives_at_level_start) {
    TICK(6);
    game_bonus_screen();
  } else {
    TICK(1);
  }
  TICK(6);
  game_award_extra_life_native();
  TICK(3);
  goto new_level;

not_apple: /* $77E8 */
  TICK(1);
  TICK(4);
  if (s_life_outcome != 0xfe) {
    TICK(1);
    goto ended;
  }
  TICK(6);
  if (s_apples_left[1]) {
    TICK(1);
    goto harder;
  }
  TICK(6);
  if (!s_apples_left[0]) {
    TICK(1);
    goto ended;
  }

harder: /* $7817 -- three more apples in the round, and three more to come */
  TICK(54);
  bcd_add16(s_apples_quota, 0x03);
  bcd_add16(s_apples_left, 0x03);
  game_place_apple_native();
  TICK(6);
  game_place_apple_native();
  TICK(6);
  game_place_apple_native();
  TICK(3);
  goto life;

ended: /* $7847 */
  TICK(8);
  if (s_life_outcome == 0xff) {
    TICK(3);
    goto new_game; // the player pressed the quit key
  }
  TICK(1);
  TICK(4);
  // $FE means the snake ran out of room rather than died, and that just starts
  // another life. Anything else falls through to the pause. Note the sense:
  // the original *branches away* when it is not $FE, so equality is the
  // fall-through, not the exception.
  if (s_life_outcome == 0xfe) {
    TICK(3);
    goto life;
  }
  TICK(1);
  TICK(6);
  if (s_demo_mode) {
    TICK(1);
    goto lose_life; // the demo does not wait to be told to carry on
  }

  /* $785D -- "PRESS SPACE BAR TO CONTINUE", then wait for space or the
     paddle button, whichever the setup screen selected. */
  TICK(16);
  s_cv = 0x17;
  s_ch = 0x00;
  game_print_inline_str(0x7867);
  for (;;) {
    TICK(6);
    if (s_joystick_selected) {
      TICK(6);
      // The button reads with bit 7 *clear* when pressed on this path.
      if (!(io_peek(0xc061) & 0x80)) {
        TICK(1);
        break;
      }
    } else {
      TICK(1);
    }
    // $7890 is on the replay coordinate -- see GAME_CYCLES_COORD.
    GAME_CYCLES_COORD(0x7890, 8);
    const uint8_t key = io_peek(0xc000);
    if (key == 0xa0) {
      TICK(4);
      io_poke(0xc010, key);
      break;
    }
    TICK(1);
  }

lose_life: /* $789A */
  TICK(6);
  if (!s_lives) {
    TICK(3);
    goto new_game;
  }
  TICK(1);
  TICK(19);
  {
    const uint16_t r = sbc_dec16(s_lives, 0x01, 0x01);
    s_lives = (uint8_t)r;
  }
  goto start_round;
}

/* ========================================================================== *
 * The machine state at $3750                                               *
 * ========================================================================== */

/* Generated by make-entry-state.sh. Do not edit. */
/* Snake Byte's machine state at $3750, captured from snake-bytec1-ext-run. */

/* RAM $0000-$0802. Outside this: the game image, the ROM, and $FF. */
#define SB_ENTRY_RAM_LEN 0x803
static const uint8_t kSnakeByteEntryRam[SB_ENTRY_RAM_LEN] = {
  76, 60, 212, 76, 58, 219, 255, 255, 255, 255, 76, 153, 225, 255, 0, 107,
  255, 0, 255, 4, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  0, 40, 0, 24, 0, 3, 255, 255, 128, 5, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 221, 255, 40, 240, 253, 27, 253, 255, 255, 255, 255, 0, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 255, 255, 255, 255, 10, 0,
  80, 55, 85, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 56, 255,
  255, 255, 255, 255, 255, 255, 255, 1, 8, 3, 8, 3, 8, 3, 8, 0,
  192, 255, 255, 0, 192, 255, 255, 255, 255, 255, 0, 255, 255, 0, 8, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 255, 255, 255, 255, 3,
  76, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 142, 0, 0,
  55, 80, 0, 0, 0, 142, 221, 64, 0, 0, 0, 0, 0, 10, 255, 3,
  8, 230, 184, 208, 2, 230, 185, 173, 6, 2, 201, 58, 176, 10, 201, 32,
  240, 239, 56, 233, 48, 56, 233, 208, 96, 128, 79, 199, 82, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 0, 255, 127, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 1, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 101, 235, 216, 220, 236, 99, 236,
  151, 14, 236, 90, 231, 218, 241, 34, 216, 193, 241, 0, 1, 1, 168, 250,
  140, 49, 52, 49, 54, 48, 0, 49, 0, 48, 0, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  89, 250, 3, 224, 69, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 193,
  208, 208, 204, 197, 160, 221, 219, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  221, 195, 193, 204, 204, 160, 177, 180, 177, 182, 176, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 193, 255, 255, 255, 255, 255, 255, 255,
  0, 0, 0,
};

#define SB_ENTRY_A 0x37
#define SB_ENTRY_X 0x9D
#define SB_ENTRY_Y 0x50
#define SB_ENTRY_SP 0xF6
#define SB_ENTRY_STATUS 0x00
#define SB_ENTRY_VID_CONTROL 0x01

/* ========================================================================== *
 * Startup                                                                  *
 * ========================================================================== */

void init_emulated(void);

void init_emulated(void) {
  /* Loads the game image at $3750-$854E, and the $00B1-$00C8 zero-page segment
     the run data carries. */
  load_images();

  /* Unwritten RAM reads as $FF on a real machine, and the game does read some
     of it: game_rand_byte_native walks a pointer up from $1800 looking for a
     byte with bit 7 clear, and against a zero-filled machine it would find one
     immediately and never move. The generated init zeroes, so restore $FF --
     but only outside the game image, which is already in place above.

     $1800-$1FFF is included: $3750's first act is to copy the level data over
     it, so what is there beforehand does not matter, and $FF is what the
     machine would have had. */
  for (unsigned a = 0x0803; a != 0x3750; ++a)
    ram_poke((uint16_t)a, 0xFF);
  for (unsigned a = 0x854F; a != 0xC000; ++a)
    ram_poke((uint16_t)a, 0xFF);

  /* Then the state the boot itself produced: zero page, the stack and its live
     contents, the $03xx vectors, and the text screen. This comes last because
     it must win over both of the above. */
  for (unsigned i = 0; i != SB_ENTRY_RAM_LEN; ++i)
    ram_poke((uint16_t)i, kSnakeByteEntryRam[i]);

  /* The plotter's block used to be $0000-$0008 and is a C object now, so the
     line above no longer initialises it. Take it from the same snapshot, or
     the first read before any write would see zero where the booting build
     sees what BASIC left there. */
  s_shape = kSnakeByteEntryRam[0x00];
  s_ch = kSnakeByteEntryRam[0x24];
  s_cv = kSnakeByteEntryRam[0x25];
  s_wndlft = kSnakeByteEntryRam[0x20];
  s_wndwdth = kSnakeByteEntryRam[0x21];
  s_wndtop = kSnakeByteEntryRam[0x22];
  s_wndbtm = kSnakeByteEntryRam[0x23];
  s_gbas = (uint16_t)(kSnakeByteEntryRam[0x26] | (kSnakeByteEntryRam[0x27] << 8));
  s_bas = (uint16_t)(kSnakeByteEntryRam[0x28] | (kSnakeByteEntryRam[0x29] << 8));
  s_bas2 = (uint16_t)(kSnakeByteEntryRam[0x2a] | (kSnakeByteEntryRam[0x2b] << 8));
  s_h2 = kSnakeByteEntryRam[0x2c];
  s_v2 = kSnakeByteEntryRam[0x2d];
  s_mask = kSnakeByteEntryRam[0x2e];
  s_color = kSnakeByteEntryRam[0x30];
  s_invflg = kSnakeByteEntryRam[0x32];
  s_csw = (uint16_t)(kSnakeByteEntryRam[0x36] | (kSnakeByteEntryRam[0x37] << 8));
  s_ksw = (uint16_t)(kSnakeByteEntryRam[0x38] | (kSnakeByteEntryRam[0x39] << 8));
  s_a2l = kSnakeByteEntryRam[0x3e];

  /* Registers. SP matters most -- the live stack bytes above are meaningless
     without it. The flags are $A0: N set, everything else clear.

     The carry is loaded with the rest even though nothing reads it before a
     ROM routine overwrites it: this block is the machine's state at $3750, and
     leaving one member out to save a line would make it something else. The
     ten *residue* carry writes -- game routines setting it because the
     original left it set -- are a different thing and are gone. */
  // No registers are loaded: there are none left, and the emulated stack went
  // with them -- every PHA/PLA in this file bracketed a call that could not
  // reach the C local it was protecting.

  /* The soft switches, which no RAM image carries. $01 is text, page 1, not
     mixed -- the power-on state, so these reads are asserting it rather than
     changing it. Reading a soft switch is what selects it. */
  (void)SB_ENTRY_VID_CONTROL;
  io_peek(0xc051); /* text */
  io_peek(0xc052); /* not mixed */
  io_peek(0xc054); /* page 1 */

  /* The keyboard, which is neither RAM nor a soft switch and is the last piece
     of state the boot leaves behind. Two things to get right.

     First, $C000 does not read zero on an Apple II that has had a key pressed:
     it reads the last key with bit 7 clear, and the boot's final act was the
     Return that submitted CALL 14160.

     Second, and only visible because the traces diverged: a2host pushes a dummy
     Return into the queue whenever a key source is present, because the first
     key pressed before initialization would otherwise be lost. A booting
     program never notices -- KEYIN swallows it at the prompt. This one would
     ingest it as the player's first keystroke. It is pushed before
     init_emulated() precisely so it can be consumed here.

     Draining the queue and then latching $0D does both at once. */
  while (a2_io_keys_count(a2host_io()) != 0)
    io_poke(0xc010, 0);
  a2_io_push_key(a2host_io(), 0x0d);
  io_peek(0xc000);
  io_poke(0xc010, 0);
}

