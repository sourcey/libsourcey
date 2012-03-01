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


#include "Sourcey/RTP/RTCP/ReceiverReportPacket.h"
#include "Sourcey/ByteOrder.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"



using namespace std;


namespace Sourcey {
namespace RTP {
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
		
		buffer.readUInt32(ssrc);
		
		int remaining = length - 4 - 4;
		while (remaining >= 24) {			
			ReceiverReportBlock* report = new ReceiverReportBlock;
			buffer.readUInt32(report->ssrc);
			buffer.readUInt8(report->fractionLost);
			buffer.readUInt16(report->totalLost);
			buffer.readUInt32(report->highestSeqNum);
			buffer.readUInt32(report->jitter);
			buffer.readUInt32(report->lsr);
			buffer.readUInt32(report->dlsr);
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
	
	buffer.writeUInt32(ssrc);
		
	for (std::vector<ReceiverReportBlock*>::const_iterator it = _reports.begin(); it != _reports.end(); ++it) {
		ReceiverReportBlock* report = *it;
		buffer.writeUInt32(report->ssrc);
		buffer.writeUInt8(report->fractionLost);
		buffer.writeUInt16(report->totalLost);
		buffer.writeUInt32(report->highestSeqNum);
		buffer.writeUInt32(report->jitter);
		buffer.writeUInt32(report->lsr);
		buffer.writeUInt32(report->dlsr);
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
} // namespace RTP
} // namespace Sourcey 