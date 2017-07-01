///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#include "scy/turn/server/server.h"
#include "scy/logger.h"
#include "scy/util.h"

#include <algorithm>


using namespace std;


namespace scy {
namespace turn {


ServerAllocation::ServerAllocation(Server& server, const FiveTuple& tuple,
                                   const std::string& username,
                                   std::int64_t lifetime)
    : IAllocation(tuple, username, lifetime)
    , _maxLifetime(server.options().allocationMaxLifetime / 1000)
    , _server(server)
{
    _server.addAllocation(this);
}


ServerAllocation::~ServerAllocation()
{
    _server.removeAllocation(this);
}


bool ServerAllocation::handleRequest(Request& request)
{
    LTrace("Handle Request")

    if (IAllocation::deleted()) {
        LWarn("Dropping request for deleted allocation")
        return false;
    }

    if (request.methodType() == stun::Message::CreatePermission)
        handleCreatePermission(request);
    else if (request.methodType() == stun::Message::Refresh)
        handleRefreshRequest(request);
    else
        return false; // respondError(request, 600, "Operation Not Supported");

    return true;
}


void ServerAllocation::handleRefreshRequest(Request& request)
{
    LTrace("Handle Refresh Request")
    assert(request.methodType() == stun::Message::Refresh);
    assert(request.classType() == stun::Message::Request);

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
    auto lifetimeAttr = request.get<stun::Lifetime>();
    if (!lifetimeAttr) {
        return;
    }
    uint32_t desiredLifetime = std::min<uint32_t>(
        _server.options().allocationMaxLifetime / 1000, lifetimeAttr->value());
    // lifetime = min(lifetime, lifetimeAttr->value() * 1000);

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

    stun::Message response(stun::Message::SuccessResponse,
                           stun::Message::Refresh);
    response.setTransactionID(request.transactionID());

    auto resLifetimeAttr = new stun::Lifetime;
    resLifetimeAttr->setValue(desiredLifetime);
    response.add(resLifetimeAttr);

    _server.respond(request, response);
    // request.socket->send(response, request.remoteAddress);
}


void ServerAllocation::handleCreatePermission(Request& request)
{
    LTrace("Handle Create Permission")

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
        auto peerAttr = request.get<stun::XorPeerAddress>(i);
        if (!peerAttr || (peerAttr && peerAttr->family() != 1)) {
            if (i == 0) {
                _server.respondError(request, 400, "Bad Request");
                return;
            } else
                break;
        }
        addPermission(std::string(peerAttr->address().host()));
    }

    stun::Message response(stun::Message::SuccessResponse,
                           stun::Message::CreatePermission);
    response.setTransactionID(request.transactionID());

    _server.respond(request, response);
    // request.socket->send(response, request.remoteAddress);
}


bool ServerAllocation::onTimer()
{
    LTrace("ServerAllocation: On timer: ", IAllocation::deleted())
    if (IAllocation::deleted())
        return false; // bye bye

    removeExpiredPermissions();
    return true;
}


std::int64_t ServerAllocation::maxTimeRemaining() const
{
    std::int64_t elapsed = static_cast<std::int64_t>(time(0) - _createdAt);
    return elapsed > _maxLifetime ? 0 : _maxLifetime - elapsed;
}


std::int64_t ServerAllocation::timeRemaining() const
{
   
    return min<std::int64_t>(IAllocation::timeRemaining(), maxTimeRemaining());
}


Server& ServerAllocation::server()
{
   
    return _server;
}


void ServerAllocation::print(std::ostream& os) const
{
    os << "ServerAllocation["
       << "\r\tTuple=" << _tuple << "\r\tUsername=" << username()
       << "\n\tBandwidth Limit=" << bandwidthLimit()
       << "\n\tBandwidth Used=" << bandwidthUsed()
       << "\n\tBandwidth Remaining=" << bandwidthRemaining()
       << "\n\tBase Time Remaining=" << IAllocation::timeRemaining()
       << "\n\tTime Remaining=" << timeRemaining()
       << "\n\tMax Time Remaining=" << maxTimeRemaining()
       << "\n\tDeletable=" << IAllocation::deleted()
       << "\n\tExpired=" << expired() << "]" << endl;
}


} // namespace turn
} // namespace scy


/// @\}
