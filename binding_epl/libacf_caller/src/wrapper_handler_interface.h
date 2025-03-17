#ifndef ACF_E_HANDLER_H_
#define ACF_E_HANDLER_H_

#include <memory>

#include "include/acf_browser.h"
#include "include/acf_devtools_observer.h"
#include "include/acf_environment.h"
#include "include/acf_profile.h"

#include "wrapper_utility.h"

namespace wrapper {

class EnvironmentHandler : public AcfEnvironmentHandler {
 public:
  EnvironmentHandler(LPVOID callback);
  ~EnvironmentHandler();

 protected:
  void OnProcessInitialized(AcfRefPtr<AcfEnvironment> env,
                            bool success) override;
  AcfRefPtr<AcfResourceRequestHandler> GetResourceRequestHandler(
      AcfRefPtr<AcfProfile> profile,
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfFrame> frame,
      AcfRefPtr<AcfRequest> request,
      acf_url_loader_factory_type_t type,
      const AcfString& request_initiator,
      bool& block_request) override;
  void OnProcessNavigateRequest(AcfRefPtr<AcfProfile> profile,
                                const AcfString& url,
                                const AcfString& referrer,
                                bool user_gesture) override;

 private:
  LPVOID callback_;

  IMPLEMENT_REFCOUNTING(EnvironmentHandler);
};

class ResourceRequestHandler : public AcfResourceRequestHandler {
 public:
  ResourceRequestHandler(LPVOID callback,
                         AcfRefPtr<AcfProfile> profile,
                         AcfRefPtr<AcfBrowser> browser,
                         AcfRefPtr<AcfFrame> frame);

  AcfRefPtr<AcfResourceHandler> OnBeforeResourceLoad(
      AcfRefPtr<AcfRequest> request) override;
  AcfRefPtr<AcfResponseFilter> OnResourceResponse(
      AcfRefPtr<AcfRequest> request,
      AcfRefPtr<AcfResponse> response,
      int& mode) override;
  void OnResourceLoadComplete(AcfRefPtr<AcfRequest> request,
                              URLRequestStatus status,
                              int64 received_content_length) override;

 private:
  LPVOID callback_;
  AcfRefPtr<AcfProfile> profile_;
  AcfRefPtr<AcfBrowser> browser_;
  AcfRefPtr<AcfFrame> frame_;

  IMPLEMENT_REFCOUNTING(ResourceRequestHandler);
};

class BrowserHandler : public AcfBrowserHandler {
 public:
  class DevtoolObserver : public AcfDevToolsMessageObserver {
   public:
    DevtoolObserver(LPVOID callback) : callback_(callback) {}
    ~DevtoolObserver() override {}

   protected:
    void OnDevToolsMessage(AcfRefPtr<AcfBrowser> browser,
                           const void* message,
                           size_t message_size) override;

   private:
    LPVOID callback_;
    IMPLEMENT_REFCOUNTING(DevtoolObserver);
  };

  BrowserHandler(LPVOID callback);
  ~BrowserHandler();

 protected:
  void OnBrowserCreated(AcfRefPtr<AcfBrowser> browser) override;
  void OnNewWindowRequest(AcfRefPtr<AcfBrowser> browser,
                          NewWindowSource source,
                          NewWindowDisposition disposition,
                          const AcfPopupFeatures& popup_features,
                          bool user_gesture,
                          AcfRefPtr<AcfNewWindowDelegate> delegate) override;
  void OnBrowserDestroyed(AcfRefPtr<AcfBrowser> browser) override;
  void OnLoadingStateChanged(AcfRefPtr<AcfBrowser> browser,
                             bool show_loading_ui) override;
  void OnNavigationStateChanged(AcfRefPtr<AcfBrowser> browser,
                                NavigationTypes flags) override;
  void OnFullscreenStateChanged(AcfRefPtr<AcfBrowser> browser,
                                bool fullscreen) override;
  void OnAuthLoginRequest(AcfRefPtr<AcfBrowser> browser,
                          bool is_proxy,
                          const AcfString& host,
                          const AcfString& url,
                          const AcfString& scheme,
                          const AcfString& realm,
                          const AcfString& challenge,
                          bool is_main_frame,
                          AcfRefPtr<AcfLoginDelegate> delegate) override;
  void OnContextMenuRequest(
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfContextMenuParams> menu_params,
      AcfRefPtr<AcfContextMenuModel> menu_model,
      AcfRefPtr<AcfContextMenuCallback> callback) override;
  void OnContextMenuExecute(AcfRefPtr<AcfBrowser> browser,
                            AcfRefPtr<AcfContextMenuParams> menu_params,
                            int command_id,
                            int event_flags) override;

  void OnConsoleMessage(AcfRefPtr<AcfBrowser> browser,
                        AcfRefPtr<AcfFrame> frame,
                        int level,
                        const AcfString& message,
                        const AcfString& source,
                        int line,
                        const AcfString& trace) override;
  void OnLoadingProgressChange(AcfRefPtr<AcfBrowser> browser,
                               double progress) override;
  void OnAudioStateChange(AcfRefPtr<AcfBrowser> browser, bool audible) override;

  void OnBeforeNavigation(AcfRefPtr<AcfBrowser> browser,
                          AcfRefPtr<AcfFrame> frame,
                          const AcfString& url,
                          const AcfString& referrer,
                          bool user_gesture,
                          bool is_redirect,
                          AcfRefPtr<AcfCallback> callback) override;

  void OnShowPermissionPrompt(
      AcfRefPtr<AcfBrowser> browser,
      uint64 prompt_id,
      const AcfString& requesting_origin,
      uint32 requested_permissions,
      AcfRefPtr<AcfPermissionPromptCallback> callback) override;
  void OnDismissPermissionPrompt(
      AcfRefPtr<AcfBrowser> browser,
      uint64 prompt_id,
      acf_permission_request_result_t result) override;
  void OnJavaScriptDialog(
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfFrame> frame,
      acf_jsdialog_type_t dialog_type,
      const AcfString& message_text,
      const AcfString& default_prompt_text,
      AcfRefPtr<AcfJavaScriptDialogCallback> callback) override;
  void OnBeforeUnloadDialog(
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfFrame> frame,
      bool is_reload,
      AcfRefPtr<AcfJavaScriptDialogCallback> callback) override;
  void OnHandleJavaScriptDialog(AcfRefPtr<AcfBrowser> browser,
                                bool accept,
                                const AcfString& prompt_override) override;
  void OnCancelJavaScriptDialogs(AcfRefPtr<AcfBrowser> browser,
                                 bool reset_state) override;
  void OnFileDialog(AcfRefPtr<AcfBrowser> browser,
                    acf_file_dialog_mode_t mode,
                    const AcfString& title,
                    const AcfString& default_file_path,
                    const std::vector<AcfString>& accept_filters,
                    AcfRefPtr<AcfFileDialogCallback> callback) override;

  AcfRefPtr<AcfWebSocketClientHandler> CreateWebSocket(
      AcfRefPtr<AcfBrowser> browser,
      AcfString& target_url) override;

  void DidStartLoading(AcfRefPtr<AcfBrowser> browser) override;
  void DidStopLoading(AcfRefPtr<AcfBrowser> browser) override;
  void LoadProgressChanged(AcfRefPtr<AcfBrowser> browser,
                           double progress) override;

 private:
  LPVOID callback_;
  AcfRefPtr<AcfRegistration> devtool_observer_;

  IMPLEMENT_REFCOUNTING(BrowserHandler);
};

}  // namespace wrapper

#endif
