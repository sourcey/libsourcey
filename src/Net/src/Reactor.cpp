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


#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Logger.h"

#include "Poco/Net/NetException.h"
#include "Poco/SingletonHolder.h"


/*
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SecureStreamSocket.h"

//#include "Sourcey/HTTP/Request.h"
*/


using namespace std;
using namespace Poco;
//using namespace Poco::Net;


namespace Sourcey {
namespace Net {


Reactor::Reactor(int timeout) : //Runner& runner, 
	_thread("Reactor"),
	_timeout(timeout),
	_stop(false)	
{
	cout << "Creating" << endl;
	_thread.start(*this);
}


Reactor::~Reactor()
{
	cout << "Destroying" << endl;
	stop();
	cout << "Destroying: OK" << endl;
}


void Reactor::run()
{	
	Poco::Net::Socket::SocketList readable;
	Poco::Net::Socket::SocketList writable;
	Poco::Net::Socket::SocketList error;
	
	while (!_stop)
	{		
		try
		{
			readable.clear();
			writable.clear();
			error.clear();

			if (_delegates.empty()) {				
				Log("trace", this) << "Sleeping" << endl;
				_wakeUp.wait();		
				Log("trace", this) << "Waking up" << endl;
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
				/*
				readable = _readable;
				writable = _writable;
				error = _error;
				*/
			}
				
			//Log("trace", this) << "Select In: " << readable.size() << ":" << writable.size() << ":" << error.size() << endl;
			if (Poco::Net::Socket::select(readable, writable, error, _timeout)) 
			{
				//Log("trace", this) << "Select Out: " << readable.size() << ":" << writable.size() << ":" << error.size() << endl;
				for (Poco::Net::Socket::SocketList::iterator it = readable.begin(); it != readable.end(); ++it) {
					dispatch(*it, SocketReadable);
					
					/*
					for (DelegateList::iterator ait = _delegates.begin(); ait != _delegates.end(); ++ait) {
						if (*(*ait)->socket == *it && (*ait)->event == SocketReadable) {

							Log("trace") << "RREACTOE ################################# RECV" << endl;
					
							char buffer[32];

							Poco::Net::SecureStreamSocket sock(*(*ait)->socket);
							int n = sock.receiveBytes(buffer, sizeof(buffer));

							//int n = ((Poco::Net::SecureStreamSocket&)(*it)).receiveBytes(buffer, sizeof(buffer));
			
							std::string response(buffer, n);
							Log("trace") << "RREACTOE ################################# response: " << response << endl;
						}
					}
					*/
				}
				for (Poco::Net::Socket::SocketList::iterator it = writable.begin(); it != writable.end(); ++it) {
					dispatch(*it, SocketWritable);
					
					/*
					for (DelegateList::iterator ait = _delegates.begin(); ait != _delegates.end(); ++ait) {
						if (*(*ait)->socket == *it && (*ait)->event == SocketWritable) {
	
							Poco::Net::HTTPRequest request("GET", "/");
							stringstream ss;
							request.write(ss);	
							Log("trace") << "RREACTOE  equest Headers: " << ss.str() << endl;
							Poco::Net::SecureStreamSocket sock(*(*ait)->socket);
							//int n = sock.receiveBytes(buffer, sizeof(buffer));
							//Poco::Net::SecureStreamSocket sock(*it);
							int n = sock.sendBytes(ss.str().data(), ss.str().length());
							//((Poco::Net::SecureStreamSocket&)(*it)).sendBytes(ss.str().data(), ss.str().length());
							Log("trace") << "RREACTOE  equest Headers: OK" << endl;
						}
					}
					*/
				}
				for (Poco::Net::Socket::SocketList::iterator it = error.begin(); it != error.end(); ++it)
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
			Log("error", this) << "Unknown Error" << endl;
			//throw;
		}
	}
	
	Log("trace", this) << "Shutdown" << endl;
	Shutdown.dispatch(this);
	Log("trace", this) << "Exiting" << endl;
}


void Reactor::handleException(const Exception& exc)
{
	Log("error", this) << "Error " << exc.displayText() << endl;

#ifdef _DEBUG	
	exc.rethrow();
#endif
}


void Reactor::handleException(const exception& exc)
{
	Log("error", this) << "Error " << exc.what() << endl;

#ifdef _DEBUG	
	throw exc;
#endif
}

	
void Reactor::stop()
{
	Log("trace", this) << "Stopping" << endl;

	_stop = true;
	_wakeUp.set();
	_thread.join();
}


void Reactor::attach(const Poco::Net::Socket& socket, const ReactorDelegate& delegate)
{
	Log("trace", this) << "Attach: " << socket.impl() << ": " << delegate.event << endl;

	detach(socket, delegate);

	FastMutex::ScopedLock lock(_mutex);	
	
	ReactorDelegate* d = delegate.clone();
	d->socket = Poco::Net::Socket(socket);
	//d->socket = &socket;
	_delegates.push_back(d);
	
	_wakeUp.set();	
	
	/*
	switch (delegate.event) {
	case SocketReadable:
		_readable.push_back(socket);
		break;		
	case SocketWritable:
		_writable.push_back(socket);
		break;
	case SocketError:
		_error.push_back(socket);
		break;
	default:
		assert(false);
		break;
	}
	*/

	//Log("trace", this) << "Attach: OK: " << socket.impl() << ": " << delegate.event << endl;
}


void Reactor::detach(const Poco::Net::Socket& socket, const ReactorDelegate& delegate) 
{	
	//Log("trace", this) << "Detach: " << socket.impl() << ": " << delegate.event << endl;

	FastMutex::ScopedLock lock(_mutex);
	
	for (DelegateList::iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
		if ((*it)->socket == socket && delegate.equals(*it)) {
			delete *it;
			_delegates.erase(it);
			break;
		}
	}

	//Log("trace", this) << "Detach: OK: " << socket.impl() << ": " << delegate.event << endl;
}


void Reactor::detach(const Poco::Net::Socket& socket)
{
	//Log("trace", this) << "Detach: " << socket.impl() << endl;

	FastMutex::ScopedLock lock(_mutex);

	for (DelegateList::iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
		if ((*it)->socket == socket) {
			delete *it;
			it = _delegates.erase(it);
		}
		else ++it;
	}
}


void Reactor::dispatch(const Poco::Net::Socket& socket, SocketEvent event)
{
	//Log("trace", this) << "Dispatch: " << socket.impl() << endl;

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


} } // namespace Sourcey::Net


/*


Reactor& Reactor::getDefault() 
{
	static Poco::SingletonHolder<Reactor> sh;
	return *sh.get();
}
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
	Log("trace", this) << "[ReactorNotifier:" << this << "] Broadcast: " << event.delegate.socket.impl() << ": " << event.type << endl;
	assert(event.delegate.locked);
	
	event.delegate.dispatch(&event.reactor, event, 0, 0, 0);
	Log("trace", this) << "[ReactorNotifier:" << this << "] Broadcast Unlock " << event.delegate.socket.impl() << ": " << event.type << endl;
	assert(event.delegate.locked);
	event.delegate.locked = false;
}


void ReactorNotifier::removeNotifications(const ReactorDelegate& delegate)
{
	Log("trace", this) << "[ReactorNotifier:" << this << "] Removing: " << &delegate << endl;
	Poco::FastMutex::ScopedLock lock(_mutex);	

	DispatchQueue<ReactorEvent>::Queue::iterator it = _queue.begin();
	while (it != _queue.end()) {
		if ((*it)->delegate == delegate) {
			Log("trace", this) << "[ReactorNotifier:" << this << "] Deleting Redundant Notification: " << (*it) << endl;
			delete *it;
			it = _queue.erase(it);
		}
		else ++it;
	}
}
*/