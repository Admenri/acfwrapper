// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=b6cd54fde2d0541b253b9d3445bc7753dd59dac5$
//

#include "libacf_dll/ctocpp/login_delegate_ctocpp.h"

// VIRTUAL METHODS - Body may be edited by hand.

void AcfLoginDelegateCToCpp::Continue(const AcfString& user_name,
                                      const AcfString& password) {
  acf_login_delegate_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, cont)) {
    return;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Unverified params: user_name, password

  // Execute
  _struct->cont(_struct, user_name.GetStruct(), password.GetStruct());
}

void AcfLoginDelegateCToCpp::Cancel() {
  acf_login_delegate_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, cancel)) {
    return;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->cancel(_struct);
}

// CONSTRUCTOR - Do not edit by hand.

AcfLoginDelegateCToCpp::AcfLoginDelegateCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

AcfLoginDelegateCToCpp::~AcfLoginDelegateCToCpp() {}

template <>
acf_login_delegate_t*
AcfCToCppRefCounted<AcfLoginDelegateCToCpp,
                    AcfLoginDelegate,
                    acf_login_delegate_t>::UnwrapDerived(AcfWrapperType type,
                                                         AcfLoginDelegate* c) {
  return nullptr;
}

template <>
AcfWrapperType AcfCToCppRefCounted<AcfLoginDelegateCToCpp,
                                   AcfLoginDelegate,
                                   acf_login_delegate_t>::kWrapperType =
    WT_LOGIN_DELEGATE;
