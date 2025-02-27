#include "include/acf_environment.h"
#include "include/acf_values.h"
#include "wrapper_types.h"
#include "wrapper_utility.h"

extern DWORD acf_cpp_fntable_value[];
extern DWORD acf_cpp_fntable_binary[];
extern DWORD acf_cpp_fntable_dictionary[];
extern DWORD acf_cpp_fntable_list[];

// Value namespace define
namespace {

BOOL ACF_CALLBACK value_is_valid(AcfValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsValid();
}

BOOL ACF_CALLBACK value_is_owned(AcfValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsOwned();
}

BOOL ACF_CALLBACK value_is_read_only(AcfValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsReadOnly();
}

BOOL ACF_CALLBACK value_is_same(AcfValue* obj, AcfValue* that) {
  ISVALIDR(obj, NULL);

  return obj->IsSame(that);
}

BOOL ACF_CALLBACK value_is_equal(AcfValue* obj, AcfValue* that) {
  ISVALIDR(obj, NULL);

  return obj->IsEqual(that);
}

BOOL ACF_CALLBACK value_copy(AcfValue* obj, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfValue> dupObj = obj->Copy();

  dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_value;

  return !!dupObj;
}

int ACF_CALLBACK value_get_type(AcfValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->GetType();
}

BOOL ACF_CALLBACK value_get_bool(AcfValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->GetBool();
}

int ACF_CALLBACK value_get_int(AcfValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->GetInt();
}

BOOL ACF_CALLBACK value_get_double(AcfValue* obj, double* ret_val) {
  ISVALIDR(obj, NULL);

  *ret_val = obj->GetDouble();

  return !!ret_val;
}

BOOL ACF_CALLBACK value_get_string(AcfValue* obj, LPSTR* outStr) {
  ISVALIDR(obj, NULL);

  *outStr = GetEString(obj->GetString());

  return !!*outStr;
}

BOOL ACF_CALLBACK value_get_binary(AcfValue* obj, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfBinaryValue> dupObj = obj->GetBinary();

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_binary;
  ;

  return !!dupObj;
}

BOOL ACF_CALLBACK value_get_dictionary(AcfValue* obj, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfDictionaryValue> dupObj = obj->GetDictionary();

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_dictionary;
  ;

  return !!dupObj;
}

BOOL ACF_CALLBACK value_get_list(AcfValue* obj, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfListValue> dupObj = obj->GetList();

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_list;
  ;

  return !!dupObj;
}

BOOL ACF_CALLBACK value_set_null(AcfValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->SetNull();
}

BOOL ACF_CALLBACK value_set_bool(AcfValue* obj, BOOL value) {
  ISVALIDR(obj, NULL);

  return obj->SetBool(value);
}

BOOL ACF_CALLBACK value_set_int(AcfValue* obj, int value) {
  ISVALIDR(obj, NULL);

  return obj->SetInt(value);
}

BOOL ACF_CALLBACK value_set_double(AcfValue* obj, double* value) {
  ISVALIDR(obj, NULL);

  return obj->SetDouble(*value);
}

BOOL ACF_CALLBACK value_set_string(AcfValue* obj, LPCSTR value) {
  ISVALIDR(obj, NULL);

  return obj->SetString(value);
}

BOOL ACF_CALLBACK value_set_binary(AcfValue* obj, AcfBinaryValue* value) {
  ISVALIDR(obj, NULL);

  return obj->SetBinary(value);
}

BOOL ACF_CALLBACK value_set_dictionary(AcfValue* obj,
                                       AcfDictionaryValue* value) {
  ISVALIDR(obj, NULL);

  return obj->SetDictionary(value);
}

BOOL ACF_CALLBACK value_set_list(AcfValue* obj, AcfListValue* value) {
  ISVALIDR(obj, NULL);

  return obj->SetList(value);
}

}  // namespace

DLL_EXPORTS(ValueCreate, BOOL)(DWORD* target) {
  AcfRefPtr<AcfValue> lpObj = AcfValue::Create();

  lpObj->AddRef();
  target[1] = (DWORD)((LPVOID)lpObj.get());
  target[2] = (DWORD)acf_cpp_fntable_value;

  return !!lpObj;
}

