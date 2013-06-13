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


#ifndef SOURCEY_ICE_COMPONENT_H
#define SOURCEY_ICE_COMPONENT_H


#include "Sourcey/ICE/State.h"
#include "Sourcey/ICE/LocalCandidate.h"
#include "Sourcey/STUN/Message.h"
#include "Sourcey/RTP/Packet.h"
#include "Sourcey/RTP/RTCP/Packet.h"
#include "Sourcey/IPacket.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Sourcey/Flaggable.h"
#include "Sourcey/Net/UDPSocket.h"
#include "Sourcey/Net/Address.h"

#include <iostream>
#include <sstream>
#include <assert.h>


namespace Sourcey {
namespace ICE {


class MediaStream;
class LocalCandidate;
class RemoteCandidate;
class CandidatePair;


class Component: public Flaggable, public ILoggable
	// A component is a piece of a media stream requiring a
	// single transport address; a media stream may require multiple
	// components, each of which has to work for the media stream as a
	// whole to work. For media streams based on RTP, there are two
	// components per media stream -- one for RTP, and one for RTCP.
	//
	// The Component maintains Net::ISocket objects, and preforms local
	// Candidate gathering. It can be either Local or Remote in type
{
public:
	enum Flags 
	{
		LocalCandidatesCreated				= 0x01, 
		LocalCandidatesGathered				= 0x02, 
	};

	Component(MediaStream& stream, int id, UInt16 localPort = 0);
	virtual ~Component();

	// Functions for local media stream components.
	virtual void gatherHostCandidates();
	virtual void gatherServerReflexiveCandidates();
	virtual void gatherRelayedCandidates();
	
	virtual void terminateGatheringCandidates();

	virtual void setDefaultLocalCandidate(LocalCandidate* candidate);
	virtual void setDefaultRemoteCandidate(RemoteCandidate* candidate);

	virtual void setSelectedCandidatePair(CandidatePair* pair);	
	
	void sendPacket(void*, IPacket& packet);
	void sendRTPPacket(void*, RTP::Packet& packet);
	void sendRTCPPacket(void*, RTP::RTCP::Packet& packet);
		// Packet broadcast delegates can connect directly to
		// these methods to send media over the selected pair.
		// RTCP packets can only be sent over component 2.
	
	virtual int id() const { return _id; };
	virtual UInt16 localPort() const { return _localPort; };
	virtual MediaStream& stream() const { return _stream; };
	virtual LocalCandidate* defaultLocalCandidate() const { return _defaultLocalCandidate; };
	virtual RemoteCandidate* defaultRemoteCandidate() const { return _defaultRemoteCandidate; };

	virtual void onCandidateStateChange(void* sender, CandidateState& state, const CandidateState&);

	virtual void printLog(std::ostream& ost) const;
	
	virtual const char* className() const { return "ICE Component"; }

private:
	virtual void startGatheringLocalCandidates();

private:
	int _id;
	//int _numGatheringCandidates;
	MediaStream& _stream;
	//Net::UDPSocket* _socket;
	UInt16 _localPort;

	LocalCandidateList _gatheringCandidates;
		// A temporary list of gathering candidates.

	LocalCandidate* _defaultLocalCandidate;	
	RemoteCandidate* _defaultRemoteCandidate;	
	CandidatePair* _selectedCandidatePair;	

	Poco::FastMutex _mutex;
	
	friend class MediaStream;
	friend class CandidatePair;
};


typedef std::map<int, Component*> ComponentMap;
	// Components are indexed by their Component ID.


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_COMPONENT_H



	//virtual int numGatheringCandidates() const { return _numGatheringCandidates; };

	//virtual bool valid();
	
	
	//virtual bool onLocalCandidateStateChange(LocalCandidate* candidate);
	//virtual void selectDefaultLocalCandidate();

	//virtual bool onLocalCandidateGathered(LocalCandidate* candidate);
	//virtual bool onLocalCandidateReady(LocalCandidate* candidate);
	//virtual bool isLocalCandidatesCreated() const { return hasFlag(LocalCandidatesCreated); };
	//virtual bool isLocalCandidatesGathered() const { return hasFlag(LocalCandidatesGathered); };
	
	//virtual void selectLocalCandidate();
	/*
	virtual int send(const char* data, int size);
	virtual int send(const char* data, int size, const Net::Address& peerAddress);
	virtual int send(const IPacket& packet);	
	virtual int send(const IPacket& packet, const Net::Address& peerAddress);	

	virtual void sendPacket(IPacket& packet);
		// PacketStream callbacks can connect directly to this method.
		// Media will be sent over the selected pair.
		// TODO: Move to Media Stream.
	*/
