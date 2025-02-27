// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_INTERNAL_ACF_ENUMS_H_
#define ACF_INCLUDE_INTERNAL_ACF_ENUMS_H_
#pragma once

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
  /// New contents, included javascript 'window.open'
  /// or click on normal link
  NEW_CONTENTS = 0,

  /// Includes open url in menu context,
  /// and other chrome built-in operation.
  OPEN_FROM_TAB_URL,
} acf_new_window_source_t;

typedef enum {
  UNKNOWN,
  CURRENT_TAB,
  /// Indicates that only one tab with the url should exist in the same window.
  SINGLETON_TAB,
  NEW_FOREGROUND_TAB,
  NEW_BACKGROUND_TAB,
  NEW_POPUP,
  NEW_WINDOW,
  SAVE_TO_DISK,
  OFF_THE_RECORD,
  IGNORE_ACTION,
  /// Activates an existing tab containing the url, rather than navigating.
  /// This is similar to SINGLETON_TAB, but searches across all windows from
  /// the current profile and anonymity (instead of just the current one);
  /// closes the current tab on switching if the current tab was the NTP with
  /// no session history; and behaves like CURRENT_TAB instead of
  /// NEW_FOREGROUND_TAB when no existing tab is found.
  SWITCH_TO_TAB,
  /// Creates a new document picture-in-picture window showing a child WebView.
  NEW_PICTURE_IN_PICTURE,
  /// Update when adding a new disposition.
  MAX_VALUE = NEW_PICTURE_IN_PICTURE,
} acf_new_window_disposition_t;

typedef enum {
  INVALIDATE_TYPE_URL = 1 << 0,    /// The URL has changed.
  INVALIDATE_TYPE_TAB = 1 << 1,    /// The favicon, app icon, or crashed
                                   /// state changed.
  INVALIDATE_TYPE_LOAD = 1 << 2,   /// The loading state has changed.
  INVALIDATE_TYPE_TITLE = 1 << 3,  /// The title changed.
  INVALIDATE_TYPE_AUDIO = 1 << 4,  /// The tab became audible or
                                   /// inaudible.
                                   /// TODO(crbug.com/846374):
                                   /// remove this.

  INVALIDATE_TYPE_ALL = (1 << 5) - 1,
} acf_navigation_types_t;

///
/// Supported value types.
///
typedef enum {
  VTYPE_INVALID = 0,
  VTYPE_NULL,
  VTYPE_BOOL,
  VTYPE_INT,
  VTYPE_DOUBLE,
  VTYPE_STRING,
  VTYPE_BINARY,
  VTYPE_DICTIONARY,
  VTYPE_LIST,
} acf_value_type_t;

///
/// Cookie priority values.
///
typedef enum {
  ACF_COOKIE_PRIORITY_LOW = 0,
  ACF_COOKIE_PRIORITY_MEDIUM = 1,
  ACF_COOKIE_PRIORITY_HIGH = 2,
} acf_cookie_priority_t;

///
/// Cookie same site values.
///
typedef enum {
  ACF_COOKIE_SAME_SITE_UNSPECIFIED = -1,
  ACF_COOKIE_SAME_SITE_NO_RESTRICTION = 0,
  ACF_COOKIE_SAME_SITE_LAX_MODE = 1,
  ACF_COOKIE_SAME_SITE_STRICT_MODE = 2,
} acf_cookie_same_site_t;

///
/// Browsing data type enum
///
typedef enum {
  DATA_TYPE_ALL = 0,
  DATA_TYPE_APP_CACHE_DEPRECATED = 1 << 0,
  DATA_TYPE_FILE_SYSTEMS = 1 << 1,
  DATA_TYPE_INDEXED_DB = 1 << 2,
  DATA_TYPE_LOCAL_STORAGE = 1 << 3,
  DATA_TYPE_WEB_SQL = 1 << 4,
  DATA_TYPE_SERVICE_WORKERS = 1 << 5,
  DATA_TYPE_CACHE_STORAGE = 1 << 6,
  DATA_TYPE_EMBEDDER_DOM_STORAGE = 1 << 7,
  DATA_TYPE_COOKIES = 1 << 8,
  DATA_TYPE_CACHE = 1 << 10,
  DATA_TYPE_DOWNLOADS = 1 << 11,
  DATA_TYPE_MEDIA_LICENSES = 1 << 12,
  DATA_TYPE_NO_CHECKS = 1 << 13,
  DATA_TYPE_AVOID_CLOSING_CONNECTIONS = 1 << 15,
  DATA_TYPE_TRUST_TOKENS = 1 << 16,
  DATA_TYPE_ATTRIBUTION_REPORTING_SITE_CREATED = 1 << 17,
  DATA_TYPE_AGGREGATION_SERVICE = 1 << 18,
  DATA_TYPE_INTEREST_GROUPS = 1 << 19,
  DATA_TYPE_SHARED_STORAGE = 1 << 20,
  DATA_TYPE_ATTRIBUTION_REPORTING_INTERNAL = 1 << 21,
  DATA_TYPE_PRIVATE_AGGREGATION_INTERNAL = 1 << 22,
  DATA_TYPE_INTEREST_GROUPS_INTERNAL = 1 << 23,
} acf_browsing_data_type_t;

