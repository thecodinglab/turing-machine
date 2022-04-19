/// @file format.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include "machine.h"

typedef struct {
  /// Whether to allow unicode characters.
  int allow_unicode_characters;
  /// Wheather to allow ansi color codes.
  int allow_ansi_colors;
  /// The ansi color code for the tape head.
  const char *ansi_tape_head;
} format_options_t;

/// Default options for outputting on the console.
extern const format_options_t format_default;
/// Default options for outputting in debug logs.
extern const format_options_t format_debug;

/// Prints the given symbol to the output buffer.
///
/// @param buf the buffer to write to.
/// @param size the number of bytes available in the buffer.
/// @param opts the formatting options to use.
/// @param symbol the symbol which should be written to the buffer.
///
/// @return the total number of bytes written to the buffer.
int format_symbol(char *buf, size_t size, format_options_t opts,
                  symbol_t symbol);

/// Prints the given state to the output buffer.
///
/// @param buf the buffer to write to.
/// @param size the number of bytes available in the buffer.
/// @param opts the formatting options to use.
/// @param state the state which should be written to the buffer.
///
/// @return the total number of bytes written to the buffer.
int format_state(char *buf, size_t size, format_options_t opts, state_t state);

/// Prints the given transition to the output buffer.
///
/// @param buf the buffer to write to.
/// @param size the number of bytes available in the buffer.
/// @param opts the formatting options to use.
/// @param transition the transition which should be written to the buffer.
///
/// @return the total number of bytes written to the buffer.
int format_transition(char *buf, size_t size, format_options_t opts,
                      transition_t transition);

/// Prints the given tape to the output buffer.
///
/// The position of the head will be highlighted using ansi colors if enabled or
/// the character `h` before the current symbol on the tape.
///
/// @param buf the buffer to write to.
/// @param size the number of bytes available in the buffer.
/// @param opts the formatting options to use.
/// @param tape the tape which should be written to the buffer.
/// @param offset a number of symbols which should be pre- and appended outside
///               of the current bounds of the tape.
///
/// @return the total number of bytes written to the buffer.
int format_tape(char *buf, size_t size, format_options_t opts, tape_t *tape,
                size_t offset);
