/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The Apple II host: everything that is not the CPU and not the display.
///
/// Owns the IO state, the keyboard queue and its replay, the frame loop, the
/// frame-hash oracle and the command line. Executes no 6502 itself -- it drives
/// whichever engine is linked, through `a2engine.h`.
///
/// **Deliberately free of sokol.** A console front end links this and nothing
/// graphical; the GUI front end adds `a2host_gui`. See `a2host_api.h` for why
/// those are separate executables rather than one binary with a flag.

#include "apple2tc/a2host_api.h"

#include "apple2tc/a2io.h"
#include "apple2tc/apple2iodefs.h"
#include "apple2tc/probe.h"
#include "apple2tc/system.h"

#include "probe_internal.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KeyPress {
  unsigned cycles;
  uint8_t ch;
} KeyPress;

static bool sound_enabled_ = true;
/// If set, a file to read keyboard input from.
static FILE *kbd_file_ = NULL;
/// If set, a probe script was loaded via --probe=. Validated after the
/// argument loop: --probe-dump and --probe-out= both require it, and that
/// has to hold regardless of the order the options were given in.
static bool probe_loaded_ = false;
/// If set, print the compiled probe script and exit instead of running.
static bool probe_dump_ = false;
/// If set, path from --probe-out=, not yet opened -- opening happens after
/// the argument loop, once we know a script was loaded to write about.
static const char *probe_out_path_ = NULL;
/// Assumed clock frequency. Can be used for "overclocking".
static unsigned clock_freq_ = A2_CLOCK_FREQ;
/// If true, dump key presses with cycle stamps.
static bool trace_keys_ = false;
/// If set, write a per-frame hash of video state to this file.
static FILE *hash_file_ = NULL;
/// If non-zero, quit after this many frames.
static unsigned frame_limit_ = 0;
/// Frames simulated so far.
static unsigned frame_no_ = 0;
/// Set when the engine asks to stop early -- a breakpoint, or a collection
/// limit. The generated engines never do; an interpreter will.
static bool engine_stopped_ = false;
/// If true, run without opening a window or initialising graphics/audio.
static bool headless_ = false;
/// Key-presses loaded from disk.
static KeyPress *key_presses_ = NULL;
/// Number of loaded key presses.
static unsigned key_press_count_ = 0;
/// Next key press to process.
static unsigned next_key_press_ = 0;
/// The very first frame is skipped rather than simulated, so that a front end
/// pacing against a wall clock does not begin with a huge catch-up burst.
static bool firstFrame_ = true;

static a2_iostate_t io_;

a2_iostate_t *a2host_io(void) {
  return &io_;
}

static a2host_elapsed_fn elapsed_fn_ = NULL;
static a2host_frame_done_fn frame_done_fn_ = NULL;

void a2host_set_elapsed_fn(a2host_elapsed_fn fn) {
  elapsed_fn_ = fn;
}
void a2host_set_frame_done_fn(a2host_frame_done_fn fn) {
  frame_done_fn_ = fn;
}