///
/// Policy for how the Referrer HTTP header value will be sent during
/// navigation. If the `--no-referrers` command-line flag is specified then the
/// policy value will be ignored and the Referrer value will never be sent. Must
/// be kept synchronized with net::URLRequest::ReferrerPolicy from Chromium.
///
typedef enum {
  REFERRER_POLICY_ALWAYS = 0,
  REFERRER_POLICY_DEFAULT = 1,
  REFERRER_POLICY_NO_REFERRER = 2,
  REFERRER_POLICY_NEVER = 3,
  REFERRER_POLICY_ORIGIN = 4,
  REFERRER_POLICY_ORIGIN_WHEN_CROSS = 5,
  REFERRER_POLICY_STRICT_ORIGIN_WHEN_CROSS = 6,
  REFERRER_POLICY_SAME_ORIGIN = 7,
  REFERRER_POLICY_STRICT_ORIGIN = 8,

  /// Always the last value in this enumeration.
  REFERRER_POLICY_LAST_VALUE = REFERRER_POLICY_STRICT_ORIGIN,
} acf_referrer_policy_t;

///
/// Resource type for a request. These constants match their equivalents in
/// Chromium's ResourceType and should not be renumbered.
///
typedef enum {
  ///
  /// Top level page.
  ///
  RT_MAIN_FRAME = 0,

  ///
  /// Frame or iframe.
  ///
  RT_SUB_FRAME,

  ///
  /// CSS stylesheet.
  ///
  RT_STYLESHEET,

  ///
  /// External script.
  ///
  RT_SCRIPT,

  ///
  /// Image (jpg/gif/png/etc).
  ///
  RT_IMAGE,

  ///
  /// Font.
  ///
  RT_FONT_RESOURCE,

  ///
  /// Some other subresource. This is the default type if the actual type is
  /// unknown.
  ///
  RT_SUB_RESOURCE,

  ///
  /// Object (or embed) tag for a plugin, or a resource that a plugin requested.
  ///
  RT_OBJECT,

  ///
  /// Media resource.
  ///
  RT_MEDIA,

  ///
  /// Main resource of a dedicated worker.
  ///
  RT_WORKER,

  ///
  /// Main resource of a shared worker.
  ///
  RT_SHARED_WORKER,

  ///
  /// Explicitly requested prefetch.
  ///
  RT_PREFETCH,

  ///
  /// Favicon.
  ///
  RT_FAVICON,

  ///
  /// XMLHttpRequest.
  ///
  RT_XHR,

  ///
  /// A request for a "<ping>".
  ///
  RT_PING,

  ///
  /// Main resource of a service worker.
  ///
  RT_SERVICE_WORKER,

  ///
  /// A report of Content Security Policy violations.
  ///
  RT_CSP_REPORT,

  ///
  /// A resource that a plugin requested.
  ///
  RT_PLUGIN_RESOURCE,

  ///
  /// A main-frame service worker navigation preload request.
  ///
  RT_NAVIGATION_PRELOAD_MAIN_FRAME = 19,

  ///
  /// A sub-frame service worker navigation preload request.
  ///
  RT_NAVIGATION_PRELOAD_SUB_FRAME,
} acf_resource_type_t;

