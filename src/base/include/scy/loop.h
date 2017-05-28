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

#include "uv.h"


namespace scy {
namespace uv {


typedef uv_loop_t Loop;


inline Loop* defaultLoop()
{
    return uv_default_loop();
}

inline void runLoop(Loop* loop = defaultLoop(), uv_run_mode mode = UV_RUN_DEFAULT)
{
    uv_run(loop, mode);
}

inline void stopLoop(Loop* loop = defaultLoop())
{
    uv_stop(loop);
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
