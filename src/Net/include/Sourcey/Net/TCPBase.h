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


#ifndef SOURCEY_Net_TCPBase_H
#define SOURCEY_Net_TCPBase_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Net/Stream.h"
#include "Sourcey/Net/Socket.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Types.h"


namespace scy {
namespace net {


class TCPSocket; 
class TCPBase: public Stream, public net::SocketBase
{
public:	
	TCPBase(uv::Loop& loop = uv::defaultLoop()); 
	
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
			
	void setError(const Error& err);
	const Error& error() const;

	virtual bool closed() const;
		/// Returns true if the native socket 
		/// handle is closed.
	
	net::Address address() const;
		/// Returns the IP address and port number of the socket.
		
	net::Address peerAddress() const;
		/// Returns the IP address and port number of the peer socket.

	net::TransportType transport() const;
		/// Returns the TCP transport protocol.
	
	bool initialized() const;
		/// Returns true if the underlying socket is initialized.

	bool connected() const;
		/// Returns true if the underlying socket is connected.

	SOCKET sockfd() const;
		/// Returns the socket descriptor for the 
		/// underlying native socket.
	
#ifdef _WIN32
	void setSimultaneousAccepts(bool enable);
#endif
	
	Signal<const net::TCPSocket&> AcceptConnection;
	
public:
	virtual void onConnect(int status);
	virtual void onAcceptConnection(uv_stream_t* handle, int status);
	virtual void onRead(const char* data, int len);
	virtual void onRecv(Buffer& buf);
	virtual void onError(const Error& error);
	virtual void onClose();
		
protected:
	virtual ~TCPBase();
	virtual void init();

protected:
	uv_connect_t _connectReq;
	//std::string _debugOutput;
};


} } // namespace scy::net


#endif // SOURCEY_Net_TCPBase_H



	//virtual void* instance() { return this; }
	//bool _connected;