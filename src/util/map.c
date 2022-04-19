#include "map.h"

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

map_t map_create(size_t key_size, size_t value_size, map_cmp_func_t cmp) {
  return (map_t){
      .items = list_create(0, key_size + value_size),
      .key_size = key_size,
      .value_size = value_size,
      .buffer = malloc(key_size + value_size),
      .cmp = cmp,
  };
}

void map_destroy(map_t *map) {
  free(map->buffer);
  list_destroy(&map->items);
}

static inline map_key_t map_get_key_ptr(map_t *map, size_t idx) {
  return list_get(&map->items, idx) + 0;
}

static inline map_value_t map_get_value_ptr(map_t *map, size_t idx) {
  return list_get(&map->items, idx) + map->key_size;
}

typedef struct {
  size_t index;
  int cmp;
} binary_search_result_t;

static binary_search_result_t binary_search(map_t *map, map_key_t key) {
  int left = 0, right = map->items.count - 1, cmp = -1;

  while (left <= right) {
    int pivot = left + (right - left) / 2;
    cmp = map->cmp(map_get_key_ptr(map, pivot), key);

    if (cmp == 0)
      return (binary_search_result_t){.index = pivot, .cmp = 0};

    if (cmp < 0)
      right = pivot - 1;
    if (cmp > 0)
      left = pivot + 1;
  }

  return (binary_search_result_t){.index = left, .cmp = cmp};
}

map_value_t map_find(map_t *map, map_key_t key) {
  binary_search_result_t res = binary_search(map, key);

  if (res.cmp == 0)
    return map_get_value_ptr(map, res.index);

  return NULL;
}

void map_insert(map_t *map, map_key_t key, const map_value_t value) {
  memcpy(map->buffer, key, map->key_size);
  memcpy(map->buffer + map->key_size, value, map->value_size);

  binary_search_result_t res = binary_search(map, key);
  if (res.cmp == 0) {
    list_set(&map->items, res.index, map->buffer);
  } else {
    list_append(&map->items, res.index, map->buffer);
  }
}
