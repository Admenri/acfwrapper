// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_INTERNAL_ACF_STRING_TYPES_H_
#define ACF_INCLUDE_INTERNAL_ACF_STRING_TYPES_H_
#pragma once

#include <stddef.h>

#include "include/internal/acf_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _acf_string_utf8_t {
  char* str;
  size_t length;
  void (*dtor)(char* str);
} acf_string_utf8_t;

typedef struct _acf_string_utf16_t {
  char16* str;
  size_t length;
  void (*dtor)(char16* str);
} acf_string_utf16_t;

ACF_EXPORT int acf_string_utf8_set(const char* src,
                                   size_t src_len,
                                   acf_string_utf8_t* output,
                                   int copy);
ACF_EXPORT int acf_string_utf16_set(const char16* src,
                                    size_t src_len,
                                    acf_string_utf16_t* output,
                                    int copy);

#define acf_string_utf8_copy(src, src_len, output) \
  acf_string_utf8_set(src, src_len, output, true)
#define acf_string_utf16_copy(src, src_len, output) \
  acf_string_utf16_set(src, src_len, output, true)

ACF_EXPORT void acf_string_utf8_clear(acf_string_utf8_t* str);
ACF_EXPORT void acf_string_utf16_clear(acf_string_utf16_t* str);

ACF_EXPORT int acf_string_utf8_cmp(const acf_string_utf8_t* str1,
                                   const acf_string_utf8_t* str2);
ACF_EXPORT int acf_string_utf16_cmp(const acf_string_utf16_t* str1,
                                    const acf_string_utf16_t* str2);

ACF_EXPORT int acf_string_utf8_to_utf16(const char* src,
                                        size_t src_len,
                                        acf_string_utf16_t* output);
ACF_EXPORT int acf_string_utf16_to_utf8(const char16* src,
                                        size_t src_len,
                                        acf_string_utf8_t* output);

typedef acf_string_utf8_t* acf_string_userfree_utf8_t;
typedef acf_string_utf16_t* acf_string_userfree_utf16_t;

ACF_EXPORT acf_string_userfree_utf8_t acf_string_userfree_utf8_alloc(void);
ACF_EXPORT acf_string_userfree_utf16_t acf_string_userfree_utf16_alloc(void);

ACF_EXPORT void acf_string_userfree_utf8_free(acf_string_userfree_utf8_t str);
ACF_EXPORT void acf_string_userfree_utf16_free(acf_string_userfree_utf16_t str);

#ifdef __cplusplus
}
#endif

#endif  //! ACF_INCLUDE_INTERNAL_ACF_STRING_TYPES_H_
