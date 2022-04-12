#pragma once

#include <stdint.h>

#include "tape.h"

#define ACCEPTING_STATE 0
#define STARTING_STATE 1

#define MAX_TRANSITION_COUNT 100

typedef uint32_t state_t;

typedef enum {
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
} direction_t;

typedef struct {
  state_t state;
  symbol_t in;
  symbol_t out;
  direction_t dir;
  state_t next;
} transition_t;

typedef struct {
  tape_t positive;
  tape_t negative;
  int32_t head;

  state_t state;

  transition_t transitions[MAX_TRANSITION_COUNT];
  size_t transition_count;
} turing_machine_t;

void turing_machine_destroy(turing_machine_t *turing_machine);

symbol_t turing_machine_read(turing_machine_t *turing_machine);
void turing_machine_write(turing_machine_t *turing_machine, symbol_t symbol);

int turing_machine_is_accepting(turing_machine_t *turing_machine);

int turing_machine_next(turing_machine_t *turing_machine);
