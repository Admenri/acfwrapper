// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=fd286c966f5f8f11f16d77878ce423189a4acb14$
//

#ifndef ACF_CPPTOC_DOMNODE_CALLBACK_CPPTOC_H_
#define ACF_CPPTOC_DOMNODE_CALLBACK_CPPTOC_H_
#pragma once

#include "include/acf_browser.h"
#include "include/acf_callback.h"
#include "include/acf_dom.h"
#include "include/acf_frame.h"
#include "include/acf_values.h"
#include "include/capi/acf_browser_capi.h"
#include "include/capi/acf_callback_capi.h"
#include "include/capi/acf_dom_capi.h"
#include "include/capi/acf_frame_capi.h"
#include "include/capi/acf_values_capi.h"
#include "libacf_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class AcfDOMNodeCallbackCppToC
    : public AcfCppToCRefCounted<AcfDOMNodeCallbackCppToC,
                                 AcfDOMNodeCallback,
                                 acf_domnode_callback_t> {
 public:
  AcfDOMNodeCallbackCppToC();
  virtual ~AcfDOMNodeCallbackCppToC();
};

#endif  // ACF_CPPTOC_DOMNODE_CALLBACK_CPPTOC_H_
