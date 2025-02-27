// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_PROFILE_H_
#define ACF_INCLUDE_ACF_PROFILE_H_

#include "include/acf_callback.h"
#include "include/acf_values.h"
#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_types.h"
#include "include/internal/acf_types_wrappers.h"

#include <vector>

class AcfCookie;
class AcfValue;
class AcfProfile;
class AcfCompleteHandler;
class AcfCookieManager;
class AcfCookieVisitor;
class AcfEnvironment;

///
/// ACF's profile object for create browser in environment.
/// In chromium is user profile configure.
///
/*--acf(source=library)--*/
class AcfProfile : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Get host environment object
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfEnvironment> GetEnvironment() = 0;

  ///
  /// Get object remote state
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Is same object
  ///
  /*--acf()--*/
  virtual bool IsSame(AcfRefPtr<AcfProfile> that) = 0;

  ///
  /// Get profile storage path
  ///
  /*--acf()--*/
  virtual AcfString GetPath() = 0;

  ///
  /// Set the |value| associated with preference |name|.
  ///
  /*--acf(optional_param=value, optional_param=handler)--*/
  virtual void SetPreference(const AcfString& name,
                             AcfRefPtr<AcfValue> value,
                             AcfRefPtr<AcfCompleteHandler> handler) = 0;

  ///
  /// Remove browsing data when running profile,
  /// when completed it will call OnProfileRemoveDataCompleted
  ///
  /*--acf(optional_param=handler)--*/
  virtual void RemoveBrowsingData(uint64 data_type,
                                  AcfRefPtr<AcfCompleteHandler> handler) = 0;

  ///
  /// Get cookie manager for current profile in default storage.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfCookieManager> GetCookieManager() = 0;
};

///
/// Cookie data storage
///
/*--acf(source=library)--*/
class AcfCookie : public virtual AcfBaseRefCounted {
 public:
  typedef acf_cookie_same_site_t AcfCookieSameSite;
  typedef acf_cookie_priority_t AcfCookiePriority;

  ///
  /// Create a canonical cookie data.
  ///
  /*--acf(optional_param=url,optional_param=name,optional_param=value,optional_param=domain,optional_param=path)--*/
  static AcfRefPtr<AcfCookie> Create(const AcfString& url,
                                     const AcfString& name,
                                     const AcfString& value,
                                     const AcfString& domain,
                                     const AcfString& path,
                                     double expiry,
                                     bool secure,
                                     bool httponly,
                                     AcfCookieSameSite same_site,
                                     AcfCookiePriority priority);

  ///
  /// Get cookie name
  ///
  /*--acf()--*/
  virtual AcfString GetName() = 0;

  ///
  /// Get cookie value
  ///
  /*--acf()--*/
  virtual AcfString GetValue() = 0;

  ///
  /// Get cookie domain
  ///
  /*--acf()--*/
  virtual AcfString GetDomain() = 0;

  ///
  /// Get cookie path
  ///
  /*--acf()--*/
  virtual AcfString GetPath() = 0;

  ///
  /// Is cookie httponly
  ///
  /*--acf()--*/
  virtual bool IsHttponly() = 0;

  ///
  /// Is cookie secure
  ///
  /*--acf()--*/
  virtual bool IsSecure() = 0;

  ///
  /// Get same site infomation
  ///
  /*--acf(default_retval=ACF_COOKIE_SAME_SITE_NO_RESTRICTION)--*/
  virtual AcfCookieSameSite GetSameSite() = 0;

  ///
  /// Get priority
  ///
  /*--acf(default_retval=ACF_COOKIE_PRIORITY_MEDIUM)--*/
  virtual AcfCookiePriority GetPriority() = 0;

  ///
  /// Get create time
  ///
  /*--acf()--*/
  virtual double GetCreation() = 0;

  ///
  /// Get expiry time
  ///
  /*--acf()--*/
  virtual double GetExpiry() = 0;

  ///
  /// Get last access time
  ///
  /*--acf()--*/
  virtual double GetLastAccess() = 0;

  ///
  /// Get last update time
  ///
  /*--acf()--*/
  virtual double GetLastUpdate() = 0;
};

///
/// Cookie visitor
///
/*--acf(source=client)--*/
class AcfCookieVisitor : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Method that will be called once for each cookie. |count| is the 0-based
  /// index for the current cookie. |total| is the total number of cookies.
  /// Return false to stop visiting cookies. This method may never be called if
  /// no cookies are found.
  ///
  /*--acf(optional_param=cookie)--*/
  virtual bool Visit(AcfRefPtr<AcfCookie> cookie, int count, int total) = 0;
};

///
/// Class used for managing cookies. The methods of this class may be called on
/// any thread unless otherwise indicated.
///
/*--acf(source=library)--*/
class AcfCookieManager : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Get hosted profile object, null for frame associated cookie manager.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfProfile> GetProfile() = 0;

  ///
  /// Visit cookies on the current thread. The returned cookies are ordered by
  /// longest path, then by earliest creation date. Returns false if cookies
  /// cannot be accessed.
  ///
  /*--acf(optional_param=url)--*/
  virtual void GetCookies(const AcfString& url,
                          bool includeHttpOnly,
                          AcfRefPtr<AcfCookieVisitor> visitor) = 0;

  ///
  /// Sets a cookie given a valid URL and explicit user-provided cookie
  /// attributes. This function expects each attribute to be well-formed. It
  /// will check for disallowed characters (e.g. the ';' character is disallowed
  /// within the cookie value attribute) and fail without setting the cookie if
  /// such characters are found. If |callback| is non-NULL it will be executed
  /// asnychronously on the UI thread after the cookie has been set. Returns
  /// false if an invalid URL is specified or if cookies cannot be accessed.
  ///
  /*--acf(optional_param=url,optional_param=callback)--*/
  virtual void SetCookie(const AcfString& url,
                         AcfRefPtr<AcfCookie> cookie,
                         AcfRefPtr<AcfCompleteHandler> callback) = 0;

  ///
  /// Delete all cookies that match the specified parameters. If both |url| and
  /// |cookie_name| values are specified all host and domain cookies matching
  /// both will be deleted. If only |url| is specified all host cookies (but not
  /// domain cookies) irrespective of path will be deleted. If |url| is empty
  /// all cookies for all hosts and domains will be deleted. If |callback| is
  /// non-NULL it will be executed asnychronously on the UI thread after the
  /// cookies have been deleted. Returns false if a non-empty invalid URL is
  /// specified or if cookies cannot be accessed. Cookies can alternately be
  /// deleted using the Visit*Cookies() methods.
  ///
  /*--acf(optional_param=url,optional_param=cookie_name,optional_param=callback)--*/
  virtual void DeleteCookies(const AcfString& url,
                             const AcfString& cookie_name,
                             AcfRefPtr<AcfCompleteHandler> callback) = 0;

  ///
  /// Flush the backing store (if any) to disk. If |callback| is non-NULL it
  /// will be executed asnychronously on the UI thread after the flush is
  /// complete. Returns false if cookies cannot be accessed.
  ///
  /*--acf(optional_param=callback)--*/
  virtual void FlushStore(AcfRefPtr<AcfCompleteHandler> callback) = 0;
};

#endif  // !ACF_INCLUDE_ACF_PROFILE_H_
