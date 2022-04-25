#!/usr/bin/env sh

function prepare_benchmarks() {
  make clean && make UNICODE=0 ANSI=0 PROFILE=0 DEBUG=0
}

function run_benchmarks() {
  NAME="$1"
  shift

  DIR="docs/benchmarks"

  mkdir -p "$DIR"
  hyperfine -w 5 -N --export-json "$DIR/$NAME.json" --export-markdown "$DIR/$NAME.md" "$@"
}

prepare_benchmarks

run_benchmarks unary \
  -n "unary-multiplication-using-list" \
  "bin/turing-machine -v 3 -s list '$(cat machines/unary_multiplication_39_19.txt)'" \
  -n "unary-multiplication-using-map" \
  "bin/turing-machine -v 3 -s map '$(cat machines/unary_multiplication_39_19.txt)'" \
  -n "unary-multiplication-using-hash-table" \
  "bin/turing-machine -v 3 -s hash_table '$(cat machines/unary_multiplication_39_19.txt)'" \

run_benchmarks binary \
  -n "binary-multiplication-using-list" \
  "bin/turing-machine -v 3 -s list --json '$(cat machines/binary_multiplication_80473151_64955687.json)'" \
  -n "binary-multiplication-using-map" \
  "bin/turing-machine -v 3 -s map --json '$(cat machines/binary_multiplication_80473151_64955687.json)'" \
  -n "binary-multiplication-using-hash-table" \
  "bin/turing-machine -v 3 -s hash_table --json '$(cat machines/binary_multiplication_80473151_64955687.json)'" \
