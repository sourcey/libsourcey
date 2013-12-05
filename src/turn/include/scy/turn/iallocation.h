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


#ifndef SCY_TURN_IAllocation_H
#define SCY_TURN_IAllocation_H


#include "scy/turn/permission.h"
#include "scy/turn/fivetuple.h"
#include "scy/turn/types.h"
#include "scy/timer.h"
#include "scy/logger.h"
#include "scy/net/address.h"
#include "scy/mutex.h"


namespace scy {
namespace turn {


class IAllocation//: public basic::Polymorphic
	//  All TURN operations revolve around allocations, and all TURN messages
	//  are associated with an allocation.  An allocation conceptually
	//  consists of the following state data:
	// 
	//  o  the relayed transport address;
	// 
	//  o  the 5-tuple: (client's IP address, client's port, server IP
	//     address, server port, transport protocol);
	// 
	//  o  the authentication information;
	// 
	//  o  the time-to-expiry;
	// 
	//  o  a list of permissions;
	// 
	//  o  a list of channel to peer bindings.
	// 
	//  The relayed transport address is the transport address allocated by
	//  the server for communicating with peers, while the 5-tuple describes
	//  the communication path between the client and the server.  On the
	//  client, the 5-tuple uses the client's host transport address; on the
	//  server, the 5-tuple uses the client's server-reflexive transport
	//  address.

	//  Both the relayed transport address and the 5-tuple MUST be unique
	//  across all allocations, so either one can be used to uniquely
	//  identify the allocation.

	//  The authentication information (e.g., username, password, realm, and
	//  nonce) is used to both verify subsequent requests and to compute the
	//  message integrity of responses.  The username, realm, and nonce
	//  values are initially those used in the authenticated Allocate request
	//  that creates the allocation, though the server can change the nonce
	//  value during the lifetime of the allocation using a 438 (Stale Nonce)
	//  reply.  Note that, rather than storing the password explicitly, for
	//  security reasons, it may be desirable for the server to store the key
	//  value, which is an MD5 hash over the username, realm, and password
	//  (see [RFC5389]).
	// 
	//  The time-to-expiry is the time in seconds left until the allocation
	//  expires.  Each Allocate or Refresh transaction sets this timer, which
	//  then ticks down towards 0.  By default, each Allocate or Refresh
	//  transaction resets this timer to the default lifetime value of 600
	//  seconds (10 minutes), but the client can request a different value in
	//  the Allocate and Refresh request. Allocations can only be refreshed
	//  using the Refresh request; sending data to a peer does not refresh an
	//  allocation. When an allocation expires, the state data associated
	//  with the allocation can be freed.
	// 
{
public:
	IAllocation(const FiveTuple& tuple = FiveTuple(), 
				const std::string& username = "", 
				Int64 lifetime = 10 * 60 * 1000);
	virtual ~IAllocation();

	virtual void updateUsage(Int64 numBytes = 0);
		// Updates the allocation's internal timeout and bandwidth 
		// usage each time the allocation is used.

	virtual void setLifetime(Int64 lifetime);
		// Sets the lifetime of the allocation and resets the timeout.

	virtual void setBandwidthLimit(Int64 numBytes);
		// Sets the bandwidth limit in bytes for this allocation.

	virtual bool expired() const;
		// Returns true if the allocation is expired ie. is timed
		// out or the bandwidth limit has been reached.

	virtual bool deleted() const;
		// Returns true if the allocation's deleted flag is set
		// and or if the allocation has expired.
		///
		// This signifies that the allocation is ready to be    
		// destroyed via async garbage collection.
		// See Server::onTimer() and Client::onTimer()
	
	virtual Int64 bandwidthLimit() const;
	virtual Int64 bandwidthUsed() const;
	virtual Int64 bandwidthRemaining() const;
	virtual Int64 timeRemaining() const;

	virtual FiveTuple& tuple();
	virtual std::string username() const;
	virtual Int64 lifetime() const;
	virtual PermissionList permissions() const;
	
	virtual net::Address relayedAddress() const = 0;
	
	virtual void addPermission(const std::string& ip);
	virtual void addPermissions(const IPList& ips);
	virtual void removePermission(const std::string& ip);
	virtual void removeAllPermissions();
	virtual void removeExpiredPermissions();	
	//virtual void refreshAllPermissions();
	virtual bool hasPermission(const std::string& peerIP);
	
	virtual void print(std::ostream& os) const 
	{ 
		os << "Allocation[" << relayedAddress() << "]" << std::endl; 
	}
		
    friend std::ostream& operator << (std::ostream& stream, const IAllocation& alloc) 
	{
		alloc.print(stream);
		return stream;
    }

	//virtual const char* className() const { return "IAllocation"; };

protected:
	FiveTuple _tuple;
	std::string	_username;
	PermissionList	_permissions;
	Int64 _lifetime;
	Int64 _bandwidthLimit;
	Int64 _bandwidthUsed;
	time_t _createdAt;
	time_t _updatedAt;
	bool _deleted;
	//bool _deleted;
	//mutable Mutex _mutex;
};


//typedef std::map<FiveTuple,IAllocation*> AllocationMap;


} } // namespace scy::turn


#endif // SCY_TURN_IAllocation_H
