#pragma once

#include "reader.h"
#include "turing-machine/machine.h"

state_t parse_state(reader_t *reader);
symbol_t parse_symbol(reader_t *reader);
direction_t parse_direction(reader_t *reader);
transition_t parse_transition(reader_t *reader);

tape_t parse_tape(reader_t *reader);
turing_machine_t parse_turing_machine(reader_t *reader);
