///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
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


// Disable unnecessary warnings
#if defined(_MSC_VER)
#pragma warning(disable : 4201) // nonstandard extension used : nameless
                                // struct/union
#pragma warning(disable : 4505) // unreferenced local function has been removed
                                // Todo: depreciate once we replace static
                                // functions with lambdas
#endif


#include "scy/base.h"
#include "scy/error.h"

#include "uv.h"

#include <functional>
#include <thread>
#include <assert.h>
#include <cstdint>


namespace scy {
namespace uv {


//
// Helpers
//


inline std::string formatError(const std::string& message, int errorno = 0)
{
    std::string m(message); // prefix the message since libuv errors are brisk
    if (errorno != UV_UNKNOWN && errorno != 0) {
        // err.code = (uv_err_code)errorno;
        // uv_err_s err;

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


//
/// UV Handle
//


/// A base class for managing a `libuv` handle during it's lifecycle and
/// safely handling its asynchronous destruction mechanism.
class SCY_EXTERN Handle
{
public:
    Handle(uv_loop_t* loop = nullptr, void* handle = nullptr);
    virtual ~Handle();

    /// The event loop may be set before the handle is initialized.
    virtual void setLoop(uv_loop_t* loop);

    /// Closes and destroys the associated `libuv` handle.
    virtual void close();

    /// Returns true if the handle has been initialized.
    bool initialized();

    /// Reference main loop again, once unref'd.
    bool ref();

    /// Unreference the main loop after initialized.
    bool unref();

    /// Returns the error context if any.
    const scy::Error& error() const;

    /// Sets the error content and triggers callbacks.
    virtual void setError(const scy::Error& err);

    /// Sets and throws the last error.
    /// Should never be called inside `libuv` callbacks.
    virtual void setAndThrowError(const std::string& prefix = "UV Error", int errorno = 0);

    /// Throws the last error.
    /// This function is const so it can be used for
    /// invalid getter operations on closed handles.
    /// The actual error would be set on the next iteraton.
    virtual void throwError(const std::string& prefix = "UV Error", int errorno = 0) const;

    /// Sets the last error and sends relevant callbacks.
    /// This method can be called inside `libuv` callbacks.
    virtual void setUVError(const std::string& prefix = "UV Error", int errorno = 0);

    /// Returns the parent thread ID.
    std::thread::id tid() const;

    /// Returns a cast pointer to the managed `libuv` handle.
    virtual uv_loop_t* loop() const;

    /// Returns true when the handle is active.
    /// This method should be used instead of closed() to determine
    /// the veracity of the `libuv` handle for stream operations.
    virtual bool active() const;

    /// Returns true after close() has been called.
    virtual bool closed() const;

    /// Returns a typecasted pointer to the managed `libuv` handle.
    template <class T> T* ptr() const
    {
        assertThread(); // conflict with uv_async_send in Synchronizer
        return reinterpret_cast<T*>(_ptr);
    }

    /// Returns a pointer to the managed `libuv` handle.
    virtual uv_handle_t* ptr() const;

    /// Make sure we are calling from the event loop thread.
    void assertThread() const;

protected:
    /// Override to handle errors.
    /// The error may be a UV error, or a custom error.
    virtual void onError(const scy::Error& error);

    /// Override to handle closure.
    virtual void onClose();

protected:
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    uv_loop_t* _loop;
    uv_handle_t* _ptr;
    scy::Error _error;
    std::thread::id _tid;
    bool _closed;
};


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
// Macro Helpers
//


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


#endif // SCY_UV_UVPP_H


/// @\}
