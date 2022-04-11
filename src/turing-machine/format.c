#include "format.h"

#include <stdio.h>
#include <string.h>

#define FMT_BUFFER_SIZE 100

int format_uint32_plain(char *buf, size_t size, uint32_t val) {
  return snprintf(buf, size, "%u", val);
}

int format_uint32_unicode(char *buf, size_t size, uint32_t val) {
  static char buffer[FMT_BUFFER_SIZE];

  format_uint32_plain(buffer, FMT_BUFFER_SIZE, val);

  int w = 0;
  for (size_t i = 0; buffer[i] != 0; i++) {
    char c = (char)(0x80 + (buffer[i] - '0'));
    w += snprintf(buf + w, size - w, "\xe2\x82%c", c);
  }

  return w;
}

int format_symbol_plain(char *buf, size_t size, symbol_t symbol) {
  return snprintf(buf, size, "S%u", symbol);
}

int format_symbol_unicode(char *buf, size_t size, symbol_t symbol) {
  int w = 0;
  w += snprintf(buf + w, size - w, "S");
  w += format_uint32_unicode(buf + w, size - w, symbol);
  return w;
}

int format_state_plain(char *buf, size_t size, state_t state) {
  return snprintf(buf, size, "q%u", state);
}

int format_state_unicode(char *buf, size_t size, state_t state) {
  int w = 0;
  w += snprintf(buf + w, size - w, "q");
  w += format_uint32_unicode(buf + w, size - w, state);
  return w;
}

int format_transition_plain(char *buf, size_t size, transition_t transition) {
  int w = 0;

  w += snprintf(buf + w, size - w, "d(");
  w += format_state_plain(buf + w, size - w, transition.state);
  w += snprintf(buf + w, size - w, ", ");
  w += format_symbol_plain(buf + w, size - w, transition.in);

  w += snprintf(buf + w, size - w, ") = (");
  w += format_symbol_plain(buf + w, size - w, transition.out);
  w += snprintf(buf + w, size - w, ", %c, ",
                (transition.dir == DIRECTION_LEFT ? 'L' : 'R'));
  w += format_state_plain(buf + w, size - w, transition.next);
  w += snprintf(buf + w, size - w, ")");

  return w;
}

int format_transition_unicode(char *buf, size_t size, transition_t transition) {
  int w = 0;

  w += snprintf(buf + w, size - w, "\xce\xb4(");
  w += format_state_unicode(buf + w, size - w, transition.state);
  w += snprintf(buf + w, size - w, ", ");
  w += format_symbol_unicode(buf + w, size - w, transition.in);

  w += snprintf(buf + w, size - w, ") = (");
  w += format_symbol_unicode(buf + w, size - w, transition.out);
  w += snprintf(buf + w, size - w, ", %c, ",
                (transition.dir == DIRECTION_LEFT ? 'L' : 'R'));
  w += format_state_unicode(buf + w, size - w, transition.next);
  w += snprintf(buf + w, size - w, ")");

  return w;
}

int format_symbol(char *buf, size_t size, symbol_t symbol) {
#ifdef UNICODE
  return format_symbol_unicode(buf, size, symbol);
#else
  return format_symbol_plain(buf, size, symbol);
#endif // UNICODE
}

int format_state(char *buf, size_t size, state_t state) {
#ifdef UNICODE
  return format_state_unicode(buf, size, state);
#else
  return format_state_plain(buf, size, state);
#endif // UNICODE
}

int format_transition(char *buf, size_t size, transition_t transition) {
#ifdef UNICODE
  return format_transition_unicode(buf, size, transition);
#else
  return format_transition_plain(buf, size, transition);
#endif // UNICODE
}

int format_tape(char *buf, size_t size, turing_machine_t *turing_machine) {
  int w = 0;

  for (size_t i = 0; i < turing_machine->negative.lim; i++) {
    symbol_t symbol = tape_read_at(&turing_machine->negative,
                                   turing_machine->negative.cap - i - 1);

    w += format_symbol(buf + w, size - w, symbol);
  }

  for (size_t i = 0; i < turing_machine->positive.lim; i++) {
    symbol_t symbol = tape_read_at(&turing_machine->positive, i);

    w += format_symbol(buf + w, size - w, symbol);
  }

  return w;
}
