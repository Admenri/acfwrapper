// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_CONTEXTMENU_H_
#define ACF_INCLUDE_ACF_CONTEXTMENU_H_

#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_list.h"
#include "include/internal/acf_types.h"

///
/// Context menu controller
///
/*--acf(source=library)--*/
class AcfContextMenuModel : public virtual AcfBaseRefCounted {
 public:
  typedef acf_menu_item_type_t MenuItemType;

  ///
  /// Returns true if this menu is a submenu.
  ///
  /*--acf()--*/
  virtual bool IsSubMenu() = 0;

  ///
  /// Clears the menu. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool Clear() = 0;

  ///
  /// Returns the number of items in this menu.
  ///
  /*--acf()--*/
  virtual size_t GetCount() = 0;

  ///
  /// Insert a separator in the menu at the specified |index|. Returns true on
  /// success.
  ///
  /*--acf()--*/
  virtual bool InsertSeparatorAt(size_t index) = 0;

  ///
  /// Insert an item in the menu at the specified |index|. Returns true on
  /// success.
  ///
  /*--acf()--*/
  virtual bool InsertItemAt(size_t index,
                            int command_id,
                            const AcfString& label) = 0;

  ///
  /// Insert a check item in the menu at the specified |index|. Returns true on
  /// success.
  ///
  /*--acf()--*/
  virtual bool InsertCheckItemAt(size_t index,
                                 int command_id,
                                 const AcfString& label) = 0;

  ///
  /// Insert a radio item in the menu at the specified |index|. Only a single
  /// item with the specified |group_id| can be checked at a time. Returns true
  /// on success.
  ///
  /*--acf()--*/
  virtual bool InsertRadioItemAt(size_t index,
                                 int command_id,
                                 const AcfString& label,
                                 int group_id) = 0;

  ///
  /// Insert a sub-menu in the menu at the specified |index|. The new sub-menu
  /// is returned.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfContextMenuModel>
  InsertSubMenuAt(size_t index, int command_id, const AcfString& label) = 0;

  ///
  /// Removes the item at the specified |index|. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool RemoveAt(size_t index) = 0;

  ///
  /// Returns the command id at the specified |index| or -1 if not found due to
  /// invalid range or the index being a separator.
  ///
  /*--acf()--*/
  virtual int GetCommandIdAt(size_t index) = 0;

  ///
  /// Returns the index associated with the specified |command_id| or -1 if not
  /// found due to the command id not existing in the menu.
  ///
  /*--acf()--*/
  virtual int GetIndexOf(int command_id) = 0;

  ///
  /// Sets the command id at the specified |index|. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool SetCommandIdAt(size_t index, int command_id) = 0;

  ///
  /// Returns the label at the specified |index| or empty if not found due to
  /// invalid range or the index being a separator.
  ///
  /*--acf()--*/
  virtual AcfString GetLabelAt(size_t index) = 0;

  ///
  /// Set the label at the specified |index|. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool SetLabelAt(size_t index, const AcfString& label) = 0;

  ///
  /// Returns true if the specified |index| is visible.
  ///
  /*--acf()--*/
  virtual bool IsVisibleAt(size_t index) = 0;

  ///
  /// Change the visibility at the specified |index|. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool SetVisibleAt(size_t index, bool visible) = 0;

  ///
  /// Returns true if the specified |index| is enabled.
  ///
  /*--acf()--*/
  virtual bool IsEnabledAt(size_t index) = 0;

  ///
  /// Change the enabled status at the specified |index|. Returns true on
  /// success.
  ///
  /*--acf()--*/
  virtual bool SetEnabledAt(size_t index, bool enabled) = 0;

  ///
  /// Returns the group id at the specified |index| or -1 if invalid.
  ///
  /*--acf()--*/
  virtual int GetGroupIdAt(size_t index) = 0;

  ///
  /// Sets the group id at the specified |index|. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool SetGroupIdAt(size_t index, int group_id) = 0;

  ///
  /// Returns true if the specified |index| is checked. Only applies to check
  /// and radio items.
  ///
  /*--acf()--*/
  virtual bool IsCheckedAt(size_t index) = 0;

