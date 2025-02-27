#pragma once

#include "include/acf_environment.h"

using namespace System;

namespace ACFSharpWrapper {

public
ref class ACFMainEntry {
 public:
  static String ^ GetWrapperVersion();
};

}  // namespace ACFSharpWrapper
