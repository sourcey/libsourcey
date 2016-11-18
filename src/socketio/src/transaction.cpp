///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup socketio
/// @{


#include "scy/socketio/transaction.h"
#include "scy/logger.h"
#include "scy/socketio/client.h"
#include <iostream>


using std::endl;


namespace scy {
namespace sockio {


Transaction::Transaction(Client& client, long timeout)
    : PacketTransaction<Packet>(timeout, 0,
                                client.ws().socket /*.base()*/->loop())
    , client(client)
{
    TraceN(this) << "Create" << endl;
}


Transaction::Transaction(Client& client, const Packet& request, long timeout)
    : PacketTransaction<Packet>(request, timeout, 0,
                                client.ws().socket /*.base()*/->loop())
    , client(client)
{
    TraceN(this) << "Create" << endl;
}


Transaction::~Transaction()
{
    TraceN(this) << "Destroy" << endl;
}


bool Transaction::send()
{
    TraceN(this) << "Send: " << _request.id() << endl;
    _request.setAck(true);
    client += slot(this, &Transaction::onPotentialResponse, -1, 100);
    if (client.send(_request))
        return PacketTransaction<Packet>::send();
    return false;
}


void Transaction::onPotentialResponse(IPacket& pkt)
{
    auto packet = dynamic_cast<sockio::Packet*>(&pkt);
    if (packet) {
        TraceN(this) << "On potential response: " << packet->id() << endl;
        PacketTransaction<Packet>::handlePotentialResponse(*packet);
    }
}


bool Transaction::checkResponse(const Packet& packet)
{
    TraceN(this) << "Check response: " << packet.id() << endl;
    return _request.id() == packet.id();
}


void Transaction::onResponse()
{
    TraceN(this) << "On success" << endl;
    client -= slot(this, &Transaction::onPotentialResponse);
    PacketTransaction<Packet>::onResponse();
}


} // namespace sockio
} // namespace scy


/// @\}
