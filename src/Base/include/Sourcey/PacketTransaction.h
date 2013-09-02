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
#include "Sourcey/Interface.h"
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
class PacketTransaction: public basic::Sendable, public Stateful<TransactionState>
	/// This class provides request/response functionality for IPacket types.
	///
	/// PacketTransactions are fire and forget. The object will be deleted
	/// after a successful response or a timeout.
{
public:
	PacketTransaction(long timeout = 10000, int retries = 0, uv::Loop& loop = uv::defaultLoop()) :
		_timer(loop), 
		_timeout(timeout), 
		_retries(retries), 
		_attempts(0),
		_destroyed(false)
	{		
	}		

	PacketTransaction(const PacketT& request, long timeout = 10000, int retries = 0, uv::Loop& loop = uv::defaultLoop()) : 
		_timer(loop),
		_timeout(timeout),
		_request(request), 
		_retries(retries), 
		_attempts(0),
		_destroyed(false)
	{
	}
	
	virtual bool send()
		// Starts the transaction timer and sends the request.
		// Overriding classes should implement send logic here.
	{
		if (!canResend())
			return false;

		_attempts++;
		if (_timer.active())
			_timer.stop();
		_timer.Timeout += delegate(this, &PacketTransaction::onTimeout);
		_timer.start(_timeout, 0);

		return setState(this, TransactionState::Running);
	}	
	
	void cancel();
	bool cancelled() const;	
		// Cancellation means that the agent will not retransmit 
		// the request, will not treat the lack of response to be
		// a failure, but will wait the duration of the transaction
		// timeout for a response.	
	
	virtual void dispose()
		// Schedules the transaction for deferred deletion.
		//
		// It is safe to call this function while the transaction
		// is still active, providing the call is made from the same 
		// loop thread which the timer is running on.
		//
		// Protected by the base implementation as this is called
		// by the internal state machine.
	{
		if (!_destroyed) {
			_destroyed = true;
			_timer.Timeout -= delegate(this, &PacketTransaction::onTimeout);
			_timer.stop();

			deleteLater<PacketTransaction>(this);
		}
	}

	virtual bool canResend();	
	int attempts() const;
	int retries() const;
		
	PacketT& request();
	PacketT request() const;
		
	PacketT& response();
	PacketT response() const;

protected:
	virtual ~PacketTransaction()
	{
		assert(!stateEquals(TransactionState::Running));
	}

	virtual void onStateChange(TransactionState& state, const TransactionState&) 
		// Override to handle post state change logic.
	{
		if (state.equals(TransactionState::Success) || 
			state.equals(TransactionState::Failed))
			dispose();
	}
	
	virtual bool handlePotentialResponse(const PacketT& packet)
		// Processes a potential response candidate
		// and updates the state accordingly.
	{	
		if (stateEquals(TransactionState::Running) && checkResponse(packet)) {
			_response = packet;
			onResponse();
			setState(this, TransactionState::Success);
			return true;
		}
		return false;
	}
	
	virtual bool checkResponse(const PacketT& packet) = 0;
		// Checks a potential response candidate and
		// returns true on successful match.
	
	virtual void onResponse() 
		// Called when a successful response is received.
	{
		traceL("PacketTransaction", this) << "Success: " 
			<< _response.toString() << std::endl;
	}

	virtual void onTimeout(void*)
	{	
		debugL("PacketTransaction", this) << "Timeout" << std::endl;	

		if (!canResend()) {
			//if (!cancelled())
			//	setState(this, TransactionState::Failed, "Transaction timeout");				
			//dispose();
			setState(this, TransactionState::Failed, "Transaction timeout");
		} 
		else send();
	}	

protected:
	friend struct std::default_delete/*friend class AsyncDeleter*/<PacketTransaction>;

	Timer _timer;
	PacketT _request;
	PacketT _response;
	int _retries;		// The maximum number of attempts before the transaction is considered failed.
	int _attempts;		// The number of times the transaction has been sent.	
	int _timeout;		// The request timeout in milliseconds.
	bool _destroyed;
};

	
template <class T> inline void PacketTransaction<T>::cancel() { setState(this, TransactionState::Cancelled); }
template <class T> inline bool PacketTransaction<T>::cancelled() const { return stateEquals(TransactionState::Cancelled); }

template <class T> inline bool PacketTransaction<T>::canResend() { return !cancelled() && attempts() <= retries(); }	
template <class T> inline int PacketTransaction<T>::attempts() const { return _attempts; }
template <class T> inline int PacketTransaction<T>::retries() const { return _retries; }
		
template <class T> inline T& PacketTransaction<T>::request() { return _request; }	
template <class T> inline T PacketTransaction<T>::request() const { return _request; }		
template <class T> inline T& PacketTransaction<T>::response() { return _response;	}	
template <class T> inline T PacketTransaction<T>::response() const { return _response; }


} // namespace scy


#endif // SOURCEY_IDepacketizerR_H