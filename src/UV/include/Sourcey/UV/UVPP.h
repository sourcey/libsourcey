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


#include "uv.h"
#include "Sourcey/Types.h"
#include "Sourcey/Exception.h"
//#include "Sourcey/Logger.h"
//#include "Sourcey/Memory.h"
//#include <string>
#include <assert.h>


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


namespace scy {
namespace uv {


//
// Helpers
//
	
inline void throwLastError(
	uv_loop_t* loop = NULL,
	const std::string& prefix = "",
	const std::string& suffix = "") 
{
	throw Exception(
		prefix + uv_strerror(
			uv_last_error(loop)) + suffix);
}


//
// Default callbacks
//
	
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


// -------------------------------------------------------------------
//
typedef uv_loop_t Loop;

inline Loop& defaultLoop()
{
	return *uv_default_loop();
}


// -------------------------------------------------------------------
//
//template <class MemoryT = StandardObject>
class Base//: public MemoryT
	/// A base class for extending libuv and handling the
	/// asynchronous destruction of libuv handle pointers in C++. 
	///
	/// The MemoryT can be a CountedObject or StandardObject
	/// depending on implementation memory requirements.
{
public:
	Base(uv_loop_t* loop = NULL, void* handle = NULL) : 
		_loop(loop ? loop : uv_default_loop()),
		_handle((uv_handle_t*)handle) // must be NULL or uv_handle_t
	{
		////traceL("UVBase", this) << "Creating: " << _handle << std::endl;	
		if (_handle)
			_handle->data = this;
	}

	virtual void setLoop(uv_loop_t* loop)
		/// Sets the libuv loop.
		/// Must be set before the handle is initialized. 
	{
		assert(_handle == NULL);
		_loop = loop;
	}

	virtual uv_loop_t* loop() const
		/// Returns a pointer to the current libuv event loop.
	{
		return _loop;
	}
	
	template <class T>
	T* handle() const
		/// Returns a cast pointer to the managed libuv handle.
	{ 		
		return reinterpret_cast<T*>(_handle);
	}
	
	virtual uv_handle_t* handle() const
		/// Returns a pointer to the managed libuv handle.
	{ 
		return _handle; 
	}
	
	virtual bool closed() const
		/// Returns true when the handle is NULL.
	{ 
		return _handle == NULL; 
	}
	
	virtual bool active() const
		/// Returns true when the handle is active.
	{ 
		return _handle && uv_is_active(_handle) == 1;
	}
		
	const scy::Error& error() const
		/// Returns the error context if any.
	{ 
		return _error;
	}

	virtual void setLastError(const std::string& prefix = "UV Error")
		/// Sets the last error and sends relevent callbacks.
		/// This method can be called inside libuv callbacks.
	{
		uv_err_t uv = uv_last_error(loop());
		Error err;
		err.message = formatError(prefix); //uv_strerror(uv);
		err.syserr = uv.sys_errno_;
		err.uverr = uv.code;
		setError(err);
	}
	
	virtual void setAndThrowLastError(const std::string& prefix = "UV Error")
		/// Sets and throws the last error.
		/// Should never be called inside libuv callbacks.
	{
		setLastError(prefix);
		throwLastError(prefix);
	}

	virtual void throwLastError(const std::string& prefix = "UV Error") const
		/// Throws the last error.
		/// This function is const so it can be used for
		/// invalid getter operations on closed handles.
		/// The actual error would be set on the next iteraton.
	{
		throw Exception(formatError(prefix));
	}
		
	virtual void setError(const Error& err) 
		/// Sets the error content and triggers callbacks.
	{ 
		//errorL("UVBase", this) << "Setting error: " << err.message << std::endl;
		if (_error.uverr != err.uverr) {
			_error = err; 
			onError(err);
		}
	}

protected:
	virtual ~Base()
		/// Destroys the Handle.
		/// Protected destructor since some livub classes  
		/// use reference counting memory management.
	{
		//traceL("UVBase", this) << "Destroying: " << _handle << std::endl;	

		// If the handle has not been deleted yet we free it now.
		if (_handle) {
			////traceL("UVBase", this) << "Destroying: Closing" << std::endl;	
			close();
		}

		assert(_handle == NULL);
	}
	
	virtual void close()
		/// Closes and destroys the associated libuv handle.
	{
		//traceL("UVBase", this) << "Closing: " << _handle << std::endl;	
		if (_handle) {
			uv_close(_handle, uv::afterClose);

			/// We no longer know about the handle.
			/// The handle pointer will be deleved by afterClose.
			_handle = NULL;

			/// onClose is only called when the handle is being
			/// destroyed.
			onClose();
		}
		//else
		//	warnL("UVBase", this) << "Already closing: " << _handle << std::endl;	
	}

	virtual void onError(const Error& error) 
		/// Override to handle errors.
		/// The error may be a UV error, or a custom error.
	{		
		//errorL("UVBase", this) << "On Error: " << error.message << std::endl;	
	}

	virtual std::string formatError(const std::string& prefix = "") const
	{	
		std::string err;
		if (!prefix.empty()) {
			err += prefix;
			err += ": ";
		}
		err += uv_strerror(uv_last_error(loop()));
		return err;
	}

	virtual void onClose()
		/// Override to handle closure.
	{
		//traceL("UVBase", this) << "On Close" << std::endl;
	}

 protected:
	uv_handle_t* _handle;
	uv_loop_t* _loop;
	Error _error;
};


} } // namespace scy::uv


