#!/usr/bin/env sh

function prepare_benchmarks() {
  make clean && make UNICODE=0 ANSI=0 PROFILE=0 DEBUG=0
}

function run_benchmark() {
  STORAGE=$1
  DIR="docs/optimizations/$STORAGE"

  mkdir -p "$DIR"
  hyperfine -w 5 -N \
    --export-json "$DIR/hyperfine.json" \
    --export-markdown "$DIR/hyperfine.md" \
    -n "turing-machine-using-$STORAGE" \
    "bin/turing-machine -v 3 -s $STORAGE '$(cat machines/multiplication_large_values.txt)'"
}

prepare_benchmarks
run_benchmark list
run_benchmark map 
run_benchmark hash_table
