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


#ifndef SOURCEY_NET_UDPSocket_H
#define SOURCEY_NET_UDPSocket_H


#include "Sourcey/Base.h"
#include "Sourcey/PacketQueue.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/ISocket.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Reactor.h"

#include "Poco/Net/DatagramSocket.h"
#include "Poco/Mutex.h"


namespace Sourcey {
namespace Net {


class UDPSocket: public ISocket, public Poco::Net::DatagramSocket
	/// This class provides efficient UDP messaging for
	/// LibSourcey.
	///
	/// This implementation uses an asynchronous message queue
	/// for dispatching received packets and minimize packet
	/// loss. The message queue runs inside the Runner thread. 
{
public:
	UDPSocket(Reactor& reactor = Reactor::getDefault(), 
		      Runner& runner = Runner::getDefault());
	UDPSocket(const UDPSocket& r);
	virtual ~UDPSocket();

	virtual void close();	

	virtual void connect(const Address& address);
		/// Restricts incoming and outgoing
		/// packets to the specified address.
		///
		/// Cannot be used together with bind().

	virtual void bind(const Address& address, bool reuseAddress = false);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. 
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// Cannot be used together with connect().
		
	virtual int send(const char* data, int size);
	virtual int send(const char* data, int size, const Address& peerAddress);
	virtual int send(const DataPacket& packet);
	virtual int send(const DataPacket& packet, const Address& peerAddress);
	virtual int send(const IPacket& packet);
	virtual int send(const IPacket& packet, const Address& peerAddress);
	virtual void send(IPacket& packet);

	virtual bool isConnected();
	virtual bool isError();	
	
	Address address() const;
	Address peerAddress() const;
	TransportProtocol transport() const;
	void setError(const std::string& err);
	std::string error() const;
	int errorno() const;
	Reactor& reactor();
	Runner& runner();
	
protected:
	virtual void recv(Buffer& buffer, const Address& peerAddress);
	virtual void packetize(Buffer& buffer, const Address& peerAddress);

	virtual void onReadable();
	virtual void onError();

	virtual void bindEvents();	
	virtual void unbindEvents();	

	Reactor&		_reactor;
	Runner&			_runner;
	Buffer			_buffer;
	bool			_closed;
	PacketQueue*	_queue;
	std::string		_error;
	mutable Poco::FastMutex	_mutex;
};


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_UDPSocket_H