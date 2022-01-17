#!/bin/bash

bin=../../cmake-build-debug
apple2tc=$bin/tools/apple2tc/apple2tc

$apple2tc robotron2084.b33 --run-data=robotron.json --simple-c > robotron.c
