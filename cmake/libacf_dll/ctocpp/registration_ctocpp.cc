// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=fbf557a792225c7b3e7d2f07cc4bb4b74d3bb1fb$
//

#include "libacf_dll/ctocpp/registration_ctocpp.h"

// CONSTRUCTOR - Do not edit by hand.

AcfRegistrationCToCpp::AcfRegistrationCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

AcfRegistrationCToCpp::~AcfRegistrationCToCpp() {}

template <>
acf_registration_t*
AcfCToCppRefCounted<AcfRegistrationCToCpp,
                    AcfRegistration,
                    acf_registration_t>::UnwrapDerived(AcfWrapperType type,
                                                       AcfRegistration* c) {
  return nullptr;
}

template <>
AcfWrapperType AcfCToCppRefCounted<AcfRegistrationCToCpp,
                                   AcfRegistration,
                                   acf_registration_t>::kWrapperType =
    WT_REGISTRATION;
