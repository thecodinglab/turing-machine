#include "args.h"
#include "parse/parse.h"
#include "turing-machine/format.h"
#include "turing-machine/machine.h"
#include "util/log.h"

#include <string.h>

#ifdef ANSI
#define ANSI_FG_RED "\033[31m"
#define ANSI_FG_GREEN "\033[32m"
#define ANSI_RESET "\033[0m"
#else
#define ANSI_FG_RED ""
#define ANSI_FG_GREEN ""
#define ANSI_RESET ""
#endif // ANSI

#define FMT_BUFFER_SIZE 1024 * 1024
static char buffer[FMT_BUFFER_SIZE];

int main(int argc, char **argv) {
  arguments_t args = {
      .machine = NULL,
      .storage_kind = STORAGE_HASH_TABLE,
      .verbosity = LEVEL_INFO,
  };

  int ret;
  if ((ret = args_parse(&args, argc, argv)) != 0)
    return ret;

  log_set_min_level(args.verbosity);

  turing_machine_t turing_machine = turing_machine_create(args.storage_kind);

  reader_t reader;
  if (args.machine != NULL)
    reader = reader_open_static(args.machine, strlen(args.machine));
  else
    reader = reader_open_file(stdin);

  parse_turing_machine(&reader, &turing_machine);
  reader_destroy(&reader);

  if (log_is_enabled(LEVEL_INFO)) {
    log_info("interpreted transitions: \n");

    list_t transitions = transition_storage_to_list(turing_machine.storage);
    for (size_t i = 0; i < transitions.count; i++) {
      transition_t *transition = list_get(&transitions, i);
      format_transition(buffer, FMT_BUFFER_SIZE, format_default, *transition);
      log_info("  %s\n", buffer);
    }

    log_info("\n");
  }

  size_t steps = 0;
  while (turing_machine_next(&turing_machine)) {
    steps++;
  }

  if (log_is_enabled(LEVEL_INFO)) {
    log_info("\n");

    log_info("state: ");
    if (turing_machine_is_accepting(&turing_machine)) {
      log_info(ANSI_FG_GREEN "ACCEPTING" ANSI_RESET "\n");
    } else {
      log_info(ANSI_FG_RED "REJECTING" ANSI_RESET "\n");
    }

    format_tape(buffer, FMT_BUFFER_SIZE, format_default, &turing_machine.tape,
                0);
    log_info("content: %s\n", buffer);

    format_tape(buffer, FMT_BUFFER_SIZE, format_default, &turing_machine.tape,
                5);
    log_info("\nturing_machine = {tape=%s, ", buffer);
    format_state(buffer, FMT_BUFFER_SIZE, format_default, turing_machine.state);
    log_info("state=%s, ", buffer);

    log_info("head=%ld, ", turing_machine.tape.head);
    log_info("steps=%lu}\n", steps);
  }

  turing_machine_destroy(&turing_machine);

  return EXIT_SUCCESS;
}
