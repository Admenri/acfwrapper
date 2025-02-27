#include "libacf_dll/transfer_util.h"

void transfer_string_list_contents(acf_string_list_t fromList,
                                   StringList& toList) {
  size_t size = acf_string_list_size(fromList);
  AcfString value;

  for (size_t i = 0; i < size; i++) {
    acf_string_list_value(fromList, i, value.GetWritableStruct());
    toList.push_back(value);
  }
}

void transfer_string_list_contents(const StringList& fromList,
                                   acf_string_list_t toList) {
  size_t size = fromList.size();
  for (size_t i = 0; i < size; ++i)
    acf_string_list_append(toList, fromList[i].GetStruct());
}

void transfer_string_map_contents(acf_string_map_t fromMap, StringMap& toMap) {
  size_t size = acf_string_map_size(fromMap);
  AcfString key, value;

  for (size_t i = 0; i < size; ++i) {
    acf_string_map_key(fromMap, i, key.GetWritableStruct());
    acf_string_map_value(fromMap, i, value.GetWritableStruct());

    toMap.insert(std::make_pair(key, value));
  }
}

void transfer_string_map_contents(const StringMap& fromMap,
                                  acf_string_map_t toMap) {
  StringMap::const_iterator it = fromMap.begin();
  for (; it != fromMap.end(); ++it)
    acf_string_map_append(toMap, it->first.GetStruct(), it->second.GetStruct());
}

void transfer_string_multimap_contents(acf_string_multimap_t fromMap,
                                       StringMultimap& toMap) {
  size_t size = acf_string_multimap_size(fromMap);
  AcfString key, value;

  for (size_t i = 0; i < size; ++i) {
    acf_string_multimap_key(fromMap, i, key.GetWritableStruct());
    acf_string_multimap_value(fromMap, i, value.GetWritableStruct());

    toMap.insert(std::make_pair(key, value));
  }
}

void transfer_string_multimap_contents(const StringMultimap& fromMap,
                                       acf_string_multimap_t toMap) {
  StringMultimap::const_iterator it = fromMap.begin();
  for (; it != fromMap.end(); ++it) {
    acf_string_multimap_append(toMap, it->first.GetStruct(),
                               it->second.GetStruct());
  }
}
