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
#include "Sourcey/Signal.h"


namespace scy {
namespace UV {


class Stream: public UV::Base
{
 public:  
	Stream(uv_loop_t* loop = uv_default_loop());
	Stream(uv_stream_t* stream, uv_loop_t* loop = uv_default_loop());
	virtual ~Stream();

	int write(char* data, int size);
	int readStart();
	int readStop();
	
	int shutdown();
		/// Stream shutdown
	void close();
	
	void setStream(uv_stream_t* stream, void* ptr = NULL);
	uv_stream_t* stream() const;

	int writeQueueSize() const;
	
	virtual void onRead(const char* data, int len);
	virtual void onClose();

	Signal2<const char*, int> OnRead;
	Signal2<const char*, int> OnWrite;
	Signal<int> OnClose;
	
	virtual void setErrno(const uv_err_t& err);

 protected:  	
	static void handleRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf);
	static void handleRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
	static void handleReadCommon(uv_stream_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
	static void afterWrite(uv_write_t* req, int status);
	static void afterShutdown(uv_shutdown_t* req, int status);
	static void afterClose(uv_handle_t* peer);
		
 protected:
	std::unique_ptr<uv_stream_t> _stream;
};



} } // namespace scy::UV


#endif // SOURCEY_UV_Stream_H
