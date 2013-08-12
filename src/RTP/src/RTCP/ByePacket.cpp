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


#include "Sourcey/RTP/RTCP/ByePacket.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"



using namespace std;


namespace scy {
namespace rtp {
namespace RTCP {


ByePacket::ByePacket() :
	Packet(Packet::BYE)
{    
}


ByePacket::~ByePacket() 
{
}


UInt16 ByePacket::computedLength() const 
{
	// An RTCP BYE packet is 4 bytes longer because
    // the ssrc identifier is stored in it
	UInt16 len = Packet::computedLength() + 4;
	
	// An RTCP receiver report is 24 bytes long
	if (!reason.empty()) {
		len += reason.length();
	}

	return len;
}


bool ByePacket::read(Buffer& buffer) 
{
	if (RTCP::Packet::read(buffer)) {
		
		buffer.getU32(ssrc);

		if (this->computedLength() > 8) {
			buffer.get(reason, this->computedLength() - 8 - 1);
		}
		return true;
	}

	return false;
}


void ByePacket::write(Buffer& buffer) const 
{
	RTCP::Packet::write(buffer);
	
	buffer.putU32(ssrc);
	
	if (!reason.empty()) {
		buffer.putU8(static_cast<UInt8>(reason.length()));
		buffer.put(reason);
	}
}


std::string ByePacket::toString() const
{
    std::stringstream out;
    out << "RTCPByePacket";
    return out.str();
}


void ByePacket::print(std::ostream& os) const 
{
	
    os << "RTCPAppPacket:" << endl;
    os << "  version = " << (int)version << endl;
    os << "  padding = " << padding << endl;
    os << "  sources = " << (int)sources << endl;
    os << "  packetType = " << (int)packetType << endl;
    os << "  length = " << (int)computedLength() << endl;
    os << "  ssrc = " << ssrc << endl;
    os << "  reason = " << reason << endl;
}


} // namespace RTCP
} // namespace rtp
} // namespace scy 