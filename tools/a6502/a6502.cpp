/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/SimpleAsm.h"
#include "apple2tc/d6502.h"
#include "apple2tc/support.h"

#include <cassert>
#include <cctype>
#include <cstdarg>
#include <cstring>
#include <optional>
#include <unordered_map>
#include <vector>

struct Location {
  unsigned line = 0, col = 0;
};

struct SymbolDef {
  std::optional<uint16_t> value{};
  Location defined{};
};

using ExprResult = SimpleAsm::ExprResult;

static std::string s_inputPath;
static std::string s_inputData;
static const char *s_curPtr;
static const char *s_curLineStart;
unsigned s_curLine;

static uint16_t s_curAddr = 0;
static std::optional<uint16_t> s_startAddr;

static const char *s_labelStart;
static const char *s_middleStart;
static const char *s_rightStart;
static std::string s_label;
static std::string s_middle;
static std::string s_right;

static std::unordered_map<std::string, SymbolDef> s_symbols{};

static unsigned s_numErrors = 0;
static unsigned s_curPass = 0;

static std::vector<uint8_t> s_binary{};

static FILE *s_listing = nullptr;

static constexpr uint8_t FILLER = 0xFF;

static uint8_t get8(uint16_t addr) {
  if (s_curPass != 2)
    return 0;
  auto start = *s_startAddr;
  if (addr < start || addr >= start + s_binary.size())
    return FILLER;
  return s_binary[addr - start];
}

static void put8(uint16_t addr, uint8_t data) {
  if (s_curPass != 2)
    return;

  addr -= *s_startAddr;
  if (addr >= s_binary.size())
    s_binary.resize(addr + 1, FILLER);
  s_binary[addr] = data;
}

static void put16(uint16_t addr, uint16_t data) {
  put8(addr, data);
  put8(addr + 1, data >> 8);
}

static void error(const char *errLoc, const char *format, ...)
    __attribute__((__format__(__printf__, 2, 3)));
static void error(const char *errLoc, const char *format, ...) {
  ++s_numErrors;

  if (!s_label.empty() && errLoc >= &s_label.front() && errLoc <= &s_label.back())
    errLoc = s_labelStart + (errLoc - &s_label.front());
  else if (!s_middle.empty() && errLoc >= &s_middle.front() && errLoc <= &s_middle.back())
    errLoc = s_middleStart + (errLoc - &s_middle.front());
  else if (!s_right.empty() && errLoc >= &s_right.front() && errLoc <= &s_right.back())
    errLoc = s_rightStart + (errLoc - &s_right.front());
  else {
    assert(!errLoc || errLoc >= s_curLineStart && errLoc < s_inputData.data() + s_inputData.size());
  }

  fprintf(
      stderr,
      "%s:%u:%zu: ",
      s_inputPath.c_str(),
      s_curLine,
      errLoc ? errLoc - s_curLineStart + 1 : 0);
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  fprintf(stderr, "\n");
}

static SimpleAsm s_asm(
    [](const char *where, const char *msg) { error(where, "%s", msg); },
    [](std::string_view label) -> std::optional<SimpleAsm::ExprResult> {
      auto it = s_symbols.find(std::string(label));
      if (it != s_symbols.end() && it->second.value.has_value()) {
        SimpleAsm::ExprResult res;
        res.value = *it->second.value;
        // If the label is defined after the current line, we treat is as 16-bit.
        res.width = it->second.defined.line > s_curLine || *it->second.value > 255 ? 2 : 1;
        return res;
      }
      return std::nullopt;
    });

static void skipBlanks() {
  for (;;) {
    if (*s_curPtr == ';') {
      do
        ++s_curPtr;
      while (*s_curPtr && *s_curPtr != '\n');
      break;
    } else if (*s_curPtr != '\n' && isspace(*s_curPtr))
      ++s_curPtr;
    else
      break;
  }
}

static inline bool isSpaceStart() {
  return *s_curPtr == ';' || isspace(*s_curPtr);
}

static inline bool isContent() {
  return *s_curPtr && *s_curPtr != ';' && !isspace(*s_curPtr);
}

static const char *skip(const char *s) {
  while (isspace(*s))
    ++s;
  return s;
}

