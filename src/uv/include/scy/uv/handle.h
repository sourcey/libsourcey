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


#ifndef SCY_UV_Handle_H
#define SCY_UV_Handle_H


#include "scy/uv/uvpp.h"
#include "scy/uv/loop.h"
#include "scy/uv/util.h"


namespace scy {
namespace uv {


/// Base class for managing a `libuv` handle during it's lifecycle and
/// safely handling its asynchronous destruction mechanism.
class UV_API Handle
{
public:
    Handle(uv::Loop* loop = nullptr, void* handle = nullptr);
    virtual ~Handle();

    /// Initialize the handle.
    ///
    /// This method should be overridden by the implementation to call `uv_init`
    /// on the handle.
    virtual void init();

    /// Close and destroy the associated `libuv` handle.
    virtual void close();

    /// Reference main loop again, once unref'd.
    bool ref();

    /// Unreference the main loop after initialized.
    bool unref();

    /// Return true if the handle has been initialized.
    bool initialized();

    /// Return true when the handle is active.
    /// This method should be used instead of closed() to determine
    /// the veracity of the `libuv` handle for stream operations.
    virtual bool active() const;

    /// Return true if the handle is closing.
    virtual bool closing() const;

    /// Return true after close() has been called.
    virtual bool closed() const;

    /// Return the error context if any.
    const scy::Error& error() const;

    /// Set the error and triggers callbacks.
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

    /// Set the event loop.
    /// The event loop may be set before the handle is initialized.
    virtual void setLoop(uv::Loop* loop);

    /// Return a cast pointer to the managed `libuv` handle.
    virtual uv::Loop* loop() const;

    /// Create the internal handle.
    template <class T> T* create()
    {
        assert(!_ptr);
        assert(!_initialized);
        T* handle = new T;
        _ptr = reinterpret_cast<uv_handle_t*>(handle);
        _ptr->data = this;
        _closed = false;
        _error.reset();
        return handle;
    }

    /// Reset the internal handle pointer and container state.
    template <class T> T* reset()
    {
        close();
        return create<T>();
    }

    /// Return a typecasted pointer to the managed `libuv` handle.
    template <class T> T* ptr() const
    {
        return reinterpret_cast<T*>(_ptr);
    }

    /// Return a pointer to the managed `libuv` handle.
    virtual uv_handle_t* ptr() const;

    /// Return the parent thread ID.
    std::thread::id tid() const;

    /// Make sure we are calling from the event loop thread.
    void assertThread() const;

protected:

    /// Invoke an internal `libuv` method.
    template<typename F, typename... Args>
    bool invoke(F&& f, Args&&... args)
    {
        int err = std::forward<F>(f)(std::forward<Args>(args)...);
        if (err)
            setUVError("UV Error", err);
        return !err;
    }

    /// Error callback.
    /// Override to handle errors.
    /// The error may be a UV error, or a custom error.
    virtual void onError(const scy::Error& error);

    /// Close callback.
    /// Override to handle closure.
    virtual void onClose();

protected:
    /// NonCopyable and NonMovable
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    uv::Loop* _loop;
    uv_handle_t* _ptr;
    scy::Error _error;
    std::thread::id _tid;
    bool _initialized;
    bool _closed;
};


} // namespace uv
} // namespace scy


#endif // SCY_UV_Handle_H


/// @\}
