/// A Turing machine is a mathematical model of computation that defines an
/// abstract machine that manipulates symbols on a strip of tape according to a
/// table of rules. Despite the model's simplicity, given any computer
/// algorithm, a Turing machine capable of implementing that algorithm's logic
/// can be constructed.
/// Source: [Wikipedia](https://en.wikipedia.org/wiki/Turing_machine)
///
/// @file machine.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include <stdint.h>

#include "tape.h"

/// The accepting state of every turing machine.
#define ACCEPTING_STATE 0
/// The starting state of every turing machine.
#define STARTING_STATE 1

/// The maximum number of transitions per turing machine.
#define MAX_TRANSITION_COUNT 100

/// A state is an indication for the turing machine to know which transitions
/// can currently apply to the input symbol. Each state can have a separate set
/// of transitions.
///
/// Theoretically, a turing machine could have an infinite amount of states.
/// However, this implementation currently only supports \f$2^{32}-1\f$
/// individual states.
typedef uint32_t state_t;

/// A transition is a 5-tuple to describe the movement of a turing machine.
typedef struct {
  /// The required state of the turing machine.
  ///
  /// If the turing machine is not in this state, this transition will not
  /// apply.
  state_t state;
  /// The required symbol on the tape.
  ///
  /// If this symbol is not the symbol on the tape, this transition will not
  /// apply.
  symbol_t in;
  /// A symbol which will be written to the tape.
  symbol_t out;
  /// The direction for the tape head to move in.
  direction_t dir;
  /// The next state for the turing machine.
  state_t next;
} transition_t;

/// A turing machine is a machine that manipulates symbols on a tape according
/// to a table of rules (transitions).
typedef struct {
  /// The tape of the turing machine.
  tape_t tape;
  /// The current state.
  state_t state;
  /// The transitions of the turing machine.
  transition_t transitions[MAX_TRANSITION_COUNT];
  /// The number of transitions.
  size_t transition_count;
} turing_machine_t;

/// Constructs a new empty turing machine.
///
/// @return an empty turing machine.
turing_machine_t turing_machine_create();

/// Destroys the given turing machine by releasing any allocated memory.
///
/// @param turing_machine the turing machine to destroy.
void turing_machine_destroy(turing_machine_t *turing_machine);

/// Checks wheather the turing machine is in an accepting state.
///
/// @return `0` if the turing machine is not accepting.
int turing_machine_is_accepting(turing_machine_t *turing_machine);

/// Processes the next transition of the turing machine.
///
/// This function should be used in a while loop until the turing machine has
/// halted. Afterwards, the accepting state of the turing machine may be
/// checked.
/// @code
/// turing_machine_t turing_machine;
/// // initialize turing machine...
///
/// while (turing_machine_next(turing_machine)) {
/// }
///
/// if (turing_machine_is_accepting(turing_machine)) {
///   // output or interpret computation result of the turing machine on the
///   // tape...
/// }
/// @endcode
///
/// @return `0` if the turing machine has halted.
int turing_machine_next(turing_machine_t *turing_machine);
