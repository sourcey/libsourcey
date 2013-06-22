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


#include "Sourcey/UV/Stream.h"

#include "Sourcey/Logger.h"


using namespace std;
using namespace scy;


namespace scy {
namespace UV {
	


Stream::Stream(uv_loop_t* loop) : 
	Base(loop) //_stream(NULL), 
{
	Log("Stream", this) << "Creating" << endl;
}


Stream::Stream(uv_stream_t* stream, uv_loop_t* loop) : 
	Base(loop) //_stream(stream), 
{
	Log("Stream", this) << "Creating" << endl;
	setStream(stream);
	//if (_stream)
	//	_stream->data = this;
}

	
Stream::~Stream() 
{	
	LogTrace("Stream", this) << "Destroying" << endl;
	//delete _stream;
}


int Stream::readStart()
{
	Log("Stream", this) << "readStart" << endl;

	int r;
	uv_stream_t* stream = this->stream();
	bool ipcPipe = stream->type == UV_NAMED_PIPE &&
					reinterpret_cast<uv_pipe_t*>(stream)->ipc;
	if (ipcPipe)
		r = uv_read2_start(stream, UV::defaultAlloc, Stream::handleRead2);
	else 	
		r = uv_read_start(stream, UV::defaultAlloc, Stream::handleRead);

	if (r) 
		setErrno(uv_last_error(_loop));
	
	Log("Stream", this) << "readStart: OK" << endl;
	return r;
}


int Stream::readStop() 
{
	int r = uv_read_stop(stream());
	if (r) setErrno(uv_last_error(_loop));
	return r;
}


int Stream::write(char* data, int size) 
{
	uv_stream_t* stream = this->stream();
	bool ipcPipe = stream->type == UV_NAMED_PIPE && ((uv_pipe_t*)stream)->ipc;
	int r;
  
	uv_write_t req;
	uv_buf_t buf = uv_buf_init((char *)data, size);

	if (!ipcPipe)
		r = uv_write(&req, stream, &buf, 1, Stream::afterWrite);
	else
		r = uv_write2(&req, stream, &buf, 1, NULL /* send_stream */, Stream::afterWrite);  

	if (r)
		setErrno(uv_last_error(_loop));

    return r;
}


void Stream::close() 
{	
	Log("Stream", this) << "Send Close" << endl;
    uv_close((uv_handle_t*)stream(), Stream::afterClose);
	onClose();
}


int Stream::shutdown() 
{
	assert(stream());
	
	Log("Stream", this) << "Send Shutdown" << endl;
	//uv_shutdown_t req;
	//int r = uv_shutdown(&req, stream(), Stream::afterShutdown);
	int r = uv_shutdown(new uv_shutdown_t, stream(), Stream::afterShutdown);
	// XXX: Skipping error here, since shutdown mostly gets called on destruction.
	// Classes can check the return value to dtermine success;
	if (r) setErrno(uv_last_error(_loop));  
	return r;
}


void Stream::onClose()
{
	Log("Stream", this) << "On Close" << endl;	
	OnClose.emit(this, error().code);
}


void Stream::onRead(const char* data, int len)
{
	Log("Stream", this) << "On Data: " << len << endl;

	// can be overridden
	OnRead.emit(this, data, len);
}


void Stream::setErrno(const uv_err_t& err)
{
	LogError("Stream", this) << "Set Error: " << uv_strerror(err) << endl;	
	UV::Base::setErrno(err);
}


void Stream::setStream(uv_stream_t* stream, void* ptr) 
{
	stream->data = this;
	_stream.reset(stream);
	
	//if ()ptr ? ptr : 
	//stream->data = ptr;
	//assert(_stream->data == NULL ||
	//	static_cast<Stream*>(_stream->data) == this);
	//_stream->data = this;
}

	
uv_stream_t* Stream::stream() const
{ 
	return _stream.get(); 
}


int Stream::writeQueueSize() const
{
	return _stream->write_queue_size;
}


//
// Callbacks
//

void Stream::handleReadCommon(uv_stream_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending) 
{	
	Stream* io = static_cast<Stream*>(handle->data);
	Log("Stream", io) << "Handle Read: " << nread << endl;
	
	// EOF or Error
	if (nread == -1)  {
		io->setErrno(uv_last_error(io->_loop));
		io->OnRead.emit(io, NULL, 0);
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


void Stream::afterWrite(uv_write_t* req, int status) 
{
	//delete req;
}


void Stream::handleRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf) 
{
	handleReadCommon(handle, nread, buf, UV_UNKNOWN_HANDLE);
}


void Stream::handleRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf,
    uv_handle_type pending) 
{
	handleReadCommon((uv_stream_t*)handle, nread, buf, pending);
}


void Stream::afterShutdown(uv_shutdown_t* req, int status) 
{	
	LogTrace("Stream") << "Shutdown" << endl;
	//uv_close((uv_handle_t*)req->handle, afterClose);
	Stream* io = static_cast<Stream*>(req->data);
	io->close();
}


void Stream::afterClose(uv_handle_t* peer) 
{	
	LogTrace("Stream") << "Closing: " << peer << endl;
	// NOTE: Sending OnClose from close() method as afterClose
	// does not always fire depending on server response.
	//io->OnClose.emit(io, io->error().code);
	Stream* io = static_cast<Stream*>(peer->data);
	assert((uv_handle_t*)io->stream() == peer);
	delete peer;
	//::free(peer);
}


} } // namespace scy::UV