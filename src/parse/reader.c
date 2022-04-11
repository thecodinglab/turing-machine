#include "reader.h"
#include "debug.h"

#include <assert.h>
#include <memory.h>

size_t static_read(void *dest, size_t size, void *handle) { return EOF; }

size_t file_read(void *dest, size_t size, void *handle) {
  return fread(dest, size, 1, (FILE *)handle);
}

reader_t reader_open_static(void *src, size_t size) {
  buffer_t buf = buffer_alloc(size);
  memcpy(buf.ptr, src, size);
  buf.lim = size;

  return (reader_t){
      .buf = buf,
      .handle = NULL,
      .read = static_read,
  };
}

reader_t reader_open_file(FILE *file) {
  return (reader_t){
      .buf = buffer_alloc(1024),
      .handle = file,
      .read = file_read,
  };
}

char reader_current(reader_t *reader) { return buffer_current(&reader->buf); }

void reader_next(reader_t *reader) {
  if (!buffer_has_next(&reader->buf)) {
    size_t count =
        reader->read(reader->buf.ptr, reader->buf.cap, reader->handle);

    if (count == EOF)
      return;

    reader->buf.pos = 0;
    reader->buf.lim = count;
  }

  if (!buffer_has_next(&reader->buf))
    return;

  buffer_next(&reader->buf);
}

uint32_t reader_count_symbols(reader_t *reader, char symbol) {
  uint32_t count = 0;

  while (reader_current(reader) == symbol) {
    count++;
    reader_next(reader);
  }

  return count;
}
