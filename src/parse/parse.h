/// @file parse.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include "reader.h"
#include "turing-machine/machine.h"

/// Parses a state from the reader.
///
/// A state \f$q_n\f$ is encoded using `DAA...A` where the `A` is repeated n
/// times.
///
/// @param reader the reader to read from.
///
/// @return the parsed state.
state_t parse_state(reader_t *reader);

/// Parses a symbol from the reader.
///
/// A symbol \f$S_n\f$ is encoded using `DCC...C` where the `C` is repeated n
/// times.
///
/// @param reader the reader to read from.
///
/// @return the parsed symbol.
symbol_t parse_symbol(reader_t *reader);

/// Parses a direction from the reader.
///
/// Either `L` for ::DIRECTION_LEFT or `R` for ::DIRECTION_RIGHT.
///
/// @param reader the reader to read from.
///
/// @return the parsed direction.
direction_t parse_direction(reader_t *reader);

/// Parses a transition from the reader.
///
/// A transition is given through its 5-tuple in the format
/// `[current state][in symbol][out symbol][direction][next state]`.
///
/// `DADCDCRDA` describes the transition \f$\delta(q_1, S_1) = (S_1, R, q_1)\f$
///
/// @param reader the reader to read from.
///
/// @return the parsed transition.
transition_t parse_transition(reader_t *reader);

/// Parses a tape from the reader.
///
/// The tape is given through its symbols on that tape. The tape is parsed until
/// a `;` is found.
///
/// @param reader the reader to read from.
///
/// @return the parsed tape.
///
/// @see parse_symbol
tape_t parse_tape(reader_t *reader);

/// Parses an entire turing machine from the reader.
///
/// A turing machine is given through the initial tape content and its
/// transitions. The encoded machine must be in the format `[initial
/// tape];[transition 1];[transition 2];...;[transition n]`.
///
/// If no initial tape is required, the string may be empty, but the leading `;`
/// is still required.
///
/// An example turing machine, which generates the sequence
/// \f$S_0S_1S_0S_1\dots\f$ may be encoded using
/// `;DADDRDAA;DADCDRDAA;DAADDCRDA;DAADCDCRDA`.
///
/// @param reader the reader to read from.
/// @param dest the destination where the turing machine will be stored.
void parse_turing_machine(reader_t *reader, turing_machine_t *dest);
