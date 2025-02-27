// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=41b709268e6f03256db5fa21b2faad10e4bd8e35$
//

#include "libacf_dll/cpptoc/web_message_factory_cpptoc.h"

#include "libacf_dll/cpptoc/web_message_handler_cpptoc.h"
#include "libacf_dll/ctocpp/browser_ctocpp.h"
#include "libacf_dll/ctocpp/frame_ctocpp.h"
#include "libacf_dll/ctocpp/web_message_proxy_ctocpp.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

acf_web_message_handler_t* ACF_CALLBACK
web_message_factory_create_host(struct _acf_web_message_factory_t* self,
                                acf_browser_t* browser,
                                const acf_string_t* origin,
                                acf_frame_t* frame,
                                struct _acf_web_message_proxy_t* reply_proxy) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  if (!self) {
    return NULL;
  }
  // Verify param: browser; type: refptr_diff
  if (!browser) {
    return NULL;
  }
  // Verify param: reply_proxy; type: refptr_diff
  if (!reply_proxy) {
    return NULL;
  }
  // Unverified params: origin, frame

  // Execute
  AcfRefPtr<AcfWebMessageHandler> _retval =
      AcfWebMessageFactoryCppToC::Get(self)->CreateHost(
          AcfBrowserCToCpp::Wrap(browser), AcfString(origin),
          AcfFrameCToCpp::Wrap(frame),
          AcfWebMessageProxyCToCpp::Wrap(reply_proxy));

  // Return type: refptr_same
  return AcfWebMessageHandlerCppToC::Wrap(_retval);
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

AcfWebMessageFactoryCppToC::AcfWebMessageFactoryCppToC() {
  GetStruct()->create_host = web_message_factory_create_host;
}

// DESTRUCTOR - Do not edit by hand.

AcfWebMessageFactoryCppToC::~AcfWebMessageFactoryCppToC() {}

template <>
AcfRefPtr<AcfWebMessageFactory> AcfCppToCRefCounted<
    AcfWebMessageFactoryCppToC,
    AcfWebMessageFactory,
    acf_web_message_factory_t>::UnwrapDerived(AcfWrapperType type,
                                              acf_web_message_factory_t* s) {
  return nullptr;
}

template <>
AcfWrapperType AcfCppToCRefCounted<AcfWebMessageFactoryCppToC,
                                   AcfWebMessageFactory,
                                   acf_web_message_factory_t>::kWrapperType =
    WT_WEB_MESSAGE_FACTORY;
