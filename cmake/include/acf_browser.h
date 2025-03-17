// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_BROWSER_H_
#define ACF_INCLUDE_ACF_BROWSER_H_

#include "include/acf_callback.h"
#include "include/acf_context_menu.h"
#include "include/acf_registration.h"
#include "include/acf_values.h"
#include "include/acf_websocket.h"
#include "include/internal/acf_enums.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_list.h"
#include "include/internal/acf_types.h"
#include "include/internal/acf_types_wrappers.h"

class AcfFrame;
class AcfBrowser;
class AcfProfile;
class AcfEnvironment;
class AcfDictionaryValue;
class AcfNewWindowDelegate;
class AcfContextMenuCallback;
class AcfJavaScriptDialogCallback;
class AcfFileDialogCallback;
class AcfWebMessageFactory;
class AcfDevToolsMessageObserver;
class AcfRegistration;
class AcfWebSocketClientHandler;
class AcfFunctionFactory;

///
/// New Window delegate,
/// set a new browser to host target page.
/// Request will be handled while object was destroyed without handled.
///
/*--acf(source=library)--*/
class AcfNewWindowDelegate : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Set a browser to handle the new window
  /// new browser's profile must suit the host browser's profile,
  /// otherwise return false.
  ///
  /*--acf()--*/
  virtual bool SetNewBrowser(AcfRefPtr<AcfBrowser> browser) = 0;

  ///
  /// Get target url
  ///
  /*--acf()--*/
  virtual AcfString GetTargetURL() = 0;

  ///
  /// Put if user has been handled the request.
  /// This method will send the final request to browser.
  /// Must called ResumeNewWindowLoading() after the calling.
  ///
  /*--acf()--*/
  virtual void HandleRequest(bool handled, bool suspend_loading) = 0;
};

///
/// Context menu callback
///
/*--acf(source=library)--*/
class AcfContextMenuCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Continue show menu
  ///
  /*--acf(capi_name=cont)--*/
  virtual void Continue(bool suppress) = 0;
};

///
/// Javascript dialogs callback
///
/*--acf(source=library)--*/
class AcfJavaScriptDialogCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Continue dialogs
  ///
  /*--acf(capi_name=cont,optional_param=user_input)--*/
  virtual void Continue(bool success, const AcfString& user_input) = 0;
};

///
/// Callback interface for asynchronous continuation of file dialog requests.
///
/*--acf(source=library)--*/
class AcfFileDialogCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Continue the file selection. |file_paths| should be a single value or a
  /// list of values depending on the dialog mode. An empty |file_paths| value
  /// is treated the same as calling Cancel().
  ///
  /*--acf(capi_name=cont,optional_param=file_paths,optional_param=base_path)--*/
  virtual void Continue(bool use_default,
                        const std::vector<AcfString>& file_paths,
                        const AcfString& base_path) = 0;

  ///
  /// Cancel the file selection.
  ///
  /*--acf()--*/
  virtual void Cancel() = 0;
};

///
/// Network auth callback delegate
///
/*--acf(source=library)--*/
class AcfLoginDelegate : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Continue with username and password.
  ///
  /*--acf(optional_param=user_name,optional_param=password,capi_name=cont)--*/
  virtual void Continue(const AcfString& user_name,
                        const AcfString& password) = 0;

  ///
  /// Cancel auth request. (Default)
  ///
  /*--acf()--*/
  virtual void Cancel() = 0;
};

///
/// Callback interface used for asynchronous continuation of permission prompts.
///
/*--acf(source=library)--*/
class AcfPermissionPromptCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Complete the permissions request with the specified |result|.
  ///
  /*--acf(capi_name=cont)--*/
  virtual void Continue(acf_permission_request_result_t result) = 0;
};

///
/// Browser event list handler.
/// All event in this handler will be called on event thread.
///
/*--acf(source=client)--*/
class AcfBrowserHandler : public virtual AcfBaseRefCounted {
 public:
  typedef acf_new_window_source_t NewWindowSource;
  typedef acf_new_window_disposition_t NewWindowDisposition;
  typedef acf_navigation_types_t NavigationTypes;

