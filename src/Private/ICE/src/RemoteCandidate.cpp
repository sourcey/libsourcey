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


#include "Sourcey/ICE/RemoteCandidate.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace Sourcey {
namespace ICE { 


RemoteCandidate* RemoteCandidate::createFromSDP(SDP::Candidate* sdp) 
{			
	Log("debug") << "Creating Remote Candidate from SDP" << endl;
	assert(Candidate::stringToType(sdp->candidateType()) != Candidate::NoType);
	assert(Candidate::stringToTransport(sdp->transport()) != Candidate::NoTransport);
	assert(sdp->foundation().size());
	assert(sdp->componentID());
	assert(sdp->priority());
	assert(sdp->address().size());
	assert(sdp->port());

	// candidate-attribute   = "candidate" ":" foundation SP component-id SP
	//                         transport SP
	//                         priority SP
	//                         connection-address SP		;from RFC 4566
	//                         port						;port from RFC 4566
	//                         SP cand-type
	//                         [SP rel-address]
	//                         [SP rel-port]
	//                         *(SP extension-att-name SP
	//                              extension-att-value)
	//
	// Examples:
	// a=candidate:Hc0a80265 1 UDP 1694498815 192.168.2.101 16052 typ host
	// a=candidate:Sc0a80266 1 UDP 2130706431 1.1.1.1 16902 typ srflx raddr 192.168.2.101 rport 16052
	if (Candidate::stringToType(sdp->candidateType()) != Candidate::NoType  &&
		Candidate::stringToTransport(sdp->transport()) != Candidate::NoTransport  &&
		sdp->foundation().size() &&
		sdp->componentID() &&
		sdp->priority() &&
		sdp->address().size() &&
		sdp->port()) {

		RemoteCandidate* c = new RemoteCandidate();		
		c->setType(Candidate::stringToType(sdp->candidateType()));
		c->setFoundation(sdp->foundation());
		c->setComponentID(sdp->componentID());
		c->setTransport(stringToTransport(sdp->transport()));
		c->setPriority(sdp->priority());
		c->setTransportAddr(Net::Address(sdp->address(), sdp->port()));
		if (c->valid())
			return c;
		delete c;
	}
	return NULL;
}


} // namespace Sourcey
} // namespace ICE 