DWORD acf_cpp_fntable_value[] = {
    (DWORD)&value_is_valid,       (DWORD)&value_is_owned,
    (DWORD)&value_is_read_only,   (DWORD)&value_is_same,
    (DWORD)&value_is_equal,       (DWORD)&value_copy,
    (DWORD)&value_get_type,       (DWORD)&value_get_bool,
    (DWORD)&value_get_int,        (DWORD)&value_get_double,
    (DWORD)&value_get_string,     (DWORD)&value_get_binary,
    (DWORD)&value_get_dictionary, (DWORD)&value_get_list,
    (DWORD)&value_set_null,       (DWORD)&value_set_bool,
    (DWORD)&value_set_int,        (DWORD)&value_set_double,
    (DWORD)&value_set_string,     (DWORD)&value_set_binary,
    (DWORD)&value_set_dictionary, (DWORD)&value_set_list,
};

// Binary namespace define
namespace {

BOOL ACF_CALLBACK binary_is_valid(AcfBinaryValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsValid();
}

BOOL ACF_CALLBACK binary_is_owned(AcfBinaryValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsOwned();
}

BOOL ACF_CALLBACK binary_is_same(AcfBinaryValue* obj, AcfBinaryValue* that) {
  ISVALIDR(obj, NULL);

  return obj->IsSame(that);
}

BOOL ACF_CALLBACK binary_is_equal(AcfBinaryValue* obj, AcfBinaryValue* that) {
  ISVALIDR(obj, NULL);

  return obj->IsEqual(that);
}

BOOL ACF_CALLBACK binary_copy(AcfBinaryValue* obj, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfBinaryValue> dupObj = obj->Copy();

  dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_binary;

  return !!dupObj;
}

int ACF_CALLBACK binary_get_size(AcfBinaryValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->GetSize();
}

int ACF_CALLBACK binary_get_data(AcfBinaryValue* obj, LPVOID buffer, int size,
                                 int offset) {
  ISVALIDR(obj, NULL);

  return obj->GetData(buffer, size, offset);
}

}  // namespace

DLL_EXPORTS(BinaryValueCreate, BOOL)(DWORD* target, LPVOID buffer, int size) {
  AcfRefPtr<AcfBinaryValue> lpObj = AcfBinaryValue::Create(buffer, size);

  lpObj->AddRef();
  target[1] = (DWORD)((LPVOID)lpObj.get());
  target[2] = (DWORD)acf_cpp_fntable_binary;

  return !!lpObj;
}

DWORD acf_cpp_fntable_binary[] = {
    (DWORD)&binary_is_valid, (DWORD)&binary_is_owned, (DWORD)&binary_is_same,
    (DWORD)&binary_is_equal, (DWORD)&binary_copy,     (DWORD)&binary_get_size,
    (DWORD)&binary_get_data,
};

// Dictionary namespace define
namespace {

BOOL ACF_CALLBACK dictionary_is_valid(AcfDictionaryValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsValid();
}

BOOL ACF_CALLBACK dictionary_is_owned(AcfDictionaryValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsOwned();
}

BOOL ACF_CALLBACK dictionary_is_read_only(AcfDictionaryValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsReadOnly();
}

BOOL ACF_CALLBACK dictionary_is_same(AcfDictionaryValue* obj,
                                     AcfDictionaryValue* that) {
  ISVALIDR(obj, NULL);

  return obj->IsSame(that);
}

BOOL ACF_CALLBACK dictionary_is_equal(AcfDictionaryValue* obj,
                                      AcfDictionaryValue* that) {
  ISVALIDR(obj, NULL);

  return obj->IsEqual(that);
}

BOOL ACF_CALLBACK dictionary_copy(AcfDictionaryValue* obj, DWORD* target,
                                  BOOL exclude_empty) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfDictionaryValue> dupObj = obj->Copy(exclude_empty);

  dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_dictionary;

  return !!dupObj;
}

int ACF_CALLBACK dictionary_get_size(AcfDictionaryValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->GetSize();
}

BOOL ACF_CALLBACK dictionary_clear(AcfDictionaryValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->Clear();
}

BOOL ACF_CALLBACK dictionary_has_key(AcfDictionaryValue* obj, LPCSTR key) {
  ISVALIDR(obj, NULL);

  return obj->HasKey(key);
}