///
/// Transition type for a request. Made up of one source value and 0 or more
/// qualifiers.
///
typedef enum {
  PAGE_TRANSITION_FIRST = 0,

  // User got to this page by clicking a link on another page.
  PAGE_TRANSITION_LINK = PAGE_TRANSITION_FIRST,

  // User got this page by typing the URL in the URL bar.  This should not be
  // used for cases where the user selected a choice that didn't look at all
  // like a URL; see GENERATED below.
  //
  // We also use this for other "explicit" navigation actions.
  PAGE_TRANSITION_TYPED = 1,

  // User got to this page through a suggestion in the UI, for example)
  // through the destinations page.
  PAGE_TRANSITION_AUTO_BOOKMARK = 2,

  // This is a subframe navigation. This is any content that is automatically
  // loaded in a non-toplevel frame. For example, if a page consists of
  // several frames containing ads, those ad URLs will have this transition
  // type. The user may not even realize the content in these pages is a
  // separate frame, so may not care about the URL (see MANUAL below). All
  // Fenced Frame navigations will be of this type because they are considered
  // a non-toplevel navigation that does not generate new navigation entries
  // in the back/forward list.
  PAGE_TRANSITION_AUTO_SUBFRAME = 3,

  // For subframe navigations that are explicitly requested by the user and
  // generate new navigation entries in the back/forward list. These are
  // probably more important than frames that were automatically loaded in
  // the background because the user probably cares about the fact that this
  // link was loaded.
  PAGE_TRANSITION_MANUAL_SUBFRAME = 4,

  // User got to this page by typing in the URL bar and selecting an entry
  // that did not look like a URL.  For example, a match might have the URL
  // of a Google search result page, but appear like "Search Google for ...".
  // These are not quite the same as TYPED navigations because the user
  // didn't type or see the destination URL.
  // See also KEYWORD.
  PAGE_TRANSITION_GENERATED = 5,

  // This is a toplevel navigation. This is any content that is automatically
  // loaded in a toplevel frame.  For example, opening a tab to show the ASH
  // screen saver, opening the devtools window, opening the NTP after the safe
  // browsing warning, opening web-based dialog boxes are examples of
  // AUTO_TOPLEVEL navigations.
  PAGE_TRANSITION_AUTO_TOPLEVEL = 6,

  // The user filled out values in a form and submitted it. NOTE that in
  // some situations submitting a form does not result in this transition
  // type. This can happen if the form uses script to submit the contents.
  PAGE_TRANSITION_FORM_SUBMIT = 7,

  // The user "reloaded" the page, either by hitting the reload button or by
  // hitting enter in the address bar.  NOTE: This is distinct from the
  // concept of whether a particular load uses "reload semantics" (i.e.
  // bypasses cached data).  For this reason, lots of code needs to pass
  // around the concept of whether a load should be treated as a "reload"
  // separately from their tracking of this transition type, which is mainly
  // used for proper scoring for consumers who care about how frequently a
  // user typed/visited a particular URL.
  //
  // SessionRestore and undo tab close use this transition type too.
  PAGE_TRANSITION_RELOAD = 8,

  // The url was generated from a replaceable keyword other than the default
  // search provider. If the user types a keyword (which also applies to
  // tab-to-search) in the omnibox this qualifier is applied to the transition
  // type of the generated url. TemplateURLModel then may generate an
  // additional visit with a transition type of KEYWORD_GENERATED against the
  // url 'http://' + keyword. For example, if you do a tab-to-search against
  // wikipedia the generated url has a transition qualifer of KEYWORD, and
  // TemplateURLModel generates a visit for 'wikipedia.org' with a transition
  // type of KEYWORD_GENERATED.
  PAGE_TRANSITION_KEYWORD = 9,

  // Corresponds to a visit generated for a keyword. See description of
  // KEYWORD for more details.
  PAGE_TRANSITION_KEYWORD_GENERATED = 10,

  // ADDING NEW CORE VALUE? Be sure to update the LAST_CORE and CORE_MASK
  // values below.  Also update CoreTransitionString().
  PAGE_TRANSITION_LAST_CORE = PAGE_TRANSITION_KEYWORD_GENERATED,
  PAGE_TRANSITION_CORE_MASK = 0xFF,

  // Qualifiers
  // Any of the core values above can be augmented by one or more qualifiers.
  // These qualifiers further define the transition.

  // The values 0x00200000 (PAGE_TRANSITION_FROM_API_3) and 0x00400000
  // (PAGE_TRANSITION_FROM_API_2) were used for experiments and were removed
  // around 6/2021. The experiments ended well before 6/2021, but it's possible
  // some databases still have the values. See https://crbug.com/1141501 for
  // more.

  // A managed user attempted to visit a URL but was blocked.
  PAGE_TRANSITION_BLOCKED = 0x00800000,

  // User used the Forward or Back button to navigate among browsing history.
  PAGE_TRANSITION_FORWARD_BACK = 0x01000000,

  // User used the address bar to trigger this navigation.
  PAGE_TRANSITION_FROM_ADDRESS_BAR = 0x02000000,

  // User is navigating to the home page.
  PAGE_TRANSITION_HOME_PAGE = 0x04000000,

  // The transition originated from an external application; the exact
  // definition of this is embedder dependent.
  PAGE_TRANSITION_FROM_API = 0x08000000,

  // The beginning of a navigation chain.
  PAGE_TRANSITION_CHAIN_START = 0x10000000,

  // The last transition in a redirect chain.
  PAGE_TRANSITION_CHAIN_END = 0x20000000,

  // Redirects caused by JavaScript or a meta refresh tag on the page.
  PAGE_TRANSITION_CLIENT_REDIRECT = 0x40000000,

  // Redirects sent from the server by HTTP headers. It might be nice to
  // break this out into 2 types in the future, permanent or temporary, if we
  // can get that information from WebKit.
  // TODO(crbug.com/40212666): Remove this as it's inaccurate.
  // NavigationHandle::WasServerRedirect() should be used instead.
  PAGE_TRANSITION_SERVER_REDIRECT = 0x80000000,

  // Used to test whether a transition involves a redirect.
  PAGE_TRANSITION_IS_REDIRECT_MASK = 0xC0000000,

  // General mask defining the bits used for the qualifiers.
  PAGE_TRANSITION_QUALIFIER_MASK = 0xFFFFFF00,
} acf_transition_type_t;

