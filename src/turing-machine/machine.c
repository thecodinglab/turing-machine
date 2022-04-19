#include "machine.h"
#include "debug.h"
#include "format.h"

#include <assert.h>
#include <stdio.h>

#define FMT_BUFFER_SIZE 2048

turing_machine_t turing_machine_create() {
  return (turing_machine_t){
      .tape = tape_create(),
      .state = STARTING_STATE,
  };
}

void turing_machine_destroy(turing_machine_t *turing_machine) {
  tape_destroy(&turing_machine->tape);
}

void turing_machine_process(turing_machine_t *turing_machine,
                            transition_t transition) {
  static char buffer[FMT_BUFFER_SIZE];

  // place tape before transition into buffer.
  format_tape(buffer, FMT_BUFFER_SIZE, format_debug, &turing_machine->tape, 0);

  // transition to the next state and update the tape.
  turing_machine->state = transition.next;
  tape_write(&turing_machine->tape, transition.out);
  tape_move(&turing_machine->tape, transition.dir);

  // log transition debug messges.
  LOG("transitioning: %s", buffer);

  format_transition(buffer, FMT_BUFFER_SIZE, format_debug, transition);
  LOG(" -> %s -> ", buffer);

  format_tape(buffer, FMT_BUFFER_SIZE, format_debug, &turing_machine->tape, 0);
  LOG("%s\n", buffer);
}

int turing_machine_is_accepting(turing_machine_t *turing_machine) {
  return turing_machine->state == ACCEPTING_STATE;
}

int turing_machine_next(turing_machine_t *turing_machine) {
  symbol_t symbol = tape_read(&turing_machine->tape);

  for (size_t i = 0; i < turing_machine->transition_count; i++) {
    transition_t transition = turing_machine->transitions[i];
    if (transition.state == turing_machine->state && transition.in == symbol) {
      turing_machine_process(turing_machine, transition);
      return 1;
    }
  }

  return 0;
}
