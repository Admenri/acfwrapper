// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=eac8b6f8d0c408a49b409466672b824b21413cae$
//

#include "libacf_dll/cpptoc/response_filter_cpptoc.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

int ACF_CALLBACK
response_filter_init_filter(struct _acf_response_filter_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return 0;
  }

  // Execute
  bool _retval = AcfResponseFilterCppToC::Get(self)->InitFilter();

  // Return type: bool
  return _retval;
}

acf_response_filter_status_t ACF_CALLBACK
response_filter_filter(struct _acf_response_filter_t* self,
                       void* data_in,
                       size_t data_in_size,
                       size_t* data_in_read,
                       void* data_out,
                       size_t data_out_size,
                       size_t* data_out_written) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return RESPONSE_FILTER_ERROR;
  }
  // Verify param: data_in_read; type: simple_byref
  if (!data_in_read) {
    return RESPONSE_FILTER_ERROR;
  }
  // Verify param: data_out; type: simple_byaddr
  if (!data_out) {
    return RESPONSE_FILTER_ERROR;
  }
  // Verify param: data_out_written; type: simple_byref
  if (!data_out_written) {
    return RESPONSE_FILTER_ERROR;
  }
  // Unverified params: data_in

  // Translate param: data_in_read; type: simple_byref
  size_t data_in_readVal = data_in_read ? *data_in_read : 0;
  // Translate param: data_out_written; type: simple_byref
  size_t data_out_writtenVal = data_out_written ? *data_out_written : 0;

  // Execute
  acf_response_filter_status_t _retval =
      AcfResponseFilterCppToC::Get(self)->Filter(
          data_in, data_in_size, data_in_readVal, data_out, data_out_size,
          data_out_writtenVal);

  // Restore param: data_in_read; type: simple_byref
  if (data_in_read) {
    *data_in_read = data_in_readVal;
  }
  // Restore param: data_out_written; type: simple_byref
  if (data_out_written) {
    *data_out_written = data_out_writtenVal;
  }

  // Return type: simple
  return _retval;
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

AcfResponseFilterCppToC::AcfResponseFilterCppToC() {
  GetStruct()->init_filter = response_filter_init_filter;
  GetStruct()->filter = response_filter_filter;
}

// DESTRUCTOR - Do not edit by hand.

AcfResponseFilterCppToC::~AcfResponseFilterCppToC() {}

template <>
AcfRefPtr<AcfResponseFilter> AcfCppToCRefCounted<
    AcfResponseFilterCppToC,
    AcfResponseFilter,
    acf_response_filter_t>::UnwrapDerived(AcfWrapperType type,
                                          acf_response_filter_t* s) {
  return nullptr;
}

template <>
AcfWrapperType AcfCppToCRefCounted<AcfResponseFilterCppToC,
                                   AcfResponseFilter,
                                   acf_response_filter_t>::kWrapperType =
    WT_RESPONSE_FILTER;
