// Copyright (c) 2023 Admenri. All rights reserved.

#ifndef ACF_CTOCPP_REF_COUNTED_H_
#define ACF_CTOCPP_REF_COUNTED_H_
#pragma once

#include "include/capi/acf_base_capi.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string_list.h"
#include "include/internal/acf_string_map.h"
#include "include/internal/acf_string_multimap.h"
#include "libacf_dll/transfer_util.h"
#include "libacf_dll/wrapper_types.h"

#ifndef DCHECK
#define DCHECK(x)
#endif

// Wrap a C structure with a C++ class. This is used when the implementation
// exists on the other side of the DLL boundary but will have methods called on
// this side of the DLL boundary.
template <class ClassName, class BaseName, class StructName>
class AcfCToCppRefCounted : public BaseName {
 public:
  AcfCToCppRefCounted(const AcfCToCppRefCounted&) = delete;
  AcfCToCppRefCounted& operator=(const AcfCToCppRefCounted&) = delete;

  // Create a new wrapper instance for a structure reference received from the
  // other side.
  static AcfRefPtr<BaseName> Wrap(StructName* s);

  // Retrieve the underlying structure reference from a wrapper instance for
  // return back to the other side.
  static StructName* Unwrap(AcfRefPtr<BaseName> c);

  // AcfBaseRefCounted methods increment/decrement reference counts on both this
  // object and the underlying wrapped structure.
  void AddRef() const {
    UnderlyingAddRef();
    ref_count_.AddRef();
  }
  bool Release() const;
  bool HasOneRef() const { return UnderlyingHasOneRef(); }
  bool HasAtLeastOneRef() const { return UnderlyingHasAtLeastOneRef(); }

 protected:
  AcfCToCppRefCounted() = default;
  virtual ~AcfCToCppRefCounted() = default;

  // If returning the structure across the DLL boundary use Unwrap() instead.
  StructName* GetStruct() const {
    WrapperStruct* wrapperStruct = GetWrapperStruct(this);

    return wrapperStruct->struct_;
  }

 private:
  // Used to associate this wrapper object and the structure reference received
  // from the other side.
  struct WrapperStruct;

  static WrapperStruct* GetWrapperStruct(const BaseName* obj);

  // Unwrap as the derived type.
  static StructName* UnwrapDerived(AcfWrapperType type, BaseName* c);

  // Increment/decrement reference counts on only the underlying class.
  void UnderlyingAddRef() const {
    acf_base_ref_counted_t* base =
        reinterpret_cast<acf_base_ref_counted_t*>(GetStruct());
    if (base->add_ref)
      base->add_ref(base);
  }

  bool UnderlyingRelease() const {
    acf_base_ref_counted_t* base =
        reinterpret_cast<acf_base_ref_counted_t*>(GetStruct());
    if (!base->release)
      return false;
    return base->release(base) ? true : false;
  }

  bool UnderlyingHasOneRef() const {
    acf_base_ref_counted_t* base =
        reinterpret_cast<acf_base_ref_counted_t*>(GetStruct());
    if (!base->has_one_ref)
      return false;
    return base->has_one_ref(base) ? true : false;
  }

  bool UnderlyingHasAtLeastOneRef() const {
    acf_base_ref_counted_t* base =
        reinterpret_cast<acf_base_ref_counted_t*>(GetStruct());
    if (!base->has_at_least_one_ref)
      return false;
    return base->has_at_least_one_ref(base) ? true : false;
  }

  AcfRefCount ref_count_;

  static AcfWrapperType kWrapperType;
};

template <class ClassName, class BaseName, class StructName>
struct AcfCToCppRefCounted<ClassName, BaseName, StructName>::WrapperStruct {
  AcfWrapperType type_;
  StructName* struct_;
  ClassName wrapper_;
};

template <class ClassName, class BaseName, class StructName>
AcfRefPtr<BaseName> AcfCToCppRefCounted<ClassName, BaseName, StructName>::Wrap(
    StructName* s) {
  if (!s)
    return nullptr;

  // Wrap their structure with the AcfCToCppRefCounted object.
  WrapperStruct* wrapperStruct = new WrapperStruct;
  wrapperStruct->type_ = kWrapperType;
  wrapperStruct->struct_ = s;

  // Put the wrapper object in a smart pointer.
  AcfRefPtr<BaseName> wrapperPtr(&wrapperStruct->wrapper_);
  // Release the reference that was added to the AcfCppToC wrapper object on
  // the other side before their structure was passed to us.
  wrapperStruct->wrapper_.UnderlyingRelease();
  // Return the smart pointer.
  return wrapperPtr;
}

template <class ClassName, class BaseName, class StructName>
StructName* AcfCToCppRefCounted<ClassName, BaseName, StructName>::Unwrap(
    AcfRefPtr<BaseName> c) {
  if (!c.get())
    return nullptr;

  WrapperStruct* wrapperStruct = GetWrapperStruct(c.get());

  // If the type does not match this object then we need to unwrap as the
  // derived type.
  if (wrapperStruct->type_ != kWrapperType)
    return UnwrapDerived(wrapperStruct->type_, c.get());

  // Add a reference to the AcfCppToC wrapper object on the other side that
  // will be released once the structure is received.
  wrapperStruct->wrapper_.UnderlyingAddRef();
  // Return their original structure.
  return wrapperStruct->struct_;
}

template <class ClassName, class BaseName, class StructName>
bool AcfCToCppRefCounted<ClassName, BaseName, StructName>::Release() const {
  UnderlyingRelease();
  if (ref_count_.Release()) {
    WrapperStruct* wrapperStruct = GetWrapperStruct(this);

    delete wrapperStruct;
    return true;
  }
  return false;
}

template <class ClassName, class BaseName, class StructName>
typename AcfCToCppRefCounted<ClassName, BaseName, StructName>::WrapperStruct*
AcfCToCppRefCounted<ClassName, BaseName, StructName>::GetWrapperStruct(
    const BaseName* obj) {
  // Offset using the WrapperStruct size instead of individual member sizes to
  // avoid problems due to platform/compiler differences in structure padding.
  return reinterpret_cast<WrapperStruct*>(
      reinterpret_cast<char*>(const_cast<BaseName*>(obj)) -
      (sizeof(WrapperStruct) - sizeof(ClassName)));
}

#endif  // ACF_CTOCPP_REF_COUNTED_H_
