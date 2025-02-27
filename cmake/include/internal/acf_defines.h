// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_INTERNAL_ACF_DEFINES_H_
#define ACF_INCLUDE_INTERNAL_ACF_DEFINES_H_
#pragma once

// Windows only
#define ACF_EXPORT extern "C" __declspec(dllexport)
#define ACF_CALLBACK __stdcall

#include <limits.h>  // For UINT_MAX
#include <stddef.h>  // For size_t

// A set of macros to use for platform detection.
#if defined(ANDROID)
#define OS_ANDROID 1
#elif defined(__APPLE__)
// Only include TargetConditionals after testing ANDROID as some Android builds
// on the Mac have this header available and it's not needed unless the target
// is really an Apple platform.
#include <TargetConditionals.h>
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define OS_IOS 1
#else
#define OS_MAC 1
// For backwards compatibility.
#define OS_MACOSX 1
#endif  // defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#elif defined(__linux__)
#if !defined(OS_CHROMEOS)
// Do not define OS_LINUX on Chrome OS build.
// The OS_CHROMEOS macro is defined in GN.
#define OS_LINUX 1
#endif  // !defined(OS_CHROMEOS)
// Include a system header to pull in features.h for glibc/uclibc macros.
#include <unistd.h>
#if defined(__GLIBC__) && !defined(__UCLIBC__)
// We really are using glibc, not uClibc pretending to be glibc.
#define LIBC_GLIBC 1
#endif
#elif defined(_WIN32)
#define OS_WIN 1
#elif defined(__Fuchsia__)
#define OS_FUCHSIA 1
#elif defined(__FreeBSD__)
#define OS_FREEBSD 1
#elif defined(__NetBSD__)
#define OS_NETBSD 1
#elif defined(__OpenBSD__)
#define OS_OPENBSD 1
#elif defined(__sun)
#define OS_SOLARIS 1
#elif defined(__QNXNTO__)
#define OS_QNX 1
#elif defined(_AIX)
#define OS_AIX 1
#elif defined(__asmjs__) || defined(__wasm__)
#define OS_ASMJS 1
#else
#error Please add support for your platform in include/internal/acf_def.h
#endif

// The NSPR system headers define 64-bit as |long| when possible, except on
// Mac OS X.  In order to not have typedef mismatches, we do the same on LP64.
//
// On Mac OS X, |long long| is used for 64-bit types for compatibility with
// <inttypes.h> format macros even in the LP64 model.
#if defined(__LP64__) && !defined(OS_MAC) && !defined(OS_OPENBSD)
typedef long int64;
typedef unsigned long uint64;
#else
typedef long long int64;
typedef unsigned long long uint64;
#endif

// TODO: Remove these type guards.  These are to avoid conflicts with
// obsolete/protypes.h in the Gecko SDK.
#ifndef _INT32
#define _INT32
typedef int int32;
#endif

// TODO: Remove these type guards.  These are to avoid conflicts with
// obsolete/protypes.h in the Gecko SDK.
#ifndef _UINT32
#define _UINT32
typedef unsigned int uint32;
#endif

#ifndef _INT16
#define _INT16
typedef short int16;
#endif

#ifndef _UINT16
#define _UINT16
typedef unsigned short uint16;
#endif

// Type detection for wchar_t.
#if defined(OS_WIN)
#define WCHAR_T_IS_UTF16
#elif defined(OS_FUCHSIA)
#define WCHAR_T_IS_UTF32
#elif defined(OS_POSIX) && defined(COMPILER_GCC) && defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
#define WCHAR_T_IS_UTF32
#elif defined(OS_POSIX) && defined(COMPILER_GCC) && defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0xffff)
// On Posix, we'll detect short wchar_t, but projects aren't guaranteed to
// compile in this mode (in particular, Chrome doesn't). This is intended for
// other projects using base who manage their own dependencies and make sure
// short wchar works for them.
#define WCHAR_T_IS_UTF16
#else
#error Please add support for your compiler in include/internal/acf_defines.h
#endif

// UTF-16 character type.
#ifndef char16
#if defined(WCHAR_T_IS_UTF16)
typedef wchar_t char16;
#elif defined(WCHAR_T_IS_UTF32)
typedef unsigned short char16;
#endif
#endif

// builtin wrapper
#define acf_window_handle_t HWND
#define acf_user_data_t void*

#define AcfWindowHandle acf_window_handle_t
#define AcfUserData acf_user_data_t

#endif  //! ACF_INCLUDE_INTERNAL_ACF_DEFINES_H_
