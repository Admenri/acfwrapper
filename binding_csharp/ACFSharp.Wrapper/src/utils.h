#pragma once

#include <sstream>
#include <vector>

#include "include/internal/acf_string.h"

#include "vcclr_local.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

namespace ACFSharpWrapper {
namespace Internals {

private
class StringUtils {
 public:
  /// <summary>
  /// Converts an unmanaged string to a (managed) .NET string.
  /// </summary>
  /// <param name="acfStr">The string that should be converted.</param>
  /// <returns>A .NET string.</returns>
  [DebuggerStepThrough] static String ^
      StringUtils::ToClr(const acf_string_t& acfStr) {
    AcfString local_str(&acfStr);
    auto str = reinterpret_cast<const wchar_t*>(local_str.ToString16().c_str());

    return gcnew String(str);
  }

  /// <summary>
  /// Converts an unmanaged string to a (managed) .NET string.
  /// </summary>
  /// <param name="acfStr">The string that should be converted.</param>
  /// <returns>A .NET string.</returns>
  [DebuggerStepThrough] static String ^
      StringUtils::ToClr(const AcfString& acfStr) {
    auto str = reinterpret_cast<const wchar_t*>(acfStr.ToString16().c_str());

    return gcnew String(str);
  }

  /// <summary>
  /// Converts an unmanaged vector of strings to a (managed) .NET List of
  /// strings.
  /// </summary>
  /// <param name="acfStr">The vector of strings that should be
  /// converted.</param> <returns>A .NET List of strings.</returns>
  [DebuggerStepThrough] static List<String ^> ^
      ToClr(const std::vector<AcfString>& acfStr) {
        auto result = gcnew List<String ^>();

        for each (AcfString s in acfStr) {
          result->Add(StringUtils::ToClr(s));
        }

        return result;
      }

          /// <summary>
          /// Converts a .NET string to native (unmanaged) format. Note that
          /// this method does not allocate a new copy of the
          // string, but rather returns a pointer to the memory in the existing
          // managed String object.
          /// </summary>
          /// <param name="str">The string that should be converted.</param>
          /// <returns>An unmanaged representation of the provided string, or an
          /// empty string if the input string is a nullptr.</returns>
          [DebuggerStepThrough] static AcfString ToNative(String ^ str) {
    if (str == nullptr) {
      return AcfString();
    }

    pin_ptr<const wchar_t> pStr = PtrToStringChars(str);
    AcfStringUTF16 acfStr(reinterpret_cast<const char16_t*>(pStr));
    return acfStr.ToString();
  }

  /// <summary>
  /// Converts a .NET List of strings to native (unmanaged) format.
  /// </summary>
  /// <param name="str">The List of strings that should be converted.</param>
  /// <returns>An unmanaged representation of the provided List of strings, or
  /// an empty List if the input is a nullptr.</returns>
  [DebuggerStepThrough] static std::vector<AcfString> ToNative(
      IEnumerable<String ^> ^ str) {
    if (str == nullptr) {
      return std::vector<AcfString>();
    }

    std::vector<AcfString> result = std::vector<AcfString>();

    for each (String ^ s in str) {
      result.push_back(StringUtils::ToNative(s));
    }

    return result;
  }

      /// <summary>
      /// Assigns the provided acf_string_t object from the given .NET string.
      /// </summary>
      /// <param name="acfStr">The acf_string_t that should be updated.</param>
      /// <param name="str">The .NET string whose value should be used to update
      /// acfStr.</param>
      [DebuggerStepThrough] static void StringUtils::AssignNativeFromClr(
          acf_string_t& acfStr,
          String ^ str) {
    acf_string_clear(&acfStr);

    if (str != nullptr) {
      pin_ptr<const wchar_t> pStr = PtrToStringChars(str);
      AcfStringUTF16 temp_utf16(reinterpret_cast<const char16_t*>(pStr));
      auto temp_utf8 = temp_utf16.ToString();
      acf_string_copy(temp_utf8.c_str(), temp_utf8.size(), &acfStr);
    }
  }
};

}  // namespace Internals
}  // namespace ACFSharpWrapper
