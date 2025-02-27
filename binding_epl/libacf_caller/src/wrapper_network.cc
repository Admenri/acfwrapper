#include <iostream>

#include "include/acf_request.h"
#include "include/acf_resource_request_handler.h"
#include "wrapper_utility.h"

extern DWORD acf_cpp_fntable_postdataelements[];
extern DWORD acf_cpp_fntable_postdata[];
extern DWORD acf_cpp_fntable_request[];
extern DWORD acf_cpp_fntable_response[];

namespace {

BOOL ACF_CALLBACK is_readonly(AcfRequest* obj) { return obj->IsReadOnly(); }

LPCSTR ACF_CALLBACK get_url(AcfRequest* obj) {
  return GetEString(obj->GetURL());
}

void ACF_CALLBACK set_url(AcfRequest* obj, LPCSTR v) { obj->SetURL(v); }

LPCSTR ACF_CALLBACK get_method(AcfRequest* obj) {
  return GetEString(obj->GetMethod());
}

void ACF_CALLBACK set_method(AcfRequest* obj, LPCSTR v) { obj->SetMethod(v); }

void ACF_CALLBACK set_referrer(AcfRequest* obj, LPCSTR v,
                               acf_referrer_policy_t p) {
  obj->SetReferrer(v, p);
}

LPCSTR ACF_CALLBACK get_referrer_url(AcfRequest* obj) {
  return GetEString(obj->GetReferrerURL());
}

acf_referrer_policy_t ACF_CALLBACK get_referrer_policy(AcfRequest* obj) {
  return obj->GetReferrerPolicy();
}

void ACF_CALLBACK set_header_map(AcfRequest* obj, LPVOID* eArray) {
  std::multimap<AcfString, AcfString> keys;

  LPSTR pAry = (LPSTR)*eArray;

  int nSize = *(LPINT)(pAry + sizeof(INT));

  PACF_STRING_PAIR* pPair = (PACF_STRING_PAIR*)(pAry + sizeof(INT) * 2);

  for (size_t i = 0; i < nSize; i++) {
    LPCSTR tmpKey = pPair[i]->key;
    LPCSTR tmpValue = pPair[i]->value;

    keys.insert(std::make_pair(tmpKey ? tmpKey : std::string(),
                               tmpValue ? tmpValue : std::string()));
  }

  obj->SetHeaderMap(keys);
}

BOOL ACF_CALLBACK get_header_map(AcfRequest* obj, LPVOID* eArray) {
  std::multimap<AcfString, AcfString> keys;
  obj->GetHeaderMap(keys);

  DWORD* pStrs = new DWORD[keys.size()];
  int index = 0;
  for (auto iter = keys.begin(); iter != keys.end(); iter++) {
    pStrs[index] = (DWORD) new ACF_STRING_PAIR;
    ((PACF_STRING_PAIR)pStrs[index])->key = GetEString((*iter).first.c_str());
    ((PACF_STRING_PAIR)pStrs[index])->value =
        GetEString((*iter).second.c_str());
    ++index;
  }

  FreeAryElement(*eArray);

  int nSize = keys.size() * sizeof(DWORD);
  LPSTR pAry = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
  *(LPINT)pAry = 1;
  *(LPINT)(pAry + sizeof(INT)) = keys.size();
  memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
  delete[] pStrs;

  *eArray = pAry;

  return eArray != NULL;
}

LPCSTR ACF_CALLBACK get_header_by_name(AcfRequest* obj, LPCSTR name) {
  return GetEString(obj->GetHeaderByName(name));
}

void ACF_CALLBACK set_header_by_name(AcfRequest* obj, LPCSTR name, LPCSTR value,
                                     BOOL overwrite) {
  obj->SetHeaderByName(name, value, overwrite);
}

acf_resource_type_t ACF_CALLBACK get_resource_type(AcfRequest* obj) {
  return obj->GetResourceType();
}

acf_transition_type_t ACF_CALLBACK get_transition_type(AcfRequest* obj) {
  return obj->GetTransitionType();
}

void ACF_CALLBACK get_id(AcfRequest* obj, int64_t* v) {
  *v = obj->GetIdentifier();
}

BOOL ACF_CALLBACK get_post_data(AcfRequest* obj, DWORD* retObj) {
  AcfRefPtr<AcfPostData> pd = obj->GetPostData();

  if (pd) {
    pd->AddRef();
    retObj[1] = (DWORD)pd.get();
    retObj[2] = (DWORD)acf_cpp_fntable_postdata;
  }

  return !!pd;
}

void ACF_CALLBACK set_post_data(AcfRequest* obj, AcfPostData* v) {
  obj->SetPostData(v);
}

}  // namespace

