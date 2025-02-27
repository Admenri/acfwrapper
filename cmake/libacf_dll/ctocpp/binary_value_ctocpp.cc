// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=8f025f4f36ceed39139ba49c561dc7c17a2c6f32$
//

#include "libacf_dll/ctocpp/binary_value_ctocpp.h"

// STATIC METHODS - Body may be edited by hand.

AcfRefPtr<AcfBinaryValue> AcfBinaryValue::Create(const void* data,
                                                 size_t data_size) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: data; type: simple_byaddr
  if (!data) {
    return nullptr;
  }

  // Execute
  acf_binary_value_t* _retval = acf_binary_value_create(data, data_size);

  // Return type: refptr_same
  return AcfBinaryValueCToCpp::Wrap(_retval);
}

// VIRTUAL METHODS - Body may be edited by hand.

bool AcfBinaryValueCToCpp::IsValid() {
  acf_binary_value_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, is_valid)) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_valid(_struct);

  // Return type: bool
  return _retval ? true : false;
}

bool AcfBinaryValueCToCpp::IsOwned() {
  acf_binary_value_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, is_owned)) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_owned(_struct);

  // Return type: bool
  return _retval ? true : false;
}

bool AcfBinaryValueCToCpp::IsSame(AcfRefPtr<AcfBinaryValue> that) {
  acf_binary_value_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, is_same)) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: that; type: refptr_same
  if (!that.get()) {
    return false;
  }

  // Execute
  int _retval = _struct->is_same(_struct, AcfBinaryValueCToCpp::Unwrap(that));

  // Return type: bool
  return _retval ? true : false;
}

bool AcfBinaryValueCToCpp::IsEqual(AcfRefPtr<AcfBinaryValue> that) {
  acf_binary_value_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, is_equal)) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: that; type: refptr_same
  if (!that.get()) {
    return false;
  }

  // Execute
  int _retval = _struct->is_equal(_struct, AcfBinaryValueCToCpp::Unwrap(that));

  // Return type: bool
  return _retval ? true : false;
}

AcfRefPtr<AcfBinaryValue> AcfBinaryValueCToCpp::Copy() {
  acf_binary_value_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, copy)) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  acf_binary_value_t* _retval = _struct->copy(_struct);

  // Return type: refptr_same
  return AcfBinaryValueCToCpp::Wrap(_retval);
}

size_t AcfBinaryValueCToCpp::GetSize() {
  acf_binary_value_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, get_size)) {
    return 0;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  size_t _retval = _struct->get_size(_struct);

  // Return type: simple
  return _retval;
}

size_t AcfBinaryValueCToCpp::GetData(void* buffer,
                                     size_t buffer_size,
                                     size_t data_offset) {
  acf_binary_value_t* _struct = GetStruct();
  if (ACF_MEMBER_MISSING(_struct, get_data)) {
    return 0;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: buffer; type: simple_byaddr
  if (!buffer) {
    return 0;
  }

  // Execute
  size_t _retval = _struct->get_data(_struct, buffer, buffer_size, data_offset);

  // Return type: simple
  return _retval;
}

// CONSTRUCTOR - Do not edit by hand.

AcfBinaryValueCToCpp::AcfBinaryValueCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

AcfBinaryValueCToCpp::~AcfBinaryValueCToCpp() {}

template <>
acf_binary_value_t*
AcfCToCppRefCounted<AcfBinaryValueCToCpp, AcfBinaryValue, acf_binary_value_t>::
    UnwrapDerived(AcfWrapperType type, AcfBinaryValue* c) {
  return nullptr;
}

template <>
AcfWrapperType AcfCToCppRefCounted<AcfBinaryValueCToCpp,
                                   AcfBinaryValue,
                                   acf_binary_value_t>::kWrapperType =
    WT_BINARY_VALUE;
