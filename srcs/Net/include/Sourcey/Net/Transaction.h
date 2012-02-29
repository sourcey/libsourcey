//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_NET_ITransaction_H
#define SOURCEY_NET_ITransaction_H


#include "Sourcey/ITransaction.h"
#include "Sourcey/Net/ISocket.h"


namespace Sourcey {
namespace Net {


template <class PacketT>
class Transaction: public ITransaction<PacketT>
	/// This class rovides request/response functionality for IPacket
	/// types emitted from an ISocket.
	/// This class is designed to be derived on a per protocol basis.
{
public:
	Transaction(ISocket* socket, 
				const Address& localAddress, 
				const Address& peerAddress, 
				int maxAttempts = 1, 
				int timeout = 10000) : 
		ITransaction<PacketT>(maxAttempts, timeout), 
		_socket(socket), _localAddress(localAddress), _peerAddress(peerAddress)
	{
		Log("debug") << "[Transaction:" << this << "] Creating" << std::endl;
	}

	virtual bool send()
	{
		if (_socket)
			_socket->attach(packetDelegate(this, &Transaction::onPotentialResponse, 100));
		if (!_socket || _socket->send(ITransaction<PacketT>::_request, _peerAddress))
			return ITransaction<PacketT>::send();
		setState(this, TransactionState::Failed);
		return false;
	}
	
	virtual void cancel()
	{
		Log("debug") << "[NetTransaction:" << this << "] Closing" << std::endl;
		{
			Poco::FastMutex::ScopedLock lock(ITransaction<PacketT>::_mutex);
			if (_socket) {
				_socket->detach(packetDelegate(this, &Transaction::onPotentialResponse));
				_socket = NULL;
			}
		}
		ITransaction<PacketT>::cancel();
	}
	
	virtual void onPotentialResponse(void*, PacketT& packet)
	{
		if (process(packet))
			throw StopPropagation();
	}	

	virtual void onComplete()
	{
		Log("debug") << "[NetTransaction:" << this << "] Closing" << std::endl;
		{
			Poco::FastMutex::ScopedLock lock(ITransaction<PacketT>::_mutex);
			if (_socket) {
				_socket->detach(packetDelegate(this, &Transaction::onPotentialResponse));
				_socket = NULL;
			}
		}
		ITransaction<PacketT>::onComplete();
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
	
	ISocket* socket() 
	{
		return _socket;
	}

protected:
	virtual ~Transaction()
	{
		Log("debug") << "[Transaction:" << this << "] Destroying" << std::endl;
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
	
	ISocket* _socket;
	Address _localAddress;
	Address _peerAddress;
};


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_ITransaction_H