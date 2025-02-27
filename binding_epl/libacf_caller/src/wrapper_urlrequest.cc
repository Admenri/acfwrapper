#include "wrapper_urlrequest.h"

#include "include/acf_frame.h"
#include "include/acf_urlrequest.h"
#include "wrapper_utility.h"

extern DWORD acf_cpp_fntable_request[];
extern DWORD acf_cpp_fntable_response[];
extern DWORD acf_cpp_fntable_urlrequest[];

namespace wrapper {

URLRequestClient::URLRequestClient(LPVOID callback) : callback_(callback) {}

URLRequestClient::~URLRequestClient() {}

void URLRequestClient::OnRequestComplete(AcfRefPtr<AcfURLRequest> request,
                                         bool success) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    request->AddRef();
    IMP_NEWECLASS(TempRequest, request.get(), eClass::m_pVfTable_UrlRequest,
                  acf_cpp_fntable_urlrequest);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      movzx eax, success;
      push eax;
			push TempRequest;
			push ecx;
			call[edx + 0x08];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    request->Release();
  }
}

void URLRequestClient::OnUploadProgress(AcfRefPtr<AcfURLRequest> request,
                                        int64 current,
                                        int64 total) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    request->AddRef();
    IMP_NEWECLASS(TempRequest, request.get(), eClass::m_pVfTable_UrlRequest,
                  acf_cpp_fntable_urlrequest);
    uint32_t cur32 = current;
    uint32_t tot32 = total;
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push tot32;
      push cur32;
			push TempRequest;
			push ecx;
			call[edx + 0x0C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    request->Release();
  }
}

void URLRequestClient::OnDownloadProgress(AcfRefPtr<AcfURLRequest> request,
                                          int64 current,
                                          int64 total) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    request->AddRef();
    IMP_NEWECLASS(TempRequest, request.get(), eClass::m_pVfTable_UrlRequest,
                  acf_cpp_fntable_urlrequest);
    uint32_t cur32 = current;
    uint32_t tot32 = total;
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push tot32;
      push cur32;
			push TempRequest;
			push ecx;
			call[edx + 0x10];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    request->Release();
  }
}

void URLRequestClient::OnDownloadData(AcfRefPtr<AcfURLRequest> request,
                                      const void* data,
                                      size_t data_length) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    request->AddRef();
    IMP_NEWECLASS(TempRequest, request.get(), eClass::m_pVfTable_UrlRequest,
                  acf_cpp_fntable_urlrequest);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push data_length;
      push data;
			push TempRequest;
			push ecx;
			call[edx + 0x14];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    request->Release();
  }
}

}  // namespace wrapper

namespace {

void ACF_CALLBACK get_request(AcfURLRequest* obj, DWORD* retobj) {
  auto r = obj->GetRequest();

  if (r) {
    r->AddRef();
    retobj[1] = (DWORD)r.get();
    retobj[2] = (DWORD)acf_cpp_fntable_request;
  }
}

uint32_t ACF_CALLBACK get_request_flags(AcfURLRequest* obj) {
  return obj->GetFlags();
}

void ACF_CALLBACK get_response(AcfURLRequest* obj, DWORD* retobj) {
  auto r = obj->GetResponse();

  if (r) {
    r->AddRef();
    retobj[1] = (DWORD)r.get();
    retobj[2] = (DWORD)acf_cpp_fntable_response;
  }
}

void ACF_CALLBACK cancel(AcfURLRequest* obj) {
  obj->Cancel();
}

}  // namespace

DWORD acf_cpp_fntable_urlrequest[] = {
    (DWORD)get_request,
    (DWORD)get_request_flags,
    (DWORD)get_response,
    (DWORD)cancel,
};
