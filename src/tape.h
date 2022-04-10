#pragma once

#include <stddef.h>
#include <stdint.h>

#define SYMBOL_EMPTY 0

typedef uint32_t symbol_t;

typedef struct tape tape_t;

struct tape {
  symbol_t symbol;

  tape_t *right;
  tape_t *left;
};

tape_t *tape_create(symbol_t symbol, tape_t *left, tape_t *right);
void tape_destroy(tape_t *tape);

tape_t *tape_from_input(symbol_t *input, size_t size);

void tape_print(tape_t *tape);