/// Parse the line into label, middle and right parts.
static void parseLine() {
  s_curLineStart = s_curPtr;
  s_labelStart = nullptr;
  s_middleStart = nullptr;
  s_rightStart = nullptr;
  s_label.clear();
  s_middle.clear();
  s_right.clear();

  // A label?
  if (isContent()) {
    s_labelStart = s_curPtr;
    do
      ++s_curPtr;
    while (isContent());
    const char *end = s_curPtr[-1] == ':' ? s_curPtr - 1 : s_curPtr;
    s_label.append(s_labelStart, end - s_labelStart);
    skipBlanks();
  } else {
    skipBlanks();
  }

  // [label] blanks ^HERE
  while (isContent()) {
    // Collect the middle part.
    const char *midStart = s_curPtr;
    do
      ++s_curPtr;
    while (isContent());
    // This might still be a label.
    if (s_label.empty() && s_curPtr[-1] == ':') {
      s_labelStart = midStart;
      s_label.append(midStart, s_curPtr - 1 - midStart);
      skipBlanks();
    } else {
      s_middleStart = midStart;
      s_middle.append(midStart, s_curPtr - midStart);
      skipBlanks();
      break;
    }
  }

  if (isContent()) {
    s_rightStart = s_curPtr;
    char endCh = 0;
    while (*s_curPtr && *s_curPtr != '\n') {
      if (*s_curPtr == endCh) {
        endCh = 0;
      } else if (endCh) {
        // Do nothing.
      } else if (*s_curPtr == '\'' || *s_curPtr == '"') {
        endCh = *s_curPtr;
      } else if (*s_curPtr == ';') {
        break;
      }
      ++s_curPtr;
    }
    while (*s_curPtr && *s_curPtr != '\n' && *s_curPtr != ';')
      ++s_curPtr;
    s_right.append(s_rightStart, s_curPtr - s_rightStart);
    skipBlanks();

    // Check for "   +" continuation on the next line
    for (;;) {
      const char *s = s_curPtr;
      if (*s != '\n')
        break;
      s = skip(s + 1);
      if (*s != '+')
        break;

      s_curLineStart = s_curPtr + 1;
      ++s_curLine;
      s_curPtr = s + 1;
      skipBlanks();
      const char *start = s_curPtr;
      endCh = 0;
      while (*s_curPtr && *s_curPtr != '\n') {
        if (*s_curPtr == endCh) {
          endCh = 0;
        } else if (endCh) {
          // Do nothing.
        } else if (*s_curPtr == '\'' || *s_curPtr == '"') {
          endCh = *s_curPtr;
        } else if (*s_curPtr == ';') {
          break;
        }
        ++s_curPtr;
      }
      s_right.append(start, s_curPtr - start);
      skipBlanks();
    }

    while (!s_right.empty() && isspace(s_right.back()))
      s_right.pop_back();
  }

  if (*s_curPtr == '\n')
    ++s_curPtr;

  for (char &ch : s_label)
    ch = (char)toupper(ch);
  for (char &ch : s_middle)
    ch = (char)toupper(ch);
  for (char &ch : s_right)
    ch = (char)toupper(ch);

  if (!s_label.empty()) {
    if (isdigit(s_label[0])) {
      error(s_labelStart, "invalid label");
    } else {
      for (char c : s_label) {
        if (!isalnum(c) && !strchr(SimpleAsm::LABEL_CHARS, c)) {
          error(s_labelStart, "invalid label");
          break;
        }
      }
    }
  }
}

static void defineLabel(uint16_t value) {
  if (s_curPass == 1 && !s_label.empty()) {
    auto &sr = s_symbols[s_label];
    sr.value = value;
    sr.defined.line = s_curLine;
    sr.defined.col = s_labelStart - s_curLineStart + 1;
  }
}

/// Keep track of the smallest binary address.
static void updateStartAddr() {
  if (s_curPass == 1 && !s_startAddr || s_curAddr < s_startAddr.value())
    s_startAddr = s_curAddr;
}

static std::string curLineString() {
  return std::string(s_curLineStart, s_curPtr - 1 - s_curLineStart);
}

static void printCurLineNoCode() {
  if (s_curPass == 2 && s_listing)
    fprintf(s_listing, "%-19s%s\n", "", curLineString().c_str());
}

enum class Dir { None, EQU, ORG, DCI, ZSTR, ASC, HEX, DFB, DW, DS };

static Dir matchDir() {
  if (s_middle == "EQU" || s_middle == "=")
    return Dir::EQU;
  if (s_middle == "ORG")
    return Dir::ORG;
  if (s_middle == "DCI")
    return Dir::DCI;
  if (s_middle == ".ZSTR")
    return Dir::ZSTR;
  if (s_middle == "ASC")
    return Dir::ASC;
  if (s_middle == "HEX")
    return Dir::HEX;
  if (s_middle == "DFB")
    return Dir::DFB;
  if (s_middle == "DW")
    return Dir::DW;
  if (s_middle == "DS")
    return Dir::DS;

  return Dir::None;
}

