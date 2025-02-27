// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_INTERNAL_ACF_STRING_LIST_H_
#define ACF_INCLUDE_INTERNAL_ACF_STRING_LIST_H_
#pragma once

#include "include/internal/acf_defines.h"
#include "include/internal/acf_string.h"

#ifdef __cplusplus
extern "C" {
#endif

///
/// string maps are a set of key/value string pairs.
///
typedef void* acf_string_list_t;

///
/// Allocate a new string map.
///
ACF_EXPORT acf_string_list_t acf_string_list_alloc(void);

///
/// Return the number of elements in the string list.
///
ACF_EXPORT size_t acf_string_list_size(acf_string_list_t list);

///
/// Retrieve the value at the specified zero-based string list index. Returns
/// true (1) if the value was successfully retrieved.
///
ACF_EXPORT int acf_string_list_value(acf_string_list_t list,
                                     size_t index,
                                     acf_string_t* value);

///
/// Append a new value at the end of the string list.
///
ACF_EXPORT void acf_string_list_append(acf_string_list_t list,
                                       const acf_string_t* value);

///
/// Clear the string list.
///
ACF_EXPORT void acf_string_list_clear(acf_string_list_t list);

///
/// Free the string list.
///
ACF_EXPORT void acf_string_list_free(acf_string_list_t list);

///
/// Creates a copy of an existing string list.
///
ACF_EXPORT acf_string_list_t acf_string_list_copy(acf_string_list_t list);

#ifdef __cplusplus
}
#endif

#endif  //! ACF_INCLUDE_INTERNAL_ACF_STRING_LIST_H_
