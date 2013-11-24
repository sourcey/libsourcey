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


#include "scy/stun/transaction.h"
#include "scy/net/socket.h"
#include "scy/logger.h"
#include <iostream>


using namespace std;

using scy::net::Transaction;
using scy::net::SocketBase;
using scy::net::Address;
using scy::net::Socket;


namespace scy {
namespace stun {


Transaction::Transaction(Socket& socket, 
						 const Address& peerAddress,
						 long timeout, 
						 int retries, 
						 uv::Loop* loop) : 
	net::Transaction<Message>(socket, peerAddress, timeout, retries, loop) 
{
	debugL("StunTransaction", this) << "Create" << std::endl;

	// Register STUN message creation strategy
	net::Transaction<Message>::factory.registerPacketType<stun::Message>(0);
}


Transaction::~Transaction() 
{
	debugL("StunTransaction", this) << "Destroy" << std::endl;	
}


bool Transaction::checkResponse(const Message& message) 
{
	return net::Transaction<Message>::checkResponse(message) 
		&& _request.transactionID() == message.transactionID();
}


void Transaction::onResponse()
{
	debugL("StunTransaction", this) << "On response" << std::endl;	

	_response.setMethod(_request.methodType());
	_response.setClass(Message::SuccessResponse);
	if (_response.get<stun::ErrorCode>())
		_response.setClass(Message::ErrorResponse);
	else if (_response.methodType() == Message::SendIndication ||
		_response.methodType() == Message::DataIndication)
		_response.setClass(Message::Indication);

	net::Transaction<Message>::onResponse();
}



} } // namespace scy:stun