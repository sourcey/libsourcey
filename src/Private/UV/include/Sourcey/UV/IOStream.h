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


#ifndef SOURCEY_UV_IOStream_H
#define SOURCEY_UV_IOStream_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Signal.h"


namespace Sourcey {
namespace UV {


class IOStream: public UV::Interface
{
 public:  
	IOStream(LoopHandle* loop = DefaultLoop);
	IOStream(uv_stream_t* stream, LoopHandle* loop = DefaultLoop);
	virtual ~IOStream();

	int write(char* data, int size);
	int readStart();
	int readStop();
	int shutdown();
	
	void setStream(uv_stream_t* stream);
	uv_stream_t* stream();

	int writeQueueSize();

	Signal2<char*, int> OnRead;
	Signal2<char*, int> OnWrite;
	
	virtual void setErrno(const uv_err_t& err);

	LoopHandle*		_loop;
	uv_stream_t*	_stream;
	uv_err_t		_error;

 protected:  	
	static void handleRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf);
	static void handleRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
	static void handleReadCommon(uv_stream_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
	static void afterWrite(uv_write_t* req, int status);
	static void afterShutdown(uv_shutdown_t* req, int status);
	static void afterClose(Handle* peer);
	
	//static uv_buf_t defaultAlloc(Handle* handle, size_t suggested_size);
};


} } // namespace Sourcey::UV


#endif // SOURCEY_UV_IOStream_H
