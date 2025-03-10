// Copyright (c) 2025 Admenri. All rights reserved.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the ACF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=b09eb1213115e0413162b61e1732e690c269b127$
//

#include "include/acf_environment.h"
#include "include/acf_parser.h"
#include "include/capi/acf_environment_capi.h"
#include "include/capi/acf_parser_capi.h"
#include "libacf_dll/ctocpp/binary_value_ctocpp.h"
#include "libacf_dll/ctocpp/value_ctocpp.h"

// Define used to facilitate parsing.
#define ACF_GLOBAL

// GLOBAL METHODS - Body may be edited by hand.

ACF_GLOBAL bool AcfGetExVersionActiveState() {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = acf_get_ex_version_active_state();

  // Return type: bool
  return _retval ? true : false;
}

ACF_GLOBAL AcfString AcfBase64Encode(const void* data, size_t data_size) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: data; type: simple_byaddr
  if (!data) {
    return AcfString();
  }

  // Execute
  acf_string_userfree_t _retval = acf_base64encode(data, data_size);

  // Return type: string
  AcfString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

ACF_GLOBAL AcfRefPtr<AcfBinaryValue> AcfBase64Decode(const AcfString& data) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: data; type: string_byref_const
  if (data.empty()) {
    return nullptr;
  }

  // Execute
  acf_binary_value_t* _retval = acf_base64decode(data.GetStruct());

  // Return type: refptr_same
  return AcfBinaryValueCToCpp::Wrap(_retval);
}

ACF_GLOBAL AcfString AcfURIEncode(const AcfString& text, bool use_plus) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: text; type: string_byref_const
  if (text.empty()) {
    return AcfString();
  }

  // Execute
  acf_string_userfree_t _retval = acf_uriencode(text.GetStruct(), use_plus);

  // Return type: string
  AcfString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

ACF_GLOBAL AcfString AcfURIDecode(const AcfString& text,
                                  bool convert_to_utf8,
                                  acf_uri_unescape_rule_t unescape_rule) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: text; type: string_byref_const
  if (text.empty()) {
    return AcfString();
  }

  // Execute
  acf_string_userfree_t _retval =
      acf_uridecode(text.GetStruct(), convert_to_utf8, unescape_rule);

  // Return type: string
  AcfString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

ACF_GLOBAL AcfRefPtr<AcfValue> AcfParseJSON(const AcfString& json_string,
                                            acf_json_parser_options_t options) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: json_string; type: string_byref_const
  if (json_string.empty()) {
    return nullptr;
  }

  // Execute
  acf_value_t* _retval = acf_parse_json(json_string.GetStruct(), options);

  // Return type: refptr_same
  return AcfValueCToCpp::Wrap(_retval);
}

ACF_GLOBAL AcfRefPtr<AcfValue> AcfParseJSON(const void* json,
                                            size_t json_size,
                                            acf_json_parser_options_t options) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: json; type: simple_byaddr
  if (!json) {
    return nullptr;
  }

  // Execute
  acf_value_t* _retval = acf_parse_json_buffer(json, json_size, options);

  // Return type: refptr_same
  return AcfValueCToCpp::Wrap(_retval);
}

ACF_GLOBAL AcfRefPtr<AcfValue> AcfParseJSONAndReturnError(
    const AcfString& json_string,
    acf_json_parser_options_t options,
    AcfString& error_msg_out) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: json_string; type: string_byref_const
  if (json_string.empty()) {
    return nullptr;
  }

  // Execute
  acf_value_t* _retval = acf_parse_jsonand_return_error(
      json_string.GetStruct(), options, error_msg_out.GetWritableStruct());

  // Return type: refptr_same
  return AcfValueCToCpp::Wrap(_retval);
}

ACF_GLOBAL AcfString AcfWriteJSON(AcfRefPtr<AcfValue> node,
                                  acf_json_writer_options_t options) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: node; type: refptr_same
  if (!node.get()) {
    return AcfString();
  }

  // Execute
  acf_string_userfree_t _retval =
      acf_write_json(AcfValueCToCpp::Unwrap(node), options);

  // Return type: string
  AcfString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}
