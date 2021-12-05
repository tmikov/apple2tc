#include "apple2tc/d6502.h"
#include "apple2tc/a2symbols.h"

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
static uint16_t peek16(unsigned addr) {
  return peek(addr) + peek(addr + 1) * 256;
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

static std::vector<char> readAll(FILE *f) {
  std::vector<char> buf;
  static constexpr unsigned CHUNK = 8192;
  for (;;) {
    size_t size = buf.size();
    buf.resize(size + CHUNK);
    size_t nr = fread(&buf[size], 1, CHUNK, f);
    buf.resize(size + nr);
    if (nr != CHUNK)
      break;
  }
  return buf;
}

static void loadROM(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    printf("*** Error: opening '%s'\n", path);
    return;
  }

  auto buf = readAll(f);

  if (buf.size() > 0x10000) {
    printf("*** Error: ROM is roo large\n");
  } else {
    unsigned start = 0x10000 - buf.size();
    printf("$%zX bytes read\n", buf.size());
    printf("ROM start at $%04X\n", start);

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

static void printHelp() {
  printf("help - print this help\n");
  printf("loadrom file - Load file at end of memory\n");
  printf("s addr - Set current address\n");
  printf("s - Print current address\n");
  printf("dis - Disassemble 20 instructions\n");
  printf("db/dw - print up to 64 hex bytes/words\n");
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
      printf("____");
    } else {
      uint16_t v = peek16(addr);
      printf("%04X", v);
    }

    if (ofs == 14)
      printf("\n");

    addr += 2;
  }
  s_curAddr = addr;
}

int main() {
  printf("Interactive 6502 Disassembler\n\n");
  printf("Use \"help\" for help\n");
  for (;;) {
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

    if (tokens.empty())
      continue;

    if (tokens[0] == "help") {
      printHelp();
    } else if (tokens[0] == "loadrom" && tokens.size() == 2) {
      loadROM(tokens[1].c_str());
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
    } else {
      printf("Error: invalid command. Use \"help\" for help.\n");
    }
  }

  return 0;
}
