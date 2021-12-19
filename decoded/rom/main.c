/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/a2io.h"
#include "apple2tc/apple2iodefs.h"
#include "apple2tc/sokol/sokol_app.h"
#include "apple2tc/sokol/sokol_audio.h"
#include "apple2tc/sokol/sokol_gfx.h"
#include "apple2tc/sokol/sokol_glue.h"
#include "apple2tc/sokol/sokol_time.h"
#include "apple2tc/system.h"

#include "apple2tc/sokol/blit.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static sg_bindings bind_;
static sg_pipeline pip_;

/// stm_now() at the start of every frame.
static uint64_t curFrameTick_ = 0;
static uint64_t lastRunTick_ = 0;
static uint64_t firstFrameTick_ = 0;
static bool firstFrame_ = true;

/// KBD handling.
/// If set to a valid character, the next character will be ignored, if it
/// matches this value. Next character, whatever it is, always clears this.
/// This is used on same platforms where some keys like ENTER arrive both as
/// characters and as keydown events.
static int ignoreNextCh_ = -1;

// Input keyboard queue.
#define KEY_QUEUE_SIZE 32
static uint8_t s_keys[KEY_QUEUE_SIZE];
static unsigned s_keys_head = 0;
static unsigned s_keys_count = 0;
/// The last key that was returned.
static uint8_t s_last_key = 0;

static a2_sound_t sound_;
static a2_screen screen_;

static void push_key(uint8_t key) {
  if (s_keys_count == KEY_QUEUE_SIZE)
    return;
  s_keys[(s_keys_head + s_keys_count++) % KEY_QUEUE_SIZE] = key;
}

static uint8_t kbd(void) {
  return s_keys_count == 0 ? s_last_key : s_keys[s_keys_head] | 0x80;
}

static void kbdstrb(void) {
  if (s_keys_count) {
    s_last_key = s_keys[s_keys_head] & 0x7F;
    s_keys_head = (s_keys_head + 1) % KEY_QUEUE_SIZE;
    --s_keys_count;
  }
}

uint8_t io_peek(uint16_t addr) {
  switch (addr & 0xCFF0) {
  case A2_KBD:
    if (g_debug & DebugIO1)
      fprintf(stdout, "[%u] KBD\n", get_cycles());
    return kbd();
  case A2_KBDSTRB:
    if (g_debug & DebugIO2)
      fprintf(stdout, "[%u] KBDSTRB\n", get_cycles());
    kbdstrb();
    break;
  case A2_TAPEOUT:
    if (g_debug & DebugIO1)
      fprintf(stdout, "[%u] TAPEOUT\n", get_cycles());
    break;
  case A2_SPKR:
    if (g_debug & DebugIO2)
      fprintf(stdout, "[%u] SPKR\n", get_cycles());
    // if (spkrCB_)
    //   spkrCB_(spkrCBCtx_, get_cycles());
    break;
  case A2_STROBE:
    if (g_debug & DebugIO1)
      fprintf(stdout, "[%u] STROBE\n", get_cycles());
    break;

  case A2_TXTCLR:
    switch (addr) {
    case A2_TXTCLR:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] TXTCLR\n", get_cycles());
      // vidControl_ &= ~VCText;
      break;
    case A2_TXTSET:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] TXTSET\n", get_cycles());
      // vidControl_ |= VCText;
      break;
    case A2_MIXCLR:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] MIXCLR\n", get_cycles());
      // vidControl_ &= ~VCMixed;
      break;
    case A2_MIXSET:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] MIXSET\n", get_cycles());
      // vidControl_ |= VCMixed;
      break;
    case A2_LOWSCR:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] LOWSCR\n", get_cycles());
      // vidControl_ &= ~VCPage2;
      break;
    case A2_HISCR:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] HISCR\n", get_cycles());
      // vidControl_ |= VCPage2;
      break;
    case A2_LORES:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] LORES\n", get_cycles());
      // vidControl_ &= ~VCHires;
      break;
    case A2_HIRES:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] HIRES\n", get_cycles());
      // vidControl_ |= VCHires;
      break;
    default:
      if (g_debug & DebugIO1)
        fprintf(stdout, "[%u] ANNUNCIATORS $%04X\n", get_cycles(), addr);
      break;
    }
    break;

  default:
    fprintf(
        stderr,
        "[%u] pc=$%04X Unsupported IO location read $%04X\n",
        get_cycles(),
        get_regs().pc,
        addr);
  }

  return 0;
}

void io_poke(uint16_t addr, uint8_t value) {
  io_peek(addr);
  io_peek(addr);
}

typedef struct {
  uint16_t addr;
  uint8_t size;
} watch_t;

enum { MAX_WATCHES = 32u };
static watch_t s_watches[MAX_WATCHES];
static unsigned s_num_watches = 0;

static void add_watch(uint16_t addr, uint8_t size) {
  if (s_num_watches < MAX_WATCHES)
    s_watches[s_num_watches++] = (watch_t){.addr = addr, .size = size};
}

typedef struct {
  uint16_t from;
  uint16_t to;
} range_t;

enum { MAX_NONDEBUG = 32 };
static range_t s_nondebug[MAX_NONDEBUG];
static unsigned s_num_nondebug = 0;

static void add_nondebug(uint16_t from, uint16_t to) {
  if (s_num_nondebug < MAX_NONDEBUG)
    s_nondebug[s_num_nondebug++] = (range_t){.from = from, .to = to};
}

