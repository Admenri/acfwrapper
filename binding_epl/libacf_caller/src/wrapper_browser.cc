
#include <iostream>

#include "include/acf_browser.h"
#include "include/acf_environment.h"
#include "include/acf_function_handler.h"
#include "include/acf_web_message_handler.h"

#include "wrapper_utility.h"

extern DWORD acf_cpp_fntable_browser[];
extern DWORD acf_cpp_fntable_new_window_delegate[];
extern DWORD acf_cpp_fntable_environment[];
extern DWORD acf_cpp_fntable_profile[];
extern DWORD acf_cpp_fntable_frame[];
extern DWORD acf_cpp_fntable_value[];
extern DWORD acf_cpp_fntable_dictionary[];
extern DWORD acf_cpp_fntable_webmessageproxy[];
extern DWORD acf_cpp_fntable_cookiemanager[];

namespace wrapper {

namespace {

void ACF_CALLBACK post_web_message(AcfWebMessageProxy* proxy, LPVOID* eArray) {
  LPSTR pData = ((LPSTR)*eArray) + sizeof(INT) * 2;
  DWORD** pList = reinterpret_cast<decltype(pList)>(pData);
  int size = *((LPINT)(((LPSTR)*eArray) + sizeof(INT)));

  std::vector<AcfRefPtr<AcfValue>> args;
  for (size_t i = 0; i < size; i++) {
    DWORD* pClass = pList[i];
    AcfValue* dict = reinterpret_cast<AcfValue*>(pClass[1]);
    args.push_back(dict);
  }

  proxy->PostWebMessage(args);
}

BOOL ACF_CALLBACK is_in_history_cache(AcfWebMessageProxy* proxy) {
  return proxy->IsInBackForwardCache();
}

}  // namespace

DWORD acf_cpp_fntable_webmessageproxy[] = {
    (DWORD)post_web_message,
    (DWORD)is_in_history_cache,
};

namespace {

void ACF_CALLBACK promise_resolve(AcfPromiseResolver* resolver, AcfValue* v) {
  resolver->Resolve(v);
}

}  // namespace

DWORD acf_cpp_fntable_promise[] = {
    (DWORD)promise_resolve,
};

class WebMessageFactory : public AcfWebMessageFactory,
                          public AcfWebMessageHandler {
 public:
  WebMessageFactory(LPVOID callback) : callback_(callback) {}
  ~WebMessageFactory() override {}

 protected:
  AcfRefPtr<AcfWebMessageHandler> CreateHost(
      AcfRefPtr<AcfBrowser> browser,
      const AcfString& origin,
      AcfRefPtr<AcfFrame> frame,
      AcfRefPtr<AcfWebMessageProxy> reply_proxy) override {
    if (frame->IsMain() && this->callback_ != NULL) {
      LPVOID pClass = this->callback_;
      LPCSTR pOrigin = GetEString(origin);
      LPVOID ppOrigin = &pOrigin;
      browser->AddRef();
      reply_proxy->AddRef();
      IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                    acf_cpp_fntable_browser);
      IMP_NEWECLASS(TempProxy, reply_proxy.get(),
                    eClass::m_pVfTable_WebMessageReplyProxy,
                    acf_cpp_fntable_webmessageproxy);
      __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push TempProxy;
      push ppOrigin;
			push TempBrowser;
			push ecx;
			call[edx + 0x08];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
      }
      reply_proxy->Release();
      browser->Release();
    }

