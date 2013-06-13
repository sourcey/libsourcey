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


#ifndef SOURCEY_TCPAsyncConnector_H
#define SOURCEY_TCPAsyncConnector_H


#include "Sourcey/Base.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Runner.h"
#include "Poco/Thread.h"


namespace Scy {
namespace Net {


template <class TSocket>
class TCPAsyncConnector: public Poco::Runnable //Task
	/// This class implements an asynchronous socket connector
	/// which runs in a separate thread.
	/// This is useful for high performance applications where
	/// the processing thread can't be blocked while potentially
	/// time consuming outgoing connections are made.
	/// This class is not implemented as an Task because it may
	/// block for too long.
{
public:
	TCPAsyncConnector(/*Runner& runner, */TSocket& socket, const Address& peerAddress, int timeout = 0) : 
		//Task(runner, false, true),
		_socket(socket),
		_peerAddress(peerAddress),
		_timeout(timeout)
	{	
		LogDebug() << "[TCPAsyncConnector:" << this << "] Creating" << std::endl;
		assert(!_socket.isConnected());		
		_thread.start(*this);
	}	

	virtual ~TCPAsyncConnector()
	{
		LogDebug() << "[TCPAsyncConnector:" << this << "] Destroying" << std::endl;
	}
	
	virtual void run()
	{
		assert(!_socket.isConnected());
		try {
			_socket.connect(_peerAddress, _timeout);
		}
		catch (Poco::Exception& exc) {
			LogError() << "[TCPAsyncConnector:" << this << "] Connection Failed: " << exc.displayText() << std::endl;
		}
		delete this;
	}

protected:
	TSocket&		_socket;
	Address			_peerAddress;
	int				_timeout;
	Poco::Thread	_thread;
};


} } // namespace Scy::Net


#endif // SOURCEY_TCPAsyncConnector_H
