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

/// Creates an empty byte buffer.
///
/// @return an empty byte buffer.
buffer_t buffer_create();

/// Destroys the given byte buffer by releasing any allocaed memory.
///
/// @param buffer the buffer to destroy.
void buffer_destroy(buffer_t *buffer);

/// Ensures that the buffer has at least the given capacity. If the minimum
/// requirement is not set, the buffer will be expanded.
///
/// @param buffer the buffer to ensure the capacity for.
/// @param size the minimum number of bytes.
void buffer_extend(buffer_t *buffer, size_t size);

/// Writes the given block of data to the buffer. If the size exceeds the
/// current buffer, the buffer will be expanded.
///
/// @param buffer the buffer to write to.
/// @param data the data to be written.
/// @param size the total bytes of data to be written.
///
/// @return the total number of written bytes.
int buffer_append(buffer_t *buffer, const void *data, size_t size);

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
