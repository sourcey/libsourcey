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


#include "Sourcey/RTP/RTCP/ReceiverReportPacket.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"



using namespace std;


namespace scy {
namespace rtp {
namespace RTCP {


ReceiverReportPacket::ReceiverReportPacket() :
	Packet(Packet::RR)
{
}


ReceiverReportPacket::~ReceiverReportPacket() 
{
}


UInt16 ReceiverReportPacket::computedLength() const 
{
	// An empty RTCP receiver report report is 4 bytes
    // longer because of the ssrc identifier stored in it
	UInt16 len = Packet::computedLength() + 4;
	
	// An RTCP receiver report is 24 bytes long
    len += _reports.size() * 24;
	return len;
}


bool ReceiverReportPacket::read(Buffer& buffer) 
{

	if (RTCP::Packet::read(buffer)) {
		
		buffer.readU32(ssrc);
		
		int remaining = length - 4 - 4;
		while (remaining >= 24) {			
			ReceiverReportBlock* report = new ReceiverReportBlock;
			buffer.readU32(report->ssrc);
			buffer.readU8(report->fractionLost);
			buffer.readU16(report->totalLost);
			buffer.readU32(report->highestSeqNum);
			buffer.readU32(report->jitter);
			buffer.readU32(report->lsr);
			buffer.readU32(report->dlsr);
			addReport(report);
			remaining -= 24;
		}

		return true;
	}

	return false;
}


void ReceiverReportPacket::write(Buffer& buffer) const 
{
	RTCP::Packet::write(buffer);
	
	buffer.writeU32(ssrc);
		
	for (std::vector<ReceiverReportBlock*>::const_iterator it = _reports.begin(); it != _reports.end(); ++it) {
		ReceiverReportBlock* report = *it;
		buffer.writeU32(report->ssrc);
		buffer.writeU8(report->fractionLost);
		buffer.writeU16(report->totalLost);
		buffer.writeU32(report->highestSeqNum);
		buffer.writeU32(report->jitter);
		buffer.writeU32(report->lsr);
		buffer.writeU32(report->dlsr);
	}
}


vector<ReceiverReportBlock*>& ReceiverReportPacket::reports() 
{
	return _reports;
}


void ReceiverReportPacket::addReport(ReceiverReportBlock* report) 
{    
	this->sources++;
	_reports.push_back(report);
}


std::string ReceiverReportPacket::toString() const
{
    std::stringstream out;
    out << "RTCPReceiverReportPacket";
    return out.str();
}


void ReceiverReportPacket::print(std::ostream& os) const 
{
    os << "RTCPReceiverReportPacket:" << endl;
    os << "  version = " << (int)version << endl;
    os << "  padding = " << padding << endl;
    os << "  sources = " << (int)sources << endl;
    os << "  packetType = " << (int)packetType << endl;
    os << "  length = " << (int)computedLength() << endl;
	for (unsigned i = 0; i < _reports.size(); i++) {
		_reports[i]->print(os);
	}
}


} // namespace RTCP
} // namespace rtp
} // namespace scy 