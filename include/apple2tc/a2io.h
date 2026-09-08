/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/soundqueue.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// A helper function to decode the text mode screen. The callback is invoked
/// consecutively with every character starting from top left.
void apple2_decode_text_screen(
    const uint8_t *pageStart,
    void *ctx,
    void (*drawGlyph)(void *ctx, uint8_t ch, unsigned x, unsigned y));

#define A2_SCREEN_W 280
#define A2_SCREEN_H 192
#define A2_SCREEN_W_POT 512
#define A2_SCREEN_H_POT 256

typedef struct a2_rgba8 {
  uint8_t r, g, b, a;
} a2_rgba8;

/// RGB encoding of the Apple2 screen.
typedef struct a2_screen {
  a2_rgba8 data[A2_SCREEN_W_POT * A2_SCREEN_H_POT];
} a2_screen;

/// Render the text page pointed by pageStart into a RGB screen.
/// \param ms - millisecond since hardware reset. This is used to determine the
///     blink phase.
void apple2_render_text_screen(const uint8_t *pageStart, a2_screen *screen, uint64_t ms);

/// Render the lowres graphics (GR) page pointed by pageStart into a RGB screen.
/// \param ms - millisecond since hardware reset. This is used to determine the
///     blink phase. Only needed if mixed == true.
void apple2_render_gr_screen(const uint8_t *pageStart, a2_screen *screen, uint64_t ms, bool mixed);

/// How a hi-res (HGR) page is turned into pixels. The Apple II's 7.16 MHz dot
/// clock is exactly twice the 3.58 MHz NTSC colourburst, so two adjacent dots
/// make one colour clock and the true colour resolution is 140, not 280.
typedef enum {
  A2_HGR_COLOR, ///< Per-dot artifact approximation; what the GUI has always drawn.
  A2_HGR_MONO, ///< 1-bit white on black.
  A2_HGR_COLOR140, ///< Decoded at colour-clock resolution: one pixel per cell,
                   ///< so apple2_render_hgr_screen_mode() only fills the first
                   ///< 140 columns of each row for this mode. A caller that
                   ///< wants the traditional 280-wide image doubles each
                   ///< column back out itself.
  A2_HGR_MONO140, ///< Ink coverage at colour-clock resolution: a cell is white if
                  ///< either of its two dots is set, else black. A colour fill is
                  ///< an alternating dot pattern, so this is the occupancy mask
                  ///< that renders as a solid region instead of A2_HGR_MONO's comb.
                  ///< Like A2_HGR_COLOR140, one pixel per cell: only the first
                  ///< 140 columns of each row are filled.
} a2_hgr_mode_t;

/// Render the hires graphics (GR) page pointed by pageStart into a RGB screen.
/// \param textPageStart - the start of the text page. Only used if mixed == true.
/// \param ms - millisecond since hardware reset. This is used to determine the
///     blink phase. Only needed if mixed == true.
/// \param mono - if true, render 1-bit white-on-black instead of colour. A thin
///     wrapper around apple2_render_hgr_screen_mode(), kept so existing callers
///     (lib/a2host/a2host_gui.c, lib/a2mcp) do not have to change.
void apple2_render_hgr_screen(
    const uint8_t *grPageStart,
    const uint8_t *textPageStart,
    a2_screen *screen,
    uint64_t ms,
    bool mixed,
    bool mono);

/// Render the hires graphics (HGR) page pointed by pageStart into a RGB
/// screen, selecting one of the a2_hgr_mode_t render modes. A2_HGR_COLOR140
/// and A2_HGR_MONO140 fill only the first 140 columns of each row -- see
/// their a2_hgr_mode_t comments; every other mode fills all 280.
/// \param textPageStart - the start of the text page. Only used if mixed == true.
/// \param ms - millisecond since hardware reset. This is used to determine the
///     blink phase. Only needed if mixed == true.
void apple2_render_hgr_screen_mode(
    const uint8_t *grPageStart,
    const uint8_t *textPageStart,
    a2_screen *screen,
    uint64_t ms,
    bool mixed,
    a2_hgr_mode_t mode);

