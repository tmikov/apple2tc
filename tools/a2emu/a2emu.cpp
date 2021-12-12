/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2plus_rom.h"
#include "apple2tc/DebugState6502.h"
#include "apple2tc/a2io.h"
#include "apple2tc/apple2.h"
#include "apple2tc/soundqueue.h"
#include "apple2tc/support.h"

#include "sokol_app.h"
#if A2EMU_SOUND
#include "sokol_audio.h"
#endif
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_time.h"

#include "blit.h"

#include <algorithm>
#include <cctype>
#include <optional>

class A2Emu {
public:
  explicit A2Emu();
  ~A2Emu();

  void event(const sapp_event *ev);
  void frame();

  uint8_t peek(uint16_t addr) {
    return emu_.peek(addr);
  }

  /// Method intended to be invoked from the host debugger.
  void disasm(uint16_t pc);
  /// Method intended to be invoked from the host debugger.
  void printDB(uint16_t startAddr);

private:
  /// Prepare the system window, init GFX.
  void initWindow();

  /// Simulate the last frame.
  void simulateFrame();

  /// Update the screen buffer.
  void updateScreen();

  /// Update the GFX image with data from the screen.
  void updateScreenImage();

private:
  sg_bindings bind_ = {0};
  sg_pipeline pip_ = {0};

  /// stm_now() at the start of every frame.
  uint64_t curFrameTick_ = 0;

  uint64_t lastRunTick_ = 0;
  uint64_t firstFrameTick_ = 0;
  bool firstFrame_ = true;

  /// KBD handling.
  /// If set to a valid character, the next character will be ignored, if it
  /// matches this value. Next character, whatever it is, always clears this.
  /// This is used on same platforms where some keys like ENTER arrive both as
  /// characters and as keydown events.
  int ignoreNextCh_ = -1;

  a2_sound_t sound_;
  a2_screen screen_;

  DebugState6502 dbg_{};
  EmuApple2 emu_{};
};

static A2Emu *s_a2emu = nullptr;

A2Emu::A2Emu() {
  initWindow();

  dbg_.addDefaultNonDebug();
  emu_.setDebugStateCB(&dbg_, DebugState6502::debugStateCB);

  a2_sound_init(&sound_);

  saudio_desc audioDesc = {
      .num_channels = 1,
      //.sample_rate = 44100,
      //.buffer_frames = 2048,
      .stream_userdata_cb =
          [](float *buffer, int num_frames, int num_channels, void *user_data) {
            a2_sound_cb((a2_sound_t *)user_data, buffer, num_frames, num_channels);
          },
      .user_data = &sound_,
  };
  saudio_setup(&audioDesc);

  emu_.setSpeakerCB(&sound_, [](void *ctx, unsigned cycles) {
    a2_sound_spkr((a2_sound_t *)ctx, Emu6502::CLOCK_FREQ, saudio_sample_rate(), cycles);
  });
  emu_.loadROM(apple2plus_rom, apple2plus_rom_len);

  stm_setup();
}

void A2Emu::initWindow() {
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

A2Emu::~A2Emu() {
  sg_shutdown();
  saudio_shutdown();
  a2_sound_free(&sound_);
}

/// Load a DOS3.3 binary buffer into emulated RAM.
/// Return the load address.
static std::optional<uint16_t> loadBin(EmuApple2 *emu, const uint8_t *data, size_t len) {
  if (len > 4) {
    uint16_t start = data[0] + data[1] * 256;
    if (len - 4 <= 0x10000 - start) {
      memcpy(emu->getMainRAMWritable() + start, data + 4, len - 4);
      printf("Loaded %zu at $%04X (%u)\n", len - 4, start, start);
      return start;
    }
  }
  return std::nullopt;
}

/// Start executing from the specified address.
static void run(EmuApple2 *emu, uint16_t addr) {
  printf("Executing!\n");
  auto r = emu->getRegs();
  r.pc = addr;
  emu->setRegs(r);
}

/// Load a DOS3.3 binary into RAM and execute it.
static void runBin(EmuApple2 *emu, const uint8_t *data, size_t len) {
  if (auto addr = loadBin(emu, data, len))
    run(emu, *addr);
}

/// Load a DOS3.3 binary file, where the first 4 bytes are start addr and
/// length.
static std::optional<uint16_t> loadBinFile(EmuApple2 *emu, const char *path) {
  if (FILE *f = fopen(path, "rb")) {
    auto b = readAll<std::vector<uint8_t>>(f);
    fclose(f);
    return loadBin(emu, b.data(), b.size());
  }
  return std::nullopt;
}

/// Load and run a DOS3.3 binary file.
static void runBinFile(EmuApple2 *emu, const char *path) {
  if (auto addr = loadBinFile(emu, path))
    run(emu, *addr);
}

#include "bolo.h"
#include "robotron2084.h"

void A2Emu::event(const sapp_event *ev) {
  int toIgnore = ignoreNextCh_;
  ignoreNextCh_ = -1;

  if (ev->type == SAPP_EVENTTYPE_CHAR && ev->char_code < 128) {
    int k = (int)ev->char_code;
    if (k == 127) // Del
      k = 8;
    else if (isalpha(k))
      k = toupper(k);
    if (k != toIgnore)
      emu_.pushKey(k);
  } else if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
    switch (ev->key_code) {
    case SAPP_KEYCODE_F1:
      runBin(&emu_, bolo_bin, bolo_bin_len);
      break;
    case SAPP_KEYCODE_F2:
      runBin(&emu_, robotron2084_bin, robotron2084_bin_len);
      break;

    case SAPP_KEYCODE_DELETE:
    case SAPP_KEYCODE_BACKSPACE:
    case SAPP_KEYCODE_LEFT:
      emu_.pushKey(ignoreNextCh_ = 8);
      break;
    case SAPP_KEYCODE_RIGHT:
      emu_.pushKey(ignoreNextCh_ = 21); // CTRL+U
      break;
    case SAPP_KEYCODE_ENTER:
      emu_.pushKey(ignoreNextCh_ = 13);
      break;
    case SAPP_KEYCODE_ESCAPE:
      emu_.pushKey(ignoreNextCh_ = 27);
      break;
    default:
      break;
    }
  }
}

