/// @file buffer.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include <stdlib.h>

/// A byte buffer from which bytes can be read or written to. The size of the
/// byte buffer is static and should not be adjusted after the allocation.
typedef struct {
  /// The pointer to the underlying memory region of the buffer.
  char *ptr;
  /// The current position inside the buffer.
  size_t pos;
  /// The maximum number of bytes readable on the buffer.
  size_t lim;
  /// The total capacity of the buffer.
  size_t cap;
} buffer_t;

/// Allocates a byte buffer with the given size.
///
/// @param size the amount of bytes to allocate.
///
/// @return a byte buffer with the given capacity.
buffer_t buffer_alloc(size_t size);

/// Destroys the given byte buffer by releasing any allocaed memory.
///
/// @param buffer the buffer to destroy.
void buffer_free(buffer_t *buffer);

/// Checks whether the given byte buffer is empty.
///
/// @param buffer the buffer to check.
///
/// @return `0` if the buffer is not empty.
int buffer_is_empty(buffer_t *buffer);

/// Gets the current byte at the position of the buffer.
///
/// @param buffer the buffer from which to get the current byte from.
///
/// @return the current byte at the position (buffer_t::pos) of the given
///         buffer.
char buffer_current(buffer_t *buffer);

/// Checks if the buffer has more bytes available.
///
/// @param buffer the buffer to check.
///
/// @return `0` if the buffer has reached the end.
int buffer_has_next(buffer_t *buffer);

/// Increments the position of the buffer to the next byte.
///
/// @param buffer the buffer whose position should be incremented.
void buffer_next(buffer_t *buffer);
