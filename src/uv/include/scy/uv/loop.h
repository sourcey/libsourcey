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


#ifndef SCY_UV_LOOP_H
#define SCY_UV_LOOP_H


#include "scy/base.h"

#include "scy/uv/uvpp.h"


namespace scy {
namespace uv {


//
// Default Event Loop
//


typedef uv_loop_t Loop;

static uv_thread_t mainThread = 0;

inline Loop* defaultLoop()
{
    // Capture the main TID the first time
    // uv_default_loop is accessed.
    if (mainThread == 0)
        mainThread = uv_thread_self();
    return uv_default_loop();
}

inline void runDefaultLoop(uv_run_mode mode = UV_RUN_DEFAULT)
{
    uv_run(defaultLoop(), mode);
}

inline void stopDefaultLoop()
{
    uv_stop(defaultLoop());
}

inline Loop* createLoop()
{
    auto loop = new uv_loop_t;
    uv_loop_init(loop);
    return loop;
}

inline bool closeLoop(Loop* loop)
{
    return uv_loop_close(loop) == 0;
}


} // namespace uv
} // namespace scy


#endif // SCY_UV_LOOP_H


/// @\}
