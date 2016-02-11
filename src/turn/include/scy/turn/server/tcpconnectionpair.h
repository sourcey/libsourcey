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


#ifndef SCY_TURN_TCPClientConnection_H
#define SCY_TURN_TCPClientConnection_H


#include "scy/collection.h"
#include "scy/timer.h"
#include "scy/stun/message.h"
#include "scy/net/tcpsocket.h"


namespace scy {
namespace turn {

    
class TCPAllocation;

class TCPConnectionPair
{
public:    
    TCPConnectionPair(TCPAllocation& allocation);
    virtual ~TCPConnectionPair();

    bool doPeerConnect(const net::Address& peerAddr);
        // Initiates an outgoing TCP connection to the
        // peer for Connect requests.

    bool makeDataConnection();
        // Binds the client <> peer relay pipe once the 
        // ConnectionBind request is successfull.
    
    void setPeerSocket(const net::TCPSocket::Ptr& socket);
    void setClientSocket(const net::TCPSocket::Ptr& socket);

    void onPeerConnectSuccess(void* sender); 
        // Connection success callback for Connect request.

    void onPeerConnectError(void* sender, const Error& error);
        // Connection error callback for Connect request.
    
    void onClientDataReceived(void* sender, const MutableBuffer& buffer, const net::Address& peerAddress);
    void onPeerDataReceived(void* sender, const MutableBuffer& buffer, const net::Address& peerAddress);

    void onConnectionClosed(void* sender);
        // Callback for handing either client or peer connections
        // which result in the destruction of the TCPConnectionPair.

    void startTimeout();
        // Starts the ConnectionBind request timeout.
        //
        // If no ConnectionBind request associated with this peer data
        // connection is received after 30 seconds, the peer data
        // connection MUST be closed.

    TCPAllocation& allocation;

    net::TCPSocket::Ptr client;
        // The client socket, nullptr to start.

    net::TCPSocket::Ptr peer;
        // The client socket, nullptr to start.

    bool expired() const;
        // Return true if the peer ConnectionBind request timed out.

    Buffer earlyPeerData;    
        // Stores early peer > client data.
        
    std::uint32_t connectionID;
        // The unique connection ID.

    bool isDataConnection;
        // True when p2p relay is flowing.
    
    Timeout    timeout;
        // The ConnectionBind request timeout counter.

    stun::TransactionID    transactionID;

private:    
    TCPConnectionPair(const TCPConnectionPair&); // = delete;
    TCPConnectionPair(TCPConnectionPair&&); // = delete;
    TCPConnectionPair& operator=(const TCPConnectionPair&); // = delete;
    TCPConnectionPair& operator=(TCPConnectionPair&&); // = delete;
};


} } // namespace scy::turn


#endif // SCY_TURN_TCPClientConnection_H
