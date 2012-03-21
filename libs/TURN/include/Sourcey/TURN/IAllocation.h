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


#ifndef SOURCEY_TURN_IAllocation_H
#define SOURCEY_TURN_IAllocation_H


#include "Sourcey/TURN/Permission.h"
#include "Sourcey/TURN/FiveTuple.h"
#include "Sourcey/TURN/Types.h"
#include "Sourcey/Timeout.h"
#include "Sourcey/Timer.h"
#include "Sourcey/Net/Address.h"
#include "Poco/Mutex.h"


namespace Sourcey {
namespace TURN {


class IAllocation 
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
	//  the Allocate and Refresh request.  IAllocations can only be refreshed
	//  using the Refresh request; sending data to a peer does not refresh an
	//  allocation. When an allocation expires, the state data associated
	//  with the allocation can be freed.
	// 
{
public:
	IAllocation(const FiveTuple& tuple = FiveTuple(), 
				const std::string& username = "", 
				UInt32 lifetime = 10 * 60 * 1000);
	virtual ~IAllocation();

	virtual void updateUsage(UInt32 numBytes = 0);
		// Updates the allocation's internal timeout and bandwidth 
		// usage each time the allocation is used.

	virtual bool isOK();
		// Returns false if the allocation is timed out or the bandwidth
		// limit has been reached.

	virtual bool expired();
		// Returns true if the allocation is timed out.

	virtual void setLifetime(UInt32 lifetime);
		// Sets the lifetime of the allocation and resets the timeout.

	virtual void setBandwidthLimit(UInt32 numBytes);
		// Sets the bandwidth limit in bytes for this allocation.
	
	virtual UInt32 bandwidthLimit(); 
	virtual UInt32 bandwidthUsed();
	virtual UInt32 bandwidthRemaining();
	virtual UInt32 timeRemaining(); 

	virtual FiveTuple& tuple();
	virtual std::string username() const;
	virtual UInt32 lifetime() const;
	virtual PermissionList permissions() const;
	
	virtual Net::Address relayedAddress() const = 0;
	
	virtual void addPermission(const Net::IP& ip);
	virtual void addPermissions(const IPList& ips);
	virtual void removePermission(const Net::IP& ip);
	virtual void removeAllPermissions();
	virtual void removeExpiredPermissions();	
	//virtual void refreshAllPermissions();
	virtual bool hasPermission(const Net::IP& peerIP);

protected:
	FiveTuple		_tuple;
	std::string		_username;
	UInt32			_lifetime;
	UInt32			_createdAt;
	UInt32			_updatedAt;
	PermissionList	_permissions;
	//Timeout			_timeout;
	UInt32	_bandwidthLimit;
	UInt32	_bandwidthUsed;
	mutable Poco::FastMutex _mutex;
};


typedef std::map<FiveTuple,IAllocation*> AllocationMap;


} } // namespace Sourcey::TURN


#endif // SOURCEY_TURN_IAllocation_H
