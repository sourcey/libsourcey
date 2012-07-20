//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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


#ifndef SOURCEY_NET_SSLSocket_H
#define SOURCEY_NET_SSLSocket_H


#include "Sourcey/Base.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/PacketSocketBase.h"

#include "Poco/Net/SecureStreamSocket.h"


namespace Sourcey {
namespace Net {


typedef SocketBase<Poco::Net::SecureStreamSocket, SSLTCP>        SSLSocket;
typedef PacketSocketBase<Poco::Net::SecureStreamSocket, SSLTCP>  SSLPacketSocket;


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_SSLSocket_H






	/*
//#include "Sourcey/Net/StatefulSocket.h"
//#include "Sourcey/Net/WebSocket.h"
*/


//class SSLSocket: public ISocket, public Poco::Net::StreamSocket
//	/// This class implements a generic TCP socket interface
//	/// over the top of Poco's StreamSocket.
//{
//public:
//	SSLSocket(Reactor& reactor/* = Reactor::getDefault()*/, bool deleteOnClose = false);
//	SSLSocket(const Poco::Net::StreamSocket& socket, Reactor& reactor/* = Reactor::getDefault()*/, bool deleteOnClose = false);
//	SSLSocket(const SSLSocket& r);
//	
//	virtual ~SSLSocket();
//	
//	virtual void connect(const Address& peerAddress);
//	virtual void connect(const Address& peerAddress, int timeout);
//		/// Connects to the given peer address.
//		///
//		/// A Poco::Net::ConnectionRefusedException or a
//		/// Poco::TimeoutException is thrown on failure.
//	
//	virtual void close();
//		/// Closes the underlying socket.
//		//
//		/// On a side note: When the StreamSocket's reference
//		/// count reaches 0 the underlying socket will be
//		/// automatically closed.
//
//	virtual int send(const char* data, int size);
//	virtual int send(const char* data, int size, const Address& peerAddress);
//	virtual int send(const DataPacket& packet);
//	virtual int send(const DataPacket& packet, const Address& peerAddress);
//	virtual int send(const IPacket& packet);
//	virtual int send(const IPacket& packet, const Address& peerAddress);
//	virtual void send(IPacket& packet);
//
//	virtual void setError(const std::string& err);
//		/// Sets the error message and closes the socket.
//	
//	virtual bool isConnected();
//	virtual bool isError();
//
//	virtual void deleteOnClose(bool flag);
//		/// When true the socket instance will automatically delete 
//		/// itself after a call to close().
//
//	TransportProtocol transport() const;
//	Address address() const;
//	Address peerAddress() const;
//	std::string error() const;
//	int errorno() const;
//	Reactor& reactor();
//
//	virtual void bindEvents();	
//	virtual void unbindEvents();
//	
//protected:		
//	virtual void recv(Buffer& buffer);
//	virtual void packetize(Buffer& buffer);
//
//	virtual void onReadable();
//	virtual void onConnect();
//	virtual void onClose();
//	virtual void onError();
//		
//	virtual void resetBuffer();
//	
//protected:
//	mutable Poco::FastMutex _mutex;
//	Buffer					_buffer;
//	Reactor&				_reactor;
//	std::string				_error;
//	bool					_connected;
//	bool					_deleteOnClose;
//};