// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=f665e9e4071447fb8c7b19368afe90f74901dc4d$
//

#ifndef ACF_CPPTOC_STRING_VISITOR_CPPTOC_H_
#define ACF_CPPTOC_STRING_VISITOR_CPPTOC_H_
#pragma once

#include "include/acf_callback.h"
#include "include/acf_values.h"
#include "include/capi/acf_callback_capi.h"
#include "include/capi/acf_values_capi.h"
#include "libacf_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class AcfStringVisitorCppToC
    : public AcfCppToCRefCounted<AcfStringVisitorCppToC,
                                 AcfStringVisitor,
                                 acf_string_visitor_t> {
 public:
  AcfStringVisitorCppToC();
  virtual ~AcfStringVisitorCppToC();
};

#endif  // ACF_CPPTOC_STRING_VISITOR_CPPTOC_H_
