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
		
		buffer.getU32(ssrc);
		
		int remaining = length - 4 - 4;
		while (remaining >= 24) {			
			ReceiverReportBlock* report = new ReceiverReportBlock;
			buffer.getU32(report->ssrc);
			buffer.getU8(report->fractionLost);
			buffer.getU16(report->totalLost);
			buffer.getU32(report->highestSeqNum);
			buffer.getU32(report->jitter);
			buffer.getU32(report->lsr);
			buffer.getU32(report->dlsr);
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
	
	buffer.putU32(ssrc);
		
	for (std::vector<ReceiverReportBlock*>::const_iterator it = _reports.begin(); it != _reports.end(); ++it) {
		ReceiverReportBlock* report = *it;
		buffer.putU32(report->ssrc);
		buffer.putU8(report->fractionLost);
		buffer.putU16(report->totalLost);
		buffer.putU32(report->highestSeqNum);
		buffer.putU32(report->jitter);
		buffer.putU32(report->lsr);
		buffer.putU32(report->dlsr);
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