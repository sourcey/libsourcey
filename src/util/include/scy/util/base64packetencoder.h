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


#ifndef SCY_HTTP_Base64PacketEncoder_H
#define SCY_HTTP_Base64PacketEncoder_H


#include "scy/packetstream.h"
#include "scy/signal.h"
#include "scy/base64.h"
#include <sstream>


namespace scy { 


class Base64PacketEncoder: public PacketProcessor
{
public:
    Base64PacketEncoder() :
        PacketProcessor(this->emitter)
    {
    }

    virtual void process(IPacket& packet)
    {        
        RawPacket& p = dynamic_cast<RawPacket&>(packet); // cast or throw

        base64::Encoder enc;
        std::vector<char> result(packet.size() * 2);
        std::size_t size = enc.encode((const char*)p.data(), p.size(), &result[0]);        
        size += enc.finalize(&result[size]);

        emit(&result[0], size);
    }

    PacketSignal emitter;
};


} // namespace scy


#endif