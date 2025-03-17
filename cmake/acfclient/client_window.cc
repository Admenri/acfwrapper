
#include "client_window.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "include/acf_devtools_observer.h"
#include "include/acf_dom.h"
#include "include/acf_environment.h"
#include "include/acf_frame.h"
#include "include/acf_function_handler.h"
#include "include/acf_web_message_handler.h"
#include "message_loop_manager.h"

#define IDC_GoBack 201
#define IDC_GoForward 202
#define IDC_Reload 203
#define IDC_Stop 204
#define IDC_Nav 205
#define IDC_More 206

#define IDC_DestroyContext 300
#define IDC_NewWindow 301
#define IDC_CloseWindow 302
#define IDC_EnumFrames 303
#define IDC_ExecuteScript 304
#define IDC_GetSource 305

#define IDC_MuteAudio 306
#define IDC_ToggleDevtools 307
#define IDC_Find 308
#define IDC_ZoomIn 309
#define IDC_ZoomReset 310
#define IDC_ZoomOut 311
#define IDC_TaskManager 312
#define IDC_ToggleExtensionsMenu 313
#define IDC_ToggleDownloadUI 314
#define IDC_ToggleMediaRouter 315
#define IDC_SendDevtoolsMessage 316
#define IDC_DOMDocument 317
#define IDC_URLRequestTest 318

extern AcfRefPtr<AcfEnvironment> g_env;
extern acfclient::MessageWindow* msg_dispatcher;

std::vector<AcfRefPtr<AcfBrowser>> g_browsers_list;

class TestWebSocketClientHandler : public AcfWebSocketClientHandler {
 public:
  void OnHandshakeResponse(AcfRefPtr<AcfWebSocketClient> client) override {
    std::cout << __FUNCTION__ << '\n';
    client_ = client;
  }

  void OnFailure(const AcfString& message,
                 int net_err,
                 int response_code) override {}

  bool OnReceivedMessageFrom(acf_websocket_message_routing_t direction,
                             AcfRefPtr<AcfValue> message) override {
    std::cout << __FUNCTION__ << '\n';
    return false;
  }

  void OnConnectionClosing() override {}

  AcfRefPtr<AcfWebSocketClient> client_;

  IMPLEMENT_REFCOUNTING(TestWebSocketClientHandler);
};

class TestURLRequestClient : public AcfURLRequestClient {
 public:
  void OnRequestComplete(AcfRefPtr<AcfURLRequest> request,
                         bool success) override {}
  void OnUploadProgress(AcfRefPtr<AcfURLRequest> request,
                        int64 current,
                        int64 total) override {}
  void OnDownloadProgress(AcfRefPtr<AcfURLRequest> request,
                          int64 current,
                          int64 total) override {}
  void OnDownloadData(AcfRefPtr<AcfURLRequest> request,
                      const void* data,
                      size_t data_length) override {
    std::string raw((char*)data, data_length);
    std::cout << raw;
  }

  IMPLEMENT_REFCOUNTING(TestURLRequestClient);
};

class TestDOMDocumentCallback : public AcfDOMDocumentCallback {
 public:
  void OnDOMResult(AcfRefPtr<AcfDOMDocument> value) override {
    std::cout << value->GetBaseURL().ToString() << '\n';
  }

  IMPLEMENT_REFCOUNTING(TestDOMDocumentCallback);
};

class TestDOMNodeCallback : public AcfDOMNodeCallback {
 public:
  bool OnQueryResult(AcfRefPtr<AcfDOMNode> value,
                     int count,
                     int total) override {
    if (value) {
      std::cout << value->GetElementBounds().x << " - "
                << value->GetElementBounds().y << " - "
                << value->GetElementBounds().width << " - "
                << value->GetElementBounds().height << '\n';
    }
    return true;
  }

  IMPLEMENT_REFCOUNTING(TestDOMNodeCallback);
};