static void add_default_nondebug(void) {
  add_nondebug(0xFCA8, 0xFCB3); // MONWAIT
  add_nondebug(0xFD0C, 0xFD3C); // Keyboard
}

static unsigned s_numDebugLines = 0;
enum { MAX_DEBUG_LINES = 2000000 };

void debug_asm(uint16_t pc) {
  for (unsigned i = 0; i != s_num_nondebug; ++i) {
    if (pc >= s_nondebug[i].from && pc <= s_nondebug[i].to)
      return;
  }

  if (g_debug & DebugCountBB) {
    if (++s_numDebugLines == MAX_DEBUG_LINES) {
      printf("Reached %u basic blocks\n", s_numDebugLines);
      exit(0);
    }
    return;
  }

  regs_t r = get_regs();
  r.pc = pc;
  printf("%04X: %-8s  ", r.pc, "");

  // Dump the registers.
  printf("A=%02X X=%02X Y=%02X SP=%02X SR=", r.a, r.x, r.y, r.sp);
  // Dump the flags.
  static const char names[9] = "NV.BDIZC";
  for (unsigned i = 0; i != 8; ++i)
    putchar((r.status & (0x80 >> i)) ? names[i] : '.');

  if (s_num_watches == 0) {
    // The PC again for convenience.
    printf(" PC=%04X", r.pc);
  }

  for (unsigned i = 0; i != s_num_watches; ++i) {
    watch_t watch = s_watches[i];
    putchar(' ');
    if (watch.addr < 256)
      printf("($%02X)=", watch.addr);
    else
      printf("($%04X)=", watch.addr);
    if (watch.size == 1)
      printf("$%02X", ram_peek(watch.addr));
    else
      printf("$%04X", ram_peek16(watch.addr));
  }
  putchar('\n');

  if (++s_numDebugLines == MAX_DEBUG_LINES) {
    fflush(stdout);
    exit(0);
  }
}

void error_handler(uint16_t pc) {
  printf("BB count=%u\n", s_numDebugLines);
  exit(1);
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

static void init_cb(void) {
  init_window();
  stm_setup();

  g_debug = DebugIO2;
  add_default_nondebug();
  reset_regs();
  set_regs((regs_t){.pc = 0, .a = 0, .x = 0, .y = 0, .sp = 0xff, .status = STATUS_IGNORED});

  init_emulated();
}

static void cleanup_cb(void) {}

static void simulate_frame(void) {
  if (firstFrame_) {
    firstFrame_ = false;
    firstFrameTick_ = curFrameTick_;
  } else {
    double elapsed = stm_sec(curFrameTick_ - lastRunTick_);
    unsigned runCycles = (unsigned)((elapsed < 0.200 ? elapsed : 0.200) * A2_CLOCK_FREQ);
    run_emulated(runCycles);
    a2_sound_submit(&sound_, A2_CLOCK_FREQ, saudio_sample_rate(), get_cycles());
  }
  lastRunTick_ = curFrameTick_;
}

static void update_screen(void) {
  // Milliseconds since hw reset. Used to determine blink phase.
  uint64_t ms = (uint64_t)stm_ms(stm_diff(curFrameTick_, firstFrameTick_));

  // switch (emu_.getVidMode()) {
  // case EmuApple2::VidMode::TEXT:
  apple2_render_text_screen(get_ram() + A2_TXT1SCRN, &screen_, ms);
  //  break;
  // case EmuApple2::VidMode::GR:
  //  apple2_render_gr_screen(emu_.getTextPageAddr(), &screen_, ms, emu_.isVidMixed());
  //  break;
  // case EmuApple2::VidMode::HGR:
  //  bool mono = false;
  //  apple2_render_hgr_screen(
  //      emu_.getHiresPageAddr(), emu_.getTextPageAddr(), &screen_, ms, emu_.isVidMixed(), mono);
  //  break;
  //}
}

static void update_screen_image(void) {
  sg_image_data imgData = {.subimage[0][0] = {.ptr = screen_.data, .size = sizeof(screen_.data)}};
  sg_update_image(bind_.fs_images[SLOT_tex], &imgData);
}

static void frame_cb(void) {
  curFrameTick_ = stm_now();
  simulate_frame();
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

  if (ev->type == SAPP_EVENTTYPE_CHAR && ev->char_code < 128) {
    int k = (int)ev->char_code;
    if (k == 127) // Del
      k = 8;
    else if (isalpha(k))
      k = toupper(k);
    if (k != toIgnore)
      push_key(k);
  } else if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
    switch (ev->key_code) {
    case SAPP_KEYCODE_DELETE:
    case SAPP_KEYCODE_BACKSPACE:
    case SAPP_KEYCODE_LEFT:
      push_key(ignoreNextCh_ = 8);
      break;
    case SAPP_KEYCODE_RIGHT:
      push_key(ignoreNextCh_ = 21); // CTRL+U
      break;
    case SAPP_KEYCODE_ENTER:
      push_key(ignoreNextCh_ = 13);
      break;
    case SAPP_KEYCODE_ESCAPE:
      push_key(ignoreNextCh_ = 27);
      break;
    default:
      break;
    }
  }
}

sapp_desc sokol_main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  return (sapp_desc){
      .init_cb = init_cb,
      .frame_cb = frame_cb,
      .cleanup_cb = cleanup_cb,
      .event_cb = event_cb,
      .width = A2_SCREEN_W * 2,
      .height = A2_SCREEN_H * 2,
      .window_title = "Rom",
      .icon.sokol_default = true,
  };
}
