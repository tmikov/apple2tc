/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/SimpleAsm.h"
#include "apple2tc/a2symbols.h"
#include "apple2tc/d6502.h"
#include "apple2tc/support.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <string>
#include <vector>

static uint8_t s_memory[0x10000];
static uint16_t s_curAddr = 0;

static uint8_t peek(unsigned addr) {
  return s_memory[addr & 0xFFFF];
}
static void poke(unsigned addr, uint8_t value) {
  s_memory[addr & 0xFFFF] = value;
}
static uint16_t peek16(unsigned addr) {
  return peek(addr) + peek(addr + 1) * 256;
}
static void poke16(unsigned addr, uint16_t value) {
  poke(addr, value);
  poke(addr + 1, value >> 8);
}

static uint8_t printInst(uint16_t pc) {
  ThreeBytes bytes{0};
  for (unsigned i = 0; i != 3; ++i)
    bytes.d[i] = peek(pc + i);
  CPUInst inst = decodeInst(pc, bytes);
  FormattedInst fmt = formatInst(inst, bytes, apple2SymbolResolver);

  printf("%04X: %-8s    %s", pc, fmt.bytes, fmt.inst);
  if (!fmt.operand.empty())
    printf("  %s", fmt.operand.c_str());
  printf("\n");
  return inst.size;
}

static void loadROM(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    perror(path);
    return;
  }

  auto buf = readAll<std::vector<uint8_t>>(f);

  if (buf.size() > 0x10000) {
    printf("*** Error: ROM is roo large\n");
  } else {
    unsigned start = 0x10000 - buf.size();
    printf("loaded: [$%04X..$%04X]\n", start, (unsigned)(start + buf.size() - 1));
    printf("len   : %u\n", (unsigned)buf.size());

    memcpy(s_memory + start, buf.data(), buf.size());
  }

  fclose(f);
}

static void loadB33(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    perror(path);
    return;
  }

  auto buf = readAll<std::vector<uint8_t>>(f);

  if (buf.size() > 0x10000) {
    printf("*** Error: bin is too large\n");
  } else if (buf.size() < 4) {
    printf("*** Error: bin is too small\n");
  } else {
    uint16_t start = buf[0] + buf[1] * 256;
    uint16_t len = buf[2] + buf[3] * 256;

    printf("loaded: [$%04X..$%04X]\n", start, start + len - 1);
    printf("len   : %u\n", len);

    if (len > buf.size() - 4) {
      printf("***Error: invalid length\n");
    } else {
      memcpy(s_memory + start, buf.data() + 4, len);
    }
  }

  fclose(f);
}

void saveB33(const char *path, uint16_t addr, uint16_t len) {
  assert(addr <= 0x10000 - len && "length should have been validated");
  FILE *f = fopen(path, "wb");
  if (!f) {
    perror(path);
    return;
  }

  fputc(addr, f);
  fputc(addr >> 8, f);
  fputc(len, f);
  fputc(len >> 8, f);
  fwrite(s_memory + addr, 1, len, f);
  fclose(f);

  printf("saved: [$%04X..$%04X] to %s\n", addr, addr + len - 1, path);
  printf("len  : %u\n", len);
}

static void loadBIN(const char *path, uint16_t start) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    printf("*** Error: opening '%s'\n", path);
    return;
  }

  auto buf = readAll<std::vector<uint8_t>>(f);

  if (buf.size() > 0x10000 - start) {
    printf("*** Error: bin is too large\n");
  } else {
    printf("loaded: [$%04X..$%04X]\n", start, start + (unsigned)buf.size() - 1);
    printf("len   : %u\n", (unsigned)buf.size());

    memcpy(s_memory + start, buf.data(), buf.size());
  }

  fclose(f);
}

static std::optional<std::string> readLine() {
  std::string res;
  int ch;
  while ((ch = getchar()) != EOF && ch != '\n')
    res.push_back((char)ch);

  if (ch == EOF)
    return std::nullopt;

  return res;
}

