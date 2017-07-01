///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#include "scy/turn/server/udpallocation.h"
#include "scy/buffer.h"
#include "scy/logger.h"
#include "scy/net/udpsocket.h"
#include "scy/turn/server/server.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>


using namespace std;


namespace scy {
namespace turn {


UDPAllocation::UDPAllocation(Server& server, const FiveTuple& tuple,
                             const std::string& username,
                             const uint32_t& lifetime)
    : ServerAllocation(server, tuple, username, lifetime)
    , _relaySocket(net::makeSocket<net::UDPSocket>())
{
    // Handle data from the relay socket directly from the allocation.
    // This will remove the need for allocation lookups when receiving
    // data from peers.
    _relaySocket->bind(net::Address(server.options().listenAddr.host(), 0));
    _relaySocket.Recv += slot(this, &UDPAllocation::onPeerDataReceived);

    LTrace(" Initializing on address: ", _relaySocket->address())
}


UDPAllocation::~UDPAllocation()
{
    LTrace("Destroy")
    _relaySocket.Recv -= slot(this, &UDPAllocation::onPeerDataReceived);
    _relaySocket->close();
}


bool UDPAllocation::handleRequest(Request& request)
{
    LTrace("Handle Request")

    if (!ServerAllocation::handleRequest(request)) {
        if (request.methodType() == stun::Message::SendIndication)
            handleSendIndication(request);
        else
            return false;
    }

    return true;
}


void UDPAllocation::handleSendIndication(Request& request)
{
    LTrace("Handle Send Indication")

    // The message is first checked for validity.  The Send indication MUST
    // contain both an XOR-PEER-ADDRESS attribute and a DATA attribute.  If
    // one of these attributes is missing or invalid, then the message is
    // discarded.  Note that the DATA attribute is allowed to contain zero
    // bytes of data.

    auto peerAttr = request.get<stun::XorPeerAddress>();
    if (!peerAttr || (peerAttr && peerAttr->family() != 1)) {
        LError("Send Indication error: No Peer Address")
        // silently discard...
        return;
    }

    auto dataAttr = request.get<stun::Data>();
    if (!dataAttr) {
        LError("Send Indication error: No Data attribute")
        // silently discard...
        return;
    }

    // The Send indication may also contain the DONT-FRAGMENT attribute.  If
    // the server is unable to set the DF bit on outgoing UDP datagrams when
    // this attribute is present, then the server acts as if the DONT-
    // FRAGMENT attribute is an unknown comprehension-required attribute
    // (and thus the Send indication is discarded).

    // The server also checks that there is a permission installed for the
    // IP address contained in the XOR-PEER-ADDRESS attribute.  If no such
    // permission exists, the message is discarded.  Note that a Send
    // indication never causes the server to refresh the permission.

    // The server MAY impose restrictions on the IP address and port values
    // allowed in the XOR-PEER-ADDRESS attribute -- if a value is not
    // allowed, the server silently discards the Send indication.

    net::Address peerAddress = peerAttr->address();
    if (!hasPermission(peerAddress.host())) {
        SError << "Send Indication error: No permission for: "
               << peerAddress.host() << endl;
        // silently discard...
        return;
    }

    // If everything is OK, then the server forms a UDP datagram as follows:

    // o  the source transport address is the relayed transport address of
    //    the allocation, where the allocation is determined by the 5-tuple
    //    on which the Send indication arrived;

    // o  the destination transport address is taken from the XOR-PEER-
    //    ADDRESS attribute;

    // o  the data following the UDP header is the contents of the value
    //    field of the DATA attribute.

    // The handling of the DONT-FRAGMENT attribute (if present), is
    // described in Section 12.

    // The resulting UDP datagram is then sent to the peer.

    STrace << "Relaying Send Indication: "
           << "\r\tFrom: " << request.remoteAddress.toString()
           << "\r\tTo: " << peerAddress << endl;

    if (send(dataAttr->bytes(), dataAttr->size(), peerAddress) == -1) {
        _server.respondError(request, 486, "Allocation Quota Reached");
        delete this;
    }
}


void UDPAllocation::onPeerDataReceived(net::Socket&,
                                       const MutableBuffer& buffer,
                                       const net::Address& peerAddress)
{
    // auto source = reinterpret_cast<net::PacketInfo*>(packet.info);
    LTrace("Received UDP Datagram from ", peerAddress)

    if (!hasPermission(peerAddress.host())) {
        LTrace("No Permission: ", peerAddress.host())
        return;
    }

    updateUsage(buffer.size());

    // Check that we have not exceeded out lifetime and bandwidth quota.
    if (IAllocation::deleted())
        return;

    stun::Message message(stun::Message::Indication,
                          stun::Message::DataIndication);

    // Try to use the externalIP value for the XorPeerAddress
    // attribute to overcome proxy and NAT issues.
    std::string peerHost(server().options().externalIP);
    if (peerHost.empty()) {
        peerHost.assign(peerAddress.host());
        assert(0 && "external IP not set");
    }

    auto peerAttr = new stun::XorPeerAddress;
    peerAttr->setAddress(net::Address(peerHost, peerAddress.port()));
    message.add(peerAttr);

    auto dataAttr = new stun::Data;
    dataAttr->copyBytes(bufferCast<const char*>(buffer), buffer.size());
    message.add(dataAttr);

    STrace << "Send data indication:"
           << "\n\tFrom: " << peerAddress << "\n\tTo: " << _tuple.remote()
           //<< "\n\tData: " << std::string(packet.data(), packet.size())
           << endl;

    server().udpSocket().sendPacket(message, _tuple.remote());
}


ssize_t UDPAllocation::send(const char* data, size_t size,
                            const net::Address& peerAddress)
{
    updateUsage(size);

    // Check that we have not exceeded our lifetime and bandwidth quota.
    if (IAllocation::deleted()) {
        LWarn("Send indication dropped: Allocation quota reached")
        return -1;
    }

    return _relaySocket.send(data, size, peerAddress);
}


net::Address UDPAllocation::relayedAddress() const
{
   
    return _relaySocket->address();
}


} } //  namespace scy::turn


/// @\}
