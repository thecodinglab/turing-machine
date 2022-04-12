#include "machine.h"
#include "debug.h"
#include "format.h"

#include <assert.h>
#include <stdio.h>

#define FMT_BUFFER_SIZE 2048

void turing_machine_destroy(turing_machine_t *turing_machine) {
  tape_destroy(&turing_machine->positive);
  tape_destroy(&turing_machine->negative);
}

tape_t *turing_machine_get_tape(turing_machine_t *turing_machine) {
  if (turing_machine->head < 0)
    return &turing_machine->negative;
  else
    return &turing_machine->positive;
}

size_t turing_machine_get_tape_index(turing_machine_t *turing_machine) {
  if (turing_machine->head < 0)
    return -(turing_machine->head + 1);
  else
    return turing_machine->head;
}

symbol_t turing_machine_read(turing_machine_t *turing_machine) {
  tape_t *tape = turing_machine_get_tape(turing_machine);
  return tape_read_at(tape, turing_machine_get_tape_index(turing_machine));
}

void turing_machine_write(turing_machine_t *turing_machine, symbol_t symbol) {
  tape_t *tape = turing_machine_get_tape(turing_machine);
  tape_write_at(tape, turing_machine_get_tape_index(turing_machine), symbol);
}

void turing_machine_process(turing_machine_t *turing_machine,
                            transition_t transition) {
  static char buffer[FMT_BUFFER_SIZE];

  format_tape(buffer, FMT_BUFFER_SIZE, format_debug, turing_machine, 0);

  turing_machine->state = transition.next;
  turing_machine_write(turing_machine, transition.out);

  switch (transition.dir) {
  case DIRECTION_RIGHT:
    turing_machine->head++;
    break;

  case DIRECTION_LEFT:
    turing_machine->head--;
    break;
  }

  LOG("transitioning: %s", buffer);

  format_transition(buffer, FMT_BUFFER_SIZE, format_debug, transition);
  LOG(" -> %s -> ", buffer);

  format_tape(buffer, FMT_BUFFER_SIZE, format_debug, turing_machine, 0);
  LOG("%s\n", buffer);
}

int turing_machine_is_accepting(turing_machine_t *turing_machine) {
  return turing_machine->state == ACCEPTING_STATE;
}

int turing_machine_next(turing_machine_t *turing_machine) {
  symbol_t symbol = turing_machine_read(turing_machine);

  for (size_t i = 0; i < turing_machine->transition_count; i++) {
    transition_t transition = turing_machine->transitions[i];
    if (transition.state == turing_machine->state && transition.in == symbol) {
      turing_machine_process(turing_machine, transition);
      return 1;
    }
  }

  return 0;
}
