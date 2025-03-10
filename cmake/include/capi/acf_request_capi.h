// Copyright (c) 2025 Admenri. All rights reserved.
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
// $hash=11512d98372498f3ca791b014d700bbbc931e411$
//

#ifndef ACF_INCLUDE_CAPI_ACF_REQUEST_CAPI_H_
#define ACF_INCLUDE_CAPI_ACF_REQUEST_CAPI_H_
#pragma once

#include "include/capi/acf_base_capi.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_multimap.h"
#include "include/internal/acf_types_wrappers.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _acf_post_data_element_t;
struct _acf_post_data_t;
struct _acf_request_t;

///
/// Structure used to represent a web request. The functions of this structure
/// may be called on any thread.
///
typedef struct _acf_request_t {
  ///
  /// Base structure.
  ///
  acf_base_ref_counted_t base;

  ///
  /// Returns true (1) if this object is read-only.
  ///
  int(ACF_CALLBACK* is_read_only)(struct _acf_request_t* self);

  ///
  /// Get the fully qualified URL.
  ///
  // The resulting string must be freed by calling acf_string_userfree_free().
  acf_string_userfree_t(ACF_CALLBACK* get_url)(struct _acf_request_t* self);

  ///
  /// Set the fully qualified URL.
  ///
  void(ACF_CALLBACK* set_url)(struct _acf_request_t* self,
                              const acf_string_t* url);

  ///
  /// Get the request function type. The value will default to POST if post data
  /// is provided and GET otherwise.
  ///
  // The resulting string must be freed by calling acf_string_userfree_free().
  acf_string_userfree_t(ACF_CALLBACK* get_method)(struct _acf_request_t* self);

  ///
  /// Set the request function type.
  ///
  void(ACF_CALLBACK* set_method)(struct _acf_request_t* self,
                                 const acf_string_t* method);

  ///
  /// Set the referrer URL and policy. If non-NULL the referrer URL must be
  /// fully qualified with an HTTP or HTTPS scheme component. Any username,
  /// password or ref component will be removed.
  ///
  void(ACF_CALLBACK* set_referrer)(struct _acf_request_t* self,
                                   const acf_string_t* referrer_url,
                                   acf_referrer_policy_t policy);

  ///
  /// Get the referrer URL.
  ///
  // The resulting string must be freed by calling acf_string_userfree_free().
  acf_string_userfree_t(ACF_CALLBACK* get_referrer_url)(
      struct _acf_request_t* self);

  ///
  /// Get the referrer policy.
  ///
  acf_referrer_policy_t(ACF_CALLBACK* get_referrer_policy)(
      struct _acf_request_t* self);

  ///
  /// Get the post data.
  ///
  struct _acf_post_data_t*(ACF_CALLBACK* get_post_data)(
      struct _acf_request_t* self);

  ///
  /// Set the post data.
  ///
  void(ACF_CALLBACK* set_post_data)(struct _acf_request_t* self,
                                    struct _acf_post_data_t* postData);

  ///
  /// Get the header values. Will not include the Referer value if any.
  ///
  void(ACF_CALLBACK* get_header_map)(struct _acf_request_t* self,
                                     acf_string_multimap_t headerMap);

  ///
  /// Set the header values. If a Referer value exists in the header map it will
  /// be removed and ignored.
  ///
  void(ACF_CALLBACK* set_header_map)(struct _acf_request_t* self,
                                     acf_string_multimap_t headerMap);

  ///
  /// Returns the first header value for |name| or an NULL string if not found.
  /// Will not return the Referer value if any. Use GetHeaderMap instead if
  /// |name| might have multiple values.
  ///
  // The resulting string must be freed by calling acf_string_userfree_free().
  acf_string_userfree_t(ACF_CALLBACK* get_header_by_name)(
      struct _acf_request_t* self,
      const acf_string_t* name);

  ///
  /// Set the header |name| to |value|. If |overwrite| is true (1) any existing
  /// values will be replaced with the new value. If |overwrite| is false (0)
  /// any existing values will not be overwritten. The Referer value cannot be
  /// set using this function.
  ///
  void(ACF_CALLBACK* set_header_by_name)(struct _acf_request_t* self,
                                         const acf_string_t* name,
                                         const acf_string_t* value,
                                         int overwrite);

  ///
  /// Set all values at one time.
  ///
  void(ACF_CALLBACK* set)(struct _acf_request_t* self,
                          const acf_string_t* url,
                          const acf_string_t* method,
                          struct _acf_post_data_t* postData,
                          acf_string_multimap_t headerMap);

  ///
  /// Get the resource type for this request. Only available in the browser
  /// process.
  ///
  acf_resource_type_t(ACF_CALLBACK* get_resource_type)(
      struct _acf_request_t* self);

  ///
  /// Get the transition type for this request. Only available in the browser
  /// process and only applies to requests that represent a main frame or sub-
  /// frame navigation.
  ///
  acf_transition_type_t(ACF_CALLBACK* get_transition_type)(
      struct _acf_request_t* self);

  ///
  /// Returns the globally unique identifier for this request or 0 if not
  /// specified. Can be used by CefResourceRequestHandler implementations in the
  /// browser process to track a single request across multiple callbacks.
  ///
  uint64(ACF_CALLBACK* get_identifier)(struct _acf_request_t* self);
} acf_request_t;

