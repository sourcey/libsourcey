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


#include "Sourcey/ICE/Component.h"
#include "Sourcey/ICE/Agent.h"
#include "Sourcey/ICE/IAgentObserver.h"
#include "Sourcey/ICE/MediaStream.h"
#include "Sourcey/ICE/CheckList.h"
#include "Sourcey/ICE/LocalCandidate.h"
#include "Sourcey/ICE/HostCandidate.h"
#include "Sourcey/ICE/RelayedCandidate.h"
#include "Sourcey/ICE/ServerReflexiveCandidate.h"
#include "Sourcey/ICE/CandidatePair.h"
#include "Sourcey/RTP/Packet.h"
#include "Poco/Net/NetworkInterface.h"


using Poco::Net::NetworkInterface;
using namespace std;


namespace Sourcey {
namespace ICE { 


Component::Component(MediaStream& stream, int id , UInt16 localPort) : 
	_stream(stream), _id(id), _localPort(localPort),
	_defaultLocalCandidate(NULL), _defaultRemoteCandidate(NULL),
	_selectedCandidatePair(NULL)
{
	//Log("debug", this) << "Creating" << endl;
}


Component::~Component() 
{
	//Log("debug", this) << "Destroying" << endl;
	
	terminateGatheringCandidates();
}


void Component::terminateGatheringCandidates()
{
	for (LocalCandidateList::iterator it = _gatheringCandidates.begin(); it != _gatheringCandidates.end(); ++it) {
		(*it)->terminate();
		delete *it;
	}
	_gatheringCandidates.clear();
}


void Component::startGatheringLocalCandidates() 
{
	Log("debug", this) << "Gathering Candidates" << endl;
	//assert(valid());

	// Host Candidates

	// The first step is to gather host candidates.  Host candidates are
	// obtained by binding to ports (typically ephemeral) on a IP address
	// attached to an interface (physical or virtual, including VPN
	// interfaces) on the host.

	// For each UDP media stream the agent wishes to use, the agent SHOULD
	// obtain a candidate for each component of the media stream on each IP
	// address that the host has.  It obtains each candidate by binding to a
	// UDP port on the specific IP address.  A host candidate (and indeed
	// every candidate) is always associated with a specific component for
	// which it is a candidate.  Each component has an ID assigned to it,
	// called the component ID.  For RTP-based media streams, the RTP itself
	// has a component ID of 1, and RTCP a component ID of 2.  If an agent
	// is using RTCP, it MUST obtain a candidate for it.  If an agent is
	// using both RTP and RTCP, it would end up with 2*K host candidates if
	// an agent has K IP addresses.

	// The base for each host candidate is set to the candidate itself.

	// Server Reflexive and Relayed Candidates

	// Agents SHOULD obtain relayed candidates and SHOULD obtain server
	// reflexive candidates.  These requirements are at SHOULD strength to
	// allow for provider variation.  Use of STUN and TURN servers may be
	// unnecessary in closed networks where agents are never connected to
	// the public Internet or to endpoints outside of the closed network.
	// In such cases, a full implementation would be used for agents that
	// are dual stack or multihomed, to select a host candidate.  Use of
	// TURN servers is expensive, and when ICE is being used, they will only
	// be utilized when both endpoints are behind NATs that perform address
	// and port dependent mapping.  Consequently, some deployments might
	// consider this use case to be marginal, and elect not to use TURN
	// servers.  If an agent does not gather server reflexive or relayed
	// candidates, it is RECOMMENDED that the functionality be implemented
	// and just disabled through configuration, so that it can be re-enabled
	// through configuration if conditions change in the future.
	// If an agent is gathering both relayed and server reflexive
	// candidates, it uses a TURN server.  If it is gathering just server
	// reflexive candidates, it uses a STUN server.

	// The agent next pairs each host candidate with the STUN or TURN server
	// with which it is configured or has discovered by some means.  If a
	// STUN or TURN server is configured, it is RECOMMENDED that a domain
	// name be configured, and the DNS procedures in [RFC5389] (using SRV
	// records with the "stun" service) be used to discover the STUN server,
	// and the DNS procedures in [RFC5766] (using SRV records with the
	// "turn" service) be used to discover the TURN server.

	// This specification only considers usage of a single STUN or TURN
	// server.  When there are multiple choices for that single STUN or TURN
	// server (when, for example, they are learned through DNS records and
	// multiple results are returned), an agent SHOULD use a single STUN or
	// TURN server (based on its IP address) for all candidates for a
	// particular session.  This improves the performance of ICE.  The
	// result is a set of pairs of host candidates with STUN or TURN
	// servers.  The agent then chooses one pair, and sends a Binding or
	// Allocate request to the server from that host candidate.  Binding
	// requests to a STUN server are not authenticated, and any ALTERNATE-
	// SERVER attribute in a response is ignored.  Agents MUST support the
	// backwards compatibility mode for the Binding request defined in
	// [RFC5389].  Allocate requests SHOULD be authenticated using a long-
	// term credential obtained by the client through some other means.

	// Every Ta milliseconds thereafter, the agent can generate another new
	// STUN or TURN transaction.  This transaction can either be a retry of
	// a previous transaction that failed with a recoverable error (such as
	// authentication failure), or a transaction for a new host candidate
	// and STUN or TURN server pair.  The agent SHOULD NOT generate
	// transactions more frequently than one every Ta milliseconds.  See
	// Section 16 for guidance on how to set Ta and the STUN retransmit
	// timer, RTO.

	// The agent will receive a Binding or Allocate response.  A successful
	// Allocate response will provide the agent with a server reflexive
	// candidate (obtained from the mapped address) and a relayed candidate
	// in the XOR-RELAYED-ADDRESS attribute.  If the Allocate request is
	// rejected because the server lacks resources to fulfill it, the agent
	// SHOULD instead send a Binding request to obtain a server reflexive
	// candidate.  A Binding response will provide the agent with only a
	// server reflexive candidate (also obtained from the mapped address).
	// The base of the server reflexive candidate is the host candidate from
	// which the Allocate or Binding request was sent.  The base of a
	// relayed candidate is that candidate itself.  If a relayed candidate
	// is identical to a host candidate (which can happen in rare cases),
	// the relayed candidate MUST be discarded.

	gatherHostCandidates();
	gatherServerReflexiveCandidates();
	gatherRelayedCandidates();
	
	setFlag(LocalCandidatesCreated);

	// If server reflexive or relayed candidates are disabled can notify the 
	// media stream that candidate gathering is complete.
	// NOTE: The LocalCandidatesGathered flag will be set via the media stream.
	//_stream.trySetLocalCandidatesGathered();
}


void Component::onCandidateStateChange(void* sender, CandidateState& state, const CandidateState&)
{
	Log("debug", this) << "Local Candidate State Changed: " << state.toString() << endl;
	LocalCandidate* candidate = reinterpret_cast<LocalCandidate*>(sender);
	
	bool pruned = false;

	switch (state.id()) {	
	case CandidateState::Waiting:
		break;

	case CandidateState::Resolving:
		_gatheringCandidates.push_back(candidate);
		break;

	case CandidateState::Success:
		for (LocalCandidateList::iterator it = _gatheringCandidates.begin(); it != _gatheringCandidates.end(); ++it) {	
			if (*it == candidate) {
				_gatheringCandidates.erase(it);
				break;
			} 
		}

		// NOTE: The candidate will be deleted if pruned!
		// TODO: Disconnect state changed event callback if pruned?
		pruned = !_stream.tryAddLocalCandidate(candidate);
		
		// Try to set our default local candidate from 
		// among our successful candidates.
		if (!pruned) {
			
			// Filter unwanted types.
			if ((candidate->type() == Candidate::Host && _stream.agent().options().disableHost) ||
				(candidate->type() == Candidate::ServerReflexive && _stream.agent().options().disableSTUN) ||
				(candidate->type() == Candidate::Relayed && _stream.agent().options().disableRelay))
				;

			// Set the first candidate unset. 
			else if (!_defaultLocalCandidate)
				_defaultLocalCandidate = candidate;

			// Other candidates will override based on
			// preference.
			else if (candidate->preference() > 
				_defaultLocalCandidate->preference())
				_defaultLocalCandidate = candidate;
		}
		break;

	case CandidateState::Failed:
		for (LocalCandidateList::iterator it = _gatheringCandidates.begin(); it != _gatheringCandidates.end(); ++it) {	
			if (*it == candidate) {
				_gatheringCandidates.erase(it);
				break;
			} 
		}
		pruned = true;
		candidate->StateChange -= delegate(this, &Component::onCandidateStateChange);	
		candidate->terminate();
		//_stream.agent().runner().deleteLater<LocalCandidate>(candidate);
		break;

	case CandidateState::Terminating:
		//delete candidate;
		return;
	}
		
	//Log("debug", this) << "Component Candidates Gathered: " << hasFlag(Component::LocalCandidatesCreated) << endl;
	//Log("debug", this) << "Component Candidates Gathered: " << hasFlag(Component::LocalCandidatesGathered) << endl;
	//Log("debug", this) << "Component Candidates Gathered: " << (_stream.agent().options().disableSTUN && _stream.agent().options().disableRelay) << endl;
	//Log("debug", this) << "Component Candidates Gathered: " << (_gatheringCandidates.size()) << endl;
	
	if (_gatheringCandidates.empty() &&
		!hasFlag(Component::LocalCandidatesGathered) &&
		(hasFlag(Component::LocalCandidatesCreated) ||
		 (_stream.agent().options().disableSTUN &&
		  _stream.agent().options().disableRelay))) {		
		setFlag(Component::LocalCandidatesGathered);
		_stream.onComponentLocalCandidatesGathered(this);
	}
}


void Component::setDefaultLocalCandidate(LocalCandidate* candidate) 
{ 
	Log("debug", this) << "Setting Default Local Candidate: " << candidate->toString() << endl;

	_defaultLocalCandidate = candidate; 
}


void Component::setDefaultRemoteCandidate(RemoteCandidate* candidate) 
{ 
	Log("debug", this) << "Setting Default Remote Candidate: " << candidate->toString() << endl;

	_defaultRemoteCandidate = candidate; 
}


void Component::setSelectedCandidatePair(CandidatePair* pair)
{
	Log("debug", this) << "Setting Selected Candidate Pair: " << pair->toString() << endl;
	assert(pair->hasFlag(CandidatePair::Selected));	
				
	_stream.agent().observer().onSelectedPairsChanged(_stream.agent(), _stream, *this, pair, _selectedCandidatePair);

	_selectedCandidatePair = pair;
}


void Component::sendPacket(void*, IPacket& packet) 
{	
	assert(_selectedCandidatePair);	
	assert(_selectedCandidatePair->hasFlag(CandidatePair::Selected));	
	assert(_id == 1);	
	
	Log("trace", this) << "Sending Data Packet" << endl;

	// send media packet to our peer
	_selectedCandidatePair->send(packet);
}


void Component::sendRTPPacket(void*, RTP::Packet& packet) 
{	
	assert(_selectedCandidatePair);	
	assert(_selectedCandidatePair->hasFlag(CandidatePair::Selected));	
	assert(_id == 1);	
	
	Log("trace", this) << "Sending RTP Data Packet" << endl;

	// send media packet to our peer
	_selectedCandidatePair->send(packet);
}


void Component::sendRTCPPacket(void*, RTP::RTCP::Packet& packet) 
{	
	assert(_selectedCandidatePair);	
	assert(_selectedCandidatePair->hasFlag(CandidatePair::Selected));
	assert(_id == 2);	

	Log("trace", this) << "Sending RTCP Packet" << endl;

	// send media packet to our peer
	_selectedCandidatePair->send(packet);
}


// ---------------------------------------------------------------------
//
// Host Candidates
//
// ---------------------------------------------------------------------
void Component::gatherHostCandidates() 
{	
	Log("debug", this) << "Gathering Host Candidates" << endl;
	
	// Get list of available IP4 interfaces and create candidates for each.
	NetworkInterface::NetworkInterfaceList list = NetworkInterface::list();
	assert (!list.empty());
	for (NetworkInterface::NetworkInterfaceList::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		if (it->address().toString() == "127.0.0.1" &&
			_stream.agent().options().disableLocalhost)	
			continue;

		// Initialize Host Candidate
		try {
			HostCandidate* c = new HostCandidate(*this);
			c->StateChange += delegate(this, &Component::onCandidateStateChange);	
			c->setTransport(Candidate::Udp);
			c->setTransportAddr(Net::Address(it->address(), _localPort));
			// NOTE: The candidate ptr may be deleted on failure, 
			// in which case an exception will be thrown.
			c->resolve(); 

			// The component port will be updated here if unset.
			if (_localPort == 0)
				_localPort = c->transportPort();

			Log("debug", this) << "Host Candidate Gathered" << endl;
		} 
		catch (Poco::Exception& exc) {
			Log("error", this) << ": " << exc.displayText() << endl;
		}

		/*
		std::cout << "Name:        " << it->name() << std::endl;
		std::cout << "DisplayName: " << it->displayName() << std::endl;
		std::cout << "Address:     " << it->address().toString() << std::endl;
		std::cout << "Subnet:      " << it->subnetMask().toString() << std::endl;
		std::cout << "Broadcast:   " << it->broadcastAddress().toString() << std::endl;
		std::cout << "Index:       " << it->index() << std::endl;
		*/
	}
}


// ---------------------------------------------------------------------
//
// Server Reflexive Candidates
//
// ---------------------------------------------------------------------
void Component::gatherServerReflexiveCandidates() 
{
	if (_stream.agent().options().disableSTUN)
		return;

	Log("debug", this) << "Gathering Server Reflexive Candidates from " << _stream.agent().options().stunAddress.toString() << endl;
	
	// Send a binding request from the host candidate to our STUN server 
	// to retreive our server reflexive candidate.
	// The GatheredServerReflexiveCandidates state will be set via the
	// resulting callback event.
	try {	
		LocalCandidateList bases = _stream.getLocalCandidatesForComponent(_id);
		for (LocalCandidateList::const_iterator it = bases.begin(); it != bases.end(); ++it) {

			ServerReflexiveCandidate* c = new ServerReflexiveCandidate(*this, *it);
			c->StateChange += delegate(this, &Component::onCandidateStateChange);

			// NOTE: Resolve our server reflexive address here
			// just in case the candidate pointer gets deleted.
			//c->resolveServerReflexiveAddr();
			c->resolve();
		}
	}
	catch (Poco::Exception& exc) {
		Log("error", this) << "ERROR: " << exc.displayText() << endl;
	}
}


// ---------------------------------------------------------------------
//
// Relayed Candidates
//
// ---------------------------------------------------------------------
void Component::gatherRelayedCandidates() 
{
	if (_stream.agent().options().disableRelay)
		return;
	
	Log("debug", this) << "Gathering Relayed Candidates from " 
		<< _stream.agent().options().turnAddress.toString() << endl;

	try	{
		TURN::Client::Options o;
		o.serverAddr = _stream.agent().options().turnAddress;
		o.timeout = _stream.agent().options().transactionTimeout;
		o.lifetime = 30 * 60 * 1000; // try for 30 minutes
		o.username = _stream.agent().options().localUFrag;
		o.password = _stream.agent().options().localPassword;

		RelayedCandidate* c = new RelayedCandidate(*this, o);
		c->StateChange += delegate(this, &Component::onCandidateStateChange);	
		c->resolve();
	}
	catch (Poco::Exception& exc) {
		Log("error", this) << "TURN ERROR: " << exc.displayText() << endl;
	}
}


void Component::printLog(std::ostream& ost) const 
{
	ost << "["
		<< className()
		<< ":"
		<< this
		<< ":"
		<< id()
		<< "] ";
}


} // namespace Sourcey
} // namespace ICE 
	


			//c->setTransport(Candidate::Udp);
			//c->setTransportAddr(_socket->address());
			//c->setBaseAddr(_socket->address());
			//c->setComponentID(_id);
			//c->computeFoundation();
			//c->computePriority();
			//onLocalCandidateGathered(c);			
			//if (_stream.tryAddLocalCandidate(c))

		//c->setTransport(Candidate::Udp);
		//c->setTransportAddr(_socket->address()); // will be set to XOR-RELAYED-ADDRESS
		//c->setBaseAddr(_socket->address());
		//c->setComponentID(_id);
		//c->computeFoundation();
		//c->computePriority();
		//_stream.tryAddLocalCandidate(c);

		/*
		_turn = new TURN::Client(_stream.agent().options().turnAddress);
		_turn->AllocationCreated += delegate(this, &Component::onTURNAllocationCreated);
		_turn->AllocationDeleted += delegate(this, &Component::onTURNAllocationDeleted);
		_turn->RequestTimeout += delegate(this, &Component::onTURNTransactionTimeout);		
		_turn->OnData += delegate(this, &Component::onTURNOnData);
		_turn->setUsername(_stream.agent().options().localUFrag);
		_turn->setPassword(_stream.agent().options().localPassword);
		_turn->setTimeout(_stream.agent().options().transactionTimeout);
		_turn->sendAllocate();
		*/

	/*
	<< localPort << endl;
	_socket = new Net::UDPSocket; //(Net::Address("127.0.0.1", localPort));
	_socket->attach(PolymorphicDelegate<Component, STUN::Message>(this, &Component::onSTUNPacketReceived, 101));
	_socket->attach(PolymorphicDelegate<Component, DataPacket>(this, &Component::onDataPacketReceived, 102));
	*/
	//Log("debug", this) << "Creating 1" << endl;
	//Log("debug", this) << "Creating 2" << endl;
	//Log("debug", this) << "Creating: OK" << endl;
	//if (_stream.provider())
	//	_stream.provider()->PacketReady += delegate(this, &Component::doPacketPacket);
	//_packetizer.PacketReady += delegate(this, &Component::doPacketPacket);
	//_packetizer.setMaxContentLength(1488); // We will have an RTP header length of 12

	/*
	Util::ClearList(_gatheringCandidates);
	_socket->detach(PolymorphicDelegate<Component, STUN::Message>(this, &Component::onSTUNPacketReceived, 101));
	_socket->detach(PolymorphicDelegate<Component, DataPacket>(this, &Component::onDataPacketReceived, 102));
	delete _socket;		
	*/
	//if (_stream.provider())
	//	_stream.provider()->PacketReady -= delegate(this, &Component::doPacketPacket);
	//_packetizer.PacketReady -= delegate(this, &Component::doPacketPacket);