  ///
  /// Check the specified |index|. Only applies to check and radio items.
  /// Returns true on success.
  ///
  /*--acf()--*/
  virtual bool SetCheckedAt(size_t index, bool checked) = 0;

  ///
  /// Returns the submenu at the specified |index| or empty if invalid.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfContextMenuModel> GetSubMenuAt(size_t index) = 0;

  ///
  /// Returns the item type at the specified |index|.
  ///
  /*--acf(default_retval=MENUITEMTYPE_NONE)--*/
  virtual MenuItemType GetTypeAt(size_t index) = 0;
};

///
/// Provides information about the context menu state. The methods of this class
/// can only be accessed on browser process the UI thread.
///
/*--acf(source=library)--*/
class AcfContextMenuParams : public virtual AcfBaseRefCounted {
 public:
  typedef acf_context_menu_type_flags_t TypeFlags;
  typedef acf_context_menu_media_type_t MediaType;
  typedef acf_context_menu_media_state_flags_t MediaStateFlags;
  typedef acf_context_menu_edit_state_flags_t EditStateFlags;

  ///
  /// Returns the X coordinate of the mouse where the context menu was invoked.
  /// Coords are relative to the associated RenderView's origin.
  ///
  /*--acf()--*/
  virtual int GetXCoord() = 0;

  ///
  /// Returns the Y coordinate of the mouse where the context menu was invoked.
  /// Coords are relative to the associated RenderView's origin.
  ///
  /*--acf()--*/
  virtual int GetYCoord() = 0;

  ///
  /// Returns flags representing the type of node that the context menu was
  /// invoked on.
  ///
  /*--acf(default_retval=CM_TYPEFLAG_NONE)--*/
  virtual TypeFlags GetTypeFlags() = 0;

  ///
  /// Returns the URL of the link, if any, that encloses the node that the
  /// context menu was invoked on.
  ///
  /*--acf()--*/
  virtual AcfString GetLinkUrl() = 0;

  ///
  /// Returns the link URL, if any, to be used ONLY for "copy link address". We
  /// don't validate this field in the frontend process.
  ///
  /*--acf()--*/
  virtual AcfString GetUnfilteredLinkUrl() = 0;

  ///
  /// Returns the source URL, if any, for the element that the context menu was
  /// invoked on. Example of elements with source URLs are img, audio, and
  /// video.
  ///
  /*--acf()--*/
  virtual AcfString GetSourceUrl() = 0;

  ///
  /// Returns the URL of the top level page that the context menu was invoked
  /// on.
  ///
  /*--acf()--*/
  virtual AcfString GetPageUrl() = 0;

  ///
  /// Returns the URL of the subframe that the context menu was invoked on.
  ///
  /*--acf()--*/
  virtual AcfString GetFrameUrl() = 0;

  ///
  /// Returns the character encoding of the subframe that the context menu was
  /// invoked on.
  ///
  /*--acf()--*/
  virtual AcfString GetFrameCharset() = 0;

  ///
  /// Returns the type of context node that the context menu was invoked on.
  ///
  /*--acf(default_retval=CM_MEDIATYPE_NONE)--*/
  virtual MediaType GetMediaType() = 0;

  ///
  /// Returns flags representing the actions supported by the media element, if
  /// any, that the context menu was invoked on.
  ///
  /*--acf(default_retval=CM_MEDIAFLAG_NONE)--*/
  virtual MediaStateFlags GetMediaStateFlags() = 0;

  ///
  /// Returns the text of the selection, if any, that the context menu was
  /// invoked on.
  ///
  /*--acf()--*/
  virtual AcfString GetSelectionText() = 0;

  ///
  /// Returns true if the context menu was invoked on an editable node.
  ///
  /*--acf()--*/
  virtual bool IsEditable() = 0;

  ///
  /// Returns flags representing the actions supported by the editable node, if
  /// any, that the context menu was invoked on.
  ///
  /*--acf(default_retval=CM_EDITFLAG_NONE)--*/
  virtual EditStateFlags GetEditStateFlags() = 0;
};

#endif  // ACF_INCLUDE_CONTEXTMENU_H_
