#include "reader.h"

#include <assert.h>

size_t static_read(void *dest, size_t size, void *handle) { return EOF; }

size_t file_read(void *dest, size_t size, void *handle) {
  size_t read = fread(dest, 1, size, (FILE *)handle);
  if (read == 0)
    return EOF;
  return read;
}

reader_t reader_open_static(const void *src, size_t size) {
  buffer_t buf = buffer_create();
  buffer_append(&buf, src, size);

  return (reader_t){
      .buf = buf,
      .handle = NULL,
      .read = static_read,
  };
}

reader_t reader_open_file(FILE *file) {
  return (reader_t){
      .buf = buffer_create(),
      .handle = file,
      .read = file_read,
  };
}

void reader_destroy(reader_t *reader) { buffer_destroy(&reader->buf); }

static size_t reader_read(reader_t *reader) {
  static char buf[1024];

  size_t count = reader->read(buf, 1024, reader->handle);
  if (count == EOF)
    return EOF;

  return buffer_append(&reader->buf, buf, count);
}

void reader_read_all(reader_t *reader) {
  while (reader_read(reader) != EOF) {
  }
}

static void reader_next_chunk(reader_t *reader) {
  reader->buf.pos = 0;
  reader_read(reader);
  reader->buf.pos = 0;
}

char reader_current(reader_t *reader) {
  if (!buffer_has_next(&reader->buf))
    reader_next_chunk(reader);

  if (!buffer_has_next(&reader->buf))
    return EOF;

  return buffer_current(&reader->buf);
}

void reader_next(reader_t *reader) {
  if (!buffer_has_next(&reader->buf))
    reader_next_chunk(reader);

  if (!buffer_has_next(&reader->buf))
    return;

  buffer_next(&reader->buf);
}
