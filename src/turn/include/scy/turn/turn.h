///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @defgroup turn TURN module (rfc5766)
///
/// The `turn` module contains TURN (rfc5766) client and server implementations.


#ifndef SCY_TURN_H
#define SCY_TURN_H



// Shared library exports
#if defined(SCY_WIN) && defined(SCY_SHARED_LIBRARY)
    #if defined(TURN_EXPORTS)
        #define TURN_API __declspec(dllexport)
    #else
        #define TURN_API __declspec(dllimport)
    #endif
#else
    #define TURN_API // nothing
#endif


#endif


/// @\}
