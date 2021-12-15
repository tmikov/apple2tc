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
#include "sokol_audio.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_time.h"

#include "blit.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <fstream>
#include <iostream>
#include <optional>

struct CLIArgs {
  enum Action {
    // Just run the specified file.
    Run,
    // Run with tracing.
    Trace,
    // Collect data for disassembly.
    Collect,
  };
  Action action = Action::Run;
  unsigned limit = 100000;
  std::string runPath{};
  std::string outputPath{};
};

class A2Emu {
public:
  explicit A2Emu(CLIArgs &&cliArgs);
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

  /// The CLI command has been activated by pressing F3.
  void cliCommand();
  /// Invoked when the simulation returns that stop was requested.
  void simulationStop();

private:
  CLIArgs cliArgs_;
  std::optional<CLIArgs::Action> curAction_{};

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

A2Emu::A2Emu(CLIArgs &&cliArgs) : cliArgs_(std::move(cliArgs)) {
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
static std::optional<uint16_t> loadB33Buf(EmuApple2 *emu, const uint8_t *data, size_t len) {
  if (len > 4) {
    uint16_t start = data[0] + data[1] * 256;
    if (len - 4 <= 0x10000 - start) {
      memcpy(emu->getMainRAMWritable() + start, data + 4, len - 4);
      printf("Loaded %zu at $%04X (%u)\n", len - 4, start, start);
      return start;
    }
  }
  fprintf(stderr, "Invalid b33 format\n");
  return std::nullopt;
}

/// Start executing from the specified address.
static void setRegsForRun(EmuApple2 *emu, uint16_t addr) {
  printf("Executing at $%04X!\n", addr);
  auto r = emu->getRegs();
  r.pc = addr;
  r.status = Emu6502::STATUS_IGNORED;
  emu->setRegs(r);
}

/// Load a DOS3.3 binary into RAM and execute it.
static void runB33(EmuApple2 *emu, const uint8_t *data, size_t len) {
  if (auto addr = loadB33Buf(emu, data, len))
    setRegsForRun(emu, *addr);
}

/// Load a DOS3.3 binary file, where the first 4 bytes are start addr and
/// length.
static std::optional<uint16_t> loadB33File(EmuApple2 *emu, const char *path) {
  if (FILE *f = fopen(path, "rb")) {
    auto b = readAll<std::vector<uint8_t>>(f);
    fclose(f);
    return loadB33Buf(emu, b.data(), b.size());
  } else {
    perror(path);
  }
  return std::nullopt;
}

/// Load and run a DOS3.3 binary file.
static void runB33File(EmuApple2 *emu, const char *path) {
  if (auto addr = loadB33File(emu, path))
    setRegsForRun(emu, *addr);
}

void A2Emu::cliCommand() {
  if (curAction_.has_value())
    return;

  if (cliArgs_.runPath.empty()) {
    fprintf(stderr, "A path was not supplied to the CLI\n");
    return;
  }
  auto addr = loadB33File(&emu_, cliArgs_.runPath.c_str());
  if (!addr)
    return;

  emu_.setDebugFlags(emu_.getDebugFlags() & ~Emu6502::DebugASM);
  dbg_.reset();

  setRegsForRun(&emu_, *addr);

  switch (cliArgs_.action) {
  case CLIArgs::Run:
    break;
  case CLIArgs::Trace:
    emu_.addDebugFlags(Emu6502::DebugASM);
    dbg_.setDebugBB(true);
    dbg_.setLimit(cliArgs_.limit);
    break;
  case CLIArgs::Collect:
    emu_.addDebugFlags(Emu6502::DebugASM);
    dbg_.setCollect(true);
    dbg_.setLimit(cliArgs_.limit);
    break;
  }

  curAction_ = cliArgs_.action;
}

void A2Emu::simulationStop() {
  if (!curAction_.has_value()) {
    // Not clear why and whether this might happen, but to be safe do nothing.
    return;
  }

  fprintf(stderr, "Command completed\n");

  if (*curAction_ == CLIArgs::Action::Collect) {
    fflush(stdout);
    std::ostream *os;
    std::ofstream of;
    if (!cliArgs_.outputPath.empty()) {
      of.open(cliArgs_.outputPath, std::ios_base::out);
      os = &of;
    } else {
      os = &std::cout;
    }
    dbg_.finishCollection(*os);
    os->flush();
  }

  emu_.setDebugFlags(emu_.getDebugFlags() & ~Emu6502::DebugASM);
  dbg_.reset();

  curAction_.reset();
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
      runB33(&emu_, bolo_bin, bolo_bin_len);
      break;
    case SAPP_KEYCODE_F2:
      runB33(&emu_, robotron2084_bin, robotron2084_bin_len);
      break;
    case SAPP_KEYCODE_F3:
      cliCommand();
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

void A2Emu::simulateFrame() {
  if (firstFrame_) {
    firstFrame_ = false;
    firstFrameTick_ = curFrameTick_;
  } else {
    double elapsed = stm_sec(curFrameTick_ - lastRunTick_);
    unsigned runCycles = (unsigned)(std::min(elapsed, 0.200) * EmuApple2::CLOCK_FREQ);
    auto stopReason = emu_.runFor(runCycles);
    a2_sound_submit(&sound_, Emu6502::CLOCK_FREQ, saudio_sample_rate(), emu_.getCycles());
    if (stopReason == Emu6502::StopReason::StopRequesed)
      simulationStop();
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

static const char *s_argv0 = "a2emu";
static void printHelp() {
  printf("syntax: %s [options] [inputFile [outputFile]]\n", s_argv0);
  printf(" --help           This help\n");
  printf(" --run            Run the binary\n");
  printf(" --trace          Trace the binary\n");
  printf(" --collect        Collect data from running and write to outputFile or stdout\n");
  printf(" --limit=number   Number of basic blocks to trace/collect\n");
}

static CLIArgs parseCLI(int argc, char **argv) {
  s_argv0 = argc ? argv[0] : "a2emu";
  CLIArgs cliArgs{};
  for (int i = 1; i != argc; ++i) {
    char *arg = argv[i];
    if (strcmp(arg, "--help") == 0) {
      printHelp();
      exit(0);
    }
    if (strcmp(arg, "--run") == 0) {
      cliArgs.action = CLIArgs::Action::Run;
      continue;
    }
    if (strcmp(arg, "--trace") == 0) {
      cliArgs.action = CLIArgs::Action::Trace;
      continue;
    }
    if (strcmp(arg, "--collect") == 0) {
      cliArgs.action = CLIArgs::Action::Collect;
      continue;
    }
    if (strncmp(arg, "--limit=", 8) == 0) {
      auto cr = std::from_chars(arg + 8, strchr(arg, 0), cliArgs.limit);
      if (*cr.ptr || cr.ec != std::errc()) {
        fprintf(stderr, "Invalid number in '%s'\n", arg);
        printHelp();
        exit(1);
      }
      continue;
    }
    if (arg[0] == '-') {
      fprintf(stderr, "Invalid option '%s'\n", arg);
      printHelp();
      exit(1);
    }
    if (cliArgs.runPath.empty()) {
      cliArgs.runPath = arg;
      continue;
    }
    if (cliArgs.outputPath.empty()) {
      cliArgs.outputPath = arg;
      continue;
    }
    fprintf(stderr, "Extra command line argument '%s'\n", arg);
    printHelp();
    exit(1);
  }

  return cliArgs;
}

sapp_desc sokol_main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  static CLIArgs s_cliArgs = parseCLI(argc, argv);

  return (sapp_desc){
      .init_cb = []() { s_a2emu = new A2Emu(std::move(s_cliArgs)); },
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
