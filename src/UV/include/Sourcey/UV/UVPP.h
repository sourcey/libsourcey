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


#ifndef SOURCEY_UV_UVPP_H
#define SOURCEY_UV_UVPP_H


// Disable unnecessary warnings
#if defined(_MSC_VER)
	#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
	#pragma warning(disable:4505) // unreferenced local function has been removed 
                                  // Todo: depreciate once we replace static functions with lambdas
#endif

#include "uv.h"
#include "Sourcey/Types.h"
#include "Sourcey/Exception.h"
#include <exception>
#include <stdexcept>
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
// Default callbacks - depreciated!
//


#define UVCallback(ClassName, Function, Handle)						 \
															         \
	static void _Function(Handle* handle) {						     \
		static_cast<ClassName*>(handle->data)->Function();			 \
    };														         \


#define UVStatusCallback(ClassName, Function, Handle)				 \
															         \
	static void Function(Handle* handle, int status) {			     \
		ClassName* self = static_cast<ClassName*>(handle->data);	 \
		if (status)											         \
			self->setUVError("UV error", status);	                 \
		self->Function(status);									     \
    }														         \
	

#define UVEmptyStatusCallback(ClassName, Function, Handle)			 \
															         \
	static void Function(Handle* handle, int status) {			     \
		ClassName* self = static_cast<ClassName*>(handle->data);	 \
		if (status)											         \
			self->setUVError("UV error", status);	                 \
		self->Function();									         \
    }														         \


#define UVStatusCallbackWithType(ClassName, Function, Handle)		\
															        \
	static void Function(Handle* handle, int status) {			    \
		ClassName* self = static_cast<ClassName*>(handle->data);	\
		if (status)											        \
			self->setUVError("UV error", status);	                \
		self->Function(handle, status);							    \
    }														        \
	

//
// Default Event Loop
//

typedef uv_loop_t Loop;
static unsigned long defaultTID = 0;

inline Loop* defaultLoop()
{
	// Capture the main TID the first time
	// uv_default_loop is accessed.
	if (defaultTID == 0)
		defaultTID = uv_thread_self();
	return uv_default_loop();
}


//
// UV Handle
//

