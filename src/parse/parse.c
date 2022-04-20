#include "parse.h"

#include <assert.h>

#define TOKEN_SEPARATOR ';'
#define TOKEN_PREFIX 'D'
#define TOKEN_STATE 'A'
#define TOKEN_SYMBOL 'C'
#define TOKEN_RIGHT 'R'
#define TOKEN_LEFT 'L'

uint32_t parse_symbol_count(reader_t *reader, char symbol) {
  uint32_t count = 0;

  while (reader_current(reader) == symbol) {
    count++;
    reader_next(reader);
  }

  return count;
}

state_t parse_state(reader_t *reader) {
  assert(reader_current(reader) == TOKEN_PREFIX);
  reader_next(reader);

  return (state_t)parse_symbol_count(reader, TOKEN_STATE);
}

symbol_t parse_symbol(reader_t *reader) {
  assert(reader_current(reader) == TOKEN_PREFIX);
  reader_next(reader);

  return (symbol_t)parse_symbol_count(reader, TOKEN_SYMBOL);
}

direction_t parse_direction(reader_t *reader) {
  switch (reader_current(reader)) {
  case TOKEN_RIGHT:
    reader_next(reader);
    return DIRECTION_RIGHT;
  case TOKEN_LEFT:
    reader_next(reader);
    return DIRECTION_LEFT;
  default:
    // TODO error
    return 0;
  }
}

int has_transition(reader_t *reader) {
  return reader_current(reader) == TOKEN_SEPARATOR;
}

transition_t parse_transition(reader_t *reader) {
  state_t state = parse_state(reader);
  symbol_t in = parse_symbol(reader);
  symbol_t out = parse_symbol(reader);
  direction_t dir = parse_direction(reader);
  state_t next = parse_state(reader);

  return (transition_t){
      .state = state,
      .in = in,
      .out = out,
      .dir = dir,
      .next = next,
  };
}

tape_t parse_tape(reader_t *reader) {
  tape_t tape = tape_create();

  size_t pos = 0;
  while (reader_current(reader) != TOKEN_SEPARATOR) {
    symbol_t symbol = parse_symbol(reader);
    tape_write_at(&tape, pos++, symbol);
  }

  return tape;
}

void parse_turing_machine(reader_t *reader, turing_machine_t *dest) {
  dest->tape = parse_tape(reader);

  while (has_transition(reader)) {
    reader_next(reader);

    transition_t transition = parse_transition(reader);
    turing_machine_add_transition(dest, transition);
  }
}
