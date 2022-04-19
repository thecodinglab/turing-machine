#include "machine.h"
#include "debug.h"
#include "format.h"

#include <assert.h>
#include <stdio.h>

#define FMT_BUFFER_SIZE 1024 * 1024

typedef struct {
  state_t state;
  symbol_t symbol;
} transition_key_t;

static int transition_key_cmp(map_key_t a_ptr, map_key_t b_ptr) {
  transition_key_t *a = (transition_key_t *)a_ptr;
  transition_key_t *b = (transition_key_t *)b_ptr;

  int cmp = b->state - a->state;
  if (cmp == 0)
    cmp = b->symbol - a->symbol;

  return cmp;
}

turing_machine_t turing_machine_create() {
  return (turing_machine_t){
      .tape = tape_create(),
      .state = STARTING_STATE,
      .transitions = map_create(sizeof(transition_key_t), sizeof(transition_t),
                                transition_key_cmp),
  };
}

void turing_machine_destroy(turing_machine_t *turing_machine) {
  tape_destroy(&turing_machine->tape);
  map_destroy(&turing_machine->transitions);
}

void turing_machine_process(turing_machine_t *turing_machine,
                            transition_t transition) {
#ifdef DEBUG
  static char buffer[FMT_BUFFER_SIZE];

  // place tape before transition into buffer.
  format_tape(buffer, FMT_BUFFER_SIZE, format_debug, &turing_machine->tape, 0);
#endif // DEBUG

  // transition to the next state and update the tape.
  turing_machine->state = transition.next;
  tape_write(&turing_machine->tape, transition.out);
  tape_move(&turing_machine->tape, transition.dir);

#ifdef DEBUG
  // log transition debug messges.
  LOG("transitioning: %s", buffer);

  format_transition(buffer, FMT_BUFFER_SIZE, format_debug, transition);
  LOG(" -> %s -> ", buffer);

  format_tape(buffer, FMT_BUFFER_SIZE, format_debug, &turing_machine->tape, 0);
  LOG("%s\n", buffer);
#endif // DEBUG
}

void turing_machine_add_transition(turing_machine_t *turing_machine,
                                   transition_t transition) {
  transition_key_t key = {
      .state = transition.state,
      .symbol = transition.in,
  };

  map_insert(&turing_machine->transitions, &key, &transition);
}

int turing_machine_is_accepting(turing_machine_t *turing_machine) {
  return turing_machine->state == ACCEPTING_STATE;
}

int turing_machine_next(turing_machine_t *turing_machine) {
  symbol_t symbol = tape_read(&turing_machine->tape);

  transition_key_t key = {
      .state = turing_machine->state,
      .symbol = symbol,
  };

  transition_t *transition = map_find(&turing_machine->transitions, &key);
  if (transition == NULL)
    return 0;

  turing_machine_process(turing_machine, *transition);
  return 1;
}
