#pragma once

#include "buffer.h"

#include <stddef.h>
#include <stdio.h>

typedef struct reader reader_t;

struct reader {
  buffer_t buf;
  void *handle;

  size_t (*read)(void *dest, size_t size, void *handle);
};

reader_t reader_open_static(void *src, size_t size);
reader_t reader_open_file(FILE *file);

char reader_current(reader_t *reader);
void reader_next(reader_t *reader);

uint32_t reader_count_symbols(reader_t *reader, char symbol);