/// Expand a cell-resolution render (A2_HGR_COLOR140 / A2_HGR_MONO140), which
/// filled only the first 140 columns of each row, back out to the
/// traditional 280-wide image by doubling each column. Works in place, right
/// to left within a row, so a column is not overwritten before it is read.
///
/// \param mixed - must match the `mixed` passed to the
///     apple2_render_hgr_screen_mode() call that produced \p screen. In
///     mixed mode that renderer only fills the graphics area at cell
///     resolution up to its own `end_line` (160, not 192) -- rows 160-191
///     are the 4-line text window, already drawn at full 280-column
///     resolution by draw_glyph_cb(). This function must stop at that same
///     boundary: doubling the text rows too would stretch them to twice
///     their width and show only their left half. Do not "simplify" this
///     into a whole-screen loop.
///
/// This lives in the library, not a front end, because more than one front
/// end (lib/a2mcp and a2emu) needs the exact same operation.
void apple2_widen_hgr_cells(a2_screen *screen, bool mixed);

typedef struct {
  /// Queue of float sound samples. Filled by the main thread, read by the
  /// sound callback.
  sound_queue_t sq;

  /// On web the sound callback may not activate until the user interacts
  /// with the page. We keep track of that to avoid filling the sound queue.
  atomic_bool cb_running;

  /// The cycle count for the previous speaker access. This allows us
  /// to detect cycle overflow/wraparound.
  unsigned last_cycle;
  /// After cycle wraparound, this keeps the high part that has been "lost".
  uint64_t cycle_base;
  /// The last speaker cycle we generated sound for.
  double last_generated_cycle;
  /// The last speaker state.
  float last_state;
} a2_sound_t;

void a2_sound_init(a2_sound_t *sound);
void a2_sound_done(a2_sound_t *sound);

/// The speaker bit has been accessed at cycle \p cycle.
void a2_sound_spkr(a2_sound_t *sound, unsigned cpu_freq, unsigned audio_rate, unsigned cycle);
/// Submit the sound generated up to cycle \p cycle.
void a2_sound_submit(a2_sound_t *sound, unsigned cpu_freq, unsigned audio_rate, unsigned cycle);
/// The asynchronous sound callback. Needs to populate the specified buffer with samples.
void a2_sound_cb(a2_sound_t *sound, float *buffer, unsigned num_frames, unsigned num_channels);

enum {
  A2_KBD_QUEUE_SIZE = 32,
  A2_DEBUG_IO1 = 1,
  A2_DEBUG_IO2 = 2,

  /// When set, text, otherwise graphics (default set).
  A2_VC_TEXT = 1,
  /// When set, mixed graphics with 4-lines of text below (default clr).
  A2_VC_MIXED = 2,
  /// When set, display page 2 (default clr).
  A2_VC_PAGE2 = 4,
  /// When set, high resolution graphics (default clr),
  A2_VC_HIRES = 8,
};

typedef enum { A2_VIDMODE_TEXT, A2_VIDMODE_GR, A2_VIDMODE_HGR } a2_vidmode_t;

/// Disk II drive state.
typedef struct {
  uint8_t *nib_data;      ///< Nibblized track data (all 35 tracks).
  uint32_t position;      ///< Byte offset within current track.
  bool write_protected;
  bool mounted;
  bool skip;              ///< Timing toggle for read.
  int current_track;      ///< 0-34.
} a2_disk2_drive_t;

/// Disk II controller state.
typedef struct {
  uint8_t data_register;      ///< 74LS323 shift register.
  bool q6, q7;                ///< Mode latches.
  bool motor_on;
  uint64_t motor_off_cycle;   ///< Cycle when motor was turned off.
  unsigned last_nibble_cycle;  ///< Cycle when last nibble byte was clocked in.
  int selected_drive;         ///< 0 or 1.
  uint8_t phases;             ///< Bitmask of active phase magnets.
  int phase_position;         ///< Half-track 0-69.
  a2_disk2_drive_t drive[2];
} a2_disk2_t;