class TestDevToolsMessageObserver : public AcfDevToolsMessageObserver {
 public:
  void OnDevToolsMessage(AcfRefPtr<AcfBrowser> browser,
                         const void* message,
                         size_t message_size) override {
    std::string msg((char*)message, message_size);
    std::cout << msg << '\n';
  }

  IMPLEMENT_REFCOUNTING(TestDevToolsMessageObserver);
};

class TestWebMessageFactory : public AcfWebMessageFactory,
                              public AcfWebMessageHandler {
 public:
  AcfRefPtr<AcfWebMessageHandler> CreateHost(
      AcfRefPtr<AcfBrowser> browser,
      const AcfString& origin,
      AcfRefPtr<AcfFrame> frame,
      AcfRefPtr<AcfWebMessageProxy> reply_proxy) override {
    reply_proxy_ = reply_proxy;
    return this;
  }

  void OnPostMessage(const WebMessage& message) override {
    std::cout << __FUNCTION__ << '\n';

    WebMessage msg(message);
    AcfRefPtr<AcfValue> v = AcfValue::Create();
    v->SetString("acf test");
    msg.push_back(v);

    reply_proxy_->PostWebMessage(msg);
  }

  AcfRefPtr<AcfWebMessageProxy> reply_proxy_;

  IMPLEMENT_REFCOUNTING(TestWebMessageFactory);
};

class TestFuncFactory : public AcfFunctionFactory, public AcfFunctionHandler {
 public:
  AcfRefPtr<AcfFunctionHandler> CreateHost(AcfRefPtr<AcfBrowser> browser,
                                           const AcfString& origin,
                                           AcfRefPtr<AcfFrame> frame) override {
    return this;
  }

  void OnExecuteFunction(const ArgumentsList& arguments,
                         AcfRefPtr<AcfPromiseResolver> promise) override {
    std::cout << __FUNCTION__ << '\n';

    AcfRefPtr<AcfValue> r = AcfValue::Create();
    r->SetString("test return");
    promise->Resolve(r);
  }

  IMPLEMENT_REFCOUNTING(TestFuncFactory);
};

class TestScriptReturnHandler : public AcfExecuteJavascriptCallback {
 public:
  void OnExecuteResult(AcfRefPtr<AcfValue> value) override {
    std::cout << __FUNCTION__ << value.get() << '\n';

    if (value) {
      std::cout << value->GetType() << '\n';
      if (value->GetType() == VTYPE_DICTIONARY) {
        std::vector<AcfString> ss;
        value->GetDictionary()->GetKeys(ss);

        for (auto& s : ss) {
          std::cout << s.ToString() << '\n';
        }
      }
    }
  }

  IMPLEMENT_REFCOUNTING(TestScriptReturnHandler);
};

class TestSrcVisitor : public AcfStringVisitor {
 public:
  void Visit(const AcfString& string) override {
    std::cout << __FUNCTION__ << " Size: " << string.size() << "\n";
  }

  IMPLEMENT_REFCOUNTING(TestSrcVisitor);
};

class TestTask_Quit : public acfclient::Task {
 public:
  TestTask_Quit() {}
  ~TestTask_Quit() override {}

  void Execute() override { PostQuitMessage(0); }

  IMPLEMENT_REFCOUNTING(TestTask_Quit);
};

class TestTask_NewWindow : public acfclient::Task {
 public:
  TestTask_NewWindow() {}
  ~TestTask_NewWindow() override {}

  void Execute() override {
    AcfRefPtr<acfclient::Window> window = new acfclient::Window(
        g_env, profile_, GetModuleHandle(0), nullptr, new_browser_delegate_);

    ::SetWindowText(window->GetEditHandle(),
                    (LPCWSTR)url_.ToString16().c_str());

    window->Show();
  }

  AcfRefPtr<AcfProfile> profile_;
  AcfRefPtr<AcfNewWindowDelegate> new_browser_delegate_;
  AcfString url_;

  IMPLEMENT_REFCOUNTING(TestTask_NewWindow);
};

