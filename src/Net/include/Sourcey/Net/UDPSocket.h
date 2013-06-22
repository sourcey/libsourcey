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


#ifndef SOURCEY_NET_UDPPacketSocket_H
#define SOURCEY_NET_UDPPacketSocket_H


#include "Sourcey/Base.h"
#include "Sourcey/Net/AsyncPacketSocketBase.h"

#include "Poco/Net/DatagramSocket.h"


namespace scy {
namespace Net {


typedef AsyncPacketSocketBase<Poco::Net::DatagramSocket, UDP> UDPPacketSocket;


} } // namespace scy::Net


#endif // SOURCEY_NET_UDPPacketSocket_H



/*
class UDPPacketSocket: public IPacketSocket, public Poco::Net::DatagramSocket //, public PacketFactory, public PacketEmitter
	/// This class provides efficient UDP messaging for
	/// LibSourcey.
	///
	/// This implementation uses an asynchronous message queue
	/// for dispatching received packets and minimize packet
	/// loss. The message queue runs inside the Runner thread. 
{
public:
	UDPPacketSocket(Reactor& reactor, 
		      Runner& runner);
	UDPPacketSocket(const UDPPacketSocket& r);
	virtual ~UDPPacketSocket();

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

	virtual bool isConnected() const;
	virtual bool isError() const;	
	
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
*/