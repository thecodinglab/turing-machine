#include "parse/parse.h"
#include "turing-machine/format.h"
#include "turing-machine/machine.h"

#include "util/map.h"

#include <argp.h>
#include <stdio.h>
#include <string.h>

#define FMT_BUFFER_SIZE 1024 * 1024

#ifdef ANSI
#define ANSI_FG_RED "\033[31m"
#define ANSI_FG_GREEN "\033[32m"
#define ANSI_RESET "\033[0m"
#else
#define ANSI_FG_RED ""
#define ANSI_FG_GREEN ""
#define ANSI_RESET ""
#endif // ANSI

static struct argp_option options[] = {
    {"storage", 's', "kind", 0,
     "The kind of storage to use for the transitions (default: hash_table)."},
    {"verbosity", 'v', "level", 0,
     "The verbosity level of the console output."},
};

typedef struct {
  const char *machine;
  transition_storage_kind_t storage_kind;

  int verbosity; // TODO
} arguments_t;

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  arguments_t *arguments = state->input;

  switch (key) {
  case 's': {
    if (strcmp(arg, "list") == 0) {
      arguments->storage_kind = STORAGE_LIST;
      return 0;
    }

    if (strcmp(arg, "map") == 0) {
      arguments->storage_kind = STORAGE_MAP;
      return 0;
    }

    if (strcmp(arg, "hash_table") == 0) {
      arguments->storage_kind = STORAGE_HASH_TABLE;
      return 0;
    }

    argp_error(state, "unknown storage option '%s'", arg);
  } break;

  case 'v': {
    int v = atoi(arg);

    if (v != 0) {
      arguments->verbosity = v;
      return 0;
    }

    argp_error(state, "invalid verbosity '%s'", arg);
  } break;

  case ARGP_KEY_ARG: {
    if (state->arg_num >= 1)
      argp_usage(state);

    arguments->machine = arg;
  } break;

  case ARGP_KEY_END: {
  } break;

  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {options, parse_opt, "[TURING MACHINE]", NULL};
static char buffer[FMT_BUFFER_SIZE];

int main(int argc, char **argv) {
  arguments_t args = {
      .machine = NULL,
      .storage_kind = STORAGE_HASH_TABLE,
      .verbosity = 0,
  };

  int ret;
  if ((ret = argp_parse(&argp, argc, argv, 0, NULL, &args)) != 0) {
    return ret;
  }

  turing_machine_t turing_machine = turing_machine_create(args.storage_kind);

  reader_t reader;
  if (args.machine != NULL)
    reader = reader_open_static(args.machine, strlen(args.machine));
  else
    reader = reader_open_file(stdin);

  parse_turing_machine(&reader, &turing_machine);
  reader_destroy(&reader);

  if (args.verbosity > 0) {
    printf("interpreted transitions: \n");

    // TODO
    // hash_table_iter_t iter = hash_table_iter(&turing_machine.transitions);
    // while (hash_table_iter_next(&iter)) {
    //   transition_t *transition = iter.value;
    //   format_transition(buffer, FMT_BUFFER_SIZE, format_default,
    //   *transition); printf("  %s\n", buffer);
    // }

    printf("\n");
  }

  size_t steps = 0;
  while (turing_machine_next(&turing_machine)) {
    steps++;
  }

  if (args.verbosity > 0) {
    printf("\n");

    printf("state: ");
    if (turing_machine_is_accepting(&turing_machine)) {
      printf(ANSI_FG_GREEN "ACCEPTING" ANSI_RESET "\n");
    } else {
      printf(ANSI_FG_RED "REJECTING" ANSI_RESET "\n");
    }

    format_tape(buffer, FMT_BUFFER_SIZE, format_default, &turing_machine.tape,
                0);
    printf("content: %s\n", buffer);

    format_tape(buffer, FMT_BUFFER_SIZE, format_default, &turing_machine.tape,
                5);
    printf("\nturing_machine = {tape=%s, ", buffer);
    format_state(buffer, FMT_BUFFER_SIZE, format_default, turing_machine.state);
    printf("state=%s, ", buffer);

    printf("head=%d, ", turing_machine.tape.head);
    printf("steps=%lu}\n", steps);
  }

  turing_machine_destroy(&turing_machine);

  return EXIT_SUCCESS;
}
