#include "buffer.h"

#include <assert.h>
#include <stdio.h>

buffer_t buffer_alloc(size_t size) {
  return (buffer_t){
      .ptr = malloc(size),
      .pos = 0,
      .lim = 0,
      .cap = size,
  };
}

void buffer_free(buffer_t *buffer) {
  if (buffer->ptr != NULL)
    free(buffer->ptr);
}

char buffer_current(buffer_t *buffer) {
  if (buffer->pos >= buffer->lim)
    return EOF;
  return buffer->ptr[buffer->pos];
}

int buffer_has_next(buffer_t *buffer) { return buffer->pos < buffer->lim; }

void buffer_next(buffer_t *buffer) { buffer->pos++; }
