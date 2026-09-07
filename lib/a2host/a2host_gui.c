/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The windowed front end: a sokol app around `a2host`.
///
/// Everything graphical or audible lives here, which is what keeps `a2host`
/// itself free of sokol so a console front end can link it alone. The division
/// is not arbitrary -- it falls exactly where the wall clock does. Rendering
/// needs one for the text blink phase, and real-time pacing needs one; replay
/// and frame hashing must not have one, or they would not be reproducible.

#include "apple2tc/a2host_gui.h"
#include "apple2tc/a2host_api.h"

#include "apple2tc/a2io.h"
#include "apple2tc/apple2iodefs.h"
#include "apple2tc/sokol/sokol_app.h"
#include "apple2tc/sokol/sokol_audio.h"
#include "apple2tc/sokol/sokol_gfx.h"
#include "apple2tc/sokol/sokol_glue.h"
#include "apple2tc/sokol/sokol_time.h"
#include "apple2tc/system.h"

#include "apple2tc/sokol/blit.h"

#include "probe_internal.h" // probe_stop_requested(), consulted in frame_cb below

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static sg_bindings bind_;
static sg_pipeline pip_;
static a2_sound_t sound_;
static a2_screen screen_;

/// Hi-res render mode, cycled at runtime with F3. A2_HGR_COLOR (the per-dot
/// artifact approximation the GUI has always drawn) is the default and must
/// stay it, so a2emu looks exactly as it always has until a key is pressed.
/// A2_HGR_COLOR, and deliberately so. A2_HGR_COLOR140 decodes at the resolution
/// the colour information actually has and is easier to read, but a real Apple II
/// on a composite monitor showed the fringing and the striping that the per-dot
/// approximation reproduces. Those artifacts are the original look rather than a
/// rendering defect, and some games are drawn to exploit them, so the emulator
/// defaults to the approximation and offers the clean decode on F3.
static a2_hgr_mode_t hgrMode_ = A2_HGR_COLOR;

/// Phosphor persistence: default off, toggled with F4, decay adjusted with
/// F5/F6. See apply_persistence() below for what it does and why it is not
/// the same thing as a2mcp's `blend`.
static bool persistEnabled_ = false;
static float persistDecay_ = 0.75f;
/// The decayed composite persistence maintains, same size as screen_ and
/// updated in place by apply_persistence(). A front end without persistence
/// on never touches this.
static a2_screen persistBuf_;

/// stm_now() at the start of every frame.
static uint64_t curFrameTick_ = 0;
static uint64_t lastRunTick_ = 0;
static uint64_t firstFrameTick_ = 0;
static bool haveFirstTick_ = false;

/// KBD handling.
/// If set to a valid character, the next character will be ignored, if it
/// matches this value. Next character, whatever it is, always clears this.
/// This is used on same platforms where some keys like ENTER arrive both as
/// characters and as keydown events.
static int ignoreNextCh_ = -1;

static a2host_gui_event_hook event_hook_ = NULL;

void a2host_gui_set_event_hook(a2host_gui_event_hook hook) {
  event_hook_ = hook;
}

/// Submit the audio produced during the frame that just ran.
static void frame_done(void) {
  if (a2host_sound_enabled())
    a2_sound_submit(&sound_, A2_CLOCK_FREQ, saudio_sample_rate(), get_cycles());
}

static void init_window(void) {
  sg_desc desc = {.context = sapp_sgcontext()};
  sg_setup(&desc);

  sg_image_desc idesc = {
      .width = A2_SCREEN_W_POT,
      .height = A2_SCREEN_H_POT,
      .usage = SG_USAGE_STREAM,
      .min_filter = SG_FILTER_LINEAR,
      .mag_filter = SG_FILTER_LINEAR,
      .label = "a2_image",
  };
  bind_.fs_images[SLOT_tex] = sg_make_image(&idesc);

  /*
   * Triangle strip:
   *    2  |  0
   * ------+------
   *    3  |  1
   */
  static const float U = (float)A2_SCREEN_W / A2_SCREEN_W_POT;
  static const float V = (float)A2_SCREEN_H / A2_SCREEN_H_POT;
  static const float vertices[][4] = {
      {1, 1, U, 0},
      {1, -1, U, V},
      {-1, 1, 0, 0},
      {-1, -1, 0, V},
  };
  sg_buffer_desc bdesc = {
      .data = SG_RANGE(vertices),
      .label = "rect vertices",
  };
  bind_.vertex_buffers[0] = sg_make_buffer(&bdesc);

  sg_shader blit = sg_make_shader(blit_shader_desc(sg_query_backend()));

  sg_pipeline_desc pdesc = {
      .shader = blit,
      .layout =
          {.attrs =
               {
                   [ATTR_vs_pos].format = SG_VERTEXFORMAT_FLOAT2,
                   [ATTR_vs_texcoord0].format = SG_VERTEXFORMAT_FLOAT2,
               }},
      .primitive_type = SG_PRIMITIVETYPE_TRIANGLE_STRIP,
      .label = "rect pipeline",
  };
  pip_ = sg_make_pipeline(&pdesc);
}

