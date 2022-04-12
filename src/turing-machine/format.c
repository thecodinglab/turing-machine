#include "format.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FMT_BUFFER_SIZE 100

#ifdef UNICODE
#define ALLOW_UNICODE 1
#else
#define ALLOW_UNICODE 0
#endif // UNICODE

#ifdef ANSI
#define ALLOW_ANSI 1
#else
#define ALLOW_ANSI 0
#endif // UNICODE

const format_options_t format_default = {
    .allow_unicode_characters = ALLOW_UNICODE,
    .allow_ansi_colors = ALLOW_ANSI,

    .ansi_tape_head = "\033[100m",
};

const format_options_t format_debug = {
    .allow_unicode_characters = ALLOW_UNICODE,
    .allow_ansi_colors = ALLOW_ANSI,

    .ansi_tape_head = "\033[100m",
};

int format_uint32_plain(char *buf, size_t size, uint32_t val) {
  return snprintf(buf, size, "%u", val);
}

int format_uint32_unicode(char *buf, size_t size, uint32_t val) {
  static char buffer[FMT_BUFFER_SIZE];

  format_uint32_plain(buffer, FMT_BUFFER_SIZE, val);

  int w = 0;
  for (size_t i = 0; buffer[i] != 0; i++) {
    char c = (char)(0x80 + (buffer[i] - '0'));

    w += snprintf(buf + w, size - w, "%s%c", "\xe2\x82", c);
    if (w >= size)
      return size;
  }

  return w;
}

int format_symbol_plain(char *buf, size_t size, symbol_t symbol) {
  return snprintf(buf, size, "S%u", symbol);
}

int format_symbol_unicode(char *buf, size_t size, symbol_t symbol) {
  int w = 0;

  w += snprintf(buf + w, size - w, "S");
  if (w >= size)
    return size;

  w += format_uint32_unicode(buf + w, size - w, symbol);
  if (w >= size)
    return size;

  return w;
}

int format_state_plain(char *buf, size_t size, state_t state) {
  return snprintf(buf, size, "q%u", state);
}

int format_state_unicode(char *buf, size_t size, state_t state) {
  int w = 0;

  w += snprintf(buf + w, size - w, "q");
  if (w >= size)
    return size;

  w += format_uint32_unicode(buf + w, size - w, state);
  if (w >= size)
    return size;

  return w;
}

int format_transition_plain(char *buf, size_t size, transition_t transition) {
  int w = 0;

  w += snprintf(buf + w, size - w, "d(");
  if (w >= size)
    return size;

  w += format_state_plain(buf + w, size - w, transition.state);
  if (w >= size)
    return size;

  w += snprintf(buf + w, size - w, ", ");
  if (w >= size)
    return size;

  w += format_symbol_plain(buf + w, size - w, transition.in);
  if (w >= size)
    return size;

  w += snprintf(buf + w, size - w, ") = (");
  if (w >= size)
    return size;

  w += format_symbol_plain(buf + w, size - w, transition.out);
  if (w >= size)
    return size;

  w += snprintf(buf + w, size - w, ", %c, ",
                (transition.dir == DIRECTION_LEFT ? 'L' : 'R'));
  if (w >= size)
    return size;

  w += format_state_plain(buf + w, size - w, transition.next);
  if (w >= size)
    return size;

  w += snprintf(buf + w, size - w, ")");
  if (w >= size)
    return size;

  return w;
}

int format_transition_unicode(char *buf, size_t size, transition_t transition) {
  int w = 0;

  w += snprintf(buf + w, size - w, "\xce\xb4(");
  if (w >= size)
    return size;

  w += format_state_unicode(buf + w, size - w, transition.state);
  if (w >= size)
    return size;

  w += snprintf(buf + w, size - w, ", ");
  if (w >= size)
    return size;

  w += format_symbol_unicode(buf + w, size - w, transition.in);
  if (w >= size)
    return size;

  w += snprintf(buf + w, size - w, ") = (");
  if (w >= size)
    return size;

  w += format_symbol_unicode(buf + w, size - w, transition.out);
  if (w >= size)
    return size;

  w += snprintf(buf + w, size - w, ", %c, ",
                (transition.dir == DIRECTION_LEFT ? 'L' : 'R'));
  if (w >= size)
    return size;

  w += format_state_unicode(buf + w, size - w, transition.next);
  if (w >= size)
    return size;

  w += snprintf(buf + w, size - w, ")");
  if (w >= size)
    return size;

  return w;
}

int format_symbol(char *buf, size_t size, format_options_t opts,
                  symbol_t symbol) {
  if (opts.allow_unicode_characters)
    return format_symbol_unicode(buf, size, symbol);
  else
    return format_symbol_plain(buf, size, symbol);
}

int format_state(char *buf, size_t size, format_options_t opts, state_t state) {
  if (opts.allow_unicode_characters)
    return format_state_unicode(buf, size, state);
  else
    return format_state_plain(buf, size, state);
}

int format_transition(char *buf, size_t size, format_options_t opts,
                      transition_t transition) {
  if (opts.allow_unicode_characters)
    return format_transition_unicode(buf, size, transition);
  else
    return format_transition_plain(buf, size, transition);
}

int format_head_start(char *buf, size_t size, format_options_t opts) {
  if (opts.allow_ansi_colors && isatty(fileno(stdout)))
    return snprintf(buf, size, "%s", opts.ansi_tape_head);
  else
    return snprintf(buf, size, " h ");
}

int format_head_end(char *buf, size_t size, format_options_t opts) {
  if (opts.allow_ansi_colors && isatty(fileno(stdout)))
    return snprintf(buf, size, "\033[49m");
  else
    return 0;
}

int format_symbol_and_head(char *buf, size_t size, format_options_t opts,
                           symbol_t symbol, int is_head) {
  int w = 0;

  if (is_head) {
    w += format_head_start(buf + w, size - w, opts);
    if (w >= size)
      return size;
  }

  w += format_symbol(buf + w, size - w, opts, symbol);
  if (w >= size)
    return size;

  if (is_head) {
    w += format_head_end(buf + w, size - w, opts);
    if (w >= size)
      return size;
  }

  return w;
}

int format_tape(char *buf, size_t size, format_options_t opts,
                turing_machine_t *turing_machine) {
  int w = 0;

  for (size_t i = 0; i < turing_machine->negative.lim; i++) {
    symbol_t symbol = tape_read_at(&turing_machine->negative,
                                   turing_machine->negative.cap - i - 1);

    w += format_symbol_and_head(buf + w, size - w, opts, symbol,
                                turing_machine->head == (-i - 1));

    if (w >= size)
      return size;
  }

  for (size_t i = 0; i < turing_machine->positive.lim; i++) {
    symbol_t symbol = tape_read_at(&turing_machine->positive, i);

    w += format_symbol_and_head(buf + w, size - w, opts, symbol,
                                turing_machine->head == i);

    if (w >= size)
      return size;
  }

  return w;
}
