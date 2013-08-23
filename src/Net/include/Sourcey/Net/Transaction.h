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


#ifndef SOURCEY_NET_Transaction_H
#define SOURCEY_NET_Transaction_H


#include "Sourcey/PacketTransaction.h"
#include "Sourcey/Net/PacketSocket.h"
//#include "Sourcey/Timer.h"
//#include "Sourcey/Stateful.h"
//#include "Sourcey/Interface.h"
//#include "Sourcey/Packet.h"


namespace scy {
namespace net {


template <class PacketT>
class Transaction: public PacketTransaction<PacketT>, public PacketSocketAdapter
	/// This class provides request/response functionality for IPacket
	/// types emitted from a Socket.
{
public:
	Transaction(net::Socket& socket, 
				const Address& peerAddress, 
				int timeout = 10000, 
				int retries = 1, 
				uv::Loop& loop = uv::defaultLoop()) : 
		PacketTransaction<PacketT>(timeout, retries, loop), 
		PacketSocketAdapter(&socket),
		_peerAddress(peerAddress)
	{
		debugL("NetTransaction", this) << "Create" << std::endl;

		// Default options, can be overridden
		PacketSocketAdapter::socket->setAdapter(this);
	}

	virtual ~Transaction()
	{
		debugL("NetTransaction", this) << "Destroy" << std::endl;
		PacketSocketAdapter::socket->setAdapter(0);
	}

	virtual bool send()
	{
		debugL("NetTransaction", this) << "send" << std::endl;
		assert(socket);
		if (socket->send(PacketTransaction<PacketT>::_request, _peerAddress) > 0)
			return PacketTransaction<PacketT>::send();
		setState(this, TransactionState::Failed);
		return false;
	}
	
	virtual void cancel()
	{
		debugL("NetTransaction", this) << "Canceling" << std::endl;
		PacketTransaction<PacketT>::cancel();
	}
	
	Address peerAddress() const
	{
		return _peerAddress;
	}

protected:	
	virtual void onPacket(IPacket& packet)
		/// Overrides the PacketSocketAdapter onPacket 
		/// callback for checking potential response candidates.
	{
		debugL("NetTransaction", this) << "On Packet: " << packet.size() << std::endl;
		if (onPossibleResponse(static_cast<PacketT&>(packet))) {

			// Stop socket data propagation since
			// we have handled the packet
			//throw StopPropagation();
		}
	}
	
	virtual void onResponse() 
		/// Called when a successful response match is received.
	{
		traceL("Transaction", this) << "On Success: " << _response.toString() << std::endl;
		PacketSignal::emit(socket, _response);
	}

	virtual bool checkResponse(const PacketT& packet) 
		/// Sub classes should derive this method to implement 
		/// response checking logic.
		/// The base implementation only performs address matching.
	{
		assert(packet.info && "socket must provide packet info");
		if (!packet.info)
			return false;
		auto info = reinterpret_cast<net::PacketInfo*>(packet.info);		
		return socket->address()  == info->socket->address() 
			&& _peerAddress == info->peerAddress;
	}
	
	Address _peerAddress;
};


} } // namespace scy::net


#endif // SOURCEY_NET_Transaction_H