typedef enum {
  // For navigations.
  URLLOADER_FACTORY_TYPE_NAVIGATION = 0,

  // For downloads.
  URLLOADER_FACTORY_TYPE_DOWNLOAD,

  // For subresource requests from a document.
  URLLOADER_FACTORY_TYPE_DOCUMENT_SUB_RESOURCE,

  // For the main script request of a dedicated worker or shared worker.
  URLLOADER_FACTORY_TYPE_WORKER_MAIN_RESOURCE,

  // For the subresource requests from a dedicated worker or shared worker,
  // including importScripts().
  URLLOADER_FACTORY_TYPE_WORKER_SUB_RESOURCE,

  // For fetching a service worker main script or subresource scripts,
  // including importScripts().
  URLLOADER_FACTORY_TYPE_SERVICE_WORKER_SCRIPT,

  // For regular fetches from a service worker (e.g., fetch(), XHR), not
  // including importScripts().
  URLLOADER_FACTORY_TYPE_SERVICE_WORKER_SUB_RESOURCE,

  // For prefetches.
  URLLOADER_FACTORY_TYPE_PREFETCH,

  // For DevTools-initiated requests.
  URLLOADER_FACTORY_TYPE_DEVTOOLS,

  // For early hints.
  URLLOADER_FACTORY_TYPE_EARLY_HINTS,
} acf_url_loader_factory_type_t;

///
/// Return values for AcfResponseFilter::Filter().
///
typedef enum {
  ///
  /// Some or all of the pre-filter data was read successfully but more data is
  /// needed in order to continue filtering (filtered output is pending).
  ///
  RESPONSE_FILTER_NEED_MORE_DATA,

  ///
  /// Some or all of the pre-filter data was read successfully and all available
  /// filtered output has been written.
  ///
  RESPONSE_FILTER_DONE,

  ///
  /// An error occurred during filtering.
  ///
  RESPONSE_FILTER_ERROR
} acf_response_filter_status_t;

///
/// Flags that represent AcfURLRequest status.
///
typedef enum {
  ///
  /// Unknown status.
  ///
  UR_UNKNOWN = 0,

  ///
  /// Request succeeded.
  ///
  UR_SUCCESS,

  ///
  /// An IO request is pending, and the caller will be informed when it is
  /// completed.
  ///
  UR_IO_PENDING,

  ///
  /// Request was canceled programatically.
  ///
  UR_CANCELED,

  ///
  /// Request failed for some reason.
  ///
  UR_FAILED,
} acf_urlrequest_status_t;

///
/// Post data elements may represent either bytes or files.
///
typedef enum {
  PDE_TYPE_EMPTY = 0,
  PDE_TYPE_BYTES,
  PDE_TYPE_FILE,
} acf_postdataelement_type_t;

///
/// Response continue mode
///
typedef enum {
  // Continue the request.
  RESPONSE_CONTINUE,
  // Restart the request.
  RESPONSE_RESTART,
  // Cancel the request.
  RESPONSE_CANCEL
} acf_response_mode_t;