class TestProfileCompleteHandler : public AcfCompleteHandler {
 public:
  void OnComplete(bool success) override {
    AcfRefPtr<TestTask_NewWindow> task = new TestTask_NewWindow();
    task->new_browser_delegate_ = nullptr;
    task->profile_ = profile;
    msg_dispatcher->PostTask(task);
  }

  AcfRefPtr<AcfProfile> profile;

  IMPLEMENT_REFCOUNTING(AcfCompleteHandler);
};

namespace acfclient {

static const wchar_t kClassName[] = L"acfClient_Window";

WNDPROC SetWndProcPtr(HWND hWnd, WNDPROC wndProc) {
  WNDPROC old =
      reinterpret_cast<WNDPROC>(::GetWindowLongPtr(hWnd, GWLP_WNDPROC));
  [[maybe_unused]] LONG_PTR result = ::SetWindowLongPtr(
      hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndProc));
  return old;
}

void SetUserDataPtr(HWND hWnd, void* ptr) {
  SetLastError(ERROR_SUCCESS);
  [[maybe_unused]] LONG_PTR result =
      ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr));
}

template <typename T>
T GetUserDataPtr(HWND hWnd) {
  return reinterpret_cast<T>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

Window::Window(AcfRefPtr<AcfEnvironment> env,
               AcfRefPtr<AcfProfile> profile,
               HINSTANCE instance,
               RECT* initial_rect,
               AcfRefPtr<AcfNewWindowDelegate> new_window)
    : instance_(instance), new_browser_delegate_(new_window) {
  RegisterWindowClass();

  [[maybe_unused]] int x, y, width, height;
  if (!initial_rect || ::IsRectEmpty(initial_rect)) {
    // Use the default window position/size.
    x = y = width = height = CW_USEDEFAULT;
  } else {
    x = initial_rect->left;
    y = initial_rect->top;
    width = initial_rect->right - initial_rect->left;
    height = initial_rect->bottom - initial_rect->top;
  }

  const DWORD dwStyle = WS_OVERLAPPEDWINDOW;
  window_ = CreateWindow(kClassName, L"acfclient", dwStyle, x, y, width, height,
                         nullptr, nullptr, instance, this);
  UpdateWindow(window_);

  // Create browser
  AcfBrowserCreateParams params{};
  RECT bound = GetBrowserBound();

  CreateMenu();

  if (IsWindow(window_)) {
    std::cout << "[ACFClient] Window:" << window_ << "\n";
  }

  params.parent = window_;
  params.rect.x = bound.left;
  params.rect.y = bound.top;
  params.rect.width = bound.right;
  params.rect.height = bound.bottom;

  // Call on any thread
  AcfBrowserSettings bsettings;
  bsettings.disable_default_javascript_dialogs = true;
  bsettings.disable_default_login_delegate = true;
  bsettings.disable_default_permission_prompt = true;
  env->CreateBrowser(profile, this, bsettings, params, nullptr);

  std::cout << "Window Create: " << g_browsers_list.size() << "\n";
}

Window::~Window() {
  std::cout << "Window Destroy: " << g_browsers_list.size() << "\n";

  if (more_menu_) {
    DestroyMenu(more_menu_);
  }

  SetUserDataPtr(window_, nullptr);
  PostMessage(window_, WM_CLOSE, 0, 0);

  if (!g_browsers_list.size()) {
    AcfRefPtr<TestTask_Quit> task = new TestTask_Quit();
    msg_dispatcher->PostTask(task);
  }
}

void Window::Show() {
  ShowWindow(window_, SW_SHOW);
}

void Window::Hide() {
  ShowWindow(window_, SW_HIDE);
}

HWND Window::GetHandle() {
  return window_;
}

LRESULT CALLBACK Window::MainWndProc(HWND hWnd,
                                     UINT message,
                                     WPARAM wParam,
                                     LPARAM lParam) {
  Window* self = nullptr;
  self = GetUserDataPtr<Window*>(hWnd);

  // Callback for the main window
  switch (message) {
    case WM_NCCREATE: {
      CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
      self = reinterpret_cast<Window*>(cs->lpCreateParams);
      SetUserDataPtr(hWnd, self);
      break;
    }
    case WM_CREATE: {
      self->OnCreate(hWnd);
      break;
    }
    case WM_SIZE: {
      if (self) {
        self->OnSize(hWnd);
      }
      break;
    }
    case WM_CLOSE:
      // wait for browser
      if (self) {
        self->OnClose();
        return 0;
      } else {
        break;
      }
    case WM_SETFOCUS: {
      if (self) {
        self->OnFocus();
      }
      break;
    }
    case WM_COMMAND:
      if (self->OnCommand(LOWORD(wParam))) {
        return 0;
      }
      break;
    default:
      break;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::OnClose() {
  if (browser_weak_ptr_ && browser_weak_ptr_->IsValid()) {
    browser_weak_ptr_->Close(false);
  }
}

void Window::OnFocus() {
  if (browser_weak_ptr_ && browser_weak_ptr_->IsValid()) {
    browser_weak_ptr_->Focus();
  }
}

void Window::CreateMenu() {
  more_menu_ = ::CreatePopupMenu();

  MENUITEMINFO info = {0};
  info.cbSize = sizeof(MENUITEMINFO);

  UINT count = 0;
  std::wstring labelStr;

#define SET_MENU(name, idc)                               \
  labelStr = L##name;                                     \
  info.fMask |= MIIM_STRING | MIIM_ID;                    \
  info.cch = static_cast<uint32_t>(labelStr.size());      \
  info.dwTypeData = const_cast<LPWSTR>(labelStr.c_str()); \
  info.wID = idc;                                         \
  ::InsertMenuItem(more_menu_, count, TRUE, &info);       \
  count++;

  SET_MENU("Destroy Context", IDC_DestroyContext);
  SET_MENU("New Window", IDC_NewWindow);
  SET_MENU("Close Window", IDC_CloseWindow);
  SET_MENU("Enum Frames", IDC_EnumFrames);
  SET_MENU("Execute Script", IDC_ExecuteScript);
  SET_MENU("Get Source", IDC_GetSource);
  SET_MENU("Mute audio", IDC_MuteAudio);
  SET_MENU("Toggle Devtools", IDC_ToggleDevtools);
  SET_MENU("Find", IDC_Find);
  SET_MENU("ZoomIn", IDC_ZoomIn);
  SET_MENU("ZoomReset", IDC_ZoomReset);
  SET_MENU("ZoomOut", IDC_ZoomOut);
  SET_MENU("Task Manager", IDC_TaskManager);
  SET_MENU("Toggle Extensions Menu", IDC_ToggleExtensionsMenu);
  SET_MENU("Toggle Download UI", IDC_ToggleDownloadUI);
  SET_MENU("Toggle Media Router", IDC_ToggleMediaRouter);
  SET_MENU("Send Devtools Message", IDC_SendDevtoolsMessage);
  SET_MENU("DOM Document", IDC_DOMDocument);
  SET_MENU("URLRequest Test", IDC_URLRequestTest);
}

bool Window::OnCommand(UINT id) {
  switch (id) {
    case IDC_GoBack:  // Back button
      if (browser_weak_ptr_) {
        browser_weak_ptr_->GoBack();
      }
      break;
    case IDC_GoForward:  // Forward button
      if (browser_weak_ptr_) {
        browser_weak_ptr_->GoForward();
      }
      break;
    case IDC_Reload:  // Reload button
      if (browser_weak_ptr_) {
        browser_weak_ptr_->Reload(false);
      }
      break;
    case IDC_Stop:  // Stop button
      if (browser_weak_ptr_) {
        browser_weak_ptr_->Stop();
      }
      break;
    case IDC_Nav:  // Navigate button
    {
#define MAX_URL_LENGTH 255
      wchar_t strPtr[MAX_URL_LENGTH + 1] = {0};
      *((LPWORD)strPtr) = MAX_URL_LENGTH;
      LRESULT strLen =
          SendMessage(edit_control_, EM_GETLINE, 0, (LPARAM)strPtr);
      if (strLen > 0) {
        strPtr[strLen] = 0;

        browser_weak_ptr_->LoadURL((const char16_t*)strPtr, AcfString(),
                                   acf_referrer_policy_t(),
                                   acf_transition_type_t(), AcfString());
      }
#undef MAX_URL_LENGTH
    } break;
    case IDC_More: {
      POINT pt = {0};
      ::GetCursorPos(&pt);

      ::TrackPopupMenu(more_menu_,
                       TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, pt.x,
                       pt.y, 0, window_, nullptr);
    } break;
    case IDC_DestroyContext: {
    } break;
    case IDC_NewWindow: {
      std::cout << "Menu: NewWindow\n";

      time_t v;
      time(&v);

      AcfRefPtr<TestProfileCompleteHandler> handler =
          new TestProfileCompleteHandler;

      AcfProfileCreateParams pset;
      AcfString(&pset.storage_path) = std::to_string(v);
      pset.enable_encrypted_cookies = false;
      handler->profile = g_env->CreateProfile(pset, handler);

    } break;
    case IDC_CloseWindow: {
      std::cout << "Menu: CloseWindow\n";

      PostMessage(window_, WM_CLOSE, 0, 0);
    } break;
    case IDC_EnumFrames: {
      std::cout << "Menu: EnumFrames\n";

      std::vector<int64_t> ids;
      browser_weak_ptr_->GetFrameIdentifiers(ids);

      std::cout << "[Frame] ----------------------------------------\n";
      for (auto it : ids) {
        AcfRefPtr<AcfFrame> f = browser_weak_ptr_->GetFrame(it);
        std::cout << "[Frame] URL: " << f->GetURL().ToString()
                  << " - Name: " << f->GetName().ToString() << "\n";
      }
      std::cout << "[Frame] ----------------------------------------\n";
    } break;
    case IDC_ExecuteScript: {
      std::string s;
      std::cout << "Enter script: ";
      std::cin >> s;
      browser_weak_ptr_->GetMainFrame()->ExecuteJavascript(
          s, browser_weak_ptr_->GetURL(), new TestScriptReturnHandler);
    } break;
    case IDC_GetSource: {
      browser_weak_ptr_->GetMainFrame()->GetSource(new TestSrcVisitor);
    } break;
    case IDC_MuteAudio: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->SetAudioMuted(!browser_weak_ptr_->IsAudioMuted());
      }
    } break;
    case IDC_ToggleDevtools: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->ToggleDevtools();
      }
    } break;
    case IDC_Find: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->RaiseFindBar();
      }
    } break;
    case IDC_ZoomIn: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->ZoomPage(ZOOM_IN);
      }
    } break;
    case IDC_ZoomReset: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->ZoomPage(ZOOM_RESET);
      }
    } break;
    case IDC_ZoomOut: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->ZoomPage(ZOOM_OUT);
      }
    } break;
    case IDC_TaskManager: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->OpenTaskManager();
      }
    } break;
    case IDC_ToggleExtensionsMenu: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->ToggleExtensionsMenu();
      }
    } break;
    case IDC_ToggleDownloadUI: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->ToggleDownloadUIForWindow();
      }
    } break;
    case IDC_ToggleMediaRouter: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->ToggleMediaRouterBubble();
      }
    } break;
    case IDC_SendDevtoolsMessage: {
      if (browser_weak_ptr_) {
        std::string in;
        std::cin >> in;
        browser_weak_ptr_->SendDevToolsMessage(in.data(), in.size());
      }
    } break;
    case IDC_DOMDocument: {
      if (browser_weak_ptr_) {
        browser_weak_ptr_->GetMainFrame()->QuerySelectorAll(
            "a", new TestDOMNodeCallback);
      }
    } break;
    case IDC_URLRequestTest: {
      if (browser_weak_ptr_) {
        AcfRefPtr<AcfRequest> request = AcfRequest::Create();
        request->SetMethod("POST");
        request->SetURL("https://tls.browserleaks.com/json");

        browser_weak_ptr_->GetEnvironment()->CreateURLRequest(
            request, 0, new TestURLRequestClient,
            browser_weak_ptr_->GetMainFrame(), browser_weak_ptr_->GetProfile());
      }
    } break;
    default:
      return false;
  }

  // menu command except
  if (id < 300) {
    OnFocus();
  }

  return true;
}

