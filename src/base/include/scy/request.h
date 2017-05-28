///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup uv
/// @{


#ifndef SCY_UV_Request_H
#define SCY_UV_Request_H


#include "scy/base.h"
#include "scy/loop.h"

#include "uv.h"

#include <functional>


namespace scy {
namespace uv {


template <typename T>
class Base_API Handle;


/// Default request callback event.
struct BasicEvent
{
    int status {0};
};


/// Wrapper class for managing `uv_req_t` variants.
///
/// This class povides safe access to the parent handle incase the handle gets
/// destroyed before the request callback returns, and should be used whenever
/// the handle pointer is accessed via the callback.
template<typename T, typename E = BasicEvent>
struct Request
{
    typedef T Type;

    T req;
    std::function<void(const E&)> callback;

    Request()
    {
        req.data = this;
    }

    static void defaultCallback(T* req, int status)
    {
        auto wrap = static_cast<Request*>(req->data);
        if (wrap->callback)
            wrap->callback(E{status});
        delete wrap;
    }

    template<typename F, typename... Args>
    auto invoke(F&& f, Args&&... args)
    -> std::enable_if_t<not std::is_void<std::result_of_t<F(Args...)>>::value, int>
    {
        auto err = std::forward<F>(f)(std::forward<Args>(args)...);
        if (err && callback) callback(E{err});
        return !err;
    }

    template<typename F, typename... Args>
    auto invoke(F&& f, Args&&... args)
    -> std::enable_if_t<std::is_void<std::result_of_t<F(Args...)>>::value>
    {
        std::forward<F>(f)(std::forward<Args>(args)...);
    }
};


} // namespace uv
} // namespace scy


#endif // SCY_UV_Request


/// @\}
