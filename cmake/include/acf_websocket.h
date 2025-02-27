// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_WEBSOCKET_H_
#define ACF_INCLUDE_ACF_WEBSOCKET_H_

#include "include/acf_values.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string_list.h"
#include "include/internal/acf_types.h"

class AcfValue;
class AcfWebSocketClient;

///
/// Websocket client handler
///
/*--acf(source=client)--*/
class AcfWebSocketClientHandler : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Received handshake response from network service.
  /// Client provide the client from this event.
  ///
  /*--acf()--*/
  virtual void OnHandshakeResponse(AcfRefPtr<AcfWebSocketClient> client) {}

  ///
  /// Handshake connection failed.
  ///
  /*--acf(optional_param=message)--*/
  virtual void OnFailure(const AcfString& message,
                         int net_err,
                         int response_code) {}

  ///
  /// Received message from spec side in websocket connection.
  /// Return true for intercept message sending to another side.
  /// message can be modified during this event.
  ///
  /*--acf()--*/
  virtual bool OnReceivedMessageFrom(acf_websocket_message_routing_t direction,
                                     AcfRefPtr<AcfValue> message) {
    return false;
  }

  ///
  /// Before connection closing, user can decide keeping socket.
  /// Return true for keep single-side connection for websocket.
  ///
  /*--acf()--*/
  virtual void OnConnectionClosing() {}
};

///
/// Websocket client used in host process
///
/*--acf(source=library)--*/
class AcfWebSocketClient : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Get object valid state.
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Get storage handler.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfWebSocketClientHandler> GetHandler() = 0;

  ///
  /// Send message to spec side in websocket connection. |message| is
  /// the AcfValue that only received String type or AcfBinary Type.
  ///
  /*--acf()--*/
  virtual void SendMessageTo(acf_websocket_message_routing_t direction,
                             AcfRefPtr<AcfValue> message) = 0;

  ///
  /// Send handshake closing request to spec direction.
  /// If all side are closed, it will force destroy connection.
  ///
  /*--acf()--*/
  virtual void SendCloseConnectionRequest(
      acf_websocket_message_routing_t direction) = 0;
};

#endif  // ACF_INCLUDE_ACF_WEBSOCKET_H_
