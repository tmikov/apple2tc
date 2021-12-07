/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/emu6502.h"

#include <cstdio>
#include <vector>

class EmuApple2 : public Emu6502 {
public:
  /// TXTTAB Applesoft Start of Program Pointer (2B).
  static constexpr uint16_t TXTTAB = 0x67;
  /// PRGEND Applesoft End of Program Pointer (2B).
  static constexpr uint16_t PRGEND = 0xAF;

  static constexpr uint16_t TXT1SCRN = 0x0400;
  static constexpr uint16_t TXT2SCRN = 0x0800;
  static constexpr uint16_t HGR1SCRN = 0x2000;
  static constexpr uint16_t HGR2SCRN = 0x4000;

  /// Enqueue a new key press.
  /// IO space range.
  ///
  /// In the future we may want to support dynamic callbacks per page, and
  /// register device behavior, etc, but for now this is sufficient.
  static constexpr uint16_t IO_RANGE_START = 0xC000;
  static constexpr uint16_t IO_RANGE_END = 0xCFFF;

  EmuApple2() : Emu6502(IO_RANGE_START, IO_RANGE_END) {}

  void pushKey(int key) {
    keys_.push_back(key & 0x7F);
  }

  /// Provide access to the keyboard queue.
  std::vector<uint8_t> &getKeys() {
    return keys_;
  }

  /// Encoding of the video control bits.
  enum VidControl {
    /// When set, text, otherwise graphics (default set).
    VCText = 1,
    /// When set, mixed graphics with 4-lines of text below (default clr).
    VCMixed = 2,
    /// When set, display page 2 (default clr).
    VCPage2 = 4,
    /// When set, high resolution graphics (default clr),
    VCHires = 8,
  };

  /// Return the status of the video control bits.
  [[nodiscard]] uint8_t getVidControl() const {
    return vidControl_;
  }

  /// Video mode.
  enum class VidMode { TEXT, GR, HGR };

  /// Decode and return the vid control bits as a video mode.
  [[nodiscard]] VidMode getVidMode() const {
    return vidControl_ & VCText ? VidMode::TEXT
        : vidControl_ & VCHires ? VidMode::HGR
                                : VidMode::GR;
  }

  /// Return true if this is a mixed graphics mode (graphics with 4 lines of
  /// text in the bottom).
  [[nodiscard]] bool isVidMixed() const {
    return (vidControl_ & (VCText | VCMixed)) == VCMixed;
  }

  /// Decode the vid ctrl bits to return an actuve page number: 0 or 1.
  [[nodiscard]] uint8_t getVidPageNo() const {
    return (vidControl_ & VCPage2) ? 1 : 0;
  }

  /// Return the starting address of the current video page.
  [[nodiscard]] uint16_t getVidPageAddr() const {
    return vidControl_ & VCText ? (vidControl_ & VCPage2 ? TXT2SCRN : TXT1SCRN)
                                : (vidControl_ & VCPage2 ? HGR2SCRN : HGR1SCRN);
  }

protected:
  /// Perform a read in the IO range.
  uint8_t ioPeek(uint16_t addr) override;
  /// Perform a write in the IO range.
  void ioPoke(uint16_t addr, uint8_t value) override;

private:
  uint8_t kbd();
  void kbdstrb();

private:
  /// Input keyboard queue.
  std::vector<uint8_t> keys_{};
  /// The last key that was returned.
  uint8_t lastKey_ = 0;

  uint8_t vidControl_ = VCText;
};

/// Dump the BASIC program as binary tokens.
void dumpApplesoftBasic(FILE *f, const EmuApple2 *emu);
