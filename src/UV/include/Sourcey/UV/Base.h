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


#ifndef SOURCEY_UV_Base_H
#define SOURCEY_UV_Base_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Logger.h"
//#include "Sourcey/Memory.h"
#include "assert.h"


namespace scy {
namespace uv {


// -------------------------------------------------------------------
//
struct AsyncDeleter
{
	uv_handle_t* handle;

	AsyncDeleter(uv_handle_t* handle) : 
		handle(handle) 
	{
		debugL("AsyncDeleter", this) << "Creating: " << handle << std::endl;
		assert(handle->loop);
		handle->data = this;
	}

	~AsyncDeleter()
	{
		debugL("AsyncDeleter", this) << "Destroying" << std::endl;
		assert(handle == NULL);
	}
	
	void run() 
	{ 				
		debugL("AsyncDeleter", this) << "Running: " << handle << std::endl;
		// handle->reqs_pending = 0

		// Call uv_close resulting in the async destruction of  
		// the handle and the current instance.
		uv_close(handle, AsyncDeleter::afterClose);

		assert(uv_is_closing(handle));
			
		// NOTE: We don't run the loop for cleanup here anymore.
		// It is now the responsibility of the application to call
		// uv_run on the loop before it is destroyed.
		// See Runner::cleanup()
		
		/*
		// Determine weather the event loop is currently active.
		// If not we need to run a single iteration so we can free
		// memory associated with the handle.
		if (handle->loop->active_handles == 1) {
			debugL("AsyncDeleter", this) << "Running cleanup loop" << std::endl;
			uv_run(handle->loop, UV_RUN_DEFAULT); // UV_RUN_ONCE doesn't seem to work?
			debugL("AsyncDeleter", this) << "Running cleanup loop: OK" << std::endl;
		}
		*/
	}
		
	static void afterClose(uv_handle_t* handle)
	{
		traceL("AsyncDeleter") << "After Close: " << handle << std::endl;		
		AsyncDeleter* self = reinterpret_cast<AsyncDeleter*>(handle->data);
		//traceL("AsyncDeleter", self) << "After Close: " << handle << std::endl;		
		assert(self->handle);
		assert(self->handle == handle);	
		assert(self->handle->data == self);	
		assert(uv_is_closing(self->handle));
		
		// print active handles
		traceL("AsyncDeleter") << "After Close: Printing Handles" << std::endl;	
		uv_walk(self->handle->loop, AsyncDeleter::onPrintHandle, NULL);
		traceL("AsyncDeleter") << "After Close: Printing Handles: OK" << std::endl;	

		delete self->handle;
		self->handle = NULL;
		delete self;
	}
	