/// Callback from a2_io_t, when speaker has been flipped.
static void speaker_cb(void *ctx, unsigned cycles) {
  a2_sound_spkr((a2_sound_t *)ctx, A2_CLOCK_FREQ, saudio_sample_rate(), cycles);
}

/// Callback from saudio to generate new sound samples.
static void stream_userdata_cb(float *buffer, int num_frames, int num_channels, void *user_data) {
  a2_sound_cb((a2_sound_t *)user_data, buffer, num_frames, num_channels);
}

static void init_cb(void) {
  init_window();
  stm_setup();

  hgrMode_ = a2host_initial_hgr_mode();
  persistEnabled_ = a2host_initial_persist_enabled();
  persistDecay_ = a2host_initial_persist_decay();

  a2_sound_init(&sound_);
  a2host_init_emulation();
  a2_io_set_spkr_cb(a2host_io(), &sound_, speaker_cb);

  if (a2host_sound_enabled()) {
    saudio_desc audioDesc = {
        .num_channels = 1,
        .stream_userdata_cb = stream_userdata_cb,
        .user_data = &sound_,
    };
    saudio_setup(&audioDesc);
  }
}

static void cleanup_cb(void) {
  a2host_shutdown();
  sg_shutdown();
  if (a2host_sound_enabled())
    saudio_shutdown();
  a2_sound_done(&sound_);
}

/// Name shown in the window title when a display option changes; see
/// report_display_state().
static const char *hgr_mode_name(a2_hgr_mode_t mode) {
  switch (mode) {
  case A2_HGR_COLOR:
    return "color";
  case A2_HGR_COLOR140:
    return "color140";
  case A2_HGR_MONO:
    return "mono";
  case A2_HGR_MONO140:
    return "mono140";
  default:
    return "?";
  }
}

/// The window title is the only status display this front end has, so every
/// key that changes a display option re-derives it and calls this. Never
/// called from init_cb(), so the title is exactly a2host_argv0() -- as it has
/// always been -- until a key is pressed.
static void report_display_state(void) {
  char title[128];
  if (persistEnabled_) {
    snprintf(
        title,
        sizeof(title),
        "%s -- HGR:%s Persist:on(%.2f)",
        a2host_argv0(),
        hgr_mode_name(hgrMode_),
        (double)persistDecay_);
  } else {
    snprintf(
        title, sizeof(title), "%s -- HGR:%s Persist:off", a2host_argv0(), hgr_mode_name(hgrMode_));
  }
  sapp_set_window_title(title);
}

/// Phosphor-persistence pass: `persist = max(newly_rendered, persist * decay)`,
/// per pixel, per colour channel, over the whole rendered buffer -- it runs on
/// the pixels update_screen() just produced, not on page bits, so it applies
/// the same way in TEXT, GR and HGR with no mode restriction. The result
/// replaces screen_ so update_screen_image() uploads the composite.
///
/// This is deliberately not what a2mcp's `blend` does. `blend` ORs page bits
/// so every composited frame contributes at full brightness, which is right
/// for a script reading an occupancy mask. This fades instead, because that is
/// what a CRT's phosphor did to a bit that switched off -- dim toward black,
/// not stay lit. Do not "unify" the two; they exist to answer different
/// questions and look similar only by coincidence.
///
/// \p decayThisFrame is false when the frame loop simulated zero emulated
/// frames this repaint (a fast monitor repainting between two due emulated
/// frames): the machine did not advance, so the image should not decay either,
/// which this implements by using a decay factor of 1 -- max(newly_rendered,
/// persist) with newly_rendered identical to last time leaves persist exactly
/// as it was.
///
/// The opposite imbalance -- a catch-up repaint where `due` is several
/// emulated frames at once (frame_cb's loop ran more than one
/// a2host_simulate_frame() before this call) -- is not specially handled:
/// decay still runs exactly once here, so a catch-up fades less than that many
/// real frames would have. Left alone deliberately: it is rare (only after a
/// stall) and cosmetic, and correcting it would mean threading `due` itself
/// through rather than just whether it was zero.
static void apply_persistence(bool decayThisFrame) {
  const float decay = decayThisFrame ? persistDecay_ : 1.0f;
  const uint8_t *newPix = (const uint8_t *)screen_.data;
  uint8_t *persistPix = (uint8_t *)persistBuf_.data;
  for (size_t i = 0, n = sizeof(screen_.data); i != n; ++i) {
    uint8_t v = persistPix[i];
    uint8_t d = (uint8_t)(v * decay + 0.5f);
    // Rounding must not stall the decay: for decay >= 0.5, v * decay + 0.5
    // rounds a small v (e.g. 1) right back to v, so without this guard a
    // pixel would decay from 255 down to 1 and then sit there forever
    // instead of reaching 0. Any *actual* decay (decayThisFrame, decay < 1)
    // must make progress; decay == 1 (decayThisFrame false, i.e. due == 0)
    // is not decay at all and must leave v untouched, so the guard is
    // conditioned on decayThisFrame rather than firing on d >= v alone.
    if (decayThisFrame && v && d >= v)
      d = v - 1;
    persistPix[i] = newPix[i] > d ? newPix[i] : d;
  }
  memcpy(screen_.data, persistBuf_.data, sizeof(screen_.data));
}