/*
void Component::selectDefaultLocalCandidate() 
{
	//FastMutex::ScopedLock lock(_mutex);
	Log("debug", this) << "Component Candidates Gathered: " << component->id() << endl;
	
	//assert(!_components.empty());
	assert(!component->defaultLocalCandidate());
	assert(hasFlag(Component::LocalCandidatesGathered));
	//assert(!_localCandidates.empty());

	// Choosing Default Candidates
	//
	// A candidate is said to be default if it would be the target of media
	// from a non-ICE peer; that target is called the DEFAULT DESTINATION.
	// If the default candidates are not selected by the ICE algorithm when
	// communicating with an ICE-aware peer, an updated offer/answer will be
	// required after ICE processing completes in order to "fix up" the SDP
	// so that the default destination for media matches the candidates
	// selected by ICE. If ICE happens to select the default candidates, no
	// updated offer/answer is required.
	// 
	// An agent MUST choose a set of candidates, one for each component of
	// each in-use media stream, to be default. A media stream is in-use if
	// it does not have a port of zero (which is used in RFC 3264 to reject
	// a media stream). Consequently, a media stream is in-use even if it
	// is marked as a=inactive [RFC4566] or has a bandwidth value of zero.
	// 
	// It is RECOMMENDED that default candidates be chosen based on the
	// likelihood of those candidates to work with the peer that is being
	// contacted. It is RECOMMENDED that the default candidates are the
	// relayed candidates (if relayed candidates are available), server
	// reflexive candidates (if server reflexive candidates are available),
	// and finally host candidates.
	//
	LocalCandidateList localCandidates = getLocalCandidatesForComponent(_id);
	LocalCandidate* candidate = NULL;
	if (!_stream.agent().options().disableRelay) {
		for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
			if ((*it)->type() == Candidate::Relayed) {
				candidate = *it;
				break;
			}
		}	
	} 
	if (!candidate && !_stream.agent().options().disableSTUN) {
		for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
			if ((*it)->type() == Candidate::ServerReflexive) {
				candidate = *it;
				break;
			}
		}		
	} 
	if (!candidate) {
		for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
			if ((*it)->type() == Candidate::Host) {
				candidate = *it;
				break;
			}
		}
	}

	assert(candidate);
	if (candidate) {
		component->setDefaultLocalCandidate(candidate);
	} else {
		Log("error", this) << "Unable to assign default candidate for component " << component->id() << endl;
		setState(this, State::Failed);
	}

	// Set the default candidate for each media component before declaring
	// candidate gathering complete.
	//for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {	
	//	if (!it->second->hasFlag(Component::LocalCandidatesGathered)())
	//		return;
	//}
	//}
}
*/

		/*
			
		ServerReflexiveCandidate* c = new ServerReflexiveCandidate(this, NULL);
		
		STUN::Transaction* transaction = new STUN::Transaction;
		transaction->peerAddress() = _stream.agent().options().serverAddress;
		transaction->timeout = _stream.agent().options().transactionTimeout;
		transaction->request().setType(STUN::Message::Binding);
		transaction->request().setTransactionID(CryptoProvider::generateRandomKey(16));

		send(transaction);

		STUN::Request* check = new STUN::Request(_socket->address(), _stream.agent().options().serverAddress, _stream.agent().options().transactionTimeout);
		check->STUN::RequestTimeout += delegate(this, &Component::onSTUNTransactionTimeout);	

		check->request.setType(STUN::Message::Binding);
		check->request.setTransactionID(CryptoProvider::generateRandomKey(16));

		_checks.push_back(check);
		return _socket->send(request, check.request.remote);

		//STUN::Message request;
		//request.setType(STUN::Message::Binding);
		//request.setTransactionID(CryptoProvider::generateRandomKey(16));
		//_socket->send(request, _stream.agent().options().serverAddress);
		*/

