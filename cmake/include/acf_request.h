// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_REQUEST_H_
#define ACF_INCLUDE_ACF_REQUEST_H_

#include <map>
#include <vector>

#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_multimap.h"
#include "include/internal/acf_types_wrappers.h"

class AcfRequest;
class AcfPostData;
class AcfPostDataElement;

///
/// Class used to represent a web request. The methods of this class may be
/// called on any thread.
///
/*--acf(source=library)--*/
class AcfRequest : public virtual AcfBaseRefCounted {
 public:
  typedef std::multimap<AcfString, AcfString> HeaderMap;
  typedef acf_referrer_policy_t ReferrerPolicy;
  typedef acf_resource_type_t ResourceType;
  typedef acf_transition_type_t TransitionType;

  ///
  /// Create a new CefRequest object.
  ///
  /*--acf()--*/
  static AcfRefPtr<AcfRequest> Create();

  ///
  /// Returns true if this object is read-only.
  ///
  /*--acf()--*/
  virtual bool IsReadOnly() = 0;

  ///
  /// Get the fully qualified URL.
  ///
  /*--acf()--*/
  virtual AcfString GetURL() = 0;

  ///
  /// Set the fully qualified URL.
  ///
  /*--acf()--*/
  virtual void SetURL(const AcfString& url) = 0;

  ///
  /// Get the request method type. The value will default to POST if post data
  /// is provided and GET otherwise.
  ///
  /*--acf()--*/
  virtual AcfString GetMethod() = 0;

  ///
  /// Set the request method type.
  ///
  /*--acf()--*/
  virtual void SetMethod(const AcfString& method) = 0;

  ///
  /// Set the referrer URL and policy. If non-empty the referrer URL must be
  /// fully qualified with an HTTP or HTTPS scheme component. Any username,
  /// password or ref component will be removed.
  ///
  /*--acf(optional_param=referrer_url)--*/
  virtual void SetReferrer(const AcfString& referrer_url,
                           ReferrerPolicy policy) = 0;

  ///
  /// Get the referrer URL.
  ///
  /*--acf()--*/
  virtual AcfString GetReferrerURL() = 0;

  ///
  /// Get the referrer policy.
  ///
  /*--acf(default_retval=REFERRER_POLICY_DEFAULT)--*/
  virtual ReferrerPolicy GetReferrerPolicy() = 0;

  ///
  /// Get the post data.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfPostData> GetPostData() = 0;

  ///
  /// Set the post data.
  ///
  /*--acf()--*/
  virtual void SetPostData(AcfRefPtr<AcfPostData> postData) = 0;

  ///
  /// Get the header values. Will not include the Referer value if any.
  ///
  /*--acf()--*/
  virtual void GetHeaderMap(HeaderMap& headerMap) = 0;

  ///
  /// Set the header values. If a Referer value exists in the header map it will
  /// be removed and ignored.
  ///
  /*--acf()--*/
  virtual void SetHeaderMap(const HeaderMap& headerMap) = 0;

  ///
  /// Returns the first header value for |name| or an empty string if not found.
  /// Will not return the Referer value if any. Use GetHeaderMap instead if
  /// |name| might have multiple values.
  ///
  /*--acf()--*/
  virtual AcfString GetHeaderByName(const AcfString& name) = 0;

  ///
  /// Set the header |name| to |value|. If |overwrite| is true any existing
  /// values will be replaced with the new value. If |overwrite| is false any
  /// existing values will not be overwritten. The Referer value cannot be set
  /// using this method.
  ///
  /*--acf(optional_param=value)--*/
  virtual void SetHeaderByName(const AcfString& name,
                               const AcfString& value,
                               bool overwrite) = 0;

  ///
  /// Set all values at one time.
  ///
  /*--acf(optional_param=postData)--*/
  virtual void Set(const AcfString& url,
                   const AcfString& method,
                   AcfRefPtr<AcfPostData> postData,
                   const HeaderMap& headerMap) = 0;

  ///
  /// Get the resource type for this request. Only available in the browser
  /// process.
  ///
  /*--acf(default_retval=RT_SUB_RESOURCE)--*/
  virtual ResourceType GetResourceType() = 0;

