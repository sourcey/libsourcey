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


#include "Sourcey/RTP/RTCP/Packet.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"


using namespace std;


namespace scy {
namespace rtp {
namespace RTCP {


Packet::Packet(Type type) : 
	version(0), 
	padding(0), 
	sources(0), 
	packetType(static_cast<UInt8>(type))
{
}


Packet::Packet(const Packet& rhs) : 
	version(rhs.version), 
	padding(rhs.padding), 
	sources(rhs.sources), 
	packetType(rhs.packetType), 
	length(rhs.length)
{
}


Packet::~Packet() 
{
}


IPacket* Packet::clone() const 
{
	return new Packet(*this);
}


RTCP::Packet::Type Packet::getPacketType() const
{
	return static_cast<RTCP::Packet::Type>(packetType);
}


UInt16 Packet::computedLength() const 
{
    // RTCP header length size is 4 bytes
    // not all rtcp packets (in particular SDESPacket) have
    // the ssrc identifier stored in the header
	return 4;
}


int Packet::getRtcpLength() const 
{
	return (int)(computedLength() / 4) - 1;
}



bool Packet::read(Buffer& buffer) 
{
	// TODO: Proper checking...

	if (buffer.available() < 4) {
		errorL() << "RTCP: Buffer too small to parse RTCP header. " 
			<< "Needed 4 but got " << buffer.available() << "."
			<< endl;
		return false;
	}
	
	UInt8 info;
	buffer.getU8(info);
	this->version = info >> 6;
	this->padding = info >> 5 & 0x1;
	this->sources = info & 0x1F;
	
	buffer.getU8(this->packetType);
	buffer.getU16(this->length);

	if (buffer.available() < this->length - 4) {
		errorL() 
			<< "RTCP: Buffer too small to parse RTCP packet: " << (int)this->packetType << ". " 
			<< "Needed " << (this->length - 4) << " but got " << buffer.available() << "."
			<< endl;
		return false;
	}

	return true;
}


void Packet::write(Buffer& buffer) const 
{
	UInt8 info = (kRtpVersionNumber << 6) |
		((UInt8)this->padding << 5) |
		this->sources;

	buffer.putU8(info);
	buffer.putU8(this->packetType);
	buffer.putU16(this->computedLength());
}


std::string Packet::toString() const
{
    std::stringstream out;
    out << "RTCPPacket";
    return out.str();
}


void Packet::print(std::ostream& os) const 
{
    os << "RTCPPacket:" << endl;
    os << "  version = " << (int)version << endl;
    os << "  padding = " << padding << endl;
    os << "  sources = " << (int)sources << endl;
    os << "  packetType = " << (int)packetType << endl;
    os << "  length = " << (int)computedLength() << endl;
}


} // namespace RTCP
} // namespace rtp
} // namespace scy