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


#include "Sourcey/ICE/RelayedCandidate.h"
#include "Sourcey/ICE/Component.h"
#include "Sourcey/ICE/Agent.h"
#include "Sourcey/ICE/MediaStream.h"


//using namespace Poco;
using namespace std;


namespace Sourcey {
namespace ICE { 


RelayedCandidate::RelayedCandidate(Component& component, const TURN::Client::Options& options) :
	LocalCandidate(component, Candidate::Relayed, this), // RFC 5245: The base of a relayed candidate is that candidate itself. 
	_client(*this, options, _component.stream().agent().reactor()) //, _component.stream().agent().runner()
{
	// If we are the Controlled agent we will already have a list of 
	// remote candidates to install permissions for.
	if (!component.stream().agent().isControlling()) {
		RemoteCandidateList remoteCandidates = _component.stream().getRemoteCandidatesForComponent(_component.id());
		assert(!remoteCandidates.empty());
		if (!remoteCandidates.empty()) {
			for (RemoteCandidateList::const_iterator it = remoteCandidates.begin(); it != remoteCandidates.end(); ++it) {	
				_client.addPermission((*it)->transportAddr().host());
				Log("debug", this) << "Creating TURN Permissions for: " << (*it)->transportIP() << endl;
			}
		}
	}

	_client.initiate();

	// This well fire on initial offer if we are the Controlling client,
	// enabling up to install TURN permissions for our remote peers.
	component.stream().RemoteCandidateAdded += delegate(this, &RelayedCandidate::onRemoteCandidateAdded);
}

	
RelayedCandidate::~RelayedCandidate()
{
	//Log("debug", this) << "Destroying" << endl;
}

	
void RelayedCandidate::resolve()
{	
	Log("debug", this) << "Resolving" << endl;

	setState(this, CandidateState::Resolving);
	_client.sendAllocate();
}


void RelayedCandidate::terminate()
{
	Log("debug", this) << "Terminating" << endl;

	_component.stream().RemoteCandidateAdded -= delegate(this, &RelayedCandidate::onRemoteCandidateAdded);
	_client.terminate();
	setState(this, CandidateState::Terminating);
}


void RelayedCandidate::makeRedundant()
{
	setState(this, CandidateState::Redundant);
}


void RelayedCandidate::onRemoteCandidateAdded(void*, RemoteCandidate& candidate) 
{
	Log("debug", this) << "RemoteCandidateAdded: " << candidate.toString() << endl;

	// Install permissions for all new remote candidates
	_client.addPermission(candidate.transportAddr().host());
	_client.sendCreatePermission();
}


int RelayedCandidate::send(const char* data, int size, const Net::Address& peerAddress) 
{
	Log("debug") << "RelayedCandidate Sending Data to: " << peerAddress.toString() << endl;

	try
	{
		_client.sendData(data, size, peerAddress);
	}
	catch (Exception& exc)
	{
		Log("error", this) << "SEND ERROR: " << exc.displayText() << endl;
		// Handle?
		return 0;
	}
	return size;
}


int RelayedCandidate::send(const IPacket& packet, const Net::Address& peerAddress) 
{
	Log("debug") << "RelayedCandidate Sending STUN to: " << peerAddress.toString() << endl;

	Buffer buf;
	packet.write(buf);
	return send(buf.data(), buf.size(), peerAddress);
}


void RelayedCandidate::onRelayStateChange(TURN::Client&, TURN::ClientState& state, const TURN::ClientState&) 
{
	Log("debug") << "TURN Initiator: State Changed: " << state.toString() << endl;

	switch(state.id()) {
	case TURN::ClientState::Waiting:				
		break;
	case TURN::ClientState::Allocating:				
		break;
	case TURN::ClientState::Authorizing:
		setTransportAddr(_client.relayedAddress());
		setState(this, CandidateState::Success); // will cause destruction if pruned...
		break;
	case TURN::ClientState::Success:		
		// permissions have been created, data can flow...

		/*
		setState(this, CandidateState::Success); // will cause destruction if pruned...
			// If we are the Controlled agent we will already have a list of 
			// remote candidates to install permissions for.
			TURN::IPList peerIPs;
			RemoteCandidateList remoteCandidates = _component.stream().getRemoteCandidatesForComponent(_component.id());
			if (!remoteCandidates.empty()) {
				for (RemoteCandidateList::const_iterator it = remoteCandidates.begin(); it != remoteCandidates.end(); ++it) {	
					peerIPs.push_back((*it)->transportAddr().host());
					Log("debug", this) << "Creating TURN Permissions for: " << (*it)->transportAddr().toString() << endl;
				}
				try
				{
					_client.sendCreatePermission(peerIPs);
				}
				catch (Exception& exc)
				{
					Log("error", this) << "Permission Request ERROR: " << exc.displayText() << endl;
					// Handle?
					return 0;
				}
			}
		}
		*/
		break;
	case TURN::ClientState::Failed:	
		setState(this, CandidateState::Failed); // will cause destruction...			
		break;
	case TURN::ClientState::Terminated:			
		break;
	}
}


void RelayedCandidate::onRelayedData(TURN::Client&, const char* data, int size, const Net::Address& peerAddress) 
{
	Log("debug", this) << "Data Indication Received" << endl;

	Buffer buf(data, size); // make TURN dispatch buffers for speedup
	STUN::Message message;
	if (message.read(buf))
		STUNMessageReceived.emit(this, message, peerAddress);
	else {
		DataPacket packet((unsigned char*)data, size); // data is only referenced
		_component.stream().agent().observer().onMediaOnData(
			_component.stream().agent(), _component.stream(), 
			_component, packet, _transportAddr, peerAddress);
	}
}

