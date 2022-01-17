#!/bin/bash

bin=../../cmake-build-debug
a2emu=$bin/tools/a2emu/a2emu

$a2emu --rom --collect --limit=30000000 --fast --out=run.json --kbd-file=all.bas