	static void onPrintHandle(uv_handle_t* handle, void* arg) 
	{
		debugL("AsyncDeleter") << "#### Active Handle: " << handle << std::endl;
	}
};


// -------------------------------------------------------------------
//
class StandardObject
	/// A standard base class for objects which can be 
	/// created and destroyed on the heap or stack ie.
	/// no garbage colleaction.
{
};


template <class MemoryT = StandardObject>
class Base: public MemoryT
	/// A base class for extending libuv and handling the
	/// asynchronous destruction of libuv handle pointers in C++. 
	///
	/// The MemoryT can be a CountedObject or StandardObject
	/// depending on implementation memory requirements.
{
public:
	Base(uv_loop_t* loop = NULL, void* handle = NULL) : 
		_loop(loop ? loop : uv_default_loop()),
		_handle((uv_handle_t*)handle) // must be uv_handle_t
	{
		traceL("Base", this) << "Creating: " << _handle << std::endl;	
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

	virtual void* instance()
		/// Returns the derived instance pointer for signal callbacks.
		/// This method must be derived to be effective.
	{ 
		return this; 
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
		
	virtual uv_err_t error() 
		/// Returns the libuv error context, if any.
	{ 
		return _error;
	}

	virtual std::string errorMessage()
		/// Returns the error message, if any.
	{ 
		return uv_strerror(_error);
	}	

	virtual void setLastError(bool throwException = false) 
		/// Sets the last error, and optionally throws an exception.
	{
		setError(uv_last_error(loop()));
		if (throwException)
			throw Exception(errorMessage());
	}
		
	virtual void setError(const uv_err_t& err) 
		/// Sets the error content and triggers callbacks.
	{ 
		//errorL("Base", this) << "Set Error: " 
		//	<< err.code << ": " 
		//	<< err.sys_errno_ << ": " 
		//	<< uv_strerror(err) << std::endl;
		assert(_error.code != err.code);
		_error = err; 
		onError(err.sys_errno_);
	}

protected:
	virtual ~Base()
		/// Destroys the Handle.
		/// Protected destructor since some livub classes use 
		/// reference counting memory management.
		/// If this is the case subclasses should extend destroy() instead.
	{
		traceL("Base", this) << "Destroying: " << _handle << std::endl;	

		// If the handle has not been deleted yet we free it now.
		if (_handle) {
			traceL("Base", this) << "Destroying: Closing" << std::endl;	
			close();
		}

		assert(_handle == NULL);
	}
	
	virtual void close()
		/// Closes and destroys the libuv handle
		/// without destroying the current instance.
	{
		traceL("Base", this) << "Closing: " << _handle << std::endl;	
		if (_handle) {
			assert(!uv_is_closing(_handle));
			AsyncDeleter* async = new AsyncDeleter(_handle);
			async->run();
			assert(uv_is_closing(_handle));
			_handle = NULL;
			onClose();
		}
		else
			warnL("Base", this) << "Already closing: " << _handle << std::endl;	
	}

	virtual void onError(int syserr) 
	{		
		errorL("Base", this) << "On Error: " << errorMessage() << std::endl;	
	}

	virtual void onClose()
		/// Can be extended to provide closing logic
		/// before the destructor is called.
	{
		traceL("Base", this) << "On Close" << std::endl;		
	}

 protected:
	uv_handle_t* _handle;
	uv_loop_t* _loop;
	uv_err_t _error;
	void* _instance;
	//bool _closeOnError;
};


} } // namespace scy::uv


#endif // SOURCEY_UV_Handle_H


		//if (_closeOnError)
		//	close();
		//,_closeOnError(false)
	/*
	virtual void closeOnError(bool flag)
		/// Sets the closeOnError option.
		/// When true close() will be called directly after
		/// setting the error context. 
	{
		_closeOnError = flag;
	}
	*/
	
	/*
		//_destroying(false), 
	virtual bool destroying() const
		/// Returns the destroying boolean value.
	{ 
		return _destroying; 
	}
	bool _destroying;
	*/

		// If the handle is not NULL or closing something went wrong.
		//assert(!_handle || uv_is_closing(_handle));

		
		/*
		if (_handle && !uv_is_closing(_handle)) { // && !_destroying
			traceL("Base", this) << "Closing Handle" << std::endl;	
			//uv_close(_handle, Base::afterClose);
		}
		else
			warnL("Base", this) << "Handle already closing: " << _handle << std::endl;	
			*/

	
	/*
	virtual void destroy()
		/// Closes and destroys the libuv handle
		/// and then destroys the current instance.
	{
		traceL("Base", this) << "Destroy" << std::endl;		
		assert(!_destroying);
		_destroying = true;
		
		// If the handle is set we try to destroy it 
		// gracefully via callback.
		if (_handle) {
				
			// Determine weather the event loop is active.
			// If not we need to run a single iteration so we
			// can free all memory associated with the handle.
			bool needCleanup = _loop->active_handles == 0;

			// Always call close().
			// This must result in a call to uv_close.
			close();
			
			// If the event loop is inavtive we run it now.
			// The current instance will be deleted via afterClose()
			if (needCleanup) {

				debugL("Base", this) << "Destroy: Running cleanup loop" << std::endl;
				uv_run(loop(), UV_RUN_DEFAULT); // UV_RUN_ONCE doesn't seem to work?
			}
			return;
		}

		// If there is no handle we just call the destructor
		onClose();
		delete this;
	}
	*/
	
	/*
	static void afterClose(uv_handle_t* handle)
	{
		Base* self = static_cast<Base*>(handle->data);
		traceL("Base", self) << "After Close: " << handle << std::endl;		
		assert(self->_handle);
		assert(self->_handle == handle);	
		assert(uv_is_closing(self->_handle));
		delete self->_handle;
		self->_handle = NULL;
		self->onClose();
		if (self->_destroying)
			delete self;
	}
	*/


/*		
	Signal<int> Error;
		/// Signals on handle error state

	NullSignal Closed;
		/// Signals on handle closed state
		*/