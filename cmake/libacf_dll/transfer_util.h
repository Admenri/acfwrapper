#ifndef ACF_TRANSFER_UTIL_H_
#define ACF_TRANSFER_UTIL_H_

#include <map>
#include <vector>

#include "include/internal/acf_string_list.h"
#include "include/internal/acf_string_map.h"
#include "include/internal/acf_string_multimap.h"

// Copy contents from one list type to another.
using StringList = std::vector<AcfString>;
void transfer_string_list_contents(acf_string_list_t fromList,
                                   StringList& toList);
void transfer_string_list_contents(const StringList& fromList,
                                   acf_string_list_t toList);

// Copy contents from one map type to another.
using StringMap = std::map<AcfString, AcfString>;
void transfer_string_map_contents(acf_string_map_t fromMap, StringMap& toMap);
void transfer_string_map_contents(const StringMap& fromMap,
                                  acf_string_map_t toMap);

// Copy contents from one map type to another.
using StringMultimap = std::multimap<AcfString, AcfString>;
void transfer_string_multimap_contents(acf_string_multimap_t fromMap,
                                       StringMultimap& toMap);
void transfer_string_multimap_contents(const StringMultimap& fromMap,
                                       acf_string_multimap_t toMap);

#endif  // ACF_TRANSFER_UTIL_H_
