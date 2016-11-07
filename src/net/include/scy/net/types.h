///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_Types_H
#define SCY_Net_Types_H


#include "scy/stateful.h"


// Conflicts with WebRTC: webrtc/base/socket.h:116
// #if defined(UNIX) && !defined(INVALID_SOCKET)
// #define INVALID_SOCKET -1
// #endif

#if defined(WIN32)
typedef int socklen_t;
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


} } // namespace scy::net


#endif
