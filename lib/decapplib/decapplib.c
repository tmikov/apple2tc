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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct KeyPress {
  unsigned cycles;
  uint8_t ch;
} KeyPress;

static bool sound_enabled_ = true;
/// If set, a file to read keyboard input from.
static FILE *kbd_file_ = NULL;
/// Assumed clock frequency. Can be used for "overclocking".
static unsigned clock_freq_ = A2_CLOCK_FREQ;
/// If true, dump key presses with cycle stamps.
static bool trace_keys_ = false;
/// Key-presses loaded from disk.
static KeyPress *key_presses_ = NULL;
/// Number of loaded key presses.
static unsigned key_press_count_ = 0;
/// Next key press to process.
static unsigned next_key_press_ = 0;

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

static a2_sound_t sound_;
static a2_iostate_t io_;
static a2_screen screen_;

uint8_t io_peek(uint16_t addr) {
  return a2_io_peek(&io_, addr, get_cycles());
}

void io_poke(uint16_t addr, uint8_t value) {
  a2_io_poke(&io_, addr, value, get_cycles());
}

typedef struct {
  uint16_t addr;
  uint8_t size;
} watch_t;

enum { MAX_WATCHES = 32u };
static watch_t s_watches[MAX_WATCHES];
static unsigned s_num_watches = 0;

