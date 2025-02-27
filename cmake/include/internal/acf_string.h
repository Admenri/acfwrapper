// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_INTERNAL_ACF_STRING_H_
#define ACF_INCLUDE_INTERNAL_ACF_STRING_H_
#pragma once

#include "include/internal/acf_string_types.h"

#ifdef __cplusplus
#include "include/internal/acf_string_wrappers.h"
typedef AcfStringUTF8 AcfString;
#endif

// C style defination
typedef char acf_char_t;
typedef acf_string_utf8_t acf_string_t;
typedef acf_string_userfree_utf8_t acf_string_userfree_t;
#define acf_string_set acf_string_utf8_set
#define acf_string_copy acf_string_utf8_copy
#define acf_string_clear acf_string_utf8_clear
#define acf_string_userfree_alloc acf_string_userfree_utf8_alloc
#define acf_string_userfree_free acf_string_userfree_utf8_free
#define acf_string_from_ascii acf_string_utf8_copy
#define acf_string_to_utf8 acf_string_utf8_copy
#define acf_string_from_utf8 acf_string_utf8_copy
#define acf_string_to_utf16 acf_string_utf8_to_utf16
#define acf_string_from_utf16 acf_string_utf16_to_utf8
#define acf_string_to_wide acf_string_utf8_to_wide
#define acf_string_from_wide acf_string_wide_to_utf8

#endif  //! ACF_INCLUDE_INTERNAL_ACF_STRING_H_
