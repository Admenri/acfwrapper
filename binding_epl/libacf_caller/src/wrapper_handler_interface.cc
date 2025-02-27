
#include "wrapper_handler_interface.h"

#include "struct_class.h"
#include "wrapper_acfclass.h"
#include "wrapper_websocket.h"

extern DWORD acf_cpp_fntable_browser[];
extern DWORD acf_cpp_fntable_new_window_delegate[];
extern DWORD acf_cpp_fntable_environment[];
extern DWORD acf_cpp_fntable_profile[];
extern DWORD acf_cpp_fntable_frame[];
extern DWORD acf_cpp_fntable_value[];
extern DWORD acf_cpp_fntable_authcallback[];
extern DWORD acf_cpp_fntable_menucallback[];
extern DWORD acf_cpp_fntable_menuparams[];
extern DWORD acf_cpp_fntable_menumodel[];
extern DWORD acf_cpp_fntable_request[];
extern DWORD acf_cpp_fntable_response[];
extern DWORD acf_cpp_fntable_responsecallback[];
extern DWORD acf_cpp_fntable_permissionprompt[];
extern DWORD acf_cpp_fntable_jsdialogcallback[];
extern DWORD acf_cpp_fntable_filedialogcallback[];

namespace wrapper {

EnvironmentHandler::EnvironmentHandler(LPVOID callback) : callback_(callback) {}

EnvironmentHandler::~EnvironmentHandler() {}

void EnvironmentHandler::OnProcessInitialized(AcfRefPtr<AcfEnvironment> env,
                                              bool success) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    env->AddRef();
    IMP_NEWECLASS(TempEnv, env.get(), eClass::m_pVfTable_Environment,
                  acf_cpp_fntable_environment);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			movzx eax, success; 
			push eax;
			push TempEnv;
			push ecx;
			call[edx + 0x08];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    env->Release();
  }
}

AcfRefPtr<AcfResourceRequestHandler>
EnvironmentHandler::GetResourceRequestHandler(
    AcfRefPtr<AcfProfile> profile,
    int64 frame_id,
    AcfRefPtr<AcfRequest> request,
    acf_url_loader_factory_type_t type,
    const AcfString& request_initiator,
    bool& block_request) {
  BOOL bRetVal = false;
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;

    profile->AddRef();
    request->AddRef();

    IMP_NEWECLASS(TempProfile, profile.get(), eClass::m_pVfTable_Profile,
                  acf_cpp_fntable_profile);
    IMP_NEWECLASS(TempRequest, request.get(), eClass::m_pVfTable_Request,
                  acf_cpp_fntable_request);
    int* pFID = (int*)&frame_id;
    int pFID_1 = *pFID;
    int pFID_2 = *(pFID + 1);

    LPSTR pinitiator = GetEString(request_initiator);
    LPVOID lpInitiator = &pinitiator;

    bool* pblock = &block_request;
    int nType = type;
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push pblock;
			push lpInitiator;
			push nType;
			push TempRequest;
			push pFID_2;
			push pFID_1;
			push TempProfile;
			push ecx;
			call[edx + 0x0C];
			mov bRetVal, eax;
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    profile->Release();
    request->Release();
  }
  return bRetVal ? new ResourceRequestHandler(callback_, profile, frame_id)
                 : nullptr;
}

void EnvironmentHandler::OnProcessNavigateRequest(AcfRefPtr<AcfProfile> profile,
                                                  const AcfString& url,
                                                  const AcfString& referrer,
                                                  bool user_gesture) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    profile->AddRef();
    IMP_NEWECLASS(TempProfile, profile.get(), eClass::m_pVfTable_Profile,
                  acf_cpp_fntable_profile);

    LPCSTR pURL = GetEString(url), pREF = GetEString(referrer);
    LPVOID ppURL = &pURL, ppREF = &pREF;

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      movzx eax, user_gesture;
			push eax;
      push ppREF;
      push ppURL;
			push TempProfile;
			push ecx;
			call[edx + 0x20];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    profile->Release();
  }
}

ResourceRequestHandler::ResourceRequestHandler(LPVOID callback,
                                               AcfRefPtr<AcfProfile> profile,
                                               int64 frame_id)
    : callback_(callback), profile_(profile), frame_id_(frame_id) {}

