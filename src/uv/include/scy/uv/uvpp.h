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


#ifndef SCY_UV_UVPP_H
#define SCY_UV_UVPP_H


// Disable unnecessary warnings
#if defined(_MSC_VER)
    #pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
    #pragma warning(disable:4505) // unreferenced local function has been removed
                                  // Todo: depreciate once we replace static functions with lambdas
#endif

#include "uv.h"
#include "scy/error.h"
#include <cstdint>
#include <functional>
#include <assert.h>


namespace scy {
namespace uv {


//
// Helpers
//


inline std::string formatError(const std::string& message, int errorno = 0)
{
    std::string m(message); // prefix the message, since libuv errors are very brisk
    if (errorno != UV_UNKNOWN &&
        errorno != 0) {
        //uv_err_s err;
        //err.code = (uv_err_code)errorno;
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


//
// UV Handle
//


class Handle
    /// A base class for managing a libuv handle during it's lifecycle and
    /// safely handling its asynchronous destruction mechanism.
{
public:
    Handle(uv_loop_t* loop = nullptr, void* handle = nullptr);
    virtual ~Handle();

    virtual void setLoop(uv_loop_t* loop);
        // The event loop may be set before the handle is initialized.

    virtual uv_loop_t* loop() const;

    template <class T>
    T* ptr() const
        // Returns a cast pointer to the managed libuv handle.
    {
        assertThread(); // conflict with uv_async_send in SyncContext
        return reinterpret_cast<T*>(_ptr);
    }

    virtual uv_handle_t* ptr() const;
        // Returns a pointer to the managed libuv handle.

    virtual bool active() const;
        // Returns true when the handle is active.
        // This method should be used instead of closed() to determine
        // the veracity of the libuv handle for stream io operations.

    virtual bool closed() const;
        // Returns true after close() has been called.

    bool ref();
        // Reference main loop again, once unref'd.

    bool unref();
        // Unreference the main loop after initialized.

    uv_thread_t tid() const;
        // Returns the parent thread ID.

    const scy::Error& error() const;
        // Returns the error context if any.]

    virtual void setAndThrowError(const std::string& prefix = "UV Error", int errorno = 0);
        // Sets and throws the last error.
        // Should never be called inside libuv callbacks.

    virtual void throwError(const std::string& prefix = "UV Error", int errorno = 0) const;
        // Throws the last error.
        // This function is const so it can be used for
        // invalid getter operations on closed handles.
        // The actual error would be set on the next iteraton.

    virtual void setUVError(const std::string& prefix = "UV Error", int errorno = 0);
        // Sets the last error and sends relevant callbacks.
        // This method can be called inside libuv callbacks.

    virtual void setError(const scy::Error& err);
        // Sets the error content and triggers callbacks.

    virtual void close();
        // Closes and destroys the associated libuv handle.

    void assertThread() const;
        // Make sure we are calling from the event loop thread.

protected:
    virtual void onError(const scy::Error& /* error */);
        // Override to handle errors.
        // The error may be a UV error, or a custom error.

    virtual void onClose();
        // Override to handle closure.

 protected:
    Handle(const Handle&); // = delete;
    Handle& operator=(const Handle&); // = delete;

    uv_loop_t* _loop;
    uv_handle_t* _ptr;
    scy::Error _error;
    uv_thread_t _tid;
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

inline void onShutdownSignal(std::function<void(void*)> callback, void* opaque = nullptr, Loop* loop = defaultLoop())
{
    auto cmd = new ShutdownCmd;
    cmd->opaque = opaque;
    cmd->callback = callback;

    auto sig = new uv_signal_t;
    sig->data = cmd;
    uv_signal_init(loop, sig);
    uv_signal_start(sig, [](uv_signal_t* req, int /* signum */) {
        auto cmd = reinterpret_cast<ShutdownCmd*>(req->data);
        uv_close((uv_handle_t*)req, [](uv_handle_t* handle) {
            delete handle;
        });
        if (cmd->callback)
            cmd->callback(cmd->opaque);
        delete cmd;
    }, SIGINT);
}

inline void waitForShutdown(std::function<void(void*)> callback, void* opaque = nullptr, Loop* loop = defaultLoop())
{
    onShutdownSignal(callback, opaque, loop);
    uv_run(defaultLoop(), UV_RUN_DEFAULT);
}


//
// Default Callbacks (Depreciated)
//


#define UVCallback(ClassName, Function, Handle)                      \
                                                                     \
    static void _Function(Handle* handle) {                          \
        static_cast<ClassName*>(handle->data)->Function();           \
    };                                                               \


#define UVStatusCallback(ClassName, Function, Handle)                \
                                                                     \
    static void Function(Handle* handle, int status) {               \
        ClassName* self = static_cast<ClassName*>(handle->data);     \
        self->Function(status);                                      \
    }                                                                \


#define UVEmptyStatusCallback(ClassName, Function, Handle)           \
                                                                     \
    static void Function(Handle* handle, int status) {               \
        ClassName* self = static_cast<ClassName*>(handle->data);     \
        if (status)                                                  \
            self->setUVError("UV error", status);                    \
        self->Function();                                            \
    }                                                                \


#define UVStatusCallbackWithType(ClassName, Function, Handle)        \
                                                                     \
    static void Function(Handle* handle, int status) {               \
        ClassName* self = static_cast<ClassName*>(handle->data);     \
        self->Function(handle, status);                              \
    }                                                                \


} } // namespace scy::uv


#endif // SCY_UV_UVPP_H