static void analyzeLine() {
  uint16_t prevAddr = s_curAddr;

  if (s_curPass == 1 && !s_label.empty()) {
    // Check for label redefinition.
    auto it = s_symbols.find(s_label);
    if (it != s_symbols.end() && it->second.value.has_value()) {
      error(
          s_labelStart,
          "Symbol '%s' already defined on line %u",
          s_label.c_str(),
          it->second.defined.line);
    }
  }

  Dir dir = matchDir();

  ExprResult res;
  if (dir == Dir::EQU || dir == Dir::ORG) {
    const char *s = skip(s_asm.parseExpr(&res, s_right.c_str(), true));
    if (*s)
      error(s, "invalid expression");
    defineLabel(res.value);
    if (dir == Dir::ORG) {
      s_curAddr = res.value;
      updateStartAddr();
    }

    printCurLineNoCode();
    return;
  }

  // EQU/ORG is the only case which sets the label to something other than the current
  // address.
  defineLabel(s_curAddr);

  if (s_middle.empty()) {
    printCurLineNoCode();
    return;
  }

  updateStartAddr();

  if (dir == Dir::DCI || dir == Dir::ZSTR || dir == Dir::ASC) {
    const char *s = s_right.c_str();
    size_t len = 0;
    bool caret = false;
    while (*s) {
      if (s[0] == '^' && s[1] == '"') {
        caret = true;
        ++s;
      }
      if (*s == '"') {
        const char *start = s + 1;
        for (;;) {
          ++s;
          if (*s == 0) {
            error(s, "non-terminated string");
            return;
          }
          if (*s == '"')
            break;
          if (dir == Dir::DCI || dir == Dir::ZSTR)
            put8(s_curAddr++, *s);
          else
            put8(s_curAddr++, caret ? *s | 0x80 : *s);
        }
        ++s;
        caret = false;
      } else {
        const char *start = s;
        s = s_asm.parseExpr(&res, s, s_curPass > 1);
        put8(s_curAddr++, res.value);
      }

      s = skip(s);
      if (*s == 0)
        break;
      if (*s != ',') {
        error(s, ", expected");
        return;
      }
      // Skip the ','.
      s = skip(s + 1);
    }

    if (dir == Dir::DCI) {
      if (s_curAddr != prevAddr)
        put8(s_curAddr - 1, get8(s_curAddr - 1) | 0x80);
    } else if (dir == Dir::ZSTR) {
      put8(s_curAddr++, 0);
    }
  } else if (dir == Dir::HEX) {
    if (s_right.size() % 2) {
      error(s_rightStart, "HEX operand must be even number of digits");
      return;
    }
    for (const char *s = s_right.c_str(); *s; s += 2) {
      if (!isxdigit(s[0]) || !isxdigit(s[1])) {
        error(s, "invalid HEX number");
        return;
      }
      put8(s_curAddr++, SimpleAsm::parseXDigit(s[0]) * 16 + SimpleAsm::parseXDigit(s[1]));
    }
  } else if (dir == Dir::DFB || dir == Dir::DW) {
    const char *s = skip(s_right.c_str());
    for (;;) {
      const char *start = s;
      s = skip(s_asm.parseExpr(&res, s, s_curPass > 1));
      if (dir == Dir::DFB) {
        put8(s_curAddr++, res.value);
      } else {
        put16(s_curAddr, res.value);
        s_curAddr += 2;
      }

      if (*s == ',')
        s = skip(s + 1);
      else
        break;
    }
  } else if (dir == Dir::DS) {
    const char *s = s_right.c_str();
    s = skip(s_asm.parseExpr(&res, s, true));
    uint32_t repeat = res.value;
    if (*s != ',') {
      error(s, ", expected");
      res.width = 1;
    } else {
      s = skip(s_asm.parseExpr(&res, skip(s + 1), s_curPass > 1));
      if (*s)
        error(s, "invalid expression");
    }

    if (res.width * repeat > UINT16_MAX) {
      error(s_rightStart, "16-bit integer overflow");
      repeat = 1;
    }

    while (repeat--) {
      if (res.width == 2)
        put16(s_curAddr, res.value);
      else
        put8(s_curAddr, res.value);
      s_curAddr += res.width;
    }
  } else {
    ThreeBytes bytes;
    if (auto optLen =
            s_asm.assemble(&bytes, s_curAddr, s_middle.c_str(), s_right.c_str(), s_curPass > 1)) {
      auto len = *optLen;
      if (s_curPass == 2) {
        for (unsigned i = 0; i != len; ++i)
          put8(s_curAddr + i, bytes.d[i]);
      }
      s_curAddr += len;
    }
  }
  if (s_curPass == 2 && s_listing) {
    fprintf(s_listing, "%04X:", prevAddr);
    unsigned i = 0;
    uint32_t curAddr = s_curAddr >= prevAddr ? s_curAddr : 0x10000 + s_curAddr;
    for (; i < 4 && i < curAddr - prevAddr; ++i) {
      fprintf(s_listing, " %02X", get8(prevAddr + i));
    }
    for (; i < 4; ++i)
      fprintf(s_listing, "   ");
    if (curAddr - prevAddr > i)
      fprintf(s_listing, "+ ");
    else
      fprintf(s_listing, "  ");
    printf("%s\n", curLineString().c_str());
  }
}

