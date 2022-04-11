#include "debug.h"
#include "parse/parse.h"
#include "turing-machine/format.h"
#include "turing-machine/machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// qn -> DAA...A
// Sn -> DCC...C

// q0 -> accepting state
// q1 -> starting state
// qn -> intermediate states

#define FMT_BUFFER_SIZE 2048

int main() {
  static char buffer[FMT_BUFFER_SIZE];

  // tape;transition;transition;...;transition
  char *in =
      "DDDDDDDDDDDDDDDDDDDDDDDDDCC;DADDRDAA;DADCDRDAA;DAADDCRDA;DAADCDCRDA";

  reader_t reader = reader_open_static(in, strlen(in));
  turing_machine_t turing_machine = parse_turing_machine(&reader);

  printf("interpreted transitions: \n");
  for (size_t i = 0; i < turing_machine.transition_count; i++) {
    transition_t transition = turing_machine.transitions[i];

    format_transition(buffer, FMT_BUFFER_SIZE, transition);
    printf("  %s\n", buffer);
  }

  printf("\n");

  while (turing_machine_next(&turing_machine)) {
    // sleep(1);
  }

  format_tape(buffer, FMT_BUFFER_SIZE, &turing_machine);
  printf("tape: %s\n", buffer);

  return EXIT_SUCCESS;
}