  ///
  /// Called when browser has been initialized from environment.
  ///
  /*--acf()--*/
  virtual void OnBrowserCreated(AcfRefPtr<AcfBrowser> browser) {}

  ///
  /// The browser will open a new Browser as NewWindow
  ///
  /*--acf()--*/
  virtual void OnNewWindowRequest(AcfRefPtr<AcfBrowser> browser,
                                  NewWindowSource source,
                                  NewWindowDisposition disposition,
                                  const AcfPopupFeatures& popup_features,
                                  bool user_gesture,
                                  AcfRefPtr<AcfNewWindowDelegate> delegate) {}

  ///
  /// When the browser was sured to be destroyed,
  /// the browser will call this event.
  ///
  /*--acf()--*/
  virtual void OnBrowserDestroyed(AcfRefPtr<AcfBrowser> browser) {}

  ///
  /// Loading state changed (async)
  ///
  /*--acf()--*/
  virtual void OnLoadingStateChanged(AcfRefPtr<AcfBrowser> browser,
                                     bool show_loading_ui) {}

  ///
  /// Navigation state changed (title url icon historical)
  ///
  /*--acf()--*/
  virtual void OnNavigationStateChanged(AcfRefPtr<AcfBrowser> browser,
                                        NavigationTypes flags) {}

  ///
  /// Activate browser event.
  ///
  /*--acf()--*/
  virtual void OnActivateContents(AcfRefPtr<AcfBrowser> browser) {}

  ///
  /// Browser fullscreen state changed.
  ///
  /*--acf()--*/
  virtual void OnFullscreenStateChanged(AcfRefPtr<AcfBrowser> browser,
                                        bool fullscreen) {}

  ///
  /// Context menu request, return true for blocking menu popup
  ///
  /*--acf()--*/
  virtual void OnContextMenuRequest(
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfContextMenuParams> menu_params,
      AcfRefPtr<AcfContextMenuModel> menu_model,
      AcfRefPtr<AcfContextMenuCallback> callback) {}

  ///
  /// Context menu request to execute |command_id| associate item command.
  ///
  /*--acf()--*/
  virtual void OnContextMenuExecute(AcfRefPtr<AcfBrowser> browser,
                                    AcfRefPtr<AcfContextMenuParams> menu_params,
                                    int command_id,
                                    int event_flags) {}

  ///
  /// Called to run a file chooser dialog. |mode| represents the type of dialog
  /// to display. |title| to the title to be used for the dialog and may be
  /// empty to show the default title ("Open" or "Save" depending on the mode).
  /// |default_file_path| is the path with optional directory and/or file name
  /// component that should be initially selected in the dialog.
  /// |accept_filters| are used to restrict the selectable file types and may
  /// any combination of (a) valid lower-cased MIME types (e.g. "text/*" or
  /// "image/*"), (b) individual file extensions (e.g. ".txt" or ".png"), or (c)
  /// combined description and file extension delimited using "|" and ";" (e.g.
  /// "Image Types|.png;.gif;.jpg"). To display a custom dialog return true and
  /// execute |callback| either inline or at a later time. To display the
  /// default dialog return false.
  ///
  /*--acf(optional_param=title,optional_param=default_file_path,
          optional_param=accept_filters)--*/
  virtual void OnFileDialog(AcfRefPtr<AcfBrowser> browser,
                            acf_file_dialog_mode_t mode,
                            const AcfString& title,
                            const AcfString& default_file_path,
                            const std::vector<AcfString>& accept_filters,
                            AcfRefPtr<AcfFileDialogCallback> callback) {}

  ///
  /// Displays a JavaScript dialog.
  ///
  /*--acf(optional_param=frame,optional_param=message_text,optional_param=default_prompt_text)--*/
  virtual void OnJavaScriptDialog(
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfFrame> frame,
      acf_jsdialog_type_t dialog_type,
      const AcfString& message_text,
      const AcfString& default_prompt_text,
      AcfRefPtr<AcfJavaScriptDialogCallback> callback) {}

