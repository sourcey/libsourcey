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


#include "Sourcey/Net/PacketSocket.h"
#include "Sourcey/Timer.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/ISendable.h"
#include "Sourcey/IPacket.h"


namespace scy {
namespace net {

	
struct TransactionState: public State 
{	
	enum Type 
	{
		Waiting = 0,
		Running,
		Success,
		Cancelled,
		Failed
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case Waiting:		return "Waiting";
		case Running:		return "Running";
		case Success:		return "Success";
		case Cancelled:		return "Cancelled";
		case Failed:		return "Failed";
		}
		return "undefined"; 
	};
};


template <class PacketT>
class PacketTransaction: public ISendable, public StatefulSignal<TransactionState>, public uv::Timer
	/// Provides request/response functionality for IPacket types.
	///
	/// This class is fire and forget. The pointer is managed by 
	/// the Runner instance, and is destroyed when complete.
{
public:
	PacketTransaction(long timeout = 10000, int retries = 0, uv_loop_t* loop = uv_default_loop()) :
		Timer(loop),
		_timeout(timeout),
		_retries(retries), 
		_cancelled(false), 
		_attempts(0)
	{
		traceL("PacketTransaction", this) << "Creating" << std::endl;
	}		

	PacketTransaction(const PacketT& request, long timeout = 10000, int retries = 0, uv_loop_t* loop = uv_default_loop()) : 
		Timer(loop), //timeout, 0, 
		_timeout(timeout),
		_request(request), 
		_retries(retries), 
		_cancelled(false), 
		_attempts(0)
	{
		traceL("PacketTransaction", this) << "Creating" << std::endl;
	}

	virtual bool send()
		/// Starts the transaction timer.
		/// Overriding classes implement sending here.
		/// This method must be called by derived classes.
	{
		if (!canSend())
			return false;
		{
			//Poco::FastMutex::ScopedLock lock(_mutex);

			traceL("PacketTransaction", this) << "Sending: " << _request.toString() << std::endl;
			_attempts++;
		}
		setState(this, TransactionState::Running);
		if (Timer::active())
			Timer::stop();
		Timer::start(_timeout, 0);
		//Timer::restart();
		return true;
	}	

	void cancel()
		/// Cancellation means that the agent will not retransmit 
        /// the request, will not treat the lack of response to be
        /// a failure, but will wait the duration of the transaction
		/// timeout for a response.
		///
		/// This method is not virtual as the Timer cancelled() 
		/// method is redundant at the transaction scope.
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		_cancelled = true;

		// Do not set the underlying Timer to cancelled as we
		// still need to receive the need timeout event.
	}
	
	bool cancelled() const
		/// Returns the transaction cancelled status.
		///
		/// This method is not virtual as the Timer cancelled() 
		/// method is redundant at the transaction scope.
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _cancelled;
	}

	virtual bool canSend()
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return !_cancelled && _attempts <= _retries;
	}
	
	virtual int attempts() const
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _attempts;
	}
		
	PacketT& request() 
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _request;
	}
	
	PacketT request() const
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _request;
	}
		
	PacketT& response() 
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _response;
	}
	
	PacketT response() const
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _response;
	}


protected:
	virtual ~PacketTransaction()
	{
		traceL("PacketTransaction", this) << "Destroying" << std::endl;
		//assert(!stateEquals(TransactionState::Running)); no more mt :)
		Timer::stop();
		traceL("PacketTransaction", this) << "Destroying: OK" << std::endl;
	}
	
	virtual bool onResponse(const PacketT& packet)
		/// Processes a potential response candidates
		/// and updates state on match.
	{	
		traceL("PacketTransaction", this) << "Process: " << _response.toString() << std::endl;
		if (stateEquals(TransactionState::Running) && checkResponse(packet)) {
			{
				//Poco::FastMutex::ScopedLock lock(_mutex);
				_response = packet;
				traceL("PacketTransaction", this) << "Transaction Response Received: " 
					<< _response.toString() << std::endl;
			}
			onSuccess();			
			setState(this, cancelled() ? 
				TransactionState::Cancelled : TransactionState::Success);		
			return true;
		}
		return false;
	}

	virtual bool checkResponse(const PacketT& packet) = 0;
		/// Checks potential response candidates.
		/// Returns true on successful match.
	
	virtual void onSuccess() 
		/// Call when a successful response is received.
	{
		traceL("PacketTransaction", this) << "onSuccess: " << _response.toString() << std::endl;
	}

	virtual void onTimeout()
	{	
		traceL("PacketTransaction", this) << "Timeout" << std::endl;	
		{		
			if (cancelled()) {
				//onSuccess();
				setState(this, TransactionState::Cancelled);
			} 
			else if (!canSend()) {
				//onSuccess();
				setState(this, TransactionState::Failed, "Transaction timeout");
			} 
			else send();
		}		
	}
	

