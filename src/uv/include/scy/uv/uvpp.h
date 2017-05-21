///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @defgroup uv UV module
///
/// The `uv` module contains C++ wrappers for `libuv`.
///
/// @addtogroup uv
/// @{


#ifndef SCY_UV_UVPP_H
#define SCY_UV_UVPP_H

#include "scy/base.h"
#include "scy/error.h"

#include "uv.h"


// Shared library exports
#if defined(SCY_WIN) && defined(SCY_SHARED_LIBRARY)
    #if defined(UV_EXPORTS)
        #define UV_API __declspec(dllexport)
    #else
        #define UV_API __declspec(dllimport)
    #endif
#else
    #define UV_API // nothing
#endif


// Disable unnecessary warnings
#if defined(_MSC_VER)
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
//#pragma warning(disable : 4505) // unreferenced local function has been removed
//                                // Todo: depreciate once we replace static
//                                // functions with lambdas
#endif


#endif // SCY_UV_UVPP_H


/// @\}
