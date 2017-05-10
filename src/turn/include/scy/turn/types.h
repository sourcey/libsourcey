///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#ifndef SCY_TURN_Types_H
#define SCY_TURN_Types_H


#include "scy/net/address.h"
#include "scy/net/socket.h"
#include "scy/stun/message.h"
#include "scy/turn/turn.h"


namespace scy {
namespace turn {


const int CLIENT_SOCK_BUF_SIZE = 65536;
const int SERVER_SOCK_BUF_SIZE = CLIENT_SOCK_BUF_SIZE * 32;


enum AuthenticationState
{
    Authenticating = 1,
    Authorized = 2,
    QuotaReached = 3,
    NotAuthorized = 4
};


class TURN_API Request : public stun::Message
{
public:
    net::TransportType transport;
    net::Address localAddress;
    net::Address remoteAddress;
    std::string hash; // for MessageIntegrity signing

    Request(const stun::Message& message, net::TransportType transport,
            const net::Address& localAddress = net::Address(),
            const net::Address& remoteAddress = net::Address())
        : stun::Message(message)
        , transport(transport)
        , localAddress(localAddress)
        , remoteAddress(remoteAddress)
    {
    }
};


typedef std::vector<std::string> IPList;


} // namespace turn
} // namespace scy


#endif // SCY_TURN_Types_H


/// @\}
