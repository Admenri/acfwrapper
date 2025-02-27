// Copyright (c) 2023 Admenri. All rights reserved.

#ifndef ACF_CPPTOC_SCOPED_H_
#define ACF_CPPTOC_SCOPED_H_
#pragma once

#include "include/capi/acf_base_capi.h"
#include "include/internal/acf_scoped_refptr.h"
#include "libacf_dll/wrapper_types.h"

// Wrap a C++ class with a C structure. This is used when the class
// implementation exists on this side of the DLL boundary but will have methods
// called from the other side of the DLL boundary.
template <class ClassName, class BaseName, class StructName>
class AcfCppToCScoped : public AcfBaseScoped {
 public:
  AcfCppToCScoped(const AcfCppToCScoped&) = delete;
  AcfCppToCScoped& operator=(const AcfCppToCScoped&) = delete;

  // Create a new wrapper instance and associated structure reference for
  // passing an object instance the other side. The wrapper object will be
  // deleted when |del| is called on the associated structure. The wrapped
  // object will be deleted when the wrapper object is deleted. For example:
  //
  // void MyMethod(AcfOwnPtr<MyType> obj) {
  //   my_method(MyTypeCppToC::WrapOwn(obj));
  // }
  //
  // void my_method(my_type_t* struct) {
  //   // Delete the MyTypeCppToC wrapper and the owned MyType object.
  //   struct->del(struct);
  // }
  static StructName* WrapOwn(AcfOwnPtr<BaseName> c) {
    if (!c)
      return nullptr;

    // Wrap our object with the AcfCppToC class.
    ClassName* wrapper = new ClassName();
    wrapper->Initialize(c.release(), true);

    // Return the structure pointer that can now be passed to the other side.
    return wrapper->GetStruct();
  }

  // Create a new wrapper instance and associated structure reference for
  // passing an object instance to the other side. The wrapper object is owned
  // by the caller. The wrapped object is unowned and must outlive the wrapper.
  // For example:
  //
  // void MyMethod(MyType* obj) {
  //   AcfOwnPtr<MyTypeCppToC> MyTypeWrapper = MyTypeCppToC::WrapRaw(obj);
  //   my_method(MyTypeWrapper->GetStruct());
  //   // MyTypeWrapper is deleted when MyMethod() goes out of scope.
  // }
  //
  // void my_method(my_type_t* struct) {
  //   // Access |struct| here but you can't delete it.
  // }
  static AcfOwnPtr<ClassName> WrapRaw(AcfRawPtr<BaseName> c) {
    if (!c)
      return AcfOwnPtr<ClassName>();

    // Wrap our object with the AcfCppToC class.
    ClassName* wrapper = new ClassName();
    wrapper->Initialize(c, false);

    // Return the owned wrapper object.
    return AcfOwnPtr<ClassName>(wrapper);
  }

  // Retrieve the underlying object instance for a structure reference passed
  // back from the other side. The caller takes ownership of the object. For
  // example:
  //
  // void my_method(my_type_t* struct) {
  //   AcfOwnPtr<MyType> MyTypePtr = MyTypeCppToC::UnwrapOwn(struct);
  //   // |struct| has been deleted and should no longer be accessed.
  // }
  static AcfOwnPtr<BaseName> UnwrapOwn(StructName* s) {
    if (!s)
      return AcfOwnPtr<BaseName>();

    // Cast our structure to the wrapper structure type.
    WrapperStruct* wrapperStruct = GetWrapperStruct(s);

    // If the type does not match this object then we need to unwrap as the
    // derived type.
    if (wrapperStruct->type_ != kWrapperType)
      return UnwrapDerivedOwn(wrapperStruct->type_, s);

    // We're giving up ownership of the underlying object. Clear the pointer so
    // it doesn't get deleted.
    BaseName* object = wrapperStruct->object_;
    wrapperStruct->object_ = nullptr;

    delete wrapperStruct->wrapper_;

    // Return the underlying object instance.
    return AcfOwnPtr<BaseName>(object);
  }