void Window::OnCreate(HWND window) {
  RECT rect;
  GetClientRect(window, &rect);

  int x_offset = 0;

  const int button_width = 72;
  const int urlbar_height = 24;

  back_button_ =
      CreateWindow(L"BUTTON", L"Back", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   x_offset, 0, button_width, urlbar_height, window,
                   reinterpret_cast<HMENU>(IDC_GoBack), instance_, 0);
  x_offset += button_width;
  EnableWindow(back_button_, false);

  forward_button_ =
      CreateWindow(L"BUTTON", L"Forward", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   x_offset, 0, button_width, urlbar_height, window,
                   reinterpret_cast<HMENU>(IDC_GoForward), instance_, 0);
  x_offset += button_width;
  EnableWindow(forward_button_, false);

  reload_button_ =
      CreateWindow(L"BUTTON", L"Reload", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   x_offset, 0, button_width, urlbar_height, window,
                   reinterpret_cast<HMENU>(IDC_Reload), instance_, 0);
  x_offset += button_width;
  EnableWindow(reload_button_, false);

  stop_button_ =
      CreateWindow(L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   x_offset, 0, button_width, urlbar_height, window,
                   reinterpret_cast<HMENU>(IDC_Stop), instance_, 0);
  x_offset += button_width;
  EnableWindow(stop_button_, false);

  more_button_ =
      CreateWindow(L"BUTTON", L"More", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   x_offset, 0, button_width, urlbar_height, window,
                   reinterpret_cast<HMENU>(IDC_More), instance_, 0);
  x_offset += button_width;

  edit_control_ = CreateWindow(L"EDIT", 0,
                               WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |
                                   ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                               x_offset, 0, rect.right - button_width * 6,
                               urlbar_height, window, 0, instance_, 0);

  nav_button_ = CreateWindow(
      L"BUTTON", L"Navigate", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      rect.right - button_width, 0, button_width, urlbar_height, window,
      reinterpret_cast<HMENU>(IDC_Nav), instance_, 0);
}