BOOL ACF_CALLBACK dictionary_get_keys(AcfDictionaryValue* obj, LPVOID* eArray) {
  ISVALIDR(obj, NULL);

  std::vector<AcfString> keys;
  obj->GetKeys(keys);

  LPSTR* pStrs = new LPSTR[keys.size()];
  for (size_t i = 0; i < keys.size(); i++) {
    pStrs[i] = GetEString(keys[i].c_str());
  }

  FreeAryElement(*eArray);

  int nSize = keys.size() * sizeof(DWORD);
  LPSTR pAry = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
  *(LPINT)pAry = 1;
  *(LPINT)(pAry + sizeof(INT)) = keys.size();
  memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
  delete[] pStrs;

  *eArray = pAry;

  return !!eArray;
}

BOOL ACF_CALLBACK dictionary_remove(AcfDictionaryValue* obj, LPCSTR key) {
  ISVALIDR(obj, NULL);

  return obj->Remove(key);
}

int ACF_CALLBACK dictionary_get_type(AcfDictionaryValue* obj, LPCSTR key) {
  ISVALIDR(obj, NULL);

  return obj->GetType(key);
}

BOOL ACF_CALLBACK dictionary_get_value(AcfDictionaryValue* obj, LPCSTR key,
                                       DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfValue> dupObj = obj->GetValue(key);

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_value;

  return !!dupObj;
}

BOOL ACF_CALLBACK dictionary_get_bool(AcfDictionaryValue* obj, LPCSTR key) {
  ISVALIDR(obj, NULL);

  return obj->GetBool(key);
}

int ACF_CALLBACK dictionary_get_int(AcfDictionaryValue* obj, LPCSTR key) {
  ISVALIDR(obj, NULL);

  return obj->GetInt(key);
}

BOOL ACF_CALLBACK dictionary_get_double(AcfDictionaryValue* obj, LPCSTR key,
                                        double* ret_val) {
  ISVALIDR(obj, NULL);

  *ret_val = obj->GetDouble(key);

  return !!ret_val;
}

BOOL ACF_CALLBACK dictionary_get_string(AcfDictionaryValue* obj, LPCSTR key,
                                        LPSTR* outStr) {
  ISVALIDR(obj, NULL);

  *outStr = GetEString(obj->GetString(key));

  return !!*outStr;
}

BOOL ACF_CALLBACK dictionary_get_binary(AcfDictionaryValue* obj, LPCSTR key,
                                        DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfBinaryValue> dupObj = obj->GetBinary(key);

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_binary;

  return !!dupObj;
}

BOOL ACF_CALLBACK dictionary_get_dictionary(AcfDictionaryValue* obj, LPCSTR key,
                                            DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfDictionaryValue> dupObj = obj->GetDictionary(key);

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_dictionary;

  return !!dupObj;
}

BOOL ACF_CALLBACK dictionary_get_list(AcfDictionaryValue* obj, LPCSTR key,
                                      DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfListValue> dupObj = obj->GetList(key);

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_list;

  return !!dupObj;
}

BOOL ACF_CALLBACK dictionary_set_value(AcfDictionaryValue* obj, LPCSTR key,
                                       AcfValue* value) {
  ISVALIDR(obj, false);

  return obj->SetValue(key, value);
}

BOOL ACF_CALLBACK dictionary_set_bool(AcfDictionaryValue* obj, LPCSTR key,
                                      BOOL value) {
  ISVALIDR(obj, NULL);

  return obj->SetBool(key, value);
}

int ACF_CALLBACK dictionary_set_int(AcfDictionaryValue* obj, LPCSTR key,
                                    int value) {
  ISVALIDR(obj, NULL);

  return obj->SetInt(key, value);
}

BOOL ACF_CALLBACK dictionary_set_double(AcfDictionaryValue* obj, LPCSTR key,
                                        double* ret_val) {
  ISVALIDR(obj, NULL);

  return obj->SetDouble(key, *ret_val);
}

BOOL ACF_CALLBACK dictionary_set_string(AcfDictionaryValue* obj, LPCSTR key,
                                        LPCSTR value) {
  ISVALIDR(obj, NULL);

  return obj->SetString(key, value);
}

BOOL ACF_CALLBACK dictionary_set_binary(AcfDictionaryValue* obj, LPCSTR key,
                                        AcfBinaryValue* value) {
  ISVALIDR(obj, false);

  return obj->SetBinary(key, value);
}

