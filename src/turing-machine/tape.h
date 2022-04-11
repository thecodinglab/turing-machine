#pragma once

#include <stddef.h>
#include <stdint.h>

#define SYMBOL_EMPTY 0

typedef uint32_t symbol_t;

typedef struct {
  symbol_t *ptr;
  size_t cap, lim;
} tape_t;

void tape_destroy(tape_t *tape);

symbol_t tape_read_at(tape_t *tape, size_t pos);
void tape_write_at(tape_t *tape, size_t pos, symbol_t symbol);