void add_watch(uint16_t addr, uint8_t size) {
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

void add_nondebug(uint16_t from, uint16_t to) {
  if (s_num_nondebug < MAX_NONDEBUG)
    s_nondebug[s_num_nondebug++] = (range_t){.from = from, .to = to};
}

void add_default_nondebug(void) {
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

/// Callback from a2_io_t, when speaker has been flipped.
static void speaker_cb(void *ctx, unsigned cycles) {
  a2_sound_spkr((a2_sound_t *)ctx, A2_CLOCK_FREQ, saudio_sample_rate(), cycles);
}

/// Callback from saudio to generate new sound samples.
static void stream_userdata_cb(float *buffer, int num_frames, int num_channels, void *user_data) {
  a2_sound_cb((a2_sound_t *)user_data, buffer, num_frames, num_channels);
}

static void push_key(uint8_t ch) {
  a2_io_push_key(&io_, ch);
  if (trace_keys_)
    printf("%u %u\n", get_cycles(), ch);
}

static void push_key_if_empty(uint8_t ch) {
  if (a2_io_keys_count(&io_) == 0)
    push_key(ch);
}

/// While the KBD file is open, read as many characters from it as possible.
/// Close the file of EOF is reached.
static void drain_kbd_file() {
  if (!kbd_file_)
    return;
  while (a2_io_keys_expect(&io_)) {
    int ch = getc(kbd_file_);
    if (ch == EOF) {
      fclose(kbd_file_);
      kbd_file_ = NULL;
      break;
    }
    if (ch == '\r')
      continue;
    if (ch == '\n')
      ch = '\r';
    push_key((uint8_t)ch);
  }
}

/// Return true if there are more keypresses to process.
static void drain_key_presses() {
  if (!key_presses_)
    return;

  static unsigned last_cycles = 0;
  unsigned cycles = get_cycles();

  while (next_key_press_ != key_press_count_ && cycles == key_presses_[next_key_press_].cycles) {
    a2_io_push_key(&io_, key_presses_[next_key_press_].ch);
    ++next_key_press_;
  }

  if (next_key_press_ != key_press_count_)
    return;

  free(key_presses_);
  key_presses_ = 0;
  next_key_press_ = 0;
  key_press_count_ = 0;
}

static void load_key_file(const char *path) {
  if (key_presses_) {
    fprintf(stderr, "Key file already loaded\n");
    exit(2);
  }
  FILE *f;
  int res;
  if ((f = fopen(path, "rt")) == NULL) {
    perror(path);
    exit(2);
  }

  unsigned capacity = 32;
  key_presses_ = (KeyPress *)malloc(sizeof(KeyPress) * capacity);
  if (!key_presses_)
    abort();
  key_press_count_ = 0;
  next_key_press_ = 0;

  unsigned cycles, ch;
  while ((res = fscanf(f, "%u %u\n", &cycles, &ch)) == 2) {
    if (key_press_count_ == capacity) {
      if (capacity > UINT_MAX / 2) {
        fprintf(stderr, "Too many keys\n");
        exit(2);
      }
      capacity *= 2;
      key_presses_ = realloc(key_presses_, sizeof(KeyPress) * capacity);
      if (!key_presses_)
        abort();
    }
    key_presses_[key_press_count_].cycles = cycles;
    key_presses_[key_press_count_].ch = (uint8_t)ch;
    ++key_press_count_;
  }
  if (res != EOF) {
    fprintf(stderr, "Error parsing key file\n");
    exit(2);
  }
  fprintf(stderr, "Loaded %u keys\n", key_press_count_);

  fclose(f);
}

static void init_cb(void) {
  init_window();
  stm_setup();

  a2_sound_init(&sound_);
  a2_io_init(&io_);
  a2_io_set_spkr_cb(&io_, &sound_, speaker_cb);
  io_.debug = 0;

  if (sound_enabled_) {
    saudio_desc audioDesc = {
        .num_channels = 1,
        .stream_userdata_cb = stream_userdata_cb,
        .user_data = &sound_,
    };
    saudio_setup(&audioDesc);
  }

  add_default_nondebug();
  reset_regs();
  // SP is 0xF0 in BASIC.
  regs_t r = get_regs();
  r.sp = 0xF0;
  set_regs(r);

  init_emulated();

  if (kbd_file_ || key_presses_) {
    // The first key pressed before initialization is lost, so just add a dummy keypress.
    a2_io_push_key(&io_, '\r');
    if (key_presses_)
      drain_key_presses();
    else
      drain_kbd_file();
  }
}

static void cleanup_cb(void) {
  shutdown_emulated();
  sg_shutdown();
  if (sound_enabled_)
    saudio_shutdown();
  a2_io_done(&io_);
  a2_sound_done(&sound_);
}

static void simulate_frame(void) {
  if (firstFrame_) {
    firstFrame_ = false;
    firstFrameTick_ = curFrameTick_;
  } else {
    if (key_presses_)
      drain_key_presses();
    else if (kbd_file_)
      drain_kbd_file();

    unsigned runCycles;
    if (trace_keys_ || key_presses_) {
      // If we are recording or replaying, we need to have reproducible cycles.
      runCycles = (unsigned)((1.0 / 60.0) * clock_freq_);
    } else {
      double elapsed = stm_sec(curFrameTick_ - lastRunTick_);
      runCycles = (unsigned)((elapsed < 0.200 ? elapsed : 0.200) * clock_freq_);
    }
    run_emulated(runCycles);
    a2_sound_submit(&sound_, A2_CLOCK_FREQ, saudio_sample_rate(), get_cycles());
  }
  lastRunTick_ = curFrameTick_;
}

static void update_screen(void) {
  // Milliseconds since hw reset. Used to determine blink phase.
  uint64_t ms = (uint64_t)stm_ms(stm_diff(curFrameTick_, firstFrameTick_));

  switch (a2_io_get_vidmode(&io_)) {
  case A2_VIDMODE_TEXT:
    apple2_render_text_screen(get_ram() + a2_io_get_text_page_offset(&io_), &screen_, ms);
    break;
  case A2_VIDMODE_GR:
    apple2_render_gr_screen(
        get_ram() + a2_io_get_text_page_offset(&io_), &screen_, ms, a2_io_is_vidmode_mixed(&io_));
    break;
  case A2_VIDMODE_HGR:
  default:;
    bool mono = false;
    apple2_render_hgr_screen(
        get_ram() + a2_io_get_hires_page_offset(&io_),
        get_ram() + a2_io_get_text_page_offset(&io_),
        &screen_,
        ms,
        a2_io_is_vidmode_mixed(&io_),
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

  if (key_presses_)
    return;

  // If we are reading from a file, just ensure that the keyboard queue us full,
  // so events here will have no effect.
  if (kbd_file_)
    drain_kbd_file();

  if (ev->type == SAPP_EVENTTYPE_CHAR && ev->char_code < 128) {
    int k = (int)ev->char_code;
    if (k == 127) // Del
      k = 8;
    else if (isalpha(k))
      k = toupper(k);
    if (k != toIgnore)
      push_key_if_empty(k);
  } else if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
    switch (ev->key_code) {
    case SAPP_KEYCODE_DELETE:
    case SAPP_KEYCODE_BACKSPACE:
    case SAPP_KEYCODE_LEFT:
      push_key_if_empty(ignoreNextCh_ = 8);
      break;
    case SAPP_KEYCODE_RIGHT:
      push_key_if_empty(ignoreNextCh_ = 21); // CTRL+U
      break;
    case SAPP_KEYCODE_ENTER:
      push_key_if_empty(ignoreNextCh_ = 13);
      break;
    case SAPP_KEYCODE_ESCAPE:
      push_key_if_empty(ignoreNextCh_ = 27);
      break;
    default:
      break;
    }
  }
}

static const char *s_argv0 = "emu";

static void print_help() {
  printf("syntax: %s [options]\n", s_argv0);
  printf(" --help           This help\n");
  printf(" --no-sound       Disable sound\n");
  printf(" --kbd-file=path  Read ascii keyboard input from the specified file\n");
  printf(" --key-file=path  Read key presses and cycles from the specified file\n");
  printf(" --fast           Emulate a faster CPU\n");
  printf(" --trace          Dump state at branch targets\n");
  printf(" --trace-mem      Dump all memory writes\n");
  printf(" --trace-keys     Dump key presses with cycle stamps\n");
  printf(" --count-bt       Count branch targets\n");
}

static void parse_args(int argc, char *argv[]) {
  if (argc) {
    // Scan backwards to a path separator.
    const char *e = strchr(argv[0], 0);
    while (e != argv[0] && e[-1] != '/' && e[-1] != '\\')
      --e;
    s_argv0 = e;
  }

  for (int i = 1; i < argc; ++i) {
    const char *arg = argv[i];
    if (strcmp(arg, "--help") == 0) {
      print_help();
      exit(0);
    }
    if (strcmp(arg, "--no-sound") == 0) {
      sound_enabled_ = false;
      continue;
    }
    if (strcmp(arg, "--trace") == 0) {
      g_debug |= DebugASM;
      continue;
    }
    if (strcmp(arg, "--trace-mem") == 0) {
      g_debug |= DebugMem;
      continue;
    }
    if (strcmp(arg, "--trace-keys") == 0) {
      trace_keys_ = true;
      continue;
    }
    if (strcmp(arg, "--debug-bt") == 0) {
      g_debug |= DebugCountBB;
      continue;
    }
    if (strncmp(arg, "--kbd-file=", 11) == 0) {
      const char *path = arg + 11;
      if ((kbd_file_ = fopen(path, "rt")) == NULL) {
        perror(path);
        exit(2);
      }
      continue;
    }
    if (strncmp(arg, "--key-file=", 11) == 0) {
      const char *path = arg + 11;
      load_key_file(path);
      continue;
    }
    if (strcmp(arg, "--fast") == 0) {
      clock_freq_ = A2_CLOCK_FREQ * 5;
      continue;
    }

    if (arg[0] == '-') {
      fprintf(stderr, "Invalid option '%s'\n", arg);
      print_help();
      exit(1);
    }
    fprintf(stderr, "Extra command line argument '%s'\n", arg);
    print_help();
    exit(1);
  }
}

sapp_desc sokol_main(int argc, char *argv[]) {
  parse_args(argc, argv);

  return (sapp_desc){
      .init_cb = init_cb,
      .frame_cb = frame_cb,
      .cleanup_cb = cleanup_cb,
      .event_cb = event_cb,
      .width = A2_SCREEN_W * 2,
      .height = A2_SCREEN_H * 2,
      .window_title = s_argv0,
      .icon.sokol_default = true,
  };
}
