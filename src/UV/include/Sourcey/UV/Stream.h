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


#ifndef SOURCEY_UV_Stream_H
#define SOURCEY_UV_Stream_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Memory.h"
#include "Sourcey/UV/Base.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Buffer.h"


namespace scy {
namespace uv {


//template <class BaseT>
class Stream: public uv::Base<CountedObject>
{
 public:  
	Stream(uv_loop_t* loop = NULL, void* stream = NULL) :
	   uv::Base<CountedObject>(loop, stream)
	{
	}
	   
		 //closeOnError(true);//, public CountedObject
	//Stream(uv_stream_t* stream)
	//{
	//	setStream(stream);
	//}
	
	void close()
		/// Closes and resets the stream handle.
		/// This will close the active socket/pipe.
		///
		/// setStream() will need to be called again 
		/// before the stream can be reused.
		///
		/// If the stream is already closed this call
		/// will have no side-effects.
	{
		traceL("Stream", this) << "Close: " << handle() << std::endl;
		if (handle())
			readStop();		
		uv::Base<CountedObject>::close();
	}
	
	bool shutdown()
		/// Sends a shutdown packet to the connected peer.
	{
		traceL("Stream", this) << "Send Shutdown: " << handle() << std::endl;
		if (!handle())
			return false;

		// TODO: Sending shutdown causes an eof error to be  
		// returned via handleRead() which sets the stream 
		// to error state. Since this is not really an error,
		// it should be handled differently.
		int r = uv_shutdown(new uv_shutdown_t, handle<uv_stream_t>(), uv::afterShutdown);

		// If the stream is not connected this will return false.
		return r == 0;
	}

	bool write(const char* data, int len)
		/// Writes data to the stream.
	{
		uv_stream_t* stream = this->handle<uv_stream_t>();
		bool ipcPipe = stream->type == UV_NAMED_PIPE && ((uv_pipe_t*)stream)->ipc;
		int r;  
		uv_buf_t buf = uv_buf_init((char*)data, len);

		if (!ipcPipe)
			r = uv_write(new uv_write_t, stream, &buf, 1, uv::afterWrite);
		else
			r = uv_write2(new uv_write_t, stream, &buf, 1, NULL, uv::afterWrite);
		if (r)
			setLastError();
		return r == 0;
	}
	
	//uv_stream_t* handle<uv_stream_t>() const
	//{ 
	//	return reinterpret_cast<uv_stream_t*>(handle());
	//}

	Signal2<const char*, int> Read;
	//Signal2<const char*, int> Write;

 protected:	
	 /*
	void setStream(uv_stream_t* stream)
	{
		assert(_handle == NULL);
		stream->data = instance();
		_handle = (uv_handle_t*)stream;
	}
	*/

	bool readStart()
	{
		traceL("Stream", this) << "Read Start: " << handle() << std::endl;
		int r;
		uv_stream_t* stream = this->handle<uv_stream_t>();
		bool ipcPipe = stream->type == UV_NAMED_PIPE && reinterpret_cast<uv_pipe_t*>(stream)->ipc;
		if (ipcPipe)
			r = uv_read2_start(stream, Stream::allocRecvBuffer, handleRead2);
		else 	
			r = uv_read_start(stream, Stream::allocRecvBuffer, handleRead);
		if (r) 
			setLastError();	
		return r == 0;
	}

	bool readStop()
	{		
		traceL("Stream", this) << "Read Stop: " << handle() << std::endl;
		int r = uv_read_stop(handle<uv_stream_t>());
		if (r) setLastError();
		return r == 0;
	}

	virtual void onRead(const char* data, int len)
	{
		traceL("Stream", this) << "On Read: " << len << std::endl;

		// can be overridden
		Read.emit(instance(), data, len);
	}

	static void handleReadCommon(uv_stream_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending) 
	{	
		Stream* io = static_cast<Stream*>(handle->data);
		traceL("Stream", io) << "Handle Read: " << nread << std::endl;
	
		// Handle EOF or error
		if (nread == -1)  {
			io->setError(uv_last_error(io->loop()));
			return;
		}
		else {
			if (pending == UV_TCP) {
				assert(0);
			} else {
				// We only support sending UV_TCP right now.
				assert(pending == UV_UNKNOWN_HANDLE);
			}		
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
	// UV static callbacks
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

	static uv_buf_t allocRecvBuffer(uv_handle_t *handle, size_t suggested_size)
	{
		Stream* self = static_cast<Stream*>(handle->data);		
		if (suggested_size > self->_buffer.size())
			self->_buffer.reserve(suggested_size); 
		assert(self->_buffer.capacity() == suggested_size);
		self->_buffer.position(0);
		return uv_buf_init(self->_buffer.begin(), suggested_size);
	}

	scy::Buffer _buffer;
};



} } // namespace scy::uv


#endif // SOURCEY_UV_Stream_H


	

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
	scy::Buffer _readBuffer1;
		}
		traceL("Stream", ptr) << "allocRecvBuffer " << suggested_size << std::endl;
		//ptr->_readBuffer.data.reserve(suggested_size); 
		//if (suggested_size > ptr->_readBuffer1.capacity())
		//	ptr->_readBuffer1.reserve(suggested_size);
		traceL("Stream", ptr) << "allocRecvBuffer: Before alloc: " << ptr->_buffer.size() << std::endl;
			traceL("Stream", ptr) << "allocRecvBuffer: DO alloc: " << std::endl;
		conn->data
		_readBuffer
		Connection *conn = (Connection*) handle;
		assert(conn->read_clear); // Ensure that the last _buffer has been read by on_read_cb.
		conn->read_clear = false;
		conn->read_buffer.reserve_more(suggested_size); 
		traceL("Stream", ptr) << "allocRecvBuffer: After alloc: " << ptr->_buffer.size() << std::endl;
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
//<HandleT> //CountedObject

	//static void afterClose(uv_handle_t* peer);
	//std::unique_ptr<uv_stream_t> _stream;

	//void close();