void Window::OnSize(HWND window) {
  RECT rect;
  GetClientRect(window, &rect);

  const int button_width = 72;
  const int urlbar_height = 24;

  SetWindowPos(edit_control_, 0, button_width * 5, 0,
               rect.right - button_width * 6, urlbar_height, SWP_NOZORDER);
  SetWindowPos(nav_button_, 0, rect.right - button_width, 0, button_width,
               urlbar_height, SWP_NOZORDER);

  if (browser_window_) {
    SetWindowPos(browser_window_, 0, 0, urlbar_height, rect.right,
                 rect.bottom - urlbar_height, SWP_NOZORDER);
  }
}

RECT Window::GetBrowserBound() {
  const int urlbar_height = 24;

  RECT rect;
  GetClientRect(window_, &rect);

  return {0, urlbar_height, rect.right, rect.bottom - urlbar_height};
}

void Window::RegisterWindowClass() {
  // Only register the class one time.
  static bool class_registered = false;
  if (class_registered) {
    return;
  }
  class_registered = true;

  WNDCLASSEX wcex = {0};

  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.lpszClassName = kClassName;
  wcex.lpfnWndProc = MainWndProc;
  wcex.hInstance = instance_;
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  RegisterClassEx(&wcex);
}

void Window::SetBrowser(HWND handle) {
  browser_window_ = handle;
  OnSize(window_);
}

