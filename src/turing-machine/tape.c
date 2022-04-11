#include "tape.h"

#include <stdlib.h>

#define BLOCK_SIZE 1024

#define MAX(x, y) ((x) > (y) ? (x) : (y))

void tape_destroy(tape_t *tape) { free(tape->ptr); }

size_t tape_determine_size(size_t required, size_t block_size) {
  size_t remainder = required % block_size;
  if (remainder == 0)
    return required;

  return required - remainder + block_size;
}

void tape_resize(tape_t *tape, size_t cap) {
  tape->ptr = realloc(tape->ptr, cap);
  tape->cap = cap;
}

void tape_write_at(tape_t *tape, size_t pos, symbol_t symbol) {
  if (pos >= tape->cap) {
    size_t size = tape_determine_size(pos + 1, BLOCK_SIZE);
    tape_resize(tape, size);
  }

  tape->lim = MAX(pos + 1, tape->lim);
  tape->ptr[pos] = symbol;
}

symbol_t tape_read_at(tape_t *tape, size_t pos) {
  tape->lim = MAX(pos + 1, tape->lim);

  if (pos >= tape->cap)
    return SYMBOL_EMPTY;

  return tape->ptr[pos];
}
