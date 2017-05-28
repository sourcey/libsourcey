///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Once_H
#define SCY_Once_H


#include "scy/base.h"
#include "scy/runner.h"
#include "scy/loop.h"


namespace scy {


/// Run the given function at the beginning of the next event loop iteration.
template<typename Function, typename... Args>
void runOnce(uv::Loop* loop, Function&& func, Args&&... args)
{
    typedef internal::DeferredCallable<Function, Args...> Callback;

    auto prepare = new uv_prepare_t;
    prepare->data = new Callback(nullptr,
                                 std::forward<Function>(func),
                                 std::forward<Args>(args)...);

    uv_prepare_init(loop, prepare);
    uv_prepare_start(prepare, [](uv_prepare_t *req) {
        auto wrap = reinterpret_cast<Callback*>(req->data);
        wrap->invoke();
        delete wrap;
        uv_prepare_stop(req);
        delete req;
    });
}


} // namespace scy


#endif // SCY_Once_H


/// @\}
