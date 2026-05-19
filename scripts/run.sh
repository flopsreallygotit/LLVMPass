#!/usr/bin/env bash

set -e

mkdir -p build/obj build/bin lls dots images

cd build
ninja
cd ..

clang -S -emit-llvm -O0 tests/test.c -o lls/test.ll

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