///
/// Supported context menu type flags.
///
typedef enum {
  ///
  /// No node is selected.
  ///
  CM_TYPEFLAG_NONE = 0,
  ///
  /// The top page is selected.
  ///
  CM_TYPEFLAG_PAGE = 1 << 0,
  ///
  /// A subframe page is selected.
  ///
  CM_TYPEFLAG_FRAME = 1 << 1,
  ///
  /// A link is selected.
  ///
  CM_TYPEFLAG_LINK = 1 << 2,
  ///
  /// A media node is selected.
  ///
  CM_TYPEFLAG_MEDIA = 1 << 3,
  ///
  /// There is a textual or mixed selection that is selected.
  ///
  CM_TYPEFLAG_SELECTION = 1 << 4,
  ///
  /// An editable element is selected.
  ///
  CM_TYPEFLAG_EDITABLE = 1 << 5,
} acf_context_menu_type_flags_t;

///
/// Supported menu item types.
///
typedef enum {
  MENUITEMTYPE_NONE = 0,
  MENUITEMTYPE_COMMAND,
  MENUITEMTYPE_CHECK,
  MENUITEMTYPE_RADIO,
  MENUITEMTYPE_SEPARATOR,
  MENUITEMTYPE_SUBMENU,
} acf_menu_item_type_t;

///
/// Supported context menu media types. These constants match their equivalents
/// in Chromium's ContextMenuDataMediaType and should not be renumbered.
///
typedef enum {
  ///
  /// No special node is in context.
  ///
  CM_MEDIATYPE_NONE = 0,
  ///
  /// An image node is selected.
  ///
  CM_MEDIATYPE_IMAGE,
  ///
  /// A video node is selected.
  ///
  CM_MEDIATYPE_VIDEO,
  ///
  /// An audio node is selected.
  ///
  CM_MEDIATYPE_AUDIO,
  ///
  /// An canvas node is selected.
  ///
  CM_MEDIATYPE_CANVAS,
  ///
  /// A file node is selected.
  ///
  CM_MEDIATYPE_FILE,
  ///
  /// A plugin node is selected.
  ///
  CM_MEDIATYPE_PLUGIN,
} acf_context_menu_media_type_t;

///
/// Supported context menu media state bit flags. These constants match their
/// equivalents in Chromium's ContextMenuData::MediaFlags and should not be
/// renumbered.
///
typedef enum {
  CM_MEDIAFLAG_NONE = 0,
  CM_MEDIAFLAG_IN_ERROR = 1 << 0,
  CM_MEDIAFLAG_PAUSED = 1 << 1,
  CM_MEDIAFLAG_MUTED = 1 << 2,
  CM_MEDIAFLAG_LOOP = 1 << 3,
  CM_MEDIAFLAG_CAN_SAVE = 1 << 4,
  CM_MEDIAFLAG_HAS_AUDIO = 1 << 5,
  CM_MEDIAFLAG_CAN_TOGGLE_CONTROLS = 1 << 6,
  CM_MEDIAFLAG_CONTROLS = 1 << 7,
  CM_MEDIAFLAG_CAN_PRINT = 1 << 8,
  CM_MEDIAFLAG_CAN_ROTATE = 1 << 9,
  CM_MEDIAFLAG_CAN_PICTURE_IN_PICTURE = 1 << 10,
  CM_MEDIAFLAG_PICTURE_IN_PICTURE = 1 << 11,
  CM_MEDIAFLAG_CAN_LOOP = 1 << 12,
} acf_context_menu_media_state_flags_t;

///
/// Supported context menu edit state bit flags. These constants match their
/// equivalents in Chromium's ContextMenuDataEditFlags and should not be
/// renumbered.
///
typedef enum {
  CM_EDITFLAG_NONE = 0,
  CM_EDITFLAG_CAN_UNDO = 1 << 0,
  CM_EDITFLAG_CAN_REDO = 1 << 1,
  CM_EDITFLAG_CAN_CUT = 1 << 2,
  CM_EDITFLAG_CAN_COPY = 1 << 3,
  CM_EDITFLAG_CAN_PASTE = 1 << 4,
  CM_EDITFLAG_CAN_DELETE = 1 << 5,
  CM_EDITFLAG_CAN_SELECT_ALL = 1 << 6,
  CM_EDITFLAG_CAN_TRANSLATE = 1 << 7,
  CM_EDITFLAG_CAN_EDIT_RICHLY = 1 << 8,
} acf_context_menu_edit_state_flags_t;

///
/// Supported file dialog modes.
///
typedef enum {
  ///
  /// Requires that the file exists before allowing the user to pick it.
  ///
  FILE_DIALOG_OPEN = 0,

  ///
  /// Like Open, but allows picking multiple files to open.
  ///
  FILE_DIALOG_OPEN_MULTIPLE,

  ///
  /// Like Open, but selects a folder to open.
  ///
  FILE_DIALOG_OPEN_FOLDER,

  ///
  /// Allows picking a nonexistent file, and prompts to overwrite if the file
  /// already exists.
  ///
  FILE_DIALOG_SAVE,
} acf_file_dialog_mode_t;

