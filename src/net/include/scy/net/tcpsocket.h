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


#ifndef SCY_Net_TCPSocket_H
#define SCY_Net_TCPSocket_H


#include "scy/uv/uvpp.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/socket.h"
#include "scy/net/stream.h"
#include "scy/net/address.h"
#include "scy/net/types.h"


namespace scy {
namespace net {

	
class TCPBase; 


class TCPSocket: public net::Socket
	/// TCPSocket is a disposable TCP socket wrapper
	/// for TCPBase which can be created on the stack.
	/// See TCPBase for implementation details.
{
public:	
	typedef TCPBase Base;
	typedef std::vector<TCPSocket> List;
	
	TCPSocket(uv::Loop* loop = uv::defaultLoop());
		// Creates an unconnected TCP socket.

	TCPSocket(TCPBase* base, bool shared);
		// Creates the Socket and attaches the given SocketBase.
		//
		// The SocketBase must be a TCPBase, otherwise an
		// exception will be thrown.

	TCPSocket(const Socket& socket);
		// Creates the UDPSocket with the SocketBase
		// from another socket. The SocketBase must be
		// a UDPBase, otherwise an exception will be thrown.
	
	TCPBase& base() const;
		// Returns the SocketBase for this socket.
};


//
// TCP Base
//


class TCPBase: public Stream, public net::SocketBase
{
public:	
	TCPBase(uv::Loop* loop = uv::defaultLoop()); 
	
	virtual bool shutdown();
	virtual void close();
	
	virtual void connect(const net::Address& peerAddress);

	virtual int send(const char* data, int len, int flags = 0);
	virtual int send(const char* data, int len, const net::Address& peerAddress, int flags = 0);
	
	virtual void bind(const net::Address& address, unsigned flags = 0);
	virtual void listen(int backlog = 64);	
	
	virtual void acceptConnection();

	virtual void setNoDelay(bool enable);
	virtual void setKeepAlive(int enable, unsigned int delay);

	virtual uv::Loop* loop() const;
			
	void setError(const Error& err);
	const Error& error() const;
	
	virtual bool closed() const;
		// Returns true if the native socket handle is closed.
	
	net::Address address() const;
		// Returns the IP address and port number of the socket.
		// A wildcard address is returned if the socket is not connected.
		
	net::Address peerAddress() const;
		// Returns the IP address and port number of the peer socket.
		// A wildcard address is returned if the socket is not connected.

	net::TransportType transport() const;
		// Returns the TCP transport protocol.
	
#ifdef _WIN32
	void setSimultaneousAccepts(bool enable);
#endif
	
	Signal<const net::TCPSocket&> AcceptConnection;
	
public:
	virtual void onConnect(int status);
	virtual void onAcceptConnection(uv_stream_t* handle, int status);
	virtual void onRead(const char* data, int len);
	virtual void onRecv(const MutableBuffer& buf);
	virtual void onError(const Error& error);
	virtual void onClose();
		
protected:
	virtual ~TCPBase();
	virtual void init();

protected:
	std::unique_ptr<uv_connect_t> _connectReq;
};


} } // namespace scy::net


#endif // SCY_Net_TCPSocket_H



	//bool connected() const;
		// Returns true if the underlying socket is connected.
	
	//bool initialized() const;
		// Returns true if the underlying socket is initialized.

	//SOCKET sockfd() const;
		// Returns the socket descriptor for the 
		// underlying native socket.