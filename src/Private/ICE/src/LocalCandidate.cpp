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


#include "Sourcey/ICE/RelayedCandidate.h"
#include "Sourcey/ICE/Component.h"
#include "Sourcey/ICE/Agent.h"
#include "Sourcey/ICE/MediaStream.h"


using namespace std;


namespace Sourcey {
namespace ICE { 


string LocalCandidate::computeFoundation(int type, const Net::IP& ip) 
	// The agent assigns each candidate a foundation. The
	// foundation is an identifier, scoped within a agent. Two candidates
	// MUST have the same foundation ID when all of the following are true:
	//
	// o  they are of the same type (host, relayed, server reflexive, or
	//    peer reflexive).
	//
	// o  their bases have the same IP address (the ports can be different).
	//
	// o  for reflexive and relayed candidates, the STUN or TURN servers
	//    used to obtain them have the same IP address.
	//
	// o  they were obtained using the same transport protocol (TCP, UDP, 
	//    etc.).
	//
	// Similarly, two candidates MUST have different foundations if their
	// types are different, their bases have different IP addresses, the
	// STUN or TURN servers used to obtain them have different IP addresses, 
	// or their transport protocols are different.
{
	return Util::itoa((type << 24) + inet_addr(ip.toString().c_str()));
}


UInt32 LocalCandidate::computePriority(int typePreference, int componentID, int localPreference) 
{
	return (2^24)* typePreference +
           (2^8) * localPreference +
           (2^0) * 256 - componentID;
}


LocalCandidate::LocalCandidate(Component& component, Candidate::Type type, LocalCandidate* base) :
	Candidate(type, component.id(), Candidate::Udp), 
	_base(base), 
	_component(component)
{
}
	

LocalCandidate::~LocalCandidate() 
{
	Log("debug") << "LocalCandidate: Destroying" << endl;

	// Local candidates must be terminate()'d before destruction.
	assert(stateEquals(CandidateState::Terminating));
}


/*
void LocalCandidate::terminate()
{
	Log("debug") << "LocalCandidate: Terminating" << endl;
	setState(this, CandidateState::Terminating);
	Log("debug") << "LocalCandidate: Terminating: OK" << endl;
}
*/


void LocalCandidate::computeFoundation() 
{
	assert(_base);
	setFoundation(LocalCandidate::computeFoundation(type(), _base->transportAddr().host()));
}


void LocalCandidate::computePriority() 
{
	setPriority(LocalCandidate::computePriority(preference(), componentID()));
}


int LocalCandidate::preference() const
{
	switch (_type) {
	case Candidate::Host:
		return _component.stream().agent().options().hostCandidateOrder;
	case Candidate::ServerReflexive:
		return _component.stream().agent().options().srflxCandidateOrder;
	case Candidate::PeerReflexive:
		return _component.stream().agent().options().prflxCandidateOrder;
	case Candidate::Relayed:
		return _component.stream().agent().options().relayCandidateOrder;
	}
	return 0;
}


string LocalCandidate::toSDP() const 
{
	assert(_base);
	// a=candidate:Sc0a80266 1 UDP 2130706431 1.1.1.1 16902 typ srflx raddr 192.168.2.101 rport 16052
    std::ostringstream ost;
	ost << "a=candidate:" 
		<< foundation() << " " 
		<< componentID() << " " 
		<< transportString() << " " 
		<< priority() << " " 
		<< transportIP() << " " 
		<< transportPort()
		<< " typ " << typeString();
	if (type() != Candidate::Host)
		ost << " raddr " << _base->transportAddr().host().toString()
			<< " rport " << _base->transportAddr().port();
	ost << "\r\n";	
	return ost.str();
}


/*
bool LocalCandidate::setState(State state) 
{
	Log("debug", this) << "Setting state to " << state << endl;
	_state = state;
	//bool pruned = !_component.onLocalCandidateStateChange(this);
	//if (pruned)
	//	terminate(); //!pruned;
	return _component.onLocalCandidateStateChange(this);
}


int LocalCandidate::send(const char* data, int size, const Net::Address& peerAddress) 
{
	Log("debug", this) << "Sending Data to " << peerAddress.toString() << endl;

	assert(_component);	
	return _component.dispatch(data, size, peerAddress);
}


int LocalCandidate::send(const IPacket& packet, const Net::Address& peerAddress) 
{
	Log("debug", this) << "Sending STUN to " << peerAddress.toString() << endl;
	
	assert(_component);	
	return _component.dispatch(message, peerAddress);
}


int LocalCandidate::send(STUN::Transaction* transaction) 
{
	Log("debug", this) << "Sending Transaction to " << transaction->peerAddress().toString() << endl;
	_transaction = transaction;
	_transaction->StateChange += delegate(this, &LocalCandidate::onTransactionStateChange);
	return _transaction->send();
}


void LocalCandidate::cancelTransaction() 
{
	Log("debug", this) << "Canceling Current Transaction" << endl;
	if (_transaction) {
		_transaction->StateChange -= delegate(this, &LocalCandidate::onTransactionStateChange);
		//delete _transaction;
		_transaction = NULL;
	}
}


void LocalCandidate::onTransactionStateChange(void* sender, TransactionState& state, const TransactionState&) 
{
	Log("debug", this) << "STUN Transaction State Changed: " << state.toString() << std::endl;

	assert(_transaction);
	if (!_transaction)
		return;

	switch (state.id()) {	
	case TransactionState::Success:
	case TransactionState::Failed:
		ConnectivityCheckResponse.dispatch(this, *_transaction);
		//delete _transaction;
		_transaction = NULL;
		break;
	}
}
*/

/*
void LocalCandidate::onSTUNTransactionComplete(STUN::Transaction* transaction, Net::ISocket* socket) {
	assert(false);

	Log("debug") << "LocalCandidate onSTUNTransactionComplete" << endl;

	if (!_transaction)
		return;

	Log("debug", this) << "Current Transaction Complete: " << transaction->response().toString() << endl;

	ConnectivityCheckSuccess.dispatch(this, *transaction);
	_transaction = NULL;
}


void LocalCandidate::onSTUNTransactionTimeout(STUN::Transaction* transaction) {
	assert(false);

	if (!_transaction)
		return;

	Log("debug", this) << "Current Transaction Timeout: Request: " << transaction->request().toString() << endl;

	ConnectivityCheckFailed.dispatch(this, *transaction);
	_transaction = NULL;
}
*/



} // namespace Sourcey
} // namespace ICE 