typedef struct {
  /// The keyboard latch: bits 0-6 of what $C000 reads. The Apple II keyboard
  /// is one byte plus one flag and no queue -- the encoder writes each new
  /// keycode straight into the byte and re-asserts the strobe, whether or not
  /// the program acknowledged the previous key. That is what makes "read
  /// $C000, act on the keys you recognise, clear the strobe only for those" a
  /// working idiom rather than a way to wedge the keyboard, and games rely on
  /// it.
  uint8_t latch;
  /// Bit 7 of what $C000 reads: set while the latch holds a key the program
  /// has not acknowledged through KBDSTRB.
  bool strobe;

  /// Type-ahead staging, which is not hardware. It exists for text typed
  /// *into* the machine -- --kbd-file, a program's own startup keys -- where
  /// every character has to arrive however slowly the program reads, instead
  /// of collapsing the way a burst of real keypresses does. Characters wait
  /// here and enter the latch as the program frees it. A keypress
  /// (a2_io_press_key) does not use it: a keypress happens at a moment in
  /// time and goes straight to the latch.
  ///
  /// Invariant: keys_count is zero unless strobe is set. Nothing waits here
  /// while the latch is free -- a2_io_push_key() latches instead of staging,
  /// and KBDSTRB refills the latch as it clears the strobe -- which is what
  /// makes a2_io_keys_count() a count of keys rather than of places.
  uint8_t keys[A2_KBD_QUEUE_SIZE];
  unsigned keys_head;
  unsigned keys_count;
  /// Video control status.
  uint8_t vid_control;
  /// Number of times A2_VC_PAGE2 has actually changed value, counted at the
  /// soft switch (LOWSCR/HISCR) in a2_io_peek() -- not the number of times
  /// either switch was accessed, which is routine and far more frequent than
  /// an actual page flip. A double-buffered program presenting a finished
  /// frame is exactly a page-2 flip, which is what this exists to detect.
  unsigned page_flips;
  /// Callback when speaker is accessed.
  void *spkr_cb_ctx;
  void (*spkr_cb)(void *ctx, unsigned cycles);
  /// Debug flags.
  uint8_t debug;
  /// Cycles since power-on, extended past the 32 bits a2_io_peek() receives.
  ///
  /// The paddle one-shots are timed against this rather than against the raw
  /// stamp because 32-bit arithmetic cannot express "already expired" for an
  /// arbitrarily old deadline. An absolute `expiry` compared with
  /// `(int32_t)(cycles - expiry) < 0` reports a one-shot as still high on a
  /// machine that never triggered one, and revives every expired timer about
  /// 35 minutes on; measuring elapsed time from a 32-bit trigger instead only
  /// moves the revival to exactly one full wrap. Widening the clock removes
  /// the class.
  uint64_t clock;
  /// The last 32-bit stamp seen, which is what `clock` is extended from.
  unsigned clock_last;
  /// Disk II controller state.
  a2_disk2_t disk2;
} a2_iostate_t;

void a2_io_init(a2_iostate_t *io);
void a2_io_done(a2_iostate_t *io);
static inline void a2_io_set_spkr_cb(
    a2_iostate_t *io,
    void *spkr_cb_ctx,
    void (*spkr_cb)(void *ctx, unsigned cycles)) {
  io->spkr_cb_ctx = spkr_cb_ctx;
  io->spkr_cb = spkr_cb;
}
/// A key was pressed at this instant. It overwrites the latch and asserts the
/// strobe, discarding whatever was there -- which is what the hardware does,
/// and the only faithful model for a real keystroke or for a cycle-stamped
/// replay of one. Use this for anything that happens at a moment in time, and
/// a2_io_push_key() for text being typed in.
void a2_io_press_key(a2_iostate_t *io, uint8_t key);
/// Stage one character of type-ahead: into the latch if the program has
/// acknowledged what was there, otherwise behind it. Return false if the
/// staging queue was full.
bool a2_io_push_key(a2_iostate_t *io, uint8_t key);
/// Stage either the entire string or nothing. Return true on success.
bool a2_io_push_str(a2_iostate_t *io, const char *str);
/// Return the number of keys the machine has yet to acknowledge: the one in
/// the latch, if the strobe is still set, plus anything staged behind it.
static inline unsigned a2_io_keys_count(const a2_iostate_t *io) {
  return (io->strobe ? 1u : 0u) + io->keys_count;
}
/// Return the number of characters that can still be staged as type-ahead.
static inline unsigned a2_io_keys_expect(const a2_iostate_t *io) {
  return A2_KBD_QUEUE_SIZE - io->keys_count;
}
uint8_t a2_io_peek(a2_iostate_t *io, uint16_t addr, unsigned cycles);
void a2_io_poke(a2_iostate_t *io, uint16_t addr, uint8_t value, unsigned cycles);

