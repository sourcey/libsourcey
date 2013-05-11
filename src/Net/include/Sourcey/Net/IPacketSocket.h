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


#ifndef SOURCEY_NET_IPacketSocket_H
#define SOURCEY_NET_IPacketSocket_H


#include "Sourcey/Base.h"
#include "Sourcey/PacketFactory.h"
#include "Sourcey/PacketEmitter.h"
#include "Sourcey/Net/ISocket.h"


namespace Scy {
namespace Net {


class IPacketSocket: public ISocket, public PacketEmitter, public PacketFactory
	/// LibSourcey's IPacketSocket interface extends ISocket   
	/// to act as a pluggable PacketStream adapter which may
	/// be a source or destination for IPackets.
	///
	/// Packets may received by attaching delegates using the 
	/// PacketEmitter signal methods.
{
public:
	IPacketSocket() {};
	IPacketSocket(const IPacketSocket& r) :
		PacketFactory(r) {}; // delegates are not copied
	virtual ~IPacketSocket() {};
	
	virtual int send(const IPacket& packet) = 0;
	virtual int send(const IPacket& packet, const Address& peerAddress) = 0;

	virtual void send(IPacket& packet) = 0; 
		/// This method provides delegate compatibility.
		/// Signals can attach to this use this method to
		/// pump data directly into the socket.
};


typedef std::vector<IPacketSocket*> IPacketSocketList;


struct PacketInfo: public IPacketInfo
	/// An abstract interface for packet sources to
	/// provide extra information about packets.
{ 
	IPacketSocket& socket;
	Address localAddress;
	Address peerAddress;

	PacketInfo(IPacketSocket& socket, const Address& peerAddress) :
		socket(socket), 
		localAddress(socket.address()), 
		peerAddress(peerAddress) {}		

	PacketInfo(const PacketInfo& r) : 
		socket(r.socket), 
		localAddress(r.localAddress), 
		peerAddress(r.peerAddress) {}
		
	/*
	PacketInfo& operator = (const PacketInfo& r) {
		socket = r.socket;
		localAddress = r.localAddress;
		peerAddress = r.peerAddress;
		return *this;
	}
	*/

	virtual ~PacketInfo() {}; 
	
	virtual IPacketInfo* clone() const {
		return new PacketInfo(*this);
	}

private:
	PacketInfo& operator = (const PacketInfo&) {}; // can't assign mutex
};


} } // namespace Scy::Net


#endif // SOURCEY_NET_IPacketSocket_H



/*
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Address.h"

#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"


#define Poco::Net::StreamSocket, TCP Poco::Net::StreamSocket, Scy::Net::TCP
#define Poco::Net::SecureStreamSocket, SSLTCP Poco::Net::SecureStreamSocket, Scy::Net::SSLTCP
#define UDPContext Poco::Net::DatagramSocket, Scy::Net::UDP
*/
/*
	IPacketSocket() {};
	
	
	virtual Address address() const = 0;
	virtual Address peerAddress() const = 0;
	virtual TransportProtocol transport() const = 0;
	virtual std::string error() const = 0;
	virtual int errorno() const = 0;
	
	virtual bool isConnected() const = 0;
	virtual bool isError() const = 0;

	NullSignal Connected;
	NullSignal Closed;
	Signal2<int&, const std::string&> Error;

	virtual void connect(const Address& address) = 0;
	virtual void close() = 0;
		
	virtual int send(const char* data, int size) = 0;
	virtual int send(const char* data, int size, const Address& peerAddress) = 0;
	virtual int send(const IPacket& packet) = 0;
	virtual int send(const IPacket& packet, const Address& peerAddress) = 0;
	*/