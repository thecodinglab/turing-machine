#!/usr/bin/env sh

function prepare_profile() {
  make clean && make UNICODE=0 ANSI=0 PROFILE=1 DEBUG=0
}

function run_profile() {
  STORAGE=$1
  DIR="docs/optimizations/$STORAGE"

  if [ -f gmon.out ]; then
    rm -f gmon.out
  fi

  bin/turing-machine -s $STORAGE "$(cat machines/multiplication_large_values.txt)"

  gprof bin/turing-machine gmon.out > "$DIR/profile.txt"
}

prepare_profile
run_profile list
run_profile map 
run_profile hash_table