  ///
  /// Displays a dialog asking the user if they want to leave a page.
  ///
  /*--acf(optional_param=frame)--*/
  virtual void OnBeforeUnloadDialog(
      AcfRefPtr<AcfBrowser> browser,
      AcfRefPtr<AcfFrame> frame,
      bool is_reload,
      AcfRefPtr<AcfJavaScriptDialogCallback> callback) {}

  ///
  /// Accepts or dismisses the active JavaScript dialog, which must be owned
  /// by the given |web_contents|. If |prompt_override| is not null, the prompt
  /// text of the dialog should be set before accepting.
  ///
  /*--acf(optional_param=prompt_override)--*/
  virtual void OnHandleJavaScriptDialog(AcfRefPtr<AcfBrowser> browser,
                                        bool accept,
                                        const AcfString& prompt_override) {}

  ///
  /// Cancels all active and pending dialogs for the given WebContents. If
  /// |reset_state| is true, resets any saved state tied to |web_contents|.
  ///
  /*--acf()--*/
  virtual void OnCancelJavaScriptDialogs(AcfRefPtr<AcfBrowser> browser,
                                         bool reset_state) {}

  ///
  /// network need auth request (Called when disable default auth login dialog.)
  ///
  /*--acf(optional_param=host,optional_param=url,optional_param=scheme,optional_param=realm,optional_param=challenge)--*/
  virtual void OnAuthLoginRequest(AcfRefPtr<AcfBrowser> browser,
                                  bool is_proxy,
                                  const AcfString& host,
                                  const AcfString& url,
                                  const AcfString& scheme,
                                  const AcfString& realm,
                                  const AcfString& challenge,
                                  bool is_main_frame,
                                  AcfRefPtr<AcfLoginDelegate> delegate) {}

  ///
  /// Called when a page should show a permission prompt. |prompt_id| uniquely
  /// identifies the prompt. |requesting_origin| is the URL origin requesting
  /// permission. |requested_permissions| is a combination of values from
  /// acf_permission_request_types_t that represent the requested permissions.
  /// Return true and call AcfPermissionPromptCallback::Continue either in this
  /// method or at a later time to continue or cancel the request. Return false
  /// to proceed with default handling. With the Chrome runtime, default
  /// handling will display the permission prompt UI. With the Alloy runtime,
  /// default handling is ACF_PERMISSION_RESULT_IGNORE.
  ///
  /*--acf(optional_param=requesting_origin)--*/
  virtual void OnShowPermissionPrompt(
      AcfRefPtr<AcfBrowser> browser,
      uint64 prompt_id,
      const AcfString& requesting_origin,
      uint32 requested_permissions,
      AcfRefPtr<AcfPermissionPromptCallback> callback) {}

  ///
  /// Called when a permission prompt handled via OnShowPermissionPrompt is
  /// dismissed. |prompt_id| will match the value that was passed to
  /// OnShowPermissionPrompt. |result| will be the value passed to
  /// AcfPermissionPromptCallback::Continue or ACF_PERMISSION_RESULT_IGNORE if
  /// the dialog was dismissed for other reasons such as navigation, browser
  /// closure, etc. This method will not be called if OnShowPermissionPrompt
  /// returned false for |prompt_id|.
  ///
  /*--acf()--*/
  virtual void OnDismissPermissionPrompt(
      AcfRefPtr<AcfBrowser> browser,
      uint64 prompt_id,
      acf_permission_request_result_t result) {}

  ///
  /// Async running on browser navigation request, default: Continue(false).
  /// The request object is readonly without any post data and cookie data.
  ///
  /*--acf(optional_param=frame,optional_param=url,optional_param=referrer)--*/
  virtual void OnBeforeNavigation(AcfRefPtr<AcfBrowser> browser,
                                  AcfRefPtr<AcfFrame> frame,
                                  const AcfString& url,
                                  const AcfString& referrer,
                                  bool user_gesture,
                                  bool is_redirect,
                                  AcfRefPtr<AcfCallback> callback) {}

