#include "parse.h"
#include "debug.h"

#include <assert.h>
#include <memory.h>
#include <stddef.h>
#include <stdint.h>

#define TOKEN_SEPARATOR ';'
#define TOKEN_PREFIX 'D'
#define TOKEN_STATE 'A'
#define TOKEN_SYMBOL 'C'
#define TOKEN_RIGHT 'R'
#define TOKEN_LEFT 'L'

size_t static_read(void *dest, size_t size, void *handle) { return EOF; }

size_t file_read(void *dest, size_t size, void *handle) {
  return fread(dest, size, 1, (FILE *)handle);
}

reader_t reader_open_static(void *src, size_t size) {
  buffer_t buf = buffer_alloc(size);
  memcpy(buf.ptr, src, size);
  buf.lim = size;

  return (reader_t){
      .buf = buf,
      .handle = NULL,
      .read = static_read,
  };
}

reader_t reader_open_file(FILE *file) {
  return (reader_t){
      .buf = buffer_alloc(1024),
      .handle = file,
      .read = file_read,
  };
}

char reader_next(reader_t *reader) {
  if (!buffer_has_next(&reader->buf)) {
    size_t count =
        reader->read(reader->buf.ptr, reader->buf.cap, reader->handle);

    if (count == EOF)
      return 0;

    reader->buf.pos = 0;
    reader->buf.lim = count;
  }

  if (!buffer_has_next(&reader->buf))
    return 0;

  char c = buffer_next(&reader->buf);
  return c;
}

void reader_back(reader_t *reader) { buffer_back(&reader->buf); }

uint32_t reader_count(reader_t *reader, char symbol) {
  uint32_t count = 0;

  while (reader_next(reader) == symbol) {
    count++;
  }

  reader_back(reader);
  return count;
}

state_t reader_parse_state(reader_t *reader) {
  assert(reader_next(reader) == TOKEN_PREFIX);
  return (state_t)reader_count(reader, TOKEN_STATE);
}

symbol_t reader_parse_symbol(reader_t *reader) {
  assert(reader_next(reader) == TOKEN_PREFIX);
  return (symbol_t)reader_count(reader, TOKEN_SYMBOL);
}

direction_t reader_parse_direction(reader_t *reader) {
  switch (reader_next(reader)) {
  case TOKEN_RIGHT:
    return DIRECTION_RIGHT;
  case TOKEN_LEFT:
    return DIRECTION_LEFT;
  default:
    // TODO error
    return 0;
  }
}

int reader_has_transition(reader_t *reader) {
  return reader_next(reader) == TOKEN_SEPARATOR;
}

transition_t reader_parse_transition(reader_t *reader) {
  state_t state = reader_parse_state(reader);
  symbol_t in = reader_parse_symbol(reader);
  symbol_t out = reader_parse_symbol(reader);
  direction_t dir = reader_parse_direction(reader);
  state_t next = reader_parse_state(reader);

  return (transition_t){
      .state = state,
      .in = in,
      .out = out,
      .dir = dir,
      .next = next,
  };
}

turing_machine_t reader_parse_turing_machine(reader_t *reader) {
  turing_machine_t turing_machine = {
      .head = NULL,
      .state = STARTING_STATE,
  };

  size_t i = 0;
  while (reader_has_transition(reader)) {
    transition_t transition = reader_parse_transition(reader);
    turing_machine.transitions[i++] = transition;
  }
  turing_machine.transition_count = i;

  return turing_machine;
}
