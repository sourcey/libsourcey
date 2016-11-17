///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup uv
/// @{

#include "scy/uv/uvpp.h"


namespace scy {
namespace uv {


Handle::Handle(uv_loop_t* loop, void* handle) :
    _loop(loop ? loop : uv_default_loop()), // nullptr will be uv_default_loop
    _ptr((uv_handle_t*)handle), // nullptr or instance of uv_handle_t
    _tid(uv_thread_self()),
    _closed(false)
{
    if (_ptr)
        _ptr->data = this;
}


Handle::~Handle()
{
    assertThread();
    if (!_closed)
        close();
    assert(_ptr == nullptr);
}


void Handle::setLoop(uv_loop_t* loop)
{
    assertThread();
    assert(_ptr == nullptr && "loop must be set before handle");
    _loop = loop;
}


uv_loop_t* Handle::loop() const
{
    assertThread();
    return _loop;
}


uv_handle_t* Handle::ptr() const
{
    assertThread();
    return _ptr;
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
    if (!active())
        return false;

    uv_ref(ptr());
    return true;
}


bool Handle::unref()
{
    if (active())
        return false;

    uv_unref(ptr());
    return true;
}


uv_thread_t Handle::tid() const
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
    //err.syserr = uv.sys_errno_;
    err.message = formatError(prefix, errorno);
    setError(err);
}


void Handle::setError(const scy::Error& err)
{
    //if (_error == err) return;
    assertThread();
    _error = err;
    onError(err);
}


void Handle::close()
{
    assertThread();
    if (!_closed) {
        if (_ptr && !uv_is_closing(_ptr)) {
            uv_close(_ptr, [](uv_handle_t* handle) {
                delete handle;
            });
        }

        // We no longer know about the handle.
        // The handle pointer will be deleted on afterClose.
        _ptr = nullptr;
        _closed = true;

        // Send the local onClose to run final callbacks.
        onClose();
    }
}


void Handle::assertThread() const
{
#ifdef _DEBUG
		uv_thread_t current = uv_thread_self();
		assert(uv_thread_equal(&_tid, &current));
#endif
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
