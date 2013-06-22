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


#ifndef SOURCEY_UV_SSLSocket_H
#define SOURCEY_UV_SSLSocket_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/TCPSocket.h"

#include "Poco/Net/SSLException.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/Session.h"
/*
#include "Sourcey/UV/SSLBuffer.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Types.h"


#include "Poco/Net/NetException.h"
#include "Poco/Net/SSLException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"
*/


namespace scy {
namespace UV {


class SSLSocket: public TCPSocket
{
public:	
	SSLSocket(uv_loop_t* loop = uv_default_loop());
	SSLSocket(Poco::Net::Context::Ptr sslContext, uv_loop_t* loop = uv_default_loop());
	SSLSocket(Poco::Net::Context::Ptr sslContext, Poco::Net::Session::Ptr pSession, uv_loop_t* loop = uv_default_loop());

	virtual ~SSLSocket();
		/// Destroys the SSLSocket.
	
	virtual void connect(const Net::Address& peerAddress);	
		/// Initializes the socket and establishes a secure connection to 
		/// the TCP server at the given address.
		///
		/// The SSL handshake is performed the first time sendBytes(), 
		/// receiveBytes() or completeHandshake() is called.	

	virtual void close();
		/// Closes the socket.
		///
		/// Shuts down the connection by attempting
		/// an orderly SSL shutdown, then actually
		/// shutting down the TCP connection.
	
	virtual int send(const char* data, int size);
		
	int available() const;
		/// Returns the number of bytes available from the
		/// SSL buffer for immediate reading.
	
	X509* peerCertificate() const;
		/// Returns the peer's certificate.
		
	Poco::Net::Context::Ptr context() const;
		/// Returns the SSL context used for this socket.
			
	Poco::Net::Session::Ptr currentSession();
		/// Returns the SSL session of the current connection,
		/// for reuse in a future connection (if session caching
		/// is enabled).
		///
		/// If no connection is established, returns null.
		
	void useSession(Poco::Net::Session::Ptr pSession);
		/// Sets the SSL session to use for the next
		/// connection. Setting a previously saved Session
		/// object is necessary to enable session caching.
		///
		/// To remove the currently set session, a null pointer
		/// can be given.
		///
		/// Must be called before connect() to be effective.
		
	bool sessionWasReused();
		/// Returns true if a reused session was negotiated during
		/// the handshake.

	virtual void onConnected(int status);
	virtual void onRead(const char* data, int len);
	virtual void onRecv(const char* data, int len);
	
//protected:
	UV::SSLBuffer _sslBuffer;
	Poco::Net::Context::Ptr _context;
	Poco::Net::Session::Ptr _session;
	//SSL_CTX* _sslCtx;
	//SSL* _ssl;
};


//
// UV Callbacks
//

//UVStatusCallback(SSLSocket, onConnected, uv_connect_t);
//UVReadCallback(SSLSocket, onRead, uv_stream_t);


} } // namespace scy::UV


#endif // SOURCEY_UV_TCPSocket_H


	//virtual void onSSLAcceptConnection(uv_stream_t* handle, int status);
	/*	
	//virtual void onRead(uv_stream_t* tcp, ssize_t nread, uv_buf_t buf);

	//virtual void onRecv(const char* data, int len);
	virtual void setNoDelay(bool enable);
	virtual void setKeepAlive(int enable, unsigned int delay);
	virtual bool bind(const Net::Address& address);
	virtual bool bind6(const Net::Address& address);
	virtual bool listen(int backlog = 64);	
	
	
#ifdef _WIN32
	virtual void setSimultaneousAccepts(bool enable);
#endif
	
	Signal<int> OnConnected;
	Signal2<const char*, int> OnRecv;
	Signal2<TCPSocket*, bool&> OnAcceptConnection;
	*/

//UVStatusCallbackWithType(SSLSocket, onSSLAcceptConnection, uv_stream_t);

	//uv_tcp_t	 socket;	
	//uv_write_t   write_req;
	//uv_connect_t connect_req; // testing

	//uv_tcp_t*		_tcp;
	//uv_err_t		_error;
	//uv_loop_t*		_loop;
	//static void afterClose(uv_handle_t* handle);
	//static void afterRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf); 
	//static void afterWrite(uv_write_t* req, int status); 

	//virtual void setErrno(const uv_err_t& err);
	//virtual bool setState(unsigned int id, const std::string& message = "");
	//static uv_buf_t defaultAlloc(uv_handle_t* handle, size_t size); 