BOOL ACF_CALLBACK dictionary_set_dictionary(AcfDictionaryValue* obj, LPCSTR key,
                                            AcfDictionaryValue* value) {
  ISVALIDR(obj, false);

  return obj->SetDictionary(key, value);
}

BOOL ACF_CALLBACK dictionary_set_list(AcfDictionaryValue* obj, LPCSTR key,
                                      AcfListValue* value) {
  ISVALIDR(obj, false);

  return obj->SetList(key, value);
}

}  // namespace

DLL_EXPORTS(DictionaryValueCreate, BOOL)(DWORD* target) {
  AcfRefPtr<AcfDictionaryValue> lpObj = AcfDictionaryValue::Create();

  lpObj->AddRef();
  target[1] = (DWORD)((LPVOID)lpObj.get());
  target[2] = (DWORD)acf_cpp_fntable_dictionary;

  return !!lpObj;
}

DWORD acf_cpp_fntable_dictionary[] = {
    (DWORD)&dictionary_is_valid,       (DWORD)&dictionary_is_owned,
    (DWORD)&dictionary_is_read_only,   (DWORD)&dictionary_is_same,
    (DWORD)&dictionary_is_equal,       (DWORD)&dictionary_copy,
    (DWORD)&dictionary_get_size,       (DWORD)&dictionary_clear,
    (DWORD)&dictionary_has_key,        (DWORD)&dictionary_get_keys,
    (DWORD)&dictionary_remove,         (DWORD)&dictionary_get_type,
    (DWORD)&dictionary_get_value,      (DWORD)&dictionary_get_bool,
    (DWORD)&dictionary_get_int,        (DWORD)&dictionary_get_double,
    (DWORD)&dictionary_get_string,     (DWORD)&dictionary_get_binary,
    (DWORD)&dictionary_get_dictionary, (DWORD)&dictionary_get_list,
    (DWORD)&dictionary_set_value,      (DWORD)&dictionary_set_bool,
    (DWORD)&dictionary_set_int,        (DWORD)&dictionary_set_double,
    (DWORD)&dictionary_set_string,     (DWORD)&dictionary_set_binary,
    (DWORD)&dictionary_set_dictionary, (DWORD)&dictionary_set_list,
};