///
/// Supported JavaScript dialog types.
///
typedef enum {
  JSDIALOGTYPE_ALERT = 0,
  JSDIALOGTYPE_CONFIRM,
  JSDIALOGTYPE_PROMPT,
} acf_jsdialog_type_t;

///
/// Zoom type
///
typedef enum {
  ZOOM_IN = 0,
  ZOOM_RESET,
  ZOOM_OUT,
} acf_zoom_type_t;

///
/// Mouse button types.
///
typedef enum {
  MBT_LEFT = 0,
  MBT_MIDDLE,
  MBT_RIGHT,
} acf_mouse_button_type_t;

///
/// Key event types.
///
typedef enum {
  ///
  /// Notification that a key transitioned from "up" to "down".
  ///
  KEYEVENT_RAWKEYDOWN = 0,

  ///
  /// Notification that a key was pressed. This does not necessarily correspond
  /// to a character depending on the key and language. Use KEYEVENT_CHAR for
  /// character input.
  ///
  KEYEVENT_KEYDOWN,

  ///
  /// Notification that a key was released.
  ///
  KEYEVENT_KEYUP,

  ///
  /// Notification that a character was typed. Use this for text input. Key
  /// down events may generate 0, 1, or more than one character event depending
  /// on the key, locale, and operating system.
  ///
  KEYEVENT_CHAR
} acf_key_event_type_t;

typedef enum {
  EVENTFLAG_NONE = 0,
  EVENTFLAG_CAPS_LOCK_ON = 1 << 0,
  EVENTFLAG_SHIFT_DOWN = 1 << 1,
  EVENTFLAG_CONTROL_DOWN = 1 << 2,
  EVENTFLAG_ALT_DOWN = 1 << 3,
  EVENTFLAG_LEFT_MOUSE_BUTTON = 1 << 4,
  EVENTFLAG_MIDDLE_MOUSE_BUTTON = 1 << 5,
  EVENTFLAG_RIGHT_MOUSE_BUTTON = 1 << 6,
  /// Mac OS-X command key.
  EVENTFLAG_COMMAND_DOWN = 1 << 7,
  EVENTFLAG_NUM_LOCK_ON = 1 << 8,
  EVENTFLAG_IS_KEY_PAD = 1 << 9,
  EVENTFLAG_IS_LEFT = 1 << 10,
  EVENTFLAG_IS_RIGHT = 1 << 11,
  EVENTFLAG_ALTGR_DOWN = 1 << 12,
  EVENTFLAG_IS_REPEAT = 1 << 13,
} acf_event_flags_t;

///
/// Permission types used with OnShowPermissionPrompt. Some types are
/// platform-specific or only supported with the Chrome runtime. Should be kept
/// in sync with Chromium's permissions::RequestType type.
///
typedef enum {
  PT_PERMISSION_TYPE_NONE = 0,
  PT_PERMISSION_TYPE_AR_SESSION = 1 << 1,
  PT_PERMISSION_TYPE_CAMERA_PAN_TILT_ZOOM = 1 << 2,
  PT_PERMISSION_TYPE_CAMERA_STREAM = 1 << 3,
  PT_PERMISSION_TYPE_CAPTURED_SURFACE_CONTROL = 1 << 4,
  PT_PERMISSION_TYPE_CLIPBOARD = 1 << 5,
  PT_PERMISSION_TYPE_TOP_LEVEL_STORAGE_ACCESS = 1 << 6,
  PT_PERMISSION_TYPE_DISK_QUOTA = 1 << 7,
  PT_PERMISSION_TYPE_FILE_SYSTEM_ACCESS = 1 << 8,
  PT_PERMISSION_TYPE_GEOLOCATION = 1 << 9,
  PT_PERMISSION_TYPE_HAND_TRACKING = 1 << 10,
  PT_PERMISSION_TYPE_IDENTITY_PROVIDER = 1 << 11,
  PT_PERMISSION_TYPE_IDLE_DETECTION = 1 << 12,
  PT_PERMISSION_TYPE_LOCAL_FONTS = 1 << 13,
  PT_PERMISSION_TYPE_MIC_STREAM = 1 << 14,
  PT_PERMISSION_TYPE_MIDI_SYSEX = 1 << 15,
  PT_PERMISSION_TYPE_MULTIPLE_DOWNLOADS = 1 << 16,
  PT_PERMISSION_TYPE_NFC_DEVICE = 1 << 17,
  PT_PERMISSION_TYPE_NOTIFICATIONS = 1 << 18,
  PT_PERMISSION_TYPE_KEYBOARD_LOCK = 1 << 19,
  PT_PERMISSION_TYPE_POINTER_LOCK = 1 << 20,
  PT_PERMISSION_TYPE_PROTECTED_MEDIA_IDENTIFIER = 1 << 21,
  PT_PERMISSION_TYPE_REGISTER_PROTOCOL_HANDLER = 1 << 22,
  PT_PERMISSION_TYPE_SMART_CARD = 1 << 23,
  PT_PERMISSION_TYPE_STORAGE_ACCESS = 1 << 24,
  PT_PERMISSION_TYPE_VR_SESSION = 1 << 25,
  PT_PERMISSION_TYPE_WEB_APP_INSTALLATION = 1 << 26,
  PT_PERMISSION_TYPE_WEB_PRINTING = 1 << 27,
  PT_PERMISSION_TYPE_WINDOW_MANAGEMENT = 1 << 28,
} acf_permission_request_types_t;

