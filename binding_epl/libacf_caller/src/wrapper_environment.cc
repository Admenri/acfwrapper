
#include <windows.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

#include "include/acf_environment.h"

#include "message_pump.h"
#include "wrapper_handler_interface.h"
#include "wrapper_urlrequest.h"
#include "wrapper_utility.h"

namespace {

/// <summary>
/// Global UI Thread message pump
/// </summary>
std::unique_ptr<acf_wrapper::MessagePump> g_message_pump;

std::thread::id g_ui_thread_id;

class UITask : public acf_wrapper::UIThreadTask {
 public:
  using TaskCallback = void(WINAPI*)(intptr_t);
  UITask(TaskCallback cb, intptr_t param) : callback_(cb), param_(param) {}
  ~UITask() override {}

 protected:
  void Execute() override { callback_(param_); }

 private:
  TaskCallback callback_;
  intptr_t param_;

  IMPLEMENT_REFCOUNTING(UITask);
};

}  // namespace

extern DWORD acf_cpp_fntable_environment[];
extern DWORD acf_cpp_fntable_profile[];
extern DWORD acf_cpp_fntable_browser[];
extern DWORD acf_cpp_fntable_new_window_delegate[];
extern DWORD acf_cpp_fntable_urlrequest[];

BOOL IsOnUIThread() {
  return g_ui_thread_id == std::this_thread::get_id();
}

DLL_EXPORTS(PostTaskOnUIThread, BOOL)
(UITask::TaskCallback callback, intptr_t param) {
  g_message_pump->PostTask(new UITask(callback, param));

  return !!g_message_pump;
}

DLL_EXPORTS(IsExVersionActive, BOOL)() {
  return AcfGetExVersionActiveState();
}

