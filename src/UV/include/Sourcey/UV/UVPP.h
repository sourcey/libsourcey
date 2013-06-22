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


#ifndef SOURCEY_UV_UVPP_H
#define SOURCEY_UV_UVPP_H


#include "uv.h"
#include <string>


#define RunDefaultLoop uv_run(uv_default_loop(), UV_RUN_DEFAULT)
#define StopDefaultLoop uv_stop(uv_default_loop())


#define UVCallback(ClassName, Function, Handle)						\
															\
	static void _Function(Handle* handle) {						\
		static_cast<ClassName*>(handle->data)->Function();			\
    };														\


#define UVStatusCallback(ClassName, Function, Handle)				\
															\
	static void Function(Handle* handle, int status) {			\
		ClassName* self = static_cast<ClassName*>(handle->data);		\
		if (status)											\
			self->setErrno(uv_last_error(self->loop()));	\
		self->Function(status);									\
    }														\


#define UVReadCallback(ClassName, Function, Handle)				    \
															\
	static void Function(Handle* handle, ssize_t nread, uv_buf_t buf) {			\
		ClassName* self = static_cast<ClassName*>(handle->data);		\
		if (nread == -1)											\
			self->setErrno(uv_last_error(self->loop()));	\
		self->Function(handle, nread, buf);									\
    }														\


#define UVStatusCallbackWithType(ClassName, Function, Handle)		\
															\
	static void Function(Handle* handle, int status) {			\
		ClassName* self = static_cast<ClassName*>(handle->data);		\
		if (status)											\
			self->setErrno(uv_last_error(self->loop()));	\
		self->Function(handle, status);							\
    }														\


namespace scy {
namespace UV {


//
// Handle Types
//
/*
typedef struct uv_loop_s			uv_loop_t;
typedef struct uv_ares_task_s		AresTaskHandle;
typedef struct uv_err_s				uv_err_t;
typedef struct uv_handle_s			Handle;
typedef struct uv_stream_s			uv_stream_t;
typedef struct uv_tcp_s				TCPHandle;
typedef struct uv_udp_s				UDPHandle;
typedef struct uv_pipe_s			PipeHandle;
typedef struct uv_tty_s				TTYHandle;
typedef struct uv_timer_s			uv_timer_t;
typedef struct uv_prepare_s			PrepareHandle;
typedef struct uv_check_s			CheckHandle;
typedef struct uv_idle_s			IdleHandle;
typedef struct uv_async_s			AsyncHandle;
typedef struct uv_getaddrinfo_s		GetaddrinfoHandle;
typedef struct uv_process_s			ProcessHandle;
typedef struct uv_counters_s		CountersHandle;
*/

//
// Req Types
//
//typedef struct uv_connect_t			ConnectReq;

//
// Other Types
//
//typedef struct uv_buf_t				Buffer;


class Base
{
public:
	Base(uv_loop_t* loop = uv_default_loop()) : _loop(loop) {}
	virtual ~Base() {};
	
	// virtual so implementations can provide
	// mutex protection or whatever...
	virtual void setErrno(const uv_err_t& err) { _error = err; }
	virtual uv_err_t error() { return _error; }
	virtual std::string errorMessage() { return uv_strerror(_error); }	
	virtual uv_loop_t* loop() { return _loop; }

protected:	
	uv_err_t	_error;
	uv_loop_t*	_loop;
};


inline uv_buf_t defaultAlloc(uv_handle_t* handle, size_t size)
{
	uv_buf_t buf;
	buf.base = new char[size];
	buf.len = size;
	return buf;
}

	
inline void defaultClose(uv_handle_t* handle)
{
}


//
// Socket Addresses
//

typedef sockaddr_in ip4_addr;
typedef sockaddr_in6 ip6_addr;

inline ip4_addr to_ip4_addr(const std::string& ip, int port) { return uv_ip4_addr(ip.c_str(), port); }
inline ip6_addr to_ip6_addr(const std::string& ip, int port) { return uv_ip6_addr(ip.c_str(), port); }

inline bool from_ip4_addr(ip4_addr* src, std::string& ip, int& port)
{
	char dest[16];
	if(uv_ip4_name(src, dest, 16) == 0)
	{
		ip = dest;
		port = static_cast<int>(ntohs(src->sin_port));
		return true;
	}
	return false;
}

inline bool from_ip6_addr(ip6_addr* src, std::string& ip, int& port)
{
	char dest[46];
	if(uv_ip6_name(src, dest, 46) == 0)
	{
		ip = dest;
		port = static_cast<int>(ntohs(src->sin6_port));
		return true;
	}
	return false;
}


} } // namespace scy::UV


#endif // SOURCEY_UV_UVPP_H