/// \p decayThisFrame -- see apply_persistence().
static void update_screen(bool decayThisFrame) {
  // Milliseconds since hw reset. Used to determine blink phase.
  uint64_t ms = (uint64_t)stm_ms(stm_diff(curFrameTick_, firstFrameTick_));

  switch (a2_io_get_vidmode(a2host_io())) {
  case A2_VIDMODE_TEXT:
    apple2_render_text_screen(get_ram() + a2_io_get_text_page_offset(a2host_io()), &screen_, ms);
    break;
  case A2_VIDMODE_GR:
    apple2_render_gr_screen(
        get_ram() + a2_io_get_text_page_offset(a2host_io()),
        &screen_,
        ms,
        a2_io_is_vidmode_mixed(a2host_io()));
    break;
  case A2_VIDMODE_HGR:
  default:;
    bool mixed = a2_io_is_vidmode_mixed(a2host_io());
    apple2_render_hgr_screen_mode(
        get_ram() + a2_io_get_hires_page_offset(a2host_io()),
        get_ram() + a2_io_get_text_page_offset(a2host_io()),
        &screen_,
        ms,
        mixed,
        hgrMode_);
    // A2_HGR_COLOR140 and A2_HGR_MONO140 only fill the first 140 columns of
    // each row; widen them back to 280, skipping the mixed-mode text window
    // the renderer already drew at full width. Every other mode fills all 280
    // itself.
    if (hgrMode_ == A2_HGR_COLOR140 || hgrMode_ == A2_HGR_MONO140)
      apple2_widen_hgr_cells(&screen_, mixed);
    break;
  }

  if (persistEnabled_)
    apply_persistence(decayThisFrame);
}

static void update_screen_image(void) {
  sg_image_data imgData = {.subimage[0][0] = {.ptr = screen_.data, .size = sizeof(screen_.data)}};
  sg_update_image(bind_.fs_images[SLOT_tex], &imgData);
}

static void frame_cb(void) {
  curFrameTick_ = stm_now();
  if (!haveFirstTick_) {
    haveFirstTick_ = true;
    firstFrameTick_ = curFrameTick_;
    // Or the first repaint's debt is measured from tick zero and the machine
    // opens by running the catch-up cap.
    lastRunTick_ = curFrameTick_;
  }

  // The elapsed time goes over by value, here, while it is still true. The
  // host owns the pacing; this front end only owns the clock.
  const unsigned due = a2host_begin_repaint(stm_sec(stm_diff(curFrameTick_, lastRunTick_)));
  lastRunTick_ = curFrameTick_;

  for (unsigned i = 0; i != due; ++i) {
    a2host_simulate_frame();

    // Inside the loop, and paired with the simulate above it: the frame limit
    // and the frame hashes count emulated frames, not repaints.
    //
    // probe_stop_requested(): see the matching check in a2host_run_headless
    // (a2host.c) for why a probe's `stop` shows up as a flag polled here
    // rather than as an early return from a2host_simulate_frame() itself.
    if (a2host_record_frame() || a2host_engine_stopped() || probe_stop_requested()) {
      sapp_request_quit();
      break;
    }
  }

  // due == 0: the machine did not advance this repaint (a fast monitor
  // between two due emulated frames), so persistence should not decay either
  // -- see apply_persistence()'s comment on decayThisFrame.
  update_screen(due != 0);
  update_screen_image();

  sg_pass_action pass_action = {.colors[0] = {.action = SG_ACTION_CLEAR}};
  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());

  {
    // We always preserve the aspect ratio. We don't care about the
    // pixel aspect ratio of the original CRT monitors.
    int w = sapp_width();
    int h = sapp_height();
    int desiredW, desiredH;

    if (w * A2_SCREEN_H / h >= A2_SCREEN_W) {
      desiredH = h;
      desiredW = h * A2_SCREEN_W / A2_SCREEN_H;
    } else {
      desiredW = w;
      desiredH = w * A2_SCREEN_H / A2_SCREEN_W;
    }
    sg_apply_viewport((w - desiredW) / 2, (h - desiredH) / 2, desiredW, desiredH, true);
  }

  sg_apply_pipeline(pip_);
  sg_apply_bindings(&bind_);
  sg_draw(0, 4, 1);
  sg_end_pass();
  sg_commit();
}

