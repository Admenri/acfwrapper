// Copyright (c) 2025 Admenri. All rights reserved.
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
// $hash=5d36aa3fbf2bde55f26351e47d34c63b7084c15a$
//

#ifndef ACF_INCLUDE_CAPI_ACF_DEVTOOLS_OBSERVER_CAPI_H_
#define ACF_INCLUDE_CAPI_ACF_DEVTOOLS_OBSERVER_CAPI_H_
#pragma once

#include "include/capi/acf_browser_capi.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _acf_browser_t;

///
/// Callback structure for DevToolsMessageObserver.
///
typedef struct _acf_dev_tools_message_observer_t {
  ///
  /// Base structure.
  ///
  acf_base_ref_counted_t base;

  ///
  /// Method that will be called on receipt of a DevTools protocol message.
  /// |browser| is the originating browser instance. |message| is a UTF8-encoded
  /// JSON dictionary representing either a function result or an event.
  /// |message| is only valid for the scope of this callback and should be
  /// copied if necessary. Return true (1) if the message was handled or false
  /// (0) if the message should be further processed and passed to the
  /// OnDevToolsMethodResult or OnDevToolsEvent functions as appropriate.
  ///
  /// Method result dictionaries include an "id" (int) value that identifies the
  /// orginating function call sent from CefBrowserHost::SendDevToolsMessage,
  /// and optionally either a "result" (dictionary) or "error" (dictionary)
  /// value. The "error" dictionary will contain "code" (int) and "message"
  /// (string) values. Event dictionaries include a "function" (string) value
  /// and optionally a "params" (dictionary) value. See the DevTools protocol
  /// documentation at https://chromedevtools.github.io/devtools-protocol/ for
  /// details of supported function calls and the expected "result" or "params"
  /// dictionary contents. JSON dictionaries can be parsed using the
  /// CefParseJSON function if desired, however be aware of performance
  /// considerations when parsing large messages (some of which may exceed 1MB
  /// in size).
  ///
  void(ACF_CALLBACK* on_dev_tools_message)(
      struct _acf_dev_tools_message_observer_t* self,
      struct _acf_browser_t* browser,
      const void* message,
      size_t message_size);

  ///
  /// Method that will be called when the DevTools agent has attached. |browser|
  /// is the originating browser instance. This will generally occur in response
  /// to the first message sent while the agent is detached.
  ///
  void(ACF_CALLBACK* on_dev_tools_agent_attached)(
      struct _acf_dev_tools_message_observer_t* self,
      struct _acf_browser_t* browser);

  ///
  /// Method that will be called when the DevTools agent has detached. |browser|
  /// is the originating browser instance. Any function results that were
  /// pending before the agent became detached will not be delivered, and any
  /// active event subscriptions will be canceled.
  ///
  void(ACF_CALLBACK* on_dev_tools_agent_detached)(
      struct _acf_dev_tools_message_observer_t* self,
      struct _acf_browser_t* browser);
} acf_dev_tools_message_observer_t;

#ifdef __cplusplus
}
#endif

#endif  // ACF_INCLUDE_CAPI_ACF_DEVTOOLS_OBSERVER_CAPI_H_