DWORD acf_cpp_fntable_request[] = {
    (DWORD)is_readonly,
    (DWORD)get_url,
    (DWORD)set_url,
    (DWORD)get_method,
    (DWORD)set_method,
    (DWORD)set_referrer,
    (DWORD)get_referrer_url,
    (DWORD)get_referrer_policy,
    (DWORD)get_header_map,
    (DWORD)set_header_map,
    (DWORD)get_header_by_name,
    (DWORD)set_header_by_name,
    (DWORD)get_resource_type,
    (DWORD)get_transition_type,
    (DWORD)get_id,
    (DWORD)get_post_data,
    (DWORD)set_post_data,
};

DLL_EXPORTS(CreateRequest, BOOL)(DWORD* target) {
  AcfRefPtr<AcfRequest> lpObj = AcfRequest::Create();

  lpObj->AddRef();
  target[1] = (DWORD)((LPVOID)lpObj.get());
  target[2] = (DWORD)acf_cpp_fntable_request;

  return !!lpObj;
}

namespace {

BOOL ACF_CALLBACK postdata_is_read_only(AcfPostData* obj) {
  return obj->IsReadOnly();
}

BOOL ACF_CALLBACK postdata_has_excluded_elements(AcfPostData* obj) {
  return obj->HasExcludedElements();
}

size_t ACF_CALLBACK postdata_get_element_count(AcfPostData* obj) {
  return obj->GetElementCount();
}

BOOL ACF_CALLBACK postdata_get_elements(AcfPostData* obj, LPVOID* eArray) {
  ISVALIDR(obj, FALSE);

  AcfPostData::ElementVector arguments;
  obj->GetElements(arguments);

  DWORD* pStrs = new DWORD[arguments.size()];
  for (size_t i = 0; i < arguments.size(); i++) {
    PECLASS_VFPTR pNewClass = new ECLASS_VFPTR;
    pNewClass->dwVfTable = eClass::m_pVfTable_PostDataElements;
    pNewClass->pCFuncs = acf_cpp_fntable_postdataelements;
    arguments[i]->AddRef();
    pNewClass->pObject = ((LPVOID)(arguments[i].get()));

    pStrs[i] = (DWORD)pNewClass;
  }

  int nSize = arguments.size() * sizeof(DWORD);
  LPSTR pAry = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
  *(LPINT)pAry = 1;
  *(LPINT)(pAry + sizeof(INT)) = arguments.size();
  memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
  delete[] pStrs;

  *eArray = pAry;

  return eArray != NULL;
}

BOOL ACF_CALLBACK postdata_remove_element(AcfPostData* obj,
                                          AcfPostDataElement* element) {
  return obj->RemoveElement(element);
}

BOOL ACF_CALLBACK postdata_add_element(AcfPostData* obj,
                                       AcfPostDataElement* element) {
  return obj->AddElement(element);
}

void ACF_CALLBACK postdata_remove_elements(AcfPostData* obj) {
  obj->RemoveElements();
}

}  // namespace

DWORD acf_cpp_fntable_postdata[] = {
    (DWORD)postdata_is_read_only,      (DWORD)postdata_has_excluded_elements,
    (DWORD)postdata_get_element_count, (DWORD)postdata_get_elements,
    (DWORD)postdata_remove_element,    (DWORD)postdata_add_element,
    (DWORD)postdata_remove_elements,
};

