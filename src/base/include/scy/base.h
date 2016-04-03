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
#define SCY_PATCH_VERSION    4

#define SCY_AUX_STR_EXP(__A) #__A
#define SCY_AUX_STR(__A)     SCY_AUX_STR_EXP(__A)
#define SCY_VERSION          SCY_AUX_STR(SCY_MAJOR_VERSION) "." SCY_AUX_STR(SCY_MINOR_VERSION) "." SCY_AUX_STR(SCY_PATCH_VERSION)


// Include build config
#include "scyconfig.h"


//
/// Platform and compiler definitions
//

#ifdef _WIN32
    // Windows (x64 and x86)
    #ifndef WIN32
    #define WIN32
    #endif
#endif
#if __unix__
    // Unix
    #ifndef UNIX
    #define UNIX
    #endif
#endif
#if __posix__
    // POSIX
    #ifndef POSIX
    #define POSIX
    #endif
#endif
#if __linux__
    // Linux
    #ifndef LINUX
    #define LINUX
    #endif
#endif
#if __APPLE__
    // Mac OS
    #ifndef APPLE
    #define APPLE
    #endif
#endif
#if __GNUC__
    // GCC compiler
    #ifndef GNUC
    #define GNUC
    #endif
#endif
#if defined(__MINGW32__) || defined(__MINGW64__)
    // MinGW
    #ifndef MINGW
    #define MINGW
    #endif
#endif


#ifdef WIN32
# ifndef SCY_SHARED_LIBRARY // scyconfig.h
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

#ifdef WIN32

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
