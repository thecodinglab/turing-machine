#include "args.h"

#include <argp.h>
#include <stdlib.h>
#include <string.h>

static struct argp_option options[] = {
    {"json", 'j', NULL, 0, "When set will parse the input from json"},
    {"storage", 's', "kind", 0,
     "The kind of storage to use for the transitions (default: hash_table)."},
    {"verbosity", 'v', "level", 0,
     "The verbosity level of the console output."},
    {0},
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  arguments_t *arguments = state->input;

  switch (key) {
  case 'j': {
    arguments->json = 1;
  } break;

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
    arguments->verbosity = atoi(arg);
    return 0;
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

int args_parse(arguments_t *args, int argc, char **argv) {
  return argp_parse(&argp, argc, argv, 0, NULL, args);
}
