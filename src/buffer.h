#pragma once

#include <stdlib.h>

typedef struct {
  char *ptr;
  size_t pos, lim, cap;
} buffer_t;

buffer_t buffer_alloc(size_t size);
void buffer_free(buffer_t *buffer);

int buffer_has_next(buffer_t *buffer);
char buffer_next(buffer_t *buffer);
void buffer_back(buffer_t *buffer);
