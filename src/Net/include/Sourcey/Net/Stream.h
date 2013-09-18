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


#ifndef SOURCEY_Net_Stream_H
#define SOURCEY_Net_Stream_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Memory.h"

#include "Sourcey/Signal.h"
#include "Sourcey/Buffer.h"
#include <stdexcept>


namespace scy {
namespace net {


class Stream: public uv::Handle
{
 public:  
	Stream(uv::Loop& loop = uv::defaultLoop(), void* stream = NULL) :
		uv::Handle(&loop, stream), 
		_buffer(65536)
	{
	}
	
	void close()
		// Closes and resets the stream handle.
		// This will close the active socket/pipe
		// and destroy the uv_stream_t handle.
		//
		// If the stream is already closed this call
		// will have no side-effects.
	{
		traceL("Stream", this) << "Close: " << handle() << std::endl;
		if (!closed()) {
			readStop();		
			uv::Handle::close();
		}
	}
	
	bool shutdown()
		// Sends a shutdown packet to the connected peer.
	{
		traceL("Stream", this) << "Send shutdown" << std::endl;
		if (closed()) {
			warnL("Stream", this) << "Attempted shutdown on closed stream" << std::endl;
			return false;
		}

		// XXX: Sending shutdown causes an eof error to be  
		// returned via handleRead() which sets the stream 
		// to error state. This is not really an error,
		// perhaps it should be handled differently?
		int r = uv_shutdown(new uv_shutdown_t, handle<uv_stream_t>(), uv::afterShutdown);

		// If the stream is not connected this will return false.
		return r == 0;
	}

	bool write(const char* data, int len)
		// Writes data to the stream.
		//
		// Throws a std::runtime_errorif the socket is closed.
	{		
		if (closed())
			throw std::runtime_error("IO error: Cannot write to closed stream");
		
		int r; 		
		uv_write_t* req = new uv_write_t;
		uv_buf_t buf = uv_buf_init((char*)data, len);
		uv_stream_t* stream = this->handle<uv_stream_t>();
		bool isIPC = stream->type == UV_NAMED_PIPE && 
			reinterpret_cast<uv_pipe_t*>(stream)->ipc;

		if (!isIPC)
			r = uv_write(req, stream, &buf, 1, uv::afterWrite);
		else
			r = uv_write2(req, stream, &buf, 1, NULL, uv::afterWrite);

		if (r) {
			delete req;
			setAndThrowLastError("Stream write error");
		}
		return r == 0;
	}
	
	Buffer& buffer()
		// Returns the read buffer.
	{ 
		return _buffer;
	}

	Signal2<const char*, int> Read;
		// Signals when data can be read from the stream.

 protected:	
	bool readStart()
	{
		traceL("Stream", this) << "Read start: " << handle() << std::endl;

		int r;
		uv_stream_t* stream = this->handle<uv_stream_t>();
		bool isIPC = stream->type == UV_NAMED_PIPE && 
			reinterpret_cast<uv_pipe_t*>(stream)->ipc;

		if (isIPC)
			r = uv_read2_start(stream, Stream::allocReadBuffer, handleRead2);
		else 	
			r = uv_read_start(stream, Stream::allocReadBuffer, handleRead);
		if (r) setLastError();	
		return r == 0;
	}

	bool readStop()
	{		
		traceL("Stream", this) << "Read stop: " << handle() << std::endl;
		int r = uv_read_stop(handle<uv_stream_t>());
		if (r) setLastError();
		return r == 0;
	}

	virtual void onRead(const char* data, int len)
	{
		traceL("Stream", this) << "On read: " << len << std::endl;

		// can be overridden
		Read.emit(instance(), data, len);
	}

	static void handleReadCommon(uv_stream_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending) 
	{	
		Stream* io = static_cast<Stream*>(handle->data);
		traceL("Stream", io) << "Handle read: " << nread << std::endl;

		// Handle EOF or error
		if (nread == -1)  {
			io->setLastError();
			return;
		}
		else {
			// We only support UV_TCP right now
			if (pending == UV_TCP)
				assert(0);
			else
				assert(pending == UV_UNKNOWN_HANDLE);
			io->onRead(buf.base, nread);
		}
	}

	virtual ~Stream() 
	{	
	}
	
	virtual void* instance() 
	{ 
		return this;
	}	
	
	//
	// UV callbacks
	//
	
	static void handleRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf) 
	{
		handleReadCommon(handle, nread, buf, UV_UNKNOWN_HANDLE);
	}
	
	static void handleRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf,
		uv_handle_type pending) 
	{
		handleReadCommon((uv_stream_t*)handle, nread, buf, pending);
	}

	static uv_buf_t allocReadBuffer(uv_handle_t *handle, size_t suggested_size)
	{
		Stream* self = static_cast<Stream*>(handle->data);

		// Reserve the recommended buffer size
		//if (suggested_size > self->_buffer.capacity())
		//	self->_buffer.capacity(suggested_size); 
		assert(self->_buffer.capacity() >= suggested_size);

		// Reset the buffer position on each read
		//self->_buffer.position(0);
		return uv_buf_init(self->_buffer.data(), suggested_size);
	}

	Buffer _buffer;
};


} } // namespace scy::net


#endif // SOURCEY_Net_Stream_H


	/*
	//Signal2<const char*, int> Write;
	void setStream(uv_stream_t* stream)
	{
		assert(_handle == NULL);
		stream->data = instance();
		_handle = (uv_handle_t*)stream;
	}
	*/
	//template <class BaseT>
	   
	 //closeOnError(true);//, public SharedObject
	//Stream(uv_stream_t* stream)
	//{
	//	setStream(stream);
	//}

	/*
	void* _instance;
	static void handleRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf);
	static void handleRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
	static void handleReadCommon(uv_stream_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
	static void afterWrite(uv_write_t* req, int status);
	static void afterShutdown(uv_shutdown_t* req, int status);
	*/
		
		/*	

	uv::Buffer _readBuffer;
	Buffer _readBuffer1;
		}
		traceL("Stream", ptr) << "allocReadBuffer " << suggested_size << std::endl;
		//ptr->_readBuffer.data.reserve(suggested_size); 
		//if (suggested_size > ptr->_readBuffer1.capacity())
		//	ptr->_readBuffer1.reserve(suggested_size);
		traceL("Stream", ptr) << "allocReadBuffer: Before alloc: " << ptr->_buffer.available() << std::endl;
			traceL("Stream", ptr) << "allocReadBuffer: DO alloc: " << std::endl;
		conn->data
		_readBuffer
		Connection *conn = (Connection*) handle;
		assert(conn->read_clear); // Ensure that the last _buffer has been read by on_read_cb.
		conn->read_clear = false;
		conn->read_buffer.reserve_more(suggested_size); 
		traceL("Stream", ptr) << "allocReadBuffer: After alloc: " << ptr->_buffer.available() << std::endl;
		 //[0]; //ptr->_readBuffer1.bytes(); //(char*)&ptr->_readBuffer.data[0]; //new char[suggested_size];&*v.begin();

		uv_buf_t buf;
		buf.base = &ptr->_buffer.at(0);
		buf.len = suggested_size;
		return buf;
		*/  


	/*

	bool closing()
	{
		return uv_is_closing(_handle) == 0;
	}
	*/
	//virtual void onError(const int errno);
	//virtual void onClose();
//template <class HandleT>
	//int writeQueueSize() const;
//	bool _closing;
//<HandleT> //SharedObject

	//static void afterClose(uv_handle_t* peer);
	//std::unique_ptr<uv_stream_t> _stream;

	//void close();