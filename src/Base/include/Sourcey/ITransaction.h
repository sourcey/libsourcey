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


#ifndef SOURCEY_ITransaction_H
#define SOURCEY_ITransaction_H


#include "Sourcey/TimerTask.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/ISendable.h"
#include "Sourcey/IPacket.h"


namespace Sourcey {


struct TransactionState: public StateT 
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
class ITransaction: protected TimerTask, public ISendable, public StatefulSignal<TransactionState>
	/// Provides request/response functionality for IPacket types.
	///
	/// This class is fire and forget. The pointer is managed by 
	/// the Runner instance, and is destroyed when complete.
{
public:
	ITransaction(Runner& runner, int maxAttempts = 1, long timeout = 10000) : 
		TimerTask(runner, timeout, timeout),
		_maxAttempts(maxAttempts), 
		_attempts(0), 
		_cancelled(false)
	{
		Log("debug") << "[ITransaction:" << this << "] Creating" << std::endl;
	}

	ITransaction(Runner& runner, const PacketT& request, int maxAttempts = 1, long timeout = 10000) : 
		TimerTask(runner, timeout, timeout),
		_request(request), 
		_maxAttempts(maxAttempts), 
		_attempts(0), 
		_cancelled(false)
	{
		Log("debug") << "[ITransaction:" << this << "] Creating" << std::endl;
	}

	virtual bool send()
		/// Starts the transaction timer.
		/// Overriding classes implement sending here.
		/// This method must be called by derived classes.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		Log("debug") << "[ITransaction:" << this << "] Sending: " 
			<< _timeout << ": " << _request.toString() << std::endl;
		stateEquals(TransactionState::Running);
		_attempts++;
		setState(this, TransactionState::Running);
		return running() || start();
	}
	
	virtual bool process(const PacketT& packet)
		/// Processed a potential response and updates state on match.
	{	
		if (stateEquals(TransactionState::Running) && match(packet)) {
			{
				Poco::FastMutex::ScopedLock lock(_mutex);
				_response = packet;
				Log("debug") << "[ITransaction:" << this << "] Transaction Response Received: " 
					<< _response.toString() << std::endl;
			}
			onResponse();
			setState(this, _cancelled ? TransactionState::Cancelled : TransactionState::Success);		
			onComplete();
			return true;
		}
		return false;
	}

	virtual void cancel()
		/// Cancellation means that the agent will not retransmit the
        /// request, will not treat the lack of response to be a failure,
        /// but will wait the duration of the transaction timeout for 
		/// a response.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_cancelled = true;
	}
	
	bool cancelled()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _cancelled;
	}
	
	int attempts()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _attempts;
	}
	
	PacketT& request() 
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _request;
	}
	
	PacketT request() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _request;
	}
	
	PacketT& response() 
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _response;
	}
	
	PacketT response() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _response;
	}

	bool canRetry()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _attempts < _maxAttempts;
	}

	//Signal<TransactionState&> TransactionComplete;
		/// Fired when the transaction is completed.

protected:
	virtual ~ITransaction()
	{
		Log("debug") << "[ITransaction:" << this << "] Destroying" << std::endl;
		assert(!stateEquals(TransactionState::Running));
	}

	virtual bool match(const PacketT& packet) = 0;
		/// Matches against a potential response.
		/// Returns true on successful match.

	virtual void onResponse() 
		/// Call when a successful response is received.
	{
	}

	virtual void onComplete() 
	{
		Log("debug") << "[ITransaction:" << this << "] Closing" << std::endl;
		//TransactionComplete.dispatch(this, _state);
		destroy();
	}

	virtual void onTimeout()
	{	
		Log("debug") << "[ITransaction:" << this << "] Timeout" << std::endl;	
		{		
			if (cancelled()) {
				setState(this, TransactionState::Cancelled);
				onComplete();
			} 
			else if (!canRetry()) {
				setState(this, TransactionState::Failed, "Transaction timeout");
				onComplete();
			} 
			else send();
		}		
	}

	PacketT _request;
	PacketT _response;
	int _maxAttempts;	// The maximum number of retries before the transaction is considered failed.
	int _attempts;		// The number of times the transaction has been sent.
	bool _autoDelete;	// Optionally delete the instance on completion.
	bool _cancelled;
	mutable Poco::FastMutex	_mutex;
};


} // namespace Sourcey


#endif // SOURCEY_ITransaction_H