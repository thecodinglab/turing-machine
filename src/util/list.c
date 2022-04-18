#include "list.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

list_t list_create(size_t capacity, size_t size) {
  assert(size != 0);

  void *ptr = NULL;
  if (capacity > 0)
    ptr = malloc(capacity * size);

  return (list_t){
      .ptr = ptr,
      .count = 0,
      .capacity = capacity,
      .size = size,
  };
}

void list_destroy(list_t *list) {
  if (list->ptr)
    free(list->ptr);
}

void list_resize(list_t *list, size_t capacity) {
  void *ptr = realloc(list->ptr, capacity * list->size);
  assert(ptr != NULL);

  list->ptr = ptr;
  list->capacity = capacity;

  if (list->count > list->capacity)
    list->count = list->capacity;
}

void *list_ptr(list_t *list, size_t idx) {
  assert(idx < list->capacity);
  return list->ptr + (idx * list->size);
}

void *list_get(list_t *list, size_t idx) {
  assert(idx < list->count);
  return list_ptr(list, idx);
}

void list_set(list_t *list, size_t idx, const void *data) {
  assert(idx < list->count);
  memmove(list_ptr(list, idx), data, list->size);
}

void list_add(list_t *list, size_t idx, const void *data) {
  if (idx >= list->capacity) {
    // item is out of capacity: resize list.
    list_resize(list, idx + 1); // TODO min block size
  }

  if (idx >= list->count) {
    // item is out of bounds: clear memory inbetween and increase bounds.

    // clear memory between bounds.
    size_t added = (idx + 1) - list->count;
    memset(list_ptr(list, list->count), 0, added * list->size);

    // increase list bounds.
    list->count = idx + 1;
  }

  // item is within bounds: set item to given value.
  list_set(list, idx, data);
}
