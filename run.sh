#!/usr/bin/env bash

set -e

cd build
ninja

cd ..
opt -load-pass-plugin=./build/libMyPass.so \
    -passes="my-pass" \
    -disable-output test0.ll

dot -Tpng graph.dot -o graph.png
