/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/SimpleAsm.h"

#include "apple2tc/d6502.h"
#include "apple2tc/support.h"

#include <cstdarg>
#include <cstring>
#include <functional>
#include <optional>
#include <string_view>

static const char *skip(const char *s) {
  while (isspace(*s))
    ++s;
  return s;
}

void SimpleAsm::error(const char *where, const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  errorCB_(where, vformat(msg, ap).c_str());
  va_end(ap);
}

/// Parse a number or label.
/// \param emit: if true, labels must be defined.
const char *SimpleAsm::parseExpr2(ExprResult *res, const char *s, bool emit) {
  s = skip(s);
  if (*s == '%') {
    // Binary.
    ++s;
    res->value = 0;
    const char *start = s;
    for (; *s == '0' || *s == '1'; ++s) {
      if (s - start == 16)
        error(s, "binary number overflow");
      else if (s - start < 16)
        res->value = (res->value << 1) + (*s - '0');
    }
    if (s == start)
      error(s, "invalid binary number");
    res->width = res->value > 255;
  } else if (*s == '$') {
    // Hex.
    ++s;
    res->value = 0;
    const char *start = s;
    for (; isxdigit(*s); ++s) {
      if (s - start == 4)
        error(s, "binary number overflow");
      else if (s - start < 4)
        res->value = (res->value << 4) + parseXDigit(*s);
    }
    if (s == start)
      error(s, "invalid hex number");
    res->width = res->value > 255 ? 2 : 1;
  } else if (isdigit(*s)) {
    res->value = 0;
    const char *start = s;
    bool ovf = false;
    for (; isdigit(*s); ++s) {
      if (ovf)
        continue;
      uint32_t tmp = (uint32_t)res->value * 10 + *s - '0';
      if (tmp > 0xFFFF) {
        ovf = true;
        error(s, "decimal number overflow");
      } else {
        res->value = (uint16_t)tmp;
      }
    }
    res->width = res->value > 255 ? 2 : 1;
  } else if (*s == '\'') {
    ++s;
    res->width = 1;
    res->value = (uint8_t)*s;
    if (!*s || s[1] != '\'') {
      error(s, "unterminated character");
    } else {
      s += 2;
    }
  } else {
    // By default we assume labels are 16-bit.
    res->value = 0;
    res->width = 2;
    const char *start = s;
    for (; *s && (isalnum(*s) || strchr(LABEL_CHARS, *s)); ++s)
      ;
    if (s == start) {
      error(s, "missing label");
    } else {
      auto label = std::string_view(start, s - start);
      if (auto labValue = resolveSymbol_(label)) {
        *res = *labValue;
      } else if (emit) {
        error(start, "undefined label '%s'", std::string(label).c_str());
      }
    }
  }

  return s;
}

/// Parse additions.
const char *SimpleAsm::parseExpr1(ExprResult *res, const char *s, bool emit) {
  s = skip(parseExpr2(res, s, emit));
  while (*s == '+' || *s == '-' || *s == '|') {
    char op = *s;
    ExprResult tmp;
    s = skip(parseExpr2(&tmp, s + 1, emit));
    switch (op) {
    case '+':
      res->value += tmp.value;
      break;
    case '-':
      res->value -= tmp.value;
      break;
    case '|':
      res->value |= tmp.value;
      break;
    }
    res->width = std::max(res->width, tmp.width);
  }
  return s;
}

// < low byte
// > high byte
// #X+1
const char *SimpleAsm::parseExpr(ExprResult *res, const char *s, bool emit) {
  s = skip(s);
  if (*s == '<') {
    s = parseExpr1(res, s + 1, emit);
    res->width = 1;
    res->value &= 0xFF;
  } else if (*s == '>') {
    s = parseExpr1(res, s + 1, emit);
    res->width = 1;
    res->value >>= 8;
  } else {
    s = parseExpr1(res, s, emit);
  }

  return skip(s);
}

