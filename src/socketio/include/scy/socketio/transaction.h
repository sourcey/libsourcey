///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
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


class SocketIO_API Client;


struct SocketIO_API Transaction : public PacketTransaction<sockio::Packet>
{
    Transaction(sockio::Client& client, long timeout = 10000);
    Transaction(sockio::Client& client, const sockio::Packet& request, long timeout = 10000);

    virtual bool send() override;
    virtual bool checkResponse(const sockio::Packet& packet) override;

    virtual void onPotentialResponse(sockio::Packet& packet);
    virtual void onResponse() override;

    virtual const char* className() const { return "SocketIO::Transaction"; }

    sockio::Client& client;

protected:
    virtual ~Transaction();
};


} // namespace sockio
} // namespace scy


#endif // SCY_SocketIO_Transaction_H


/// @\}
