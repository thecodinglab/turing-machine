#include "buffer.h"

#include <assert.h>

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

int buffer_has_next(buffer_t *buffer) { return buffer->pos < buffer->lim; }

char buffer_next(buffer_t *buffer) {
  assert(buffer->pos < buffer->lim);
  return buffer->ptr[buffer->pos++];
}

void buffer_back(buffer_t *buffer) {
  if (buffer->pos != 0)
    buffer->pos--;
}
