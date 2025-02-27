// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_WEB_MESSAGE_HANDLER_H_
#define ACF_INCLUDE_ACF_WEB_MESSAGE_HANDLER_H_

#include "include/acf_browser.h"
#include "include/acf_frame.h"
#include "include/acf_values.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_types.h"

class AcfWebMessageProxy;
class AcfValue;
class AcfFrame;

typedef std::vector<AcfRefPtr<AcfValue>> WebMessage;

///
/// Web message factory handler
///
/*--acf(source=client)--*/
class AcfWebMessageHandler : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Create web message host handler for user-side
  ///
  /*--acf(optional_param=message)--*/
  virtual void OnPostMessage(const WebMessage& message) {}

  ///
  /// Page cache state changed.
  ///
  /*--acf()--*/
  virtual void OnBackForwardCacheStateChanged() {}
};

///
/// Web message factory
///
/*--acf(source=client)--*/
class AcfWebMessageFactory : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Create web message host handler for user-side
  ///
  /*--acf(optional_param=origin,optional_param=frame)--*/
  virtual AcfRefPtr<AcfWebMessageHandler> CreateHost(
      AcfRefPtr<AcfBrowser> browser,
      const AcfString& origin,
      AcfRefPtr<AcfFrame> frame,
      AcfRefPtr<AcfWebMessageProxy> reply_proxy) = 0;
};

///
/// Web message proxy
///
/*--acf(source=library)--*/
class AcfWebMessageProxy : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Post reply message to renderer.
  ///
  /*--acf(optional_param=message)--*/
  virtual void PostWebMessage(const WebMessage& message) = 0;

  ///
  /// Get if page in forward-back cache
  ///
  /*--acf()--*/
  virtual bool IsInBackForwardCache() = 0;
};

#endif  // ACF_INCLUDE_WEB_MESSAGE_HANDLER_H_