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


#include "Sourcey/ICE/HostCandidate.h"
#include "Sourcey/ICE/Agent.h"
#include "Sourcey/ICE/MediaStream.h"
#include "Sourcey/ICE/Component.h"
#include "Sourcey/ICE/IAgentObserver.h"


using namespace std;


namespace Sourcey {
namespace ICE { 


HostCandidate::HostCandidate(Component& component) :
	LocalCandidate(component, Candidate::Host, this), 
	_socket(NULL) 
{
}
	

HostCandidate::~HostCandidate()
{
	//Log("debug", this) << "Destroying" << endl;

	if (_socket) {
		_socket->detach(packetDelegate(this, &HostCandidate::onSTUNPacketReceived, 101));
		_socket->detach(packetDelegate(this, &HostCandidate::onDataPacketReceived, 102));
		delete _socket;
	}
}


void HostCandidate::resolve()
{
	Log("debug", this) << "Resolving" << endl;

	setState(this, CandidateState::Resolving);	

	_socket = new Net::UDPSocket(_component.stream().agent().reactor());
	try {				
		reinterpret_cast<Net::UDPSocket*>(_socket)->bind(transportAddr(), true); // The transportAddr() must be set before calling resolve()
		_socket->registerPacketType<STUN::Message>(1);
		_socket->attach(packetDelegate(this, &HostCandidate::onSTUNPacketReceived, 101));
		_socket->attach(packetDelegate(this, &HostCandidate::onDataPacketReceived, 102));
		setTransportAddr(_socket->address()); // Set to our bound addr (if port was 0)
		setState(this, CandidateState::Success);
	} 
	catch (Poco::Exception& exc) {
		Log("error", this) << ": " << exc.displayText() << endl;
		setState(this, CandidateState::Failed);
		exc.rethrow();
	}
}


void HostCandidate::makeRedundant()
{
	setState(this, CandidateState::Redundant);
}


void HostCandidate::terminate()
{
	Log("debug", this) << "Terminating" << endl;

	if (_socket) {
		_socket->detach(packetDelegate(this, &HostCandidate::onSTUNPacketReceived, 101));
		_socket->detach(packetDelegate(this, &HostCandidate::onDataPacketReceived, 102));
		delete _socket;
		_socket = NULL;
	}
	setState(this, CandidateState::Terminating);
}


int HostCandidate::send(const char* data, int size, const Net::Address& peerAddress) 
{
	return _socket->send(data, size, peerAddress);
}


int HostCandidate::send(const IPacket& packet, const Net::Address& peerAddress) 
{
	return _socket->send(packet, peerAddress);
}


// All STUN messages coming via this component are authenticated and 
// sent back to the media stream. STUN request will be handled here,
// while STUN responses will be handeled by the initiating candidate 
// pair.
void HostCandidate::onSTUNPacketReceived(void* sender, STUN::Message& message
	//Net::ISocket& socket, const STUN::Message& message, 
	//const Net::Address& localAddress, const Net::Address& peerAddress
	) 
{
	Log("debug", this) << "STUN Received: " << message.toString() << endl;			
	
	Net::PacketInfo* source = reinterpret_cast<Net::PacketInfo*>(message.info);	
	if (!source)
		return;

	// The STUN packet will be picked up either by the Candidate Pair
	// to which this candidate belongs (first priority) for processing
	// connectivity check responses, or the Check List, where it will
	// be treated as a connectivity check request from the remore peer.
	STUNMessageReceived.dispatch(this, message, source->peerAddress);
}


void HostCandidate::onDataPacketReceived(void* sender, DataPacket& packet) 
{
	Net::PacketInfo* source = reinterpret_cast<Net::PacketInfo*>(packet.info);	
	if (!source)
		return;

	//Log("debug", this) << "Data packet Received from " << source->peerAddress.toString() << endl;	
	_component.stream().agent().observer().onMediaOnData(
		_component.stream().agent(), _component.stream(), _component, 
		packet, source->localAddress, source->peerAddress);
}


	/*
	//Net::ISocket& socket, const DataPacket& packet, const Net::Address& localAddress, const Net::Address& peerAddress
	
	
	switch (message.type())
	{
	// TODO: Detecting and Repairing Role Conflicts
	case STUN::Message::Binding:
		{			
			// Authenticated! Send the request to the system for processing.
			_component.stream().queue().onConnectivityCheckRequest(this, message, localAddress, peerAddress);		
		}
		break;
	default:
		assert(false);
		break;
	}
	*/

} // namespace Sourcey
} // namespace ICE 