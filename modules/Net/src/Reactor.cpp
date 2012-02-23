//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Logger.h"

#include "Poco/Net/NetException.h"
#include "Poco/SingletonHolder.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Sourcey {
namespace Net {


Reactor::Reactor(Runner& runner, int timeout) :
	_thread("Runner"),
	_runner(runner),
	_timeout(timeout),
	_stop(false)	
{
	cout << "[Reactor:" << this << "] Creating" << endl;
	_thread.start(*this);
}


Reactor::~Reactor()
{
	cout << "[Reactor:" << this << "] Drestroying" << endl;

	stop();
}


void Reactor::run()
{	
	Socket::SocketList readable;
	Socket::SocketList writable;
	Socket::SocketList error;
	
	while (!_stop)
	{		
		try
		{
			readable.clear();
			writable.clear();
			error.clear();

			if (_delegates.empty()) {				
				Log("trace") << "[Reactor:" << this << "] Skeeping" << endl;
				_wakeUp.wait();		
				Log("trace") << "[Reactor:" << this << "] Waking up" << endl;
			}
			{
				FastMutex::ScopedLock lock(_mutex);
				for (DelegateList::iterator it = _delegates.begin(); it != _delegates.end(); ++it)
				{
					switch ((*it)->event) {
					case SocketReadable:
						readable.push_back((*it)->socket);
						break;		
					case SocketWritable:
						writable.push_back((*it)->socket);
						break;
					case SocketError:
						error.push_back((*it)->socket);
						break;
					default:
						assert(false);
						break;
					}
				}
			}

			if (Socket::select(readable, writable, error, _timeout)) 
			{
				for (Socket::SocketList::iterator it = readable.begin(); it != readable.end(); ++it)
					dispatch(*it, SocketReadable);
				for (Socket::SocketList::iterator it = writable.begin(); it != writable.end(); ++it)
					dispatch(*it, SocketWritable);
				for (Socket::SocketList::iterator it = error.begin(); it != error.end(); ++it)
					dispatch(*it, SocketError);
			} 

			Thread::sleep(3); // gulp!
		}
		catch (Exception& exc)
		{
			handleException(exc);
		}
		catch (exception& exc)
		{
			handleException(exc);
		}
		catch (...)
		{
			Log("error") << "[Reactor:" << this << "] Unknown Error" << endl;
			//throw;
		}
	}

	Shutdown.dispatch(this);
}


void Reactor::handleException(const Exception& exc)
{
	Log("error") << "[Reactor:" << this << "] Error " << exc.displayText() << endl;

#ifdef _DEBUG	
	exc.rethrow();
#endif
}


void Reactor::handleException(const std::exception& exc)
{
	Log("error") << "[Reactor:" << this << "] Error " << exc.what() << endl;

#ifdef _DEBUG	
	throw exc;
#endif
}

	
void Reactor::stop()
{
	Log("trace") << "[Reactor:" << this << "] Stopping" << endl;

	_stop = true;
	_wakeUp.set();
	_thread.join();
}


void Reactor::attach(const Poco::Net::Socket& socket, const ReactorDelegate& delegate)
{
	//Log("trace") << "[Reactor:" << this << "] Attach: " << socket.impl() << ": " << delegate.event << endl;

	detach(socket, delegate);

	FastMutex::ScopedLock lock(_mutex);	
	
	ReactorDelegate* d = delegate.clone();
	d->socket = Poco::Net::Socket(socket);
	_delegates.push_back(d);
	
	_wakeUp.set();
}


void Reactor::detach(const Poco::Net::Socket& socket, const ReactorDelegate& delegate) 
{	
	//Log("trace") << "[Reactor:" << this << "] Detach: " << socket.impl() << ": " << delegate.event << endl;

	FastMutex::ScopedLock lock(_mutex);
	
	for (DelegateList::iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
		if ((*it)->socket == socket && delegate.equals(*it)) {
			delete *it;
			_delegates.erase(it);
			break;
		}
	}
}


void Reactor::detach(const Poco::Net::Socket& socket)
{
	//Log("trace") << "[Reactor:" << this << "] Detach: " << socket.impl() << endl;

	FastMutex::ScopedLock lock(_mutex);

	for (DelegateList::iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
		if ((*it)->socket == socket) {
			delete *it;
			it = _delegates.erase(it);
		}
		else ++it;
	}
}


void Reactor::dispatch(const Socket& socket, SocketEvent event)
{
	ReactorDelegate* delegate = NULL;
	{
		FastMutex::ScopedLock lock(_mutex);	
		for (DelegateList::iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
			if ((*it)->socket == socket && (*it)->event == event) {
				delegate = *it;
				break;
			}
		}
	}

	if (delegate)
		delegate->dispatch(this, *this, socket, event, 0);
}


Reactor& Reactor::getDefault() 
{
	static Poco::SingletonHolder<Reactor> sh;
	return *sh.get();
}


/*
// ---------------------------------------------------------------------
//
ReactorNotifier::ReactorNotifier(Reactor& reactor, Runner& runner, int queueSize, int dispatchTimeout) : 
	DispatchQueue<ReactorEvent>(runner, queueSize, dispatchTimeout),
	_reactor(reactor)
{
}


void ReactorNotifier::run()
{	
	DispatchQueue<ReactorEvent>::run();
}


void ReactorNotifier::dispatch(ReactorEvent& event)
{
	Log("trace") << "[ReactorNotifier:" << this << "] Broadcast: " << event.delegate.socket.impl() << ": " << event.type << endl;
	assert(event.delegate.locked);
	
	event.delegate.dispatch(&event.reactor, event, 0, 0, 0);
	Log("trace") << "[ReactorNotifier:" << this << "] Broadcast Unlock " << event.delegate.socket.impl() << ": " << event.type << endl;
	assert(event.delegate.locked);
	event.delegate.locked = false;
}


void ReactorNotifier::removeNotifications(const ReactorDelegate& delegate)
{
	Log("trace") << "[ReactorNotifier:" << this << "] Removing: " << &delegate << endl;
	Poco::FastMutex::ScopedLock lock(_mutex);	

	DispatchQueue<ReactorEvent>::Queue::iterator it = _queue.begin();
	while (it != _queue.end()) {
		if ((*it)->delegate == delegate) {
			Log("trace") << "[ReactorNotifier:" << this << "] Deleting Redundant Notification: " << (*it) << endl;
			delete *it;
			it = _queue.erase(it);
		}
		else ++it;
	}
}
*/