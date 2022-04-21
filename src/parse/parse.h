/// @file parse.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include "reader.h"
#include "turing-machine/machine.h"

/// Parses an entire turing machine from the reader using the encoding from Alan
/// Turing.
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
void parse_turing_machine_from_alan(reader_t *reader, turing_machine_t *dest);

/// Parses an entire turing machine from the reader using the json encoding from
/// https://flaci.com.
///
/// @param reader the reader to read from.
/// @param dest the destination where the turing machine will be stored.
void parse_turing_machine_from_json(reader_t *reader, turing_machine_t *dest);
