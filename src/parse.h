#pragma once

#include "buffer.h"
#include "turing_machine.h"

#include <stdio.h>

typedef struct reader reader_t;

struct reader {
  buffer_t buf;
  void *handle;

  size_t (*read)(void *dest, size_t size, void *handle);
};

reader_t reader_open_static(void *src, size_t size);
reader_t reader_open_file(FILE *file);

turing_machine_t reader_parse_turing_machine(reader_t *reader);
