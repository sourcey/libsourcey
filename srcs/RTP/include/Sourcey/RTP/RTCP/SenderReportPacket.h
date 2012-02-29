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


#ifndef SOURCEY_RTCP_SenderReportPacket_H
#define SOURCEY_RTCP_SenderReportPacket_H


#include "Sourcey/RTP/RTCP/Packet.h"

#include <vector>


namespace Sourcey {
namespace RTP {
namespace RTCP {


struct SenderReportInfo 
{
	UInt32 ntpMsw;
	UInt32 ntpLsw;
	UInt32 rtpTimestamp;
	UInt32 senderPacketCount;
	UInt32 senderOctetCount;

	SenderReportInfo() :
		ntpMsw(0),
		ntpLsw(0),
		rtpTimestamp(0),
		senderPacketCount(0),
		senderOctetCount(0) {}
};


class SenderReportPacket: public RTCP::Packet, public SenderReportInfo
	/// The class RTPSenderInfo is used by an RTP end system for storing information
	/// about itself. With the stored information it can create a SenderReport.
	///
	///         0                   1                   2                   3
	///         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	/// header |V=2|P|    RC   |   PT=SR=200   |             length            |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                         SSRC of sender                        |
	///        +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	/// sender |              NTP timestamp, most significant word             |
	/// info   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |             NTP timestamp, least significant word             |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                         RTP timestamp                         |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                     sender's packet count                     |
	///        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	///        |                      sender's octet count                     |
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
	SenderReportPacket();
	virtual ~SenderReportPacket();

	// TODO: clone()

	virtual bool read(Buffer& buffer);
	virtual void write(Buffer& buffer) const;
	
	virtual std::string toString() const;	
	virtual void print(std::ostream& os) const;
	
	virtual UInt16 computedLength() const;	

	UInt32 ssrc;

	virtual const char* className() const { return "RTCPSenderReportPacket"; }
};


} // namespace RTCP
} // namespace RTP
} // namespace Sourcey 


#endif