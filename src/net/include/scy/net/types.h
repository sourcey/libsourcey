///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_Types_H
#define SCY_Net_Types_H


#include "scy/base.h"

#if defined(SCY_WIN)
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
// typedef SOCKET scy_socket_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
// typedef int scy_socket_t;
// Conflicts with WebRTC: webrtc/base/socket.h:116
// #define INVALID_SOCKET -1
#endif

#define LibSourcey_HAVE_IPv6 1 // fixme


namespace scy {
namespace net {


const int MAX_TCP_PACKET_SIZE = 64 * 1024;
const int MAX_UDP_PACKET_SIZE = 1500;


enum TransportType
{
    UDP,
    TCP,
    SSLTCP
};


enum SocketMode
{
    ServerSide, ///< Server-side adapter.
    ClientSide  ///< Client-side adapter.
};


} // namespace net
} // namespace scy


#endif
