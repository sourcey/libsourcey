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


#ifndef SCY_JSON_Parser_h
#define SCY_JSON_Parser_h


#include "scy/json/json.h"
#include "scy/packetstream.h"
#include <ostream>


namespace scy {
namespace json {

    

/*
class Parser: public PacketProcessor
{
public:
    virtual void parse(RawPacket& packet)
    {
        json::Value data;
        json::Reader reader;
        if (reader.parse(packet.message(), data)) {
        }
    }

    PacketSignal Output;

    virtual ~Parser() {};
};


class Parser: public PacketProcessor
{
public:
    virtual void process(IPacket& packet);

    virtual void parse(RawPacket& packet);

    virtual ~Parser() {};
};
*/


} } // namespace scy::json


#endif // SCY_JSON_Parser.h