// ---------------------------------------------------------------------
//
// Events
//
// ---------------------------------------------------------------------
/*
void Component::onDataPacketReceived(Net::ISocket& socket, Packet* packet, const Net::Address& localAddress, const Net::Address& peerAddress) {
	DataPacket* data = dynamic_cast<DataPacket*>(packet);
	assert(data);
	if (data) {
		onDataPacketReceived(socket, data, localAddress, peerAddress);
		return;
	}
	
	STUN::Message* message = dynamic_cast<STUN::Message*>(packet);
	assert(message);
	if (message) {
		onSTUNPacketReceived(socket, message, localAddress, peerAddress);
		return;
	}
}
*/



//void Component::onSTUNTransactionResponse(const void* sender, STUN::TransactionIPacket& packet) {
//	//Net::ISocket* connection = const_cast<Net::ISocket*>(reinterpret_cast<const Net::ISocket*>(sender));
//	Log("debug", this) << "Received: " << transaction->response().toString() << endl;
//
//	switch (transaction->response().type())
//	{
//	case STUN::Message::BindingResponse:
//		{					
//			Log("debug", this) << "Received BINDING-RESPONSE from " << response.peerAddress.toString() << endl;
//
//			/*
//			// Catch responses from the STUN/TURN server.
//			if (response.peerAddress == _stream.agent().options().turnAddress ||
//				response.peerAddress == _stream.agent().options().serverAddress) {
//
//
//				// The BINDING-RESPONSE is from the STUN/TURN server.
//				// Agents MUST support the backwards compatibility mode for
//				// the Binding request defined in [RFC5389].
//				bool hasAddress = false;
//				const STUN::AddressAttribute* addressAttr = reinterpret_cast<const STUN::AddressAttribute*>(transaction->response().get<STUN::XorMappedAddress>());
//				if (addressAttr || addressAttr->family() == 1) {
//					hasAddress = true;
//				} else {
//					addressAttr = reinterpret_cast<const STUN::AddressAttribute*>(transaction->response().getMappedAddress());
//					if (addressAttr && addressAttr->family() == 1)
//						hasAddress = true;
//				}
//
//				if (hasAddress) {
//					Net::Address srflxAddr(addressAttr->ipString(), addressAttr->port());
//
//					// Our stream also needs to know our server reflexive IP address
//					// for SDP generation.
//					_stream.agent().srflxIP(srflxAddr.host());
//
//					// Add the Server Reflexive Candidate to our Media Stream.
//					Candidate* c = new Candidate(Candidate::ServerReflexive);
//					c->setBaseAddr(response.socket.address());
//					c->setTransportAddr(srflxAddr);
//					//c->setComponentID(_id);
//					c->computePriority();
//					c->computeFoundation();
//					//onLocalCandidateGathered(c);
//					setFlag(ServerReflexiveCandidatesCreated);
//					assert(false);
//					Log("debug", this) << "Server Reflexive Candidate Gathered" << endl;
//				}
//
//
//			} else {
//			*/
//
//				// When a Binding response is received from a peer, it is correlated to its
//				// Binding request using the transaction ID, as defined in [RFC5389], which
//				// then ties it to the candidate pair for which the Binding request was sent.
//				// This section defines additional procedures for processing Binding
//				// responses specific to this usage of STUN.
//
//				// Success Cases
//				// A check is considered to be a success if all of the following are
//				// true:
//				//
//				// o  The STUN transaction generated a success message.
//				//
//				// o  The source IP address and port of the response equals the
//				//    destination IP address and port to which the Binding request was
//				//    sent.
//				//
//				// o  The destination IP address and port of the response request the
//				//    source IP address and port from which the Binding request was
//				//    sent.
//
//				/*
//				// Catch Connectivity Check responses.
//				STUN::Request* check = NULL;
//				for (STUN::RequestList::const_iterator it = _checks.begin(); it != _checks.end(); ++it) {
//					Log("debug", this) << "CHECKING:" 
//						<< "\n\tMatching: " << (*it)->local.toString() << ":" << (*it)->remote.toString()
//						<< "\n\tWith: " << packet.localAddress.toString() << ":" << packet.peerAddress.toString()
//						<< endl;
//					if ((*it)->matchWith(transaction->response, packet.local, packet.remote)) {
//						check = (*it); // Ensure deletion of message
//						_checks.erase(it);
//						break;
//					}
//				}
//
//				if (!check) {
//					Log("debug", this) << "Discarding mismatched BINDING-RESPONSE." << endl;
//					// Just discard the message; it has probably been cancelled.
//					return;
//				}		
//
//				// Delete the Connectivity Check.
//				delete check;
//				*/
//
//				// Authenticated! Send the response to the system for processing.
//				ReceivedConnectivityCheckSuccess.dispatch(this, response);	
//			}
//		//}
//		
//		break;
//	case STUN::Message::BindingErrorResponse:
//		
//		// Failure Cases
//		// If the STUN transaction generates a 487 (Role Conflict) error
//		// response, the agent checks whether it included the ICE-CONTROLLED or
//		// ICE-CONTROLLING attribute in the Binding request. If the request
//		// contained the ICE-CONTROLLED attribute, the agent MUST switch to the
//		// controlling role if it has not already done so. If the request
//		// contained the ICE-CONTROLLING attribute, the agent MUST switch to the
//		// controlled role if it has not already done so. Once it has switched, 
//		// the agent MUST enqueue the candidate pair whose check generated the
//		// 487 into the triggered check queue. The state of that pair is set to
//		// Waiting. When the triggered check is sent, it will contain an ICE-
//		// CONTROLLING or ICE-CONTROLLED attribute reflecting its new role.
//		// Note, however, that the tie-breaker value MUST NOT be reselected.
//
//		// A change in roles will require an agent to recompute pair priorities
//		// (Section 5.7.2), since those priorities are a function of controlling
//		// and controlled roles. The change in role will also impact whether
//		// the agent is responsible for selecting nominated pairs and generating
//		// updated offers upon conclusion of ICE.
//
//		// Agents MAY support receipt of ICMP errors for connectionectivity checks.
//		// If the STUN transaction generates an ICMP error, the agent sets the
//		// state of the pair to Failed. If the STUN transaction generates a
//		// STUN error response that is unrecoverable (as defined in [RFC5389])
//		// or times out, the agent sets the state of the pair to Failed.
//
//		// The agent MUST check that the source IP address and port of the
//		// response equal the destination IP address and port to which the
//		// Binding request was sent, and that the destination IP address and
//		// port of the response request the source IP address and port from which
//		// the Binding request was sent. In other words, the source and
//		// destination transport addresses in the request and responses are
//		// symmetric. If they are not symmetric, the agent sets the state of
//		// the pair to Failed.
//		assert(false);
//		break;
//	default:
//		assert(false);
//		break;
//	}
//}


