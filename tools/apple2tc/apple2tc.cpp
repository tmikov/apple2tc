/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Disas.h"
#include "PubIR.h"

#include "apple2tc/support.h"

#include <stdexcept>
#include <tuple>

static std::tuple<std::vector<uint8_t>, uint16_t> loadInputBinary(const char *inputPath, bool rom) {
  std::vector<uint8_t> binary;
  uint16_t start;
  uint16_t size;

  if (FILE *f = fopen(inputPath, "rb")) {
    binary = readAll<std::vector<uint8_t>>(f);
    fclose(f);
  } else {
    perror(inputPath);
    exit(2);
  }

  if (rom) {
    if (binary.size() < 6 || binary.size() > 0xFE00) {
      fprintf(stderr, "%s: invalid rom file\n", inputPath);
      exit(3);
    }
    start = 0x10000 - binary.size();
  } else {
    if (binary.size() < 4) {
      fprintf(stderr, "%s: missing DOS3.3 file header\n", inputPath);
      exit(3);
    }
    start = binary[0] + binary[1] * 256;
    size = binary[2] + binary[3] * 256;
    if (size > 0x10000 - start || size != binary.size() - 4) {
      fprintf(stderr, "%s: invalid DOS3.3 file header\n", inputPath);
      exit(3);
    }

    binary.erase(binary.begin(), binary.begin() + 4);
  }

  return {std::move(binary), start};
}

static const char *s_appPath;
static void printHelp() {
  fprintf(stderr, "syntax: %s [options] input_file\n", s_appPath);
  fprintf(stderr, "  --rom               Input file is a ROM\n");
  fprintf(stderr, "  --asm               Generate asm listing (default)\n");
  fprintf(stderr, "  --simple-c          Generate simple C code\n");
  fprintf(stderr, "  --ir                Generate IR\n");
  fprintf(stderr, "  --irc1              Generate C1 representation of the IR\n");
  fprintf(stderr, "  --no-ir-trees       Do not reconstruct trees in IR dump\n");
  fprintf(stderr, "  -O<number>          Optimization level (default 0)\n");
  fprintf(stderr, "  --run-data=d.json   Load runtime data from specified file\n");
  fprintf(stderr, "  --no-gen            Ignore runtime generations\n");
}

int main(int argc, char **argv) {
  enum class Action {
    GenAsm,
    GenSimpleC,
    GenIR,
    GenIRC1,
  };
  bool noGenerations = false;
  Action action = Action::GenAsm;
  unsigned optLevel = 0;
  bool irTrees = true;
  s_appPath = argc ? argv[0] : "apple2tc";

  std::string inputPath;
  std::string runDataPath;
  bool rom = false;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--rom") == 0) {
      rom = true;
      continue;
    }
    if (strcmp(argv[i], "--asm") == 0) {
      action = Action::GenAsm;
      continue;
    }
    if (strcmp(argv[i], "--simple-c") == 0) {
      action = Action::GenSimpleC;
      continue;
    }
    if (strcmp(argv[i], "--ir") == 0) {
      action = Action::GenIR;
      continue;
    }
    if (strcmp(argv[i], "--irc1") == 0) {
      action = Action::GenIRC1;
      continue;
    }
    if (strcmp(argv[i], "--no-ir-trees") == 0) {
      irTrees = false;
      continue;
    }
    if (strncmp(argv[i], "-O", 2) == 0 && strlen(argv[i]) == 3 && isdigit(argv[i][2])) {
      optLevel = argv[i][2] - '0';
      continue;
    }
    if (strncmp(argv[i], "--run-data=", 11) == 0) {
      runDataPath = argv[i] + 11;
      continue;
    }
    if (strcmp(argv[i], "--no-gen") == 0) {
      noGenerations = true;
      continue;
    }
    if (argv[i][0] == '-') {
      printHelp();
      return 1;
    }

    if (inputPath.empty()) {
      inputPath = argv[i];
      continue;
    }

    fprintf(stderr, "Too many arguments\n");
    printHelp();
    return 1;
  }

  if (inputPath.empty()) {
    fprintf(stderr, "Not enough arguments\n");
    printHelp();
    return 1;
  }

  // Load the input binary.
  auto [binary, start] = loadInputBinary(inputPath.c_str(), rom);

  try {
    auto dis = std::make_shared<Disas>(runDataPath);
    if (rom) {
      dis->loadROM(binary.data(), binary.size());
      dis->setStart(dis->peek16(0xFFFC));
    } else {
      dis->loadBinary(start, binary.data(), binary.size());
      dis->setStart(start);
    }
    dis->run(noGenerations);
    switch (action) {
    case Action::GenAsm:
      dis->printAsmListing();
      break;
    case Action::GenSimpleC:
      dis->printSimpleC(stdout);
      break;
    case Action::GenIR:
    case Action::GenIRC1: {
      auto irCtx = newIRContext();
      auto *mod = genIR(dis, *irCtx);
      if (optLevel > 0)
        localCPURegSSA(mod);
      if (optLevel > 1)
        dce(mod);
      if (optLevel > 2) {
        if (simplify(mod))
          dce(mod);
      }
      if (action == Action::GenIR)
        dumpModule(mod, irTrees);
      else
        printIRC1(mod, stdout, irTrees);
      break;
    }
    }
  } catch (std::exception &ex) {
    fprintf(stderr, "*** FATAL: %s\n", ex.what());
    return 2;
  }

  return 0;
}