  ///
  /// Called to display a console message.
  ///
  /*--acf(optional_param=frame,optional_param=message,optional_param=source,optional_param=trace)--*/
  virtual void OnConsoleMessage(AcfRefPtr<AcfBrowser> browser,
                                AcfRefPtr<AcfFrame> frame,
                                int level,
                                const AcfString& message,
                                const AcfString& source,
                                int line,
                                const AcfString& trace) {}

  ///
  /// Called when the overall page loading progress has changed. |progress|
  /// ranges from 0.0 to 1.0.
  ///
  /*--acf()--*/
  virtual void OnLoadingProgressChange(AcfRefPtr<AcfBrowser> browser,
                                       double progress) {}

  ///
  /// Called when audible state was changed.
  ///
  /*--acf()--*/
  virtual void OnAudioStateChange(AcfRefPtr<AcfBrowser> browser, bool audible) {
  }

  ///
  /// Called when renderer request to create websocket connection.
  /// Allowed user to hook websocket connection. |target_url| can
  /// be modified in the event.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfWebSocketClientHandler> CreateWebSocket(
      AcfRefPtr<AcfBrowser> browser,
      AcfString& target_url) {
    return nullptr;
  }

  ///
  /// These three methods correspond to the points in time when any document in
  /// the frame tree starts loading for the first time (initiates outgoing
  /// requests), when incoming data subsequently starts arriving, and when the
  /// whole frame tree finishes loading.
  /// Notes:
  /// - There is no guarantee that calls to DidStartLoading/DidStopLoading are
  ///   interleaved (e.g. there can be 2 calls to DidStartLoading in a row).
  /// - These functions are different and unrelated from DidFinishLoad, which
  ///   is a notification about a specific document instead of the whole frame
  ///   tree, and uses a slightly different trigger to signify that the load had
  ///   finished.
  ///
  /*--acf()--*/
  virtual void DidStartLoading(AcfRefPtr<AcfBrowser> browser) {}

  ///
  /// See "DidStartLoading" handler description.
  ///
  /*--acf()--*/
  virtual void DidStopLoading(AcfRefPtr<AcfBrowser> browser) {}

  ///
  /// The page has made some progress loading. |progress| is a value between 0.0
  /// (nothing loaded) to 1.0 (page fully loaded).
  ///
  /*--acf()--*/
  virtual void LoadProgressChanged(AcfRefPtr<AcfBrowser> browser,
                                   double progress) {}
};

///
/// ACF's browser object host,
/// in fact a browser was a window in chromium with tabs control.
///
/*--acf(source=library)--*/
class AcfBrowser : public virtual AcfBaseRefCounted {
 public:
  typedef acf_mouse_button_type_t MouseButtonType;

  ///
  /// Is same object.
  ///
  /*--acf()--*/
  virtual bool IsSame(AcfRefPtr<AcfBrowser> that) = 0;

  ///
  /// Get browser create's sign.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfDictionaryValue> GetExtraInfo() = 0;

  ///
  /// Get parent environment.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfEnvironment> GetEnvironment() = 0;

  ///
  /// Close browser, it will call browser close event. (or unlaod dialog)
  ///
  /*--acf()--*/
  virtual void Close(bool force) = 0;

  ///
  /// Resume new window loading
  ///
  /*--acf()--*/
  virtual void ResumeLoadingBrowser() = 0;

  ///
  /// Get browser window handle.
  ///
  /*--acf()--*/
  virtual AcfWindowHandle GetWindowHandle() = 0;

  ///
  /// Get browser delegate invalid status.
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Browser navigation entry controller: can go back
  ///
  /*--acf()--*/
  virtual bool CanGoBack() = 0;

  ///
  /// Browser navigation entry controller: can go forward
  ///
  /*--acf()--*/
  virtual bool CanGoForward() = 0;

  ///
  /// Browser navigation entry controller: go back
  ///
  /*--acf()--*/
  virtual void GoBack() = 0;

  ///
  /// Browser navigation entry controller: go forward
  ///
  /*--acf()--*/
  virtual void GoForward() = 0;

  ///
  /// Browser navigation entry controller: reload
  ///
  /*--acf()--*/
  virtual void Reload(bool ignore_cache) = 0;

