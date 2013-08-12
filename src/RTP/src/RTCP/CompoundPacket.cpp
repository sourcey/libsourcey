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


#include "Sourcey/RTP/RTCP/CompoundPacket.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"


using namespace std;


namespace scy {
namespace rtp {
namespace RTCP {
	

#if __BYTE_ORDER == __BIG_ENDIAN
const UInt16 CompoundPacket::RTCP_VALID_MASK = (0xc000 | 0x2000  | 0xfe);
const UInt16 CompoundPacket::RTCP_VALID_VALUE = ((kRtpVersionNumber << 14) | RTCP::Packet::SR);
#else
const UInt16 CompoundPacket::RTCP_VALID_MASK = (0x00c0 | 0x0020 | 0xfe00);
const UInt16 CompoundPacket::RTCP_VALID_VALUE = ((kRtpVersionNumber << 6) | (RTCP::Packet::SR << 8));
#endif


CompoundPacket::CompoundPacket() 
{
}


CompoundPacket::~CompoundPacket() 
{
	util::clearVector(_packets);
}


bool CompoundPacket::read(Buffer& buffer) 
{
	if (buffer.available() < 4) {
		errorL() << "RTCP: Received empty packet." << endl;
		return false;
	}
	
	// For all RTCP packets in the UDP packet
	// TODO: Specify error sources so we don't get stuck in infinite loop.
	int errorCount = 0;
	int maxErrors = 6;
	while (buffer.available() > 0) {

		//UInt8* data = reinterpret_cast<UInt8*>(buffer.data());	
		int startPos = buffer.position();
		buffer.consume(1);
		//buffer++;
		UInt8 type;
		UInt16 length;
		buffer.getU8(type);
		buffer.getU16(length);
		buffer.position(startPos);
		
		debugL() << "RTCP: Parsing packet with type " 
			<< (int)type << " and length: " << (int)length << ". " 
			<< "Remaining in buffer " << buffer.available() << "."
			<< endl;

		Packet* packet = NULL;
		switch (type) {
			case Packet::SR:
				packet = new SenderReportPacket;
				break;

			case Packet::RR:
				packet = new ReceiverReportPacket;
				break;

			case Packet::SDES:
				packet = new SDESPacket;
				break;

			case Packet::APP:
				packet = new AppPacket;
				break;

			case Packet::BYE:
				packet = new ByePacket;
				break;

			default:
				errorL() << "RTCP: Parsed unknown packet type " << (int)type << endl;                
				goto error;
				break;
		};
		
		if (!packet->read(buffer)) {
			errorL() << "RTCP: Failed to parse packet with type " << (int)type << endl;            
			goto error;
		}

		// successfully read packet, add and continue...
		addPacket(packet);
		continue;

		// error reading packet...
		error: 

		if (packet)
			delete packet;

		// consume failed packet bytes...
		buffer.position(startPos);
		buffer.consume(length);
		errorCount++;
		if (errorCount > maxErrors) {
			errorL() << "RTCP: Parse failed because of too many errors: " << errorCount << endl;
			return false;
		}
	}

	return true;
}


void CompoundPacket::write(Buffer& buffer) const 
{
    for (int i = 0; i < _packets.size(); i++) {		
		//debugL() << "RTCP: Writing packet:" << endl;
		//_packets[i]->print(cout);
		_packets[i]->write(buffer);
	}
}


vector<Packet*>& CompoundPacket::packets() 
{
	return _packets;
}


void CompoundPacket::addPacket(Packet* packet) 
{
	_packets.push_back(packet);
}


int CompoundPacket::computedLength()
{
	return _length;
}


std::string CompoundPacket::toString() const
{
    std::stringstream out;
    out << "RTCPCompoundPacket: number of rtcp packets=" << _packets.size();
    return out.str();
};


void CompoundPacket::print(std::ostream& os) const 
{
    os << "RTCPCompoundPacket:" << endl;
    for (int i = 0; i < _packets.size(); i++) {
        _packets[i]->print(os);
    }
};



} // namespace RTCP
} // namespace rtp
} // namespace scy 