void A2Emu::frame() {
  curFrameTick_ = stm_now();
  simulateFrame();
  updateScreen();
  updateScreenImage();

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

static bool started_ = false;
void A2Emu::simulateFrame() {
  if (firstFrame_) {
    firstFrame_ = false;
    firstFrameTick_ = curFrameTick_;
  } else {
    double elapsed = stm_sec(curFrameTick_ - lastRunTick_);
    unsigned runCycles = (unsigned)(std::min(elapsed, 0.200) * EmuApple2::CLOCK_FREQ);
    started_ = true;
    auto stopReason = emu_.runFor(runCycles);
    if (stopReason == Emu6502::StopReason::StopRequesed)
      exit(1);
    a2_sound_submit(&sound_, Emu6502::CLOCK_FREQ, saudio_sample_rate(), emu_.getCycles());
  }
  lastRunTick_ = curFrameTick_;
}

void A2Emu::updateScreen() {
  // Milliseconds since hw reset. Used to determine blink phase.
  auto ms = (uint64_t)stm_ms(stm_diff(curFrameTick_, firstFrameTick_));

  switch (emu_.getVidMode()) {
  case EmuApple2::VidMode::TEXT:
    apple2_render_text_screen(emu_.getTextPageAddr(), &screen_, ms);
    break;
  case EmuApple2::VidMode::GR:
    apple2_render_gr_screen(emu_.getTextPageAddr(), &screen_, ms, emu_.isVidMixed());
    break;
  case EmuApple2::VidMode::HGR:
    bool mono = false;
    apple2_render_hgr_screen(
        emu_.getHiresPageAddr(), emu_.getTextPageAddr(), &screen_, ms, emu_.isVidMixed(), mono);
    break;
  }
}

void A2Emu::updateScreenImage() {
  sg_image_data imgData = {.subimage[0][0] = {.ptr = screen_.data, .size = sizeof(screen_.data)}};
  sg_update_image(bind_.fs_images[SLOT_tex], &imgData);
}

void A2Emu::disasm(uint16_t pc) {
  for (unsigned i = 0; i != 20; ++i) {
    ThreeBytes bytes{0};
    for (unsigned i = 0; i != 3; ++i)
      bytes.d[i] = peek(pc + i);
    CPUInst inst = decodeInst(pc, bytes);
    FormattedInst fmt = formatInst(inst, bytes);

    printf("%04X: %-8s    %s", pc, fmt.bytes, fmt.inst);
    if (!fmt.operand.empty())
      printf("  %s", fmt.operand.c_str());
    printf("\n");

    pc += inst.size;
  }
}

void A2Emu::printDB(uint16_t startAddr) {
  char asciiBuf[17];
  asciiBuf[16] = 0;
  uint16_t addr = startAddr & ~15;
  uint16_t end = addr + 64;
  while (addr != end) {
    // Offset in a 16 byte row.
    uint16_t ofs = addr & 15;
    // Address.
    if (ofs == 0) {
      printf("%04X:  ", addr);
    }

    // Spaces between values.
    if (ofs == 8)
      printf("  ");
    else if (ofs != 0)
      printf(" ");

    // Actual values.
    if (addr < startAddr) {
      printf("__");
      asciiBuf[ofs] = '.';
    } else {
      uint8_t v = peek(addr);
      printf("%02X", v);
      asciiBuf[ofs] = v >= 32 && v < 128 ? v : '.';
    }

    if (ofs == 15)
      printf("  %s\n", asciiBuf);

    ++addr;
  }
  // s_curAddr = addr;
}

sapp_desc sokol_main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  return (sapp_desc){
      .init_cb = []() { s_a2emu = new A2Emu(); },
      .frame_cb =
          []() {
            if (s_a2emu)
              s_a2emu->frame();
          },
      .cleanup_cb = []() { delete s_a2emu; },
      .event_cb =
          [](const sapp_event *ev) {
            if (s_a2emu)
              s_a2emu->event(ev);
          },
      .width = A2_SCREEN_W * 2,
      .height = A2_SCREEN_H * 2,
      .window_title = "A2Emu",
      .icon.sokol_default = true,
  };
}