//void Component::onSTUNOnData(Net::ISocket& socket, const char* data, int size, const Net::Address& localAddress, const Net::Address& peerAddress) {
//}


// ---------------------------------------------------------------------
//
// Connectivity
//
// ---------------------------------------------------------------------
/*
int Component::send(const char* data, int size) 
{	
	assert(_defaultLocalCandidate);
	assert(_defaultRemoteCandidate);
	Log("debug", this) << "Sending Packet to " 
		 << _defaultRemoteCandidate->transportAddr().toString() << endl;
	return send(data, size, _defaultRemoteCandidate->transportAddr());
}


int Component::send(const char* data, int size, const Net::Address& peerAddress) 
{	
	assert(_socket);
	
	int sent = 0;
	try	{
		//Log("debug") << "[Net::UDPSocket:" << this << "] SEND: " << address().toString() << "-->" << peerAddress.toString() << endl;
		//Mutex::ScopedLock lock(_mutex); 
		
		sent = _socket->send(data, size, peerAddress);
	}	
	//catch (Poco::Exception& exc) {
	//	Log("error") << "[Net::UDPSocket] " << exc.displayText() << endl;
	//} 

	// TODO: If our peer becomes unavailable we need to continue
	// ICE processing.
	catch (Poco::Exception& exc) {
		Log("error", this)  <<  exc.displayText() << endl;
	}

	return sent; //_socket && _socket->send(data, size, peerAddress);
}


int Component::send(const IPacket& packet) 
{	
	assert(_defaultLocalCandidate);
	assert(_defaultRemoteCandidate);
	Log("debug", this) << "Sending Packet to " 
		<< _defaultRemoteCandidate->transportAddr().toString() << endl;
	return send(packet, _defaultRemoteCandidate->transportAddr());
}


int Component::send(const IPacket& packet, const Net::Address& peerAddress) 
{		
	assert(_socket);
	return _socket && _socket->send(packet, peerAddress);
}


void Component::sendPacket(IPacket& packet) 
{	
	send((const IPacket&)packet);
}
*/




	/*
	// If the transport address differs from the old one we need to
	// re-bind the local candidate.
	if (_defaultLocalCandidate && 
		_defaultLocalCandidate->transportPort() != candidate->transportPort()) {
		if (_socket)
			delete _socket;
		_socket = new Net::UDPSocket(candidate->transportAddr());
		_socket->STUNReceived += delegate(this, &Component::onSTUNReceived);
		_socket->STUNTransactionResponse += delegate(this, &Component::onSTUNTransactionResponse);
		_socket->OnData += delegate(this, &Component::onMediaOnData);
		_socket->STUNRequestTimeout += delegate(this, &Component::onSTUNTransactionTimeout);
	}
	*/
