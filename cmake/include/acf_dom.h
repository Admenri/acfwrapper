// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_DOM_H_
#define ACF_INCLUDE_ACF_DOM_H_

#include <map>

#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_map.h"
#include "include/internal/acf_types.h"
#include "include/internal/acf_types_wrappers.h"

class AcfDOMDocument;
class AcfDOMNode;

///
/// Class used to represent a DOM document. The methods of this class should
/// only be called on the render process main thread thread.
///
/*--acf(source=library)--*/
class AcfDOMDocument : public virtual AcfBaseRefCounted {
 public:
  typedef acf_dom_document_type_t Type;

  ///
  /// Returns the document type.
  ///
  /*--acf(default_retval=DOM_DOCUMENT_TYPE_UNKNOWN)--*/
  virtual Type GetType() = 0;

  ///
  /// Returns the root document node.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfDOMNode> GetDocument() = 0;

  ///
  /// Returns the title of an HTML document.
  ///
  /*--acf()--*/
  virtual AcfString GetTitle() = 0;

  ///
  /// Returns the base URL for the document.
  ///
  /*--acf()--*/
  virtual AcfString GetBaseURL() = 0;
};

///
/// Class used to represent a DOM node. The methods of this class should only be
/// called on the render process main thread.
///
/*--acf(source=library)--*/
class AcfDOMNode : public virtual AcfBaseRefCounted {
 public:
  typedef std::map<AcfString, AcfString> AttributeMap;
  typedef acf_dom_node_type_t Type;

  ///
  /// Returns the type for this node.
  ///
  /*--acf(default_retval=DOM_NODE_TYPE_UNSUPPORTED)--*/
  virtual Type GetType() = 0;

  ///
  /// Returns the name of this node.
  ///
  /*--acf()--*/
  virtual AcfString GetName() = 0;

  ///
  /// Returns the value of this node.
  ///
  /*--acf()--*/
  virtual AcfString GetValue() = 0;

  ///
  /// Returns the contents of this node as markup.
  ///
  /*--acf()--*/
  virtual AcfString GetAsMarkup() = 0;

  ///
  /// Return the child node count.
  ///
  /*--acf()--*/
  virtual int GetChildNodeCount() = 0;

  ///
  /// Get child node.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfDOMNode> GetChildNode(int index) = 0;

  // The following methods are valid only for element nodes.

  ///
  /// Returns the tag name of this element.
  ///
  /*--acf()--*/
  virtual AcfString GetElementTagName() = 0;

  ///
  /// Returns true if this element has attributes.
  ///
  /*--acf()--*/
  virtual bool HasElementAttributes() = 0;

  ///
  /// Returns true if this element has an attribute named |attrName|.
  ///
  /*--acf()--*/
  virtual bool HasElementAttribute(const AcfString& attrName) = 0;

  ///
  /// Returns the element attribute named |attrName|.
  ///
  /*--acf()--*/
  virtual AcfString GetElementAttribute(const AcfString& attrName) = 0;

  ///
  /// Returns a map of all element attributes.
  ///
  /*--acf()--*/
  virtual void GetElementAttributes(AttributeMap& attrMap) = 0;

  ///
  /// Returns the inner text of the element.
  ///
  /*--acf()--*/
  virtual AcfString GetElementInnerText() = 0;

  ///
  /// Returns the bounds of the element in device pixels. Use
  /// "window.devicePixelRatio" to convert to/from CSS pixels.
  ///
  /*--acf()--*/
  virtual AcfRect GetElementBounds() = 0;
};

#endif  // ACF_INCLUDE_ACF_DOM_H_
