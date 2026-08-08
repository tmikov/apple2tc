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

#include "apple2tc/a2host_api.h"

#include "apple2tc/a2io.h"
#include "apple2tc/apple2iodefs.h"
#include "apple2tc/system.h"
#include "apple2tc/sokol/sokol_app.h"
#include "apple2tc/sokol/sokol_audio.h"
#include "apple2tc/sokol/sokol_gfx.h"
#include "apple2tc/sokol/sokol_glue.h"
#include "apple2tc/sokol/sokol_time.h"

#include "apple2tc/sokol/blit.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static sg_bindings bind_;
static sg_pipeline pip_;
static a2_sound_t sound_;
static a2_screen screen_;

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

/// Wall-clock pacing, handed to a2host. Only consulted for runs that are not
/// required to be reproducible.
static double elapsed_since_last_frame(void) {
  return stm_sec(curFrameTick_ - lastRunTick_);
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

static void update_screen(void) {
  // Milliseconds since hw reset. Used to determine blink phase.
  uint64_t ms = (uint64_t)stm_ms(stm_diff(curFrameTick_, firstFrameTick_));

  switch (a2_io_get_vidmode(a2host_io())) {
  case A2_VIDMODE_TEXT:
    apple2_render_text_screen(get_ram() + a2_io_get_text_page_offset(a2host_io()), &screen_, ms);
    break;
  case A2_VIDMODE_GR:
    apple2_render_gr_screen(
        get_ram() + a2_io_get_text_page_offset(a2host_io()), &screen_, ms, a2_io_is_vidmode_mixed(a2host_io()));
    break;
  case A2_VIDMODE_HGR:
  default:;
    bool mono = false;
    apple2_render_hgr_screen(
        get_ram() + a2_io_get_hires_page_offset(a2host_io()),
        get_ram() + a2_io_get_text_page_offset(a2host_io()),
        &screen_,
        ms,
        a2_io_is_vidmode_mixed(a2host_io()),
        mono);
    break;
  }
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
  }

  a2host_simulate_frame();
  lastRunTick_ = curFrameTick_;

  if (a2host_record_frame() || a2host_engine_stopped())
    sapp_request_quit();

  update_screen();
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
      a2host_push_key_if_empty(k);
  } else if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
    switch (ev->key_code) {
    case SAPP_KEYCODE_DELETE:
    case SAPP_KEYCODE_BACKSPACE:
    case SAPP_KEYCODE_LEFT:
      a2host_push_key_if_empty(ignoreNextCh_ = 8);
      break;
    case SAPP_KEYCODE_RIGHT:
      a2host_push_key_if_empty(ignoreNextCh_ = 21); // CTRL+U
      break;
    case SAPP_KEYCODE_ENTER:
      a2host_push_key_if_empty(ignoreNextCh_ = 13);
      break;
    case SAPP_KEYCODE_ESCAPE:
      a2host_push_key_if_empty(ignoreNextCh_ = 27);
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

  a2host_set_elapsed_fn(elapsed_since_last_frame);
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

