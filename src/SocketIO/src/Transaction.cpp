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


#include "Sourcey/SocketIO/Transaction.h"
#include "Sourcey/SocketIO/Client.h"
#include "Sourcey/Logger.h"
#include <iostream>


using namespace std;
using namespace Poco;


namespace scy {
namespace sockio {


Transaction::Transaction(Client& client, long timeout) : 
	PacketTransaction<Packet>(client.runner(), timeout, 0), client(client)
{
	debugL("SocketIOTransaction", this) << "Creating" << endl;
}


Transaction::Transaction(Client& client, const Packet& request, long timeout) : 
	PacketTransaction<Packet>(client.runner(), request, timeout, 0), client(client)
{
	debugL("SocketIOTransaction", this) << "Creating" << endl;
}


Transaction::~Transaction() 
{
	debugL("SocketIOTransaction", this) << "Destroying" << endl;	
}


bool Transaction::send()
{
	debugL("SocketIOTransaction", this) << "Sending" << endl;	
	_request.setAck(true);	
	client += packetDelegate(this, &Transaction::onPotentialResponse, 100);
	if (client.send(_request)) //, true
		return PacketTransaction<Packet>::send();
	return false;
}

	
void Transaction::onPotentialResponse(void*, Packet& packet)
{
	//debugL("SocketIOTransaction", this) << "Potential Response: " 
	//	<< packet.className() << endl;	
	if (process(packet))
		throw StopPropagation();
}


bool Transaction::match(const Packet& packet) 
{
	return _request.id() == packet.id();
}


void Transaction::onResponse()
{
	debugL("SocketIOTransaction", this) << "Response" << endl;
	client -= packetDelegate(this, &Transaction::onPotentialResponse);	
	PacketTransaction<Packet>::onResponse();
}


void Transaction::onComplete()
{
	debugL("SocketIOTransaction", this) << "Complete" << endl;
	//client -= packetDelegate(this, &Transaction::onPotentialResponse);
	PacketTransaction<Packet>::onComplete();
}


} } // namespace scy::sockio