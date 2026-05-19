#!/usr/bin/env bash

set -e

# FIXME: Its better to separate building from running since I dont want to rebuild project each time
mkdir -p build/obj build/bin lls dots images

cd build
ninja
cd ..

clang -S -emit-llvm -O0 tests/test.c -o lls/test.ll

# FIXME: you could use clang to pass your plugin
# FIXME: want to see multi-module tests
# FIXME: want to see tests with recursion, loops
# FIXME: .gitignore, .clang-format may be useful here
opt -load-pass-plugin=./build/lib/libMyPass.so \
    -passes="my-pass" \
    -S lls/test.ll -o lls/test_inst.ll

clang -c src/Runtime.c -o build/obj/Runtime.o

clang lls/test_inst.ll build/obj/Runtime.o -o build/bin/test_inst
./build/bin/test_inst

python3 scripts/Annotate.py

# [flops]: This can be done more accurate via Python
dot -Tpng dots/graph.dot -o images/graph.png
dot -Tpng dots/graph_annotated.dot -o images/graph_annotated.png

echo "Done! See images/graph.png (clean) and images/graph_annotated.png (with runtime values)"
