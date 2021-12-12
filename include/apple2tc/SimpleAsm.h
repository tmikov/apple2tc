/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/d6502.h"

class SimpleAsm {
public:
  struct ExprResult {
    uint16_t value = 0;
    // 1 or 2.
    uint8_t width = 1;
  };

  explicit SimpleAsm(
      std::function<void(char const *, char const *)> &&errorCb,
      std::function<std::optional<ExprResult>(std::string_view)> &&resolveSymbol =
          [](std::string_view) { return std::nullopt; })
      : errorCB_(std::move(errorCb)), resolveSymbol_(std::move(resolveSymbol)) {}

  /// Return the encoded length on success.
  std::optional<uint8_t>
  assemble(ThreeBytes *bytes, uint16_t pc, const char *middle, const char *right, bool emit);

  /// Parse an expression.
  const char *parseExpr(ExprResult *res, const char *s, bool emit);

  static uint8_t parseXDigit(char c) {
    assert(isxdigit(c));
    c = (char)tolower(c);
    return c >= 'a' ? c - 'a' + 10 : c - '0';
  }

  /// Extra characters valid in a label.
  static constexpr char LABEL_CHARS[] = "._?";

private:
  void error(const char *where, const char *msg, ...);

  /// Parse a number or label.
  /// \param emit: if true, we are emitting bytes and labels must be defined.
  const char *parseExpr2(ExprResult *res, const char *s, bool emit);
  /// Parse additions.
  const char *parseExpr1(ExprResult *res, const char *s, bool emit);
  std::optional<CPUAddrMode> parseInstOperand(ExprResult *res, const char *s, bool emit);

private:
  std::function<void(const char *where, const char *msg)> errorCB_;
  std::function<std::optional<ExprResult>(std::string_view name)> resolveSymbol_;
};
