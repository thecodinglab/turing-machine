#include "buffer.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>

buffer_t buffer_create() {
  return (buffer_t){
      .ptr = NULL,
      .pos = 0,
      .lim = 0,
      .cap = 0,
  };
}

void buffer_destroy(buffer_t *buffer) {
  if (buffer->ptr != NULL)
    free(buffer->ptr);
}

void buffer_extend(buffer_t *buffer, size_t size) {
  if (buffer->cap >= size)
    return;

  buffer->ptr = realloc(buffer->ptr, size);
  buffer->cap = size;
}

int buffer_append(buffer_t *buffer, const void *data, size_t size) {
  size_t req = buffer->pos + size;
  if (req > buffer->cap)
    buffer_extend(buffer, req);

  memcpy(buffer->ptr + buffer->pos, data, size);
  buffer->pos += size;
  buffer->lim = buffer->pos;

  return size;
}

char buffer_current(buffer_t *buffer) {
  if (buffer->pos >= buffer->lim)
    return EOF;

  return buffer->ptr[buffer->pos];
}

int buffer_has_next(buffer_t *buffer) { return buffer->pos < buffer->lim; }

void buffer_next(buffer_t *buffer) { buffer->pos++; }
