#pragma once

#include <stddef.h>

typedef struct {
  /// Pointer to the first element.
  void *ptr;
  /// Number of elements currently present.
  size_t count;
  /// Number of total elements allocated.
  size_t capacity;
  /// Number of bytes per element.
  size_t size;
} list_t;

/// Constructs a new list with the given element size and given initial
/// capacity.
list_t list_create(size_t capacity, size_t size);

/// Destroys the list by freeing any allocated memory.
void list_destroy(list_t *list);

/// Resizes the list to the given capacity.
void list_resize(list_t *list, size_t capacity);

/// Gets the pointer of the element at the given index.
void *list_get(list_t *list, size_t idx);

// Sets the value of the element at the given index to the data from the
// provided data pointer.
//
// If the index is not within the element count of the list, this function will
// fail. If you want to add elements to the list, use `list_add` instead.
void list_set(list_t *list, size_t idx, const void *data);

// Adds the value of the provided data pointer to the list at the given index.
//
// If the index is not within the element count of the list, this function will
// extend the list and set the value. Any memory between the last element count
// and the new one will be set to zero.
void list_add(list_t *list, size_t idx, const void *data);
