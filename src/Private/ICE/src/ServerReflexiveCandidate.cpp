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


#include "Sourcey/ICE/ServerReflexiveCandidate.h"
#include "Sourcey/ICE/Agent.h"
#include "Sourcey/ICE/MediaStream.h"
#include "Sourcey/ICE/HostCandidate.h"


using namespace std;


namespace Sourcey {
namespace ICE { 


ServerReflexiveCandidate::ServerReflexiveCandidate(Component& component, LocalCandidate* base) :
	LocalCandidate(component, Candidate::ServerReflexive, base), _transaction(NULL)
{
}


ServerReflexiveCandidate::~ServerReflexiveCandidate() 
{
	//Log("debug", this) << "Destroying" << endl;

	//if (_transaction) {
	//	_transaction->StateChange -= delegate(this, &ServerReflexiveCandidate::onTransactionStateChange);	
	//	//delete _transaction;
	//	_transaction = NULL;
	//}
}


void ServerReflexiveCandidate::resolve()
{	
	Log("debug", this) << "Resolving Server Reflexive Address from " << _component.stream().agent().options().stunAddress.toString() << endl;
	
	setState(this, CandidateState::Resolving);
	Net::ISocket* socket = &reinterpret_cast<HostCandidate*>(_base)->socket();
	_transaction = new STUN::Transaction(socket, socket->address(), 
		_component.stream().agent().options().stunAddress, 
		_component.stream().agent().options().maxRequestRetries,
		_component.stream().agent().options().transactionTimeout);
	_transaction->StateChange += delegate(this, &ServerReflexiveCandidate::onTransactionStateChange);	
	_transaction->request().setType(STUN::Message::Binding);
	_transaction->send();
}


void ServerReflexiveCandidate::terminate()
{
	Log("debug", this) << "Terminating" << endl;

	if (_transaction) {
		_transaction->StateChange -= delegate(this, &ServerReflexiveCandidate::onTransactionStateChange);	
		//delete _transaction;
		_transaction = NULL;
	}
	setState(this, CandidateState::Terminating);
}


void ServerReflexiveCandidate::makeRedundant()
{
	setState(this, CandidateState::Redundant);
}


int ServerReflexiveCandidate::send(const char* data, int size, const Net::Address& peerAddress) 
{
	return _base->send(data, size, peerAddress);
}


int ServerReflexiveCandidate::send(const IPacket& packet, const Net::Address& peerAddress) 
{
	return _base->send(packet, peerAddress);
}


void ServerReflexiveCandidate::onTransactionStateChange(void* sender, TransactionState& state, const TransactionState& oldState) 
{
	Log("debug", this) << "ServerReflexiveCandidate: STUN Transaction State Changed: " << state.toString() << endl;
	assert(_transaction);	
	assert(sender == _transaction);
	assert(stateEquals(CandidateState::Resolving));
	if (!_transaction)
		return;

	switch (state.id()) {	
	case TransactionState::Running:
		break;

	case TransactionState::Success:
		// Handle the initial binding response from the server,
		// and set our server reflexive transport address.
		{	
			const STUN::XorMappedAddress* addressAttr = _transaction->response().get<STUN::XorMappedAddress>();
			if (addressAttr && addressAttr->family() == 1) {
				setTransportAddr(addressAttr->address());			
				Log("debug", this) << "Resolved Server Reflexive Address from " << transportAddr().toString() << endl;
				//delete _transaction;
				_transaction = NULL;
				//delete this;
				setState(this, CandidateState::Success); // will cause destruction if pruned...
				return;
			}
			Log("error", this) << "Server Reflexive Binding Response has no Mapped Address" << endl;
			//assert(false);
			//delete _transaction;
			_transaction = NULL;
			setState(this, CandidateState::Failed); // will cause destruction...
		}
		return;

	case TransactionState::Failed:
		//if (stateEquals(CandidateState::Resolving)) {	
		Log("error", this)  
			<< " Timeout: Unable to resolve Server Reflexive Address from " 
			<< _component.stream().agent().options().stunAddress.toString() << endl;
		//delete _transaction;
		_transaction = NULL;
		setState(this, CandidateState::Failed); // will cause destruction...
		//}
		return;
	}

	/*
	LocalCandidate::onTransactionStateChange(sender, state, oldState);
	*/
};


/*
void ServerReflexiveCandidate::onSTUNTransactionComplete(STUN::Transaction* transaction, Net::ISocket* socket) {
	Log("debug") << "ServerReflexiveCandidate onSTUNTransactionComplete: " << transaction << endl;

	if (!_transaction)
		return;

	Log("debug") << "ServerReflexiveCandidate onSTUNTransactionComplete: 1 " << transaction << endl;

	Log("debug", this) << "Current Transaction Complete: " << transaction->response().toString() << endl;

	Log("debug") << "ServerReflexiveCandidate onSTUNTransactionComplete: 2 " << transaction << endl;

	// Handle the initial binding response from the server,
	// and set our server reflexive transport address.
	if (_state == Gathering) {	
		const STUN::XorMappedAddress* addressAttr = transaction->response().get<STUN::XorMappedAddress>();
		if (addressAttr && addressAttr->family() == 1) {
			setTransportAddr(addressAttr->address());			
			Log("debug", this) << "Resolved Server Reflexive Address from " << transportAddr().toString() << endl;
			_transaction = NULL;
			setState(this, CandidateState::Success);
			return;
		}
		Log("error", this) << "Server Reflexive Binding Response has no Mapped Address" << endl;
		assert(false);
		_transaction = NULL;
		setState(this, CandidateState::Failed);
		return;
	}

	ConnectivityCheckSuccess.dispatch(this, *transaction);
	_transaction = NULL;
}


void ServerReflexiveCandidate::onSTUNTransactionTimeout(STUN::Transaction* transaction) {
	if (!_transaction)
		return;

	Log("debug", this) << "Current Transaction Timeout: Request: " << transaction->request().toString() << endl;

	if (_state == Gathering) {	
		Log("error", this)  
			<< " Timeout: Unable to resolve Server Reflexive Address from " 
			<< _component.stream().agent().options().stunAddress.toString() << endl;
		setState(this, CandidateState::Failed);
	}

	ConnectivityCheckFailed.dispatch(this, *transaction);
	_transaction = NULL;
}
*/




	/*
void ServerReflexiveCandidate::resolveServerReflexiveAddr() 
{

	STUN::Transaction* transaction = new STUN::Transaction(//this, 
		_component.socket(), 
		_component.stream().agent().options().stunAddress, 
		_component.stream().agent().options().transactionTimeout);
	transaction->request().setType(STUN::Message::Binding);
	transaction->send();
	*/
	/*


	STUN::Transaction* transaction = new STUN::Transaction(
		//reinterpret_cast<STUN::TransactionHandler*>(this), 
		this, 
		_component.socket(), 
		_component.stream().agent().options().stunAddress, 
		_component.stream().agent().options().transactionTimeout);
	transaction->request().setType(STUN::Message::Binding);
	transaction->send();

}
	*/


} // namespace Sourcey
} // namespace ICE 