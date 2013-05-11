//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/RTP/RTCP/AppPacket.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"


using namespace std;


namespace Scy {
namespace RTP {
namespace RTCP {


AppPacket::AppPacket() :
	Packet(Packet::APP)
{    
}


AppPacket::~AppPacket() 
{
}


UInt16 AppPacket::computedLength() const 
{
	return Packet::computedLength() 
		+ 4					// ssrc
		+ 4					// name
		+ data.length();	// data
}


bool AppPacket::read(Buffer& buffer) 
{
	if (RTCP::Packet::read(buffer)) {
		
		if (buffer.size() < 4) {
			LogError() << "Buffer too short to parse APP packet." << endl;
			return false;
		}
		
		buffer.readUInt32(ssrc);		
		buffer.readString(name, 4);
		buffer.readString(data, length - 12);
		return true;
	}

	return false;
}


void AppPacket::write(Buffer& buffer) const 
{
	assert(name.length() <= 4); // name is one octet in length

	RTCP::Packet::write(buffer);	
	buffer.writeUInt32(ssrc);	
	buffer.writeBytes(name.data(), 4);
	buffer.writeString(data);
}


std::string AppPacket::toString() const
{
    std::stringstream out;
    out << "RTCPAppPacket";
    return out.str();
}


void AppPacket::print(std::ostream& os) const 
{
	//RTCP::Packet::print(os);
    os << "RTCPAppPacket:" << endl;
    os << "  version = " << (int)version << endl;
    os << "  padding = " << padding << endl;
    os << "  sources = " << (int)sources << endl;
    os << "  packetType = " << (int)packetType << endl;
    os << "  length = " << (int)computedLength() << endl;
    os << "  name = " << name << endl;
    os << "  data = " << data << endl;
}


} // namespace RTCP
} // namespace RTP
} // namespace Scy 