// On Browser IPC Thread
void Window::OnBrowserCreated(AcfRefPtr<AcfBrowser> browser) {
  std::cout << "OnBrowserCreated: " << browser->GetWindowHandle() << "\n";
  browser_weak_ptr_ = browser.get();
  SetBrowser(browser->GetWindowHandle());
  Show();

  AcfRefPtr<AcfValue> v = AcfValue::Create();
  AcfRefPtr<AcfDictionaryValue> d = AcfDictionaryValue::Create();

  srand(time(nullptr));
  d->SetInt("canvas.profile", rand());
  d->SetInt("client.rects", rand());
  d->SetInt("webgl.profile", rand());
  d->SetInt("audio.profile", rand());

  d->SetString("webrtc.ip_policy", "disable_non_proxied_udp");

  v->SetDictionary(d);
  browser_weak_ptr_->SetContextProfile(v);

  browser->AddDocumentStartJavaScript("function aaa() {return 1233214;}",
                                      {"*"});

  browser->AddWebMessageHostFactory(new TestWebMessageFactory, "acfTest",
                                    {"*"});

  browser->AddFunctionRegisterHostFactory(new TestFuncFactory, "acfFunc",
                                          "testFunc", {"*"});

  devtools_observer_ =
      browser->AddDevToolsMessageObserver(new TestDevToolsMessageObserver);

  if (new_browser_delegate_) {
    new_browser_delegate_->SetNewBrowser(browser);
    new_browser_delegate_->HandleRequest(true, true);
    new_browser_delegate_.reset();

    browser->ResumeLoadingBrowser();
  } else {
    browser->LoadURL("https://www.bilibili.com/", AcfString(),
                     acf_referrer_policy_t(), acf_transition_type_t(),
                     AcfString());
  }

  g_browsers_list.push_back(browser);
}

