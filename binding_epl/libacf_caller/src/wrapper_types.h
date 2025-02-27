#ifndef ACF_E_TYPES_H_
#define ACF_E_TYPES_H_

#include <wtypes.h>

#define DLL_EXPORTS(name, type) \
  extern "C" __declspec(dllexport) type __cdecl acf_##name

#define ISVALID(v) \
  if (!v) return;
#define ISVALIDR(v, r) \
  if (!v) return r;

typedef struct _cookie_t {
  LPCSTR name;
  LPCSTR value;
  LPCSTR domain;
  LPCSTR path;
  BOOL secure;
  BOOL httponly;
  double creation;
  double expires;
  double last_access;
  double last_update;
  int same_site;
  int priority;
} ACF_COOKIE, *PACF_COOKIE;

typedef struct _string_pair_t {
  LPCSTR key;
  LPCSTR value;
} ACF_STRING_PAIR, *PACF_STRING_PAIR;

#endif