static void event_cb(const sapp_event *ev) {
  // The front end gets first refusal, so it can add keys of its own.
  if (event_hook_ && event_hook_(ev))
    return;

  int toIgnore = ignoreNextCh_;
  ignoreNextCh_ = -1;

  // A cycle-stamped recording is driving input; real keystrokes must not
  // perturb it.
  if (a2host_key_replay_active())
    return;

  // If we are reading from a file, just ensure that the keyboard queue us full,
  // so events here will have no effect.
  a2host_drain_kbd_file();

  if (ev->type == SAPP_EVENTTYPE_CHAR && ev->char_code < 128) {
    int k = (int)ev->char_code;
    if (k == 127) // Del
      k = 8;
    else if (isalpha(k))
      k = toupper(k);
    if (k != toIgnore)
      a2host_press_key(k);
  } else if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
    switch (ev->key_code) {
    case SAPP_KEYCODE_DELETE:
    case SAPP_KEYCODE_BACKSPACE:
    case SAPP_KEYCODE_LEFT:
      a2host_press_key(ignoreNextCh_ = 8);
      break;
    case SAPP_KEYCODE_RIGHT:
      a2host_press_key(ignoreNextCh_ = 21); // CTRL+U
      break;
    case SAPP_KEYCODE_ENTER:
      a2host_press_key(ignoreNextCh_ = 13);
      break;
    case SAPP_KEYCODE_ESCAPE:
      a2host_press_key(ignoreNextCh_ = 27);
      break;
    // Display options -- not part of the emulated machine, so function keys
    // rather than anything printable, which the machine would see instead.
    // F1/F2 are already a2emu's bundled-game keys (see event_hook_ above);
    // this front end owns everything else.
    case SAPP_KEYCODE_F3:
      // Cycle the hi-res render mode: color -> color140 -> mono -> mono140 ->
      // color. Stated explicitly, not as hgrMode_ + 1: a2_hgr_mode_t's
      // declaration order (color, mono, color140, mono140) is not this
      // presentation order -- color and color140 are the pair this feature
      // exists to A/B, so they must be adjacent here regardless of how the
      // enum is declared. Do not derive this from the enum again.
      switch (hgrMode_) {
      case A2_HGR_COLOR:
        hgrMode_ = A2_HGR_COLOR140;
        break;
      case A2_HGR_COLOR140:
        hgrMode_ = A2_HGR_MONO;
        break;
      case A2_HGR_MONO:
        hgrMode_ = A2_HGR_MONO140;
        break;
      case A2_HGR_MONO140:
      default:
        hgrMode_ = A2_HGR_COLOR;
        break;
      }
      report_display_state();
      break;
    case SAPP_KEYCODE_F4:
      // Toggle phosphor persistence. Reset the buffer on the way to "on" so
      // stale content from a previous on-period does not flash back in.
      persistEnabled_ = !persistEnabled_;
      if (persistEnabled_)
        memset(&persistBuf_, 0, sizeof(persistBuf_));
      report_display_state();
      break;
    case SAPP_KEYCODE_F5:
      // Less persistence (faster fade).
      if (persistDecay_ > 0.05f)
        persistDecay_ -= 0.05f;
      else
        persistDecay_ = 0.0f;
      report_display_state();
      break;
    case SAPP_KEYCODE_F6:
      // More persistence (slower fade). Kept below 1: at or above it the
      // buffer would never fade, which is a stuck screen, not a CRT.
      if (persistDecay_ < 0.90f)
        persistDecay_ += 0.05f;
      else
        persistDecay_ = 0.95f;
      report_display_state();
      break;
    default:
      break;
    }
  }
}

sapp_desc sokol_main(int argc, char *argv[]) {
  a2host_parse_args(argc, argv);

  if (a2host_headless())
    a2host_run_headless(); // Does not return.

  a2host_set_wall_clock_pacing();
  a2host_set_frame_done_fn(frame_done);

  return (sapp_desc){
      .init_cb = init_cb,
      .frame_cb = frame_cb,
      .cleanup_cb = cleanup_cb,
      .event_cb = event_cb,
      .width = A2_SCREEN_W * 2,
      .height = A2_SCREEN_H * 2,
      .window_title = a2host_argv0(),
      .icon.sokol_default = true,
  };
}
