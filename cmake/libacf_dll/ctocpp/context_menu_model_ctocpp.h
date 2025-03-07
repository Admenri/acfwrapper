// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=fb1e25637fc2cd17c96eed3dfbad1ce29a029892$
//

#ifndef ACF_CTOCPP_CONTEXT_MENU_MODEL_CTOCPP_H_
#define ACF_CTOCPP_CONTEXT_MENU_MODEL_CTOCPP_H_
#pragma once

#include "include/acf_context_menu.h"
#include "include/capi/acf_context_menu_capi.h"
#include "libacf_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class AcfContextMenuModelCToCpp
    : public AcfCToCppRefCounted<AcfContextMenuModelCToCpp,
                                 AcfContextMenuModel,
                                 acf_context_menu_model_t> {
 public:
  AcfContextMenuModelCToCpp();
  virtual ~AcfContextMenuModelCToCpp();

  // AcfContextMenuModel methods.
  bool IsSubMenu() override;
  bool Clear() override;
  size_t GetCount() override;
  bool InsertSeparatorAt(size_t index) override;
  bool InsertItemAt(size_t index,
                    int command_id,
                    const AcfString& label) override;
  bool InsertCheckItemAt(size_t index,
                         int command_id,
                         const AcfString& label) override;
  bool InsertRadioItemAt(size_t index,
                         int command_id,
                         const AcfString& label,
                         int group_id) override;
  AcfRefPtr<AcfContextMenuModel> InsertSubMenuAt(
      size_t index,
      int command_id,
      const AcfString& label) override;
  bool RemoveAt(size_t index) override;
  int GetCommandIdAt(size_t index) override;
  int GetIndexOf(int command_id) override;
  bool SetCommandIdAt(size_t index, int command_id) override;
  AcfString GetLabelAt(size_t index) override;
  bool SetLabelAt(size_t index, const AcfString& label) override;
  bool IsVisibleAt(size_t index) override;
  bool SetVisibleAt(size_t index, bool visible) override;
  bool IsEnabledAt(size_t index) override;
  bool SetEnabledAt(size_t index, bool enabled) override;
  int GetGroupIdAt(size_t index) override;
  bool SetGroupIdAt(size_t index, int group_id) override;
  bool IsCheckedAt(size_t index) override;
  bool SetCheckedAt(size_t index, bool checked) override;
  AcfRefPtr<AcfContextMenuModel> GetSubMenuAt(size_t index) override;
  MenuItemType GetTypeAt(size_t index) override;
};

#endif  // ACF_CTOCPP_CONTEXT_MENU_MODEL_CTOCPP_H_
