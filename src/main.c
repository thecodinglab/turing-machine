#include "parse/parse.h"
#include "turing-machine/format.h"
#include "turing-machine/machine.h"

#include <stdio.h>
#include <string.h>

#define FMT_BUFFER_SIZE 2048

#ifdef ANSI
#define ANSI_FG_RED "\033[31m"
#define ANSI_FG_GREEN "\033[32m"
#define ANSI_RESET "\033[0m"
#else
#define ANSI_FG_RED ""
#define ANSI_FG_GREEN ""
#define ANSI_RESET ""
#endif // ANSI

int main(int argc, char **argv) {
  static char buffer[FMT_BUFFER_SIZE];

  reader_t reader;
  if (argc > 1)
    reader = reader_open_static(argv[1], strlen(argv[1]));
  else
    reader = reader_open_file(stdin);

  turing_machine_t turing_machine = parse_turing_machine(&reader);
  reader_destroy(&reader);

  printf("interpreted transitions: \n");
  for (size_t i = 0; i < turing_machine.transition_count; i++) {
    transition_t transition = turing_machine.transitions[i];

    format_transition(buffer, FMT_BUFFER_SIZE, format_default, transition);
    printf("  %s\n", buffer);
  }

  printf("\n");

  while (turing_machine_next(&turing_machine)) {
    // sleep(1);
  }

  printf("\n");

  printf("state: ");
  if (turing_machine_is_accepting(&turing_machine))
    printf(ANSI_FG_GREEN "ACCEPTING" ANSI_RESET "\n");
  else
    printf(ANSI_FG_RED "REJECTING" ANSI_RESET "\n");

  printf("head: %d\n", turing_machine.head);

  format_tape(buffer, FMT_BUFFER_SIZE, format_default, &turing_machine);
  printf("tape: %s\n", buffer);

  turing_machine_destroy(&turing_machine);

  return EXIT_SUCCESS;
}