AcfRefPtr<AcfResourceHandler> ResourceRequestHandler::OnBeforeResourceLoad(
    AcfRefPtr<AcfRequest> request) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    request->AddRef();
    profile_->AddRef();
    IMP_NEWECLASS(TempProfile, profile_.get(), eClass::m_pVfTable_Profile,
                  acf_cpp_fntable_profile);
    int* pFID = (int*)&frame_id_;
    int pFID_1 = *pFID;
    int pFID_2 = *(pFID + 1);

    IMP_NEWECLASS(TempRequest, request.get(), eClass::m_pVfTable_Request,
                  acf_cpp_fntable_request);
    AcfRefPtr<ACFResourceResponseCallback> callback =
        new ACFResourceResponseCallback();
    IMP_NEWECLASS(TempCallback, callback.get(),
                  eClass::m_pVfTable_ResponseCallback,
                  acf_cpp_fntable_responsecallback);

    callback->AddRef();

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempCallback;
			push TempRequest;
      push pFID_2;
      push pFID_1;
      push TempProfile;
			push ecx;
			call[edx + 0x10];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    request->Release();
    profile_->Release();

    callback->Release();
    return callback->GetResponse();
  }
  return nullptr;
}

AcfRefPtr<AcfResponseFilter> ResourceRequestHandler::OnResourceResponse(
    AcfRefPtr<AcfRequest> request,
    AcfRefPtr<AcfResponse> response,
    int& mode) {
  BOOL bRetVal = FALSE;
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    request->AddRef();
    response->AddRef();
    profile_->AddRef();

    IMP_NEWECLASS(TempProfile, profile_.get(), eClass::m_pVfTable_Profile,
                  acf_cpp_fntable_profile);
    int* pFID = (int*)&frame_id_;
    int pFID_1 = *pFID;
    int pFID_2 = *(pFID + 1);

    IMP_NEWECLASS(TempRequest, request.get(), eClass::m_pVfTable_Request,
                  acf_cpp_fntable_request);
    IMP_NEWECLASS(TempResponse, response.get(), eClass::m_pVfTable_Response,
                  acf_cpp_fntable_response);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempResponse;
			push TempRequest;
      push pFID_2;
      push pFID_1;
      push TempProfile;
			push ecx;
			call[edx + 0x14];
			mov bRetVal, eax;
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    request->Release();
    response->Release();
    profile_->Release();
  }
  return bRetVal ? new ACFResourceFilterHandler(callback_, profile_, frame_id_,
                                                request, response)
                 : nullptr;
}

void ResourceRequestHandler::OnResourceLoadComplete(
    AcfRefPtr<AcfRequest> request,
    URLRequestStatus status,
    int64 received_content_length) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    request->AddRef();
    profile_->AddRef();

    IMP_NEWECLASS(TempProfile, profile_.get(), eClass::m_pVfTable_Profile,
                  acf_cpp_fntable_profile);
    int* pFID = (int*)&frame_id_;
    int pFID_1 = *pFID;
    int pFID_2 = *(pFID + 1);

    IMP_NEWECLASS(TempRequest, request.get(), eClass::m_pVfTable_Request,
                  acf_cpp_fntable_request);
    int* pLEN = (int*)&received_content_length;
    int pLEN_1 = *pLEN;
    int pLEN_2 = *(pLEN + 1);

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push pLEN_2;
			push pLEN_1;
			push status;
			push TempRequest;
      push pFID_2;
      push pFID_1;
      push TempProfile;
			push ecx;
			call[edx + 0x1C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    request->Release();
    profile_->Release();
  }
}

BrowserHandler::BrowserHandler(LPVOID callback) : callback_(callback) {}

BrowserHandler::~BrowserHandler() {}

void BrowserHandler::OnBrowserCreated(AcfRefPtr<AcfBrowser> browser) {
  devtool_observer_ =
      browser->AddDevToolsMessageObserver(new DevtoolObserver(callback_));

  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempBrowser;
			push ecx;
			call[edx + 0x08];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnNewWindowRequest(
    AcfRefPtr<AcfBrowser> browser,
    NewWindowSource source,
    NewWindowDisposition disposition,
    const AcfPopupFeatures& popup_features,
    bool user_gesture,
    AcfRefPtr<AcfNewWindowDelegate> delegate) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    delegate->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempDelegate, delegate.get(),
                  eClass::m_pVfTable_NewWindowDelegate,
                  acf_cpp_fntable_new_window_delegate);

    acf_popup_features_t popup_info = popup_features;
    auto* p_popup_info = &popup_info;
    auto* pp_popup = &p_popup_info;

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempDelegate;
			movzx eax, user_gesture;
			push eax;
      push pp_popup;
			push disposition;
			push source;
			push TempBrowser;
			push ecx;
			call[edx + 0x0C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    delegate->Release();
    browser->Release();
  }
}

