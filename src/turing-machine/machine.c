#include "machine.h"
#include "format.h"
#include "util/log.h"

#include <assert.h>
#include <stdio.h>

#define FMT_BUFFER_SIZE 1024 * 1024

turing_machine_t turing_machine_create(transition_storage_kind_t kind) {
  return (turing_machine_t){
      .tape = tape_create(),
      .state = 0,
      .accepting_states = list_create(0, sizeof(state_t)),
      .storage = transition_storage_create(kind),
  };
}

void turing_machine_destroy(turing_machine_t *turing_machine) {
  tape_destroy(&turing_machine->tape);
  transition_storage_destroy(turing_machine->storage);
}

void turing_machine_process(turing_machine_t *turing_machine,
                            transition_t transition) {
  static char buffer[FMT_BUFFER_SIZE];

  if (log_is_enabled(LEVEL_DEBUG)) {
    // place tape before transition into buffer.
    format_tape(buffer, FMT_BUFFER_SIZE, format_debug, &turing_machine->tape,
                0);
  }

  // transition to the next state and update the tape.
  turing_machine->state = transition.next;
  tape_write(&turing_machine->tape, transition.out);
  tape_move(&turing_machine->tape, transition.dir);

  if (log_is_enabled(LEVEL_DEBUG)) {
    // log transition debug messges.
    log_debug("transitioning: %s", buffer);

    format_transition(buffer, FMT_BUFFER_SIZE, format_debug, transition);
    log_debug(" -> %s -> ", buffer);

    format_tape(buffer, FMT_BUFFER_SIZE, format_debug, &turing_machine->tape,
                0);
    log_debug("%s\n", buffer);
  }
}

void turing_machine_add_transition(turing_machine_t *turing_machine,
                                   transition_t transition) {
  transition_storage_add(turing_machine->storage, transition);
}

void turing_machine_add_accepting_state(turing_machine_t *turing_machine,
                                        state_t state) {
  list_append(&turing_machine->accepting_states,
              turing_machine->accepting_states.count, &state);
}

int turing_machine_is_accepting(turing_machine_t *turing_machine,
                                state_t state) {
  for (size_t i = 0; i < turing_machine->accepting_states.count; i++) {
    state_t *s = list_get(&turing_machine->accepting_states, i);
    if ((*s) == state) {
      return 1;
    }
  }

  return 0;
}

int turing_machine_next(turing_machine_t *turing_machine) {
  symbol_t symbol = tape_read(&turing_machine->tape);

  transition_t *transition = transition_storage_find(
      turing_machine->storage, turing_machine->state, symbol);
  if (transition == NULL)
    return 0;

  turing_machine_process(turing_machine, *transition);
  return 1;
}
