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


namespace scy {
namespace sockio {


Transaction::Transaction(Client& client, long timeout) : 
	PacketTransaction<Packet>(timeout, 0, client.loop()), client(client)
{
	debugL("SocketIOTransaction", this) << "create" << endl;
}


Transaction::Transaction(Client& client, const Packet& request, long timeout) : 
	PacketTransaction<Packet>(request, timeout, 0, client.loop()), client(client)
{
	debugL("SocketIOTransaction", this) << "creating" << endl;
}


Transaction::~Transaction() 
{
	debugL("SocketIOTransaction", this) << "destroying" << endl;	
}


bool Transaction::send()
{
	debugL("SocketIOTransaction", this) << "sending: " << _request.toString() << endl;	
	_request.setAck(true);	
	client += packetDelegate(this, &Transaction::onPotentialResponse, 100);
	if (client.send(_request))
		return PacketTransaction<Packet>::send();
	return false;
}

	
void Transaction::onPotentialResponse(void*, Packet& packet)
{
	PacketTransaction<Packet>::onPossibleResponse(packet);
}


bool Transaction::checkResponse(const Packet& packet) 
{
	return _request.id() == packet.id();
}


void Transaction::onResponse()
{
	debugL("SocketIOTransaction", this) << "success" << endl;
	client -= packetDelegate(this, &Transaction::onPotentialResponse);	
	PacketTransaction<Packet>::onResponse();
}


} } // namespace scy::sockio



/*
void Transaction::onPossibleResponse(const Packet& packet)
{
	debugL("SocketIOTransaction", this) << "Response" << endl;
}
*/
