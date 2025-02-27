#ifndef WRAPPER_WEBSOCKET_H_
#define WRAPPER_WEBSOCKET_H_

#include "include/acf_browser.h"
#include "include/acf_frame.h"
#include "include/acf_websocket.h"
#include "wrapper_utility.h"

namespace wrapper {

class WebSocketHandler : public AcfWebSocketClientHandler {
 public:
  WebSocketHandler(AcfRefPtr<AcfBrowser> browser, LPVOID callback);
  ~WebSocketHandler() override;

 protected:
  void OnHandshakeResponse(AcfRefPtr<AcfWebSocketClient> client) override;
  void OnFailure(const AcfString& message,
                 int net_err,
                 int response_code) override;
  bool OnReceivedMessageFrom(acf_websocket_message_routing_t direction,
                             AcfRefPtr<AcfValue> message) override;
  void OnConnectionClosing() override;

 private:
  LPVOID callback_;
  DWORD connect_id_;
  AcfRefPtr<AcfBrowser> browser_;
  AcfRefPtr<AcfWebSocketClient> ws_;
  IMPLEMENT_REFCOUNTING(WebSocketHandler);
};

}  // namespace wrapper

#endif