void Window::OnNewWindowRequest(AcfRefPtr<AcfBrowser> browser,
                                NewWindowSource source,
                                NewWindowDisposition disposition,
                                const AcfPopupFeatures& popup_features,
                                bool user_gesture,
                                AcfRefPtr<AcfNewWindowDelegate> delegate) {
  std::cout << "OnNewWindow Request\n";

  AcfRefPtr<TestTask_NewWindow> task = new TestTask_NewWindow();
  task->new_browser_delegate_ = delegate;
  task->url_ = delegate->GetTargetURL();
  task->profile_ = browser->GetProfile();
  msg_dispatcher->PostTask(task);
}

void Window::OnBrowserDestroyed(AcfRefPtr<AcfBrowser> browser) {
  std::cout << "OnBrowserDestroyed\n";

  for (auto it = g_browsers_list.begin(); it != g_browsers_list.end(); it++) {
    if ((*it)->IsSame(browser)) {
      g_browsers_list.erase(it);
      break;
    }
  }
}

void Window::OnLoadingStateChanged(AcfRefPtr<AcfBrowser> browser,
                                   bool show_loading_ui) {
  std::cout << "OnLoadingStateChanged\n";

  if (back_button_) {
    EnableWindow(back_button_, browser->CanGoBack());
  }
  if (forward_button_) {
    EnableWindow(forward_button_, browser->CanGoForward());
  }
  if (reload_button_) {
    EnableWindow(reload_button_, !browser->IsLoading());
  }
  if (stop_button_) {
    EnableWindow(stop_button_, browser->IsLoading());
  }
}

void Window::OnNavigationStateChanged(AcfRefPtr<AcfBrowser> browser,
                                      NavigationTypes flags) {
  std::cout << "OnNavigationStateChanged\n";

  if (flags & NavigationTypes::INVALIDATE_TYPE_TITLE) {
    AcfString rtitle = browser->GetTitle().ToString() + " - acfclient";
    SetWindowText(window_, (LPWSTR)rtitle.ToString16().c_str());
  }
  if (flags & NavigationTypes::INVALIDATE_TYPE_URL) {
    SetWindowText(edit_control_,
                  (LPWSTR)browser->GetURL().ToString16().c_str());
  }
}

