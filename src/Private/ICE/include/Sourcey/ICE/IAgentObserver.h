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


#ifndef SOURCEY_ICE_IAgentObserver_H
#define SOURCEY_ICE_IAgentObserver_H


#include "Sourcey/ICE/CandidatePair.h"
#include "Sourcey/ICE/State.h"
#include "Sourcey/UserManager.h"
#include "Sourcey/Logger.h"
#include "Sourcey/SDP/Message.h"

//#include "Sourcey/ICE/MediaStream.h"
//#include "Sourcey/Stateful.h"
//#include "Sourcey/Flaggable.h"
//#include "Sourcey/Timer.h"


namespace Sourcey {
namespace ICE {

	
class Agent;
class MediaStream;
class Component;


class IAgentObserver 
{
public:
	virtual bool onMediaStreamCreated(Agent& agent, MediaStream& stream) = 0;
		// Called when a media stream has been created, but has not yet
		// been added to the ICE stack. Returning false here will prevent
		// creation, and the pointer will be deleted.

	virtual void onMediaStreamValidPairsChanged(Agent& agent, MediaStream& stream, const CandidatePairList& validPairs) {};
		// Called first when candidate pairs for each component of the
		// media stream enters the valid list. The answerer can begin
		// sending media on the media stream at this point. This 
		// callback is optional.

	virtual void onSelectedPairsChanged(Agent& agent, MediaStream& stream, Component& component, 
		const CandidatePair* selectedPair, const CandidatePair* oldPair = NULL) = 0;
		// Called as when the selected candidate pair for the given
		// Component is changed. The implementation should change the
		// media destination to the new selected pair straight away.

	virtual const IUser* authenticateBindingRequest(Agent& agent, const std::string& username) = 0;
		// Called when a STUN binding request is received from a peer.
		// The implementation can use a user manager for authenticating
		// against the provided username and returning user objects.
		// If a user object is NULL the request is considered not 
		// authenticated.

	virtual void onMediaOnData(Agent& agent, MediaStream& stream, Component& component, 
		const DataPacket& packet, const Net::Address& localAddress, const Net::Address& peerAddress) = 0;
		// Called when a media packet is received from a peer.

	virtual void onOutgoingSDP(Agent& agent, SDP::Message& sdp) = 0;
		// Called when outgoing SDP packet is ready to be sent to the
		// peer.

	virtual void onICEStateChange(Agent& agent, State& state) {};
		// Called when the ICE agent state has changed.
};


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_IAgentObserver_H