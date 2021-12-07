/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/apple2.h"

#include <cassert>

/// KBD =($C00X)[R] Keyboard Input Register:
/// Gets set to a Key's /// High-ASCII Value when a Key is pressed (e.g., KBD > 127
/// if any Key has been pressed since last cleared via KBDSTRB).
static constexpr uint16_t KBD = 0xC000;
/// KBDSTRB =($C01X)[W] Clear Keyboard Strobe:
/// Resets Keyboard Input /// Register's high bit (from 1 to 0) so that the next
/// keypress can set KBD again.
static constexpr uint16_t KBDSTRB = 0xC010;
/// TAPEOUT =($C02X)[R] Cassette Data Out:
/// Digital to Analog Audio.
/// Output Toggle; Read Only!--Do NOT Write to these
/// addresses [(49184~49199)= ($C020~$C02F)] (which are
/// decoded {hard-wired} as the same single bit location);
/// Toggles Audio Output
/// (creates a 'click' on Cassette-Tape recordings).
static constexpr uint16_t TAPEOUT = 0xC020;
/// SPKR =($C03X)[R/W] Speaker Data Out:
/// Digital to Analog Audio
/// Output Toggle; Read Only!--Do NOT Write to these
/// addresses [(49200~49215)=($C030~ $C03F)] (which are
/// decoded {hard-wired} as the same single bitlocation);
/// Toggles Audio Output
/// (Your Apple's speaker' clicks' once).
static constexpr uint16_t SPKR = 0xC030;
/// STROBE =($C04X); Outputs Strobe Pulse to Game I/O Connector;
/// Any one of these 16 locations
/// [(49216~49231)=($C040~$C04F)] has the same effect
static constexpr uint16_t STROBE = 0xC040;

/// [R/W] Sets Graphics Mode without Clearing Screen;
static constexpr uint16_t TXTCLR = 0xC050;
/// [R/W] Sets Text Mode without Resetting Scrolling Window;
static constexpr uint16_t TXTSET = 0xC051;
/// [R/W] Sets Full-Screen Graphics Mode;
static constexpr uint16_t MIXCLR = 0xC052;
/// [R/W] Sets Mixed Text & Graphics Mode  (with 4 lines of text at the bottom of the screen).
static constexpr uint16_t MIXSET = 0xC053;
/// [R/W] Displays Page 1 without Clearing the Screen.
static constexpr uint16_t LOWSCR = 0xC054;
/// [R/W] Displays Page 2 without Clearing the Screen.
static constexpr uint16_t HISCR = 0xC055;
/// [R/W] Resets Page from Hi-Res to Lo-Res/Text Mode.
static constexpr uint16_t LORES = 0xC056;
/// [R/W] Resets Page from Lo-Res/Text to Hi-Res Mode.
static constexpr uint16_t HIRES = 0xC057;

/// [R/W] CLRAN0: Reset AN0: Toggle OFF (0VDC)
static constexpr uint16_t AN0OFF = 0xC058;
/// [R/W] SETAN0: * Set AN0: Toggle ON (+5VDC)
static constexpr uint16_t AN0ON = 0xC059;
/// [R/W] CLRAN1: Reset AN1: Toggle OFF (0VDC)
static constexpr uint16_t AN1OFF = 0xC05A;
/// [R/W] SETAN1: * Set AN1: Toggle ON (+5VDC)
static constexpr uint16_t AN1ON = 0xC05B;
/// [R/W] CLRAN2: Reset AN2: Toggle OFF (0VDC)
static constexpr uint16_t AN2OFF = 0xC05C;
/// [R/W] SETAN2: * Set AN2: Toggle ON (+5VDC)
static constexpr uint16_t AN2ON = 0xC05D;
/// [R/W] CLRAN3: Reset AN3: Toggle OFF (0VDC)
static constexpr uint16_t AN3OFF = 0xC05E;
/// [R/W] SETAN3: * Set AN3: Toggle ON (+5VDC)
static constexpr uint16_t AN3ON = 0xC05F;

uint8_t EmuApple2::kbd() {
  return keys_.empty() ? lastKey_ : keys_.front() | 0x80;
}

void EmuApple2::kbdstrb() {
  if (!keys_.empty()) {
    lastKey_ = keys_.front() & 0x7F;
    keys_.erase(keys_.begin());
  }
}

uint8_t EmuApple2::ioPeek(uint16_t addr) {
  assert(addr >= IO_RANGE_START && addr <= IO_RANGE_END);

  switch (addr & 0xCFF0) {
  case KBD:
    if (debug_ & DebugIO2)
      fprintf(stderr, "[%u] KBD\n", getCycles());
    return kbd();
  case KBDSTRB:
    if (debug_ & DebugIO1)
      fprintf(stderr, "[%u] KBDSTRB\n", getCycles());
    kbdstrb();
    break;
  case TAPEOUT:
    if (debug_ & DebugIO1)
      fprintf(stderr, "[%u] TAPEOUT\n", getCycles());
    break;
  case SPKR:
    if (debug_ & DebugIO2)
      fprintf(stderr, "[%u] SPKR\n", getCycles());
    break;
  case STROBE:
    if (debug_ & DebugIO1)
      fprintf(stderr, "[%u] STROBE\n", getCycles());
    break;

  case TXTCLR:
    switch (addr) {
    case TXTCLR:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] TXTCLR\n", getCycles());
      vidControl_ &= ~VCText;
      break;
    case TXTSET:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] TXTSET\n", getCycles());
      vidControl_ |= VCText;
      break;
    case MIXCLR:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] MIXCLR\n", getCycles());
      vidControl_ &= ~VCMixed;
      break;
    case MIXSET:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] MIXSET\n", getCycles());
      vidControl_ |= VCMixed;
      break;
    case LOWSCR:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] LOWSCR\n", getCycles());
      vidControl_ &= ~VCPage2;
      break;
    case HISCR:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] HISCR\n", getCycles());
      vidControl_ |= VCPage2;
      break;
    case LORES:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] LORES\n", getCycles());
      vidControl_ &= ~VCHires;
      break;
    case HIRES:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] HIRES\n", getCycles());
      vidControl_ |= VCHires;
      break;
    default:
      if (debug_ & DebugIO1)
        fprintf(stderr, "[%u] ANNUNCIATORS $%04X\n", getCycles(), addr);
      break;
    }
    break;

  default:
    fprintf(stderr, "[%u] Unsupported IO location read $%04X\n", getCycles(), addr);
  }

  return 0;
}

void EmuApple2::ioPoke(uint16_t addr, uint8_t value) {
  if (debug_ & DebugIO1)
    fprintf(stderr, "[%u] IOPOKE\n", getCycles());
  EmuApple2::ioPeek(addr);
}
