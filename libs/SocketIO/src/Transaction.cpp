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


#include "Sourcey/SocketIO/Transaction.h"
#include "Sourcey/SocketIO/Socket.h"
#include "Sourcey/Logger.h"
#include <iostream>


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace SocketIO {


Transaction::Transaction(Runner& runner, Socket& socket, int maxAttempts, long timeout) : 
	ITransaction<Packet>(runner, maxAttempts, timeout), socket(socket)
{
	Log("debug") << "[SocketIOTransaction::" << this << "] Initializing" << endl;
}


Transaction::Transaction(Runner& runner, Socket& socket, const Packet& request, int maxAttempts, long timeout) : 
	ITransaction<Packet>(runner, request, maxAttempts, timeout), socket(socket)
{
	Log("debug") << "[SocketIOTransaction::" << this << "] Initializing" << endl;
}


Transaction::~Transaction() 
{
	Log("debug") << "[SocketIOTransaction::" << this << "] Destroying" << endl;	
}


bool Transaction::send()
{
	Log("debug") << "[SocketIOTransaction::" << this << "] Sending" << endl;	
	_request.setAck(true);	
	socket += packetDelegate(this, &Transaction::onPotentialResponse, 100);
	if (socket.send(_request), true)
		return ITransaction::send();
	return false;
}

	
void Transaction::onPotentialResponse(void*, Packet& packet)
{
	//Log("debug") << "[SocketIOTransaction::" << this << "] Potential Response: " 
	//	<< packet.className() << endl;	
	if (process(packet))
		throw StopPropagation();
}


bool Transaction::match(const Packet& packet) 
{
	Log("debug") << "[SocketIOTransaction::" << this << "] Match: "
		<< "\n\tRequest: " << _request.id()
		<< "\n\tResponse: " << packet.id()
		<< endl;	

	return _request.id() == packet.id();
}


void Transaction::onComplete()
{
	Log("debug") << "[SocketIOTransaction::" << this << "] Closing" << endl;

	socket -= packetDelegate(this, &Transaction::onPotentialResponse);
	
	ITransaction::onComplete();
}


} } // namespace Sourcey::SocketIO