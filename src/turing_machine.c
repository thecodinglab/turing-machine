#include "turing_machine.h"
#include "debug.h"

#include <assert.h>

void turing_machine_process(turing_machine_t *turing_machine,
                            transition_t transition) {
#ifdef UNICODE
  LOG("transitioning: \xce\xb4(q\xe2\x82%c, S\xe2\x82%c) = (S\xe2\x82%c, %d, "
      "q\xe2\x82%c)\n",
      (char)(0x80 + transition.state), (char)(0x80 + transition.in),
      (char)(0x80 + transition.out), transition.dir,
      (char)(0x80 + transition.next));
#else
  LOG("transitioning: d(q%u, S%u) = (S%u, %d, q%u)\n", transition.state,
      transition.in, transition.out, transition.dir, transition.next);
#endif // UNICODE

  turing_machine->state = transition.next;

  if (turing_machine->head == NULL)
    turing_machine->head = tape_create(transition.out, NULL, NULL);
  else
    turing_machine->head->symbol = transition.out;

  tape_t *next, *left, *right;

  switch (transition.dir) {
  case DIRECTION_LEFT: {
    next = turing_machine->head->left;
    left = NULL;
    right = turing_machine->head;
  } break;

  case DIRECTION_RIGHT: {
    next = turing_machine->head->right;
    left = turing_machine->head;
    right = NULL;
  } break;
  }

  if (next == NULL)
    next = tape_create(SYMBOL_EMPTY, left, right);

  turing_machine->head = next;
}

int turing_machine_is_accepting(turing_machine_t *turing_machine) {
  return turing_machine->state == ACCEPTING_STATE;
}

int turing_machine_next(turing_machine_t *turing_machine) {
  symbol_t symbol = SYMBOL_EMPTY;
  if (turing_machine->head != NULL)
    symbol = turing_machine->head->symbol;

  for (size_t i = 0; i < turing_machine->transition_count; i++) {
    transition_t transition = turing_machine->transitions[i];
    if (transition.state == turing_machine->state && transition.in == symbol) {
      turing_machine_process(turing_machine, transition);
      return 1;
    }
  }

  return 0;
}
