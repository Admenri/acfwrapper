
#include "wrapper_main.h"
#include "utils.h"

namespace ACFSharpWrapper {

String ^ ACFMainEntry::GetWrapperVersion() {
  return Internals::StringUtils::ToClr("Ver 2025.2.26-��-M131");
}

}  // namespace ACFSharpWrapper