/*
PacketStream* Component::provider() { 
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _provider; 
}


void Component::setProvider(PacketStream* provider) { 
	Poco::FastMutex::ScopedLock lock(_mutex);
	_provider = provider; 		
	_provider->PacketReady += delegate(this, &Component::doPacketPacket);
}


void Component::doPacketPacket(void* sender, Media::IPacket& packet) { 
	Poco::FastMutex::ScopedLock lock(_mutex);
	//assert(_packetizer);
	//if (_packetizer) {
	_packetizer.process((const char*)packet.data, packet.size);
	//}
}


void Component::doPacketPacket(IPacket& packet) 
{
	Log("debug", this) << "Packet Ready" << endl;
	assert(_defaultRemoteCandidate);
	assert(_socket);
	if (_defaultRemoteCandidate &&
		_socket) {
		_socket->send(packet, _defaultRemoteCandidate->transportAddr());
	}
}
*/



/*
void Component::onTURNAllocationCreated(STUN::MessageIPacket& packet) {
	Candidate* c = new Candidate(Candidate::Relayed);
	c->setTransport(Candidate::Udp);
	c->setTransportAddr(_turn->relayedAddress());
	c->setBaseAddr(_socket->address());
	//c->setComponentID(_id);
	c->computePriority();
	c->computeFoundation();
	//_relayCandidateFailed = false;
	//onLocalCandidateGathered(c);
	setFlag(RelayedCandidatesCreated);
	Log("debug", this) << "Relayed Candidate Gathered" << endl;
}


void Component::onTURNAllocationDeleted(STUN::MessageIPacket& packet) {
	Log("debug", this) << "Relayed Candidate Failed: Allocation Deleted" << endl;
	//_relayCandidateFailed = true;
	delete _turn;
	_turn = NULL;
	assert(false);
}


void Component::onTURNTransactionTimeout(STUN::TransactionIPacket& packet) {
	Log("debug", this) << "Relayed Candidate Failed: Timeout" << endl;

	// TODO: Candidate failed callback event.
	//_relayCandidateFailed = true;
	delete _turn;
	_turn = NULL;
}


void Component::onTURNOnData(STUN::DataPacket& packet) {
	assert(false);
}
*/

