/// @file args.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include "turing-machine/transition.h"
#include "util/log.h"

typedef struct {
  const char *machine;
  transition_storage_kind_t storage_kind;

  int json;

  level_t verbosity;
} arguments_t;

int args_parse(arguments_t *args, int argc, char **argv);
