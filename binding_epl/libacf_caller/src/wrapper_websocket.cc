
#include "wrapper_websocket.h"

#include "include/acf_websocket.h"
#include "wrapper_utility.h"

extern DWORD acf_cpp_fntable_websocketclient[];
extern DWORD acf_cpp_fntable_browser[];
extern DWORD acf_cpp_fntable_value[];

namespace wrapper {

WebSocketHandler::WebSocketHandler(AcfRefPtr<AcfBrowser> browser,
                                   LPVOID callback)
    : browser_(browser),
      callback_(callback),
      connect_id_(reinterpret_cast<DWORD>((LPVOID)this)) {}

WebSocketHandler::~WebSocketHandler() {}

void WebSocketHandler::OnHandshakeResponse(
    AcfRefPtr<AcfWebSocketClient> client) {
  ws_ = client;
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser_->AddRef();
    client->AddRef();
    IMP_NEWECLASS(TempBrowser, browser_.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempWebSocket, client.get(),
                  eClass::m_pVfTable_WebSocketClient,
                  acf_cpp_fntable_websocketclient);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push connect_id_;
      push TempWebSocket;
			push TempBrowser;
			push ecx;
			call[edx + 0x60];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser_->Release();
    client->Release();
  }
}

void WebSocketHandler::OnFailure(const AcfString& message,
                                 int net_err,
                                 int response_code) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser_->AddRef();
    IMP_NEWECLASS(TempBrowser, browser_.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push response_code;
      push net_err;
      push connect_id_;
			push TempBrowser;
			push ecx;
			call[edx + 0x64];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser_->Release();
  }
}

bool WebSocketHandler::OnReceivedMessageFrom(
    acf_websocket_message_routing_t direction,
    AcfRefPtr<AcfValue> message) {
  BOOL bRetVal = FALSE;
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser_->AddRef();
    ws_->AddRef();
    IMP_NEWECLASS(TempBrowser, browser_.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempWS, ws_.get(), eClass::m_pVfTable_WebSocketClient,
                  acf_cpp_fntable_websocketclient);
    IMP_NEWECLASS(TempValue, message.get(), eClass::m_pVfTable_Value,
                  acf_cpp_fntable_value);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push TempValue;
      push direction;
      push connect_id_;
      push TempWS;
			push TempBrowser;
			push ecx;
			call[edx + 0x68];
			mov bRetVal, eax;
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser_->Release();
    ws_->Release();
  }
  return bRetVal;
}

void WebSocketHandler::OnConnectionClosing() {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser_->AddRef();
    IMP_NEWECLASS(TempBrowser, browser_.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push connect_id_;
      push TempBrowser;
			push ecx;
			call[edx + 0x6C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }

    browser_->Release();
  }
}

}  // namespace wrapper

namespace {

BOOL ACF_CALLBACK is_valid(AcfWebSocketClient* obj) {
  return obj->IsValid();
}

void ACF_CALLBACK send_message(AcfWebSocketClient* obj,
                               acf_websocket_message_routing_t dir,
                               AcfValue* v) {
  obj->SendMessageTo(dir, v);
}

void ACF_CALLBACK send_closing_request(AcfWebSocketClient* obj,
                                       acf_websocket_message_routing_t dir) {
  obj->SendCloseConnectionRequest(dir);
}

}  // namespace

DWORD acf_cpp_fntable_websocketclient[] = {
    (DWORD)is_valid,
    (DWORD)send_message,
    (DWORD)send_closing_request,
};
