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


#include "Sourcey/STUN/Transaction.h"
#include "Sourcey/Net/Socket.h"
#include "Sourcey/Logger.h"
#include <iostream>


using namespace std;

using scy::net::Transaction;
using scy::net::SocketBase;
using scy::net::Address;
using scy::net::Socket;


namespace scy {
namespace stun {


Transaction::Transaction(Socket& socket, 
						 //const Address& localAddress, 
						 const Address& peerAddress,
						 long timeout, 
						 int retries, 
						 uv::Loop& loop) : 
	net::Transaction<Message>(socket, peerAddress, timeout, retries, loop) //localAddress, 
{
	debugL("STUNTransaction", this) << "Initializing" << std::endl;

	// Register STUN message creation strategy
	net::Transaction<Message>::factory.registerPacketType<stun::Message>(0);
}


Transaction::~Transaction() 
{
	debugL("STUNTransaction", this) << "Destroying" << std::endl;	
}


bool Transaction::checkResponse(const Message& message) 
{
	return net::Transaction<Message>::checkResponse(message) 
		&& _request.transactionID() == message.transactionID();
}


void Transaction::onSuccess()
{
	debugL("STUNTransaction", this) << "On Response" << std::endl;	
	_response.setType(_request.type());
	_response.setState(Message::SuccessResponse);
	if (_response.get<stun::ErrorCode>())
		_response.setState(Message::ErrorResponse);
	else if (_response.type() == Message::SendIndication ||
		_response.type() == Message::DataIndication)
		_response.setState(Message::Indication);

	net::Transaction<Message>::onSuccess();
}



} } // namespace scy::STUN





/*
bool Transaction::match(const Message& message, const net::Address& localAddress, const net::Address& peerAddress)
{
	debugL("STUNTransaction", this) << "Match" << std::endl;	

	return _request.transactionID() == message.transactionID()
		&& _localAddress == localAddress
		&& _peerAddress == peerAddress;
}
*/


/*
	response = response;
void Transaction::onPacketReceived(void* sender, Message& message) //, net::SocketBase& socket, const net::Address& localAddress, const net::Address& peerAddress
{
	net::PacketInfo* source = reinterpret_cast<net::PacketInfo*>(message.info);
	assert(source);
	if (!source)
		return;

	assert(&socket);
	assert(&socket == &source->socket);

	if (match(message, source->localAddress, source->peerAddress)) {	
		update(message);
		socket.detach(PolymorphicDelegate<Transaction, Message>(this, &Transaction::onPacketReceived));
		Timer::getDefault().stop(TimerCallback<Transaction>(this, &Transaction::onTransactionTimeout));
		setState(this, net::TransactionState::Success);
		throw StopPropagation();
	}
}
*/

/*
void Transaction::cancel() 
{
	debugL("STUNTransaction", this) << "Cancelling" << std::endl;
	socket.detach(PolymorphicDelegate<Transaction, Message>(this, &Transaction::onPacketReceived, 0));
	Timer::getDefault().stop(TimerCallback<Transaction>(this, &Transaction::onTransactionTimeout));
}

bool Transaction::receive(const Message& message, const net::Address& localAddress, const net::Address& peerAddress)
{	
	if (match(message, localAddress, peerAddress)) {	
		update(message);
		debugL("STUNTransaction", this) << "Transaction Response Received: " << response.toString() << std::endl;
		Timer::getDefault().stop(TimerCallback<Transaction>(this, &Transaction::onTransactionTimeout));
		setState(this, net::TransactionState::Success);
		return true;
	}
}
*/