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


#include "Sourcey/TURN/server/Server.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"

#include <algorithm>


using namespace std;
using namespace Poco;


namespace Scy {
namespace TURN {


ServerAllocation::ServerAllocation(Server& server, const FiveTuple& tuple, const std::string& username, UInt32 lifetime) : 
	IAllocation(tuple, username, lifetime),
	_maxLifetime(server.options().allocationMaxLifetime / 1000),
	_server(server)
{
	_server.addAllocation(this);
	
	log("trace") << "Created: " 
		<< "\r\tUsername: " << username
		<< "\r\tLifetime: " << _lifetime
		<< "\r\tMaximum Lifetime: " << _maxLifetime
		<< endl;
}


ServerAllocation::~ServerAllocation() 
{
	log("trace") << "Destroying:" 
		<< "\r\tUsername: " << username()
		<< "\n\tBandwidth Limit: " << bandwidthLimit()
		<< "\n\tBandwidth Used: " << bandwidthUsed()
		<< "\n\tBandwidth Remaining: " << bandwidthRemaining()
		<< "\n\tTime Remaining: " << timeRemaining()
		<< endl;

	_server.removeAllocation(this);	
}


bool ServerAllocation::handleRequest(const Request& request) 
{	
	log("trace") << "Handle Request" << endl;	
	
	// FIXME: Adding this check as there is a possibility receiving 
	// requests after the allocation is received under heavy traffic.
	assert(isOK());

	if (request.type() == STUN::Message::CreatePermission)
		handleCreatePermission(request);
	else if (request.type() == STUN::Message::Refresh)	
		handleRefreshRequest(request);
	else
		return false; //sendError(request, 600, "Operation Not Supported");
	
	return true; 
}


void ServerAllocation::handleRefreshRequest(const Request& request) 
{
	log("trace") << "Handle Refresh Request" << endl;
	assert(request.type() == STUN::Message::Refresh);
	assert(request.state() == STUN::Message::Request);

	// 7.2. Receiving a Refresh Request

	// When the server receives a Refresh request, it processes as per
	// Section 4 plus the specific rules mentioned here.

	// The server computes a value called the "desired lifetime" as follows:
	// if the request contains a LIFETIME attribute and the attribute value
	// is 0, then the "desired lifetime" is 0.  Otherwise, if the request
	// contains a LIFETIME attribute, then the server computes the minimum
	// of the client's requested lifetime and the server's maximum allowed
	// lifetime.  If this computed value is greater than the default
	// lifetime, then the "desired lifetime" is the computed value.
	// Otherwise, the "desired lifetime" is the default lifetime.	

	// Compute the appropriate LIFETIME for this allocation.
	const STUN::Lifetime* lifetimeAttr = request.get<STUN::Lifetime>();
	if (!lifetimeAttr) {
		return;
	}	
	UInt32 desiredLifetime = min(_server.options().allocationMaxLifetime / 1000, lifetimeAttr->value());
	//lifetime = min(lifetime, lifetimeAttr->value() * 1000);

	// Subsequent processing depends on the "desired lifetime" value:

	// o  If the "desired lifetime" is 0, then the request succeeds and the
	//    allocation is deleted.

	// o  If the "desired lifetime" is non-zero, then the request succeeds
	//    and the allocation's time-to-expiry is set to the "desired
	//    lifetime".

	if (desiredLifetime > 0)
		setLifetime(desiredLifetime);
	else {
		delete this;
	}

	// If the request succeeds, then the server sends a success response
	// containing:

	// o  A LIFETIME attribute containing the current value of the time-to-
	//    expiry timer.

	//    NOTE: A server need not do anything special to implement
	//    idempotency of Refresh requests over UDP using the "stateless
	//    stack approach".  Retransmitted Refresh requests with a non-zero
	//    "desired lifetime" will simply refresh the allocation.  A
	//    retransmitted Refresh request with a zero "desired lifetime" will
	//    cause a 437 (Allocation Mismatch) response if the allocation has
	//    already been deleted, but the client will treat this as equivalent
	//    to a success response (see below).
	
	STUN::Message response;
	response.setType(STUN::Message::Refresh);
	response.setTransactionID(request.transactionID());

	STUN::Lifetime* resLifetimeAttr = new STUN::Lifetime;
	resLifetimeAttr->setValue(desiredLifetime);
	response.add(resLifetimeAttr);

	request.socket.send(response, request.remoteAddr);
}


void ServerAllocation::handleCreatePermission(const Request& request) 
{	
	log("trace") << "Handle Create Permission" << endl;

	// 9.2. Receiving a CreatePermission Request
	// 
	// When the server receives the CreatePermission request, it processes
	// as per Section 4 plus the specific rules mentioned here.
	// 
	// The message is checked for validity.  The CreatePermission request
	// MUST contain at least one XOR-PEER-ADDRESS attribute and MAY contain
	// multiple such attributes.  If no such attribute exists, or if any of
	// these attributes are invalid, then a 400 (Bad Request) error is
	// returned.  If the request is valid, but the server is unable to
	// satisfy the request due to some capacity limit or similar, then a 508
	// (Insufficient Capacity) error is returned.
	// 
	// The server MAY impose restrictions on the IP address allowed in the
	// XOR-PEER-ADDRESS attribute -- if a value is not allowed, the server
	// rejects the request with a 403 (Forbidden) error.
	// 
	// If the message is valid and the server is capable of carrying out the
	// request, then the server installs or refreshes a permission for the
	// IP address contained in each XOR-PEER-ADDRESS attribute as described
	// in Section 8.  The port portion of each attribute is ignored and may
	// be any arbitrary value.
	// 
	// The server then responds with a CreatePermission success response.
	// There are no mandatory attributes in the success response.
	// 
	//   NOTE: A server need not do anything special to implement
	//   idempotency of CreatePermission requests over UDP using the
	//   "stateless stack approach".  Retransmitted CreatePermission
	//   requests will simply refresh the permissions.			
	//
    for (int i = 0; i < _server.options().allocationMaxPermissions; i++) {
		const STUN::XorPeerAddress* peerAttr = request.get<STUN::XorPeerAddress>(i);
		if (!peerAttr || peerAttr && peerAttr->family() != 1) {
			if (i == 0) {
				_server.sendError(request, 400, "Bad Request");
				return;
			}
			else
				break;	
		}
		addPermission(Net::IP(peerAttr->ipString()));
	}
	
	STUN::Message response;
	response.setType(STUN::Message::CreatePermission);
	response.setTransactionID(request.transactionID());
  
	request.socket.send(response, request.remoteAddr);
}


UInt32 ServerAllocation::timeRemaining()
{
	FastMutex::ScopedLock lock(_mutex);
	time_t currentTime = time(0);	

	UInt32 remaining = static_cast<UInt32>(
		min(static_cast<UInt32>(_lifetime - (currentTime - _updatedAt)),
			static_cast<UInt32>(_maxLifetime - (currentTime - _createdAt)))
	);
	
	//log("trace") << "Seconds Remaining: " 
	//	<< "\r\tLifetime: " << (_lifetime - (currentTime - _updatedAt)) 
	//	<< "\r\tMaximum: " << (_maxLifetime - (currentTime - _updatedAt)) 
	//	<< endl;

	return remaining > 0 ? remaining : 0;
}


Server& ServerAllocation::server()
{
	FastMutex::ScopedLock lock(_mutex);
	return _server;
}


bool ServerAllocation::onTimer()
{
	if (!isOK())
		return false; // will cause deletion

	removeExpiredPermissions();
	return true;
}


} } // namespace Scy::TURN