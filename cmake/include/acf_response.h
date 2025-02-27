// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_RESPONSE_H_
#define ACF_INCLUDE_ACF_RESPONSE_H_

#include <map>

#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_multimap.h"
#include "include/internal/acf_types_wrappers.h"

///
/// Class used to represent a web response. The methods of this class may be
/// called on any thread.
///
/*--acf(source=library)--*/
class AcfResponse : public virtual AcfBaseRefCounted {
 public:
  typedef std::multimap<AcfString, AcfString> HeaderMap;

  ///
  /// Create a new AcfResponse object.
  ///
  /*--acf()--*/
  static AcfRefPtr<AcfResponse> Create();

  ///
  /// Returns true if this object is read-only.
  ///
  /*--acf()--*/
  virtual bool IsReadOnly() = 0;

  ///
  /// Set response data from other
  ///
  /*--acf(optional_param=other)--*/
  virtual void Set(AcfRefPtr<AcfResponse> other) = 0;

  ///
  /// Get the response error code. Returns ERR_NONE if there was no error.
  ///
  /*--acf()--*/
  virtual int GetError() = 0;

  ///
  /// Set the response error code. This can be used by custom scheme handlers
  /// to return errors during initial request processing.
  ///
  /*--acf()--*/
  virtual void SetError(int error) = 0;

  ///
  /// Get the response status code.
  ///
  /*--acf()--*/
  virtual int GetStatus() = 0;

  ///
  /// Set the response status code.
  ///
  /*--acf()--*/
  virtual void SetStatus(int status) = 0;

  ///
  /// Get the response status text.
  ///
  /*--acf()--*/
  virtual AcfString GetStatusText() = 0;

  ///
  /// Set the response status text.
  ///
  /*--acf(optional_param=statusText)--*/
  virtual void SetStatusText(const AcfString& statusText) = 0;

  ///
  /// Get the response mime type.
  ///
  /*--acf()--*/
  virtual AcfString GetMimeType() = 0;

  ///
  /// Set the response mime type.
  ///
  /*--acf(optional_param=mimeType)--*/
  virtual void SetMimeType(const AcfString& mimeType) = 0;

  ///
  /// Get the response charset.
  ///
  /*--acf()--*/
  virtual AcfString GetCharset() = 0;

  ///
  /// Set the response charset.
  ///
  /*--acf(optional_param=charset)--*/
  virtual void SetCharset(const AcfString& charset) = 0;

  ///
  /// Get the value for the specified response header field.
  ///
  /*--acf()--*/
  virtual AcfString GetHeaderByName(const AcfString& name) = 0;

  ///
  /// Set the header |name| to |value|. If |overwrite| is true any existing
  /// values will be replaced with the new value. If |overwrite| is false any
  /// existing values will not be overwritten.
  ///
  /*--acf(optional_param=value)--*/
  virtual void SetHeaderByName(const AcfString& name,
                               const AcfString& value,
                               bool overwrite) = 0;

  ///
  /// Get all response header fields.
  ///
  /*--acf()--*/
  virtual void GetHeaderMap(HeaderMap& headerMap) = 0;

  ///
  /// Set all response header fields.
  ///
  /*--acf()--*/
  virtual void SetHeaderMap(const HeaderMap& headerMap) = 0;

  ///
  /// Get the resolved URL after redirects or changed as a result of HSTS.
  ///
  /*--acf()--*/
  virtual AcfString GetURL() = 0;

  ///
  /// Set the resolved URL after redirects or changed as a result of HSTS.
  ///
  /*--acf(optional_param=url)--*/
  virtual void SetURL(const AcfString& url) = 0;
};

#endif  // ACF_INCLUDE_RESPONSE_H_
