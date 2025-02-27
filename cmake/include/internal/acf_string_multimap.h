// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_STRING_MULTIMAP_H_
#define ACF_STRING_MULTIMAP_H_
#pragma once

#include "include/internal/acf_defines.h"
#include "include/internal/acf_string.h"

#ifdef __cplusplus
extern "C" {
#endif

///
/// string multimaps are a set of key/value string pairs.
/// More than one value can be assigned to a single key.
///
typedef void* acf_string_multimap_t;

///
/// Allocate a new string multimap.
///
ACF_EXPORT acf_string_multimap_t acf_string_multimap_alloc(void);

///
/// Return the number of elements in the string multimap.
///
ACF_EXPORT size_t acf_string_multimap_size(acf_string_multimap_t map);

///
/// Return the number of values with the specified key.
///
ACF_EXPORT size_t acf_string_multimap_find_count(acf_string_multimap_t map,
                                                 const acf_string_t* key);

///
/// Return the value_index-th value with the specified key.
///
ACF_EXPORT int acf_string_multimap_enumerate(acf_string_multimap_t map,
                                             const acf_string_t* key,
                                             size_t value_index,
                                             acf_string_t* value);

///
/// Return the key at the specified zero-based string multimap index.
///
ACF_EXPORT int acf_string_multimap_key(acf_string_multimap_t map,
                                       size_t index,
                                       acf_string_t* key);

///
/// Return the value at the specified zero-based string multimap index.
///
ACF_EXPORT int acf_string_multimap_value(acf_string_multimap_t map,
                                         size_t index,
                                         acf_string_t* value);

///
/// Append a new key/value pair at the end of the string multimap.
///
ACF_EXPORT int acf_string_multimap_append(acf_string_multimap_t map,
                                          const acf_string_t* key,
                                          const acf_string_t* value);

///
/// Clear the string multimap.
///
ACF_EXPORT void acf_string_multimap_clear(acf_string_multimap_t map);

///
/// Free the string multimap.
///
ACF_EXPORT void acf_string_multimap_free(acf_string_multimap_t map);

#ifdef __cplusplus
}
#endif

#endif  // ACF_STRING_MULTIMAP_H_
