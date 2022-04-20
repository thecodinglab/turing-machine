/// @file transition.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include "tape.h"

#include <stdint.h>

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

typedef enum {
  STORAGE_LIST,
  STORAGE_MAP,
  STORAGE_HASH_TABLE,
} transition_storage_kind_t;

typedef struct transition_storage transition_storage_t;

/// Creates a new transition storage container using the given storage kind.
///
/// @return a new transition storage container.
transition_storage_t *transition_storage_create(transition_storage_kind_t kind);

/// Destroys the given transition storage container by releasing any allocated
/// memory.
///
/// @param storage the storage container to destroy.
void transition_storage_destroy(transition_storage_t *storage);

/// Tries to find a transition matching the given state and symbol from the
/// underlying storage container.
///
/// @param storage the storage container to fetch from.
/// @param state the input state for the transition.
/// @param symbol the input symbol for the transition.
///
/// @return a pointer to the transition object or `NULL` if no transition has
///         been found.
transition_t *transition_storage_find(transition_storage_t *storage,
                                      state_t state, symbol_t symbol);

/// Adds the given transition to the transition storage container.
///
/// @param storage the transition storage container to attach the transition to.
/// @param transition the transition to attach.
void transition_storage_add(transition_storage_t *storage,
                            transition_t transition);
