#pragma once

#include "machine.h"

typedef struct {
  int allow_unicode_characters;
  int allow_ansi_colors;

  const char *ansi_tape_head;
} format_options_t;

extern const format_options_t format_default;
extern const format_options_t format_debug;

int format_symbol(char *buf, size_t size, format_options_t opts,
                  symbol_t symbol);
int format_state(char *buf, size_t size, format_options_t opts, state_t state);
int format_transition(char *buf, size_t size, format_options_t opts,
                      transition_t transition);
int format_tape(char *buf, size_t size, format_options_t opts, tape_t *tape,
                size_t offset);
