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


#ifndef SOURCEY_UV_TCPSocket_H
#define SOURCEY_UV_TCPSocket_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/Stream.h"
#include "Sourcey/UV/SSLBuffer.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Types.h"


#include "Poco/Net/NetException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"


namespace scy {
namespace UV {


	
//template <class ManagedT>
class Collectable {
public:
	Collectable() 
		/// Creates the RefCountedObject.
		/// The initial reference count is one.
	{
		count = 1;
	};
	
	void duplicate()
		/// Increments the object's reference count.
	{
		++count;
		LogTrace("Collectable", this) << "Duplicating: " << count << std::endl;
	}
		
	void release()
		/// Decrements the object's reference count
		/// and deletes the object if the count
		/// reaches zero.
	{
		--count;
		LogTrace("Collectable", this) << "Releasing: " << count << std::endl;
			
		if (count == 0)
			delete this; // TODO: Send to GC
	}
		
	int referenceCount() const
		/// Returns the reference count.
	{
		return count;
	}

protected:
	virtual ~Collectable()
		/// Destroys the RefCountedObject.
	{
	}

protected:
	Collectable(const Collectable&) {};
	Collectable& operator = (const Collectable&) {};
	
	int count; // TODO: sync
};




template <class SocketT>
class SocketPtr : public SocketT {
	SocketT* _ptr;

public:
	SocketPtr() :
		_ptr(new SocketT) 
	{
	}

	SocketPtr(const SocketPtr& ptr) :
		_ptr(ptr._ptr) 
	{
		LogTrace("SocketPtr", this) << "Creating from ptr: " << _ptr << std::endl;
		_ptr->duplicate();
	}

	SocketPtr(SocketT* socket) :
		_ptr(socket) 
	{
		LogTrace("SocketPtr", this) << "Creating from socket: " << socket << std::endl;
		_ptr->duplicate();
	}
		
	~SocketPtr()
	{
		LogTrace("SocketPtr", this) << "Destroying" << std::endl;
		_ptr->release();
	}

	SocketPtr& operator = (const SocketPtr& ptr)
	{
		LogTrace("SocketPtr", this) << "Creating from assign ptr: " << sock << std::endl;
		if (&socket != this)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr._ptr;
			if (_ptr) _ptr->duplicate();
		}
		return *this;
	}

	bool operator == (const SocketPtr& socket) const
	{
		return _ptr == socket._ptr;
	}
};


class TCPSocket: public Stream, public Collectable
{
public:
	TCPSocket(uv_loop_t* loop = uv_default_loop());
	virtual ~TCPSocket();
	
	virtual void acceptConnection();
	
	virtual void connect(const Net::Address& peerAddress);
	virtual void close();

	virtual int send(const char* data, int size);
	
	virtual void setNoDelay(bool enable);
	virtual void setKeepAlive(int enable, unsigned int delay);
	virtual bool bind(const Net::Address& address);
	virtual bool bind6(const Net::Address& address);
	virtual bool listen(int backlog = 64);	
	
	virtual Net::Address address();
		/// Returns the IP address and port number of the socket.
		
	virtual Net::Address peerAddress();
		/// Returns the IP address and port number of the peer socket.
	
	bool initialized() const;
		/// Returns true if the underlying socket is initialized.

	SOCKET sockfd() const;
		/// Returns the socket descriptor for the 
		/// underlying native socket.
	
#ifdef _WIN32
	virtual void setSimultaneousAccepts(bool enable);
#endif
	
	virtual void onConnected(int status);
	virtual void onRead(const char* data, int len);
	virtual void onRecv(const char* data, int len);
	virtual void onClose();
	virtual void onAcceptConnection(uv_stream_t* handle, int status);
	
	typedef SocketPtr<TCPSocket> Ptr;
	
	Signal<int> OnConnected;
	Signal2<const char*, int> OnRecv;
	Signal<Ptr&> OnAcceptConnection; //, bool&


//protected:
	//int refCount;
	uv_write_t   write_req;
	uv_connect_t connect_req; // testing
};





//
// UV Callbacks
//

UVStatusCallback(TCPSocket, onConnected, uv_connect_t);
UVStatusCallbackWithType(TCPSocket, onAcceptConnection, uv_stream_t);


//
// Poco stuff
//

inline SOCKET TCPSocket::sockfd() const
{
	return reinterpret_cast<uv_tcp_t*>(stream())->socket;
}


inline bool TCPSocket::initialized() const
{
	return sockfd() != POCO_INVALID_SOCKET;
}


} } // namespace scy::UV


#endif // SOURCEY_UV_TCPSocket_H


	
	/*
	//virtual void onRead(uv_stream_t* tcp, ssize_t nread, uv_buf_t buf);
	int socketError();
		/// Returns the value of the SO_ERROR socket option.
		
	static int lastError();
		/// Returns the last error code.

	static void error();
		/// Throws an appropriate exception for the last error.

	static void error(const std::string& arg);
		/// Throws an appropriate exception for the last error.

	static void error(int code);
		/// Throws an appropriate exception for the given error code.
		
	static void error(int code, const std::string& arg);
		/// Throws an appropriate exception for the given error code.
		*/
