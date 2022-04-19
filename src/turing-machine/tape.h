/// @file tape.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include "util/list.h"

#include <stdint.h>

/// The empty symbol with which any tape will be initialized.
#define SYMBOL_EMPTY 0

/// A symbol is an element on the tape.
typedef uint32_t symbol_t;

/// The tape head describes a specific position on a tape.
///
/// Theoretically, the tape head would be able to move indefinitely in any
/// direction on the tape. However, this implementation currently only supports
/// moving the tape \f$2^{31}-1\f$ positions into each direction.
typedef int32_t tape_head_t;

/// A direction describes the movement of the head of a tape.
///
/// @see tape_move
typedef enum {
  /// A move into the negative (`-1`).
  DIRECTION_LEFT,
  /// A move into the positive (`+1`).
  DIRECTION_RIGHT,
} direction_t;

typedef struct {
  /// Tape symbols of the positive side of the tape (0 to infinity).
  list_t positive;
  /// Tape symbols on the negative side of the tape (-1 to -infinity).
  list_t negative;
  /// Current position of the head.
  tape_head_t head;
} tape_t;

/// Constructs a new empty tape.
///
/// @return a new empty tape.
tape_t tape_create();

/// Destroys the given tape.
///
/// @param tape the tape to destroy.
void tape_destroy(tape_t *tape);

/// Reads the symbol from the provided position on the tape.
///
/// @param tape the tape to read from.
/// @param pos the position to read from.
///
/// @return the read symbol.
symbol_t tape_read_at(tape_t *tape, tape_head_t pos);

/// Writes the given symbol to the provided position on the tape.
///
/// If the underlying tape buffer is not large enough, the buffer will be
/// extended until the position can be written to.
///
/// @param tape the tape to write to.
/// @param pos the position to write to.
/// @param symbol the symbol to write.
void tape_write_at(tape_t *tape, tape_head_t pos, symbol_t symbol);

/// Reads the symbol at the head of the tape.
///
/// @param tape the tape to read from.
///
/// @return the read symbol.
symbol_t tape_read(tape_t *tape);

/// Writes the given symbol to the head of the tape.
///
/// @param tape the tape to write to.
/// @param symbol the symbol to write.
void tape_write(tape_t *tape, symbol_t symbol);

/// Moves the head of the tape one step to the given direction.
///
/// ::DIRECTION_LEFT moves the head one position to the left (negative) and
/// ::DIRECTION_RIGHT moves the head one position to the right (positive). Any
/// other value will be rejected and may result in an error.
///
/// @param tape the tape whose head should be moved.
/// @param direction the direction in which to move.
void tape_move(tape_t *tape, direction_t direction);
