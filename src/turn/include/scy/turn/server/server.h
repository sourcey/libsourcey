///
///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#ifndef SCY_TURN_Server_H
#define SCY_TURN_Server_H


#include "scy/net/tcpsocket.h"
#include "scy/net/udpsocket.h"
#include "scy/stun/message.h"
#include "scy/timer.h"
#include "scy/turn/server/serverallocation.h"
#include "scy/turn/server/tcpallocation.h"
#include "scy/turn/server/udpallocation.h"
#include "scy/turn/turn.h"


#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>


namespace scy {
namespace turn {


/// Configuration options for the TURN server.
struct ServerOptions
{
    std::string software;
    std::string realm;

    uint32_t allocationDefaultLifetime;
    uint32_t allocationMaxLifetime;
    int allocationMaxPermissions;
    int timerInterval;
    int earlyMediaBufferSize;

    net::Address listenAddr; ///< The TCP and UDP bind() address
    std::string externalIP; ///< The external public facing IP address of the server

    bool enableTCP;
    bool enableUDP;

    ServerOptions()
    {
        software = "Sourcey STUN/TURN Server [rfc5766]";
        realm = "sourcey.com";
        listenAddr = net::Address("0.0.0.0", 3478);
        externalIP = "";
        allocationDefaultLifetime = 2 * 60 * 1000;
        allocationMaxLifetime = 15 * 60 * 1000;
        allocationMaxPermissions = 10;
        timerInterval = 10 * 1000;
        earlyMediaBufferSize = 8192;
        enableTCP = true;
        enableUDP = true;
    }
};


/// The ServerObserver receives callbacks for and is responsible
/// for managing allocation and bandwidth quotas, authentication
/// methods and authentication.
struct ServerObserver
{
    virtual void onServerAllocationCreated(Server* server, IAllocation* alloc) = 0;
    virtual void onServerAllocationRemoved(Server* server, IAllocation* alloc) = 0;

    /// The observer class can implement authentication
    /// using the long-term credential mechanism of [RFC5389].
    /// The class design is such that authentication can be preformed
    /// asynchronously against a remote database, or locally.
    /// The default implementation returns true to all requests.
    ///
    /// To mitigate either intentional or unintentional denial-of-service
    /// attacks against the server by clients with valid usernames and
    /// passwords, it is RECOMMENDED that the server impose limits on both
    /// the number of allocations active at one time for a given username and
    /// on the amount of bandwidth those allocations can use.  The server
    /// should reject new allocations that would exceed the limit on the
    /// allowed number of allocations active at one time with a 486
    /// (Allocation Quota Exceeded) (see Section 6.2), and should discard
    /// application data traffic that exceeds the bandwidth quota.
    virtual AuthenticationState authenticateRequest(Server* server, Request& request) = 0;
};


typedef std::map<FiveTuple, ServerAllocation*> ServerAllocationMap;


/// TURN server rfc5766 implementation
class TURN_API Server
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
    TCPAllocation* getTCPAllocation(const uint32_t& connectionID);
    net::TCPSocket::Ptr getTCPSocket(const net::Address& remoteAddr);
    void releaseTCPSocket(const net::Socket& socket);

    ServerObserver& observer();
    ServerOptions& options();
    net::UDPSocket& udpSocket();
    net::TCPSocket& tcpSocket();
    Timer& timer();

    void onTCPAcceptConnection(const net::TCPSocket::Ptr& sock);
    void onTCPSocketClosed(net::Socket& socket);
    void onSocketRecv(net::Socket& socket, const MutableBuffer& buffer,
                      const net::Address& peerAddress);
    void onTimer();

private:
    ServerObserver& _observer;
    ServerOptions _options;
    net::SocketEmitter _udpSocket; // net::UDPSocket
    net::SocketEmitter _tcpSocket; // net::TCPSocket
    std::vector<net::SocketEmitter> _tcpSockets;
    ServerAllocationMap _allocations;
    Timer _timer;
};


} } //  namespace scy::turn


#endif // SCY_TURN_Server_H


/// @\}
