// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=f22d4a3fae7f66ed8aec0159d579853ea88a2192$
//

#include "libacf_dll/cpptoc/environment_handler_cpptoc.h"

#include "libacf_dll/cpptoc/resource_request_handler_cpptoc.h"
#include "libacf_dll/ctocpp/environment_ctocpp.h"
#include "libacf_dll/ctocpp/profile_ctocpp.h"
#include "libacf_dll/ctocpp/request_ctocpp.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

void ACF_CALLBACK environment_handler_on_process_initialized(
    struct _acf_environment_handler_t* self,
    struct _acf_environment_t* environment,
    int success) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return;
  }
  // Verify param: environment; type: refptr_diff
  if (!environment) {
    return;
  }

  // Execute
  AcfEnvironmentHandlerCppToC::Get(self)->OnProcessInitialized(
      AcfEnvironmentCToCpp::Wrap(environment), success ? true : false);
}

struct _acf_resource_request_handler_t* ACF_CALLBACK
environment_handler_get_resource_request_handler(
    struct _acf_environment_handler_t* self,
    struct _acf_profile_t* profile,
    int64 frame_id,
    struct _acf_request_t* request,
    acf_url_loader_factory_type_t type,
    const acf_string_t* request_initiator,
    int* block_request) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return NULL;
  }
  // Verify param: request; type: refptr_diff
  if (!request) {
    return NULL;
  }
  // Verify param: block_request; type: bool_byref
  if (!block_request) {
    return NULL;
  }
  // Unverified params: profile, request_initiator

  // Translate param: block_request; type: bool_byref
  bool block_requestBool = (block_request && *block_request) ? true : false;

  // Execute
  AcfRefPtr<AcfResourceRequestHandler> _retval =
      AcfEnvironmentHandlerCppToC::Get(self)->GetResourceRequestHandler(
          AcfProfileCToCpp::Wrap(profile), frame_id,
          AcfRequestCToCpp::Wrap(request), type, AcfString(request_initiator),
          block_requestBool);

  // Restore param: block_request; type: bool_byref
  if (block_request) {
    *block_request = block_requestBool ? true : false;
  }

  // Return type: refptr_same
  return AcfResourceRequestHandlerCppToC::Wrap(_retval);
}

void ACF_CALLBACK environment_handler_on_process_navigate_request(
    struct _acf_environment_handler_t* self,
    struct _acf_profile_t* profile,
    const acf_string_t* url,
    const acf_string_t* referrer,
    int user_gesture) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return;
  }
  // Unverified params: profile, url, referrer

  // Execute
  AcfEnvironmentHandlerCppToC::Get(self)->OnProcessNavigateRequest(
      AcfProfileCToCpp::Wrap(profile), AcfString(url), AcfString(referrer),
      user_gesture ? true : false);
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

AcfEnvironmentHandlerCppToC::AcfEnvironmentHandlerCppToC() {
  GetStruct()->on_process_initialized =
      environment_handler_on_process_initialized;
  GetStruct()->get_resource_request_handler =
      environment_handler_get_resource_request_handler;
  GetStruct()->on_process_navigate_request =
      environment_handler_on_process_navigate_request;
}

// DESTRUCTOR - Do not edit by hand.

AcfEnvironmentHandlerCppToC::~AcfEnvironmentHandlerCppToC() {}

template <>
AcfRefPtr<AcfEnvironmentHandler> AcfCppToCRefCounted<
    AcfEnvironmentHandlerCppToC,
    AcfEnvironmentHandler,
    acf_environment_handler_t>::UnwrapDerived(AcfWrapperType type,
                                              acf_environment_handler_t* s) {
  return nullptr;
}

template <>
AcfWrapperType AcfCppToCRefCounted<AcfEnvironmentHandlerCppToC,
                                   AcfEnvironmentHandler,
                                   acf_environment_handler_t>::kWrapperType =
    WT_ENVIRONMENT_HANDLER;
