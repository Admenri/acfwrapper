// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_PARSER_H_
#define ACF_INCLUDE_ACF_PARSER_H_

#include <vector>

#include "include/acf_values.h"

#include "include/internal/acf_enums.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_list.h"
#include "include/internal/acf_types.h"
#include "include/internal/acf_types_wrappers.h"

///
/// Encodes |data| as a base64 string.
///
/*--acf()--*/
AcfString AcfBase64Encode(const void* data, size_t data_size);

///
/// Decodes the base64 encoded string |data|. The returned value will be NULL if
/// the decoding fails.
///
/*--acf()--*/
AcfRefPtr<AcfBinaryValue> AcfBase64Decode(const AcfString& data);

///
/// Escapes characters in |text| which are unsuitable for use as a query
/// parameter value. Everything except alphanumerics and -_.!~*'() will be
/// converted to "%XX". If |use_plus| is true spaces will change to "+". The
/// result is basically the same as encodeURIComponent in Javacript.
///
/*--acf()--*/
AcfString AcfURIEncode(const AcfString& text, bool use_plus);

///
/// Unescapes |text| and returns the result. Unescaping consists of looking for
/// the exact pattern "%XX" where each X is a hex digit and converting to the
/// character with the numerical value of those digits (e.g. "i%20=%203%3b"
/// unescapes to "i = 3;"). If |convert_to_utf8| is true this function will
/// attempt to interpret the initial decoded result as UTF-8. If the result is
/// convertable into UTF-8 it will be returned as converted. Otherwise the
/// initial decoded result will be returned.  The |unescape_rule| parameter
/// supports further customization the decoding process.
///
/*--acf()--*/
AcfString AcfURIDecode(const AcfString& text,
                       bool convert_to_utf8,
                       acf_uri_unescape_rule_t unescape_rule);

///
/// Parses the specified |json_string| and returns a dictionary or list
/// representation. If JSON parsing fails this method returns NULL.
///
/*--acf()--*/
AcfRefPtr<AcfValue> AcfParseJSON(const AcfString& json_string,
                                 acf_json_parser_options_t options);

///
/// Parses the specified UTF8-encoded |json| buffer of size |json_size| and
/// returns a dictionary or list representation. If JSON parsing fails this
/// method returns NULL.
///
/*--acf(capi_name=acf_parse_json_buffer)--*/
AcfRefPtr<AcfValue> AcfParseJSON(const void* json,
                                 size_t json_size,
                                 acf_json_parser_options_t options);

///
/// Parses the specified |json_string| and returns a dictionary or list
/// representation. If JSON parsing fails this method returns NULL and populates
/// |error_msg_out| with a formatted error message.
///
/*--acf()--*/
AcfRefPtr<AcfValue> AcfParseJSONAndReturnError(
    const AcfString& json_string,
    acf_json_parser_options_t options,
    AcfString& error_msg_out);

///
/// Generates a JSON string from the specified root |node| which should be a
/// dictionary or list value. Returns an empty string on failure. This method
/// requires exclusive access to |node| including any underlying data.
///
/*--acf()--*/
AcfString AcfWriteJSON(AcfRefPtr<AcfValue> node,
                       acf_json_writer_options_t options);

#endif  // ACF_INCLUDE_ACF_PARSER_H_