///
/// Permission request results.
///
typedef enum {
  ///
  /// Accept the permission request as an explicit user action.
  ///
  PR_PERMISSION_RESULT_ACCEPT = 0,

  ///
  /// Deny the permission request as an explicit user action.
  ///
  PR_PERMISSION_RESULT_DENY,

  ///
  /// Dismiss the permission request as an explicit user action.
  ///
  PR_PERMISSION_RESULT_DISMISS,

  ///
  /// Ignore the permission request. If the prompt remains unhandled (e.g.
  /// OnShowPermissionPrompt returns false and there is no default permissions
  /// UI) then any related promises may remain unresolved.
  ///
  PR_PERMISSION_RESULT_IGNORE,
} acf_permission_request_result_t;

///
// DOM document types.
///
typedef enum {
  DOM_DOCUMENT_TYPE_UNKNOWN = 0,
  DOM_DOCUMENT_TYPE_HTML,
  DOM_DOCUMENT_TYPE_XHTML,
  DOM_DOCUMENT_TYPE_PLUGIN,
} acf_dom_document_type_t;

///
// DOM node types.
///
typedef enum {
  DOM_NODE_TYPE_UNSUPPORTED = 0,
  DOM_NODE_TYPE_ELEMENT,
  DOM_NODE_TYPE_ATTRIBUTE,
  DOM_NODE_TYPE_TEXT,
  DOM_NODE_TYPE_CDATA_SECTION,
  DOM_NODE_TYPE_PROCESSING_INSTRUCTIONS,
  DOM_NODE_TYPE_COMMENT,
  DOM_NODE_TYPE_DOCUMENT,
  DOM_NODE_TYPE_DOCUMENT_TYPE,
  DOM_NODE_TYPE_DOCUMENT_FRAGMENT,
} acf_dom_node_type_t;

///
/// Composition underline style.
///
typedef enum {
  CUS_SOLID = 0,
  CUS_DOT,
  CUS_DASH,
  CUS_NONE,
} acf_composition_underline_style_t;

///
/// Flags used to customize the behavior of AcfURLRequest.
///
typedef enum {
  ///
  /// Default behavior.
  ///
  UR_FLAG_NONE = 0,

  ///
  /// If set the cache will be skipped when handling the request. Setting this
  /// value is equivalent to specifying the "Cache-Control: no-cache" request
  /// header. Setting this value in combination with UR_FLAG_ONLY_FROM_CACHE
  /// will cause the request to fail.
  ///
  UR_FLAG_SKIP_CACHE = 1 << 0,

  ///
  /// If set the request will fail if it cannot be served from the cache (or
  /// some equivalent local store). Setting this value is equivalent to
  /// specifying the "Cache-Control: only-if-cached" request header. Setting
  /// this value in combination with UR_FLAG_SKIP_CACHE or UR_FLAG_DISABLE_CACHE
  /// will cause the request to fail.
  ///
  UR_FLAG_ONLY_FROM_CACHE = 1 << 1,

  ///
  /// If set the cache will not be used at all. Setting this value is equivalent
  /// to specifying the "Cache-Control: no-store" request header. Setting this
  /// value in combination with UR_FLAG_ONLY_FROM_CACHE will cause the request
  /// to fail.
  ///
  UR_FLAG_DISABLE_CACHE = 1 << 2,

  ///
  /// If set user name, password, and cookies may be sent with the request, and
  /// cookies may be saved from the response.
  ///
  UR_FLAG_ALLOW_STORED_CREDENTIALS = 1 << 3,

  ///
  /// If set upload progress events will be generated when a request has a body.
  ///
  UR_FLAG_REPORT_UPLOAD_PROGRESS = 1 << 4,

  ///
  /// If set the AcfURLRequestClient::OnDownloadData method will not be called.
  ///
  UR_FLAG_NO_DOWNLOAD_DATA = 1 << 5,

  ///
  /// If set 5XX redirect errors will be propagated to the observer instead of
  /// automatically re-tried. This currently only applies for requests
  /// originated in the browser process.
  ///
  UR_FLAG_NO_RETRY_ON_5XX = 1 << 6,

  ///
  /// If set 3XX responses will cause the fetch to halt immediately rather than
  /// continue through the redirect.
  ///
  UR_FLAG_STOP_ON_REDIRECT = 1 << 7,
} acf_urlrequest_flags_t;

