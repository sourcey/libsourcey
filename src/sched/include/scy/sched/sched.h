///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup sched
/// @{


#ifndef SCY_Sched_h
#define SCY_Sched_h


#include "scy/base.h"


// Shared library exports
#if defined(SCY_WIN) && defined(SCY_SHARED_LIBRARY)
    #if defined(Sched_EXPORTS)
        #define Sched_API __declspec(dllexport)
    #else
        #define Sched_API __declspec(dllimport)
    #endif
#else
    #define Sched_API // nothing
#endif


#endif // SCY_Sched_H


/// @\}
