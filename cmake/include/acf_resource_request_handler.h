// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_RESOURCE_REQUEST_HANDLER_H_
#define ACF_INCLUDE_ACF_RESOURCE_REQUEST_HANDLER_H_

#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_types.h"

#include "include/acf_callback.h"
#include "include/acf_frame.h"
#include "include/acf_profile.h"
#include "include/acf_request.h"
#include "include/acf_response.h"

class AcfFrame;
class AcfCallback;
class AcfResponseFilter;
class AcfResourceRequestHandler;
class AcfResourceHandler;
class AcfResourceSkipCallback;
class AcfResourceReadCallback;

///
/// Implement this interface to handle events related to browser requests. The
/// methods of this class will be called on the runner thread unless otherwise
/// indicated.
///
/*--acf(source=client)--*/
class AcfResourceRequestHandler : public virtual AcfBaseRefCounted {
 public:
  typedef acf_urlrequest_status_t URLRequestStatus;

  ///
  /// Called on the runner thread before a resource request is loaded.
  /// You can modify request object data in this event until return.
  /// Return a custom resource handler to intercept network route.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfResourceHandler> OnBeforeResourceLoad(
      AcfRefPtr<AcfRequest> request) {
    return nullptr;
  }

  ///
  /// Called on the runner thread when a resource response is received.
  /// The response maybe a redirect request.
  /// You can modify request's URL and determined request's next action here.
  /// Return a filter object for modifing response body data.
  /// mode must fill with acf_response_mode_t type data.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfResponseFilter> OnResourceResponse(
      AcfRefPtr<AcfRequest> request,
      AcfRefPtr<AcfResponse> response,
      int& mode) {
    return nullptr;
  }

  ///
  /// Called on the runner thread when a resource load has completed.
  /// Failed request maybe sent in this event.
  ///
  /*--acf()--*/
  virtual void OnResourceLoadComplete(AcfRefPtr<AcfRequest> request,
                                      URLRequestStatus status,
                                      int64 received_content_length) {}
};

///
/// Callback for asynchronous continuation of AcfResourceHandler::Skip().
///
/*--acf(source=library)--*/
class AcfResourceSkipCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Callback for asynchronous continuation of Skip(). If |bytes_skipped| > 0
  /// then either Skip() will be called again until the requested number of
  /// bytes have been skipped or the request will proceed. If |bytes_skipped|
  /// <= 0 the request will fail with ERR_REQUEST_RANGE_NOT_SATISFIABLE.
  ///
  /*--acf(capi_name=cont)--*/
  virtual void Continue(int64 bytes_skipped) = 0;
};

///
/// Callback for asynchronous continuation of AcfResourceHandler::Read().
///
/*--acf(source=library)--*/
class AcfResourceReadCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Callback for asynchronous continuation of Read(). If |bytes_read| == 0
  /// the response will be considered complete. If |bytes_read| > 0 then Read()
  /// will be called again until the request is complete (based on either the
  /// result or the expected content length). If |bytes_read| < 0 then the
  /// request will fail and the |bytes_read| value will be treated as the error
  /// code.
  ///
  /*--acf(capi_name=cont)--*/
  virtual void Continue(int bytes_read) = 0;
};