///
/// URI unescape rules passed to AcfURIDecode().
///
typedef enum {
  ///
  /// Don't unescape anything at all.
  ///
  UU_NONE = 0,

  ///
  /// Don't unescape anything special, but all normal unescaping will happen.
  /// This is a placeholder and can't be combined with other flags (since it's
  /// just the absence of them). All other unescape rules imply "normal" in
  /// addition to their special meaning. Things like escaped letters, digits,
  /// and most symbols will get unescaped with this mode.
  ///
  UU_NORMAL = 1 << 0,

  ///
  /// Convert %20 to spaces. In some places where we're showing URLs, we may
  /// want this. In places where the URL may be copied and pasted out, then
  /// you wouldn't want this since it might not be interpreted in one piece
  /// by other applications.
  ///
  UU_SPACES = 1 << 1,

  ///
  /// Unescapes '/' and '\\'. If these characters were unescaped, the resulting
  /// URL won't be the same as the source one. Moreover, they are dangerous to
  /// unescape in strings that will be used as file paths or names. This value
  /// should only be used when slashes don't have special meaning, like data
  /// URLs.
  ///
  UU_PATH_SEPARATORS = 1 << 2,

  ///
  /// Unescapes various characters that will change the meaning of URLs,
  /// including '%', '+', '&', '#'. Does not unescape path separators.
  /// If these characters were unescaped, the resulting URL won't be the same
  /// as the source one. This flag is used when generating final output like
  /// filenames for URLs where we won't be interpreting as a URL and want to do
  /// as much unescaping as possible.
  ///
  UU_URL_SPECIAL_CHARS_EXCEPT_PATH_SEPARATORS = 1 << 3,

  ///
  /// URL queries use "+" for space. This flag controls that replacement.
  ///
  UU_REPLACE_PLUS_WITH_SPACE = 1 << 4,
} acf_uri_unescape_rule_t;

///
/// Options that can be passed to AcfParseJSON.
///
typedef enum {
  ///
  /// Parses the input strictly according to RFC 4627. See comments in
  /// Chromium's base/json/json_reader.h file for known limitations/
  /// deviations from the RFC.
  ///
  JSON_PARSER_RFC = 0,

  ///
  /// Allows commas to exist after the last element in structures.
  ///
  JSON_PARSER_ALLOW_TRAILING_COMMAS = 1 << 0,
} acf_json_parser_options_t;

///
/// Options that can be passed to AcfWriteJSON.
///
typedef enum {
  ///
  /// Default behavior.
  ///
  JSON_WRITER_DEFAULT = 0,

  ///
  /// This option instructs the writer that if a Binary value is encountered,
  /// the value (and key if within a dictionary) will be omitted from the
  /// output, and success will be returned. Otherwise, if a binary value is
  /// encountered, failure will be returned.
  ///
  JSON_WRITER_OMIT_BINARY_VALUES = 1 << 0,

  ///
  /// This option instructs the writer to write doubles that have no fractional
  /// part as a normal integer (i.e., without using exponential notation
  /// or appending a '.0') as long as the value is within the range of a
  /// 64-bit int.
  ///
  JSON_WRITER_OMIT_DOUBLE_TYPE_PRESERVATION = 1 << 1,

  ///
  /// Return a slightly nicer formatted json string (pads with whitespace to
  /// help with readability).
  ///
  JSON_WRITER_PRETTY_PRINT = 1 << 2,
} acf_json_writer_options_t;

///
/// Websocket message send direction
///
typedef enum {
  WEBSOCKET_MESSAGE_NETWORK_SERVICE = 0,
  WEBSOCKET_MESSAGE_RENDERER,
} acf_websocket_message_routing_t;

#endif  //! ACF_INCLUDE_INTERNAL_ACF_ENUMS_H_