class Handle
	/// A base class for managing the lifecycle of a libuv handle,  
	/// including its asynchronous destruction mechanism.
{
public:
	Handle(uv_loop_t* loop = nullptr, void* handle = nullptr) : 
		_loop(loop ? loop : uv_default_loop()), // nullptr will be uv_default_loop
		_ptr((uv_handle_t*)handle), // can be nullptr or uv_handle_t
		_tid(uv_thread_self())
	{
		if (_ptr)
			_ptr->data = this;
	}
		
	virtual ~Handle()
	{
		assertTID();
		if (_ptr) 
			close();
		assert(_ptr == nullptr);
	}

	virtual void setLoop(uv_loop_t* loop)
		// The event loop may be set before the handle is initialized. 
	{
		assertTID();
		assert(_ptr == nullptr && "set loop before handle");
		_loop = loop;
	}

	virtual uv_loop_t* loop() const
	{
		assertTID();
		return _loop;
	}
	
	template <class T>
	T* handle() const
		// Returns a cast pointer to the managed libuv handle.
	{ 		
		// assertTID(); // conflict with uv_async_send in SyncContext
		return reinterpret_cast<T*>(_ptr);
	}
	
	virtual uv_handle_t* handle() const
		// Returns a pointer to the managed libuv handle.
	{ 
		assertTID();
		return _ptr; 
	}
	
	virtual bool active() const
		// Returns true when the handle is active.
		// This method should be used instead of closed() to determine 
		// the veracity of the libuv handle for stream io operations.
	{ 
		return _ptr && uv_is_active(_ptr) != 0;
	}
	
	virtual bool closed() const
		// Returns true when the handle is nullptr.
	{ 
		return !_ptr; // && uv_is_closing(_ptr) != 0;
	}
	
	unsigned int tid() const
		// Returns the parent thread ID.
	{ 
		return _tid;
	}
		
	const scy::Error& error() const
		// Returns the error context if any.
	{ 
		return _error;
	}
	
	virtual void setAndThrowError(const std::string& prefix = "UV Error", int errorno = 0)
		// Sets and throws the last error.
		// Should never be called inside libuv callbacks.
	{
		setUVError(prefix, errorno);
		throwError(prefix, errorno);
	}

	virtual void throwError(const std::string& prefix = "UV Error", int errorno = 0) const
		// Throws the last error.
		// This function is const so it can be used for
		// invalid getter operations on closed handles.
		// The actual error would be set on the next iteraton.
	{
		throw std::runtime_error(formatError(prefix, errorno));
	}

	virtual void setUVError(const std::string& prefix = "UV Error", int errorno = 0)
		// Sets the last error and sends relevant callbacks.
		// This method can be called inside libuv callbacks.
	{
		scy::Error err;
		err.errorno = errorno;
		//err.syserr = uv.sys_errno_;
		err.message = formatError(prefix, errorno);
		setError(err);
	}
		
	virtual void setError(const scy::Error& err) 
		// Sets the error content and triggers callbacks.
	{ 
		assertTID();
		_error = err; 
		onError(err);
		//if (_error.uverr != err.uverr) {
		//	_error = err; 
		//	onError(err);
		//}
	}

	virtual void close()
		// Closes and destroys the associated libuv handle.
	{
		assertTID();
		if (_ptr) {
			if (!uv_is_closing(_ptr)) {
				uv_close(_ptr, [](uv_handle_t* handle) {
					delete handle;
				});
			}

			// We no longer know about the handle.
			// The handle pointer will be deleted on afterClose.
			_ptr = nullptr;

			// Send the local onClose to run final callbacks.
			onClose();
		}
	}
		
	void assertTID() const
		// Make sure we are calling from the event loop thread
	{
#ifdef _DEBUG
		assert(_tid == defaultTID
			|| _tid == uv_thread_self());
#endif
	}

protected:	
	virtual void onError(const scy::Error& /* error */) 
		// Override to handle errors.
		// The error may be a UV error, or a custom error.
	{
	}

	virtual void onClose()
		// Override to handle closure.
	{
	}

 protected:
	Handle(const Handle&); // = delete;
	Handle(Handle&&); // = delete;
	Handle& operator=(const Handle&); // = delete;
	Handle& operator=(Handle&&); // = delete;
	
	uv_handle_t* _ptr;
	uv_loop_t* _loop;
	scy::Error _error;
	unsigned long _tid;
};


} } // namespace scy::uv


#endif // SOURCEY_UV_UVPP_H


	

/* 
#define UVReadCallback(ClassName, Function, Handle)				    \
															        \
	static void Function(Handle* handle, ssize_t nread, uv_buf_t buf) {	\
		ClassName* self = static_cast<ClassName*>(handle->data);	\
		if (nread == -1)											\
			self->setUVError(nread);	TODO: proper error code?   \
		self->Function(handle, nread, buf);							\
    }														        \
	*/
/*
inline void throwLastError(uv_loop_t* loop, const std::string& message = "") 
{
	throwError(message, uv_last_error(loop).code);
}
*/
/*
static void afterWrite(uv_write_t* req, int status) 
{
	delete req;
}

static void afterShutdown(uv_shutdown_t* req, int status) 
{	
	delete req;
}

static void afterClose(uv_handle_t* handle) 
{	
	delete handle;
}
*/
	//std::string err;
	//if (!prefix.empty()) {
	//	err += prefix;
	//	err += ": ";
	//}
	//err += uv_strerror(uv_last_error(loop()));
	//std::string m(uv_strerror());
	//if (!message.empty()) {
	//	m.append(": ");
	//	m.append(message);
	//}