/*
inline int TCPSocket::lastError()
{
#if defined(_WIN32)
	return WSAGetLastError();
#else
	return errno;
#endif
}

inline void TCPSocket::error()
{
	int err = lastError();
	std::string empty;
	error(err, empty);
}


inline void TCPSocket::error(const std::string& arg)
{
	error(lastError(), arg);
}


inline void TCPSocket::error(int code)
{
	std::string arg;
	error(code, arg);
}


inline void TCPSocket::error(int code, const std::string& arg)
{
	switch (code)
	{
	case POCO_ESYSNOTREADY:
		throw Poco::Net::NetException("Net subsystem not ready", code);
	case POCO_ENOTINIT:
		throw Poco::Net::NetException("Net subsystem not initialized", code);
	case POCO_EINTR:
		throw Poco::IOException("Interrupted", code);
	case POCO_EACCES:
		throw Poco::IOException("Permission denied", code);
	case POCO_EFAULT:
		throw Poco::IOException("Bad address", code);
	case POCO_EINVAL:
		throw Poco::InvalidArgumentException(code);
	case POCO_EMFILE:
		throw Poco::IOException("Too many open files", code);
	case POCO_EWOULDBLOCK:
		throw Poco::IOException("Operation would block", code);
	case POCO_EINPROGRESS:
		throw Poco::IOException("Operation now in progress", code);
	case POCO_EALREADY:
		throw Poco::IOException("Operation already in progress", code);
	case POCO_ENOTSOCK:
		throw Poco::IOException("Socket operation attempted on non-socket", code);
	case POCO_EDESTADDRREQ:
		throw Poco::Net::NetException("Destination address required", code);
	case POCO_EMSGSIZE:
		throw Poco::Net::NetException("Message too long", code);
	case POCO_EPROTOTYPE:
		throw Poco::Net:: NetException("Wrong protocol type", code);
	case POCO_ENOPROTOOPT:
		throw Poco::Net::NetException("Protocol not available", code);
	case POCO_EPROTONOSUPPORT:
		throw Poco::Net::NetException("Protocol not supported", code);
	case POCO_ESOCKTNOSUPPORT:
		throw Poco::Net::NetException("Socket type not supported", code);
	case POCO_ENOTSUP:
		throw Poco::Net::NetException("Operation not supported", code);
	case POCO_EPFNOSUPPORT:
		throw Poco::Net::NetException("Protocol family not supported", code);
	case POCO_EAFNOSUPPORT:
		throw Poco::Net::NetException("Address family not supported", code);
	case POCO_EADDRINUSE:
		throw Poco::Net::NetException("Address already in use", arg, code);
	case POCO_EADDRNOTAVAIL:
		throw Poco::Net::NetException("Cannot assign requested address", arg, code);
	case POCO_ENETDOWN:
		throw Poco::Net::NetException("Network is down", code);
	case POCO_ENETUNREACH:
		throw Poco::Net::NetException("Network is unreachable", code);
	case POCO_ENETRESET:
		throw Poco::Net::NetException("Network dropped connection on reset", code);
	case POCO_ECONNABORTED:
		throw Poco::Net::ConnectionAbortedException(code);
	case POCO_ECONNRESET:
		throw Poco::Net::ConnectionResetException(code);
	case POCO_ENOBUFS:
		throw Poco::IOException("No buffer space available", code);
	case POCO_EISCONN:
		throw Poco::Net::NetException("Socket is already connected", code);
	case POCO_ENOTCONN:
		throw Poco::Net::NetException("Socket is not connected", code);
	case POCO_ESHUTDOWN:
		throw Poco::Net::NetException("Cannot send after socket shutdown", code);
	case POCO_ETIMEDOUT:
		throw Poco::TimeoutException(code);
	case POCO_ECONNREFUSED:
		throw Poco::Net::ConnectionRefusedException(arg, code);
	case POCO_EHOSTDOWN:
		throw Poco::Net::NetException("Host is down", arg, code);
	case POCO_EHOSTUNREACH:
		throw Poco::Net::NetException("No route to host", arg, code);
	default:
		throw Poco::IOException(Poco::NumberFormatter::format(code), arg, code);
	}
}
*/



//UVReadCallback(TCPSocket, onRead, uv_stream_t);
	//uv_tcp_t	 socket;	
 //((uv_tcp_t*)_stream)->socket; //_sockfd;
	//uv_tcp_t*		_tcp;
	//uv_err_t		_error;
	//uv_loop_t*		_loop;
	//static void afterClose(uv_handle_t* handle);
	//static void afterRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf); 
	//static void afterWrite(uv_write_t* req, int status); 

	//virtual void setErrno(const uv_err_t& err);
	//virtual bool setState(unsigned int id, const std::string& message = "");
	//static uv_buf_t defaultAlloc(uv_handle_t* handle, size_t size); 