bool a2host_headless(void) {
  return headless_;
}
bool a2host_sound_enabled(void) {
  return sound_enabled_;
}
bool a2host_key_replay_active(void) {
  return key_presses_ != NULL;
}
bool a2host_engine_stopped(void) {
  return engine_stopped_;
}

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
  if (!(g_debug & DebugEmu)) {
    printf("%8u %04X:", get_cycles(), r.pc);
  } else {
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

  while (next_key_press_ != key_press_count_ && cycles >= key_presses_[next_key_press_].cycles) {
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

/// OP_KEY's other half: probe_vm.c has no access to key_presses_,
/// next_key_press_, key_press_count_ or io_, so it calls out to here instead
/// of delivering keys itself the way OP_STOP sets its flag locally. \p now is
/// whatever coordinate the script passed to `key(...)` -- typically a
/// counter -- compared directly against the `cycles` stamps drain_key_presses
/// above compares against get_cycles(). The two draw from the same
/// key_presses_ list and share next_key_press_ as their cursor, so whichever
/// one advances it, the other still sees a consistent view; this function
/// does not free the list on exhaustion because drain_key_presses already
/// does, on whichever frame notices the cursor has reached the end.
void probe_deliver_keys(uint32_t now) {
  while (next_key_press_ != key_press_count_ && key_presses_[next_key_press_].cycles <= now) {
    a2_io_push_key(&io_, key_presses_[next_key_press_].ch);
    ++next_key_press_;
  }
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

void a2host_drain_kbd_file(void) {
  drain_kbd_file();
}
void a2host_push_key_if_empty(uint8_t ch) {
  push_key_if_empty(ch);
}

/// Everything needed to start the emulated program, with no graphics or audio.
/// Shared by the windowed and headless front ends.
void a2host_init_emulation(void) {
  a2_io_init(&io_);
  io_.debug = 0;

  add_default_nondebug();
  reset_regs();
  // SP is 0xF0 in BASIC.
  regs_t r = get_regs();
  r.sp = 0xF0;
  set_regs(r);

  init_emulated();

  if (kbd_file_ || key_presses_) {
    // The first key pressed before initialization is lost, so just add a dummy
    // keypress.
    a2_io_push_key(&io_, '\r');
    if (key_presses_)
      drain_key_presses();
    else
      drain_kbd_file();
  }
}

void a2host_simulate_frame(void) {
  if (firstFrame_) {
    firstFrame_ = false;
  } else {
    if (key_presses_)
      drain_key_presses();
    else if (kbd_file_)
      drain_kbd_file();

    unsigned runCycles;
    // A run that must be reproducible -- replay, hashing, tracing -- always
    // gets exactly a frame's worth of emulated time. So does a front end that
    // installed no clock, which is every console one.
    if (!elapsed_fn_ || trace_keys_ || key_presses_ || hash_file_ ||
        (g_debug & (DebugASM | DebugMem)) != 0) {
      runCycles = (unsigned)((1.0 / 60.0) * clock_freq_);
    } else {
      double elapsed = elapsed_fn_();
      runCycles = (unsigned)((elapsed < 0.200 ? elapsed : 0.200) * clock_freq_);
    }
    run_emulated(runCycles);
    if (engine_stop_reason() == A2_STOP_REQUESTED)
      engine_stopped_ = true;
    if (frame_done_fn_)
      frame_done_fn_();
  }
}

/// FNV-1a over the video state: mode, mixed flag, text page and hires page.
/// Deliberately hashes memory rather than rendered pixels — rendering depends on
/// wall-clock time for the blink phase and would not be reproducible, and memory
/// hashing needs no graphics context, which is what makes headless replay work.
///
/// Both pages are hashed regardless of the current video mode, on purpose: a
/// game may use the currently-undisplayed page as a data structure rather than a
/// display. Snake Byte does exactly this — the never-shown lo-res page is its
/// collision grid, read back via SCRN. The tradeoff is that a rewrite which
/// stores that state differently but renders identically will show up as a
/// mismatch. That is the intended behaviour: it is game state, not scratch.
static uint64_t hash_video_state(void) {
  uint64_t h = 1469598103934665603ULL;
  const uint8_t *ram = get_ram();

  uint8_t header[2] = {
      (uint8_t)a2_io_get_vidmode(&io_), (uint8_t)a2_io_is_vidmode_mixed(&io_)};
  for (unsigned i = 0; i < sizeof(header); ++i) {
    h ^= header[i];
    h *= 1099511628211ULL;
  }

  const uint8_t *text = ram + a2_io_get_text_page_offset(&io_);
  for (unsigned i = 0; i < 0x400; ++i) {
    h ^= text[i];
    h *= 1099511628211ULL;
  }

  const uint8_t *hires = ram + a2_io_get_hires_page_offset(&io_);
  for (unsigned i = 0; i < 0x2000; ++i) {
    h ^= hires[i];
    h *= 1099511628211ULL;
  }

  return h;
}

/// Emit this frame's hash if requested and advance the frame counter.
/// Returns true when the frame limit has been reached.
bool a2host_record_frame(void) {
  if (hash_file_) {
    fprintf(hash_file_, "%u %u %016llx\n", frame_no_, get_cycles(),
            (unsigned long long)hash_video_state());
  }
  ++frame_no_;
  if (frame_limit_ && frame_no_ >= frame_limit_) {
    if (hash_file_) {
      fclose(hash_file_);
      hash_file_ = NULL;
    }
    return true;
  }
  return false;
}

void a2host_shutdown(void) {
  if (hash_file_) {
    fclose(hash_file_);
    hash_file_ = NULL;
  }
  // Before probe_close_output(): once the report file is closed, hit counts
  // are the only record left of what ran, and stderr (never the report file
  // itself -- see probe_report_unfired's comment) is where they belong.
  probe_report_unfired();
  probe_close_output();
  shutdown_emulated();
  a2_io_done(&io_);
}

static const char *s_argv0 = "emu";

const char *a2host_argv0(void) {
  return s_argv0;
}

static void print_help(void) {
  printf("syntax: %s [options]\n", s_argv0);
  printf(" --help           This help\n");
  printf(" --no-sound       Disable sound\n");
  printf(" --kbd-file=path  Read ascii keyboard input from the specified file\n");
  printf(" --key-file=path  Read key presses and cycles from the specified file\n");
  printf(" --fast           Emulate a faster CPU\n");
  printf(" --compat         Debug info compatible with the emulator\n");
  printf(" --trace          Dump state at branch targets\n");
  printf(" --trace-mem      Dump all memory writes\n");
  printf(" --trace-keys     Dump key presses with cycle stamps\n");
  printf(" --hash-frames=p  Write per-frame video state hashes to the given file\n");
  printf(" --frames=n       Quit after simulating n frames\n");
  printf(" --headless       Run with no window. Requires --frames\n");
  printf(" --count-bt       Count branch targets\n");
  printf(" --probe=path     Load a probe script\n");
  printf(" --probe-out=p    Write probe output to the given file\n");
  printf(" --probe-dump     Print the compiled probe script and exit\n");
  engine_print_help();
}

void a2host_parse_args(int argc, char *argv[]) {
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
    if (strcmp(arg, "--compat") == 0) {
      g_debug |= DebugEmu;
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
    if (strncmp(arg, "--hash-frames=", 14) == 0) {
      const char *path = arg + 14;
      if ((hash_file_ = fopen(path, "wt")) == NULL) {
        perror(path);
        exit(2);
      }
      continue;
    }
    if (strncmp(arg, "--frames=", 9) == 0) {
      frame_limit_ = (unsigned)strtoul(arg + 9, NULL, 10);
      continue;
    }
    if (strcmp(arg, "--headless") == 0) {
      headless_ = true;
      sound_enabled_ = false;
      continue;
    }
    if (strncmp(arg, "--probe=", 8) == 0) {
      probe_load_script(arg + 8);
      probe_loaded_ = true;
      continue;
    }
    if (strncmp(arg, "--probe-out=", 12) == 0) {
      probe_out_path_ = arg + 12;
      continue;
    }
    if (strcmp(arg, "--probe-dump") == 0) {
      probe_dump_ = true;
      continue;
    }

    // Not ours -- it may be the engine's. An interpreter has options a
    // generated program cannot have: which ROM, which disk, which binary.
    if (engine_parse_arg(arg))
      continue;

    if (arg[0] == '-') {
      fprintf(stderr, "Invalid option '%s'\n", arg);
      print_help();
      exit(1);
    }
    fprintf(stderr, "Extra command line argument '%s'\n", arg);
    print_help();
    exit(1);
  }

  // Checked here rather than in the handlers above so the result does not
  // depend on the order --probe=, --probe-dump and --probe-out= were given
  // in.
  if (probe_dump_ && !probe_loaded_)
    probe_fatal("--probe-dump requires --probe=<script>");
  if (probe_out_path_ && !probe_loaded_)
    probe_fatal("--probe-out requires --probe=<script>");
  // Before opening --probe-out=: a dump exits immediately and never runs, so
  // it must not truncate an existing report on its way out.
  if (probe_dump_) {
    probe_dump(stdout);
    exit(0);
  }

  if (probe_out_path_)
    probe_set_output_path(probe_out_path_);
}

/// Run to the frame limit with no window, hashing frames as we go. Never
/// returns -- exits the process when the limit is reached.
void a2host_run_headless(void) {
  if (!frame_limit_) {
    fprintf(stderr, "--headless requires --frames=<n>\n");
    exit(2);
  }

  a2host_init_emulation();

  for (;;) {
    a2host_simulate_frame();
    // probe_stop_requested(): a probe's `stop` cannot unwind out of a
    // mid-block engine call, so it sets a flag instead (probe_vm.c) and this
    // is where the host notices it, next to the pre-existing frame-limit and
    // engine_stopped_ checks. Ordering the three with `||` is safe even if
    // more than one becomes true on the same frame -- a2host_record_frame()
    // still runs exactly once per frame regardless of which reason ends the
    // loop, and a2host_shutdown() below closes hash_file_ if record_frame()
    // did not already (e.g. `stop` firing on a frame under the limit).
    if (a2host_record_frame() || engine_stopped_ || probe_stop_requested())
      break;
  }

  a2host_shutdown();
  exit(0);
}