/*
// Catches STUN Binding Request failures and Connectivity Check failures 
// from peers.
void Component::onSTUNTransactionTimeout(STUN::TransactionIPacket& packet) {
	Log("debug", this) << "Connectivity Check Timeout" << endl;

	// Catch STUN Binding Request failures.
	if (packet.peerAddress.toString() == _stream.agent().options().serverAddress.toString()) {
		Log("debug", this) << "Server Reflexive Candidate Failed: Timeout" << endl;

		// TODO: Candidate failed callback event.
		//_srflxCandidateFailed = true;
	}

	// Catch STUN Binding Request failures.
	else {
		ReceivedConnectivityCheckFailed.dispatch(this, packet);
	}
}
*/



	/*
	Net::NetworkAddressList& interfaces = Net::NetworkHelper::instance().getInterfaceList();
	for (Net::NetworkAddressList::const_iterator it = interfaces.begin(); it != interfaces.end(); ++it) {

		if ((*it).toString() == "127.0.0.1" &&
			_stream.agent().options().disableLocalhost	)	
			continue;		

		// Initialize Host Candidate
		try {
			HostCandidate* c = new HostCandidate(*this);
			c->StateChange += delegate(this, &Component::onCandidateStateChange);	
			c->setTransport(Candidate::Udp);
			c->setTransportAddr(Net::Address(*it, _localPort));
			// NOTE: The candidate ptr may be deleted on failure, 
			// in which case exception will be thrown.
			c->resolve(); 

			// The component port will be updated here if unset.
			if (_localPort == 0)
				_localPort = c->transportPort();

			Log("debug", this) << "Host Candidate Gathered" << endl;
		} 
		catch (Poco::Exception& exc) {
			Log("error", this) << ": " << exc.displayText() << endl;
		}
	}
	*/

				/*
	// If candidate gathering is still incomplete check if
	// we can transition our stream state.
	if (state().id() == State::GatheringCandidates && 
		candidate->state() != LocalCandidate::Resolving &&
		candidate->component()->hasFlag(Component::LocalCandidatesCreated)) {		
		
			//Log("debug", this) << "ATTEMPTING TO UPDATE STATE: " << candidate->toString() << endl;
			trySetLocalCandidatesGathered(); //selectComponentDefaultLocalCandidate(this);

	}



			for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {	

				// If the flag is already set then continue...
				if (it->second->hasFlag(Component::LocalCandidatesGathered)) {
					candidateGatheringComplete = true;
					continue;
				}	

			}
		//Component* component = candidate->component();
		bool candidateGatheringComplete = false;
		for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {	

			// If the flag is already set then continue...
			if (it->second->hasFlag(Component::LocalCandidatesGathered)) {
				candidateGatheringComplete = true;
				continue;
			}			

			int numCandidates = getLocalCandidatesForComponent(it->second->id()).size();
			int numCompletedCandidates = 0;		
			for (LocalCandidateList::const_iterator lit = _localCandidates.begin(); lit != _localCandidates.end(); ++lit) {	
				if ((*lit)->state() == LocalCandidate::Success ||
					(*lit)->state() == LocalCandidate::Failed) {
					numCompletedCandidates++;
				}
			}

			// If all candidates have completed then we can update
			// default candidates.
			if (numCandidates == 
				numCompletedCandidates) {
				candidateGatheringComplete = true;
				selectComponentDefaultLocalCandidate(component);
			} else {
				candidateGatheringComplete = false;
				break;
			}
		}
		if (candidateGatheringComplete) {
			setFlag(LocalCandidatesGathered);
			//setLocalCandidatesGathered();
		
			Log("debug", this) << "Local Candidates Gathered" << endl;
			_agent->onMediaStreamLocalCandidatesGathered(this, _localCandidates);

			tryTransitionState(State::CheckingConnectivity);
		}
		*/


