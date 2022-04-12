#pragma once

#ifdef DEBUG

#include <stdio.h>

#define ANSI_GREEN "\033[32m"
#define ANSI_RESET "\033[0m"

#define LOG(fmt, ...)                                                          \
  printf(ANSI_GREEN fmt ANSI_RESET __VA_OPT__(, ) __VA_ARGS__);                \
  fflush(stdout)

#else // DEBUG

#define LOG(...)

#endif // DEBUG
