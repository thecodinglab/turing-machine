#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#define ANSI_RESET "\033[0m"

static level_t min_level = LEVEL_INFO;

#ifdef ANSI
static const char *ansi_level_colors[] = {
    "\033[90m", // TRACE
    "\033[32m", // DEBUG
    "\033[0m",  // INFO
    "\033[33m", // WARN
    "\033[31m", // ERROR
};
#endif // ANSI

int log_is_enabled(level_t level) { return level >= min_level; }

void log_set_min_level(level_t level) { min_level = level; }

void log_message(level_t level, const char *fmt, ...) {
  if (!log_is_enabled(level))
    return;

  va_list args;
  va_start(args, fmt);

#ifdef ANSI
  if (isatty(fileno(stdout))) {
    fprintf(stdout, "%s", ansi_level_colors[level]);
  }
#endif // ANSI

  vfprintf(stdout, fmt, args);

#ifdef ANSI
  if (isatty(fileno(stdout))) {
    fprintf(stdout, "%s", ANSI_RESET);
  }
#endif // ANSI

  va_end(args);
}
