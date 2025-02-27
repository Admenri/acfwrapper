// Copyright (c) 2014 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_CAPI_ACF_BASE_CAPI_H_
#define ACF_INCLUDE_CAPI_ACF_BASE_CAPI_H_

#include <stdint.h>

#include "include/internal/acf_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

///
// All ref-counted framework structures must include this structure first.
///
typedef struct _acf_base_ref_counted_t {
  ///
  // Size of the data structure.
  ///
  size_t size;

  ///
  // Called to increment the reference count for the object. Should be called
  // for every new copy of a pointer to a given object.
  ///
  void(ACF_CALLBACK* add_ref)(struct _acf_base_ref_counted_t* self);

  ///
  // Called to decrement the reference count for the object. If the reference
  // count falls to 0 the object should self-delete. Returns true (1) if the
  // resulting reference count is 0.
  ///
  int(ACF_CALLBACK* release)(struct _acf_base_ref_counted_t* self);

  ///
  // Returns true (1) if the current reference count is 1.
  ///
  int(ACF_CALLBACK* has_one_ref)(struct _acf_base_ref_counted_t* self);

  ///
  // Returns true (1) if the current reference count is at least 1.
  ///
  int(ACF_CALLBACK* has_at_least_one_ref)(struct _acf_base_ref_counted_t* self);
} acf_base_ref_counted_t;

///
// All scoped framework structures must include this structure first.
///
typedef struct _acf_base_scoped_t {
  ///
  // Size of the data structure.
  ///
  size_t size;

  ///
  // Called to delete this object. May be NULL if the object is not owned.
  ///
  void(ACF_CALLBACK* del)(struct _acf_base_scoped_t* self);

} acf_base_scoped_t;

// Check that the structure |s|, which is defined with a size_t member at the
// top, is large enough to contain the specified member |f|.
#define ACF_MEMBER_EXISTS(s, f) \
  ((intptr_t) &                 \
   ((s)->f) - (intptr_t)(s) + sizeof((s)->f) <= *reinterpret_cast<size_t*>(s))

#define ACF_MEMBER_MISSING(s, f) (!ACF_MEMBER_EXISTS(s, f) || !((s)->f))

#ifdef __cplusplus
}
#endif

#endif  //! ACF_INCLUDE_CAPI_ACF_BASE_CAPI_H_
