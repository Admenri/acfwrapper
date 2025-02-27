// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_FRAME_H_
#define ACF_INCLUDE_ACF_FRAME_H_

#include "include/acf_browser.h"
#include "include/acf_callback.h"
#include "include/acf_profile.h"
#include "include/acf_values.h"

#include "include/internal/acf_enums.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_list.h"
#include "include/internal/acf_types.h"
#include "include/internal/acf_types_wrappers.h"

class AcfBrowser;
class AcfValue;
class AcfStringVisitor;
class AcfDOMDocument;
class AcfDOMNode;

///
/// Execute result handler
///
/*--acf(source=client)--*/
class AcfExecuteJavascriptCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Get execute return value
  ///
  /*--acf(optional_param=value)--*/
  virtual void OnExecuteResult(AcfRefPtr<AcfValue> value) = 0;
};

///
/// DOM result handler
///
/*--acf(source=client)--*/
class AcfDOMDocumentCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// DOM snapshot
  ///
  /*--acf(optional_param=value)--*/
  virtual void OnDOMResult(AcfRefPtr<AcfDOMDocument> value) = 0;
};

///
/// Node result handler
///
/*--acf(source=client)--*/
class AcfDOMNodeCallback : public virtual AcfBaseRefCounted {
 public:
  ///
  /// DOM query result, return false to stop.
  ///
  /*--acf(optional_param=value)--*/
  virtual bool OnQueryResult(AcfRefPtr<AcfDOMNode> value,
                             int count,
                             int total) = 0;
};

///
/// ACF's frame object host.
///
/*--acf(source=library)--*/
class AcfFrame : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Get frame object valid state.
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Compare object with other object.
  ///
  /*--acf()--*/
  virtual bool IsSame(AcfRefPtr<AcfFrame> object) = 0;

  ///
  /// Returns the parent of this frame or NULL if this is the main (top-level)
  /// frame.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfFrame> GetParent() = 0;

  ///
  /// Returns the browser that this frame belongs to.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfBrowser> GetBrowser() = 0;

  ///
  /// Returns the URL currently loaded in this frame.
  ///
  /*--acf()--*/
  virtual AcfString GetURL() = 0;

  ///
  /// Returns the name for this frame. If the frame has an assigned name (for
  /// example, set via the iframe "name" attribute) then that value will be
  /// returned. Otherwise a unique name will be constructed based on the frame
  /// parent hierarchy. The main (top-level) frame will always have an empty
  /// name value.
  ///
  /*--acf()--*/
  virtual AcfString GetName() = 0;

  ///
  /// Returns the globally unique identifier for this frame or < 0 if the
  /// underlying frame does not yet exist.
  ///
  /*--acf()--*/
  virtual int64 GetIdentifier() = 0;

  ///
  /// Is frame main context in browser.
  ///
  /*--acf()--*/
  virtual bool IsMain() = 0;

  ///
  /// Execute javascripts in current frame context.
  /// value is invalid when return
  ///
  /*--acf(optional_param=script,optional_param=url,optional_param=handler)--*/
  virtual void ExecuteJavascript(
      const AcfString& script,
      const AcfString& url,
      AcfRefPtr<AcfExecuteJavascriptCallback> handler) = 0;

  ///
  /// Retrieve this frame's HTML source as a string sent to the specified
  /// visitor.
  ///
  /*--acf()--*/
  virtual void GetSource(AcfRefPtr<AcfStringVisitor> visitor) = 0;

  ///
  /// Get the snapshot of the current DOM.
  ///
  /*--acf()--*/
  virtual void GetDOMDocument(AcfRefPtr<AcfDOMDocumentCallback> visitor) = 0;

  ///
  /// Get selector snapshot of the current DOM.
  ///
  /*--acf()--*/
  virtual void QuerySelectorAll(const AcfString& selector,
                                AcfRefPtr<AcfDOMNodeCallback> visitor) = 0;
};

#endif  //! ACF_INCLUDE_ACF_FRAME_H_
