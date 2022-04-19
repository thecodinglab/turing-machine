/// @file map.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include "list.h"

#include <stddef.h>

typedef void *map_key_t;
typedef void *map_value_t;

/// A compare function to compare two keys. The function should return `0` for
/// equal keys, a negative value when a < b and a positive value when a > b.
typedef int (*map_cmp_func_t)(map_key_t a, map_key_t b);

/// A map is a list of sorted key value pairs where each key can be associated
/// with a single value.
///
/// In this implementation each key and each value must always have the same
/// size as defined in map_t::key_size and map_t::value_size.
typedef struct {
  /// The ordered list of key value pairs.
  ///
  /// Each element in the list has a size of map_t::key_size + map_t::value_size
  /// and stores the pair aligned one after another. Querying for a specific
  /// value at a particular index requires the addition of map_t::key_size to
  /// the returned pointer to receive the value address.
  list_t items;
  /// The size in bytes of a key.
  size_t key_size;
  /// The size in bytes of a value.
  size_t value_size;
  /// A temporary buffer to align key and value during insertion.
  void *buffer;
  /// A function to compare two keys.
  map_cmp_func_t cmp;
} map_t;

/// Constructs an empty map using the given parameters.
///
/// @param key_size the size in bytes for each key.
/// @param value_size the size in bytes for each value.
/// @param cmp a function to compare two keys.
///
/// @return an empty map.
map_t map_create(size_t key_size, size_t value_size, map_cmp_func_t cmp);

/// Destroys the given map by releasing any allocated memory.
///
/// @param map the map to destroy.
void map_destroy(map_t *map);

/// Tries to find a value for the given key. If no value is associated with that
/// key, `NULL` will be returned.
///
/// @param map the map to query.
/// @param key the key to search for.
///
/// @return the pointer to the value or `NULL` if no association exists.
map_value_t map_find(map_t *map, map_key_t key);

/// Associates the key to the given value.
///
/// If the key is already associated with another value, the old value will be
/// overwritten with the given value parameter.
///
/// @param map the map to insert the pair into.
/// @param key the key to insert.
/// @param value the value to insert.
void map_insert(map_t *map, map_key_t key, const map_value_t value);
