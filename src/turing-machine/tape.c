#include "tape.h"

tape_t tape_create() {
  return (tape_t){
      .positive = list_create(0, sizeof(symbol_t)),
      .negative = list_create(0, sizeof(symbol_t)),
      .head = 0,
  };
}

void tape_destroy(tape_t *tape) {
  list_destroy(&tape->positive);
  list_destroy(&tape->negative);
}

static list_t *tape_get_list(tape_t *tape, tape_head_t pos) {
  if (pos < 0)
    return &tape->negative;
  else
    return &tape->positive;
}

static size_t tape_get_index(tape_head_t pos) {
  if (pos < 0)
    return -(pos + 1);
  else
    return pos;
}

symbol_t tape_read_at(tape_t *tape, tape_head_t pos) {
  size_t idx = tape_get_index(pos);
  list_t *list = tape_get_list(tape, pos);

  if (idx >= list->count)
    return SYMBOL_EMPTY;

  return *((symbol_t *)list_get(list, idx));
}

void tape_write_at(tape_t *tape, tape_head_t pos, symbol_t symbol) {
  size_t idx = tape_get_index(pos);
  list_t *list = tape_get_list(tape, pos);
  list_add(list, idx, &symbol);
}

symbol_t tape_read(tape_t *tape) { return tape_read_at(tape, tape->head); }

void tape_write(tape_t *tape, symbol_t symbol) {
  tape_write_at(tape, tape->head, symbol);
}

void tape_move(tape_t *tape, direction_t direction) {
  switch (direction) {
  case DIRECTION_RIGHT:
    tape->head++;
    break;

  case DIRECTION_LEFT:
    tape->head--;
    break;
  }
}
