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


#ifndef SOURCEY_NET_PacketTransaction_H
#define SOURCEY_NET_PacketTransaction_H


#include "Sourcey/PacketTransaction.h"
#include "Sourcey/Net/IPacketSocket.h"


namespace Sourcey {
namespace Net {


template <class PacketT>
class Transaction: public PacketTransaction<PacketT>
	/// This class provides request/response functionality for IPacket
	/// types emitted from an IPacketSocket.
	/// This class is designed to be derived on a per protocol basis.
{
public:
	Transaction(Runner& runner, 
				IPacketSocket* socket, 
				const Address& localAddress, 
				const Address& peerAddress, 
				int timeout = 10000, 
				int retries = 1) : 
		PacketTransaction<PacketT>(runner, timeout, retries), 
		_socket(socket), _localAddress(localAddress), _peerAddress(peerAddress)
	{
		LogDebug() << "[NetTransaction:" << this << "] Creating" << std::endl;
	}

	virtual bool send()
	{
		if (_socket)
			_socket->attach(packetDelegate(this, &Transaction::onPotentialResponse, 100));
		if (!_socket || _socket->send(PacketTransaction<PacketT>::_request, _peerAddress))
			return PacketTransaction<PacketT>::send();
		setState(this, TransactionState::Failed);
		return false;
	}
	
	virtual void cancel()
	{
		LogDebug() << "[NetTransaction:" << this << "] Canceling" << std::endl;
		{
			Poco::FastMutex::ScopedLock lock(PacketTransaction<PacketT>::_mutex);
			if (_socket) {
				_socket->detach(packetDelegate(this, &Transaction::onPotentialResponse));
				_socket = NULL;
			}
		}
		PacketTransaction<PacketT>::cancel();
	}
	
	virtual void onPotentialResponse(void*, PacketT& packet)
	{
		if (process(packet))
			throw StopPropagation();
	}	

	virtual void onResponse()
	{
		LogDebug() << "[NetTransaction:" << this << "] Response" << std::endl;
		{
			// Detach delegates onResponse before state callback.
			// Remove callbacks, but do not nullify the socket,
			// because the outside application might need it.
			Poco::FastMutex::ScopedLock lock(PacketTransaction<PacketT>::_mutex);
			if (_socket) {
				_socket->detach(packetDelegate(this, &Transaction::onPotentialResponse));
				//_socket = NULL;
			}
		}
		PacketTransaction<PacketT>::onResponse();
	}

	virtual void onComplete()
	{
		LogDebug() << "[NetTransaction:" << this << "] Complete" << std::endl;
		{
			// Nullify the socket on completion.
			Poco::FastMutex::ScopedLock lock(PacketTransaction<PacketT>::_mutex);
			if (_socket) {
				//_socket->detach(packetDelegate(this, &Transaction::onPotentialResponse));
				_socket = NULL;
			}
		}
		PacketTransaction<PacketT>::onComplete();
	}
	
	Address& localAddress() 
	{
		return _localAddress;
	}
	
	Address localAddress() const
	{
		return _localAddress;
	}
	
	Address& peerAddress() 
	{
		return _peerAddress;
	}
	
	Address peerAddress() const
	{
		return _peerAddress;
	}
	
	IPacketSocket* socket() 
	{
		return _socket;
	}

protected:
	virtual ~Transaction()
	{
		LogDebug() << "[Transaction:" << this << "] Destroying" << std::endl;
	}

	virtual bool match(const PacketT& packet) 
	{
		PacketInfo* info = reinterpret_cast<PacketInfo*>(packet.info);
		assert(info);
		if (!info)
			return false;

		return _localAddress == info->localAddress 
			&& _peerAddress == info->peerAddress;
	};
	
	IPacketSocket* _socket;
	Address _localAddress;
	Address _peerAddress;
};


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_PacketTransaction_H