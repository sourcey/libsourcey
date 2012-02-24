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


#ifndef SOURCEY_UV_UVPP_H
#define SOURCEY_UV_UVPP_H


#include "uv.h"
#include <string>

	
#define DefaultLoop uv_default_loop()
#define Run(loop) uv_run(loop)
#define RunDefaultLoop uv_run(uv_default_loop())


#define UVCallback(Name, Func, Handle)						\
															\
	static void _Func(Handle* handle) {						\
		static_cast<Name*>(handle->data)->Func();			\
    };														\


#define UVStatusCallback(Name, Func, Handle)				\
															\
	static void Func(Handle* handle, int status) {			\
		Name* self = static_cast<Name*>(handle->data);		\
		if (status)											\
			self->setErrno(uv_last_error(self->loop()));		\
		self->Func(status);									\
    }														\

#define UVStatusCallbackWithType(Name, Func, Handle)		\
															\
	static void Func(Handle* handle, int status) {			\
		Name* self = static_cast<Name*>(handle->data);		\
		if (status)											\
			self->setErrno(uv_last_error(self->loop()));		\
		self->Func(handle, status);							\
    }														\


namespace Sourcey {
namespace UV {


//
// Handle Types
//
typedef struct uv_loop_s			LoopHandle;
typedef struct uv_ares_task_s		AresTaskHandle;
typedef struct uv_err_s				ErrorHandle;
typedef struct uv_handle_s			Handle;
typedef struct uv_stream_s			StreamHandle;
typedef struct uv_tcp_s				TCPHandle;
typedef struct uv_udp_s				UDPHandle;
typedef struct uv_pipe_s			PipeHandle;
typedef struct uv_tty_s				TTYHandle;
typedef struct uv_timer_s			TimerHandle;
typedef struct uv_prepare_s			PrepareHandle;
typedef struct uv_check_s			CheckHandle;
typedef struct uv_idle_s			IdleHandle;
typedef struct uv_async_s			AsyncHandle;
typedef struct uv_getaddrinfo_s		GetaddrinfoHandle;
typedef struct uv_process_s			ProcessHandle;
typedef struct uv_counters_s		CountersHandle;

//
// Req Types
//
typedef struct uv_connect_s			ConnectReq;

//
// Other Types
//
typedef struct uv_buf_t				Buffer;


class Interface
{
public:
	Interface(LoopHandle* loop = DefaultLoop) : _loop(loop) {}
	virtual ~Interface() {};
	
	// virtual so implementations can provide
	// mutex protection or whatever...
	virtual void setErrno(const ErrorHandle& err) { _error = err; }
	virtual ErrorHandle error() { return _error; }
	virtual std::string errorMessage() { return uv_strerror(_error); }	
	virtual LoopHandle* loop() { return _loop; }

protected:	
	ErrorHandle	_error;
	LoopHandle*	_loop;
};


inline Buffer defaultAlloc(Handle* handle, size_t size)
{
	Buffer buf;
	buf.base = new char[size];
	buf.len = size;
	return buf;
}

	
inline void defaultClose(Handle* handle)
{
}


} } // namespace Sourcey::UV


#endif // SOURCEY_UV_UVPP_H
