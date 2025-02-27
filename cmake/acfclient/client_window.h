#ifndef ACF_CLIENT_WINDOW_H_
#define ACF_CLIENT_WINDOW_H_

#include <windows.h>

#include <iostream>
#include <string>
#include <vector>

#include "include/acf_browser.h"
#include "include/acf_profile.h"

namespace acfclient {

class Window : public AcfBrowserHandler {
 public:
  Window(AcfRefPtr<AcfEnvironment> env,
         AcfRefPtr<AcfProfile> profile,
         HINSTANCE instance,
         RECT* initial_rect,
         AcfRefPtr<AcfNewWindowDelegate> new_window = nullptr);
  ~Window() override;

  // Interface
  void Show();
  void Hide();

  HWND GetHandle();
  HWND GetEditHandle() { return edit_control_; }

  RECT GetBrowserBound();
  void SetBrowser(HWND handle);

 public:
  // Event
  void OnCreate(HWND window);
  void OnSize(HWND window);
  bool OnCommand(UINT id);
  void OnFocus();
  void OnClose();
  void CreateMenu();

 protected:
  // BrowserHandler override
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
  void OnContextMenuRequest(
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfContextMenuParams> menu_params,
      AcfRefPtr<AcfContextMenuModel> menu_model,
      AcfRefPtr<AcfContextMenuCallback> callback) override;
  void OnContextMenuExecute(AcfRefPtr<AcfBrowser> browser,
                            AcfRefPtr<AcfContextMenuParams> menu_params,
                            int command_id,
                            int event_flags) override;

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
  void OnBeforeNavigation(AcfRefPtr<AcfBrowser> browser,
                          AcfRefPtr<AcfFrame> frame,
                          const AcfString& url,
                          const AcfString& referrer,
                          bool user_gesture,
                          bool is_redirect,
                          AcfRefPtr<AcfCallback> callback) override;

  void OnAuthLoginRequest(AcfRefPtr<AcfBrowser> browser,
                          bool is_proxy,
                          const AcfString& host,
                          const AcfString& url,
                          const AcfString& scheme,
                          const AcfString& realm,
                          const AcfString& challenge,
                          bool is_main_frame,
                          AcfRefPtr<AcfLoginDelegate> delegate) override;

  AcfRefPtr<AcfWebSocketClientHandler> CreateWebSocket(
      AcfRefPtr<AcfBrowser> browser,
      AcfString& target_url) override;

  AcfBrowser* browser() { return browser_weak_ptr_; }

 private:
  void RegisterWindowClass();
  static LRESULT CALLBACK MainWndProc(HWND hWnd,
                                      UINT message,
                                      WPARAM wParam,
                                      LPARAM lParam);

  // host
  HWND window_;
  HINSTANCE instance_;

  // control
  HWND back_button_;
  HWND forward_button_;
  HWND reload_button_;
  HWND stop_button_;
  [[maybe_unused]] HWND nav_button_;
  HWND edit_control_;

  [[maybe_unused]] HWND more_button_;
  HMENU more_menu_;

  HWND browser_window_ = nullptr;
  AcfBrowser* browser_weak_ptr_ = nullptr;

  AcfRefPtr<AcfNewWindowDelegate> new_browser_delegate_;
  AcfRefPtr<AcfRegistration> devtools_observer_;

  IMPLEMENT_REFCOUNTING(Window);
};

}  // namespace acfclient

#endif
