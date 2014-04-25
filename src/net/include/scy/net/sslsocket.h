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


#ifndef SCY_Net_SSLSocket_H
#define SCY_Net_SSLSocket_H


#include "scy/uv/uvpp.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/socket.h"
#include "scy/net/ssladapter.h"
#include "scy/net/sslcontext.h"
#include "scy/net/sslsession.h"


namespace scy {
namespace net {


class SSLSocket: public TCPSocket	
{
public:	
	typedef std::shared_ptr<SSLSocket> Ptr;
	typedef std::vector<Ptr> Vec;

	SSLSocket(uv::Loop* loop = uv::defaultLoop());
	SSLSocket(SSLContext::Ptr sslContext, uv::Loop* loop = uv::defaultLoop());
	SSLSocket(SSLContext::Ptr sslContext, SSLSession::Ptr session, uv::Loop* loop = uv::defaultLoop());
	
	virtual ~SSLSocket();

	//virtual void connect(const Address& peerAddress);	
		// Initializes the socket and establishes a secure connection to 
		// the TCP server at the given address.
		//
		// The SSL handshake is performed when the socket is connected.	
	
	virtual bool shutdown();

	virtual void close();
		// Closes the socket.
		//
		// Shuts down the connection by attempting
		// an orderly SSL shutdown, then actually
		// shutting down the TCP connection.
	
	virtual int send(const char* data, std::size_t len, int flags = 0);
	virtual int send(const char* data, std::size_t len, const net::Address& peerAddress, int flags = 0);
		
	int available() const;
		// Returns the number of bytes available from the
		// SSL buffer for immediate reading.
	
	X509* peerCertificate() const;
		// Returns the peer's certificate.
		
	SSLContext::Ptr context() const;
		// Returns the SSL context used for this socket.
			
	SSLSession::Ptr currentSession();
		// Returns the SSL session of the current connection,
		// for reuse in a future connection (if session caching
		// is enabled).
		//
		// If no connection is established, returns nullptr.
		
	void useSession(SSLSession::Ptr session);
		// Sets the SSL session to use for the next
		// connection. Setting a previously saved Session
		// object is necessary to enable session caching.
		//
		// To remove the currently set session, a nullptr pointer
		// can be given.
		//
		// Must be called before connect() to be effective.
		
	bool sessionWasReused();
		// Returns true if a reused session was negotiated during
		// the handshake.

	net::TransportType transport() const;

	virtual void onConnect(uv_connect_t* handle, int status);

	virtual void onRead(const char* data, std::size_t len);
		// Reads raw encrypted SSL data

protected:
	//virtual void* self() { return this; }

	net::SSLContext::Ptr _context;
	net::SSLSession::Ptr _session;
	net::SSLAdapter _sslAdapter;

	friend class net::SSLAdapter;
};


#if 0
class SSLSocket: public Socket
	/// SSLSocket is a disposable SSL socket wrapper
	/// for SSLSocket which can be created on the stack.
	/// See SSLSocket for implementation details.
{
public:	
	typedef net::SSLSocket Base;
	typedef std::vector<SSLSocket> List;
	
	SSLSocket(uv::Loop* loop = uv::defaultLoop());
		// Creates an unconnected SSL socket.

	SSLSocket(SSLContext::Ptr sslContext, uv::Loop* loop = uv::defaultLoop());
	SSLSocket(SSLContext::Ptr sslContext, SSLSession::Ptr session, uv::Loop* loop = uv::defaultLoop());

	SSLSocket(SSLSocket* base, bool shared = false);
		// Creates the Socket and attaches the given Socket.
		//
		// The Socket must be a SSLSocket, otherwise an
		// exception will be thrown.

	SSLSocket(const Socket& socket);
		// Creates the SSLSocket with the Socket
		// from another socket. The Socket must be
		// a SSLSocket, otherwise an exception will be thrown.
	
	SSLSocket& base() const;
		// Returns the Socket for this socket.
};
#endif


} } // namespace scy::net


#endif // SCY_Net_SSLSocket_H