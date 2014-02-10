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


#ifndef SCY_Net_Transaction_H
#define SCY_Net_Transaction_H


#include "scy/packettransaction.h"
#include "scy/net/packetsocket.h"


namespace scy {
namespace net {


template <class PacketT>
class Transaction: public PacketTransaction<PacketT>, public PacketSocketAdapter
	/// This class provides request/response functionality for IPacket
	/// types emitted from a Socket.
{
public:
	Transaction(const net::Socket::Ptr& socket, 
				const Address& peerAddress, 
				int timeout = 10000, 
				int retries = 1, 
				uv::Loop* loop = uv::defaultLoop()) : 
		PacketTransaction<PacketT>(timeout, retries, loop), 
		PacketSocketAdapter(socket),
		_peerAddress(peerAddress)
	{
		TraceLS(this) << "Create" << std::endl;

		PacketSocketAdapter::socket->addReceiver(this, 100); // highest prioority
	}

	virtual bool send()
	{
		TraceLS(this) << "Send" << std::endl;
		assert(PacketSocketAdapter::socket);
		//assert(PacketSocketAdapter::socket->recvAdapter() == this);
		if (PacketSocketAdapter::socket->sendPacket(PacketTransaction<PacketT>::_request, _peerAddress) > 0)
			return PacketTransaction<PacketT>::send();
		PacketTransaction<PacketT>::setState(this, TransactionState::Failed);
		return false;
	}
	
	virtual void cancel()
	{
		TraceLS(this) << "Cancel" << std::endl;
		PacketTransaction<PacketT>::cancel();
	}

	virtual void dispose()
	{
		TraceLS(this) << "Dispose" << std::endl;
		//if (!PacketTransaction<PacketT>::_destroyed) {
		//	PacketSocketAdapter::socket->setAdapter(nullptr);
		//}
		PacketSocketAdapter::socket->removeReceiver(this);
		PacketTransaction<PacketT>::dispose(); // gc
	}
	
	Address peerAddress() const
	{
		return _peerAddress;
	}

protected:	
	virtual ~Transaction()
	{
	}

	virtual void onPacket(IPacket& packet)
		// Overrides the PacketSocketAdapter::onPacket 
		// callback for checking potential response candidates.
	{
		TraceLS(this) << "On packet: " << packet.size() << std::endl;
		if (PacketTransaction<PacketT>::handlePotentialResponse(static_cast<PacketT&>(packet))) {

			// Stop socket data propagation since
			// we have handled the packet
			throw StopPropagation();
		}
	}
	
	virtual void onResponse() 
		// Called when a successful response match is received.
	{
		TraceLS(this) << "On success: " << 
			PacketTransaction<PacketT>::_response.toString() << std::endl;
		PacketSignal::emit(socket.get(), PacketTransaction<PacketT>::_response);
	}

	virtual bool checkResponse(const PacketT& packet) 
		// Sub classes should derive this method to implement 
		// response checking logic.
		// The base implementation only performs address matching.
	{
		assert(packet.info && "socket must provide packet info");
		if (!packet.info)
			return false;
		auto info = reinterpret_cast<net::PacketInfo*>(packet.info);		
		return socket->address()  == info->socket->address() 
			&& _peerAddress == info->peerAddress;
	}
	
	Address _peerAddress;
	//net::Socket::Ptr PacketSocketAdapter::socket;
};


} } // namespace scy::net


#endif // SCY_Net_Transaction_H