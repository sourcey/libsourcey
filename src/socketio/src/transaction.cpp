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


#include "scy/socketio/transaction.h"
#include "scy/socketio/client.h"
#include "scy/logger.h"
#include <iostream>


using std::endl;


namespace scy {
namespace sockio {


Transaction::Transaction(Client& client, long timeout) : 
	PacketTransaction<Packet>(timeout, 0, client.socket().base().loop()), client(client)
{
	DebugLS(this) << "Create" << endl;
}


Transaction::Transaction(Client& client, const Packet& request, long timeout) : 
	PacketTransaction<Packet>(request, timeout, 0, client.socket().base().loop()), client(client)
{
	DebugLS(this) << "Create" << endl;
}


Transaction::~Transaction() 
{
	DebugLS(this) << "Destroy" << endl;	
}


bool Transaction::send()
{
	DebugLS(this) << "Send: " << _request.id() << endl;	
	_request.setAck(true);	
	client += packetDelegate(this, &Transaction::onPotentialResponse, 100);
	if (client.send(_request))
		return PacketTransaction<Packet>::send();
	return false;
}

	
void Transaction::onPotentialResponse(void*, Packet& packet)
{
	PacketTransaction<Packet>::handlePotentialResponse(packet);
}


bool Transaction::checkResponse(const Packet& packet) 
{
	return _request.id() == packet.id();
}


void Transaction::onResponse()
{
	DebugLS(this) << "On success" << endl;
	client -= packetDelegate(this, &Transaction::onPotentialResponse);	
	PacketTransaction<Packet>::onResponse();
}


} } // namespace scy::sockio



/*
void Transaction::handlePotentialResponse(const Packet& packet)
{
	DebugLS(this) << "Response" << endl;
}
*/