using EnvironmentParams = struct {
  LPCSTR user_data_dir;
  BOOL no_sandbox;
};
DLL_EXPORTS(CreateEnvironment, BOOL)
(LPCSTR browser_path,
 EnvironmentParams* params,
 LPVOID* eArray,
 LPVOID handler,
 BOOL sync,
 DWORD* retObj) {
  if (!g_message_pump) {
    g_ui_thread_id = std::this_thread::get_id();
    g_message_pump.reset(new acf_wrapper::MessagePump(GetModuleHandle(0)));
  }

  AcfRefPtr<wrapper::EnvironmentHandler> lpHandler(
      new wrapper::EnvironmentHandler(handler));

  AcfEnvironmentSettings settings;
  if (params) {
    AcfString(&settings.user_data_dir).FromString(params->user_data_dir);
  }

  AcfEnvironment::ExtraCommandLine command_line;
  if (eArray) {
    LPSTR pAry = (LPSTR)*eArray;
    int nSize = *(LPINT)(pAry + sizeof(INT));
    PACF_STRING_PAIR* pPair = (PACF_STRING_PAIR*)(pAry + sizeof(INT) * 2);
    for (size_t i = 0; i < nSize; i++) {
      LPCSTR tmpKey = pPair[i]->key;
      LPCSTR tmpValue = pPair[i]->value;
      command_line.insert(std::make_pair(tmpKey ? tmpKey : std::string(),
                                         tmpValue ? tmpValue : std::string()));
    }
  }

  AcfRefPtr<AcfEnvironment> env = AcfEnvironment::CreateEnvironment(
      browser_path, command_line, settings, lpHandler);

  if (sync) {
    while (!env->IsValid()) {
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
  }

  if (env) {
    env->AddRef();
    retObj[1] = (DWORD)env.get();
    retObj[2] = (DWORD)acf_cpp_fntable_environment;
  }

  return !!env.get();
}

/// <summary>
/// Export function table pass by memory
/// </summary>
namespace {

BOOL ACF_CALLBACK is_same(AcfEnvironment* obj, AcfEnvironment* that) {
  return obj->IsSame(that);
}

BOOL ACF_CALLBACK is_valid(AcfEnvironment* obj) {
  return obj->IsValid();
}

BOOL ACF_CALLBACK create_profile(AcfEnvironment* obj,
                                 LPCSTR path,
                                 BOOL incognito,
                                 BOOL persist_session,
                                 BOOL restore_old_cookie,
                                 BOOL enable_encrypt_cookie,
                                 BOOL ignore_certificate_errors,
                                 LPCSTR host_mapping,
                                 LPCSTR host_resolver_mapping,
                                 DWORD* retObj) {
  AcfProfileCreateParams settings;
  AcfString(&settings.storage_path).FromString(path);
  settings.incognito_mode = incognito;
  settings.persist_session_cookies = persist_session;
  settings.restore_old_session_cookies = restore_old_cookie;
  settings.enable_encrypted_cookies = enable_encrypt_cookie;
  settings.ignore_certificate_errors = enable_encrypt_cookie;
  AcfString(&settings.host_mapping_rules) = host_mapping;
  AcfString(&settings.host_resolver_mapping_rules) = host_resolver_mapping;
  AcfRefPtr<AcfProfile> profile = obj->CreateProfile(settings, nullptr);

  // Wait for thread synchronize
  while (!profile->IsValid()) {
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

  if (profile) {
    profile->AddRef();
    retObj[1] = (DWORD)profile.get();
    retObj[2] = (DWORD)acf_cpp_fntable_profile;
  }

  return !!profile.get();
}

using BrowserParams = struct {
  HWND parent;
  BOOL visible;
  int x;
  int y;
  int width;
  int height;
};
BOOL ACF_CALLBACK create_browser(AcfEnvironment* obj,
                                 AcfProfile* profile,
                                 BrowserParams* pparams,
                                 acf_browser_settings_t* settings,
                                 AcfDictionaryValue* extra,
                                 LPVOID callback,
                                 BOOL sync,
                                 DWORD* retObj) {
  AcfRefPtr<wrapper::BrowserHandler> lphandler(
      new wrapper::BrowserHandler(callback));

  AcfBrowserCreateParams params;
  params.parent = pparams->parent;
  params.visible = pparams->visible;
  params.rect.x = pparams->x;
  params.rect.y = pparams->y;
  params.rect.width = pparams->width;
  params.rect.height = pparams->height;

  AcfRefPtr<AcfBrowser> browser =
      obj->CreateBrowser(profile, lphandler, *settings, params, extra);

  // Wait for thread synchronize
  if (sync) {
    while (!browser->IsValid()) {
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
  }

  if (browser) {
    browser->AddRef();
    retObj[1] = (DWORD)browser.get();
    retObj[2] = (DWORD)acf_cpp_fntable_browser;
  }

  return !!browser.get();
}

BOOL ACF_CALLBACK get_default_profile(AcfEnvironment* obj, DWORD* retObj) {
  AcfRefPtr<AcfProfile> pf = obj->GetDefaultProfile();

  if (pf) {
    pf->AddRef();
    retObj[1] = (DWORD)pf.get();
    retObj[2] = (DWORD)acf_cpp_fntable_profile;
  }

  return !!pf.get();
}

BOOL ACF_CALLBACK create_urlrequest(AcfEnvironment* obj,
                                    AcfRequest* request,
                                    acf_urlrequest_flags_t flags,
                                    LPVOID callback,
                                    AcfFrame* frame,
                                    AcfProfile* profile,
                                    DWORD* retObj) {
  AcfRefPtr<AcfURLRequest> pf = obj->CreateURLRequest(
      request, flags, new wrapper::URLRequestClient(callback), frame, profile);

  if (pf) {
    pf->AddRef();
    retObj[1] = (DWORD)pf.get();
    retObj[2] = (DWORD)acf_cpp_fntable_profile;
  }

  return !!pf.get();
}

void ACF_CALLBACK set_network_profile(AcfEnvironment* obj, AcfValue* profile) {
  obj->SetNetworkContextProfile(profile);
}

}  // namespace

DWORD acf_cpp_fntable_environment[] = {
    (DWORD)is_same,
    (DWORD)is_valid,
    (DWORD)create_profile,
    (DWORD)create_browser,
    (DWORD)get_default_profile,
    (DWORD)create_urlrequest,
    (DWORD)set_network_profile,
};
