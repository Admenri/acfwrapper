#ifndef WRAPPER_URLREQUEST_H_
#define WRAPPER_URLREQUEST_H_

#include "include/acf_urlrequest.h"
#include "wrapper_utility.h"

namespace wrapper {

class URLRequestClient : public AcfURLRequestClient {
 public:
  URLRequestClient(LPVOID callback);
  ~URLRequestClient() override;

 protected:
  void OnRequestComplete(AcfRefPtr<AcfURLRequest> request,
                         bool success) override;
  void OnUploadProgress(AcfRefPtr<AcfURLRequest> request,
                        int64 current,
                        int64 total) override;
  void OnDownloadProgress(AcfRefPtr<AcfURLRequest> request,
                          int64 current,
                          int64 total) override;
  void OnDownloadData(AcfRefPtr<AcfURLRequest> request,
                      const void* data,
                      size_t data_length) override;

 private:
  LPVOID callback_;
  IMPLEMENT_REFCOUNTING(URLRequestClient);
};

}  // namespace wrapper

#endif