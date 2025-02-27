// Copyright (c) 2023 Admenri. All rights reserved.

#ifndef ACF_CTOCPP_SCOPED_H_
#define ACF_CTOCPP_SCOPED_H_
#pragma once

#include "include/capi/acf_base_capi.h"
#include "include/internal/acf_scoped_refptr.h"
#include "libacf_dll/wrapper_types.h"

// Wrap a C structure with a C++ class. This is used when the implementation
// exists on the other side of the DLL boundary but will have methods called on
// this side of the DLL boundary.
template <class ClassName, class BaseName, class StructName>
class AcfCToCppScoped : public BaseName {
 public:
  AcfCToCppScoped(const AcfCToCppScoped&) = delete;
  AcfCToCppScoped& operator=(const AcfCToCppScoped&) = delete;

  // Create a new wrapper instance for a structure reference received from the
  // other side. The caller owns the CToCpp wrapper instance but not necessarily
  // the underling object on the CppToC side (depends if s->del is non-NULL).
  // The returned wrapper object can be used as either a scoped argument or to
  // pass ownership. For example:
  //
  // void my_method(my_type1_t* struct1, my_type2_t* struct2) {
  //  // Passes ownership to MyMethod1().
  //  MyMethod1(MyType1CToCpp::Wrap(struct1));
  //
  //  // Passes reference to MyMethod2().
  //  AcfOwnPtr<MyType1> obj2 = MyType2CToCpp::Wrap(struct2);
  //  MyMethod2(obj2.get());
  //  // |obj2| is deleted when my_method() goes out of scope.
  // }
  //
  // void MyMethod1(AcfOwnPtr<MyType1> obj1) {
  //   // |obj1| is deleted when MyMethod1() goes out of scope.
  // }
  //
  // void MyMethod2(AcfRawPtr<MyType2> obj2) {
  // }
  static AcfOwnPtr<BaseName> Wrap(StructName* s);

  // Retrieve the underlying structure reference from a wrapper instance for
  // return back to the other side. Ownership will be passed back to the other
  // side and the wrapper will be deleted. For example:
  //
  // void MyMethod(AcfOwnPtr<MyType> obj) {
  //   // Ownership of the underlying MyType object is passed to my_method().
  //   my_method(MyTypeCToCpp::UnwrapOwn(std::move(obj)));
  //   // |obj| is now NULL.
  // }
  static StructName* UnwrapOwn(AcfOwnPtr<BaseName> c);

  // Retrieve the underlying structure reference from a wrapper instance for
  // return back to the other side. Ownership does not change. For example:
  //
  // void MyMethod(AcfRawPtr<MyType> obj) {
  //   // A reference is passed to my_method(). Ownership does not change.
  //   my_method2(MyTypeCToCpp::UnwrapRaw(obj));
  // }
  static StructName* UnwrapRaw(AcfRawPtr<BaseName> c);

  // Override delete operator to properly delete the WrapperStruct.
  // ~AcfCToCppScoped will be called first followed by this method.
  static void operator delete(void* ptr);

 protected:
  AcfCToCppScoped() = default;
  virtual ~AcfCToCppScoped() = default;

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
  static StructName* UnwrapDerivedOwn(AcfWrapperType type,
                                      AcfOwnPtr<BaseName> c);
  static StructName* UnwrapDerivedRaw(AcfWrapperType type,
                                      AcfRawPtr<BaseName> c);

  static AcfWrapperType kWrapperType;
};

template <class ClassName, class BaseName, class StructName>
struct AcfCToCppScoped<ClassName, BaseName, StructName>::WrapperStruct {
  AcfWrapperType type_;
  StructName* struct_;
  ClassName wrapper_;
};

template <class ClassName, class BaseName, class StructName>
AcfOwnPtr<BaseName> AcfCToCppScoped<ClassName, BaseName, StructName>::Wrap(
    StructName* s) {
  if (!s)
    return AcfOwnPtr<BaseName>();

  // Wrap their structure with the AcfCToCpp object.
  WrapperStruct* wrapperStruct = new WrapperStruct;
  wrapperStruct->type_ = kWrapperType;
  wrapperStruct->struct_ = s;

  return AcfOwnPtr<BaseName>(&wrapperStruct->wrapper_);
}

template <class ClassName, class BaseName, class StructName>
StructName* AcfCToCppScoped<ClassName, BaseName, StructName>::UnwrapOwn(
    AcfOwnPtr<BaseName> c) {
  if (!c.get())
    return nullptr;

  WrapperStruct* wrapperStruct = GetWrapperStruct(c.get());

  // If the type does not match this object then we need to unwrap as the
  // derived type.
  if (wrapperStruct->type_ != kWrapperType)
    return UnwrapDerivedOwn(wrapperStruct->type_, std::move(c));

  StructName* orig_struct = wrapperStruct->struct_;

  // Don't delete the original object when the wrapper is deleted.
  wrapperStruct->struct_ = nullptr;

  // Return the original structure.
  return orig_struct;
  // The wrapper |c| is deleted when this method goes out of scope.
}

template <class ClassName, class BaseName, class StructName>
StructName* AcfCToCppScoped<ClassName, BaseName, StructName>::UnwrapRaw(
    AcfRawPtr<BaseName> c) {
  if (!c)
    return nullptr;

  WrapperStruct* wrapperStruct = GetWrapperStruct(c);

  // If the type does not match this object then we need to unwrap as the
  // derived type.
  if (wrapperStruct->type_ != kWrapperType)
    return UnwrapDerivedRaw(wrapperStruct->type_, c);

  // Return the original structure.
  return wrapperStruct->struct_;
}

template <class ClassName, class BaseName, class StructName>
void AcfCToCppScoped<ClassName, BaseName, StructName>::operator delete(
    void* ptr) {
  WrapperStruct* wrapperStruct = GetWrapperStruct(static_cast<BaseName*>(ptr));

  // May be NULL if UnwrapOwn() was called.
  acf_base_scoped_t* base =
      reinterpret_cast<acf_base_scoped_t*>(wrapperStruct->struct_);

  // If we own the object (base->del != NULL) then notify the other side that
  // the object has been deleted.
  if (base && base->del)
    base->del(base);

  // Delete the wrapper structure without executing ~AcfCToCppScoped() an
  // additional time.
  ::operator delete(wrapperStruct);
}

template <class ClassName, class BaseName, class StructName>
typename AcfCToCppScoped<ClassName, BaseName, StructName>::WrapperStruct*
AcfCToCppScoped<ClassName, BaseName, StructName>::GetWrapperStruct(
    const BaseName* obj) {
  // Offset using the WrapperStruct size instead of individual member sizes to
  // avoid problems due to platform/compiler differences in structure padding.
  return reinterpret_cast<WrapperStruct*>(
      reinterpret_cast<char*>(const_cast<BaseName*>(obj)) -
      (sizeof(WrapperStruct) - sizeof(ClassName)));
}

#endif  // acf_LIBacf_DLL_CTOCPP_CTOCPP_SCOPED_H_