#endif // SOURCEY_UV_UVPP_H

	
	/*
	virtual void tryThrowException(const std::string& reason) const
		/// Throws the last error depending on weather 
		/// or not if the whiny flag is set.
		/// This function is const so it can be used for
		/// invalid getter operations on closed handles.
		/// The actual error would be set on the next iteraton.
	{
		if (_whiny)
			throw Exception(reason);
	}
		
	virtual void setWhiny(bool whiny) 
		/// Sets the error content and triggers callbacks.
	{ 
		_whiny = whiny;
	}
	*/
		
		//if (throwException)
			//throw Exception(formatError(prefix, suffix);
			//throw Exception("UV Error" + err.message);
		/*
		inline void throwLastError(
			uv_loop_t* loop = NULL,
			const std::string& prefix = "",
			const std::string& suffix = "") 
		{
			throw Exception(
				prefix + uv_strerror(
					uv_last_error(loop)) + suffix);
		}
		*/

		/*
		uv_err_t uv = uv_last_error(loop());
		Error err;
		err.message = uv_strerror(uv);
		err.syserr = uv.sys_errno_;
		err.uverr = uv.code;
		setError(err);
		if (throwException)
			throw Exception("UV Error" + err.message);
			*/


	/*
			assert(!uv_is_closing(_handle));
			assert(uv_is_closing(_handle));
			//AsyncDeleter* async = new AsyncDeleter(_handle);
			//async->run();
	virtual void* instance()
		/// Returns the derived instance pointer for signal callbacks.
		/// This method must be derived to be effective.
	{ 
		return this; 
	}
	*/



//
// Handle Types
//
/*
typedef struct uv_loop_s			uv_loop_t;
typedef struct uv_ares_task_s		AresTaskHandle;
typedef struct uv_err_s				uv_err_t;
typedef struct uv_handle_s			Handle;
typedef struct uv_stream_s			uv_stream_t;
typedef struct uv_tcp_s				TCPHandle;
typedef struct uv_udp_s				UDPHandle;
typedef struct uv_pipe_s			PipeHandle;
typedef struct uv_tty_s				TTYHandle;
typedef struct uv_timer_s			uv_timer_t;
typedef struct uv_prepare_s			PrepareHandle;
typedef struct uv_check_s			CheckHandle;
typedef struct uv_idle_s			IdleHandle;
typedef struct uv_async_s			AsyncHandle;
typedef struct uv_getaddrinfo_s		GetaddrinfoHandle;
typedef struct uv_process_s			ProcessHandle;
typedef struct uv_counters_s		CountersHandle;
*/

//
// Req Types
//
//typedef struct uv_connect_t			ConnectReq;

//
// Other Types
//
//typedef struct uv_buf_t				Buffer;
	
	


/*

	//void* _instance;
	//bool _closeOnError;

inline uv_buf_t defaultAlloc(uv_handle_t* handle, size_t size)
	// NOTE: All data allocated via this function must be freed!
{
	uv_buf_t buf;
	buf.base = new char[size];
	buf.len = size;
	return buf;
}

	
inline void defaultClose(uv_handle_t* handle)
{
	delete handle;
}
*/


		/*
struct AsyncDeleter
{
	uv_handle_t* handle;

	AsyncDeleter(uv_handle_t* handle) : 
		handle(handle) 
	{
		debugL("UVAsyncDeleter", this) << "Creating: " << handle << std::endl;
		assert(handle->loop);
		handle->data = this;
	}

	~AsyncDeleter()
	{
		debugL("UVAsyncDeleter", this) << "Destroying" << std::endl;
		assert(handle == NULL);
	}
	
	void run() 
	{ 				
		debugL("UVAsyncDeleter", this) << "Running: " << handle << std::endl;
		// handle->reqs_pending = 0

		// Call uv_close resulting in the async destruction of  
		// the handle and the current instance.
		uv_close(handle, AsyncDeleter::afterClose);

		assert(uv_is_closing(handle));
			
		// NOTE: We don't run the loop for cleanup here anymore.
		// It is now the responsibility of the application to call
		// uv_run on the loop before it is destroyed.
		// See Runner::cleanup()
		
		// Determine weather the event loop is currently active.
		// If not we need to run a single iteration so we can free
		// memory associated with the handle.
		if (handle->loop->active_handles == 1) {
			debugL("UVAsyncDeleter", this) << "Running cleanup loop" << std::endl;
			uv_run(handle->loop, UV_RUN_DEFAULT); // UV_RUN_ONCE doesn't seem to work?
			debugL("UVAsyncDeleter", this) << "Running cleanup loop: OK" << std::endl;
		}
	}
		
	static void afterClose(uv_handle_t* handle)
	{
		//traceL("UVAsyncDeleter") << "After Close: " << handle << std::endl;		
		AsyncDeleter* self = reinterpret_cast<AsyncDeleter*>(handle->data);
		////traceL("UVAsyncDeleter", self) << "After Close: " << handle << std::endl;		
		assert(self->handle);
		assert(self->handle == handle);	
		assert(self->handle->data == self);	
		assert(uv_is_closing(self->handle));
		
		// print active handles
		//traceL("UVAsyncDeleter") << "After Close: Printing Handles" << std::endl;	
		uv_walk(self->handle->loop, AsyncDeleter::onPrintHandle, NULL);
		//traceL("UVAsyncDeleter") << "After Close: Printing Handles: OK" << std::endl;	

		//delete self->handle; // TODO: Fixme
		self->handle = NULL;
		delete self;
	}
	
	static void onPrintHandle(uv_handle_t* handle, void* arg) 
	{
		debugL("UVAsyncDeleter") << "#### Active Handle: " << handle << std::endl;
	}
};
		*/