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
	
inline std::string formatError(const std::string& message, int errorno = UV_UNKNOWN)
{	
	std::string m(message); // prefix the message, since libuv errors are very brisk
	if (errorno != UV_UNKNOWN && 
		errorno != UV_OK) {
		uv_err_s err;
		err.code = (uv_err_code)errorno;
		if (!m.empty())
			m.append(": ");
		m.append(uv_strerror(err));
	}
	return m;
}
	

inline void throwError(const std::string& message, int errorno = UV_UNKNOWN) 
{
	throw std::runtime_error(formatError(message, errorno));
}
	
inline void throwLastError(uv_loop_t* loop, const std::string& message = "") 
{
	throwError(message, uv_last_error(loop).code);
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
			self->setLastError();	                                 \
		self->Function(status);									     \
    }														         \
	

#define UVEmptyStatusCallback(ClassName, Function, Handle)			 \
															         \
	static void Function(Handle* handle, int status) {			     \
		ClassName* self = static_cast<ClassName*>(handle->data);	 \
		if (status)											         \
			self->setLastError();	                                 \
		self->Function();									         \
    }														         \


#define UVReadCallback(ClassName, Function, Handle)				    \
															        \
	static void Function(Handle* handle, ssize_t nread, uv_buf_t buf) {	\
		ClassName* self = static_cast<ClassName*>(handle->data);	\
		if (nread == -1)											\
			self->setLastError();	                                \
		self->Function(handle, nread, buf);							\
    }														        \


#define UVStatusCallbackWithType(ClassName, Function, Handle)		\
															        \
	static void Function(Handle* handle, int status) {			    \
		ClassName* self = static_cast<ClassName*>(handle->data);	\
		if (status)											        \
			self->setLastError();	                                \
		self->Function(handle, status);							    \
    }														        \


static void afterWrite(uv_write_t* req, int /* status */) 
{
	delete req;
}

static void afterShutdown(uv_shutdown_t* req, int /* status */) 
{	
	delete req;
}

static void afterClose(uv_handle_t* handle) 
{	
	delete handle;
}


//
// Default Event Loop
//

typedef uv_loop_t Loop;

inline Loop& defaultLoop()
{
	return *uv_default_loop();
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
		_loop(loop ? loop : uv_default_loop()),
		_handle((uv_handle_t*)handle) // must be nullptr or uv_handle_t
	{
		if (_handle)
			_handle->data = this;
	}
		
	virtual ~Handle()
	{
		if (_handle) 
			close();
		assert(_handle == nullptr);
	}

	virtual void setLoop(uv_loop_t* loop)
		// The event loop may be set before the handle is initialized. 
	{
		assert(_handle == nullptr);
		_loop = loop;
	}

	virtual uv_loop_t* loop() const
	{
		return _loop;
	}
	
	template <class T>
	T* handle() const
		// Returns a cast pointer to the managed libuv handle.
	{ 		
		return reinterpret_cast<T*>(_handle);
	}
	
	virtual uv_handle_t* handle() const
		// Returns a pointer to the managed libuv handle.
	{ 
		return _handle; 
	}
	
	virtual bool closed() const
		// Returns true when the handle is nullptr.
	{ 
		return _handle == nullptr; 
	}
	
	virtual bool active() const
		// Returns true when the handle is active.
	{ 
		return _handle && uv_is_active(_handle) == 1;
	}
		
	const scy::Error& error() const
		// Returns the error context if any.
	{ 
		return _error;
	}

	virtual void setLastError(const std::string& prefix = "UV Error")
		// Sets the last error and sends relevant callbacks.
		// This method can be called inside libuv callbacks.
	{
		uv_err_t uv = uv_last_error(loop());
		Error err;
		err.uverr = uv.code;
		err.syserr = uv.sys_errno_;
		err.message = formatError(prefix, uv.code);
		setError(err);
	}
	
	virtual void setAndThrowLastError(const std::string& prefix = "UV Error")
		// Sets and throws the last error.
		// Should never be called inside libuv callbacks.
	{
		setLastError(prefix);
		throwLastError(prefix);
	}

	virtual void throwLastError(const std::string& prefix = "UV Error") const
		// Throws the last error.
		// This function is const so it can be used for
		// invalid getter operations on closed handles.
		// The actual error would be set on the next iteraton.
	{
		throw std::runtime_error(formatError(prefix));
	}
		
	virtual void setError(const Error& err) 
		// Sets the error content and triggers callbacks.
	{ 
		if (_error.uverr != err.uverr) {
			_error = err; 
			onError(err);
		}
	}

protected:	
	virtual void close()
		// Closes and destroys the associated libuv handle.
	{
		if (_handle) {
			uv_close(_handle, uv::afterClose);

			// We no longer know about the handle.
			// The handle pointer will be deleted on afterClose.
			_handle = nullptr;

			// Send the local onClose to run final callbacks.
			onClose();
		}
	}

	virtual void onError(const Error& /* error */) 
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

	uv_handle_t* _handle;
	uv_loop_t* _loop;
	Error _error;
};


} } // namespace scy::uv


#endif // SOURCEY_UV_UVPP_H

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