///
/// Class used to implement a custom request handler interface. The methods of
/// this class will be called on the IO thread unless otherwise indicated.
///
/*--acf(source=client)--*/
class AcfResourceHandler : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Retrieve response header information. If the response length is not known
  /// set |response_length| to -1 and ReadResponse() will be called until it
  /// returns false. If the response length is known set |response_length|
  /// to a positive value and ReadResponse() will be called until it returns
  /// false or the specified number of bytes have been read. Use the |response|
  /// object to set the mime type, http status code and other optional header
  /// values. To redirect the request to a new URL set |redirectUrl| to the new
  /// URL. |redirectUrl| can be either a relative or fully qualified URL.
  /// It is also possible to set |response| to a redirect http status code
  /// and pass the new URL via a Location header. Likewise with |redirectUrl| it
  /// is valid to set a relative or fully qualified URL as the Location header
  /// value. If an error occured while setting up the request you can call
  /// SetError() on |response| to indicate the error condition.
  ///
  /*--acf()--*/
  virtual void GetResponseHeaders(AcfRefPtr<AcfResponse> response,
                                  int64& response_length) = 0;

  ///
  /// Skip response data when requested by a Range header. Skip over and discard
  /// |bytes_to_skip| bytes of response data. If data is available immediately
  /// set |bytes_skipped| to the number of bytes skipped and return true. To
  /// read the data at a later time set |bytes_skipped| to 0, return true and
  /// execute |callback| when the data is available. To indicate failure set
  /// |bytes_skipped| to < 0 (e.g. -2 for ERR_FAILED) and return false. This
  /// method will be called in sequence but not from a dedicated thread.
  ///
  /*--acf()--*/
  virtual void Skip(int64 bytes_to_skip,
                    AcfRefPtr<AcfResourceSkipCallback> callback) {}

  ///
  /// Read response data. If data is available immediately copy up to
  /// |bytes_to_read| bytes into |data_out|, set |bytes_read| to the number of
  /// bytes copied, and return true. To read the data at a later time keep a
  /// pointer to |data_out|, set |bytes_read| to 0, return true and execute
  /// |callback| when the data is available (|data_out| will remain valid until
  /// the callback is executed). To indicate response completion set
  /// |bytes_read| to 0 and return false. To indicate failure set |bytes_read|
  /// to < 0 (e.g. -2 for ERR_FAILED) and return false. This method will be
  /// called in sequence but not from a dedicated thread. For backwards
  /// compatibility set |bytes_read| to -1 and return false and the ReadResponse
  /// method will be called.
  ///
  /*--acf()--*/
  virtual void Read(void* data_out,
                    int bytes_to_read,
                    AcfRefPtr<AcfResourceReadCallback> callback) {}
};

///
/// Implement this interface to filter resource response content. The methods of
/// this class will be called on the browser process IO thread.
///
/*--acf(source=client)--*/
class AcfResponseFilter : public virtual AcfBaseRefCounted {
 public:
  typedef acf_response_filter_status_t FilterStatus;

  ///
  /// Initialize the response filter. Will only be called a single time. The
  /// filter will not be installed if this method returns false.
  ///
  /*--acf()--*/
  virtual bool InitFilter() = 0;

  ///
  /// Called to filter a chunk of data. Expected usage is as follows:
  ///
  ///  1. Read input data from |data_in| and set |data_in_read| to the number of
  ///     bytes that were read up to a maximum of |data_in_size|. |data_in| will
  ///     be NULL if |data_in_size| is zero.
  ///  2. Write filtered output data to |data_out| and set |data_out_written| to
  ///     the number of bytes that were written up to a maximum of
  ///     |data_out_size|. If no output data was written then all data must be
  ///     read from |data_in| (user must set |data_in_read| = |data_in_size|).
  ///  3. Return RESPONSE_FILTER_DONE if all output data was written or
  ///     RESPONSE_FILTER_NEED_MORE_DATA if output data is still pending.
  ///
  /// This method will be called repeatedly until the input buffer has been
  /// fully read (user sets |data_in_read| = |data_in_size|) and there is no
  /// more input data to filter (the resource response is complete). This method
  /// may then be called an additional time with an empty input buffer if the
  /// user filled the output buffer (set |data_out_written| = |data_out_size|)
  /// and returned RESPONSE_FILTER_NEED_MORE_DATA to indicate that output data
  /// is still pending.
  ///
  /// Calls to this method will stop when one of the following conditions is
  /// met:
  ///
  ///  1. There is no more input data to filter (the resource response is
  ///     complete) and the user sets |data_out_written| = 0 or returns
  ///     RESPONSE_FILTER_DONE to indicate that all data has been written, or;
  ///  2. The user returns RESPONSE_FILTER_ERROR to indicate an error.
  ///
  /// Do not keep a reference to the buffers passed to this method.
  ///
  /*--acf(optional_param=data_in,default_retval=RESPONSE_FILTER_ERROR)--*/
  virtual FilterStatus Filter(void* data_in,
                              size_t data_in_size,
                              size_t& data_in_read,
                              void* data_out,
                              size_t data_out_size,
                              size_t& data_out_written) = 0;
};

#endif  // ACF_INCLUDE_RESOURCE_REQUEST_HANDLER_H_
