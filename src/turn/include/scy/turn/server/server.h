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


#ifndef SCY_TURN_Server_H
#define SCY_TURN_Server_H


#include "scy/net/tcpsocket.h"
#include "scy/net/udpsocket.h"
#include "scy/timer.h"
#include "scy/stun/message.h"
#include "scy/turn/server/serverallocation.h"
#include "scy/turn/server/udpallocation.h"
#include "scy/turn/server/tcpallocation.h"
#include "scy/turn/util.h"


#include <assert.h>
#include <string>
#include <iostream>
#include <algorithm>


namespace scy {
namespace turn {

    
struct ServerOptions 
    /// Configuration options for the TURN server.
{
    std::string software;
    std::string realm;

    UInt32 allocationDefaultLifetime;
    UInt32 allocationMaxLifetime;    
    int allocationMaxPermissions;
    int timerInterval;
    int earlyMediaBufferSize;
    
    net::Address listenAddr; // The TCP and UDP bind() address
    std::string externalIP;  // The external public facing IP address of the server

    bool enableTCP;
    bool enableUDP;

    ServerOptions() {
        software                            = "Sourcey STUN/TURN Server [rfc5766]";
        realm                                = "sourcey.com";
        listenAddr                            = net::Address("0.0.0.0", 3478);
        externalIP                            = "";
        allocationDefaultLifetime            = 2 * 60 * 1000;
        allocationMaxLifetime                = 15 * 60 * 1000;
        allocationMaxPermissions            = 10;
        timerInterval                        = 10 * 1000;
        earlyMediaBufferSize                = 8192;
        enableTCP                            = true;
        enableUDP                            = true;
    }
};
    

struct ServerObserver 
    /// The ServerObserver receives callbacks for and is responsible
    /// for managing allocation and bandwidth quotas, authentication 
    /// methods and authentication.
{
    virtual void onServerAllocationCreated(Server* server, IAllocation* alloc) = 0;
    virtual void onServerAllocationRemoved(Server* server, IAllocation* alloc) = 0;

    virtual AuthenticationState authenticateRequest(Server* server, Request& request) = 0;
        // The observer class can implement authentication 
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


typedef std::map<FiveTuple, ServerAllocation*> ServerAllocationMap;


class Server
    /// TURN server rfc5766 implementation
{
public:
    Server(ServerObserver& observer, const ServerOptions& options = ServerOptions());
    virtual ~Server();

    virtual void start();
    virtual void stop();
    
    void handleRequest(Request& request, AuthenticationState state);
    void handleAuthorizedRequest(Request& request);
    void handleBindingRequest(Request& request);
    void handleAllocateRequest(Request& request);
    void handleConnectionBindRequest(Request& request);
    
    void respond(Request& request, stun::Message& response);
    void respondError(Request& request, int errorCode, const char* errorDesc);
    
    ServerAllocationMap allocations() const;
    void addAllocation(ServerAllocation* alloc);
    void removeAllocation(ServerAllocation* alloc);
    ServerAllocation* getAllocation(const FiveTuple& tuple);
    TCPAllocation* getTCPAllocation(const UInt32& connectionID);
    net::TCPSocket::Ptr getTCPSocket(const net::Address& remoteAddr);
    void releaseTCPSocket(net::Socket* socket);
    
    ServerObserver& observer();
    ServerOptions& options();
    net::UDPSocket& udpSocket();
    net::TCPSocket& tcpSocket();
    Timer& timer();
    
    void onTCPAcceptConnection(void* sender, const net::TCPSocket::Ptr& sock);
    void onTCPSocketClosed(void* sender);
    void onSocketRecv(void* sender, const MutableBuffer& buffer, const net::Address& peerAddress);
    void onTimer(void*);
    
private:    
    ServerObserver& _observer;
    ServerOptions _options;
    net::UDPSocket _udpSocket;
    net::TCPSocket _tcpSocket;    
    net::TCPSocket::Vec _tcpSockets;
    ServerAllocationMap    _allocations;
    Timer _timer;
};


} } //  namespace scy::turn


#endif // SCY_TURN_Server_H
