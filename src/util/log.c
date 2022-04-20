#include "log.h"

#include <stdarg.h>
#include <stdio.h>

static level_t min_level = LEVEL_INFO;

int log_is_enabled(level_t level) { return level >= min_level; }

void log_set_min_level(level_t level) { min_level = level; }

void log_message(level_t level, const char *fmt, ...) {
  if (!log_is_enabled(level))
    return;

  va_list args;
  va_start(args, fmt);

  vfprintf(stdout, fmt, args);

  va_end(args);
}