    return this;
  }

  void OnPostMessage(const WebMessage& message) override {
    if (this->callback_ != NULL) {
      LPVOID pClass = this->callback_;

      DWORD* pStrs = new DWORD[message.size()];
      for (size_t i = 0; i < message.size(); i++) {
        PECLASS_VFPTR pNewClass = new ECLASS_VFPTR;
        pNewClass->dwVfTable = eClass::m_pVfTable_Value;
        pNewClass->pCFuncs = acf_cpp_fntable_value;
        pNewClass->pObject = ((LPVOID)(message[i].get()));

        pStrs[i] = (DWORD)pNewClass;
      }

      int nSize = message.size() * sizeof(DWORD);
      LPSTR pAry =
          (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
      *(LPINT)pAry = 1;
      *(LPINT)(pAry + sizeof(INT)) = message.size();
      memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
      delete[] pStrs;

      LPVOID pList = &pAry;

      __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push pList;
			push ecx;
			call[edx + 0x0C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
      }
    }
  }

  void OnBackForwardCacheStateChanged() override {
    if (this->callback_ != NULL) {
      LPVOID pClass = this->callback_;
      __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
			push ecx;
			call[edx + 0x10];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
      }
    }
  }

 private:
  LPVOID callback_;
  IMPLEMENT_REFCOUNTING(WebMessageFactory);
};

