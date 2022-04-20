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

#include "tape.h"
#include "transition.h"

#include <stdint.h>

/// The accepting state of every turing machine.
#define ACCEPTING_STATE 0
/// The starting state of every turing machine.
#define STARTING_STATE 1

/// A turing machine is a machine that manipulates symbols on a tape according
/// to a table of rules (transitions).
typedef struct {
  /// The tape of the turing machine.
  tape_t tape;
  /// The current state.
  state_t state;
  /// The storage container for the transitions.
  transition_storage_t *storage;
} turing_machine_t;

/// Constructs a new empty turing machine.
///
/// @param kind the storage kind to store the transitions.
///
/// @return an empty turing machine.
turing_machine_t turing_machine_create(transition_storage_kind_t kind);

/// Destroys the given turing machine by releasing any allocated memory.
///
/// @param turing_machine the turing machine to destroy.
void turing_machine_destroy(turing_machine_t *turing_machine);

/// Adds the given transition to the turing machine.
///
/// @param turing_machine the turing machine to attach the transition to.
/// @param transition the transition to attach.
void turing_machine_add_transition(turing_machine_t *turing_machine,
                                   transition_t transition);

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