std::optional<CPUAddrMode> SimpleAsm::parseInstOperand(ExprResult *res, const char *s, bool emit) {
  const char *start = s;
  if (*s == 0)
    return CPUAddrMode::Implied;
  if (eqci(s, "A"))
    return CPUAddrMode::A;

  if (*s == '#') {
    s = skip(parseExpr(res, s + 1, emit));
    if (*s) {
      error(s, "invalid expression");
      return std::nullopt;
    }
    if (res->width == 2) {
      error(start, "16-bit immediate value");
      return std::nullopt;
    }
    return CPUAddrMode::Imm;
  }

  if (*s == '(') {
    s = skip(parseExpr(res, s + 1, emit));
    if (*s == ',') {
      s = skip(s + 1);
      if (!eqci(s, "X)")) {
        error(s, "X) expected");
        return std::nullopt;
      }
      return CPUAddrMode::X_Ind;
    }
    if (*s != ')') {
      error(s, ") expected");
      return std::nullopt;
    }
    s = skip(s + 1);
    if (*s == 0)
      return CPUAddrMode::Ind;
    if (*s != ',') {
      error(s, ", expected");
      return std::nullopt;
    }
    s = skip(s + 1);
    if (!eqci(s, "Y")) {
      error(s, "Y expected");
      return std::nullopt;
    }
    return CPUAddrMode::Ind_Y;
  }

  s = skip(parseExpr(res, s, emit));
  if (*s == ',') {
    s = skip(s + 1);
    if (eqci(s, "X")) {
      return res->width == 2 ? CPUAddrMode::Abs_X : CPUAddrMode::Zpg_X;
    } else if (eqci(s, "Y")) {
      return res->width == 2 ? CPUAddrMode::Abs_Y : CPUAddrMode::Zpg_Y;
    } else {
      error(s, "X or Y expected");
      return std::nullopt;
    }
  }

  if (*s) {
    error(s, "invalid expression");
    return std::nullopt;
  }

  return res->width == 2 ? CPUAddrMode::Abs : CPUAddrMode::Zpg;
}

std::optional<uint8_t> SimpleAsm::assemble(
    ThreeBytes *bytes,
    uint16_t pc,
    const char *middle,
    const char *right,
    bool emit) {
  auto optKind = findInstKind(middle);
  if (!optKind) {
    error(middle, "Unknown instruction '%s'", middle);
    return std::nullopt;
  }

  ExprResult res;
  std::optional<uint8_t> encoding;
  auto optAm = parseInstOperand(&res, right, emit);
  if (!optAm)
    return std::nullopt;
  CPUAddrMode am = *optAm;
  encoding = encodeInst(CPUOpcode{.kind = *optKind, .addrMode = am});

  if (!encoding) {
    bool tryIt = true;
    switch (am) {
    case CPUAddrMode::Zpg:
      am = CPUAddrMode::Abs;
      break;
    case CPUAddrMode::Zpg_X:
      am = CPUAddrMode::Abs_X;
      break;
    case CPUAddrMode::Zpg_Y:
      am = CPUAddrMode::Abs_Y;
      break;
    default:
      tryIt = false;
      break;
    }
    if (tryIt)
      encoding = encodeInst(CPUOpcode{.kind = *optKind, .addrMode = am});
  }
  if (!encoding && am == CPUAddrMode::Abs) {
    am = CPUAddrMode::Rel;
    encoding = encodeInst(CPUOpcode{.kind = *optKind, .addrMode = am});
  }
  if (!encoding && *optKind == CPUInstKind::BRK && am == CPUAddrMode::Implied) {
    // BRK is also recognized as a 1-byte instruction.
    encoding = 0;
  }
  if (!encoding) {
    error(*right ? right : middle, "unsupported address mode for '%s'", middle);
    return std::nullopt;
  }

  if (emit) {
    bytes->d[0] = encoding.value();
    switch (am) {
    default:
    case CPUAddrMode::A:
    case CPUAddrMode::Implied:
      break;

    case CPUAddrMode::Abs:
    case CPUAddrMode::Ind:
    case CPUAddrMode::Abs_X:
    case CPUAddrMode::Abs_Y:
      bytes->d[1] = res.value;
      bytes->d[2] = res.value >> 8;
      break;

    case CPUAddrMode::Rel: {
      int32_t d = (int32_t)res.value - pc - 2;
      if (d < INT8_MIN || d > INT8_MAX)
        error(right, "relative branch out of range");
      bytes->d[1] = d;
      break;
    }

    case CPUAddrMode::Imm:
    case CPUAddrMode::X_Ind:
    case CPUAddrMode::Ind_Y:
    case CPUAddrMode::Zpg:
    case CPUAddrMode::Zpg_X:
    case CPUAddrMode::Zpg_Y:
      bytes->d[1] = res.value;
      break;
    }
  }

  return cpuInstSize(am);
}
