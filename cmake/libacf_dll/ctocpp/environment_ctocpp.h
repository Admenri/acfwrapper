// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=52403cae29623dc667ff3b19738c022ab651502e$
//

#ifndef ACF_CTOCPP_ENVIRONMENT_CTOCPP_H_
#define ACF_CTOCPP_ENVIRONMENT_CTOCPP_H_
#pragma once

#include "include/acf_browser.h"
#include "include/acf_callback.h"
#include "include/acf_environment.h"
#include "include/acf_frame.h"
#include "include/acf_profile.h"
#include "include/acf_request.h"
#include "include/acf_resource_request_handler.h"
#include "include/capi/acf_browser_capi.h"
#include "include/capi/acf_callback_capi.h"
#include "include/capi/acf_environment_capi.h"
#include "include/capi/acf_frame_capi.h"
#include "include/capi/acf_profile_capi.h"
#include "include/capi/acf_request_capi.h"
#include "include/capi/acf_resource_request_handler_capi.h"
#include "libacf_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class AcfEnvironmentCToCpp : public AcfCToCppRefCounted<AcfEnvironmentCToCpp,
                                                        AcfEnvironment,
                                                        acf_environment_t> {
 public:
  AcfEnvironmentCToCpp();
  virtual ~AcfEnvironmentCToCpp();

  // AcfEnvironment methods.
  bool IsSame(AcfRefPtr<AcfEnvironment> that) override;
  bool IsValid() override;
  AcfString GetKernelVersion() override;
  AcfRefPtr<AcfProfile> GetDefaultProfile() override;
  AcfRefPtr<AcfProfile> CreateProfile(
      const AcfProfileCreateParams& params,
      AcfRefPtr<AcfCompleteHandler> handler) override;
  AcfRefPtr<AcfBrowser> CreateBrowser(
      AcfRefPtr<AcfProfile> profile,
      AcfRefPtr<AcfBrowserHandler> handler,
      const AcfBrowserSettings& settings,
      const AcfBrowserCreateParams& params,
      AcfRefPtr<AcfDictionaryValue> extra_info) override;
  AcfRefPtr<AcfURLRequest> CreateURLRequest(
      AcfRefPtr<AcfRequest> request,
      uint32 flags,
      AcfRefPtr<AcfURLRequestClient> client,
      AcfRefPtr<AcfFrame> frame,
      AcfRefPtr<AcfProfile> profile) override;
  void SetNetworkContextProfile(AcfRefPtr<AcfValue> tokens) override;
};

#endif  // ACF_CTOCPP_ENVIRONMENT_CTOCPP_H_