// List namespace define
namespace {

BOOL ACF_CALLBACK list_is_valid(AcfListValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsValid();
}

BOOL ACF_CALLBACK list_is_owned(AcfListValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsOwned();
}

BOOL ACF_CALLBACK list_is_read_only(AcfListValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsReadOnly();
}

BOOL ACF_CALLBACK list_is_same(AcfListValue* obj, AcfListValue* that) {
  ISVALIDR(obj, NULL);

  return obj->IsSame(that);
}

BOOL ACF_CALLBACK list_is_equal(AcfListValue* obj, AcfListValue* that) {
  ISVALIDR(obj, NULL);

  return obj->IsEqual(that);
}

BOOL ACF_CALLBACK list_copy(AcfListValue* obj, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfListValue> dupObj = obj->Copy();

  dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_list;

  return !!dupObj;
}

BOOL ACF_CALLBACK list_set_size(AcfListValue* obj, int size) {
  ISVALIDR(obj, NULL);

  return obj->SetSize(size);
}

int ACF_CALLBACK list_get_size(AcfListValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->GetSize();
}

BOOL ACF_CALLBACK list_clear(AcfListValue* obj) {
  ISVALIDR(obj, NULL);

  return obj->Clear();
}

BOOL ACF_CALLBACK list_remove(AcfListValue* obj, int index) {
  ISVALIDR(obj, NULL);

  return obj->Remove(index);
}

int ACF_CALLBACK list_get_type(AcfListValue* obj, int index) {
  ISVALIDR(obj, NULL);

  return obj->GetType(index);
}

BOOL ACF_CALLBACK list_get_value(AcfListValue* obj, int key, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfValue> dupObj = obj->GetValue(key);

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_value;

  return !!dupObj;
}

BOOL ACF_CALLBACK list_get_bool(AcfListValue* obj, int key) {
  ISVALIDR(obj, NULL);

  return obj->GetBool(key);
}

int ACF_CALLBACK list_get_int(AcfListValue* obj, int key) {
  ISVALIDR(obj, NULL);

  return obj->GetInt(key);
}

BOOL ACF_CALLBACK list_get_double(AcfListValue* obj, int key, double* ret_val) {
  ISVALIDR(obj, NULL);

  *ret_val = obj->GetDouble(key);

  return !!ret_val;
}

BOOL ACF_CALLBACK list_get_string(AcfListValue* obj, int key, LPSTR* outStr) {
  ISVALIDR(obj, NULL);

  *outStr = GetEString(obj->GetString(key));

  return !!*outStr;
}

BOOL ACF_CALLBACK list_get_binary(AcfListValue* obj, int key, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfBinaryValue> dupObj = obj->GetBinary(key);

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_binary;

  return !!dupObj;
}

BOOL ACF_CALLBACK list_get_dictionary(AcfListValue* obj, int key,
                                      DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfDictionaryValue> dupObj = obj->GetDictionary(key);

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_dictionary;

  return !!dupObj;
}

BOOL ACF_CALLBACK list_get_list(AcfListValue* obj, int key, DWORD* target) {
  ISVALIDR(obj, false);

  AcfRefPtr<AcfListValue> dupObj = obj->GetList(key);

  if (dupObj) dupObj->AddRef();
  target[1] = (DWORD)((LPVOID)dupObj.get());
  target[2] = (DWORD)acf_cpp_fntable_list;

  return !!dupObj;
}

BOOL ACF_CALLBACK list_set_value(AcfListValue* obj, int key, AcfValue* value) {
  ISVALIDR(obj, false);

  return obj->SetValue(key, value);
}

BOOL ACF_CALLBACK list_set_bool(AcfListValue* obj, int key, BOOL value) {
  ISVALIDR(obj, NULL);

  return obj->SetBool(key, value);
}

int ACF_CALLBACK list_set_int(AcfListValue* obj, int key, int value) {
  ISVALIDR(obj, NULL);

  return obj->SetInt(key, value);
}

BOOL ACF_CALLBACK list_set_double(AcfListValue* obj, int key, double* ret_val) {
  ISVALIDR(obj, NULL);

  return obj->SetDouble(key, *ret_val);
}

BOOL ACF_CALLBACK list_set_string(AcfListValue* obj, int key, LPCSTR value) {
  ISVALIDR(obj, NULL);

  return obj->SetString(key, value);
}

BOOL ACF_CALLBACK list_set_binary(AcfListValue* obj, int key,
                                  AcfBinaryValue* value) {
  ISVALIDR(obj, false);

  return obj->SetBinary(key, value);
}

BOOL ACF_CALLBACK list_set_dictionary(AcfListValue* obj, int key,
                                      AcfDictionaryValue* value) {
  ISVALIDR(obj, false);

  return obj->SetDictionary(key, value);
}

BOOL ACF_CALLBACK list_set_list(AcfListValue* obj, int key,
                                AcfListValue* value) {
  ISVALIDR(obj, false);

  return obj->SetList(key, value);
}

}  // namespace

DLL_EXPORTS(ListValueCreate, BOOL)(DWORD* target) {
  AcfRefPtr<AcfListValue> lpObj = AcfListValue::Create();

  lpObj->AddRef();
  target[1] = (DWORD)((LPVOID)lpObj.get());
  target[2] = (DWORD)acf_cpp_fntable_list;

  return !!lpObj;
}

DWORD acf_cpp_fntable_list[] = {
    (DWORD)&list_is_valid,     (DWORD)&list_is_owned,
    (DWORD)&list_is_read_only, (DWORD)&list_is_same,
    (DWORD)&list_is_equal,     (DWORD)&list_copy,
    (DWORD)&list_set_size,     (DWORD)&list_get_size,
    (DWORD)&list_clear,        (DWORD)&list_remove,
    (DWORD)&list_get_type,     (DWORD)&list_get_value,
    (DWORD)&list_get_bool,     (DWORD)&list_get_int,
    (DWORD)&list_get_double,   (DWORD)&list_get_string,
    (DWORD)&list_get_binary,   (DWORD)&list_get_dictionary,
    (DWORD)&list_get_list,     (DWORD)&list_set_value,
    (DWORD)&list_set_bool,     (DWORD)&list_set_int,
    (DWORD)&list_set_double,   (DWORD)&list_set_string,
    (DWORD)&list_set_binary,   (DWORD)&list_set_dictionary,
    (DWORD)&list_set_list,
};
