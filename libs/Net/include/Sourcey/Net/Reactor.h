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


#ifndef SOURCEY_NET_Reactor_H
#define SOURCEY_NET_Reactor_H


#include "Sourcey/Base.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Flaggable.h"
#include "Sourcey/DispatchQueue.h"
#include "Sourcey/BasicManager.h"

#include "Poco/Net/Socket.h"
#include "Poco/Mutex.h"

#include <deque>


namespace Sourcey {


template<class C, typename Reactor, typename Socket, typename Event>
struct DelegateCallback<C, 11, false, Reactor, Socket, Event> 
	/// This custom callback enables us to specify an
	/// empty() callback function.
{
	typedef void (C::*Method)();
	virtual void dispatch(Void, Reactor, Socket, Event, Void) const {
		(_object->*_method)();
	}
	DefineCallbackFields
};


namespace Net {
	

class Reactor;
class ReactorNotifier;
class ReactorEvent;


// ---------------------------------------------------------------------
//
enum SocketEvent
{
	SocketReadable	= 0x02, 
	SocketWritable	= 0x04, 
	SocketError	= 0x08
};


// -------------------------------------------------------------------
//
struct ReactorDelegateBase: public DelegateBase<Reactor&, const Poco::Net::Socket&, const SocketEvent&>
	/// Filters delegates based on reactor event type.
{
	typedef SocketEvent DataT;
	
	SocketEvent event;
	Poco::Net::Socket socket;

	ReactorDelegateBase(DataT event = SocketReadable) : event(event) {};
	ReactorDelegateBase(const ReactorDelegateBase& r) : event(r.event) {};

	virtual ReactorDelegateBase* clone() const = 0;
};


typedef ReactorDelegateBase ReactorDelegate;


// ---------------------------------------------------------------------
//
class Reactor: public Poco::Runnable
	/// This class implements the Reactor pattern described in
	/// the book "Pattern Languages of Program Design" by Jim
	/// Coplien and Douglas C. Schmidt (Addison Wesley, 1995).
	///
	/// Poco's SocketReactor was found to be inefficient in high
	/// traffic situations. This implementation significantly
	/// improves performance and scalability by employing separate
	/// notification thread.
	///
	/// TODO: Poco's poll method using epoll on linux reinitializes
	/// the epoll context on each call. Implement poll() locally.
{
public:
	Reactor(Runner& runner = Runner::getDefault(), int timeout = 5000000);
	virtual ~Reactor();
	
	void run();
		/// Runs the Reactor. The reactor will run until stop()
		/// is called (from a separate thread).
		
	void stop();
		/// Stops the Reactor.
		//
		/// The reactor will be stopped when the next event
		/// (including a timeout event) occurs.
		
	virtual void attach(const Poco::Net::Socket& socket, const ReactorDelegate& delegate);
		/// Attaches the given delegate to the given socket.
	
	virtual void detach(const Poco::Net::Socket& socket, const ReactorDelegate& delegate);
		/// Detaches the given delegate from the given socket.

	virtual void detach(const Poco::Net::Socket& socket);
		/// Detaches all delegates from the given socket.
	
	static Reactor& getDefault();
		/// Returns the default Reactor singleton, although
		/// Reactor instances may be initialized individually.
	
	Runner& runner();
	
	NullSignal Shutdown;
	
protected:	
	void dispatch(const Poco::Net::Socket& socket, SocketEvent event);
		/// Dispatches the given event notification to all delegate
		/// registered to the given socket.
	
	virtual void handleException(const Exception& exc);
	virtual void handleException(const std::exception& exc);
		/// Handles exceptions thrown inside the reactor loop by
		/// delegates. Default behavior is to swallow them in 
		/// release build, and rethrow in debug build.

protected:
	typedef std::vector<ReactorDelegate*> DelegateList;

	DelegateList	_delegates;
	Runner&			_runner;
	bool            _stop;
	Poco::Thread	_thread;
	Poco::Event		_wakeUp;
	Poco::Timespan  _timeout;
	mutable Poco::FastMutex _mutex;
};


// ---------------------------------------------------------------------
//
template <class C>
static Delegate<C, 
	ReactorDelegate,
	DelegateCallback<C, 3, false, Reactor&, const Poco::Net::Socket&, const SocketEvent&>, 
	Reactor&, const Poco::Net::Socket&, const SocketEvent&
> reactorDelegate(C* pObj, void (C::*Method)(Reactor&, const Poco::Net::Socket&, const SocketEvent&), SocketEvent event, int priority = 0)
	/// This callback receives reactor event notifications
	/// to the given callback.
{
	return Delegate<C, 
		ReactorDelegate,
		DelegateCallback<C, 3, false, Reactor&, const Poco::Net::Socket&, const SocketEvent&>, 
		Reactor&, const Poco::Net::Socket&, const SocketEvent&
	>(pObj, Method, event, priority);
}


template <class C>
static Delegate<C, 
	ReactorDelegate,
	DelegateCallback<C, 11, false, Reactor&, const Poco::Net::Socket&, const SocketEvent&>, 
	Reactor&, const Poco::Net::Socket&, const SocketEvent&
> reactorDelegate(C* pObj, void (C::*Method)(), SocketEvent event, int priority = 0)
	/// This delegate receives empty reactor notifications
	/// to the given delegate.
{
	return Delegate<C, 
		ReactorDelegate,
		DelegateCallback<C, 11, false, Reactor&, const Poco::Net::Socket&, const SocketEvent&>, 
		Reactor&, const Poco::Net::Socket&, const SocketEvent&
	>(pObj, Method, event, priority);
}


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_Reactor_H
