#include "hash_table.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#define INITIAL_CAPACITY (1 << 6)

#define FNV_OFFSET 2166136261
#define FNV_PRIME 16777619

static uint32_t fnv1a_hash(const void *data, size_t size) {
  uint64_t hash = FNV_OFFSET;
  for (size_t i = 0; i < size; i++) {
    hash ^= (uint64_t)(*(uint8_t *)(data + i));
    hash *= FNV_PRIME;
  }
  return hash;
}

typedef uint8_t hash_table_flags_t;

#define FLAGS_EMPTY 0
#define FLAGS_SET 1

hash_table_t hash_table_create(size_t key_size, size_t value_size,
                               hash_table_cmp_func_t cmp) {
  hash_table_t table = {
      .capacity = INITIAL_CAPACITY,
      .count = 0,

      .key_size = key_size,
      .value_size = value_size,
      .entry_size = sizeof(hash_table_flags_t) + key_size + value_size,

      .hash = fnv1a_hash,
      .cmp = cmp,
  };

  table.entries = calloc(INITIAL_CAPACITY, table.entry_size);

  return table;
}

void hash_table_destroy(hash_table_t *table) { free(table->entries); }

static inline hash_table_flags_t *get_flags_ptr(hash_table_t *table,
                                                size_t idx) {
  return table->entries + idx * table->entry_size;
}

static inline hash_table_key_t get_key_ptr(hash_table_t *table, size_t idx) {
  return get_flags_ptr(table, idx) + sizeof(hash_table_flags_t);
}

static inline hash_table_key_t get_value_ptr(hash_table_t *table, size_t idx) {
  return get_key_ptr(table, idx) + table->key_size;
}

hash_table_value_t hash_table_find(hash_table_t *table, hash_table_key_t key) {
  uint32_t hash = table->hash(key, table->key_size);
  size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

  // loop till we reach an empty key.
  while (*get_flags_ptr(table, index) != FLAGS_EMPTY) {
    if (table->cmp(key, get_key_ptr(table, index)) == 0) {
      // found matching key, return value.
      return get_value_ptr(table, index);
    }

    // TODO use & (capacity - 1) for speed improvement
    index = (index + 1) % table->capacity;
  }

  return NULL;
}

static int set_entry(hash_table_t *table, hash_table_key_t key,
                     hash_table_value_t value) {
  uint32_t hash = table->hash(key, table->key_size);
  size_t index = (size_t)(hash & (uint32_t)(table->capacity - 1));

  // loop till we reach an empty key.
  while (*get_flags_ptr(table, index) != FLAGS_EMPTY) {
    if (table->cmp(key, get_key_ptr(table, index)) == 0) {
      // found matching key (key already exists), update value.
      memmove(get_value_ptr(table, index), value, table->value_size);
      return 0;
    }

    // TODO use & (capacity - 1) for speed improvement
    index = (index + 1) % table->capacity;
  }

  *get_flags_ptr(table, index) = FLAGS_SET;
  memmove(get_key_ptr(table, index), key, table->key_size);
  memmove(get_value_ptr(table, index), value, table->value_size);

  return 1;
}

static void hash_table_expand(hash_table_t *table) {
  hash_table_t new = *table;

  new.capacity = table->capacity * 2;
  assert(new.capacity > table->capacity); // check for overflow

  new.entries = calloc(new.capacity, new.entry_size);
  assert(new.entries); // check for memory allocation

  // iterate entries, move all non-empty ones to new table's entries.
  for (size_t i = 0; i < table->capacity; i++) {
    if (*get_flags_ptr(table, i) != FLAGS_EMPTY) {
      set_entry(&new, get_key_ptr(table, i), get_value_ptr(table, i));
    }
  }

  // free old entries array and update this table's details.
  free(table->entries);
  *table = new;
}

void hash_table_insert(hash_table_t *table, hash_table_key_t key,
                       hash_table_value_t value) {
  assert(value != NULL);

  // if length will exceed half of current capacity, expand it.
  if (table->count >= table->capacity / 2) {
    hash_table_expand(table);
  }

  // set entry and update length.
  if (set_entry(table, key, value)) {
    table->count++;
  }
}

hash_table_iter_t hash_table_iter(hash_table_t *table) {
  return (hash_table_iter_t){
      .key = NULL,
      .value = NULL,
      .table = table,
      .index = 0,
  };
}

int hash_table_iter_next(hash_table_iter_t *iter) {
  if (iter->index >= iter->table->capacity) {
    return 0;
  }

  // loop till we reach an non empty key.
  while (*get_flags_ptr(iter->table, iter->index) == FLAGS_EMPTY) {
    iter->index++;

    if (iter->index >= iter->table->capacity) {
      return 0;
    }
  }

  iter->key = get_key_ptr(iter->table, iter->index);
  iter->value = get_value_ptr(iter->table, iter->index);
  iter->index++;
  return 1;
}
