#include "parse/parse.h"
#include "turing-machine/format.h"
#include "turing-machine/machine.h"

#include <stdio.h>

#define FMT_BUFFER_SIZE 2048

int main() {
  static char buffer[FMT_BUFFER_SIZE];

  reader_t reader = reader_open_file(stdin);
  turing_machine_t turing_machine = parse_turing_machine(&reader);
  reader_destroy(&reader);

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
