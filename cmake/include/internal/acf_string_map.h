// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_INTERNAL_ACF_STRING_MAP_H_
#define ACF_INCLUDE_INTERNAL_ACF_STRING_MAP_H_
#pragma once

#include "include/internal/acf_defines.h"
#include "include/internal/acf_string.h"

#ifdef __cplusplus
extern "C" {
#endif

///
/// string maps are a set of key/value string pairs.
///
typedef void* acf_string_map_t;

///
/// Allocate a new string map.
///
ACF_EXPORT acf_string_map_t acf_string_map_alloc(void);

///
/// Return the number of elements in the string map.
///
ACF_EXPORT size_t acf_string_map_size(acf_string_map_t map);

///
/// Return the value assigned to the specified key.
///
ACF_EXPORT int acf_string_map_find(acf_string_map_t map,
                                   const acf_string_t* key,
                                   acf_string_t* value);

///
/// Return the key at the specified zero-based string map index.
///
ACF_EXPORT int acf_string_map_key(acf_string_map_t map,
                                  size_t index,
                                  acf_string_t* key);

///
/// Return the value at the specified zero-based string map index.
///
ACF_EXPORT int acf_string_map_value(acf_string_map_t map,
                                    size_t index,
                                    acf_string_t* value);

///
/// Append a new key/value pair at the end of the string map.
///
ACF_EXPORT int acf_string_map_append(acf_string_map_t map,
                                     const acf_string_t* key,
                                     const acf_string_t* value);

///
/// Clear the string map.
///
ACF_EXPORT void acf_string_map_clear(acf_string_map_t map);

///
/// Free the string map.
///
ACF_EXPORT void acf_string_map_free(acf_string_map_t map);

#ifdef __cplusplus
}
#endif

#endif  //! ACF_INCLUDE_INTERNAL_ACF_STRING_MAP_H_
