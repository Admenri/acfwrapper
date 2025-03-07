// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=7af5071524d902fd2c6d4c5d7193e0df4c6de77e$
//

#ifndef ACF_CPPTOC_COOKIE_VISITOR_CPPTOC_H_
#define ACF_CPPTOC_COOKIE_VISITOR_CPPTOC_H_
#pragma once

#include "include/acf_callback.h"
#include "include/acf_environment.h"
#include "include/acf_profile.h"
#include "include/acf_values.h"
#include "include/capi/acf_callback_capi.h"
#include "include/capi/acf_environment_capi.h"
#include "include/capi/acf_profile_capi.h"
#include "include/capi/acf_values_capi.h"
#include "libacf_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class AcfCookieVisitorCppToC
    : public AcfCppToCRefCounted<AcfCookieVisitorCppToC,
                                 AcfCookieVisitor,
                                 acf_cookie_visitor_t> {
 public:
  AcfCookieVisitorCppToC();
  virtual ~AcfCookieVisitorCppToC();
};

#endif  // ACF_CPPTOC_COOKIE_VISITOR_CPPTOC_H_