class FunctionRegisterFactory : public AcfFunctionFactory,
                                public AcfFunctionHandler {
 public:
  FunctionRegisterFactory(LPVOID callback) : callback_(callback) {}
  ~FunctionRegisterFactory() override {}

 protected:
  AcfRefPtr<AcfFunctionHandler> CreateHost(AcfRefPtr<AcfBrowser> browser,
                                           const AcfString& origin,
                                           AcfRefPtr<AcfFrame> frame) override {
    if (this->callback_ != NULL) {
      LPVOID pClass = this->callback_;
      LPCSTR pOrigin = GetEString(origin);
      LPVOID ppOrigin = &pOrigin;
      browser->AddRef();
      IMP_NEWECLASS(TempBrowser, browser.get(), eClass::m_pVfTable_Browser,
                    acf_cpp_fntable_browser);
      IMP_NEWECLASS(TempFrame, frame.get(), eClass::m_pVfTable_Frame,
                    acf_cpp_fntable_frame);
      __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push TempFrame;
      push ppOrigin;
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

    return this;
  }

  void OnExecuteFunction(const ArgumentsList& arguments,
                         AcfRefPtr<AcfPromiseResolver> promise) override {
    if (this->callback_ != NULL) {
      LPVOID pClass = this->callback_;

      DWORD* pStrs = new DWORD[arguments.size()];
      for (size_t i = 0; i < arguments.size(); i++) {
        PECLASS_VFPTR pNewClass = new ECLASS_VFPTR;
        pNewClass->dwVfTable = eClass::m_pVfTable_Value;
        pNewClass->pCFuncs = acf_cpp_fntable_value;
        pNewClass->pObject = ((LPVOID)(arguments[i].get()));

        pStrs[i] = (DWORD)pNewClass;
      }

      int nSize = arguments.size() * sizeof(DWORD);
      LPSTR pAry =
          (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
      *(LPINT)pAry = 1;
      *(LPINT)(pAry + sizeof(INT)) = arguments.size();
      memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
      delete[] pStrs;

      LPVOID pList = &pAry;

      IMP_NEWECLASS(TempResolver, promise.get(), eClass::m_pVfTable_Promise,
                    acf_cpp_fntable_promise);

      __asm {
			push ecx;
			push ebx;
			push edi;
			push esi;
			mov ebx, pClass;
			mov edx, [ebx];
			lea ecx, pClass;
      push TempResolver;
      push pList;
			push ecx;
			call[edx + 0x0C];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
      }
    }
  }

 private:
  LPVOID callback_;
  IMPLEMENT_REFCOUNTING(FunctionRegisterFactory);
};

}  // namespace wrapper

namespace {

BOOL ACF_CALLBACK is_same(AcfBrowser* obj, AcfBrowser* that) {
  return obj->IsSame(that);
}

BOOL ACF_CALLBACK is_valid(AcfBrowser* obj) {
  return obj->IsValid();
}

BOOL ACF_CALLBACK get_env(AcfBrowser* obj, DWORD* retObj) {
  AcfRefPtr<AcfEnvironment> env = obj->GetEnvironment();

  if (env) {
    env->AddRef();
    retObj[1] = (DWORD)env.get();
    retObj[2] = (DWORD)acf_cpp_fntable_environment;
  }

  return !!env.get();
}

BOOL ACF_CALLBACK get_profile(AcfBrowser* obj, DWORD* retObj) {
  AcfRefPtr<AcfProfile> env = obj->GetProfile();

  if (env) {
    env->AddRef();
    retObj[1] = (DWORD)env.get();
    retObj[2] = (DWORD)acf_cpp_fntable_profile;
  }

  return !!env.get();
}

HWND ACF_CALLBACK get_hwnd(AcfBrowser* obj) {
  return obj->GetWindowHandle();
}

void ACF_CALLBACK close(AcfBrowser* obj, BOOL force) {
  obj->Close(force);
}

BOOL ACF_CALLBACK can_goback(AcfBrowser* obj) {
  return obj->CanGoBack();
}

BOOL ACF_CALLBACK can_goforward(AcfBrowser* obj) {
  return obj->CanGoForward();
}

void ACF_CALLBACK goback(AcfBrowser* obj) {
  obj->GoBack();
}

void ACF_CALLBACK goforward(AcfBrowser* obj) {
  obj->GoForward();
}

void ACF_CALLBACK reload(AcfBrowser* obj, BOOL cache) {
  obj->Reload(cache);
}

void ACF_CALLBACK stop(AcfBrowser* obj) {
  obj->Stop();
}

BOOL ACF_CALLBACK isloading(AcfBrowser* obj) {
  return obj->IsLoading();
}

void ACF_CALLBACK load_url(AcfBrowser* obj,
                           LPCSTR url,
                           LPCSTR referrer,
                           acf_referrer_policy_t referrer_policy,
                           acf_transition_type_t tid,
                           LPCSTR extra_header) {
  return obj->LoadURL(url, referrer, referrer_policy, tid, extra_header);
}

LPSTR ACF_CALLBACK get_url(AcfBrowser* obj) {
  return GetEString(obj->GetURL());
}

LPSTR ACF_CALLBACK get_title(AcfBrowser* obj) {
  return GetEString(obj->GetTitle());
}

void ACF_CALLBACK focus(AcfBrowser* obj) {
  obj->Focus();
}

void ACF_CALLBACK set_visible(AcfBrowser* obj, BOOL visible) {
  obj->SetVisible(visible);
}

BOOL ACF_CALLBACK get_visible(AcfBrowser* obj) {
  return obj->GetVisible();
}

BOOL ACF_CALLBACK get_user_data(AcfBrowser* obj, DWORD* retObj) {
  AcfRefPtr<AcfDictionaryValue> data = obj->GetExtraInfo();

  if (retObj && data) {
    data->AddRef();
    retObj[1] = (DWORD)data.get();
    retObj[2] = (DWORD)acf_cpp_fntable_dictionary;
  }

  return !!retObj;
}

int ACF_CALLBACK get_frame_count(AcfBrowser* obj) {
  return obj->GetFrameCount();
}

BOOL ACF_CALLBACK get_frame_identifiers(AcfBrowser* obj, LPVOID* eArray) {
  ISVALIDR(obj, NULL);

  std::vector<int64> data;

  obj->GetFrameIdentifiers(data);

  FreeAryElement(*eArray);

  int nSize = data.size() * sizeof(int64);
  LPSTR pAry = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
  *(LPINT)pAry = 1;
  *(LPINT)(pAry + sizeof(INT)) = data.size();
  memcpy(pAry + sizeof(INT) * 2, data.data(), nSize);

  *eArray = pAry;

  return !!eArray;
}

BOOL ACF_CALLBACK get_frame_names(AcfBrowser* obj, LPVOID* eArray) {
  ISVALIDR(obj, NULL);

  std::vector<AcfString> keys;
  obj->GetFrameNames(keys);

  LPSTR* pStrs = new LPSTR[keys.size()];
  for (size_t i = 0; i < keys.size(); i++) {
    pStrs[i] = GetEString(keys[i].c_str());
  }

  FreeAryElement(*eArray);

  int nSize = keys.size() * sizeof(DWORD);
  LPSTR pAry = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
  *(LPINT)pAry = 1;
  *(LPINT)(pAry + sizeof(INT)) = keys.size();
  memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
  delete[] pStrs;

  *eArray = pAry;

  return 0;
}

BOOL ACF_CALLBACK get_frame_by_name(AcfBrowser* obj,
                                    LPCSTR name,
                                    DWORD* retObj) {
  AcfRefPtr<AcfFrame> frame = obj->GetFrame(name);

  if (frame) {
    frame->AddRef();
    retObj[1] = (DWORD)frame.get();
    retObj[2] = (DWORD)acf_cpp_fntable_frame;
  }

  return !!frame.get();
}

BOOL ACF_CALLBACK get_frame_by_id(AcfBrowser* obj, int64_t* id, DWORD* retObj) {
  AcfRefPtr<AcfFrame> frame = obj->GetFrame(*id);

  if (frame) {
    frame->AddRef();
    retObj[1] = (DWORD)frame.get();
    retObj[2] = (DWORD)acf_cpp_fntable_frame;
  }

  return !!frame.get();
}

BOOL ACF_CALLBACK get_main_frame(AcfBrowser* obj, DWORD* retObj) {
  AcfRefPtr<AcfFrame> frame = obj->GetMainFrame();

  if (frame) {
    frame->AddRef();
    retObj[1] = (DWORD)frame.get();
    retObj[2] = (DWORD)acf_cpp_fntable_frame;
  }

  return !!frame.get();
}

void ACF_CALLBACK set_browser_settings(AcfBrowser* obj,
                                       acf_browser_settings_t* s) {
  obj->SetSettings(*s);
}

void ACF_CALLBACK key_event(AcfBrowser* obj, acf_key_event_t** event) {
  obj->SendKeyEvent(**event);
}

void ACF_CALLBACK mouse_click_event(AcfBrowser* obj,
                                    acf_mouse_event_t** event,
                                    acf_mouse_button_type_t type,
                                    BOOL up,
                                    int count) {
  obj->SendMouseClickEvent(**event, type, up, count);
}

void ACF_CALLBACK mouse_move_event(AcfBrowser* obj,
                                   acf_mouse_event_t** event,
                                   BOOL leave) {
  obj->SendMouseMoveEvent(**event, leave);
}

void ACF_CALLBACK mouse_wheel_event(AcfBrowser* obj,
                                    acf_mouse_event_t** event,
                                    int x,
                                    int y) {
  obj->SendMouseWheelEvent(**event, x, y);
}

void ACF_CALLBACK mute_audio(AcfBrowser* obj, BOOL mute) {
  obj->SetAudioMuted(mute);
}

BOOL ACF_CALLBACK get_mute_audio(AcfBrowser* obj) {
  return obj->IsAudioMuted();
}

void ACF_CALLBACK toggle_devtools(AcfBrowser* obj) {
  obj->ToggleDevtools();
}

void ACF_CALLBACK toggle_taskmanager(AcfBrowser* obj) {
  obj->OpenTaskManager();
}

void ACF_CALLBACK toggle_findbar(AcfBrowser* obj) {
  obj->RaiseFindBar();
}

void ACF_CALLBACK zoom_page(AcfBrowser* obj, acf_zoom_type_t type) {
  obj->ZoomPage(type);
}

int ACF_CALLBACK add_document_script(AcfBrowser* obj,
                                     LPCSTR script,
                                     LPCSTR match) {
  return obj->AddDocumentStartJavaScript(script, {match});
}

void ACF_CALLBACK add_webmessage_factory(AcfBrowser* obj,
                                         LPCSTR object_name,
                                         LPVOID callback) {
  obj->AddWebMessageHostFactory(new wrapper::WebMessageFactory(callback),
                                object_name, {"*"});
}

void ACF_CALLBACK send_devtool_message(AcfBrowser* obj, LPCSTR msg) {
  AcfString str(msg);
  obj->SendDevToolsMessage(str.c_str(), str.size());
}

void ACF_CALLBACK toggle_extensions_menu(AcfBrowser* obj) {
  obj->ToggleExtensionsMenu();
}

class CaptureCallback : public AcfCompleteValueHandler {
 public:
  CaptureCallback(std::atomic<BOOL>* notify) : notify_(notify) {}
  ~CaptureCallback() {}

  AcfRefPtr<AcfValue> GetResult() { return result_; }

 protected:
  void OnComplete(AcfRefPtr<AcfValue> value) override {
    result_ = value;
    if (notify_)
      *notify_ = true;
  }

 private:
  std::atomic<BOOL>* notify_;
  AcfRefPtr<AcfValue> result_;

  IMPLEMENT_REFCOUNTING(CaptureCallback);
};

void ACF_CALLBACK capture_snapshot(AcfBrowser* obj,
                                   BOOL fencode,
                                   BOOL surface,
                                   DWORD* retObj) {
  std::unique_ptr<std::atomic<BOOL>> notify =
      std::make_unique<std::atomic<BOOL>>(false);
  AcfRefPtr<CaptureCallback> lpHandler = new CaptureCallback(notify.get());
  obj->CaptureScreenshot(fencode, surface, lpHandler);

  while (!*notify) {
    if (IsOnUIThread()) {
      MSG uiMsg;
      if (PeekMessage(&uiMsg, NULL, NULL, NULL, PM_REMOVE)) {
        TranslateMessage(&uiMsg);
        DispatchMessage(&uiMsg);
      }
    } else {
      ::Sleep(1);
    }
  }

  AcfRefPtr<AcfValue> value = lpHandler->GetResult();

  if (value) {
    value->AddRef();
    retObj[1] = (DWORD)value.get();
    retObj[2] = (DWORD)acf_cpp_fntable_value;
  }
}

void ACF_CALLBACK set_context_profile(AcfBrowser* obj, AcfValue* profile) {
  obj->SetContextProfile(profile);
}

LPCSTR ACF_CALLBACK get_favicon(AcfBrowser* obj) {
  return GetEString(obj->GetFavicon());
}

void ACF_CALLBACK resume_loading_browser(AcfBrowser* obj) {
  obj->ResumeLoadingBrowser();
}

void ACF_CALLBACK add_register_function_factory(AcfBrowser* obj,
                                                LPCSTR object_name,
                                                LPCSTR func_name,
                                                LPVOID callback) {
  obj->AddFunctionRegisterHostFactory(
      new wrapper::FunctionRegisterFactory(callback), object_name, func_name,
      {"*"});
}

void ACF_CALLBACK remove_document_script(AcfBrowser* obj, int script_id) {
  obj->RemoveDocumentStartJavaScript(script_id);
}

/// <summary>
/// New Window Delagate
/// </summary>
/// <param name="obj"></param>
/// <returns></returns>

BOOL ACF_CALLBACK set_new_browser(AcfNewWindowDelegate* obj,
                                  AcfBrowser* browser) {
  return obj->SetNewBrowser(browser);
}

LPSTR ACF_CALLBACK get_target_url(AcfNewWindowDelegate* obj) {
  return GetEString(obj->GetTargetURL());
}

void ACF_CALLBACK handle_new_window(AcfNewWindowDelegate* obj,
                                    BOOL handle,
                                    BOOL suspend) {
  obj->HandleRequest(handle, suspend);
}

}  // namespace

DWORD acf_cpp_fntable_browser[] = {
    (DWORD)is_same,
    (DWORD)is_valid,
    (DWORD)get_env,
    (DWORD)get_profile,
    (DWORD)get_hwnd,
    (DWORD)close,
    (DWORD)can_goback,
    (DWORD)can_goforward,
    (DWORD)goback,
    (DWORD)goforward,
    (DWORD)reload,
    (DWORD)stop,
    (DWORD)isloading,
    (DWORD)load_url,
    (DWORD)get_url,
    (DWORD)get_title,
    (DWORD)focus,
    (DWORD)set_visible,
    (DWORD)get_visible,
    (DWORD)get_user_data,
    (DWORD)get_frame_count,
    (DWORD)get_frame_identifiers,
    (DWORD)get_frame_names,
    (DWORD)get_frame_by_id,
    (DWORD)get_frame_by_name,
    (DWORD)get_main_frame,
    (DWORD)set_browser_settings,
    (DWORD)key_event,
    (DWORD)mouse_click_event,
    (DWORD)mouse_move_event,
    (DWORD)mouse_wheel_event,
    (DWORD)mute_audio,
    (DWORD)get_mute_audio,
    (DWORD)toggle_devtools,
    (DWORD)toggle_taskmanager,
    (DWORD)toggle_findbar,
    (DWORD)zoom_page,
    (DWORD)add_document_script,
    (DWORD)add_webmessage_factory,
    (DWORD)send_devtool_message,
    (DWORD)toggle_extensions_menu,
    (DWORD)capture_snapshot,
    (DWORD)set_context_profile,
    (DWORD)get_favicon,
    (DWORD)resume_loading_browser,
    (DWORD)add_register_function_factory,
    (DWORD)remove_document_script,
};

DWORD acf_cpp_fntable_new_window_delegate[] = {
    (DWORD)set_new_browser,
    (DWORD)get_target_url,
    (DWORD)handle_new_window,
};

namespace {

BOOL ACF_CALLBACK frame_is_valid(AcfFrame* obj) {
  return obj->IsValid();
}

BOOL ACF_CALLBACK frame_is_same(AcfFrame* obj, AcfFrame* that) {
  return obj->IsSame(that);
}

BOOL ACF_CALLBACK frame_get_parent(AcfFrame* obj, DWORD* retObj) {
  AcfRefPtr<AcfFrame> frame = obj->GetParent();

  if (frame) {
    frame->AddRef();
    retObj[1] = (DWORD)frame.get();
    retObj[2] = (DWORD)acf_cpp_fntable_frame;
  }

  return !!frame.get();
}

BOOL ACF_CALLBACK frame_get_browser(AcfFrame* obj, DWORD* retObj) {
  AcfRefPtr<AcfBrowser> frame = obj->GetBrowser();

  if (frame) {
    frame->AddRef();
    retObj[1] = (DWORD)frame.get();
    retObj[2] = (DWORD)acf_cpp_fntable_browser;
  }

  return !!frame.get();
}

LPSTR ACF_CALLBACK frame_get_url(AcfFrame* obj) {
  return GetEString(obj->GetURL());
}

void ACF_CALLBACK frame_get_id(AcfFrame* obj, int64_t* ret) {
  *ret = obj->GetIdentifier();
}

BOOL ACF_CALLBACK frame_is_main(AcfFrame* obj) {
  return obj->IsMain();
}

class ExecJsCallback : public AcfExecuteJavascriptCallback {
 public:
  ExecJsCallback() : notify_(false) {}
  ~ExecJsCallback() {}

  AcfRefPtr<AcfValue> GetResult() { return result_; }
  std::atomic<BOOL>& GetAtomic() { return notify_; }

 protected:
  void OnExecuteResult(AcfRefPtr<AcfValue> value) override {
    result_ = value;
    notify_ = true;
  }

 private:
  std::atomic<BOOL> notify_;
  AcfRefPtr<AcfValue> result_;

  IMPLEMENT_REFCOUNTING(ExecJsCallback);
};

BOOL ACF_CALLBACK frame_execute_javascript(AcfFrame* obj,
                                           LPCSTR script,
                                           LPCSTR url,
                                           DWORD* retObj) {
  AcfRefPtr<ExecJsCallback> lpHandler = nullptr;
  if (retObj)
    lpHandler = new ExecJsCallback();

  obj->ExecuteJavascript(script, url, lpHandler);

  if (lpHandler) {
    while (!lpHandler->GetAtomic()) {
      if (IsOnUIThread()) {
        MSG uiMsg;
        if (PeekMessage(&uiMsg, NULL, NULL, NULL, PM_REMOVE)) {
          TranslateMessage(&uiMsg);
          DispatchMessage(&uiMsg);
        }
      } else {
        ::Sleep(10);
      }
    }

    AcfRefPtr<AcfValue> value = lpHandler->GetResult();
    if (value) {
      value->AddRef();
      retObj[1] = (DWORD)value.get();
      retObj[2] = (DWORD)acf_cpp_fntable_value;
    }

    return !!value.get();
  }

  return TRUE;
}

class ExecJsCallbackAsync : public AcfExecuteJavascriptCallback {
 public:
  ExecJsCallbackAsync(LPVOID lpCallback) : callback_(lpCallback) {}
  ~ExecJsCallbackAsync() {}

 protected:
  void OnExecuteResult(AcfRefPtr<AcfValue> value) override {
    if (this->callback_ != NULL) {
      LPVOID pClass = this->callback_;
      value->AddRef();
      IMP_NEWECLASS(TempValue, value.get(), eClass::m_pVfTable_Value,
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
			push ecx;
			call[edx + 0x08];
			pop esi;
			pop edi;
			pop ebx;
			pop ecx;
      }
      value->Release();
    }
  }

 private:
  LPVOID callback_;

  IMPLEMENT_REFCOUNTING(ExecJsCallbackAsync);
};

BOOL ACF_CALLBACK frame_execute_javascript_async(AcfFrame* obj,
                                                 LPCSTR script,
                                                 LPCSTR url,
                                                 LPVOID callback) {
  AcfRefPtr<ExecJsCallbackAsync> lpHandler = new ExecJsCallbackAsync(callback);
  obj->ExecuteJavascript(script, url, lpHandler);
  return !!callback;
}

class GetTextSync : public AcfStringVisitor {
 public:
  GetTextSync(std::atomic<BOOL>* notify) : notify_(notify) {}
  ~GetTextSync() {}

  std::string GetResult() { return result_; }

 protected:
  void Visit(const AcfString& string) override {
    result_ = string;
    *notify_ = true;
  }

 private:
  std::string result_;
  std::atomic<BOOL>* notify_;

  IMPLEMENT_REFCOUNTING(GetTextSync);
};

void ACF_CALLBACK frame_get_source(AcfFrame* obj, LPSTR* retStr) {
  std::unique_ptr<std::atomic<BOOL>> notify =
      std::make_unique<std::atomic<BOOL>>(false);
  AcfRefPtr<GetTextSync> lpHandler = new GetTextSync(notify.get());
  obj->GetSource(lpHandler);

  while (!*notify) {
    if (IsOnUIThread()) {
      MSG uiMsg;
      if (PeekMessage(&uiMsg, NULL, NULL, NULL, PM_REMOVE)) {
        TranslateMessage(&uiMsg);
        DispatchMessage(&uiMsg);
      }
    } else {
      ::Sleep(1);
    }
  }

  *retStr = GetEString(lpHandler->GetResult());
}

}  // namespace

DWORD acf_cpp_fntable_frame[] = {
    (DWORD)frame_is_valid,   (DWORD)frame_is_same,
    (DWORD)frame_get_parent, (DWORD)frame_get_browser,
    (DWORD)frame_get_url,    (DWORD)frame_get_id,
    (DWORD)frame_is_main,    (DWORD)frame_execute_javascript,
    (DWORD)frame_get_source, (DWORD)frame_execute_javascript_async,
};