DLL_EXPORTS(CreatePostData, BOOL)(DWORD* target) {
  AcfRefPtr<AcfPostData> lpObj = AcfPostData::Create();

  lpObj->AddRef();
  target[1] = (DWORD)((LPVOID)lpObj.get());
  target[2] = (DWORD)acf_cpp_fntable_postdata;

  return !!lpObj;
}

namespace {

BOOL ACF_CALLBACK postelements_is_readonly(AcfPostDataElement* obj) {
  return obj->IsReadOnly();
}

void ACF_CALLBACK postelements_set_empty(AcfPostDataElement* obj) {
  obj->SetToEmpty();
}

void ACF_CALLBACK postelements_set_file(AcfPostDataElement* obj, LPCSTR file) {
  obj->SetToFile(file);
}

void ACF_CALLBACK postelements_set_bytes(AcfPostDataElement* obj, int size,
                                         LPVOID buffer) {
  obj->SetToBytes(size, buffer);
}

acf_postdataelement_type_t postelements_get_type(AcfPostDataElement* obj) {
  return obj->GetType();
}

LPCSTR ACF_CALLBACK postelements_get_file(AcfPostDataElement* obj) {
  return GetEString(obj->GetFile());
}

size_t ACF_CALLBACK postelements_get_bytes_count(AcfPostDataElement* obj) {
  return obj->GetBytesCount();
}

int ACF_CALLBACK postelement_get_bytes(AcfPostDataElement* obj, int size,
                                       LPVOID bytes) {
  return obj->GetBytes(size, bytes);
}

}  // namespace

DWORD acf_cpp_fntable_postdataelements[] = {
    (DWORD)postelements_is_readonly,     (DWORD)postelements_set_empty,
    (DWORD)postelements_set_file,        (DWORD)postelements_set_bytes,
    (DWORD)postelements_get_type,        (DWORD)postelements_get_file,
    (DWORD)postelements_get_bytes_count, (DWORD)postelement_get_bytes,
};

DLL_EXPORTS(CreatePostDataElements, BOOL)(DWORD* target) {
  AcfRefPtr<AcfPostDataElement> lpObj = AcfPostDataElement::Create();

  lpObj->AddRef();
  target[1] = (DWORD)((LPVOID)lpObj.get());
  target[2] = (DWORD)acf_cpp_fntable_postdataelements;

  return !!lpObj;
}