  // Retrieve the underlying object instance for a structure reference passed
  // back from the other side. Ownership does not change. For example:
  //
  // void my_method(my_type_t* struct) {
  //   AcfRawPtr<MyType> MyTypePtr = MyTypeCppToC::UnwrapRaw(struct);
  //   // |struct| is still valid.
  // }
  static AcfRawPtr<BaseName> UnwrapRaw(StructName* s) {
    if (!s)
      return nullptr;

    // Cast our structure to the wrapper structure type.
    WrapperStruct* wrapperStruct = GetWrapperStruct(s);

    // If the type does not match this object then we need to unwrap as the
    // derived type.
    if (wrapperStruct->type_ != kWrapperType)
      return UnwrapDerivedRaw(wrapperStruct->type_, s);

    // Return the underlying object instance.
    return wrapperStruct->object_;
  }

  // Retrieve the same side wrapper associated with the structure. Ownership
  // does not change.
  static ClassName* GetWrapper(StructName* s) {
    WrapperStruct* wrapperStruct = GetWrapperStruct(s);

    return static_cast<ClassName*>(wrapperStruct->wrapper_);
  }

  // Retrieve the underlying object instance from our own structure reference
  // when the reference is passed as the required first parameter of a C API
  // function call. Ownership of the object does not change.
  static BaseName* Get(StructName* s) {
    WrapperStruct* wrapperStruct = GetWrapperStruct(s);

    return wrapperStruct->object_;
  }

  // If returning the structure across the DLL boundary you should call
  // AddRef() on this AcfCppToC object.  On the other side of the DLL boundary,
  // call UnderlyingRelease() on the wrapping AcfCToCpp object.
  StructName* GetStruct() { return &wrapper_struct_.struct_; }

 protected:
  AcfCppToCScoped() {
    wrapper_struct_.type_ = kWrapperType;
    wrapper_struct_.wrapper_ = this;
    memset(GetStruct(), 0, sizeof(StructName));
  }

  virtual ~AcfCppToCScoped() {
    // Only delete the underlying object if we own it.
    if (owned_ && wrapper_struct_.object_)
      delete wrapper_struct_.object_;
  }

 private:
  // Used to associate this wrapper object, the underlying object instance and
  // the structure that will be passed to the other side.
  struct WrapperStruct {
    AcfWrapperType type_;
    BaseName* object_;
    AcfCppToCScoped<ClassName, BaseName, StructName>* wrapper_;
    StructName struct_;
  };

  void Initialize(BaseName* obj, bool owned) {
    wrapper_struct_.object_ = obj;
    owned_ = owned;

    acf_base_scoped_t* base = reinterpret_cast<acf_base_scoped_t*>(GetStruct());
    base->size = sizeof(StructName);
    if (owned)
      base->del = struct_del;
  }

  static WrapperStruct* GetWrapperStruct(StructName* s) {
    // Offset using the WrapperStruct size instead of individual member sizes
    // to avoid problems due to platform/compiler differences in structure
    // padding.
    return reinterpret_cast<WrapperStruct*>(
        reinterpret_cast<char*>(s) -
        (sizeof(WrapperStruct) - sizeof(StructName)));
  }

  // Unwrap as the derived type.
  static AcfOwnPtr<BaseName> UnwrapDerivedOwn(AcfWrapperType type,
                                              StructName* s);
  static AcfRawPtr<BaseName> UnwrapDerivedRaw(AcfWrapperType type,
                                              StructName* s);

  static void ACF_CALLBACK struct_del(acf_base_scoped_t* base) {
    if (!base)
      return;

    WrapperStruct* wrapperStruct =
        GetWrapperStruct(reinterpret_cast<StructName*>(base)); 
    
    delete wrapperStruct->wrapper_;
  }

  WrapperStruct wrapper_struct_;
  bool owned_;

  static AcfWrapperType kWrapperType;
};

#endif  // acf_LIBacf_DLL_CPPTOC_CPPTOC_SCOPED_H_
