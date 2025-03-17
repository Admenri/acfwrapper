// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_ENVIRONMENT_H_
#define ACF_INCLUDE_ACF_ENVIRONMENT_H_

#include <wtypes.h>

#include <map>
#include <string>

#include "include/acf_callback.h"
#include "include/acf_request.h"
#include "include/acf_resource_request_handler.h"
#include "include/acf_urlrequest.h"
#include "include/internal/acf_defines.h"
#include "include/internal/acf_enums.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_list.h"
#include "include/internal/acf_string_multimap.h"
#include "include/internal/acf_types.h"
#include "include/internal/acf_types_wrappers.h"

class AcfEnvironmentHandler;
class AcfEnvironment;
class AcfProfile;
class AcfBrowser;
class AcfBrowserHandler;
class AcfCompleteHandler;
class AcfResourceRequestHandler;
class AcfRequest;
class AcfFrame;

///
/// Get Ex functions activated state.
///
/*--acf()--*/
bool AcfGetExVersionActiveState();

///
/// ACF's environment event handler.
///
/*--acf(source=client)--*/
class AcfEnvironmentHandler : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Called when the environment has been initialized.
  /// This event will be called on runner thread.
  ///
  /*--acf()--*/
  virtual void OnProcessInitialized(AcfRefPtr<AcfEnvironment> environment,
                                    bool success) {}

  ///
  /// Browser process create a url loader.
  /// Request handler will intercept the network request on browser process.
  /// Raise that |frame_id| maybe nullptr when request is sent from service
  /// worker. |frame_id| can be matched with AcfFrame::GetIdentifier().
  ///
  /*--acf(optional_param=profile,optional_param=browser,optional_param=frame,optional_param=request_initiator)--*/
  virtual AcfRefPtr<AcfResourceRequestHandler> GetResourceRequestHandler(
      AcfRefPtr<AcfProfile> profile,
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfFrame> frame,
      AcfRefPtr<AcfRequest> request,
      acf_url_loader_factory_type_t type,
      const AcfString& request_initiator,
      bool& block_request) {
    return nullptr;
  }

  ///
  /// Browser kernel send a browser navigate request,
  /// intercepting navigate request for custom process.
  /// Must enable browser settings |disable_default_navigate|.
  ///
  /*--acf(optional_param=profile,optional_param=url,optional_param=referrer)--*/
  virtual void OnProcessNavigateRequest(AcfRefPtr<AcfProfile> profile,
                                        const AcfString& url,
                                        const AcfString& referrer,
                                        bool user_gesture) {}
};

///
/// ACF process environment control object.
///
/*--acf(source=library)--*/
class AcfEnvironment : public virtual AcfBaseRefCounted {
 public:
  typedef std::multimap<AcfString, AcfString> ExtraCommandLine;
  typedef acf_urlrequest_flags_t URLRequestFlags;

  ///
  /// Create new environment on target user data dir.
  /// Avoid create multi environment on the same data dir.
  ///
  /*--acf(optional_param=extra_command_line,optional_param=handler)--*/
  static AcfRefPtr<AcfEnvironment> CreateEnvironment(
      const AcfString& browser_path,
      const ExtraCommandLine& extra_command_line,
      const AcfEnvironmentSettings& settings,
      AcfRefPtr<AcfEnvironmentHandler> handler);

  ///
  /// Adjust the same object to other.
  ///
  /*--acf()--*/
  virtual bool IsSame(AcfRefPtr<AcfEnvironment> that) = 0;

  ///
  /// Get if environment is now valid.
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Get chromium kernel version number:
  /// e.g.:49.0.2623.125
  ///
  /*--acf()--*/
  virtual AcfString GetKernelVersion() = 0;

  ///
  /// Get default profile, path: "Default".
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfProfile> GetDefaultProfile() = 0;

  ///
  /// Create a profile hosted a user profile
  /// each path could only create a profile.
  /// Called handler on runner thread.
  ///
  /*--acf(optional_param=handler)--*/
  virtual AcfRefPtr<AcfProfile> CreateProfile(
      const AcfProfileCreateParams& params,
      AcfRefPtr<AcfCompleteHandler> handler) = 0;

  ///
  /// Create browser from environment (async)
  /// return object immediately but the object was invalid.
  ///
  /*--acf(optional_param=profile,optional_param=handler,optional_param=extra_info)--*/
  virtual AcfRefPtr<AcfBrowser> CreateBrowser(
      AcfRefPtr<AcfProfile> profile,
      AcfRefPtr<AcfBrowserHandler> handler,
      const AcfBrowserSettings& settings,
      const AcfBrowserCreateParams& params,
      AcfRefPtr<AcfDictionaryValue> extra_info) = 0;

  ///
  /// Create a new URL request that is not associated with a specific browser or
  /// frame. Use CefFrame::CreateURLRequest instead if you want the request to
  /// have this association, in which case it may be handled differently (see
  /// documentation on that method). A request created with this method may only
  /// originate from the browser process, and will behave as follows:
  ///   - POST data may only contain only a single element of type PDE_TYPE_FILE
  ///     or PDE_TYPE_BYTES.
  ///
  /// The |request| object will be marked as read-only after calling this
  /// method.
  ///
  /*--acf(optional_param=frame,optional_param=profile)--*/
  virtual AcfRefPtr<AcfURLRequest> CreateURLRequest(
      AcfRefPtr<AcfRequest> request,
      uint32 flags,
      AcfRefPtr<AcfURLRequestClient> client,
      AcfRefPtr<AcfFrame> frame,
      AcfRefPtr<AcfProfile> profile) = 0;

  ///
  /// Set network service context profile infomation.
  ///
  /*--acf(optional_param=tokens)--*/
  virtual void SetNetworkContextProfile(AcfRefPtr<AcfValue> tokens) = 0;
};

#endif  //! ACF_INCLUDE_ACF_ENVIRONMENT_H_
