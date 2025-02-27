
#include <iostream>

#include "include/acf_environment.h"
#include "include/acf_profile.h"
#include "wrapper_utility.h"

extern DWORD acf_cpp_fntable_profile[];
extern DWORD acf_cpp_fntable_environment[];
extern DWORD acf_cpp_fntable_cookiemanager[];

namespace {

BOOL ACF_CALLBACK is_valid(AcfProfile* obj) {
  return obj->IsValid();
}

LPSTR ACF_CALLBACK get_path(AcfProfile* obj) {
  return GetEString(obj->GetPath());
}

void ACF_CALLBACK set_prefs(AcfProfile* obj, LPCSTR name, AcfValue* value) {
  std::unique_ptr<std::atomic<BOOL>> notify =
      std::make_unique<std::atomic<BOOL>>(false);
  obj->SetPreference(name, value, nullptr);
}

void ACF_CALLBACK remove_browsing_data(AcfProfile* obj, uint64_t* type) {
  std::unique_ptr<std::atomic<BOOL>> notify =
      std::make_unique<std::atomic<BOOL>>(false);
  obj->RemoveBrowsingData(*type, nullptr);
}

void ACF_CALLBACK get_cookie_manager(AcfProfile* obj, DWORD* retObj) {
  AcfRefPtr<AcfCookieManager> ckm = obj->GetCookieManager();

  if (ckm) {
    ckm->AddRef();
    retObj[1] = (DWORD)ckm.get();
    retObj[2] = (DWORD)acf_cpp_fntable_cookiemanager;
  }
}

}  // namespace

DWORD acf_cpp_fntable_profile[] = {
    (DWORD)is_valid,           (DWORD)get_path,
    (DWORD)set_prefs,          (DWORD)remove_browsing_data,
    (DWORD)get_cookie_manager,
};

namespace {

PACF_COOKIE transfer_cookie_data(AcfRefPtr<AcfCookie> cookie) {
  PACF_COOKIE pCookie = new ACF_COOKIE;

  pCookie->name = GetEString(cookie->GetName());
  pCookie->value = GetEString(cookie->GetValue());
  pCookie->domain = GetEString(cookie->GetDomain());
  pCookie->path = GetEString(cookie->GetPath());

  pCookie->secure = cookie->IsSecure();
  pCookie->httponly = cookie->IsHttponly();

  pCookie->same_site = cookie->GetSameSite();
  pCookie->priority = cookie->GetPriority();

  pCookie->creation = cookie->GetCreation();
  pCookie->expires = cookie->GetExpiry();
  pCookie->last_access = cookie->GetLastAccess();
  pCookie->last_update = cookie->GetLastUpdate();

  return pCookie;
}

AcfRefPtr<AcfCookie> transfer_cookie_data(LPCSTR url, PACF_COOKIE cookie) {
  std::string name, value, domain, path;
  if (cookie->name && *cookie->name)
    name = cookie->name;
  if (cookie->value && *cookie->value)
    value = cookie->value;
  if (cookie->domain && *cookie->domain)
    domain = cookie->domain;
  if (cookie->path && *cookie->path)
    path = cookie->path;

  AcfRefPtr<AcfCookie> pCookie = AcfCookie::Create(
      url, name, value, domain, path, cookie->expires, cookie->secure,
      cookie->httponly, (acf_cookie_same_site_t)cookie->same_site,
      (acf_cookie_priority_t)cookie->priority);

  return pCookie;
}

class CookieVisitorSync : public AcfCookieVisitor {
 public:
  CookieVisitorSync(std::atomic<BOOL>* notify) : notify_(notify) {}
  ~CookieVisitorSync() {}

  std::vector<AcfRefPtr<AcfCookie>>& GetResult() { return data_; }

 protected:
  bool Visit(AcfRefPtr<AcfCookie> cookie, int count, int total) override {
    if (!cookie || !total) {
      data_.clear();
      *notify_ = true;
      return false;
    }

    data_.push_back(cookie);

    if (count + 1 >= total)
      *notify_ = true;

    return true;
  }

 private:
  std::vector<AcfRefPtr<AcfCookie>> data_;
  std::atomic<BOOL>* notify_;

  IMPLEMENT_REFCOUNTING(CookieVisitorSync);
};

int ACF_CALLBACK visit_cookies_sync(AcfCookieManager* obj,
                                    LPCSTR url,
                                    BOOL httpOnly,
                                    LPVOID* eArray) {
  ISVALIDR(obj, false);

  std::unique_ptr<std::atomic<BOOL>> notify =
      std::make_unique<std::atomic<BOOL>>(false);

  AcfRefPtr<CookieVisitorSync> lpHandler = new CookieVisitorSync(notify.get());
  obj->GetCookies(url, httpOnly, lpHandler);

  while (!*notify) {
    if (IsOnUIThread()) {
      MSG uiMsg;
      if (PeekMessage(&uiMsg, NULL, NULL, NULL, PM_REMOVE)) {
        TranslateMessage(&uiMsg);
        DispatchMessage(&uiMsg);
      }
    } else {
      ::Sleep(1);
    }
  }

  FreeAryElement(*eArray);
  std::vector<AcfRefPtr<AcfCookie>>& data = lpHandler->GetResult();

  DWORD* pStrs = new DWORD[data.size()];
  for (size_t i = 0; i < data.size(); i++)
    pStrs[i] = (DWORD)transfer_cookie_data(data[i]);

  int nSize = data.size() * sizeof(DWORD);
  LPSTR pAry = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(INT) * 2 + nSize);
  *(LPINT)pAry = 1;
  *(LPINT)(pAry + sizeof(INT)) = data.size();
  memcpy(pAry + sizeof(INT) * 2, pStrs, nSize);
  delete[] pStrs;

  *eArray = pAry;

  return lpHandler->GetResult().size();
}

BOOL ACF_CALLBACK set_cookie(AcfCookieManager* obj,
                             LPCSTR url,
                             PACF_COOKIE ckdata) {
  AcfRefPtr<AcfCookie> ck = transfer_cookie_data(url, ckdata);
  obj->SetCookie(url, ck, nullptr);
  return !!ck.get();
}

void ACF_CALLBACK delete_cookies(AcfCookieManager* obj,
                                 LPCSTR url,
                                 LPCSTR name) {
  obj->DeleteCookies(url, name, nullptr);
}

void ACF_CALLBACK flush_cookies(AcfCookieManager* obj) {
  obj->FlushStore(nullptr);
}

}  // namespace

DWORD acf_cpp_fntable_cookiemanager[] = {
    (DWORD)visit_cookies_sync,
    (DWORD)set_cookie,
    (DWORD)delete_cookies,
    (DWORD)flush_cookies,
};