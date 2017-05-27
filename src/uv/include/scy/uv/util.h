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


#ifndef SCY_UV_UTIL_H
#define SCY_UV_UTIL_H


#include "scy/base.h"

#include "scy/uv/uvpp.h"

#include <functional>
#include <thread>
#include <assert.h>
#include <cstdint>


namespace scy {
namespace uv {


//
// Helpers
//


inline std::string formatError(const std::string& message, int errorno = UV_UNKNOWN)
{
    std::string m(message); // prefix the message since libuv errors are brisk
    if (errorno != UV_UNKNOWN && errorno != 0) {
        if (!m.empty())
            m.append(": ");
        m.append(uv_strerror(errorno));
    }
    return m;
}


inline void throwError(const std::string& message, int errorno = UV_UNKNOWN)
{
    throw std::runtime_error(formatError(message, errorno));
}


//
// Shutdown Signal Handler
//


struct ShutdownCmd
{
    void* opaque;
    std::function<void(void*)> callback;
};

inline void onShutdownSignal(std::function<void(void*)> callback = nullptr,
                             void* opaque = nullptr, Loop* loop = defaultLoop())
{
    auto cmd = new ShutdownCmd;
    cmd->opaque = opaque;
    cmd->callback = callback;

    auto sig = new uv_signal_t;
    sig->data = cmd;
    uv_signal_init(loop, sig);
    uv_signal_start(sig, [](uv_signal_t* req, int /* signum */) {
        auto cmd = reinterpret_cast<ShutdownCmd*>(req->data);
        uv_close((uv_handle_t*)req, [](uv_handle_t* handle) { delete handle; });
        if (cmd->callback)
            cmd->callback(cmd->opaque);
        delete cmd;
    }, SIGINT);
}

inline void waitForShutdown(std::function<void(void*)> callback = nullptr,
                            void* opaque = nullptr, Loop* loop = defaultLoop())
{
    onShutdownSignal(callback, opaque, loop);
    uv_run(loop, UV_RUN_DEFAULT);
}


//
// Helpers
//


/// Call a function with the given argument tuple.
///
/// Note: This will become redundant once C++17 `std::apply` is fully supported.
template<typename Function, typename Tuple, size_t ... I>
auto invoke(Function f, Tuple t, std::index_sequence<I ...>)
{
     return f(std::get<I>(t)...);
}


/// Call a function with the given argument tuple.
///
/// Create an index sequence for the array, and pass it to the
/// implementation `call` function.
///
/// Note: This will become redundant once C++17 `std::apply` is fully supported.
template<typename Function, typename Tuple>
auto invoke(Function f, Tuple t)
{
    static constexpr auto size = std::tuple_size<Tuple>::value;
    return invoke(f, t, std::make_index_sequence<size>{});
}


/// Invoke a `libuv` method on the handle.
template<typename F, typename... Args>
auto invoke(F&& f, Args&&... args)
{
    return std::forward<F>(f)(std::forward<Args>(args)...);
}


//
// Macros
//


/// @deprecated
#define UVCallOrThrow(message, func, ...)                                      \
    {                                                                          \
       int err = func(__VA_ARGS__);                                            \
       if (err)                                                                \
           setAndThrowError(message, err);                                     \
    }


/// @deprecated
#define UVCallback(ClassName, Function, Handle)                                \
                                                                               \
    static void _Function(Handle* handle)                                      \
    {                                                                          \
        static_cast<ClassName*>(handle->data)->Function();                     \
    };


/// @deprecated
#define UVStatusCallback(ClassName, Function, Handle)                          \
                                                                               \
    static void Function(Handle* handle, int status)                           \
    {                                                                          \
        ClassName* self = static_cast<ClassName*>(handle->data);               \
        self->Function(status);                                                \
    }


/// @deprecated
#define UVEmptyStatusCallback(ClassName, Function, Handle)                     \
                                                                               \
    static void Function(Handle* handle, int status)                           \
    {                                                                          \
        ClassName* self = static_cast<ClassName*>(handle->data);               \
        if (status)                                                            \
            self->setUVError("UV error", status);                              \
        self->Function();                                                      \
    }


/// @deprecated
#define UVStatusCallbackWithType(ClassName, Function, Handle)                  \
                                                                               \
    static void Function(Handle* handle, int status)                           \
    {                                                                          \
        ClassName* self = static_cast<ClassName*>(handle->data);               \
        self->Function(handle, status);                                        \
    }


} // namespace uv
} // namespace scy


#endif // SCY_UV_UTIL_H


/// @\}
