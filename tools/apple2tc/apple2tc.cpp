/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Disas.h"
#include "PubIR.h"

#include "apple2tc/support.h"

#include <cstring>
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
  fprintf(stderr, "  -v<number>          Verbosity level\n");
  fprintf(stderr, "  --rom               Input file is a ROM\n");
  fprintf(stderr, "  --asm               Generate asm listing (default)\n");
  fprintf(stderr, "  --simple-c          Generate simple C code\n");
  fprintf(stderr, "  --ir                Generate IR\n");
  fprintf(stderr, "  --irc1              Generate C1 representation of the IR\n");
  fprintf(stderr, "  --no-ir-trees       Do not reconstruct trees in IR dump\n");
  fprintf(stderr, "  --ret-addr          Preserve subroutines return address on the stack\n");
  fprintf(stderr, "  --alt-exit          Identify routines that exit by discarding their return\n");
  fprintf(stderr, "                      address and jumping into the caller\n");
  fprintf(stderr, "  -O<number>          Optimization level (default 0)\n");
  fprintf(stderr, "  --run-data=d.json   Load runtime data from specified file\n");
  fprintf(stderr, "  --routines-report=f Write routine candidate analysis to file\n");
  fprintf(stderr, "  --extern-routines=f Declare the routines listed in the file external\n");
  fprintf(stderr, "  --code-at=f         Add hand-asserted 'origin target' branch edges\n");
  fprintf(stderr, "  --inline-str=f      Routines taking a NUL-terminated string after the JSR\n");
  fprintf(stderr, "  --coverage=f        Write a code/data coverage report to file\n");
  fprintf(stderr, "  --known-data=f      Declare 'from to name' data ranges for --coverage\n");
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
  bool preserveRetAddr = false;
  bool altExits = false;
  Action action = Action::GenAsm;
  unsigned verbosity = 0;
  unsigned optLevel = 0;
  bool irTrees = true;
  s_appPath = argc ? argv[0] : "apple2tc";

  std::string inputPath;
  std::string runDataPath;
  std::string routinesReportPath;
  std::string externRoutinesPath;
  std::string codeAtPath;
  std::string inlineStrPath;
  std::string coveragePath;
  std::string knownDataPath;
  bool rom = false;
  for (int i = 1; i < argc; ++i) {
    if (strncmp(argv[i], "-v", 2) == 0 && strlen(argv[i]) == 3 && isdigit(argv[i][2])) {
      verbosity = argv[i][2] - '0';
      continue;
    }
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
    if (strcmp(argv[i], "--ret-addr") == 0) {
      preserveRetAddr = true;
      continue;
    }
    if (strcmp(argv[i], "--alt-exit") == 0) {
      altExits = true;
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
    if (strncmp(argv[i], "--routines-report=", 18) == 0) {
      routinesReportPath = argv[i] + 18;
      continue;
    }
    if (strncmp(argv[i], "--extern-routines=", 18) == 0) {
      externRoutinesPath = argv[i] + 18;
      continue;
    }
    if (strncmp(argv[i], "--inline-str=", 13) == 0) {
      inlineStrPath = argv[i] + 13;
      continue;
    }
    if (strncmp(argv[i], "--code-at=", 10) == 0) {
      codeAtPath = argv[i] + 10;
      continue;
    }
    if (strncmp(argv[i], "--coverage=", 11) == 0) {
      coveragePath = argv[i] + 11;
      continue;
    }
    if (strncmp(argv[i], "--known-data=", 13) == 0) {
      knownDataPath = argv[i] + 13;
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
    if (!codeAtPath.empty())
      dis->setCodeAt(loadCodeAt(codeAtPath));
    if (!inlineStrPath.empty()) {
      // Refused rather than warned about, because the failure is invisible.
      // A declared routine whose body is still generated pops its return
      // address off the emulated stack -- and that address has just been moved
      // past the string, so it prints from whatever follows and every check
      // downstream compares two engines doing the same wrong thing.
      if (externRoutinesPath.empty()) {
        fprintf(
            stderr,
            "--inline-str requires --extern-routines: a declared routine must be replaced,\n"
            "not generated, or it will read its argument from the wrong place.\n");
        return 2;
      }
      dis->setInlineStr(loadInlineStr(inlineStrPath));
    }
    if (rom) {
      dis->loadROM(binary.data(), binary.size());
      dis->setStart(dis->peek16(0xFFFC));
    } else {
      dis->loadBinary(start, binary.data(), binary.size());
      dis->setStart(start);
    }
    dis->run(noGenerations);
    if (!coveragePath.empty()) {
      FILE *cf = fopen(coveragePath.c_str(), "wt");
      if (!cf) {
        perror(coveragePath.c_str());
        return 2;
      }
      dis->printCoverage(cf, knownDataPath.empty() ? std::vector<KnownDataRange>{}
                                                    : loadKnownData(knownDataPath));
      fclose(cf);
    }
    switch (action) {
    case Action::GenAsm:
      dis->printAsmListing();
      break;
    case Action::GenSimpleC:
      dis->printSimpleC(stdout);
      break;
    case Action::GenIR:
    case Action::GenIRC1: {
      auto irCtx = newIRContext(verbosity, preserveRetAddr, altExits);
      auto *mod = genIR(dis, *irCtx);
      if (optLevel > 0)
        localCPURegSSA(mod);
      if (optLevel > 1)
        dce(mod);
      if (optLevel > 2) {
        if (simplify(mod))
          dce(mod);
      }
      // Externalizing must happen before routine identification: a routine that
      // calls an unidentifiable ROM entry point is itself rejected.
      if (!externRoutinesPath.empty()) {
        auto externs = loadExternRoutines(mod, externRoutinesPath);
        // Every --inline-str routine must be among them; see the check at
        // parse time for why this is fatal rather than advisory.
        for (uint16_t addr : dis->getInlineStr()) {
          bool found = false;
          for (auto &[externAddr, name] : externs)
            found = found || externAddr == addr;
          if (!found) {
            fprintf(
                stderr,
                "--inline-str: $%04X is not in %s. A declared routine must be replaced by a\n"
                "hand-written one, which receives the string's address as its ret_addr.\n",
                addr,
                externRoutinesPath.c_str());
            return 2;
          }
        }
        externRoutines(mod, externs);
      }
      if (optLevel > 1) {
        FILE *report = nullptr;
        if (!routinesReportPath.empty()) {
          if ((report = fopen(routinesReportPath.c_str(), "wt")) == nullptr) {
            perror(routinesReportPath.c_str());
            return 2;
          }
        }
        while (identifySimpleRoutines(mod, report)) {
        }
        if (report)
          fclose(report);
      }
      if (optLevel > 2) {
        simplifyCFG(mod);
        simplify(mod);
        localCPURegSSA(mod);
        dce(mod);
      }
      if (action == Action::GenIR) {
        auto regLiveness = liveness(mod);
        dumpModule(mod, irTrees, regLiveness.get());
      } else {
        printIRC1(mod, stdout, irTrees);
      }
      break;
    }
    }
  } catch (std::exception &ex) {
    fprintf(stderr, "*** FATAL: %s\n", ex.what());
    return 2;
  }

  return 0;
}
