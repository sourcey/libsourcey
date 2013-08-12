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


#ifndef SOURCEY_UV_Handle_H
#define SOURCEY_UV_Handle_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Logger.h"
//#include "Sourcey/Memory.h"
#include "assert.h"


namespace scy {
namespace uv {

} } // namespace scy::uv


#endif // SOURCEY_UV_Handle_H


		//if (_closeOnError)
		//	close();
		//,_closeOnError(false)
	/*

	virtual void reset()
		// Reinitializes the libuv handle after closure.
	{
	}
	virtual void closeOnError(bool flag)
		// Sets the closeOnError option.
		// When true close() will be called directly after
		// setting the error context. 
	{
		_closeOnError = flag;
	}
	*/
	
	/*
		//_destroying(false), 
	virtual bool destroying() const
		// Returns the destroying boolean value.
	{ 
		return _destroying; 
	}
	bool _destroying;
	*/

		// If the handle is not NULL or closing something went wrong.
		//assert(!_handle || uv_is_closing(_handle));

		
		/*
		if (_handle && !uv_is_closing(_handle)) { // && !_destroying
			traceL("Handle", this) << "Closing Handle" << std::endl;	
			//uv_close(_handle, Handle::afterClose);
		}
		else
			warnL("Handle", this) << "Handle already closing: " << _handle << std::endl;	
			*/

	
	/*
	virtual void destroy()
		// Closes and destroys the libuv handle
		// and then destroys the current instance.
	{
		traceL("Handle", this) << "Destroy" << std::endl;		
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

				debugL("Handle", this) << "Destroy: Running cleanup loop" << std::endl;
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
		Handle* self = static_cast<Handle*>(handle->data);
		traceL("Handle", self) << "After Close: " << handle << std::endl;		
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
		// Signals on handle error state

	NullSignal Closed;
		// Signals on handle closed state
		*/