//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_TURN_SERVER_IServerObserver_H
#define SOURCEY_TURN_SERVER_IServerObserver_H


#include "Sourcey/TURN/Types.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <algorithm>


namespace Sourcey {
namespace TURN {
	
	
class Server;
class IAllocation;


struct IServerObserver 
	/// The TURN server implementation is ignorant of allocation quotas,
	/// bandwidth and authentication methods. If is the job of the observer
	/// to implement these features.
{
	virtual void onServerAllocationCreated(Server* server, IAllocation* alloc) = 0;
	virtual void onServerAllocationRemoving(Server* server, IAllocation* alloc) = 0;

	virtual AuthenticationState authenticateRequest(Server* server, const Request& request) = 0;
		// The observer class should implement authentication 
		// using the long-term credential mechanism of [RFC5389].
		// The class design is such that authentication can be preformed
		// asynchronously against a remote database, or locally.
		// The default implementation returns true to all requests.
		//
		// To mitigate either intentional or unintentional denial-of-service
	    // attacks against the server by clients with valid usernames and
	    // passwords, it is RECOMMENDED that the server impose limits on both
	    // the number of allocations active at one time for a given username and
	    // on the amount of bandwidth those allocations can use.  The server
	    // should reject new allocations that would exceed the limit on the
	    // allowed number of allocations active at one time with a 486
	    // (Allocation Quota Exceeded) (see Section 6.2), and should discard
	    // application data traffic that exceeds the bandwidth quota.
};


} } //  namespace Sourcey::TURN


#endif // SOURCEY_TURN_SERVER_IServerObserver_H
