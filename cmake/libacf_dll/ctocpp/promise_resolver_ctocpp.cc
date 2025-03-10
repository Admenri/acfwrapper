// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=1c5c4bddd7955c749836143a3d062bf036ccff33$
//

#include "libacf_dll/ctocpp/promise_resolver_ctocpp.h"

#include "libacf_dll/ctocpp/value_ctocpp.h"

// VIRTUAL METHODS - Body may be edited by hand.

void AcfPromiseResolverCToCpp::Resolve(AcfRefPtr<AcfValue> return_value) {
  acf_promise_resolver_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, resolve)) {
    return;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Unverified params: return_value

  // Execute
  _struct->resolve(_struct, AcfValueCToCpp::Unwrap(return_value));
}

// CONSTRUCTOR - Do not edit by hand.

AcfPromiseResolverCToCpp::AcfPromiseResolverCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

AcfPromiseResolverCToCpp::~AcfPromiseResolverCToCpp() {}

template <>
acf_promise_resolver_t* AcfCToCppRefCounted<
    AcfPromiseResolverCToCpp,
    AcfPromiseResolver,
    acf_promise_resolver_t>::UnwrapDerived(AcfWrapperType type,
                                           AcfPromiseResolver* c) {
  return nullptr;
}

template <>
AcfWrapperType AcfCToCppRefCounted<AcfPromiseResolverCToCpp,
                                   AcfPromiseResolver,
                                   acf_promise_resolver_t>::kWrapperType =
    WT_PROMISE_RESOLVER;
