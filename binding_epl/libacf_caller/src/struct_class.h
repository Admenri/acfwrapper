#ifndef ACF_STRUCT_WRAPPER_CLASS_H_
#define ACF_STRUCT_WRAPPER_CLASS_H_
#pragma once

#include "wrapper_types.h"

typedef struct _eclass_vfptr {
  DWORD dwVfTable;
  LPVOID pObject;
  LPVOID pCFuncs;
} ECLASS_VFPTR, *PECLASS_VFPTR;

enum class EClassVTable {
  VT_ENVIRONMENT = 0,
  VT_BROWSER,
  VT_PROFILE,
  VT_NEWWINDOWDELEGATE,
  VT_VALUE,
  VT_BINARY,
  VT_DICTIONARY,
  VT_LIST,
  VT_COOKIEMANAGER,
  VT_FRAME,
  VT_AUTHCALLBACK,
  VT_MENUCALLBACK,
  VT_MENUPARAMS,
  VT_MENUMODEL,
  VT_REQUEST,
  VT_RESPONSE,
  VT_POSTDATA,
  VT_POSTDATAELEMENTS,
  VT_RESPONSECALLBACK,
  VT_RESPONSEFILTER,
  VT_WEBMESSAGEPROXY,
  VT_URLREQUEST,
  VT_PERMISSIONPROMPT,
  VT_JSDIALOGCALLBACK,
  VT_FILEDIALOGCAALLBACK,
  VT_WEBSOCKETCLIENT,
  VT_PROMISE,
};

DLL_EXPORTS(RegisterClass, void)(DWORD** pNewClass, EClassVTable nType);

namespace eClass {

void Copy(void);

LPVOID __stdcall Alloc(int nSize);
void __stdcall memcpy(void* dest, void* src, int size);

LPVOID SetFunctionHookAddr(LPVOID lpSrcAddr, LPVOID lpHookAddr);

// Module Part
extern DWORD m_pVfTable_Environment;
extern DWORD m_pVfTable_Browser;
extern DWORD m_pVfTable_Profile;
extern DWORD m_pVfTable_NewWindowDelegate;
extern DWORD m_pVfTable_Value;
extern DWORD m_pVfTable_Binary;
extern DWORD m_pVfTable_Dictionary;
extern DWORD m_pVfTable_List;
extern DWORD m_pVfTable_CookieManager;
extern DWORD m_pVfTable_Frame;
extern DWORD m_pVfTable_AuthCallback;
extern DWORD m_pVfTable_MenuCallback;
extern DWORD m_pVfTable_MenuParams;
extern DWORD m_pVfTable_MenuModel;
extern DWORD m_pVfTable_Request;
extern DWORD m_pVfTable_Response;
extern DWORD m_pVfTable_PostData;
extern DWORD m_pVfTable_PostDataElements;
extern DWORD m_pVfTable_ResponseCallback;
extern DWORD m_pVfTable_ResponseFilter;
extern DWORD m_pVfTable_WebSocketClient;
extern DWORD m_pVfTable_WebMessageReplyProxy;
extern DWORD m_pVfTable_UrlRequest;
extern DWORD m_pVfTable_PermissionPrompt;
extern DWORD m_pVfTable_JSDialogCallback;
extern DWORD m_pVfTable_RepostCallback;
extern DWORD m_pVfTable_FileDialogCallback;
extern DWORD m_pVfTable_Promise;

}  // namespace eClass

// EPL Class Struct
#define IMP_NEWECLASS(LocalName, Object, Vfptr, funcs)    \
  ECLASS_VFPTR LocalName##T, *LocalName##TT, **LocalName; \
  LocalName##T.dwVfTable = Vfptr;                         \
  LocalName##T.pObject = (LPVOID)Object;                  \
  LocalName##T.pCFuncs = (LPVOID)funcs;                   \
  LocalName##TT = &LocalName##T;                          \
  LocalName = &LocalName##TT;

#endif