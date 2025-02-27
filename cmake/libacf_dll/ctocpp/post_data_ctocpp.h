// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=5618d4a1d2b820b4cd5f57349e5ab9c324618a25$
//

#ifndef ACF_CTOCPP_POST_DATA_CTOCPP_H_
#define ACF_CTOCPP_POST_DATA_CTOCPP_H_
#pragma once

#include "include/acf_request.h"
#include "include/capi/acf_request_capi.h"
#include "libacf_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class AcfPostDataCToCpp : public AcfCToCppRefCounted<AcfPostDataCToCpp,
                                                     AcfPostData,
                                                     acf_post_data_t> {
 public:
  AcfPostDataCToCpp();
  virtual ~AcfPostDataCToCpp();

  // AcfPostData methods.
  bool IsReadOnly() override;
  bool HasExcludedElements() override;
  size_t GetElementCount() override;
  void GetElements(ElementVector& elements) override;
  bool RemoveElement(AcfRefPtr<AcfPostDataElement> element) override;
  bool AddElement(AcfRefPtr<AcfPostDataElement> element) override;
  void RemoveElements() override;
};

#endif  // ACF_CTOCPP_POST_DATA_CTOCPP_H_
