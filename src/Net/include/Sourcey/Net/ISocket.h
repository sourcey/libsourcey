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


#ifndef SOURCEY_NET_ISocket_H
#define SOURCEY_NET_ISocket_H


#include "Sourcey/Base.h"
#include "Sourcey/IPacket.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Address.h"


namespace scy {
namespace Net {
	

class ISocket
	/// This class defines the generic socket interface   
	/// from which all sockets classes derive.
{
public:
	virtual ~ISocket() {};

	virtual void connect(const Address& address) = 0;
	virtual void close() = 0;
		
	virtual int send(const char* data, int size, int flags = 0) = 0;
	virtual int send(const char* data, int size, const Address& peerAddress, int flags = 0) = 0;
	virtual int send(const IPacket& packet, int flags = 0) = 0;
	virtual int send(const IPacket& packet, const Address& peerAddress, int flags = 0) = 0;
	
	virtual Address address() const = 0;
		/// The locally bound address.

	virtual Address peerAddress() const = 0;
		/// The connected peer address.

	virtual TransportProtocol transport() const = 0;
		/// The transport protocol: TCP, UDP or SSLTCP.
		/// See TransportProtocol definition.

	virtual std::string error() const = 0;
	virtual int errorno() const = 0;
	
	virtual bool isConnected() const = 0;
	virtual bool isError() const = 0;

	NullSignal Connected;
		/// Signals that the socket is connected.

	NullSignal Closed;
		/// Signals that the socket is closed.

	Signal2<int&, const std::string&> Error;
		/// Signals that the socket is closed in error.
		/// This signal will be sent just before the 
		/// Closed signal.

	Signal2<Buffer&, const Address&> Data;
		/// Signals data received by the socket.
		/// The address argument is for UDP compatibility.
		/// For TCP it will always return the peerAddress.
};


typedef std::vector<ISocket*> ISocketList;


} } // namespace scy::Net


#endif // SOURCEY_NET_ISocket_H


//#include "Sourcey/PacketFactory.h"
//#include "Sourcey/PacketEmitter.h"
	//ISocket() {};
	//ISocket(const ISocket& r) :
	//	PacketFactory(r) {}; // delegates are not copied
	//: public PacketFactory, public PacketEmitter
