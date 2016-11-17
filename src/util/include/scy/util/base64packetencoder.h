///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup util
/// @{

#ifndef SCY_HTTP_Base64PacketEncoder_H
#define SCY_HTTP_Base64PacketEncoder_H

#include "scy/base64.h"
#include "scy/packetstream.h"
#include "scy/signal.h"
#include <sstream>

namespace scy {

/// @addtogroup util
class Base64PacketEncoder : public PacketProcessor
{
public:
    Base64PacketEncoder()
        : PacketProcessor(this->emitter)
    {
    }

    virtual void process(IPacket& packet)
    {
        RawPacket& p = dynamic_cast<RawPacket&>(packet); // cast or throw

        base64::Encoder enc;
        std::vector<char> result(packet.size() * 2);
        std::size_t size =
            enc.encode((const char*)p.data(), p.size(), &result[0]);
        size += enc.finalize(&result[size]);

        emit(&result[0], size);
    }

    PacketSignal emitter;
};

} // namespace scy

#endif
