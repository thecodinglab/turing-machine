#include "list.h"
#include "debug.h"

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
  int remainder = capacity % 1024;
  if (remainder != 0)
    capacity = capacity - remainder + 1024;

  LOG("list_resize(%p, %lu);\n", list, capacity);

  void *ptr = realloc(list->ptr, capacity * list->size);
  assert(ptr != NULL);

  list->ptr = ptr;
  list->capacity = capacity;

  if (list->count > list->capacity)
    list->count = list->capacity;
}

static inline void *list_ptr(list_t *list, size_t idx) {
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

/// Ensures that the list has has enough space for the given amount of elements.
///
/// If the requirement is not fulfilled, the list will be resized.
///
/// @param list the list to ensure the capacity.
/// @param req the required minimum capacity.
void list_ensure(list_t *list, size_t req) {
  if (req > list->capacity) {
    // item is out of capacity: resize list.
    list_resize(list, req);
  }

  if (req > list->count) {
    // item is out of bounds: clear memory inbetween and increase bounds.

    // clear memory between bounds.
    size_t added = req - list->count;
    memset(list_ptr(list, list->count), 0, added * list->size);

    // increase list bounds.
    list->count = req;
  }
}

void list_add(list_t *list, size_t idx, const void *data) {
  list_ensure(list, idx + 1);
  list_set(list, idx, data);
}

/// Shifts all elements starting from the given offset to the right.
void list_shift(list_t *list, size_t offset) {
  assert(offset < list->count);

  for (size_t i = list->count - 1; i > offset; i--) {
    list_set(list, i, list_get(list, i - 1));
  }
}

void list_append(list_t *list, size_t idx, const void *data) {
  size_t req = idx + 1;
  if (req < list->count + 1)
    req = list->count + 1;

  list_ensure(list, req);

  list_shift(list, idx);
  list_set(list, idx, data);
}
