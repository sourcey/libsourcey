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


#ifndef SOURCEY_PacketTransaction_H
#define SOURCEY_PacketTransaction_H


#include "Sourcey/Timer.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Interfaces.h"
#include "Sourcey/Packet.h"


namespace scy {

		
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
class PacketTransaction: public abstract::Sendable, public StatefulSignal<TransactionState>
	// This class provides request/response functionality for IPacket types.
	//
	// PacketTransactions are fire and forget. The object will be deleted
	// after a successful response or a timeout.
{
public:
	PacketTransaction(long timeout = 10000, int retries = 0, uv::Loop& loop = uv::defaultLoop()) :
		_timer(loop),
		_timeout(timeout),
		_retries(retries), 
		_attempts(0)
	{		
		traceL("PacketTransaction", this) << "Creating" << std::endl;
	}		

	PacketTransaction(const PacketT& request, long timeout = 10000, int retries = 0, uv::Loop& loop = uv::defaultLoop()) : 
		_timer(loop),
		_timeout(timeout),
		_request(request), 
		_retries(retries), 
		_attempts(0)
	{
		traceL("PacketTransaction", this) << "Creating" << std::endl;
	}

	virtual bool send()
		// Starts the transaction timer.
		// Overriding classes implement sending here.
		// This method must be called by derived classes.
	{
		if (!canResend())
			return false;
		{
			//ScopedLock lock(_mutex);
			traceL("PacketTransaction", this) << "Sending" << std::endl;
			_attempts++;
		}
		setState(this, TransactionState::Running);
		if (_timer.active())
			_timer.stop();
		_timer.Timeout += delegate(this, &PacketTransaction::onTimeout);
		_timer.start(_timeout, 0);
		//_timer.restart();
		return true;
	}	

	void cancel()
		// Cancellation means that the agent will not retransmit 
        /// the request, will not treat the lack of response to be
        /// a failure, but will wait the duration of the transaction
		// timeout for a response.
	{
		//ScopedLock lock(_mutex);
		
		//_cancelled = true;
		setState(this, TransactionState::Cancelled);
	}
	
	bool cancelled() const
		// Returns the transaction cancelled status.
	{
		//ScopedLock lock(_mutex);

		//_cancelled;
		return stateEquals(TransactionState::Cancelled);
	}

	virtual bool canResend()
	{
		//ScopedLock lock(_mutex);
		return !cancelled() && _attempts <= _retries;
	}
	
	virtual int attempts() const
	{
		//ScopedLock lock(_mutex);
		return _attempts;
	}
		
	PacketT& request() 
	{
		//ScopedLock lock(_mutex);
		return _request;
	}
	
	PacketT request() const
	{
		//ScopedLock lock(_mutex);
		return _request;
	}
		
	PacketT& response() 
	{
		//ScopedLock lock(_mutex);
		return _response;
	}
	
	PacketT response() const
	{
		//ScopedLock lock(_mutex);
		return _response;
	}


protected:
	virtual ~PacketTransaction()
	{
		traceL("PacketTransaction", this) << "Destroying" << std::endl;
		assert(!stateEquals(TransactionState::Running));
	}

	virtual void destroy()
		// Schedules the transaction for deferred deletion.
		//
		// It is safe to call this function while the transaction
		// is still active, providing the call is made from the same 
		// loop thread which the timer is running on.
	{
		_timer.Timeout -= delegate(this, &PacketTransaction::onTimeout);
		_timer.stop();

		deleteLater<PacketTransaction>(this);
	}
	
	virtual bool onPossibleResponse(const PacketT& packet)
		// Processes a potential response candidates
		// and updates state on match.
	{	
		traceL("PacketTransaction", this) << "Process" << std::endl;
		if (stateEquals(TransactionState::Running) && checkResponse(packet)) {
			{
				//ScopedLock lock(_mutex);
				_response = packet;
			}
			onResponse();
			setState(this, TransactionState::Success);
			destroy();
			return true;
		}
		return false;
	}

	virtual bool checkResponse(const PacketT& packet) = 0;
		// Checks potential response candidates.
		// Returns true on successful match.
	
	virtual void onResponse() 
		// Called when a successful response is received.
	{
		traceL("PacketTransaction", this) << "Response received: " 
			<< _response.toString() << std::endl;
	}

	virtual void onTimeout(void*)
	{	
		traceL("PacketTransaction", this) << "Timeout" << std::endl;	
		{		
			if (!canResend()) {
				if (!cancelled())
					setState(this, TransactionState::Failed, "Transaction timeout");				
				destroy();
			} 
			else send();
		}		
	}
	

protected:
	friend class GCDeleter<PacketTransaction>;

	Timer _timer;
	PacketT _request;
	PacketT _response;
	int _retries;		// The maximum number of attempts before the transaction is considered failed.
	int _attempts;		// The number of times the transaction has been sent.	
	int _timeout;		// The request timeout in milliseconds.

//private:
	//mutable Mutex	_mutex;
};



} // namespace scy


#endif // SOURCEY_IDepacketizerR_H