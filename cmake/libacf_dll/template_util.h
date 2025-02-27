// Copyright (c) 2023 Admenri. All rights reserved.

#ifndef ACF_TEMPLATE_UTIL_H_
#define ACF_TEMPLATE_UTIL_H_
#pragma once

#include <type_traits>

namespace template_util {

// Used to detect whether the given C struct has a size_t size field or has a
// base field and a base field has a size field.
template <typename T, typename = void>
struct HasValidSize {
  bool operator()(const T*) { return true; }
};
template <typename T>
struct HasValidSize<
    T,
    typename std::enable_if_t<std::is_same<decltype(T::size), size_t>::value>> {
  bool operator()(const T* s) { return s->size == sizeof(*s); }
};
template <typename T>
struct HasValidSize<T, decltype(void(T::base.size))> {
  bool operator()(const T* s) { return s->base.size == sizeof(*s); }
};

template <typename T>
inline bool has_valid_size(const T* s) {
  return HasValidSize<T>()(s);
}

}  // namespace template_util

#endif  // ACF_TEMPLATE_UTIL_H_
