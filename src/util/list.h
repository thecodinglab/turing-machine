/// @file list.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include <stddef.h>

/// A list is a dynamically allocated array of elements. A list may be resized
/// at any time.
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
///
/// @param capacity the initial capacity of the list.
/// @param size the number of bytes per element.
///
/// @return a new list with the given initial capacity.
list_t list_create(size_t capacity, size_t size);

/// Destroys the list by freeing any allocated memory.
///
/// @param list the list to destroy.
void list_destroy(list_t *list);

/// Resizes the list to the given capacity.
///
/// If the new capacity is smaller than the current capacity, the bounds of the
/// list will be adjusted if necessary.
///
/// @param list the list to resize.
/// @param capacity the new capacity of the list.
void list_resize(list_t *list, size_t capacity);

/// Gets the pointer of the element at the given index.
void *list_get(list_t *list, size_t idx);

/// Sets the value of the element at the given index to the data from the
/// provided data pointer.
///
/// Any element already present at the given index will be overwritten.
///
/// If the index is not within the element count of the list, this function will
/// fail. If you want to add elements to the list, use `list_add` instead.
///
/// The given data pointer must be list_t::size bytes long.
///
/// @param list the list where to set the value.
/// @param idx the index at which to set the value.
/// @param data the data which should be set.
void list_set(list_t *list, size_t idx, const void *data);

/// Adds the value of the provided data pointer to the list at the given index.
///
/// Any element already present at the given index will be overwritten.
///
/// If the index is not within the element count of the list, this function will
/// extend the list and set the value. Any memory between the last element count
/// and the new one will be set to zero.
///
/// The given data pointer must be list_t::size bytes long.
///
/// @param list the list where to add the value.
/// @param idx the index at which to add the value.
/// @param data the data which should be added.
void list_add(list_t *list, size_t idx, const void *data);

/// Adds the value of the provided data pointer to the list at the given index.
///
/// If an element is already present at the given location, the data will be
/// moved to make space for the given data.
///
/// If the index is not within the element count of the list, this function will
/// extend the list and set the value. Any memory between the last element count
/// and the new one will be set to zero.
///
/// The given data pointer must be list_t::size bytes long.
///
/// @param list the list where to add the value.
/// @param idx the index at which to add the value.
/// @param data the data which should be added.
void list_append(list_t *list, size_t idx, const void *data);
