///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup uv
/// @{


#include "scy/uv/handle.h"
#include <iostream>


namespace scy {
namespace uv {


Handle::Handle(uv::Loop* loop, void* handle)
    : _loop(loop ? loop : uv::defaultLoop()) // nullptr will be uv_default_loop
    , _ptr((uv_handle_t*)handle) // nullptr or instance of uv_handle_t
    , _tid(std::this_thread::get_id())
    , _closed(false)
{
    if (_ptr)
        _ptr->data = this;
}


Handle::~Handle()
{
    assertThread();
    if (!_closed)
        close();
    if (_ptr)
        delete _ptr;
}


void Handle::setLoop(uv::Loop* loop)
{
    assertThread();
    assert(_ptr == nullptr && "loop must be set before handle");
    _loop = loop;
}


uv::Loop* Handle::loop() const
{
    assertThread();
    return _loop;
}


uv_handle_t* Handle::ptr() const
{
    assertThread();
    return _ptr;
}


bool Handle::initialized()
{
    if (_ptr &&
        _ptr->type > UV_UNKNOWN_HANDLE &&
        _ptr->type < UV_HANDLE_TYPE_MAX)
        return true;
    return false;
}


bool Handle::active() const
{
    return _ptr && uv_is_active(_ptr) != 0;
}


bool Handle::closed() const
{
    return _closed; //_ptr && uv_is_closing(_ptr) != 0;
}


bool Handle::ref()
{
    if (uv_has_ref(ptr()))
        return false;

    uv_ref(ptr());
    return true;
}


bool Handle::unref()
{
    if (!uv_has_ref(ptr()))
        return false;

    uv_unref(ptr());
    return true;
}


std::thread::id Handle::tid() const
{
    return _tid;
}


const scy::Error& Handle::error() const
{
    return _error;
}


void Handle::setAndThrowError(const std::string& prefix, int errorno)
{
    setUVError(prefix, errorno);
    throwError(prefix, errorno);
}


void Handle::throwError(const std::string& prefix, int errorno) const
{
    throw std::runtime_error(formatError(prefix, errorno));
}


void Handle::setUVError(const std::string& prefix, int errorno)
{
    scy::Error err;
    err.errorno = errorno;
    // err.syserr = uv.sys_errno_;
    err.message = formatError(prefix, errorno);
    setError(err);
}


void Handle::setError(const scy::Error& err)
{
    // if (_error == err) return;
    assertThread();
    _error = err;
    onError(err);
}


void Handle::close()
{
    assertThread();
    if (_ptr) {
        if (initialized() && !_closed && !uv_is_closing(_ptr)) {
            uv_close(_ptr, [](uv_handle_t* handle) {
                delete handle;
            });
        }
        else {
            // If the handle isn't initialized don't call uv_close,
            // just delete the pointer.
            delete _ptr;
        }

        // We no longer know about the handle.
        // The handle pointer has been deleted or will be deleted by uv_close.
        _ptr = nullptr;
        _closed = true;

        // Call onClose to run final callbacks.
        onClose();
    }
}


void Handle::assertThread() const
{
    // uv_thread_t current = uv_thread_self();
    // assert(uv_thread_equal(&_tid, &current));
    assert(std::this_thread::get_id() == _tid);
}


void Handle::onError(const scy::Error& /* error */)
{
}


void Handle::onClose()
{
}


} // namespace uv
} // namespace scy


/// @\}