  ///
  /// Browser navigation entry controller: stop loading
  ///
  /*--acf()--*/
  virtual void Stop() = 0;

  ///
  /// Browser navigation entry controller: loading
  ///
  /*--acf()--*/
  virtual bool IsLoading() = 0;

  ///
  /// Navigate to target url (Main frame).
  ///
  /*--acf(optional_param=referrer_url,optional_param=extra_header)--*/
  virtual void LoadURL(const AcfString& url,
                       const AcfString& referrer_url,
                       acf_referrer_policy_t referrer_policy,
                       acf_transition_type_t transition_type,
                       const AcfString& extra_header) = 0;

  ///
  /// Get current url
  ///
  /*--acf()--*/
  virtual AcfString GetURL() = 0;

  ///
  /// Get title
  ///
  /*--acf()--*/
  virtual AcfString GetTitle() = 0;

  ///
  /// Get favicon url
  ///
  /*--acf()--*/
  virtual AcfString GetFavicon() = 0;

  ///
  /// Focus to native widget
  ///
  /*--acf()--*/
  virtual void Focus() = 0;

  ///
  /// Set/Get window visible
  ///
  /*--acf()--*/
  virtual void SetVisible(bool visible) = 0;

  ///
  /// Get visibility
  ///
  /*--acf()--*/
  virtual bool GetVisible() = 0;

  ///
  /// Get profile object, maybe null
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfProfile> GetProfile() = 0;

  ///
  /// Returns the number of frames that currently exist.
  ///
  /*--acf()--*/
  virtual size_t GetFrameCount() = 0;

  ///
  /// Returns the identifiers of all existing frames.
  ///
  /*--acf(count_func=identifiers:GetFrameCount)--*/
  virtual void GetFrameIdentifiers(std::vector<int64>& identifiers) = 0;

  ///
  /// Returns the names of all existing frames.
  ///
  /*--acf()--*/
  virtual void GetFrameNames(std::vector<AcfString>& names) = 0;

  ///
  /// Returns the frame with the specified identifier, or NULL if not found.
  ///
  /*--acf(capi_name=get_frame_byident)--*/
  virtual AcfRefPtr<AcfFrame> GetFrame(int64 identifier) = 0;

  ///
  /// Returns the frame with the specified name, or NULL if not found.
  ///
  /*--acf(optional_param=name)--*/
  virtual AcfRefPtr<AcfFrame> GetFrame(const AcfString& name) = 0;

  ///
  /// Returns the main (top-level) frame for the browser.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfFrame> GetMainFrame() = 0;

  ///
  /// Set browser additional settings. (Dynamic)
  ///
  /*--acf()--*/
  virtual void SetSettings(const AcfBrowserSettings& settings) = 0;

  ///
  /// Set audio mute state.
  ///
  /*--acf()--*/
  virtual void SetAudioMuted(bool muted) = 0;

  ///
  /// Get audio mute state.
  ///
  /*--acf()--*/
  virtual bool IsAudioMuted() = 0;

  ///
  /// Toggle devtools.
  ///
  /*--acf()--*/
  virtual void ToggleDevtools() = 0;

  ///
  /// Open a browser task manager.
  ///
  /*--acf()--*/
  virtual void OpenTaskManager() = 0;

  ///
  /// Raise find bar.
  ///
  /*--acf()--*/
  virtual void RaiseFindBar() = 0;

  ///
  /// Zoom page in current browser.
  ///
  /*--acf()--*/
  virtual void ZoomPage(acf_zoom_type_t zoom_type) = 0;

  ///
  /// Toggle extensions menu in current browser window.
  ///
  /*--acf()--*/
  virtual void ToggleExtensionsMenu() = 0;

  ///
  /// Toggle browser built-in download ui.
  ///
  /*--acf()--*/
  virtual void ToggleDownloadUIForWindow() = 0;

  ///
  /// Toggle media router bubble for media controller.
  ///
  /*--acf()--*/
  virtual void ToggleMediaRouterBubble() = 0;