void BrowserHandler::OnBrowserDestroyed(AcfRefPtr<AcfBrowser> browser) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempBrowser;
			push ecx;
			call[edx + 0x10];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnLoadingStateChanged(AcfRefPtr<AcfBrowser> browser,
                                           bool show_loading_ui) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			movzx eax, show_loading_ui;
			push eax;
			push TempBrowser;
			push ecx;
			call[edx + 0x14];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnNavigationStateChanged(AcfRefPtr<AcfBrowser> browser,
                                              NavigationTypes flags) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push flags;
			push TempBrowser;
			push ecx;
			call[edx + 0x18];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnFullscreenStateChanged(AcfRefPtr<AcfBrowser> browser,
                                              bool fullscreen) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			movzx eax, fullscreen;
			push eax;
			push TempBrowser;
			push ecx;
			call[edx + 0x1C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnAuthLoginRequest(AcfRefPtr<AcfBrowser> browser,
                                        bool is_proxy,
                                        const AcfString& host,
                                        const AcfString& url,
                                        const AcfString& scheme,
                                        const AcfString& realm,
                                        const AcfString& challenge,
                                        bool is_main_frame,
                                        AcfRefPtr<AcfLoginDelegate> delegate) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempCallback, delegate.get(), eClass::m_pVfTable_AuthCallback,
                  acf_cpp_fntable_authcallback);

    LPCSTR pHost = GetEString(host), pUrl = GetEString(url),
           pScheme = GetEString(scheme), pRealm = GetEString(realm),
           pChallenge = GetEString(challenge);

    LPVOID ppHost = &pHost, ppURL = &pUrl, ppScheme = &pScheme,
           ppRealm = &pRealm, ppChallenge = &pChallenge;

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempCallback;
			movzx eax, is_main_frame;
			push eax;
			push ppChallenge;
			push ppRealm;
			push ppScheme;
			push ppURL;
      push ppHost;
			movzx eax, is_proxy;
			push eax;
			push TempBrowser;
			push ecx;
			call[edx + 0x20];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnContextMenuRequest(
    AcfRefPtr<AcfBrowser> browser,
    AcfRefPtr<AcfContextMenuParams> menu_params,
    AcfRefPtr<AcfContextMenuModel> menu_model,
    AcfRefPtr<AcfContextMenuCallback> callback) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    menu_params->AddRef();
    menu_model->AddRef();
    callback->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempParams, menu_params.get(), eClass::m_pVfTable_MenuParams,
                  acf_cpp_fntable_menuparams);
    IMP_NEWECLASS(TempModel, menu_model.get(), eClass::m_pVfTable_MenuModel,
                  acf_cpp_fntable_menumodel);
    IMP_NEWECLASS(TempCallback, callback.get(), eClass::m_pVfTable_MenuCallback,
                  acf_cpp_fntable_menucallback);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempCallback;
			push TempModel;
			push TempParams;
			push TempBrowser;
			push ecx;
			call[edx + 0x24];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
    menu_params->Release();
    menu_model->Release();
    callback->Release();
  }
}

