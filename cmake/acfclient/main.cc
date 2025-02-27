#include <iostream>

#include "client_window.h"
#include "include/acf_environment.h"
#include "include/acf_profile.h"
#include "include/acf_resource_request_handler.h"
#include "message_loop_manager.h"

acfclient::MessageWindow* msg_dispatcher;
AcfRefPtr<AcfEnvironment> g_env;

class ClientSchemeHandler : public AcfResourceHandler {
 public:
  ClientSchemeHandler() : offset_(0) {
    // Build the response html
    data_ =
        "<html><head><title>Client Scheme Handler</title></head>"
        "<body bgcolor=\"white\">"
        "This contents of this page page are served by the "
        "ClientSchemeHandler class handling the client:// protocol."
        "<br/>You should see an image:"
        "<br/><img src=\"client://tests/logo.png\"><pre>";
    data_.append(
        "</pre><br/>Try the test form:"
        "<form method=\"POST\" action=\"handler.html\">"
        "<input type=\"text\" name=\"field1\">"
        "<input type=\"text\" name=\"field2\">"
        "<input type=\"submit\">"
        "</form></body></html>");

    // Set the resulting mime type
    mime_type_ = "text/html";
  }

  void GetResponseHeaders(AcfRefPtr<AcfResponse> response,
                          int64& response_length) override {
    response->SetMimeType(mime_type_);
    response->SetStatus(200);

    // Set the resulting response length
    response_length = data_.length();
  }

  void Read(void* data_out,
            int bytes_to_read,
            AcfRefPtr<AcfResourceReadCallback> callback) override {
    bool has_data = false;
    int bytes_read = 0;

    if (offset_ < data_.length()) {
      // Copy the next block of data into the buffer.
      int transfer_size =
          std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
      memcpy(data_out, data_.c_str() + offset_, transfer_size);
      offset_ += transfer_size;

      bytes_read = transfer_size;
      has_data = true;
    }

    callback->Continue(has_data ? bytes_read : 0);
  }

 private:
  std::string data_;
  std::string mime_type_;
  size_t offset_;

  IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
};

// Filter that writes out all of the contents unchanged.
class PassThruResponseFilter : public AcfResponseFilter {
 public:
  PassThruResponseFilter() {}

  bool InitFilter() override { return true; }

  FilterStatus Filter(void* data_in,
                      size_t data_in_size,
                      size_t& data_in_read,
                      void* data_out,
                      size_t data_out_size,
                      size_t& data_out_written) override {
    // Write out the contents unchanged.
    data_out_written = std::min(data_in_size, data_out_size);

    // All data will be read.
    data_in_read = data_out_written;

    if (data_out_written > 0) {
      memcpy(data_out, data_in, data_out_written);
    }

    return RESPONSE_FILTER_DONE;
  }

 private:
  IMPLEMENT_REFCOUNTING(PassThruResponseFilter);
};

class TestResourceRequestHandler : public AcfResourceRequestHandler {
 public:
  AcfRefPtr<AcfResourceHandler> OnBeforeResourceLoad(
      AcfRefPtr<AcfRequest> request) override {
    if (request->GetURL().ToString().find("://test/acf_page") !=
        std::string::npos) {
      return new ClientSchemeHandler;
    }

    return nullptr;
  }

  AcfRefPtr<AcfResponseFilter> OnResourceResponse(
      AcfRefPtr<AcfRequest> request,
      AcfRefPtr<AcfResponse> response,
      int& mode) override {
    if (request->GetURL().ToString().find("chrome://") != std::string::npos &&
        request->GetURL().ToString().find("logo") != std::string::npos) {
      std::cout << "chrome logo\n";
    }

    return new PassThruResponseFilter;
  }

 private:
  IMPLEMENT_REFCOUNTING(TestResourceRequestHandler);
};

class TestCookieVisitor : public AcfCookieVisitor {
 public:
  bool Visit(AcfRefPtr<AcfCookie> cookie, int count, int total) override {
    std::cout << __FUNCTION__ << '\n';

    if (!cookie) {
      return false;
    }

    std::cout << cookie->GetName().ToString() << "="
              << cookie->GetValue().ToString() << '\n';
    return true;
  }

  IMPLEMENT_REFCOUNTING(TestCookieVisitor);
};

class EnvTest : public AcfEnvironmentHandler {
 public:
  void OnProcessInitialized(AcfRefPtr<AcfEnvironment> environment,
                            bool success) override {}

  AcfRefPtr<AcfResourceRequestHandler> GetResourceRequestHandler(
      AcfRefPtr<AcfProfile> profile,
      int64 frame_id,
      AcfRefPtr<AcfRequest> request,
      acf_url_loader_factory_type_t type,
      const AcfString& request_initiator,
      bool& block_request) override {
    return new TestResourceRequestHandler;
  }

  void OnProcessNavigateRequest(AcfRefPtr<AcfProfile> profile,
                                const AcfString& url,
                                const AcfString& referrer,
                                bool user_gesture) override {
    std::cout << url.ToString() << '\n';
  }

  IMPLEMENT_REFCOUNTING(EnvTest);
};

void MessageLoopRun() {
  MSG msg;
  // Run the application message loop.
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

int main(int argc, char* argv[]) {
  ::SetProcessDPIAware();

  if (AcfGetExVersionActiveState()) {
    MessageBox(0, L"Ex Version Activated.", 0, 0);
  }

  {
    msg_dispatcher = new acfclient::MessageWindow(GetModuleHandle(0));

    AcfEnvironment::ExtraCommandLine cmdline;
    AcfEnvironmentSettings settings;
    AcfString(&settings.user_data_dir) = "ACFUserData";
    cmdline.emplace("acf-allow-offstore-extension", "F");

    g_env = AcfEnvironment::CreateEnvironment("chrome", cmdline, settings,
                                              new EnvTest);

    while (!g_env->IsValid()) {
      ::Sleep(10);
    }

    AcfProfileCreateParams pset;
    AcfString(&pset.storage_path) = "testProfile";
    pset.enable_encrypted_cookies = false;
    AcfRefPtr<AcfProfile> profile = g_env->CreateProfile(pset, nullptr);
    while (!profile->IsValid()) {
      ::Sleep(10);
    }

    auto ck = AcfCookie::Create(
        "https://www.baidu.com/", "test_k", "test_v", ".baidu.com", "/", 0,
        true, true, acf_cookie_same_site_t(), acf_cookie_priority_t());

    if (ck) {
      std::cout << profile->GetPath().ToString() << '\n';
      profile->GetCookieManager()->SetCookie("https://www.baidu.com/", ck,
                                             nullptr);
      profile->GetCookieManager()->GetCookies("", false, new TestCookieVisitor);
    }

    AcfRefPtr<acfclient::Window> win =
        new acfclient::Window(g_env, profile, GetModuleHandle(0), nullptr);
    win->Show();
    win.reset();

    // Main loop
    MessageLoopRun();
    delete msg_dispatcher;
  }

  return 0;
}