  ///
  /// Send a key event to the browser.
  ///
  /*--acf()--*/
  virtual void SendKeyEvent(const AcfKeyEvent& event) = 0;

  ///
  /// Send a mouse click event to the browser. The |x| and |y| coordinates are
  /// relative to the upper-left corner of the view.
  ///
  /*--acf()--*/
  virtual void SendMouseClickEvent(const AcfMouseEvent& event,
                                   MouseButtonType type,
                                   bool mouseUp,
                                   int clickCount) = 0;

  ///
  /// Send a mouse move event to the browser. The |x| and |y| coordinates are
  /// relative to the upper-left corner of the view.
  ///
  /*--acf()--*/
  virtual void SendMouseMoveEvent(const AcfMouseEvent& event,
                                  bool mouseLeave) = 0;

  ///
  /// Send a mouse wheel event to the browser. The |x| and |y| coordinates are
  /// relative to the upper-left corner of the view. The |deltaX| and |deltaY|
  /// values represent the movement delta in the X and Y directions
  /// respectively.
  ///
  /*--acf()--*/
  virtual void SendMouseWheelEvent(const AcfMouseEvent& event,
                                   int deltaX,
                                   int deltaY) = 0;

  ///
  /// Add script before document created.
  /// Origins: * for all match urls
  ///
  /*--acf(optional_param=allow_origins)--*/
  virtual int AddDocumentStartJavaScript(
      const AcfString& script,
      const std::vector<AcfString>& allow_origins) = 0;

  ///
  /// Remove script by script-id.
  ///
  /*--acf()--*/
  virtual void RemoveDocumentStartJavaScript(int script_id) = 0;

  ///
  /// Create a Javascript object on renderer for web message routing.
  ///
  /*--acf()--*/
  virtual void AddWebMessageHostFactory(
      AcfRefPtr<AcfWebMessageFactory> factory,
      const AcfString& js_object_name,
      const std::vector<AcfString>& allow_origins) = 0;

  ///
  /// Remove host factory for web message.
  ///
  /*--acf()--*/
  virtual void RemoveWebMessageHostFactory(const AcfString& js_object_name) = 0;

  ///
  /// Create a Javascript object on renderer for web message routing.
  ///
  /*--acf()--*/
  virtual void AddFunctionRegisterHostFactory(
      AcfRefPtr<AcfFunctionFactory> factory,
      const AcfString& js_object_name,
      const AcfString& function_name,
      const std::vector<AcfString>& allow_origins) = 0;

  ///
  /// Remove host factory for web message.
  ///
  /*--acf()--*/
  virtual void RemoveFunctionRegisterHostFactory(
      const AcfString& js_object_name) = 0;

  ///
  /// Send a method call message over the DevTools protocol. |message| must be a
  /// UTF8-encoded JSON dictionary that contains "id" (int), "method" (string)
  /// and "params" (dictionary, optional) values. See the DevTools protocol
  /// documentation at https://chromedevtools.github.io/devtools-protocol/ for
  /// details of supported methods and the expected "params" dictionary
  /// contents. |message| will be copied if necessary. This method will return
  /// true if called on the UI thread and the message was successfully submitted
  /// for validation, otherwise false. Validation will be applied asynchronously
  /// and any messages that fail due to formatting errors or missing parameters
  /// may be discarded without notification. Prefer ExecuteDevToolsMethod if a
  /// more structured approach to message formatting is desired.
  ///
  /// Every valid method call will result in an asynchronous method result or
  /// error message that references the sent message "id". Event messages are
  /// received while notifications are enabled (for example, between method
  /// calls for "Page.enable" and "Page.disable"). All received messages will be
  /// delivered to the observer(s) registered with AddDevToolsMessageObserver.
  /// See AcfDevToolsMessageObserver::OnDevToolsMessage documentation for
  /// details of received message contents.
  ///
  /// Usage of the SendDevToolsMessage, ExecuteDevToolsMethod and
  /// AddDevToolsMessageObserver methods does not require an active DevTools
  /// front-end or remote-debugging session. Other active DevTools sessions will
  /// continue to function independently. However, any modification of global
  /// browser state by one session may not be reflected in the UI of other
  /// sessions.
  ///
  /// Communication with the DevTools front-end (when displayed) can be logged
  /// for development purposes by passing the
  /// `--devtools-protocol-log-file=<path>` command-line flag.
  ///
  /*--acf()--*/
  virtual void SendDevToolsMessage(const void* message,
                                   size_t message_size) = 0;

