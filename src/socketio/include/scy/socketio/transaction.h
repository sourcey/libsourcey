///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup socketio
/// @{


#ifndef SCY_SocketIO_Transaction_H
#define SCY_SocketIO_Transaction_H


#include "scy/packettransaction.h"
#include "scy/socketio/packet.h"


namespace scy {
namespace sockio {


class /* SCY_EXTERN */ Client;


struct Transaction : public PacketTransaction<sockio::Packet>
{
    Transaction(sockio::Client& client, long timeout = 10000);
    Transaction(sockio::Client& client, const sockio::Packet& request, long timeout = 10000);

    virtual bool send();
    virtual bool checkResponse(const sockio::Packet& packet);

    virtual void onPotentialResponse(sockio::Packet& packet);
    virtual void onResponse();

    virtual const char* className() const { return "SocketIO::Transaction"; }

    sockio::Client& client;

protected:
    virtual ~Transaction();
};


} // namespace sockio
} // namespace scy


#endif // SCY_SocketIO_Transaction_H


/// @\}
