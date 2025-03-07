// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=3200df749192f6df3433218a52ef48ecd30a6d79$
//

#include "libacf_dll/cpptoc/resource_handler_cpptoc.h"

#include "libacf_dll/ctocpp/resource_read_callback_ctocpp.h"
#include "libacf_dll/ctocpp/resource_skip_callback_ctocpp.h"
#include "libacf_dll/ctocpp/response_ctocpp.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

void ACF_CALLBACK
resource_handler_get_response_headers(struct _acf_resource_handler_t* self,
                                      struct _acf_response_t* response,
                                      int64* response_length) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return;
  }
  // Verify param: response; type: refptr_diff
  if (!response) {
    return;
  }
  // Verify param: response_length; type: simple_byref
  if (!response_length) {
    return;
  }

  // Translate param: response_length; type: simple_byref
  int64 response_lengthVal = response_length ? *response_length : 0;

  // Execute
  AcfResourceHandlerCppToC::Get(self)->GetResponseHeaders(
      AcfResponseCToCpp::Wrap(response), response_lengthVal);

  // Restore param: response_length; type: simple_byref
  if (response_length) {
    *response_length = response_lengthVal;
  }
}

void ACF_CALLBACK
resource_handler_skip(struct _acf_resource_handler_t* self,
                      int64 bytes_to_skip,
                      acf_resource_skip_callback_t* callback) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return;
  }
  // Verify param: callback; type: refptr_diff
  if (!callback) {
    return;
  }

  // Execute
  AcfResourceHandlerCppToC::Get(self)->Skip(
      bytes_to_skip, AcfResourceSkipCallbackCToCpp::Wrap(callback));
}

void ACF_CALLBACK
resource_handler_read(struct _acf_resource_handler_t* self,
                      void* data_out,
                      int bytes_to_read,
                      acf_resource_read_callback_t* callback) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return;
  }
  // Verify param: data_out; type: simple_byaddr
  if (!data_out) {
    return;
  }
  // Verify param: callback; type: refptr_diff
  if (!callback) {
    return;
  }

  // Execute
  AcfResourceHandlerCppToC::Get(self)->Read(
      data_out, bytes_to_read, AcfResourceReadCallbackCToCpp::Wrap(callback));
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

AcfResourceHandlerCppToC::AcfResourceHandlerCppToC() {
  GetStruct()->get_response_headers = resource_handler_get_response_headers;
  GetStruct()->skip = resource_handler_skip;
  GetStruct()->read = resource_handler_read;
}

// DESTRUCTOR - Do not edit by hand.

AcfResourceHandlerCppToC::~AcfResourceHandlerCppToC() {}

template <>
AcfRefPtr<AcfResourceHandler> AcfCppToCRefCounted<
    AcfResourceHandlerCppToC,
    AcfResourceHandler,
    acf_resource_handler_t>::UnwrapDerived(AcfWrapperType type,
                                           acf_resource_handler_t* s) {
  return nullptr;
}

template <>
AcfWrapperType AcfCppToCRefCounted<AcfResourceHandlerCppToC,
                                   AcfResourceHandler,
                                   acf_resource_handler_t>::kWrapperType =
    WT_RESOURCE_HANDLER;
