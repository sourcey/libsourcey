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


#ifndef SOURCEY_RTCP_ReceiverReportPacket_H
#define SOURCEY_RTCP_ReceiverReportPacket_H


#include "Sourcey/RTP/RTCP/Packet.h"

#include <vector>


namespace Sourcey {
namespace RTP {
namespace RTCP {


struct ReceiverReportBlock 
	/// Struct for the data contained in a receiver info
	/// block. Receiver info blocks can be found in SR (sender
	/// report) or RR (receiver report) RTCP reports.
{
	UInt32 ssrc;
    UInt8 fractionLost;      ///< report fraction lost.
    UInt16 totalLost;        ///< cumulative number of lost reports.
    UInt32 highestSeqNum;    ///< highest sequence number.
    UInt32 jitter;           ///< arrival jitter.
    UInt32 lsr;              ///< last sender report timestamp.
    UInt32 dlsr;             ///< delay since last sender report.

	ReceiverReportBlock(UInt32 ssrc = 0) :
		ssrc(ssrc),
		fractionLost(0),
		totalLost(0),
		highestSeqNum(0),
		jitter(0),
		lsr(0),
		dlsr(0) {}

	virtual void print(std::ostream& os) const
	{
		os << "RTCPReceiverReportBlock:" << std::endl;
		os << "  ssrc = " << (int)ssrc << std::endl;
		os << "  fractionLost = " << (int)fractionLost << std::endl;
		os << "  totalLost = " << (int)totalLost << std::endl;
		os << "  highestSeqNum = " << (int)highestSeqNum << std::endl;
		os << "  jitter = " << (int)jitter << std::endl;
		os << "  lsr = " << (int)lsr << std::endl;
		os << "  dlsr = " << (int)dlsr << std::endl;
	}
};

	
class ReceiverReportPacket: public RTCP::Packet
	/// This class represents RTCP receiver report report. It can hold 0 to 31
	/// ReceptionReports. Also the header field ssrc is included.
	///
	/// 	    0                   1                   2                   3
	///         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	/// header |V=2|P|    RC   |   PT=RR=201   |             length            |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                     SSRC of packet sender                     |
	///        +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	/// report |                 SSRC_1 (SSRC of first source)                 |
	/// block  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///   1    | fraction lost |       cumulative number of packets lost       |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |           extended highest sequence number received           |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                      interarrival jitter                      |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                         last SR (LSR)                         |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                   delay since last SR (DLSR)                  |
	///        +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	/// report |                 SSRC_2 (SSRC of second source)                |
	/// block  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///   2    :                               ...                             :
	///        +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	///        |                  profile-specific extensions                  |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
{
public:
	ReceiverReportPacket();
	virtual ~ReceiverReportPacket();

	// TODO: clone()
	
	std::vector<ReceiverReportBlock*>& reports();
    virtual void addReport(ReceiverReportBlock* report);
		// Adds a receiver report to this receiver report report.
	
	virtual bool read(Buffer& buffer);
	virtual void write(Buffer& buffer) const;
	
	virtual std::string toString() const;	
	virtual void print(std::ostream& os) const;
	
	virtual UInt16 computedLength() const;	
	
	UInt32 ssrc;

	virtual const char* className() const { return "RTCPReceiverReportPacket"; }

protected:
	std::vector<ReceiverReportBlock*> _reports;	
};


} // namespace RTCP
} // namespace RTP
} // namespace Sourcey 


#endif