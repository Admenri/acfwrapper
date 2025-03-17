#ifndef ACF_WRAPPER_ACFCLASS_H_
#define ACF_WRAPPER_ACFCLASS_H_
#pragma once

#include "include/acf_environment.h"
#include "include/acf_resource_request_handler.h"

#include <sstream>

class ACFResourceFilterHandler;

class AcfRequestResponseWrapper : public AcfResourceHandler {
 public:
  AcfRequestResponseWrapper(const void* buffer,
                            size_t size,
                            AcfRefPtr<AcfResponse> response,
                            int64_t length);
  ~AcfRequestResponseWrapper();

 protected:
  void GetResponseHeaders(AcfRefPtr<AcfResponse> response,
                          int64& response_length) override;
  void Skip(int64 bytes_to_skip,
            AcfRefPtr<AcfResourceSkipCallback> callback) override;
  void Read(void* data_out,
            int bytes_to_read,
            AcfRefPtr<AcfResourceReadCallback> callback) override;

 private:
  std::string data_;
  AcfRefPtr<AcfResponse> response_;
  int64_t length_;

  size_t offset_ = 0;

  IMPLEMENT_REFCOUNTING(AcfRequestResponseWrapper);
};

class ACFResourceResponseCallback : public virtual AcfBaseRefCounted {
 public:
  ACFResourceResponseCallback() {}
  ~ACFResourceResponseCallback() {}

  void Continue(AcfRefPtr<AcfRequestResponseWrapper> v) { value_ = v; }

  AcfRefPtr<AcfRequestResponseWrapper> GetResponse() { return value_; }

 private:
  AcfRefPtr<AcfRequestResponseWrapper> value_;

  IMPLEMENT_REFCOUNTING(ACFResourceResponseCallback);
};

class ACFResourceFilter : public virtual AcfBaseRefCounted {
 public:
  ACFResourceFilter(AcfRefPtr<ACFResourceFilterHandler> filter);
  ~ACFResourceFilter();

  size_t GetTotalBytes();
  void GetBuffer(void* outputBuffer, size_t length);
  void SetBuffer(void* inputBuffer, size_t length);

 private:
  AcfRefPtr<ACFResourceFilterHandler> _rawptr;

 protected:
  IMPLEMENT_REFCOUNTING(ACFResourceFilter);
};

class ACFResourceFilterHandler : public AcfResponseFilter {
 public:
  ACFResourceFilterHandler(LPVOID callback,
                           AcfRefPtr<AcfProfile> profile,
                           AcfRefPtr<AcfBrowser> browser,
                           AcfRefPtr<AcfFrame> frame,
                           AcfRefPtr<AcfRequest> request,
                           AcfRefPtr<AcfResponse> response);
  ~ACFResourceFilterHandler();

  size_t GetTotalBytes();
  void GetBuffer(void* outputBuffer, size_t length);
  void SetBuffer(void* inputBuffer, size_t length);

 private:
  LPVOID _rawcallback;

  AcfRefPtr<AcfProfile> profile_;
  AcfRefPtr<AcfBrowser> browser_;
  AcfRefPtr<AcfFrame> frame_;

  AcfRefPtr<AcfRequest> request_;
  AcfRefPtr<AcfResponse> response_;

  BOOL _readEnded = FALSE;
  std::stringstream _rdbuf;
  size_t _totalReadBytes = 0;

 protected:
  bool InitFilter() override;
  FilterStatus Filter(void* data_in,
                      size_t data_in_size,
                      size_t& data_in_read,
                      void* data_out,
                      size_t data_out_size,
                      size_t& data_out_written) override;

  IMPLEMENT_REFCOUNTING(ACFResourceFilterHandler);
};

#endif  // ACF_WRAPPER_ACFCLASS_H_
