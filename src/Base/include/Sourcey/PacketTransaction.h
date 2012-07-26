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


#ifndef SOURCEY_PacketTransaction_H
#define SOURCEY_PacketTransaction_H


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
class PacketTransaction: protected TimerTask, public ISendable, public StatefulSignal<TransactionState>
	/// Provides request/response functionality for IPacket types.
	///
	/// This class is fire and forget. The pointer is managed by 
	/// the Runner instance, and is destroyed when complete.
{
public:
	PacketTransaction(long timeout = 10000, int retries = 0) : 
		TimerTask(timeout, 0),
		_retries(retries), 
		_cancelled(false), 
		_attempts(0)
	{
		Log("trace") << "[PacketTransaction:" << this << "] Creating" << std::endl;
	}


	PacketTransaction(const PacketT& request, long timeout = 10000, int retries = 0) : 
		TimerTask(timeout, 0),
		_request(request), 
		_retries(retries), 
		_cancelled(false), 
		_attempts(0)
	{
		Log("trace") << "[PacketTransaction:" << this << "] Creating" << std::endl;
	}


	PacketTransaction(Runner& runner, long timeout = 10000, int retries = 0) :
		TimerTask(timeout, 0),
		_retries(retries), 
		_cancelled(false), 
		_attempts(0)
	{
		Log("trace") << "[PacketTransaction:" << this << "] Creating" << std::endl;
	}
		

	PacketTransaction(Runner& runner, const PacketT& request, long timeout = 10000, int retries = 0) : 
		TimerTask(runner, timeout, 0), 
		_request(request), 
		_retries(retries), 
		_cancelled(false), 
		_attempts(0)
	{
		Log("trace") << "[PacketTransaction:" << this << "] Creating" << std::endl;
	}


	virtual bool send()
		/// Starts the transaction timer.
		/// Overriding classes implement sending here.
		/// This method must be called by derived classes.
	{
		if (!canSend())
			return false;
			//throw Exception("The transaction cannot be sent.");

		{
			Poco::FastMutex::ScopedLock lock(_mutex);

			Log("trace") << "[PacketTransaction:" << this << "] Sending: " 
				<< _timeout << ": " << _request.toString() << std::endl;
			_attempts++;
		}
		setState(this, TransactionState::Running);
		TimerTask::cancel();
		TimerTask::start();
		return true;
	}
	

	void cancel()
		/// Cancellation means that the agent will not retransmit 
        /// the request, will not treat the lack of response to be
        /// a failure, but will wait the duration of the transaction
		/// timeout for a response.
		///
		/// This method is not virtual as the TimerTask cancelled() 
		/// method is redundant at the transaction scope.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_cancelled = true;

		// Do not set the underlying TimerTask to cancelled as we
		// still need to receive the need timeout event.
	}
	

	bool cancelled() const
		/// Returns the transaction cancelled status.
		///
		/// This method is not virtual as the TimerTask cancelled() 
		/// method is redundant at the transaction scope.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _cancelled;
	}


	virtual bool canSend()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return !_cancelled && _attempts <= _retries;
	}
	

	virtual int attempts() const
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


protected:
	virtual ~PacketTransaction()
	{
		Log("trace") << "[PacketTransaction:" << this << "] Destroying" << std::endl;
		assert(!stateEquals(TransactionState::Running));
	}

	
	virtual bool process(const PacketT& packet)
		/// Processes a potential response and updates state on match.
	{	
		if (stateEquals(TransactionState::Running) && match(packet)) {
			{
				Poco::FastMutex::ScopedLock lock(_mutex);
				_response = packet;
				Log("trace") << "[PacketTransaction:" << this << "] Transaction Response Received: " 
					<< _response.toString() << std::endl;
			}
			onResponse();
			setState(this, cancelled() ? TransactionState::Cancelled : TransactionState::Success);		
			onComplete();
			return true;
		}
		return false;
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
		Log("trace") << "[PacketTransaction:" << this << "] Complete" << std::endl;
		TimerTask::destroy();
	}


	virtual void onTimeout()
	{	
		Log("trace") << "[PacketTransaction:" << this << "] Timeout" << std::endl;	
		{		
			if (cancelled()) {
				setState(this, TransactionState::Cancelled);
				onComplete();
			} 
			else if (!canSend()) {
				setState(this, TransactionState::Failed, "Transaction timeout");
				onComplete();
			} 
			else send();
		}		
	}
	

protected:
	mutable Poco::FastMutex	_mutex;
	
	PacketT _request;
	PacketT _response;
	bool _cancelled;
	int _retries;		// The maximum number of attempts before the transaction is considered failed.
	int _attempts;		// The number of times the transaction has been sent.	
};


} // namespace Sourcey


#endif // SOURCEY_PacketTransaction_H


	//bool _autoDelete;	// Optionally delete the instance on completion.

	//Signal<TransactionState&> TransactionComplete;
		/// Fired when the transaction is completed.
		