void BrowserHandler::OnContextMenuExecute(
    AcfRefPtr<AcfBrowser> browser,
    AcfRefPtr<AcfContextMenuParams> menu_params,
    int command_id,
    int event_flags) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempParams, menu_params.get(), eClass::m_pVfTable_MenuParams,
                  acf_cpp_fntable_menuparams);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push event_flags;
			push command_id;
			push TempParams;
			push TempBrowser;
			push ecx;
			call[edx + 0x28];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnConsoleMessage(AcfRefPtr<AcfBrowser> browser,
                                      int level,
                                      const AcfString& message,
                                      const AcfString& source,
                                      int line,
                                      const AcfString& trace) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);

    LPCSTR pMsg = GetEString(message);
    LPVOID ppMsg = &pMsg;

    LPCSTR pSrc = GetEString(source);
    LPVOID ppSrc = &pSrc;

    LPCSTR pTrace = GetEString(trace);
    LPVOID ppTrace = &pTrace;

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push ppTrace;
			push line;
			push ppSrc;
			push ppMsg;
			push level;
			push TempBrowser;
			push ecx;
			call[edx + 0x2C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnLoadingProgressChange(AcfRefPtr<AcfBrowser> browser,
                                             double progress) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);

    int* pPro = (int*)&progress;
    int pPro_1 = *pPro;
    int pPro_2 = *(pPro + 1);

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push pPro_2;
			push pPro_1;
			push TempBrowser;
			push ecx;
			call[edx + 0x30];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnAudioStateChange(AcfRefPtr<AcfBrowser> browser,
                                        bool audible) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			movzx eax, audible;
			push eax;
			push TempBrowser;
			push ecx;
			call[edx + 0x34];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnBeforeNavigation(AcfRefPtr<AcfBrowser> browser,
                                        AcfRefPtr<AcfFrame> frame,
                                        const AcfString& url,
                                        const AcfString& referrer,
                                        bool user_gesture,
                                        bool is_redirect,
                                        AcfRefPtr<AcfCallback> callback) {
  BOOL bRetVal = FALSE;
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    frame->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempFrame, frame.get(), eClass::m_pVfTable_Frame,
                  acf_cpp_fntable_frame);
    LPVOID pUrl = GetEString(url);
    LPVOID ppUrl = &pUrl;
    LPVOID pRef = GetEString(referrer);
    LPVOID ppRef = &pRef;

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			movzx eax, is_redirect;
			push eax;
			movzx eax, user_gesture;
			push eax;
      push ppRef;
			push ppUrl;
			push TempFrame;
			push TempBrowser;
			push ecx;
			call[edx + 0x38];
			mov bRetVal, eax;
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
    frame->Release();
  }
  callback->Continue(bRetVal);
}

void BrowserHandler::OnShowPermissionPrompt(
    AcfRefPtr<AcfBrowser> browser,
    uint64 prompt_id,
    const AcfString& requesting_origin,
    uint32 requested_permissions,
    AcfRefPtr<AcfPermissionPromptCallback> callback) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    callback->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempCallback, callback.get(),
                  eClass::m_pVfTable_PermissionPrompt,
                  acf_cpp_fntable_permissionprompt);
    uint32_t pid32 = prompt_id;
    LPCSTR porigin = GetEString(requesting_origin);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempCallback;
      push requested_permissions;
      lea eax, porigin;
      push eax;
			push pid32;
			push TempBrowser;
			push ecx;
			call[edx + 0x3C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
    callback->Release();
  }
}

void BrowserHandler::OnDismissPermissionPrompt(
    AcfRefPtr<AcfBrowser> browser,
    uint64 prompt_id,
    acf_permission_request_result_t result) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    uint32_t pid32 = prompt_id;
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push result;
      push pid32;
			push TempBrowser;
			push ecx;
			call[edx + 0x40];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnJavaScriptDialog(
    AcfRefPtr<AcfBrowser> browser,
    AcfRefPtr<AcfFrame> frame,
    acf_jsdialog_type_t dialog_type,
    const AcfString& message_text,
    const AcfString& default_prompt_text,
    AcfRefPtr<AcfJavaScriptDialogCallback> callback) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    if (frame)
      frame->AddRef();
    callback->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempFrame, frame.get(), eClass::m_pVfTable_Frame,
                  acf_cpp_fntable_frame);
    IMP_NEWECLASS(TempCallback, callback.get(),
                  eClass::m_pVfTable_JSDialogCallback,
                  acf_cpp_fntable_jsdialogcallback);
    LPCSTR pmsg = GetEString(message_text);
    LPCSTR pdefmsg = GetEString(default_prompt_text);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push TempCallback;
      lea eax, pdefmsg;
      push eax;
      lea ebx, pmsg;
      push ebx;
      push dialog_type;
      push TempFrame;
			push TempBrowser;
			push ecx;
			call[edx + 0x44];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
    if (frame)
      frame->Release();
    callback->Release();
  }
}

