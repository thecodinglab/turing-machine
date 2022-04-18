#pragma once

#include "util/list.h"

#include <stdint.h>

#define SYMBOL_EMPTY 0

typedef uint32_t symbol_t;
typedef int32_t tape_head_t;

typedef enum {
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
} direction_t;

typedef struct {
  list_t positive;
  list_t negative;
  // TODO instead of negative numbers, use one `size_t` for the index and
  // another variable for the current list pointer.
  tape_head_t head;
} tape_t;

// Constructs a new empty tape.
tape_t tape_create();
// Destroys the given tape.
void tape_destroy(tape_t *tape);

// Reads the symbol from the provided position on the tape.
symbol_t tape_read_at(tape_t *tape, tape_head_t pos);
// Writes the given symbol to the provided position on the tape.
void tape_write_at(tape_t *tape, tape_head_t pos, symbol_t symbol);

// Reads the symbol at the head of the tape.
symbol_t tape_read(tape_t *tape);
// Writes the given symbol to the head of the tape.
void tape_write(tape_t *tape, symbol_t symbol);
// Moves the head of the tape one step to the given direction.
void tape_move(tape_t *tape, direction_t direction);
