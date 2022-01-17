#!/bin/bash

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

$apple2tc bolo-patched.b33 --run-data=bolo.json --simple-c > bolo.c