void BrowserHandler::OnBeforeUnloadDialog(
    AcfRefPtr<AcfBrowser> browser,
    AcfRefPtr<AcfFrame> frame,
    bool is_reload,
    AcfRefPtr<AcfJavaScriptDialogCallback> callback) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    if (frame)
      frame->AddRef();
    callback->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempFrame, frame.get(), eClass::m_pVfTable_Frame,
                  acf_cpp_fntable_frame);
    IMP_NEWECLASS(TempCallback, callback.get(),
                  eClass::m_pVfTable_JSDialogCallback,
                  acf_cpp_fntable_jsdialogcallback);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push TempCallback;
      movzx eax, is_reload;
      push eax;
      push TempFrame;
			push TempBrowser;
			push ecx;
			call[edx + 0x48];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
    if (frame)
      frame->Release();
    callback->Release();
  }
}

void BrowserHandler::OnHandleJavaScriptDialog(
    AcfRefPtr<AcfBrowser> browser,
    bool accept,
    const AcfString& prompt_override) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    LPCSTR poverride = GetEString(prompt_override);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      lea eax, poverride;
      push eax;
      movzx eax, accept;
      push eax;
			push TempBrowser;
			push ecx;
			call[edx + 0x4C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnCancelJavaScriptDialogs(AcfRefPtr<AcfBrowser> browser,
                                               bool reset_state) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      movzx eax, reset_state;
      push eax;
			push TempBrowser;
			push ecx;
			call[edx + 0x50];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::OnFileDialog(AcfRefPtr<AcfBrowser> browser,
                                  acf_file_dialog_mode_t mode,
                                  const AcfString& title,
                                  const AcfString& default_file_path,
                                  const std::vector<AcfString>& accept_filters,
                                  AcfRefPtr<AcfFileDialogCallback> callback) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    callback->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    IMP_NEWECLASS(TempCallback, callback.get(),
                  eClass::m_pVfTable_FileDialogCallback,
                  acf_cpp_fntable_filedialogcallback);

    LPVOID pTitle = GetEString(title);
    LPVOID pPath = GetEString(default_file_path);

    DWORD* pStrs = new DWORD[accept_filters.size()];
    for (size_t i = 0; i < accept_filters.size(); i++) {
      pStrs[i] = (DWORD)GetEString(accept_filters[i]);
    }

    int nSize = accept_filters.size() * sizeof(DWORD);
    LPSTR pAry = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
    *(LPINT)pAry = 1;
    *(LPINT)(pAry + sizeof(INT)) = accept_filters.size();
    memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
    delete[] pStrs;

    LPVOID pList = &pAry;
    LPVOID lpAddr1 = &pTitle;
    LPVOID lpAddr2 = &pPath;

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push TempCallback;
      push pList;
			push lpAddr2;
			push lpAddr1;
			push mode;
			push TempBrowser;
			push ecx;
			call[edx + 0x54];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
    callback->Release();
  }
}

AcfRefPtr<AcfWebSocketClientHandler> BrowserHandler::CreateWebSocket(
    AcfRefPtr<AcfBrowser> browser,
    AcfString& target_url) {
  if (this->callback_ != NULL) {
    BOOL bRetVal = FALSE;
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);

    LPVOID pTitle = GetEString(target_url);
    LPVOID ppTitle = &pTitle;

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push ppTitle;
			push TempBrowser;
			push ecx;
			call[edx + 0x5C];
      mov bRetVal, eax;
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();

    return bRetVal ? new WebSocketHandler(browser, callback_) : nullptr;
  }

  return nullptr;
}

void BrowserHandler::DidStartLoading(AcfRefPtr<AcfBrowser> browser) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempBrowser;
			push ecx;
			call[edx + 0x70];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::DidStopLoading(AcfRefPtr<AcfBrowser> browser) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push TempBrowser;
			push ecx;
			call[edx + 0x74];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::LoadProgressChanged(AcfRefPtr<AcfBrowser> browser,
                                         double progress) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    uint32_t* ptr = (uint32_t*)&progress;
    uint32_t pg1 = *ptr;
    uint32_t pg2 = *(ptr + 1);

    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push pg2;
      push pg1;
			push TempBrowser;
			push ecx;
			call[edx + 0x78];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

void BrowserHandler::DevtoolObserver::OnDevToolsMessage(
    AcfRefPtr<AcfBrowser> browser,
    const void* message,
    size_t message_size) {
  if (this->callback_ != NULL) {
    LPVOID pClass = this->callback_;
    browser->AddRef();
    IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                  acf_cpp_fntable_browser);
    __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push message_size;
      push message;
			push TempBrowser;
			push ecx;
			call[edx + 0x58];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
    }
    browser->Release();
  }
}

}  // namespace wrapper