protected:
	PacketT _request;
	PacketT _response;
	bool _cancelled;
	int _retries;		// The maximum number of attempts before the transaction is considered failed.
	int _attempts;		// The number of times the transaction has been sent.	
	int _timeout;		// The request timeout in milliseconds.

//private:
	//mutable Poco::FastMutex	_mutex;
};


template <class PacketT> //, class EmitterT
class Transaction: public PacketTransaction<PacketT>, public PacketSocketEmitter
	/// This class provides request/response functionality for IPacket
	/// types emitted from a SocketBase.
	/// This class is designed to be derived on a per protocol basis.
{
public:
	Transaction(net::Socket& socket, 
				const Address& peerAddress, 
				int timeout = 10000, 
				int retries = 1, 
				uv_loop_t* loop = NULL) : 
		PacketTransaction<PacketT>(timeout, retries, loop), 
		PacketSocketEmitter(&socket),
		_peerAddress(peerAddress)
	{
		debugL("NetTransaction", this) << "Creating" << std::endl;

		// Default options, can be overridden
		PacketSocketEmitter::socket->base().addObserver(*this, true);
		PacketSocketEmitter::priority = 100;
	}

	virtual ~Transaction()
	{
		debugL("NetTransaction", this) << "Destroying" << std::endl;
		PacketSocketEmitter::socket->base().removeObserver(*this);
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
		/// Overrides the PacketSocketEmitter onPacket 
		/// callback for checking potential response candidates.
	{
		debugL("NetTransaction", this) << "On Packet: " << packet.size() << std::endl;
		if (onResponse(static_cast<PacketT&>(packet))) {

			// Stop socket data propagation since
			// we have handled the packet
			//throw StopPropagation();
		}
	}
	
	virtual void onSuccess() 
		/// Called when a successful response match is received.
	{
		traceL("Transaction", this) << "On Success: " << _response.toString() << std::endl;
		PacketEmitter::emit(socket, _response);
	}

	virtual bool checkResponse(const PacketT& packet) 
		/// Sub classes should derive this method to implement 
		/// response checking logic.
		/// The base implementation only preforms address matching.
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
				uv_loop_t* loop = NULL) : 
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
		_socket.emitter() += packetDelegate(this, &Transaction::onPotentialResponse, 100);
		if (_socket.base().send(PacketTransaction<PacketT>::_request, _peerAddress))
			return PacketTransaction<PacketT>::send();
		setState(this, TransactionState::Failed);
		return false;
	}
	
	virtual void cancel()
	{
		debugL("NetTransaction", this) << "Canceling" << std::endl;
		_socket.emitter() -= packetDelegate(this, &Transaction::onPotentialResponse);
		PacketTransaction<PacketT>::cancel();
	}
	
	virtual void onPotentialResponse(void*, PacketT& packet)
	{
		if (onResponse(packet))
			throw StopPropagation();
	}	

	virtual void onSuccess()
	{
		debugL("NetTransaction", this) << "Response" << std::endl;
		_socket.emitter() -= packetDelegate(this, &Transaction::onPotentialResponse);
		PacketTransaction<PacketT>::onSuccess();
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
	//mutable Poco::FastMutex	_mutex;
};
*/


} } // namespace scy::net


#endif // SOURCEY_NET_PacketTransaction_H


		//_socket(socket),
		//_localAddress(localAddress), 
		//,
		//_registered(false)
		//if (!_registered)
	
		//_socket.emitter() += packetDelegate(this, &Transaction::onPotentialResponse, 100);
		//_socket.emitter() -= packetDelegate(this, &Transaction::onPotentialResponse);
	/*
	//Socket& _socket;
	Socket& socket() 
	{
		return _socket;
	}
	*/

	/*void* sender,

//private:
	//mutable Poco::FastMutex	_mutex;
	virtual void onSuccess()
	{
		debugL("NetTransaction", this) << "Response" << std::endl;
		//_socket.emitter() -= packetDelegate(this, &Transaction::onPotentialResponse);
		PacketTransaction<PacketT>::onSuccess();
	}
	*/
	
	/*
	virtual void onSuccess() 
	{
		traceL("PacketTransaction", this) << "Complete" << std::endl;
		Timer::close();
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
		//Poco::FastMutex::ScopedLock lock(_mutex);
	//net::SocketBase* _socket;
	virtual void onSuccess()
	{
		debugL("NetTransaction", this) << "Complete" << std::endl;
		{
			// Nullify the socket on completion.
			//Poco::FastMutex::ScopedLock lock(_mutex);
			//if (_socket) {
				//_socket.detach(packetDelegate(this, &Transaction::onPotentialResponse));
				//_socket = NULL;
			//}
		}
		PacketTransaction<PacketT>::onSuccess();
	}
	*/