static void run() {
  s_curAddr = 0;
  s_curPtr = s_inputData.c_str();
  s_curLine = 1;
  s_curPass = 1;

  while (*s_curPtr) {
    parseLine();
    analyzeLine();
    ++s_curLine;
  }

  if (s_numErrors)
    return;

  s_curAddr = 0;
  s_curPtr = s_inputData.c_str();
  s_curLine = 1;
  s_curPass = 2;

  while (*s_curPtr) {
    parseLine();
    analyzeLine();
    ++s_curLine;
  }
}

static void printHelp(const char **argv) {
  fprintf(stderr, "syntax: %s [options] input_file [output_file]\n", argv[0]);
  fprintf(stderr, "  --lst    print listing to stdout\n");
  fprintf(stderr, "  --syms   print symbol list to stdout\n");
  fprintf(stderr, "  --csyms  print symbol list to stdout as C code\n");
  fprintf(stderr, "  --bin    if output file is specified, generate binary instead of b33\n");
}

int main(int argc, const char **argv) {
  bool lst = false;
  bool dsyms = false;
  bool dcsyms = false;
  bool b33 = true;
  std::string outPath{};

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--lst") == 0) {
      lst = true;
      continue;
    }
    if (strcmp(argv[i], "--syms") == 0) {
      dsyms = true;
      continue;
    }
    if (strcmp(argv[i], "--csyms") == 0) {
      dcsyms = true;
      continue;
    }
    if (strcmp(argv[i], "--bin") == 0) {
      b33 = false;
      continue;
    }
    if (argv[i][0] == '-') {
      printHelp(argv);
      return 1;
    }
    if (s_inputPath.empty()) {
      s_inputPath = argv[i];
      continue;
    }
    if (outPath.empty()) {
      outPath = argv[i];
      continue;
    }
    fprintf(stderr, "too many arguments\n");
    printHelp(argv);
    return 1;
  }
  if (s_inputPath.empty()) {
    printHelp(argv);
    return 1;
  }

  FILE *f = fopen(s_inputPath.c_str(), "rt");
  if (!f) {
    perror(s_inputPath.c_str());
    return 1;
  }
  s_inputData = readAll<std::string>(f);
  fclose(f);

  if (lst)
    s_listing = stdout;

  run();
  if (s_numErrors)
    return 2;

  uint16_t startAddr = s_startAddr.value_or(0);
  if (!lst) {
    printf("START : $%04X\n", startAddr);
    printf("LENGTH: %zu\n", s_binary.size());
  }

  if (dsyms) {
    struct Sym {
      const char *name;
      const SymbolDef *def;
    };
    std::vector<Sym> syms{};
    syms.reserve(s_symbols.size());
    for (const auto &p : s_symbols)
      syms.push_back({p.first.c_str(), &p.second});

    std::sort(syms.begin(), syms.end(), [](const Sym &a, const Sym &b) {
      return strcmp(a.name, b.name) < 0;
    });

    for (const auto &p : syms) {
      printf("%-12s  %s$%X\n", p.name, p.def->value ? "" : "?", p.def->value ? *p.def->value : 0);
    }
  }
  if (dcsyms) {
    struct Sym {
      const char *name;
      const SymbolDef *def;
    };
    std::vector<Sym> syms{};
    syms.reserve(s_symbols.size());
    for (const auto &p : s_symbols)
      syms.push_back({p.first.c_str(), &p.second});

    std::sort(syms.begin(), syms.end(), [](const Sym &a, const Sym &b) {
      return a.def->value.value_or(0x10000) < b.def->value.value_or(0x10000);
    });

    uint32_t lastAddr = UINT32_MAX;
    for (const auto &p : syms) {
      if (!p.def->value)
        continue;
      if (*p.def->value == lastAddr)
        continue;
      lastAddr = *p.def->value;
      printf("  {0x%04x, \"%s\"},\n", *p.def->value, p.name);
    }
  }

  if (!outPath.empty()) {
    FILE *out = fopen(outPath.c_str(), "wb");
    if (!out) {
      perror(outPath.c_str());
      return 1;
    }

    if (b33) {
      fputc(startAddr, out);
      fputc(startAddr >> 8, out);
      fputc((int)s_binary.size(), out);
      fputc((int)s_binary.size() >> 8, out);
    }
    fwrite(s_binary.data(), 1, s_binary.size(), out);
    fclose(out);
  }

  return 0;
}
