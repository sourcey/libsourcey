///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @defgroup base Base module
///
/// The `base` module contains reusable cross platform tools and utilities.


#ifndef SCY_H
#define SCY_H


//
/// Version number
//

#define SCY_MAJOR_VERSION    0
#define SCY_MINOR_VERSION    9
#define SCY_PATCH_VERSION    5

#define SCY_AUX_STR_EXP(__A) #__A
#define SCY_AUX_STR(__A)     SCY_AUX_STR_EXP(__A)
#define SCY_VERSION          SCY_AUX_STR(SCY_MAJOR_VERSION) "." SCY_AUX_STR(SCY_MINOR_VERSION) "." SCY_AUX_STR(SCY_PATCH_VERSION)


// Include build config
#include "libsourcey.h"


//
/// Platform and compiler definitions
//

#ifdef _WIN32   /// Windows (x64 and x86)
    #define SCY_WIN
#endif
#if __unix__    /// Unix
    #define SCY_UNIX
#endif
#if __posix__    /// POSIX
    #define SCY_POSIX
#endif
#if __linux__    /// Linux
    #define SCY_LINUX
#endif
#if __APPLE__    /// Mac OS
    #define SCY_APPLE
#endif
#if __GNUC__    /// GCC compiler
    #define SCY_GNUC
#endif
#if defined(__MINGW32__) || defined(__MINGW64__)    /// MinGW
    #define SCY_MINGW
#endif


#ifdef SCY_WIN
# ifndef SCY_SHARED_LIBRARY // libsourcey.h
#   define SCY_EXTERN __declspec(dllexport)
# else
#   define SCY_EXTERN __declspec(dllimport)
# endif
#else
# define SCY_EXTERN // nothing
#endif


//
/// Windows specific
//

#ifdef SCY_WIN

// Verify that we're built with the multithreaded
// versions of the runtime libraries
#if defined(_MSC_VER) && !defined(_MT)
    #error Must compile with /MD, /MDd, /MT or /MTd
#endif


// Check debug/release settings consistency
#if defined(NDEBUG) && defined(_DEBUG)
    #error Inconsistent build settings (check for /MD[d])
#endif


// Unicode Support
#if defined(UNICODE)
    #define SCY_UNICODE
#endif


// Disable unnecessary warnings
#if defined(_MSC_VER)
    #pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
    #pragma warning(disable:4251) // ... needs to have dll-interface warning
    #pragma warning(disable:4355) // 'this' : used in base member initializer list
    #pragma warning(disable:4996) // VC++ 8.0 deprecation warnings
    #pragma warning(disable:4351) // new behavior: elements of array '...' will be default initialized
    #pragma warning(disable:4675) // resolved overload was found by argument-dependent lookup
    #pragma warning(disable:4275) // non dll-interface class 'std::exception' used as base for dll-interface class 'scy::Exception'
#endif

#endif // WIN32


#endif // SCY_H
