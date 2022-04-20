/// @file hash_map.h
/// @author Florian Walter (walteflo@students.zhaw.ch)

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef void *hash_table_key_t;
typedef void *hash_table_value_t;

/// A function to hash the given data.
typedef uint32_t (*hash_func_t)(const void *data, size_t size);

/// A function to compare two keys. The function should return `0` for equal
/// keys, a negative value when a < b and a positive value when a > b.
typedef int (*hash_table_cmp_func_t)(const hash_table_key_t a,
                                     const hash_table_key_t b);

/// A hash table is a list of sorted key value pairs where each key can be
/// associated with a single value.
///
/// The usage of a hash function allows it to lookup values in constant time on
/// average (O(1)).
///
/// In this implementation each key and each value must always have the same
/// size as defined in hash_table_t::key_size and hash_table_t::value_size.
typedef struct {
  /// Pointer to the first entry.
  void *entries;
  /// The total number of allocated entries in hash_table_t::entries.
  size_t capacity;
  /// The number of entries.
  size_t count;

  /// The size in bytes of a key.
  size_t key_size;
  /// The size in bytes of a value.
  size_t value_size;
  /// The size in bytes of a single entry. An entry consists of flags, key and
  /// value.
  size_t entry_size;

  /// A function to hash the table key.
  hash_func_t hash;
  /// A function to compare two table keys.
  hash_table_cmp_func_t cmp;
} hash_table_t;

/// Constructs an empty hash table using the given parameters.
///
/// @param key_size the size in bytes for each key.
/// @param value_size the size in bytes for each value.
/// @param cmp a function to compare two keys.
///
/// @return an empty hash table.
hash_table_t hash_table_create(size_t key_size, size_t value_size,
                               hash_table_cmp_func_t cmp);

/// Destroys the given hash table by releasing any allocated memory.
///
/// @param table the hash table to destroy.
void hash_table_destroy(hash_table_t *table);

/// Tries to find a value for the given key. If no value is associated with that
/// key, `NULL` will be returned.
///
/// @param table the hash table to query.
/// @param key the key to search for.
///
/// @return the pointer to the value or `NULL` if no association exists.
void *hash_table_find(hash_table_t *table, hash_table_key_t key);

/// Associates the key to the given value.
///
/// If the key is already associated with another value, the old value will be
/// overwritten with the given value parameter.
///
/// @param table the table to insert the pair into.
/// @param key the key to insert.
/// @param value the value to insert.
void hash_table_insert(hash_table_t *table, hash_table_key_t key,
                       hash_table_value_t value);

/// An iterator used to list the elements inside a hash table.
typedef struct {
  /// Current entry key.
  hash_table_key_t key;
  /// Current entry value.
  hash_table_value_t value;

  /// The table to iterate over.
  hash_table_t *table;
  /// Index of the next entry in the table.
  size_t index;
} hash_table_iter_t;

/// Creates an iterator over the given hash table.
///
/// @param table the table over which to iterate.
///
/// @return an iterator over the given table.
hash_table_iter_t hash_table_iter(hash_table_t *table);

/// Increments the given iterator to the next entry.
///
/// @param iter the iterator to increment.
///
/// @return whether there are still more entries available.
int hash_table_iter_next(hash_table_iter_t *iter);