/*
bool Component::selectLocalCandidate() 
{
	//FastMutex::ScopedLock lock(_mutex);
	Log("debug", this) << "Component Candidates Gathered: " << component->id() << endl;
	
	//assert(!_components.empty());
	//assert(!component->defaultLocalCandidate());
	//assert(!localCandidates.empty());

	// Choosing Default Candidates
	//
	// A candidate is said to be default if it would be the target of media
	// from a non-ICE peer; that target is called the DEFAULT DESTINATION.
	// If the default candidates are not selected by the ICE algorithm when
	// communicating with an ICE-aware peer, an updated offer/answer will be
	// required after ICE processing completes in order to "fix up" the SDP
	// so that the default destination for media matches the candidates
	// selected by ICE. If ICE happens to select the default candidates, no
	// updated offer/answer is required.
	// 
	// An agent MUST choose a set of candidates, one for each component of
	// each in-use media stream, to be default. A media stream is in-use if
	// it does not have a port of zero (which is used in RFC 3264 to reject
	// a media stream). Consequently, a media stream is in-use even if it
	// is marked as a=inactive [RFC4566] or has a bandwidth value of zero.
	// 
	// It is RECOMMENDED that default candidates be chosen based on the
	// likelihood of those candidates to work with the peer that is being
	// contacted. It is RECOMMENDED that the default candidates are the
	// relayed candidates (if relayed candidates are available), server
	// reflexive candidates (if server reflexive candidates are available),
	// and finally host candidates.
	//
	LocalCandidateList localCandidates = localCandidates();
	LocalCandidate* candidate = NULL;
	if (!_agent->options().disableRelay) {
		// This implementation will not be using use relayed candidates as 
		// default candidates.
		for (LocalCandidateList::const_iterator it = localCandidates.begin(); it != localCandidates.end(); ++it) {	
			if ((*it)->type() == Candidate::Relayed) {
				candidate = *it;
				break;
			}
		}	
	} 
	if (!candidate && !_agent->options().disableSTUN) {
		for (LocalCandidateList::const_iterator it = localCandidates.begin(); it != localCandidates.end(); ++it) {	
			if ((*it)->type() == Candidate::ServerReflexive) {
				candidate = *it;
				break;
			}
		}		
	} 
	if (!candidate) {
		for (LocalCandidateList::const_iterator it = localCandidates.begin(); it != localCandidates.end(); ++it) {	
			if ((*it)->type() == Candidate::Host) {
				candidate = *it;
				break;
			}
		}
	}

	assert(candidate);
	if (candidate) {
		component->setDefaultLocalCandidate(candidate);
	} else {
		Log("error", this) << "Unable to assign default candidate for component " << component->id() << endl;
		setState(this, State::Failed);
	}

	// Set the default candidate for each media component before declaring
	// candidate gathering complete.
	//for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {	
	//	if (!it->second->hasFlag(LocalCandidatesGathered)())
	//		return;
	//}
	//}
}
	*/


/*
bool Component::isLocalCandidatesCreated() const { 
	return (hasFlag(HostCandidatesCreated) &&
		  ((hasFlag(ServerReflexiveCandidatesCreated)	|| _stream.agent().options().disableSTUN)) &&
		  ((hasFlag(RelayedCandidatesCreated)			|| _stream.agent().options().disableRelay))); 
};


void Component::setFlag(FlagT flag) { 
	Flaggable::setFlag(flag); 

	// Send a callback to the media stream when we have all our 
	// local candidates.
	if (isLocalCandidatesCreated() &&
		!hasFlag(LocalCandidatesGathered)) {
		setFlag(LocalCandidatesGathered);
		_stream.selectLocalCandidates(this);
	}
};
*/


/*
bool Component::onLocalCandidateGathered(LocalCandidate* candidate) {
	// TODO: Mutex protect

	// Send the candidate to our parent media stream.
	if (candidate)
		_stream.tryAddLocalCandidate(candidate);

	return true;
}
*/