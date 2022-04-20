#include "transition.h"
#include "util/hash_table.h"
#include "util/list.h"
#include "util/map.h"

#include <stdlib.h>

struct transition_storage {
  transition_storage_kind_t kind;

  union {
    list_t list;
    map_t map;
    hash_table_t table;
  };
};

typedef struct {
  state_t state;
  symbol_t symbol;
} transition_key_t;

static int transition_key_cmp(void *a_ptr, void *b_ptr) {
  transition_key_t *a = (transition_key_t *)a_ptr;
  transition_key_t *b = (transition_key_t *)b_ptr;

  int cmp = b->state - a->state;
  if (cmp == 0)
    cmp = b->symbol - a->symbol;

  return cmp;
}

transition_storage_t *
transition_storage_create(transition_storage_kind_t kind) {
  transition_storage_t *storage = malloc(sizeof(transition_storage_t));
  storage->kind = kind;

  switch (kind) {
  case STORAGE_LIST:
    storage->list = list_create(1024, sizeof(transition_t));
    break;

  case STORAGE_MAP:
    storage->map = map_create(sizeof(transition_key_t), sizeof(transition_t),
                              transition_key_cmp);
    break;

  case STORAGE_HASH_TABLE:
    storage->table = hash_table_create(
        sizeof(transition_key_t), sizeof(transition_t), transition_key_cmp);
    break;
  }

  return storage;
}

void transition_storage_destroy(transition_storage_t *storage) {
  switch (storage->kind) {
  case STORAGE_LIST:
    list_destroy(&storage->list);
    break;

  case STORAGE_MAP:
    map_destroy(&storage->map);
    break;

  case STORAGE_HASH_TABLE:
    hash_table_destroy(&storage->table);
    break;
  }

  free(storage);
}

static inline transition_t *list_storage_find(list_t *list, state_t state,
                                              symbol_t symbol) {
  for (size_t i = 0; i < list->count; i++) {
    transition_t *transition = list_get(list, i);
    if (transition->state == state && transition->in == symbol) {
      return transition;
    }
  }

  return NULL;
}

static inline transition_t *map_storage_find(map_t *map, state_t state,
                                             symbol_t symbol) {
  transition_key_t key = {
      .state = state,
      .symbol = symbol,
  };

  return map_find(map, &key);
}

static inline transition_t *table_storage_find(hash_table_t *table,
                                               state_t state, symbol_t symbol) {
  transition_key_t key = {
      .state = state,
      .symbol = symbol,
  };

  return hash_table_find(table, &key);
}

transition_t *transition_storage_find(transition_storage_t *storage,
                                      state_t state, symbol_t symbol) {
  switch (storage->kind) {
  case STORAGE_LIST:
    return list_storage_find(&storage->list, state, symbol);

  case STORAGE_MAP:
    return map_storage_find(&storage->map, state, symbol);

  case STORAGE_HASH_TABLE:
    return table_storage_find(&storage->table, state, symbol);
  }

  return NULL;
}

static inline void list_storage_add(list_t *list, transition_t transition) {
  list_append(list, list->count, &transition);
}

static inline void map_storage_add(map_t *map, transition_t transition) {
  transition_key_t key = {
      .state = transition.state,
      .symbol = transition.in,
  };

  map_insert(map, &key, &transition);
}

static inline void table_storage_add(hash_table_t *table,
                                     transition_t transition) {
  transition_key_t key = {
      .state = transition.state,
      .symbol = transition.in,
  };

  hash_table_insert(table, &key, &transition);
}

void transition_storage_add(transition_storage_t *storage,
                            transition_t transition) {
  switch (storage->kind) {
  case STORAGE_LIST:
    return list_storage_add(&storage->list, transition);

  case STORAGE_MAP:
    return map_storage_add(&storage->map, transition);

  case STORAGE_HASH_TABLE:
    return table_storage_add(&storage->table, transition);
  }
}