namespace {

BOOL ACF_CALLBACK response_is_read_only(AcfResponse* obj) {
  ISVALIDR(obj, NULL);

  return obj->IsReadOnly();
}

int ACF_CALLBACK response_get_error(AcfResponse* obj) {
  ISVALIDR(obj, NULL);

  return obj->GetError();
}

void ACF_CALLBACK response_set_error(AcfResponse* obj, int error) {
  ISVALID(obj);

  obj->SetError(error);
}

int ACF_CALLBACK response_get_status(AcfResponse* obj) {
  ISVALIDR(obj, NULL);

  return obj->GetStatus();
}

void ACF_CALLBACK response_set_status(AcfResponse* obj, int status) {
  ISVALID(obj);

  obj->SetStatus(status);
}

LPCSTR ACF_CALLBACK response_get_status_text(AcfResponse* obj) {
  ISVALIDR(obj, NULL);

  return GetEString(obj->GetStatusText());
}

void ACF_CALLBACK response_set_status_text(AcfResponse* obj, LPCSTR status) {
  ISVALID(obj);

  obj->SetStatusText(status);
}

LPCSTR ACF_CALLBACK response_get_mime_type(AcfResponse* obj) {
  ISVALIDR(obj, NULL);

  return GetEString(obj->GetMimeType());
}

void ACF_CALLBACK response_set_mime_type(AcfResponse* obj, LPCSTR status) {
  ISVALID(obj);

  obj->SetMimeType(status);
}

LPCSTR ACF_CALLBACK response_get_charset(AcfResponse* obj) {
  ISVALIDR(obj, NULL);

  return GetEString(obj->GetCharset());
}

void ACF_CALLBACK response_set_charset(AcfResponse* obj, LPCSTR status) {
  ISVALID(obj);

  obj->SetCharset(status);
}

LPCSTR ACF_CALLBACK response_get_header_by_name(AcfResponse* obj, LPCSTR name) {
  ISVALIDR(obj, NULL);

  return GetEString(obj->GetHeaderByName(name));
}

void ACF_CALLBACK response_set_header_by_name(AcfResponse* obj, LPCSTR name,
                                              LPCSTR value, BOOL overwrite) {
  ISVALID(obj);

  obj->SetHeaderByName(name, value, overwrite);
}

BOOL ACF_CALLBACK response_get_header_map(AcfResponse* obj, LPVOID* eArray) {
  ISVALIDR(obj, NULL);

  std::multimap<AcfString, AcfString> keys;
  obj->GetHeaderMap(keys);

  DWORD* pStrs = new DWORD[keys.size()];
  int index = 0;
  for (auto iter = keys.begin(); iter != keys.end(); iter++) {
    pStrs[index] = (DWORD) new ACF_STRING_PAIR;
    ((PACF_STRING_PAIR)pStrs[index])->key = GetEString((*iter).first.c_str());
    ((PACF_STRING_PAIR)pStrs[index])->value =
        GetEString((*iter).second.c_str());
    ++index;
  }

  FreeAryElement(*eArray);

  int nSize = keys.size() * sizeof(DWORD);
  LPSTR pAry = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
  *(LPINT)pAry = 1;
  *(LPINT)(pAry + sizeof(INT)) = keys.size();
  memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
  delete[] pStrs;

  *eArray = pAry;

  return eArray != NULL;
}

void ACF_CALLBACK response_set_header_map(AcfResponse* obj, LPVOID* eArray) {
  ISVALID(obj);

  std::multimap<AcfString, AcfString> keys;

  LPSTR pAry = (LPSTR)*eArray;

  int nSize = *(LPINT)(pAry + sizeof(INT));

  PACF_STRING_PAIR* pPair = (PACF_STRING_PAIR*)(pAry + sizeof(INT) * 2);

  for (size_t i = 0; i < nSize; i++) {
    LPCSTR tmpKey = GetEString(pPair[i]->key);
    LPCSTR tmpValue = GetEString(pPair[i]->value);

    keys.insert(std::make_pair(tmpKey ? tmpKey : std::string(),
                               tmpValue ? tmpValue : std::string()));
  }

  obj->SetHeaderMap(keys);
}

LPCSTR ACF_CALLBACK response_get_url(AcfResponse* obj) {
  ISVALIDR(obj, NULL);

  return GetEString(obj->GetURL());
}

void ACF_CALLBACK response_set_url(AcfResponse* obj, LPCSTR status) {
  ISVALID(obj);

  obj->SetURL(status);
}

}  // namespace

DWORD acf_cpp_fntable_response[] = {
    (DWORD)&response_is_read_only,
    (DWORD)&response_get_error,
    (DWORD)&response_set_error,
    (DWORD)&response_get_status,
    (DWORD)&response_set_status,
    (DWORD)&response_get_status_text,
    (DWORD)&response_set_status_text,
    (DWORD)&response_get_mime_type,
    (DWORD)&response_set_mime_type,
    (DWORD)&response_get_charset,
    (DWORD)&response_set_charset,
    (DWORD)&response_get_header_by_name,
    (DWORD)&response_set_header_by_name,
    (DWORD)&response_get_header_map,
    (DWORD)&response_set_header_map,
    (DWORD)&response_get_url,
    (DWORD)&response_set_url,
};

DLL_EXPORTS(CreateResponse, BOOL)(DWORD* target) {
  AcfRefPtr<AcfResponse> lpObj = AcfResponse::Create();

  lpObj->AddRef();
  target[1] = (DWORD)((LPVOID)lpObj.get());
  target[2] = (DWORD)acf_cpp_fntable_response;

  return !!lpObj;
}
