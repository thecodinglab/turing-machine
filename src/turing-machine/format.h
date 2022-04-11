#pragma once

#include "machine.h"

int format_symbol(char *buf, size_t size, symbol_t symbol);
int format_state(char *buf, size_t size, state_t state);
int format_transition(char *buf, size_t size, transition_t transition);
int format_tape(char *buf, size_t size, turing_machine_t *turing_machine);
