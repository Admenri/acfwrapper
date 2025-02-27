// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_URLREQUEST_H_
#define ACF_INCLUDE_URLREQUEST_H_

#include "include/acf_frame.h"
#include "include/acf_profile.h"
#include "include/acf_request.h"
#include "include/acf_response.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_types_wrappers.h"

class AcfEnvironment;
class AcfURLRequestClient;

///
/// Class used to make a URL request. URL requests are not associated with a
/// browser instance so no CefClient callbacks will be executed. URL requests
/// can be created on any valid ACF thread in either the browser or render
/// process. Once created the methods of the URL request object must be accessed
/// on the same thread that created it.
///
/*--acf(source=library)--*/
class AcfURLRequest : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Returns the request object used to create this URL request. The returned
  /// object is read-only and should not be modified.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfRequest> GetRequest() = 0;

  ///
  /// Returns the client.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfURLRequestClient> GetClient() = 0;

  ///
  /// Returns the hosted environment.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfEnvironment> GetEnvironment() = 0;

  ///
  /// Returns the url request flags.
  ///
  /*--acf()--*/
  virtual uint32 GetFlags() = 0;

  ///
  /// Returns the response, or NULL if no response information is available.
  /// Response information will only be available after the upload has
  /// completed. The returned object is read-only and should not be modified.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfResponse> GetResponse() = 0;

  ///
  /// Cancel the request.
  ///
  /*--acf()--*/
  virtual void Cancel() = 0;
};

///
/// Interface that should be implemented by the CefURLRequest client. The
/// methods of this class will be called on the same thread that created the
/// request unless otherwise documented.
///
/*--acf(source=client)--*/
class AcfURLRequestClient : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Notifies the client that the request has completed. Use the
  /// CefURLRequest::GetRequestStatus method to determine if the request was
  /// successful or not.
  ///
  /*--acf()--*/
  virtual void OnRequestComplete(AcfRefPtr<AcfURLRequest> request,
                                 bool success) = 0;

  ///
  /// Notifies the client of upload progress. |current| denotes the number of
  /// bytes sent so far and |total| is the total size of uploading data (or -1
  /// if chunked upload is enabled). This method will only be called if the
  /// UR_FLAG_REPORT_UPLOAD_PROGRESS flag is set on the request.
  ///
  /*--acf()--*/
  virtual void OnUploadProgress(AcfRefPtr<AcfURLRequest> request,
                                int64 current,
                                int64 total) = 0;

  ///
  /// Notifies the client of download progress. |current| denotes the number of
  /// bytes received up to the call and |total| is the expected total size of
  /// the response (or -1 if not determined).
  ///
  /*--acf()--*/
  virtual void OnDownloadProgress(AcfRefPtr<AcfURLRequest> request,
                                  int64 current,
                                  int64 total) = 0;

  ///
  /// Called when some part of the response is read. |data| contains the current
  /// bytes received since the last call. This method will not be called if the
  /// UR_FLAG_NO_DOWNLOAD_DATA flag is set on the request.
  ///
  /*--acf()--*/
  virtual void OnDownloadData(AcfRefPtr<AcfURLRequest> request,
                              const void* data,
                              size_t data_length) = 0;
};

#endif  // ACF_INCLUDE_URLREQUEST_H_
