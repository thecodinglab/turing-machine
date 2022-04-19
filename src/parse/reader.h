/// @file reader.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include "buffer.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/// A reader can read bytes from some kind of underlying data storage.
///
/// Currently, a reader may be initialized from a static block of memory
/// (::reader_open_static) or from a file (::reader_open_file).
typedef struct {
  /// A buffer for the reader.
  buffer_t buf;
  /// A user data object for the read function.
  void *handle;
  /// A function from which more bytes may be read.
  size_t (*read)(void *dest, size_t size, void *handle);
} reader_t;

/// Creates a reader from the given static memory.
///
/// The entire memory block will be copied and written to the buffer of the
/// reader. Trying to read bytes past the buffer is not possible.
///
/// @param src the block of memory.
/// @param size the number of bytes.
///
/// @return a reader which reads from the given memory.
reader_t reader_open_static(void *src, size_t size);

/// Creates a reader from the given file.
///
/// The reader will read the file in blocks once the current buffer is
/// exhausted. The file must stay open until the reader is destroyed.
///
/// @param file the file to read from.
///
/// @return a reader which reads from the given file.
reader_t reader_open_file(FILE *file);

/// Destroys the reader by releasing any allocated memory.
///
/// @warning if the reader reads from a file, the file must be closed manually.
///
/// @param reader the reader to destroy.
void reader_destroy(reader_t *reader);

/// Gets the current byte of the reader.
///
/// @param reader the reader from which to read the byte.
///
/// @return the current byte of the reader.
char reader_current(reader_t *reader);

/// Increments the position of the reader.
///
/// If the underlying buffer does not have more bytes available, the reader will
/// try to read from the underlying handle (e.g. file).
///
/// @param reader the reader to increment.
void reader_next(reader_t *reader);
