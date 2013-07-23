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


#ifndef SOURCEY_NET_PacketTransaction_H
#define SOURCEY_NET_PacketTransaction_H


#include "Sourcey/PacketTransaction.h"
#include "Sourcey/Net/PacketSocket.h"
#include "Sourcey/Timer.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Interfaces.h"
#include "Sourcey/Packet.h"


namespace scy {
namespace net {


template <class PacketT> //, class EmitterT
class Transaction: public PacketTransaction<PacketT>, public PacketSocketAdapter
	/// This class provides request/response functionality for IPacket
	/// types emitted from a SocketBase.
	/// This class is designed to be derived on a per protocol basis.
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
		debugL("NetTransaction", this) << "Creating" << std::endl;

		// Default options, can be overridden
		PacketSocketAdapter::socket->setAdapter(this);
		//PacketSocketAdapter::priority = 100;
	}

	virtual ~Transaction()
	{
		debugL("NetTransaction", this) << "Destroying" << std::endl;
		PacketSocketAdapter::socket->setAdapter(0);
	}

	virtual bool send()
	{
		debugL("NetTransaction", this) << "Sending" << std::endl;
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
		net::PacketInfo* info = reinterpret_cast<net::PacketInfo*>(packet.info);		
		return socket->address()  == info->socket.address() 
			&& _peerAddress == info->peerAddress;
	}
	
	Address _peerAddress;
};


} } // namespace scy::net


#endif // SOURCEY_NET_PacketTransaction_H


/*
template <class PacketT>
class Transaction: public PacketTransaction<PacketT>
	/// This class provides request/response functionality for IPacket
	/// types emitted from a SocketBase.
	/// This class is designed to be derived on a per protocol basis.
{
public:
	Transaction(const net::Socket& socket, 
				const Address& localAddress, 
				const Address& peerAddress, 
				int timeout = 10000, 
				int retries = 1, 
				uv::Loop& loop = uv::defaultLoop()) : 
		PacketTransaction<PacketT>(timeout, retries, loop), 
		_socket(socket),
		_localAddress(localAddress), 
		_peerAddress(peerAddress)
	{
		debugL("NetTransaction", this) << "Creating" << std::endl;
	}

	virtual bool send()
	{
		debugL("NetTransaction", this) << "Sending" << std::endl;
		_socket.adapter() += packetDelegate(this, &Transaction::onPotentialResponse, 100);
		if (_socket.base().send(PacketTransaction<PacketT>::_request, _peerAddress))
			return PacketTransaction<PacketT>::send();
		setState(this, TransactionState::Failed);
		return false;
	}
	
	virtual void cancel()
	{
		debugL("NetTransaction", this) << "Canceling" << std::endl;
		_socket.adapter() -= packetDelegate(this, &Transaction::onPotentialResponse);
		PacketTransaction<PacketT>::cancel();
	}
	
	virtual void onPotentialResponse(void*, PacketT& packet)
	{
		if (onPossibleResponse(packet))
			throw StopPropagation();
	}	

	virtual void onResponse()
	{
		debugL("NetTransaction", this) << "Response" << std::endl;
		_socket.adapter() -= packetDelegate(this, &Transaction::onPotentialResponse);
		PacketTransaction<PacketT>::onResponse();
	}
	
	Address localAddress() const
	{
		return _localAddress;
	}
	
	Address peerAddress() const
	{
		return _peerAddress;
	}
	
	PacketSocket& socket() 
	{
		return _socket;
	}

protected:
	virtual ~Transaction()
	{
		debugL("NetTransaction", this) << "Destroying" << std::endl;
	}

	virtual bool checkResponse(const PacketT& packet) 
	{
		assert(packet.info && "socket must provide packet onfo");
		if (!packet.info)
			return false;
		net::PacketInfo* info = reinterpret_cast<net::PacketInfo*>(packet.info);
		
		return _localAddress == info->socket.address() 
			&& _peerAddress == info->peerAddress;
	};
	
	PacketSocket _socket;
	Address _localAddress;
	Address _peerAddress;

private:
	//mutable Mutex	_mutex;
};
*/


		//_socket(socket),
		//_localAddress(localAddress), 
		//,
		//_registered(false)
		//if (!_registered)
	
		//_socket.adapter() += packetDelegate(this, &Transaction::onPotentialResponse, 100);
		//_socket.adapter() -= packetDelegate(this, &Transaction::onPotentialResponse);
	/*
	//Socket& _socket;
	Socket& socket() 
	{
		return _socket;
	}
	*/

	/*void* sender,

//private:
	//mutable Mutex	_mutex;
	virtual void onResponse()
	{
		debugL("NetTransaction", this) << "Response" << std::endl;
		//_socket.adapter() -= packetDelegate(this, &Transaction::onPotentialResponse);
		PacketTransaction<PacketT>::onResponse();
	}
	*/
	
	/*
	virtual void onResponse() 
	{
		traceL("PacketTransaction", this) << "Complete" << std::endl;
		_timer.close();
	}
	*/
	
	/*
	//PacketSocket _socket;


	Address& localAddress() 
	{
		return _localAddress;
	}
	
	Address& peerAddress() 
	{
		return _peerAddress;
	}
	*/


	/*
		//Mutex::ScopedLock lock(_mutex);
	//net::SocketBase* _socket;
	virtual void onResponse()
	{
		debugL("NetTransaction", this) << "Complete" << std::endl;
		{
			// Nullify the socket on completion.
			//Mutex::ScopedLock lock(_mutex);
			//if (_socket) {
				//_socket.detach(packetDelegate(this, &Transaction::onPotentialResponse));
				//_socket = NULL;
			//}
		}
		PacketTransaction<PacketT>::onResponse();
	}
	*/