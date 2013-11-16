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


#ifndef SCY_Net_Stream_H
#define SCY_Net_Stream_H


#include "scy/uv/uvpp.h"
#include "scy/memory.h"

#include "scy/signal.h"
#include "scy/buffer.h"
#include <stdexcept>


namespace scy {
namespace net {


class Stream: public uv::Handle
{
 public:  
	Stream(uv::Loop* loop = uv::defaultLoop(), void* stream = nullptr) :
		uv::Handle(loop, stream), 
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
		traceL("Stream", this) << "Close: " << ptr() << std::endl;
		if (active())
			readStop();
		uv::Handle::close();
	}
	
	bool shutdown()
		// Sends a shutdown packet to the connected peer.
		// Returns true if the shutdown packet was sent.
	{
		assertTID();

		traceL("Stream", this) << "Send shutdown" << std::endl;
		if (!active()) {
			warnL("Stream", this) << "Attempted shutdown on closed stream" << std::endl;
			return false;
		}

		// XXX: Sending shutdown causes an eof error to be  
		// returned via handleRead() which sets the stream 
		// to error state. This is not really an error,
		// perhaps it should be handled differently?
		int r = uv_shutdown(new uv_shutdown_t, ptr<uv_stream_t>(), [](uv_shutdown_t* req, int) {
			delete req;
		});

		return r == 0;
	}

	bool write(const char* data, int len)
		// Writes data to the stream.
		//
		// Returns false if the underlying socket is closed.
		// This method does not throw an exception.
	{		
		assertTID();

		//if (closed())
		//	throw std::runtime_error("IO error: Cannot write to closed stream");
		if (!active())
			return false;

		int r; 		
		uv_write_t* req = new uv_write_t;
		uv_buf_t buf = uv_buf_init((char*)data, len);
		uv_stream_t* stream = this->ptr<uv_stream_t>();
		bool isIPC = stream->type == UV_NAMED_PIPE && 
			reinterpret_cast<uv_pipe_t*>(stream)->ipc;

		if (!isIPC) {
			r = uv_write(req, stream, &buf, 1, [](uv_write_t* req, int) {
				delete req;
			});
		}
		else {
			r = uv_write2(req, stream, &buf, 1, nullptr, [](uv_write_t* req, int) {
				delete req;
			});
		}

		if (r) {
			delete req;
			//setAndThrowError(r, "Stream write error");
		}
		return r == 0;
	}
	
	Buffer& buffer()
		// Returns the read buffer.
	{ 
		assertTID();
		return _buffer;
	}

	virtual bool closed() const
		// Returns true if the native socket handle is closed.
	{
		return uv::Handle::closed();
	}

	Signal2<const char*, int> Read;
		// Signals when data can be read from the stream.

 protected:	
	bool readStart()
	{
		//traceL("Stream", this) << "Read start: " << ptr() << std::endl;
		int r;
		uv_stream_t* stream = this->ptr<uv_stream_t>();
		bool isIPC = stream->type == UV_NAMED_PIPE && 
			reinterpret_cast<uv_pipe_t*>(stream)->ipc;

		if (isIPC)
			r = uv_read2_start(stream, Stream::allocReadBuffer, handleRead2);
		else 	
			r = uv_read_start(stream, Stream::allocReadBuffer, handleRead);
		if (r) setUVError("Stream read error", r);	
		return r == 0;
	}

	bool readStop()
	{		
		//traceL("Stream", this) << "Read stop: " << ptr() << std::endl;
		int r = uv_read_stop(ptr<uv_stream_t>());
		if (r) setUVError("Stream read error", r);
		return r == 0;
	}

	virtual void onRead(const char* data, int len)
	{
		//traceL("Stream", this) << "On read: " << len << std::endl;
		Read.emit(instance(), data, len);
	}

	static void handleReadCommon(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending) 
	{	
		auto io = static_cast<Stream*>(handle->data);
		//traceL("Stream", io) << "Handle read: " << nread << std::endl;
		
		if (nread >= 0) {
			// We only support UV_TCP right now
			if (pending == UV_TCP)
				assert(0);
			else
				assert(pending == UV_UNKNOWN_HANDLE);
			io->onRead(buf->base, nread);
		}
		else {
			//assert(nread == UV_EOF);
			// NOTE: Not setting error here since it's not
			// really an error, just close() directly.
			// This will result in handle destruction.
			assert(nread == UV_ECONNRESET || nread == UV_EOF);
			io->setUVError("Stream error", nread);
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
	
	static void handleRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) 
	{
		handleReadCommon(handle, nread, buf, UV_UNKNOWN_HANDLE);
	}
	
	static void handleRead2(uv_pipe_t* handle, ssize_t nread, const uv_buf_t* buf, uv_handle_type pending) 
	{
		handleReadCommon((uv_stream_t*)handle, nread, buf, pending);
	}
	
	static void allocReadBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t* buf)
	{
		auto self = static_cast<Stream*>(handle->data);

		// Reserve the recommended buffer size
		//if (suggested_size > self->_buffer.capacity())
		//	self->_buffer.capacity(suggested_size); 
		assert(self->_buffer.size() >= suggested_size);

		// Reset the buffer position on each read
		buf->base = self->_buffer.data();
		buf->len = self->_buffer.size();
	}

	Buffer _buffer;
};


} } // namespace scy::net


#endif // SCY_Net_Stream_H


	/*

	//virtual bool initialized() const
		// Returns true if the native socket handle is initialized.
	//{
	//	return uv::Handle::active();
	//}
	//Signal2<const char*, int> Write;
	void setStream(uv_stream_t* stream)
	{
		assert(_handle == nullptr);
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
		traceL("Stream", ptr) << "AllocReadBuffer " << suggested_size << std::endl;
		//ptr->_readBuffer.data.reserve(suggested_size); 
		//if (suggested_size > ptr->_readBuffer1.capacity())
		//	ptr->_readBuffer1.reserve(suggested_size);
		traceL("Stream", ptr) << "AllocReadBuffer: Before alloc: " << ptr->_buffer.available() << std::endl;
			traceL("Stream", ptr) << "AllocReadBuffer: DO alloc: " << std::endl;
		conn->data
		_readBuffer
		Connection *conn = (Connection*) handle;
		assert(conn->read_clear); // Ensure that the last _buffer has been read by on_read_cb.
		conn->read_clear = false;
		conn->read_buffer.reserve_more(suggested_size); 
		traceL("Stream", ptr) << "AllocReadBuffer: After alloc: " << ptr->_buffer.available() << std::endl;
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