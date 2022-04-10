#pragma once

#ifdef DEBUG

#include <stdio.h>

#define ANSI_GRAY "\033[90m"
#define ANSI_RESET "\033[0m"

#define LOG(fmt, ...)                                                          \
  printf(ANSI_GRAY fmt ANSI_RESET __VA_OPT__(, ) __VA_ARGS__);                 \
  fflush(stdout)

#else // DEBUG

#define LOG(...)

#endif // DEBUG