static std::optional<uint16_t> parse16(const char *token) {
  int base = 0;
  if (*token == '$') {
    ++token;
    base = 16;
  }
  char *end;
  long res = strtol(token, &end, base);
  if (*end)
    return std::nullopt;
  return res & 0xFFFF;
}

static void printDB() {
  char asciiBuf[17];
  asciiBuf[16] = 0;
  uint16_t addr = s_curAddr & ~15;
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
    if (addr < s_curAddr) {
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
  s_curAddr = addr;
}

static void printDW() {
  uint16_t addr = s_curAddr;
  uint16_t end = addr + 16;
  printf("%04X: ", addr);
  while (addr != end) {
    uint16_t v = peek16(addr);
    printf(" %04X", v);
    addr += 2;
  }
  printf("\n");
  s_curAddr = addr;
}

static void printHelp() {
  printf("help - print this help\n");
  printf("loadrom file - Load file at end of memory\n");
  printf("loadb33 file - Load DOS3.3 binary file to addr encoded in file header\n");
  printf("loadbin file addr - Load arbitrary binary file to specified addr\n");
  printf("saveb33 file addr L len - Store a DOS3.3 binary file\n");
  printf("saveb33 file addr last_addr - Store a DOS3.3 binary file\n");
  printf("s addr - Set current address\n");
  printf("s - Print current address\n");
  printf("dis - Disassemble 20 instructions\n");
  printf("db - print up to 64 bytes/words\n");
  printf("dw - print 8 words\n");
  printf("memcpy dest src len - copy memory\n");
  printf("asm inst operand - assemble an instruction at the current location\n");
  printf("wb v1 v2... - write bytes\n");
  printf("ww v1 v2... - write words\n");
}

int main() {
  printf("Interactive 6502 Disassembler\n\n");
  printf("Use \"help\" for help\n");

  // Keep track of how many sasm errors occurred in the last loop.
  unsigned errors = 0;
  SimpleAsm sasm([&errors](const char *, const char *msg) {
    ++errors;
    printf("Error: %s\n", msg);
  });

  for (;;) {
    // Reset errors.
    errors = 0;
    printf("\n> ");
    auto line = readLine();
    if (!line)
      break;

    // Split.
    std::vector<std::string> tokens;
    char *str = &line.value()[0];
    while (auto *token = strtok(str, " \t\n\r\v\f")) {
      str = nullptr;
      tokens.emplace_back(token);
    }

    if (tokens.empty()) {
      printf("%04X:\n", s_curAddr);
      continue;
    }

    if (tokens[0] == "help") {
      printHelp();
    } else if (tokens[0] == "loadrom" && tokens.size() == 2) {
      loadROM(tokens[1].c_str());
    } else if (tokens[0] == "loadb33" && tokens.size() == 2) {
      loadB33(tokens[1].c_str());
    } else if (tokens[0] == "loadbin" && tokens.size() == 3) {
      auto addr = parse16(tokens[2].c_str());
      if (!addr)
        printf("Error: invalid number.\n");
      else
        loadBIN(tokens[1].c_str(), *addr);
    } else if (tokens[0] == "saveb33" && tokens.size() == 4) {
      // saveb33 path addr end_addr
      auto addr = parse16(tokens[2].c_str());
      auto end_addr = parse16(tokens[3].c_str());
      if (!addr || !end_addr) {
        printf("Error: invalid number.\n");
      } else if (*addr > *end_addr) {
        printf("Error: invalid start address.\n");
      } else if (*end_addr - *addr == 0xFFFF) {
        printf("Error: length exceeds 16-bits.\n");
      } else {
        saveB33(tokens[1].c_str(), *addr, *end_addr - *addr + 1);
      }
    } else if (
        tokens[0] == "saveb33" && tokens.size() == 5 && (tokens[3] == "L" || tokens[3] == "l")) {
      // saveb33 path addr L length
      auto addr = parse16(tokens[2].c_str());
      auto len = parse16(tokens[4].c_str());
      if (!addr || !len) {
        printf("Error: invalid number.\n");
      } else if (*addr > 0x10000 - *len) {
        printf("Error: invalid start address.\n");
      } else {
        saveB33(tokens[1].c_str(), *addr, *len);
      }
    } else if (tokens[0] == "s" && tokens.size() == 1) {
      printf("Current address is $%04X (%u)\n", s_curAddr, s_curAddr);
    } else if (tokens[0] == "s" && tokens.size() == 2) {
      auto num = parse16(tokens[1].c_str());
      if (!num)
        printf("Error: invalid number.\n");
      else
        s_curAddr = *num;
    } else if (tokens[0] == "dis" && tokens.size() == 1) {
      for (unsigned i = 0; i != 20; ++i) {
        s_curAddr = (uint16_t)(s_curAddr + printInst(s_curAddr));
      }
    } else if (tokens[0] == "db" && tokens.size() == 1) {
      printDB();
    } else if (tokens[0] == "dw" && tokens.size() == 1) {
      printDW();
    } else if (tokens[0] == "memcpy" && tokens.size() == 4) {
      auto dest = parse16(tokens[1].c_str());
      auto src = parse16(tokens[2].c_str());
      auto len = parse16(tokens[3].c_str());
      if (!dest || !src || !len) {
        printf("Error: invalid number.\n");
      } else if (*dest > 0x10000 - *len || *src > 0x10000 - *len) {
        printf("Error: invalid copy length.\n");
      } else {
        memmove(s_memory + *dest, s_memory + *src, *len);
        printf("%u bytes copied from $%04X to $%04X\n", *len, *src, *dest);
      }
    } else if (tokens[0] == "asm" && tokens.size() >= 2) {
      // Concatenate all tokens >= 2 into one "right" operand.
      std::string right = tokens.size() > 2 ? tokens[2] : std::string();
      for (size_t i = 3; i < tokens.size(); ++i) {
        right += ' ';
        right += tokens[i];
      }

      ThreeBytes bytes;
      if (auto optLen = sasm.assemble(&bytes, s_curAddr, tokens[1].c_str(), right.c_str(), true))
        if (!errors) {
          unsigned len = *optLen;
          for (unsigned i = 0; i != len; ++i)
            poke(s_curAddr + i, bytes.d[i]);
          printInst(s_curAddr);
          s_curAddr += len;
        }
    } else if (tokens[0] == "wb" && tokens.size() > 1) {
      std::vector<uint8_t> bytes;
      for (size_t i = 1; i < tokens.size(); ++i) {
        if (auto val = parse16(tokens[i].c_str())) {
          if (*val <= 0xFF) {
            bytes.push_back(*val);
            continue;
          }
        }
        printf("Error: invalid byte %zu", i - 1);
        break;
      }
      // Did we parse all tokens?
      if (bytes.size() == tokens.size() - 1) {
        for (size_t i = 0; i != bytes.size(); ++i)
          poke(s_curAddr + i, bytes[i]);
        s_curAddr += bytes.size();
        printf("%04X:\n", s_curAddr);
      }
    } else if (tokens[0] == "ww" && tokens.size() > 1) {
      std::vector<uint16_t> words;
      for (size_t i = 1; i < tokens.size(); ++i) {
        if (auto val = parse16(tokens[i].c_str())) {
          words.push_back(*val);
        } else {
          printf("Error: invalid word %zu", i - 1);
          break;
        }
      }
      // Did we parse all tokens?
      if (words.size() == tokens.size() - 1) {
        for (size_t i = 0; i != words.size(); ++i)
          poke16(s_curAddr + i * 2, words[i]);
        s_curAddr += words.size() * 2;
        printf("%04X:\n", s_curAddr);
      }
    } else {
      printf("Error: invalid command. Use \"help\" for help.\n");
    }
  }

  return 0;
}
