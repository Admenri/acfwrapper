// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=24488e30acf48dce599cd7d78d20d21432ad4d82$
//

#include "libacf_dll/cpptoc/resource_request_handler_cpptoc.h"

#include "libacf_dll/cpptoc/resource_handler_cpptoc.h"
#include "libacf_dll/cpptoc/response_filter_cpptoc.h"
#include "libacf_dll/ctocpp/request_ctocpp.h"
#include "libacf_dll/ctocpp/response_ctocpp.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

struct _acf_resource_handler_t* ACF_CALLBACK
resource_request_handler_on_before_resource_load(
    struct _acf_resource_request_handler_t* self,
    acf_request_t* request) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return NULL;
  }
  // Verify param: request; type: refptr_diff
  if (!request) {
    return NULL;
  }

  // Execute
  AcfRefPtr<AcfResourceHandler> _retval =
      AcfResourceRequestHandlerCppToC::Get(self)->OnBeforeResourceLoad(
          AcfRequestCToCpp::Wrap(request));

  // Return type: refptr_same
  return AcfResourceHandlerCppToC::Wrap(_retval);
}

struct _acf_response_filter_t* ACF_CALLBACK
resource_request_handler_on_resource_response(
    struct _acf_resource_request_handler_t* self,
    acf_request_t* request,
    struct _acf_response_t* response,
    int* mode) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return NULL;
  }
  // Verify param: request; type: refptr_diff
  if (!request) {
    return NULL;
  }
  // Verify param: response; type: refptr_diff
  if (!response) {
    return NULL;
  }
  // Verify param: mode; type: simple_byref
  if (!mode) {
    return NULL;
  }

  // Translate param: mode; type: simple_byref
  int modeVal = mode ? *mode : 0;

  // Execute
  AcfRefPtr<AcfResponseFilter> _retval =
      AcfResourceRequestHandlerCppToC::Get(self)->OnResourceResponse(
          AcfRequestCToCpp::Wrap(request), AcfResponseCToCpp::Wrap(response),
          modeVal);

  // Restore param: mode; type: simple_byref
  if (mode) {
    *mode = modeVal;
  }

  // Return type: refptr_same
  return AcfResponseFilterCppToC::Wrap(_retval);
}

void ACF_CALLBACK resource_request_handler_on_resource_load_complete(
    struct _acf_resource_request_handler_t* self,
    acf_request_t* request,
    acf_urlrequest_status_t status,
    int64 received_content_length) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return;
  }
  // Verify param: request; type: refptr_diff
  if (!request) {
    return;
  }

  // Execute
  AcfResourceRequestHandlerCppToC::Get(self)->OnResourceLoadComplete(
      AcfRequestCToCpp::Wrap(request), status, received_content_length);
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

AcfResourceRequestHandlerCppToC::AcfResourceRequestHandlerCppToC() {
  GetStruct()->on_before_resource_load =
      resource_request_handler_on_before_resource_load;
  GetStruct()->on_resource_response =
      resource_request_handler_on_resource_response;
  GetStruct()->on_resource_load_complete =
      resource_request_handler_on_resource_load_complete;
}

// DESTRUCTOR - Do not edit by hand.

AcfResourceRequestHandlerCppToC::~AcfResourceRequestHandlerCppToC() {}

template <>
AcfRefPtr<AcfResourceRequestHandler>
AcfCppToCRefCounted<AcfResourceRequestHandlerCppToC,
                    AcfResourceRequestHandler,
                    acf_resource_request_handler_t>::
    UnwrapDerived(AcfWrapperType type, acf_resource_request_handler_t* s) {
  return nullptr;
}

template <>
AcfWrapperType
    AcfCppToCRefCounted<AcfResourceRequestHandlerCppToC,
                        AcfResourceRequestHandler,
                        acf_resource_request_handler_t>::kWrapperType =
        WT_RESOURCE_REQUEST_HANDLER;
