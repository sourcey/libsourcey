//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#include "Sourcey/RTP/RTCP/SenderReportPacket.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"



using namespace std;


namespace Sourcey {
namespace RTP {
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
		
		buffer.readUInt32(ssrc);
		buffer.readUInt32(ntpMsw);
		buffer.readUInt32(ntpLsw);
		buffer.readUInt32(rtpTimestamp);
		buffer.readUInt32(senderPacketCount);
		buffer.readUInt32(senderOctetCount);
		return true;
	}

	return false;
}


void SenderReportPacket::write(Buffer& buffer) const 
{
	RTCP::Packet::write(buffer);
	
	buffer.writeUInt32(ssrc);
	buffer.writeUInt32(ntpMsw);
	buffer.writeUInt32(ntpLsw);
	buffer.writeUInt32(rtpTimestamp);
	buffer.writeUInt32(senderPacketCount);
	buffer.writeUInt32(senderOctetCount);
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
} // namespace RTP
} // namespace Sourcey 