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


#ifndef SCY_UV_Handle_H
#define SCY_UV_Handle_H


#include "scy/uv/uvpp.h"


namespace scy {
namespace uv {


/// A base class for managing a `libuv` handle during it's lifecycle and
/// safely handling its asynchronous destruction mechanism.
class UV_API Handle
{
public:
    Handle(uv::Loop* loop = nullptr, void* handle = nullptr);
    virtual ~Handle();

    /// The event loop may be set before the handle is initialized.
    virtual void setLoop(uv::Loop* loop);

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
    virtual uv::Loop* loop() const;

    /// Returns true when the handle is active.
    /// This method should be used instead of closed() to determine
    /// the veracity of the `libuv` handle for stream operations.
    virtual bool active() const;

    /// Returns true after close() has been called.
    virtual bool closed() const;

    /// Returns a typecasted pointer to the managed `libuv` handle.
    template <class T> T* ptr() const
    {
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

    uv::Loop* _loop;
    uv_handle_t* _ptr;
    scy::Error _error;
    std::thread::id _tid;
    bool _closed;
};


} // namespace uv
} // namespace scy


#endif // SCY_UV_Handle_H


/// @\}
