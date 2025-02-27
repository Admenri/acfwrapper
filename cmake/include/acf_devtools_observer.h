// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_DEVTOOLS_OBSERVER_H_
#define ACF_INCLUDE_ACF_DEVTOOLS_OBSERVER_H_

#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_types.h"

#include "include/acf_browser.h"

class AcfBrowser;

///
/// Callback interface for DevToolsMessageObserver.
///
/*--acf(source=client)--*/
class AcfDevToolsMessageObserver : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Method that will be called on receipt of a DevTools protocol message.
  /// |browser| is the originating browser instance. |message| is a UTF8-encoded
  /// JSON dictionary representing either a method result or an event. |message|
  /// is only valid for the scope of this callback and should be copied if
  /// necessary. Return true if the message was handled or false if the message
  /// should be further processed and passed to the OnDevToolsMethodResult or
  /// OnDevToolsEvent methods as appropriate.
  ///
  /// Method result dictionaries include an "id" (int) value that identifies the
  /// orginating method call sent from CefBrowserHost::SendDevToolsMessage, and
  /// optionally either a "result" (dictionary) or "error" (dictionary) value.
  /// The "error" dictionary will contain "code" (int) and "message" (string)
  /// values. Event dictionaries include a "method" (string) value and
  /// optionally a "params" (dictionary) value. See the DevTools protocol
  /// documentation at https://chromedevtools.github.io/devtools-protocol/ for
  /// details of supported method calls and the expected "result" or "params"
  /// dictionary contents. JSON dictionaries can be parsed using the
  /// CefParseJSON function if desired, however be aware of performance
  /// considerations when parsing large messages (some of which may exceed 1MB
  /// in size).
  ///
  /*--acf()--*/
  virtual void OnDevToolsMessage(AcfRefPtr<AcfBrowser> browser,
                                 const void* message,
                                 size_t message_size) {}

  ///
  /// Method that will be called when the DevTools agent has attached. |browser|
  /// is the originating browser instance. This will generally occur in response
  /// to the first message sent while the agent is detached.
  ///
  /*--acf()--*/
  virtual void OnDevToolsAgentAttached(AcfRefPtr<AcfBrowser> browser) {}

  ///
  /// Method that will be called when the DevTools agent has detached. |browser|
  /// is the originating browser instance. Any method results that were pending
  /// before the agent became detached will not be delivered, and any active
  /// event subscriptions will be canceled.
  ///
  /*--acf()--*/
  virtual void OnDevToolsAgentDetached(AcfRefPtr<AcfBrowser> browser) {}
};

#endif  // ACF_INCLUDE_ACF_DEVTOOLS_OBSERVER_H_
