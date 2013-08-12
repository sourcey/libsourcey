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


#include "Sourcey/RTP/RTCP/SenderReportPacket.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"



using namespace std;


namespace scy {
namespace rtp {
namespace RTCP {


SenderReportPacket::SenderReportPacket() :
	Packet(Packet::SR)
{
}


SenderReportPacket::~SenderReportPacket() 
{
}


UInt16 SenderReportPacket::computedLength() const 
{
	// A sender report is 20 bytes long
	return Packet::computedLength() + 20;
}


bool SenderReportPacket::read(Buffer& buffer) 
{
	if (RTCP::Packet::read(buffer)) {
		
		buffer.getU32(ssrc);
		buffer.getU32(ntpMsw);
		buffer.getU32(ntpLsw);
		buffer.getU32(rtpTimestamp);
		buffer.getU32(senderPacketCount);
		buffer.getU32(senderOctetCount);
		return true;
	}

	return false;
}


void SenderReportPacket::write(Buffer& buffer) const 
{
	RTCP::Packet::write(buffer);
	
	buffer.putU32(ssrc);
	buffer.putU32(ntpMsw);
	buffer.putU32(ntpLsw);
	buffer.putU32(rtpTimestamp);
	buffer.putU32(senderPacketCount);
	buffer.putU32(senderOctetCount);
}


std::string SenderReportPacket::toString() const
{
    std::stringstream out;
    out << "RTCPSenderReportPacket";
    return out.str();
}


void SenderReportPacket::print(std::ostream& os) const 
{
    os << "RTCPSenderReportPacket:" << endl;
    os << "  version = " << (int)version << endl;
    os << "  padding = " << padding << endl;
    os << "  sources = " << (int)sources << endl;
    os << "  packetType = " << (int)packetType << endl;
    os << "  length = " << (int)computedLength() << endl;
	os << "  ssrc = " << (int)ssrc << endl;
    os << "  ntpMsw = " << (int)ntpMsw << endl;
    os << "  ntpLsw = " << (int)ntpLsw << endl;
    os << "  rtpTimestamp = " << (int)rtpTimestamp << endl;
    os << "  senderPacketCount = " << (int)senderPacketCount << endl;
    os << "  senderOctetCount = " << (int)senderOctetCount << endl;
}


} // namespace RTCP
} // namespace rtp
} // namespace scy 