/// Advance the extended clock without performing an IO access.
///
/// `clock` is exact only while consecutive stamps are **strictly less than**
/// 2^32 cycles apart -- a gap of exactly one wrap presents the same value and
/// yields a delta of zero -- and arrive in execution order, equal stamps
/// allowed. A reset of the CPU counter must be paired with a2_io_init(), so
/// the restart is not mistaken for a wrap.
///
/// Soft-switch accesses advance it on their own. This exists so a host can
/// guarantee the bound for a program that touches no soft switch for a long
/// time: a2host calls it once per frame, and a frame is ~17,000 cycles.
void a2_io_tick(a2_iostate_t *io, unsigned cycles);

/// Initialize disk2 controller state.
void a2_disk2_init(a2_disk2_t *disk);
/// Free disk2 controller state.
void a2_disk2_done(a2_disk2_t *disk);
/// Mount a DSK/DO format disk image (143360 bytes) on drive 0 or 1.
/// The data is copied and nibblized internally. Returns false on error.
bool a2_disk2_mount(a2_disk2_t *disk, int drive_num, const uint8_t *dsk_data, size_t size);
/// Unmount a drive.
void a2_disk2_unmount(a2_disk2_t *disk, int drive_num);
/// Handle a read from disk II soft switch ($C0E0-$C0EF). offset is 0x0-0xF.
uint8_t a2_disk2_peek(a2_disk2_t *disk, unsigned offset, unsigned cycles);
/// Handle a write to disk II soft switch ($C0E0-$C0EF). offset is 0x0-0xF.
void a2_disk2_poke(a2_disk2_t *disk, unsigned offset, uint8_t value, unsigned cycles);
/// Read from Disk II boot ROM ($C600-$C6FF). addr is 0x00-0xFF.
uint8_t a2_disk2_rom_peek(unsigned addr);
/// Copy the Disk II P5 boot ROM into ram at $C600-$C6FF.
void a2_disk2_install_rom(uint8_t *ram);
/// Clear the Disk II P5 boot ROM from ram at $C600-$C6FF.
void a2_disk2_remove_rom(uint8_t *ram);

/// Decode and return the vid control bits as a video mode.
static inline a2_vidmode_t a2_io_get_vidmode(const a2_iostate_t *io) {
  return io->vid_control & A2_VC_TEXT ? A2_VIDMODE_TEXT
      : io->vid_control & A2_VC_HIRES ? A2_VIDMODE_HGR
                                      : A2_VIDMODE_GR;
}

/// Return true if this is a mixed graphics mode (graphics with 4 lines of
/// text in the bottom).
static inline bool a2_io_is_vidmode_mixed(const a2_iostate_t *io) {
  return (io->vid_control & (A2_VC_TEXT | A2_VC_MIXED)) == A2_VC_MIXED;
}

/// Decode the vid ctrl bits to return an actuve page number: 0 or 1.
static inline uint8_t a2_io_get_vidpage_index(const a2_iostate_t *io) {
  return (io->vid_control & A2_VC_PAGE2) ? 1 : 0;
}

/// Return the starting offset of the active Hires page.
uint16_t a2_io_get_hires_page_offset(const a2_iostate_t *io);
/// Return the starting address of the active text page.
uint16_t a2_io_get_text_page_offset(const a2_iostate_t *io);

#ifdef __cplusplus
}
#endif
