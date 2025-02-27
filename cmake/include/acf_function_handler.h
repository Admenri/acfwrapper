// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_FUNCTION_HANDLER_H_
#define ACF_INCLUDE_ACF_FUNCTION_HANDLER_H_

#include "include/acf_browser.h"
#include "include/acf_frame.h"
#include "include/acf_values.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_types.h"

class AcfValue;
class AcfFrame;

typedef std::vector<AcfRefPtr<AcfValue>> ArgumentsList;

///
/// Promise return resolver
///
/*--acf(source=library)--*/
class AcfPromiseResolver : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Reply message to promise.
  ///
  /*--acf(optional_param=return_value)--*/
  virtual void Resolve(AcfRefPtr<AcfValue> return_value) = 0;
};

///
/// Function register factory handler
///
/*--acf(source=client)--*/
class AcfFunctionHandler : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Function execution register
  ///
  /*--acf(optional_param=arguments)--*/
  virtual void OnExecuteFunction(const ArgumentsList& arguments,
                                 AcfRefPtr<AcfPromiseResolver> promise) {}
};

///
/// Function register factory
///
/*--acf(source=client)--*/
class AcfFunctionFactory : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Create web message host handler for user-side
  ///
  /*--acf(optional_param=origin,optional_param=frame)--*/
  virtual AcfRefPtr<AcfFunctionHandler> CreateHost(
      AcfRefPtr<AcfBrowser> browser,
      const AcfString& origin,
      AcfRefPtr<AcfFrame> frame) = 0;
};

#endif  // ACF_INCLUDE_ACF_FUNCTION_HANDLER_H_