	/*
	//bool isSTUN = message.read(buf);
		
	//Log("debug", this) << "Data Indication Received: isSTUN: " << isSTUN << endl;
	//Log("debug", this) << "Data Indication Received: _transaction: " << _transaction << endl;
	//assert(false);
	
	// A little bit of hackery here - we want it to appear that the
	// packet was received from our component socket so we will send
	// the data driently there.
	// A little bit of hackery here - if we have an active connectivity
	// check running then check if the received data is our transaction
	// response, otherwise treat it as a binding reques and send it 
	// directly to our component socket's receive() method.
	if (isSTUN &&
		_transaction &&
		_transaction->receive(message, _component.socket().address(), peerAddress)) {

		// We have our STUN respose! Callbacks have been handled.
		Log("debug", this) << "Data Indication Received @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	}
	else {
		// Pass the data directly to our component...
		_component.socket().receive(data, size, peerAddress);
	}
		{		
			// If we are the Controlled agent we will already have a list of 
			// remote candidates to install permissions for.
			TURN::IPList peerIPs;
			RemoteCandidateList remoteCandidates = _component.stream().getRemoteCandidatesForComponent(_component.id());
			if (!remoteCandidates.empty()) {
				for (RemoteCandidateList::const_iterator it = remoteCandidates.begin(); it != remoteCandidates.end(); ++it) {	
					peerIPs.push_back((*it)->transportAddr().host());
					Log("debug", this) << "Creating TURN Permissions for: " << (*it)->transportAddr().toString() << endl;
				}
				_client.sendCreatePermission(peerIPs);
			}
			//else
			//	_client.setState(TURN::ClientState::Success);
		}
			*/
/*
void RelayedCandidate::onAllocationCreated(TURN::Client* client, TURN::ClientAllocation* allocation) 
{
	Log("debug", this) << "TURN Allocation Created" << endl;		
	assert(!_allocation);

	_allocation = allocation;
	setTransportAddr(_allocation->relayedAddress());
	if (setState(CandidateState::Success)) { // will cause destruction if pruned...

		// If we are the Controlled agent we will already have a list of 
		// remote candidates to install permissions for.
		TURN::IPList peerIPs;
		RemoteCandidateList remoteCandidates = _component.stream().getRemoteCandidatesForComponent(_component.id());
		if (!remoteCandidates.empty()) {
			for (RemoteCandidateList::const_iterator it = remoteCandidates.begin(); it != remoteCandidates.end(); ++it) {	
				peerIPs.push_back((*it)->transportAddr().host());
				Log("debug", this) << "Creating TURN Permissions for: " << (*it)->transportAddr().toString() << endl;
			}
			try
			{
				_client.sendCreatePermission(peerIPs);
			}
			catch (Exception& exc)
			{
				Log("error", this) << "Permission Request ERROR: " << exc.displayText() << endl;
				// Handle?
				return 0;
			}
		}

	}
}


void RelayedCandidate::onAllocationPermissionsCreated(TURN::Client* client, TURN::ClientAllocation* allocation)
{
	Log("debug", this) << "Allocation Permissions Created" << endl;
}


void RelayedCandidate::onAllocationFailed(TURN::Client* client, int errorCode)
{
	Log("error", this) << "Allocation Failed: " << errorCode << endl;
	setState(this, CandidateState::Failed); // will cause destruction...
}


void RelayedCandidate::onAllocationDeleted(TURN::Client* client, TURN::ClientAllocation* allocation) 
{
	Log("debug", this) << "Allocation Deleted" << endl;

	// TODO: Create a new one...
	setState(this, CandidateState::Failed); // will cause destruction...
}


// If the transaction failes for any reason - ie. the
// server is down we will receive the callback here.
void RelayedCandidate::onTransactionResponse(TURN::Client* client, const TransactionState& state, const STUN::Message& response)
{
	Log("debug") << "RelayedCandidate: Transaction Response State: " << state.toString() << endl;
	Log("debug") << "RelayedCandidate: Transaction Response Response: " << response.toString() << endl;

	if (state.id() == TransactionState::Failed) {
		setState(this, CandidateState::Failed); // will cause destruction...
		return;
	}

	else if (state.id() == TransactionState::Success &&
		response.state() == STUN::Message::ErrorResponse &&
		response.get<STUN::MessageIntegrity>() != NULL)	
	{ 		
		//Log("error", this) << "STUN Transaction Failed: " << response.toString() << endl;
		if (response.type() == STUN::Message::Allocate)	 {
			setState(this, CandidateState::Failed); // will cause destruction...
			return;
		}

		// Handle other message failures... ie CreatePermission?
	}
}


void RelayedCandidate::onRelayedData(TURN::Client* client, const char* data, int size, const Net::Address& peerAddress) 
{
	Log("debug", this) << "Data Indication Received %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;

	// A little bit of hackery here - if we have an active connectivity
	// check running then check if the received data is our transaction
	// response, otherwise treat it as a binding reques and send it 
	// directly to our component socket's receive() method.
	Buffer buf(data, size);
	STUN::Message message;
	bool isSTUN = message.read(buf);
		
	//Log("debug", this) << "Data Indication Received: isSTUN: " << isSTUN << endl;
	//Log("debug", this) << "Data Indication Received: _transaction: " << _transaction << endl;

	assert(false);




	// A little bit of hackery here - we want it to appear that the
	// packet was received from our component socket so we will send
	// the data driently there.
}


	*/
	/*
	if (isSTUN &&
		_transaction &&
		_transaction->receive(message, _component.socket().address(), peerAddress)) {

		// We have our STUN respose! Callbacks have been handled.
		Log("debug", this) << "Data Indication Received @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	}
	else {
		// Pass the data directly to our component...
		_component.socket().receive(data, size, peerAddress);
	}
				try
				{
				}
				catch (Exception& exc)
				{
					Log("error", this) << "Permission Request ERROR: " << exc.displayText() << endl;
					// Handle?
					return 0;
				}
	*/

/*
bool RelayedCandidate::send(STUN::Transaction* transaction) {
	Log("debug") << "RelayedCandidate Sending STUN Transaction to: " << transaction->peerAddress().toString() << endl;

	assert(_component);	
	//_transaction = transaction; // The transaction pointer is automatically deleted on response or timeout.
	//_component.socket()->TransactionResponse += delegate(this, &RelayedCandidate::onTransactionResponse);
	//_component.socket()->TransactionTimeout += delegate(this, &RelayedCandidate::onTransactionTimeout);
	//return _component.emit(_transaction);
	return false;
}
	

	// Ordinarily we would set permissions here, but we do not know
	// our prospective peers yet so we will beed to wait until we 
	// start sending data.

	//setState(CandidateState::Success);
	//_state = Success;
	//_component.stream().onLocalCandidateStateChange(this);
*/


/*
void RelayedCandidate::onTransactionTimeout(STUN::TransactionIPacket& packet) {
	Log("debug", this) << "Relayed Candidate Failed: Timeout" << endl;
}


void RelayedCandidate::onRelayedData(STUN::DataPacket& packet) {
	assert(false);
}
*/


} // namespace Sourcey
} // namespace ICE 