//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


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

#ifdef _WIN32
    // Windows (x64 and x86)
    #ifndef SCY_WIN32
    #define SCY_WIN32
    #endif
#endif
#if __unix__
    // Unix
    #ifndef SCY_UNIX
    #define SCY_UNIX
    #endif
#endif
#if __posix__
    // POSIX
    #ifndef SCY_POSIX
    #define SCY_POSIX
    #endif
#endif
#if __linux__
    // Linux
    #ifndef SCY_LINUX
    #define SCY_LINUX
    #endif
#endif
#if __APPLE__
    // Mac OS
    #ifndef SCY_APPLE
    #define SCY_APPLE
    #endif
#endif
#if __GNUC__
    // GCC compiler
    #ifndef SCY_GNUC
    #define SCY_GNUC
    #endif
#endif
#if defined(__MINGW32__) || defined(__MINGW64__)
    // MinGW
    #ifndef SCY_MINGW
    #define SCY_MINGW
    #endif
#endif


#ifdef SCY_WIN32
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

#ifdef SCY_WIN32

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