void Window::OnContextMenuRequest(AcfRefPtr<AcfBrowser> browser,
                                  AcfRefPtr<AcfContextMenuParams> menu_params,
                                  AcfRefPtr<AcfContextMenuModel> menu_model,
                                  AcfRefPtr<AcfContextMenuCallback> callback) {
  std::cout << "OnContextMenuRequest\n";

  menu_model->InsertSeparatorAt(menu_model->GetCount());

  menu_model->InsertItemAt(menu_model->GetCount(), 1310001, "Test Menu Item");
  AcfRefPtr<AcfContextMenuModel> sub_menu = menu_model->InsertSubMenuAt(
      menu_model->GetCount(), 1310002, "Sub menu test");
  sub_menu->InsertItemAt(sub_menu->GetCount(), 1310003, "Normal item");
  sub_menu->InsertSeparatorAt(sub_menu->GetCount());

  sub_menu->InsertRadioItemAt(sub_menu->GetCount(), 1310004, "Item 1", 1000);
  sub_menu->SetCheckedAt(sub_menu->GetCount() - 1, true);

  sub_menu->InsertRadioItemAt(sub_menu->GetCount(), 1310005, "Item 2", 1000);
  sub_menu->InsertRadioItemAt(sub_menu->GetCount(), 1310006, "Item 3", 1000);
  sub_menu->InsertSeparatorAt(sub_menu->GetCount());
  sub_menu->InsertCheckItemAt(sub_menu->GetCount(), 1310007, "Item 1");

  sub_menu->InsertCheckItemAt(sub_menu->GetCount(), 1310008, "Item 2");
  sub_menu->SetCheckedAt(sub_menu->GetCount() - 1, true);

  sub_menu->InsertCheckItemAt(sub_menu->GetCount(), 1310009, "Item 3");
}

void Window::OnContextMenuExecute(AcfRefPtr<AcfBrowser> browser,
                                  AcfRefPtr<AcfContextMenuParams> menu_params,
                                  int command_id,
                                  int event_flags) {
  if (command_id == 310001) {
    MessageBox(window_, L"Execute test menu", L"AcfClient", 0);
  }
  std::cout << "Execute command id: " << command_id << '\n';
}

void Window::OnShowPermissionPrompt(
    AcfRefPtr<AcfBrowser> browser,
    uint64 prompt_id,
    const AcfString& requesting_origin,
    uint32 requested_permissions,
    AcfRefPtr<AcfPermissionPromptCallback> callback) {
  std::cout << __FUNCTION__ << '\n';
  callback->Continue(PR_PERMISSION_RESULT_ACCEPT);
}

void Window::OnDismissPermissionPrompt(AcfRefPtr<AcfBrowser> browser,
                                       uint64 prompt_id,
                                       acf_permission_request_result_t result) {
}

void Window::OnBeforeNavigation(AcfRefPtr<AcfBrowser> browser,
                                AcfRefPtr<AcfFrame> frame,
                                const AcfString& url,
                                const AcfString& referrer,
                                bool user_gesture,
                                bool is_redirect,
                                AcfRefPtr<AcfCallback> callback) {}

void Window::OnAuthLoginRequest(AcfRefPtr<AcfBrowser> browser,
                                bool is_proxy,
                                const AcfString& host,
                                const AcfString& url,
                                const AcfString& scheme,
                                const AcfString& realm,
                                const AcfString& challenge,
                                bool is_main_frame,
                                AcfRefPtr<AcfLoginDelegate> delegate) {
  std::cout << __FUNCTION__ << '\n';
}

void Window::OnConsoleMessage(AcfRefPtr<AcfBrowser> browser,
                              AcfRefPtr<AcfFrame> frame,
                              int level,
                              const AcfString& message,
                              const AcfString& source,
                              int line,
                              const AcfString& trace) {
  std::cout << message.ToString() << '\n';
}

AcfRefPtr<AcfWebSocketClientHandler> Window::CreateWebSocket(
    AcfRefPtr<AcfBrowser> browser,
    AcfString& target_url) {
  return new TestWebSocketClientHandler();
}

}  // namespace acfclient