  ///
  /// Add an observer for DevTools protocol messages (method results and
  /// events). The observer will remain registered until the returned
  /// Registration object is destroyed. See the SendDevToolsMessage
  /// documentation for additional usage information.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfRegistration> AddDevToolsMessageObserver(
      AcfRefPtr<AcfDevToolsMessageObserver> observer) = 0;

  ///
  /// Begins a new composition or updates the existing composition. Blink has a
  /// special node (a composition node) that allows the input method to change
  /// text without affecting other DOM nodes. |text| is the optional text that
  /// will be inserted into the composition node. |underlines| is an optional
  /// set of ranges that will be underlined in the resulting text.
  /// |replacement_range| is an optional range of the existing text that will be
  /// replaced. |selection_range| is an optional range of the resulting text
  /// that will be selected after insertion or replacement. The
  /// |replacement_range| value is only used on OS X.
  ///
  /// This method may be called multiple times as the composition changes. When
  /// the client is done making changes the composition should either be
  /// canceled or completed. To cancel the composition call
  /// ImeCancelComposition. To complete the composition call either
  /// ImeCommitText or ImeFinishComposingText. Completion is usually signaled
  /// when:
  ///
  /// 1. The client receives a WM_IME_COMPOSITION message with a GCS_RESULTSTR
  ///    flag (on Windows), or;
  /// 2. The client receives a "commit" signal of GtkIMContext (on Linux), or;
  /// 3. insertText of NSTextInput is called (on Mac).
  ///
  /// This method is only used when window rendering is disabled.
  ///
  /*--acf(optional_param=text, optional_param=underlines)--*/
  virtual void ImeSetComposition(
      const AcfString& text,
      const std::vector<AcfCompositionUnderline>& underlines,
      const AcfRange& replacement_range,
      const AcfRange& selection_range) = 0;

  ///
  /// Completes the existing composition by optionally inserting the specified
  /// |text| into the composition node. |replacement_range| is an optional range
  /// of the existing text that will be replaced. |relative_cursor_pos| is where
  /// the cursor will be positioned relative to the current cursor position. See
  /// comments on ImeSetComposition for usage. The |replacement_range| and
  /// |relative_cursor_pos| values are only used on OS X.
  /// This method is only used when window rendering is disabled.
  ///
  /*--acf(optional_param=text)--*/
  virtual void ImeCommitText(const AcfString& text,
                             const AcfRange& replacement_range,
                             int relative_cursor_pos) = 0;

  ///
  /// Completes the existing composition by applying the current composition
  /// node contents. If |keep_selection| is false the current selection, if any,
  /// will be discarded. See comments on ImeSetComposition for usage. This
  /// method is only used when window rendering is disabled.
  ///
  /*--acf()--*/
  virtual void ImeFinishComposingText(bool keep_selection) = 0;

  ///
  /// Cancels the existing composition and discards the composition node
  /// contents without applying them. See comments on ImeSetComposition for
  /// usage.
  /// This method is only used when window rendering is disabled.
  ///
  /*--acf()--*/
  virtual void ImeCancelComposition() = 0;

  ///
  /// Capture screenshot from frame's widget view host. Return encoded png data.
  /// Handler value is AcfBinaryValue which data is the png result.
  ///
  /*--acf()--*/
  virtual void CaptureScreenshot(
      bool fast_encoded,
      bool from_surface,
      AcfRefPtr<AcfCompleteValueHandler> handler) = 0;

  ///
  /// Set browser renderer context profile info (environment features).
  ///
  /*--acf()--*/
  virtual void SetContextProfile(AcfRefPtr<AcfValue> context_profile) = 0;
};

#endif  //! ACF_INCLUDE_ACF_BROWSER_H_