  ///
  /// Get the transition type for this request. Only available in the browser
  /// process and only applies to requests that represent a main frame or
  /// sub-frame navigation.
  ///
  /*--acf(default_retval=PAGE_TRANSITION_LINK)--*/
  virtual TransitionType GetTransitionType() = 0;

  ///
  /// Returns the globally unique identifier for this request or 0 if not
  /// specified. Can be used by CefResourceRequestHandler implementations in the
  /// browser process to track a single request across multiple callbacks.
  ///
  /*--acf()--*/
  virtual uint64 GetIdentifier() = 0;
};

///
/// Class used to represent post data for a web request. The methods of this
/// class may be called on any thread.
///
/*--acf(source=library,no_debugct_check)--*/
class AcfPostData : public virtual AcfBaseRefCounted {
 public:
  typedef std::vector<AcfRefPtr<AcfPostDataElement>> ElementVector;

  ///
  /// Create a new CefPostData object.
  ///
  /*--acf()--*/
  static AcfRefPtr<AcfPostData> Create();

  ///
  /// Returns true if this object is read-only.
  ///
  /*--acf()--*/
  virtual bool IsReadOnly() = 0;

  ///
  /// Returns true if the underlying POST data includes elements that are not
  /// represented by this CefPostData object (for example, multi-part file
  /// upload data). Modifying CefPostData objects with excluded elements may
  /// result in the request failing.
  ///
  /*--acf()--*/
  virtual bool HasExcludedElements() = 0;

  ///
  /// Returns the number of existing post data elements.
  ///
  /*--acf()--*/
  virtual size_t GetElementCount() = 0;

  ///
  /// Retrieve the post data elements.
  ///
  /*--acf(count_func=elements:GetElementCount)--*/
  virtual void GetElements(ElementVector& elements) = 0;

  ///
  /// Remove the specified post data element.  Returns true if the removal
  /// succeeds.
  ///
  /*--acf()--*/
  virtual bool RemoveElement(AcfRefPtr<AcfPostDataElement> element) = 0;

  ///
  /// Add the specified post data element.  Returns true if the add succeeds.
  ///
  /*--acf()--*/
  virtual bool AddElement(AcfRefPtr<AcfPostDataElement> element) = 0;

  ///
  /// Remove all existing post data elements.
  ///
  /*--acf()--*/
  virtual void RemoveElements() = 0;
};

///
/// Class used to represent a single element in the request post data. The
/// methods of this class may be called on any thread.
///
/*--acf(source=library,no_debugct_check)--*/
class AcfPostDataElement : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Create a new AcfPostDataElement object.
  ///
  /*--acf()--*/
  static AcfRefPtr<AcfPostDataElement> Create();

  ///
  /// Post data elements may represent either bytes or files.
  ///
  typedef acf_postdataelement_type_t Type;

  ///
  /// Returns true if this object is read-only.
  ///
  /*--acf()--*/
  virtual bool IsReadOnly() = 0;

  ///
  /// Remove all contents from the post data element.
  ///
  /*--acf()--*/
  virtual void SetToEmpty() = 0;

  ///
  /// The post data element will represent a file.
  ///
  /*--acf()--*/
  virtual void SetToFile(const AcfString& fileName) = 0;

  ///
  /// The post data element will represent bytes.  The bytes passed
  /// in will be copied.
  ///
  /*--acf()--*/
  virtual void SetToBytes(size_t size, const void* bytes) = 0;

  ///
  /// Return the type of this post data element.
  ///
  /*--acf(default_retval=PDE_TYPE_EMPTY)--*/
  virtual Type GetType() = 0;

  ///
  /// Return the file name.
  ///
  /*--acf()--*/
  virtual AcfString GetFile() = 0;

  ///
  /// Return the number of bytes.
  ///
  /*--acf()--*/
  virtual size_t GetBytesCount() = 0;

  ///
  /// Read up to |size| bytes into |bytes| and return the number of bytes
  /// actually read.
  ///
  /*--acf()--*/
  virtual size_t GetBytes(size_t size, void* bytes) = 0;
};

#endif  // ACF_INCLUDE_REQUEST_H_
