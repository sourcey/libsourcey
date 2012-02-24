//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/UV/IOStream.h"

#include "Sourcey/Logger.h"


using namespace std;
using namespace Sourcey;


namespace Sourcey {
namespace UV {
	


IOStream::IOStream(LoopHandle* loop) : 
	_stream(NULL), _loop(loop)
{
	//Log("debug") << "[IOStream:" << this << "] Creating" << endl;
}


IOStream::IOStream(uv_stream_t* stream, LoopHandle* loop) : 
	_stream(stream), _loop(loop)
{
	//Log("debug") << "[IOStream:" << this << "] Creating" << endl;

	if (_stream)
		_stream->data = this;
}

	
IOStream::~IOStream() 
{	
	Log("debug") << "[IOStream:" << this << "] Destroying" << endl;
	delete _stream;
}


int IOStream::readStart()
{
	//Log("debug") << "[IOStream:" << this << "] readStart" << endl;

	int r;
	bool ipc_pipe = _stream->type == UV_NAMED_PIPE &&
		((uv_pipe_t*)_stream)->ipc;
	if (ipc_pipe) {
		r = uv_read2_start(_stream, UV::defaultAlloc, IOStream::handleRead2);
	} 
	else {
		r = uv_read_start(_stream, UV::defaultAlloc, IOStream::handleRead);
	}

	if (r) 
		setErrno(uv_last_error(_loop));
	
	//Log("debug") << "[IOStream:" << this << "] readStart: OK" << endl;

	return r;
}


int IOStream::readStop() 
{
	int r = uv_read_stop(_stream);
	if (r) 
		setErrno(uv_last_error(_loop));

	return r;
}


int IOStream::write(char* data, int size) 
{
	bool ipc_pipe = _stream->type == UV_NAMED_PIPE && ((uv_pipe_t*)_stream)->ipc;
	int r;
  
	uv_write_t req;
	uv_buf_t buf = uv_buf_init((char *)data, size);

	if (!ipc_pipe) {
		r = uv_write(&req, _stream, &buf, 1, IOStream::afterWrite);
	} 
	else {
		r = uv_write2(&req, _stream, &buf, 1, NULL /* send_stream */, IOStream::afterWrite);
	}
  
	if (r)
		setErrno(uv_last_error(_loop));
  
    return r;
}


int IOStream::shutdown() 
{
	uv_shutdown_t req;

	int r = uv_shutdown(&req, _stream, IOStream::afterShutdown);
	if (r)
		setErrno(uv_last_error(_loop));
  
	return r;
}


void IOStream::setStream(uv_stream_t* stream) 
{
	_stream = stream;
	_stream->data = this;
}


void IOStream::setErrno(const uv_err_t& err)
{
	Log("error") << "[IOStream:" << this << "] Error: " << uv_strerror(err) << endl;	
	UV::Interface::setErrno(err);
}


int IOStream::writeQueueSize()
{
	return _stream->write_queue_size;
}

	
uv_stream_t* IOStream::stream() 
{ 
	return _stream; 
}


//
// Callbacks
//
void IOStream::handleReadCommon(uv_stream_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending) 
{	
	IOStream* io = static_cast<IOStream*>(handle->data);

	//Log("debug") << "[IOStream: " << io << "] Handle Read: " << nread << endl;
	
	// EOF or Error
	if (nread == -1)  {
		io->setErrno(uv_last_error(io->_loop));
		io->OnRead.dispatch(io, NULL, 0);
		return;
	}

	else {
		if (pending == UV_TCP) {
			assert(0);
		} else {
			// We only support sending UV_TCP right now.
			assert(pending == UV_UNKNOWN_HANDLE);
		}
		
		io->OnRead.dispatch(io, buf.base, nread);
	}
}


void IOStream::afterWrite(uv_write_t* req, int status) 
{
	//delete req;
}


void IOStream::handleRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf) 
{
  handleReadCommon(handle, nread, buf, UV_UNKNOWN_HANDLE);
}


void IOStream::handleRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf,
    uv_handle_type pending) 
{
	handleReadCommon((uv_stream_t*)handle, nread, buf, pending);
}


void IOStream::afterShutdown(uv_shutdown_t* req, int status) 
{	
	uv_close((Handle*)req->handle, afterClose);
}


void IOStream::afterClose(Handle* peer) 
{	
	free(peer);
}


} } // namespace Sourcey::UV