///
/// Create a new CefRequest object.
///
ACF_EXPORT acf_request_t* acf_request_create(void);

///
/// Structure used to represent post data for a web request. The functions of
/// this structure may be called on any thread.
///
typedef struct _acf_post_data_t {
  ///
  /// Base structure.
  ///
  acf_base_ref_counted_t base;

  ///
  /// Returns true (1) if this object is read-only.
  ///
  int(ACF_CALLBACK* is_read_only)(struct _acf_post_data_t* self);

  ///
  /// Returns true (1) if the underlying POST data includes elements that are
  /// not represented by this CefPostData object (for example, multi-part file
  /// upload data). Modifying CefPostData objects with excluded elements may
  /// result in the request failing.
  ///
  int(ACF_CALLBACK* has_excluded_elements)(struct _acf_post_data_t* self);

  ///
  /// Returns the number of existing post data elements.
  ///
  size_t(ACF_CALLBACK* get_element_count)(struct _acf_post_data_t* self);

  ///
  /// Retrieve the post data elements.
  ///
  void(ACF_CALLBACK* get_elements)(struct _acf_post_data_t* self,
                                   size_t* elementsCount,
                                   struct _acf_post_data_element_t** elements);

  ///
  /// Remove the specified post data element.  Returns true (1) if the removal
  /// succeeds.
  ///
  int(ACF_CALLBACK* remove_element)(struct _acf_post_data_t* self,
                                    struct _acf_post_data_element_t* element);

  ///
  /// Add the specified post data element.  Returns true (1) if the add
  /// succeeds.
  ///
  int(ACF_CALLBACK* add_element)(struct _acf_post_data_t* self,
                                 struct _acf_post_data_element_t* element);

  ///
  /// Remove all existing post data elements.
  ///
  void(ACF_CALLBACK* remove_elements)(struct _acf_post_data_t* self);
} acf_post_data_t;

///
/// Create a new CefPostData object.
///
ACF_EXPORT acf_post_data_t* acf_post_data_create(void);

///
/// Structure used to represent a single element in the request post data. The
/// functions of this structure may be called on any thread.
///
typedef struct _acf_post_data_element_t {
  ///
  /// Base structure.
  ///
  acf_base_ref_counted_t base;

  ///
  /// Returns true (1) if this object is read-only.
  ///
  int(ACF_CALLBACK* is_read_only)(struct _acf_post_data_element_t* self);

  ///
  /// Remove all contents from the post data element.
  ///
  void(ACF_CALLBACK* set_to_empty)(struct _acf_post_data_element_t* self);

  ///
  /// The post data element will represent a file.
  ///
  void(ACF_CALLBACK* set_to_file)(struct _acf_post_data_element_t* self,
                                  const acf_string_t* fileName);

  ///
  /// The post data element will represent bytes.  The bytes passed in will be
  /// copied.
  ///
  void(ACF_CALLBACK* set_to_bytes)(struct _acf_post_data_element_t* self,
                                   size_t size,
                                   const void* bytes);

  ///
  /// Return the type of this post data element.
  ///
  acf_postdataelement_type_t(ACF_CALLBACK* get_type)(
      struct _acf_post_data_element_t* self);

  ///
  /// Return the file name.
  ///
  // The resulting string must be freed by calling acf_string_userfree_free().
  acf_string_userfree_t(ACF_CALLBACK* get_file)(
      struct _acf_post_data_element_t* self);

  ///
  /// Return the number of bytes.
  ///
  size_t(ACF_CALLBACK* get_bytes_count)(struct _acf_post_data_element_t* self);

  ///
  /// Read up to |size| bytes into |bytes| and return the number of bytes
  /// actually read.
  ///
  size_t(ACF_CALLBACK* get_bytes)(struct _acf_post_data_element_t* self,
                                  size_t size,
                                  void* bytes);
} acf_post_data_element_t;

///
/// Create a new acf_post_data_element_t object.
///
ACF_EXPORT acf_post_data_element_t* acf_post_data_element_create(void);

#ifdef __cplusplus
}
#endif

#endif  // ACF_INCLUDE_CAPI_ACF_REQUEST_CAPI_H_
