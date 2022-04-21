#include "parse.h"
#include "util/log.h"
#include "util/map.h"

#include <assert.h>
#include <json-c/json.h>
#include <string.h>

#define SYMBOL_LEN 10
static char symbol_buf[SYMBOL_LEN];

static int str_cmp(void *a, void *b) { return strcmp(a, b); }

static void json_parse_alphabet(struct json_object *obj, map_t *symbol_lookup) {
  assert(json_object_get_type(obj) == json_type_array);

  struct json_object *item;

  size_t len = json_object_array_length(obj);
  for (size_t i = 0; i < len; i++) {
    item = json_object_array_get_idx(obj, i);
    assert(json_object_get_type(item) == json_type_string);

    const char *str = json_object_get_string(item);
    strncpy(symbol_buf, str, SYMBOL_LEN - 1);

    symbol_t symbol = i;
    map_insert(symbol_lookup, symbol_buf, &symbol);
  }
}

static symbol_t json_parse_symbol(struct json_object *obj,
                                  map_t *symbol_lookup) {

  assert(json_object_get_type(obj) == json_type_string);

  const char *str = json_object_get_string(obj);
  strncpy(symbol_buf, str, SYMBOL_LEN - 1);

  symbol_t *symbol = map_find(symbol_lookup, symbol_buf);
  return *symbol;
}

static void json_parse_transition(struct json_object *obj, map_t *symbol_lookup,
                                  transition_t *dest) {
  assert(json_object_get_type(obj) == json_type_array);
  assert(json_object_array_length(obj) == 3);

  json_object *curr;

  // parse input/output symbols
  dest->in =
      json_parse_symbol(json_object_array_get_idx(obj, 0), symbol_lookup);
  dest->out =
      json_parse_symbol(json_object_array_get_idx(obj, 1), symbol_lookup);

  // parse tape direction
  assert((curr = json_object_array_get_idx(obj, 2)));
  assert(json_object_get_type(curr) == json_type_string);

  const char *dir_str = json_object_get_string(curr);
  dest->dir = (strcmp(dir_str, "L") == 0) ? DIRECTION_LEFT : DIRECTION_RIGHT;
}

static void json_parse_state_transitions(struct json_object *obj,
                                         map_t *symbol_lookup,
                                         turing_machine_t *machine) {
  assert(json_object_get_type(obj) == json_type_array);
  transition_t transition = {};

  struct json_object *item, *curr;

  size_t len = json_object_array_length(obj);
  for (size_t i = 0; i < len; i++) {
    item = json_object_array_get_idx(obj, i);
    assert(json_object_get_type(item) == json_type_object);

    // parse current state before transition
    assert(json_object_object_get_ex(item, "Source", &curr));
    assert(json_object_get_type(curr) == json_type_int);
    transition.state = json_object_get_int64(curr);

    // parse next state after transition
    assert(json_object_object_get_ex(item, "Target", &curr));
    assert(json_object_get_type(curr) == json_type_int);
    transition.next = json_object_get_int64(curr);

    // parse transitions using the given states
    assert(json_object_object_get_ex(item, "Labels", &curr));
    assert(json_object_get_type(curr) == json_type_array);

    size_t len = json_object_array_length(curr);
    for (size_t j = 0; j < len; j++) {
      json_parse_transition(json_object_array_get_idx(curr, j), symbol_lookup,
                            &transition);
      transition_storage_add(machine->storage, transition);
    }
  }
}

static void json_parse_states(struct json_object *obj, map_t *symbol_lookup,
                              turing_machine_t *machine) {
  assert(json_object_get_type(obj) == json_type_array);

  struct json_object *item, *curr;

  size_t len = json_object_array_length(obj);
  for (size_t i = 0; i < len; i++) {
    item = json_object_array_get_idx(obj, i);

    // parse state identifier
    assert(json_object_object_get_ex(item, "ID", &curr));
    assert(json_object_get_type(curr) == json_type_int);
    state_t id = json_object_get_int64(curr);

    // parse starting state
    assert(json_object_object_get_ex(item, "Start", &curr));
    assert(json_object_get_type(curr) == json_type_boolean);

    if (json_object_get_boolean(curr)) {
      machine->state = id;
    }

    // parse accepting state
    assert(json_object_object_get_ex(item, "Final", &curr));
    assert(json_object_get_type(curr) == json_type_boolean);

    if (json_object_get_boolean(curr)) {
      turing_machine_add_accepting_state(machine, id);
    }

    // parse transitions
    json_parse_state_transitions(json_object_object_get(item, "Transitions"),
                                 symbol_lookup, machine);
  }
}

static void json_parse_tape(struct json_object *obj, map_t *symbol_lookup,
                            tape_t *tape) {
  assert(json_object_get_type(obj) == json_type_array);

  size_t len = json_object_array_length(obj);
  for (size_t i = 0; i < len; i++) {
    symbol_t symbol =
        json_parse_symbol(json_object_array_get_idx(obj, i), symbol_lookup);
    tape_write_at(tape, i, symbol);
  }
}

static void json_parse_turing_machine(struct json_object *obj,
                                      turing_machine_t *machine) {
  assert(json_object_get_type(obj) == json_type_object);

  struct json_object *automaton, *curr;

  assert(json_object_object_get_ex(obj, "type", &curr));
  assert(json_object_get_type(curr) == json_type_string);
  assert(strcmp(json_object_get_string(curr), "TM") == 0);

  assert(json_object_object_get_ex(obj, "automaton", &automaton));
  assert(json_object_get_type(automaton) == json_type_object);

  map_t symbol_lookup = map_create(SYMBOL_LEN, sizeof(symbol_t), str_cmp);

  assert(json_object_object_get_ex(automaton, "StackAlphabet", &curr));
  json_parse_alphabet(curr, &symbol_lookup);

  assert(json_object_object_get_ex(automaton, "simulationInput", &curr));
  json_parse_tape(curr, &symbol_lookup, &machine->tape);

  assert(json_object_object_get_ex(automaton, "States", &curr));
  json_parse_states(curr, &symbol_lookup, machine);

  map_destroy(&symbol_lookup);
}

void parse_turing_machine_from_json(reader_t *reader, turing_machine_t *dest) {
  reader_read_all(reader);
  reader->buf.pos = 0;

  if ((uint8_t)reader_current(reader) == 0xef) {
    // skip utf-8 header
    reader_next(reader);
    reader_next(reader);
    reader_next(reader);
  }

  struct json_tokener *tokener = json_tokener_new();

  struct json_object *obj = json_tokener_parse_ex(
      tokener, reader->buf.ptr + reader->buf.pos, reader->buf.lim);

  enum json_tokener_error err = json_tokener_get_error(tokener);
  if (err != json_tokener_success) {
    log_error("parse json: %s at offset %lu\n", json_tokener_error_desc(err),
              json_tokener_get_parse_end(tokener));
    return;
  }

  json_parse_turing_machine(obj, dest);

  json_tokener_free(tokener);
}
