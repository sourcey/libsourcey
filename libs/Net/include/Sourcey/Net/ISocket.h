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


#ifndef SOURCEY_NET_ISocket_H
#define SOURCEY_NET_ISocket_H


#include "Sourcey/Base.h"
#include "Sourcey/IPacket.h"
#include "Sourcey/PacketCreator.h"
#include "Sourcey/PacketDispatcher.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Address.h"


namespace Sourcey {
namespace Net {


class ISocket: public PacketCreator, public PacketDispatcher
	/// LibSourcey's ISocket interface is designed to act as
	/// a pluggable PacketStream adapter which may be a source or
	/// destination for IPackets.
	///
	/// Packets may received by delegating listeners using the 
	/// PacketDispatcher signal methods.
{
public:
	ISocket() {};
	ISocket(const ISocket& r) :
		PacketCreator(r) {}; // delegates are not copied
	virtual ~ISocket() {};

	virtual void connect(const Address& address) = 0;
	virtual void close() = 0;
		
	virtual int send(const char* data, int size) = 0;
	virtual int send(const char* data, int size, const Address& peerAddress) = 0;
	virtual int send(const IPacket& packet) = 0;
	virtual int send(const IPacket& packet, const Address& peerAddress) = 0;
	virtual void send(IPacket& packet) = 0; // delegate compatibility
	
	virtual Address address() const = 0;
	virtual Address peerAddress() const = 0;
	virtual TransportProtocol transport() const = 0;
	virtual std::string error() const = 0;
	virtual int errorno() const = 0;
	
	NullSignal Connected;
	NullSignal Closed;
	Signal2<const std::string&, int&> Error;
};


typedef std::vector<ISocket*> ISocketList;


struct PacketInfo: public IPacketInfo
	/// An abstract interface for packet sources to
	/// provide extra information about packets.
{ 
	ISocket& socket;
	Address localAddress;
	Address peerAddress;

	PacketInfo(ISocket& socket, const Address& peerAddress) :
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


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_ISocket_H
