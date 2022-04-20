/// @file list.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#ifdef __GNUC__
#define PRINTFLIKE(n, m) __attribute__((format(printf, n, m)))
#else
#define PRINTFLIKE(n, m)
#endif // __GNUC__

typedef enum {
  LEVEL_TRACE,
  LEVEL_DEBUG,
  LEVEL_INFO,
  LEVEL_WARN,
  LEVEL_ERROR,
} level_t;

int log_is_enabled(level_t level);
void log_set_min_level(level_t level);

void log_message(level_t level, const char *fmt, ...) PRINTFLIKE(2, 3);

#define log_trace(...) log_message(LEVEL_TRACE, __VA_ARGS__)
#define log_debug(...) log_message(LEVEL_DEBUG, __VA_ARGS__)
#define log_info(...) log_message(LEVEL_INFO, __VA_ARGS__)
#define log_warn(...) log_message(LEVEL_WARN, __VA_ARGS__)
#define log_error(...) log_message(LEVEL_ERROR, __VA_ARGS__)
