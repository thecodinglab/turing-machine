#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// q0 -> accepting state
// q1 -> starting state
// qn -> intermediate states

int main() {
  char *in = ";DADDRDAA;DADCDRDAA;DAADDCRDA;DAADCDCRDA";

  reader_t reader = reader_open_static(in, strlen(in));
  turing_machine_t turing_machine = reader_parse_turing_machine(&reader);

  printf("interpreted transitions: \n");
  for (size_t i = 0; i < turing_machine.transition_count; i++) {
    transition_t transition = turing_machine.transitions[i];

#ifdef UNICODE
    printf("  \xce\xb4(q\xe2\x82%c, S\xe2\x82%c) = (S\xe2\x82%c, %d, "
           "q\xe2\x82%c)\n",
           (char)(0x80 + transition.state), (char)(0x80 + transition.in),
           (char)(0x80 + transition.out), transition.dir,
           (char)(0x80 + transition.next));
#else
    printf("  d(q%u, S%u) = (S%u, %d, q%u)\n", transition.state, transition.in,
           transition.out, transition.dir, transition.next);
#endif // UNICODE
  }

  printf("\n");

  while (!turing_machine_is_accepting(&turing_machine)) {
    turing_machine_next(&turing_machine);

    printf("tape: ");
    tape_print(turing_machine.head);

    sleep(1);
  }

  return EXIT_SUCCESS;
}
