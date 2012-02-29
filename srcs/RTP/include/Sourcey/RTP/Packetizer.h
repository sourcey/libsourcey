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


#ifndef SOURCEY_RTP_PACKETIZER_H
#define SOURCEY_RTP_PACKETIZER_H


#include "Sourcey/IPacketizer.h"
#include "Sourcey/Signal.h"
#include "Sourcey/RTP/Packet.h"


namespace Sourcey {
namespace RTP {


class Packetizer: public IPacketizer
	/// This class automates packetizing of an arbitrary stream 
	/// into RTP packets.
{
public:
	Packetizer(int mtu = RTP_MAX_PACKET_LEN) :
		_sequenceNumber(0),
		_headerLength(12),
		_payloadType(0),
		_timestamp(0),
		_ssrc(0)
	{
		setMTU(mtu);
	}

	virtual void process(IPacket& packet)
		/// Processes incoming data, encodes it into RTP packets,
		/// and sends it to the packet handler. 
	{
		Buffer buf;
		packet.write(buf);

		int size = 0;
		while (!buf.eof()) {		
			size = std::min<int>((int)buf.size() - buf.position(), (int)_mtu - _headerLength);			
			_timestamp = Util::getTime();
			RTP::Packet rtpPacket(buf.data(), size, _sequenceNumber++, _timestamp, _ssrc);
			buf.consume(size);
			rtpPacket.header().payloadType = _payloadType;
			rtpPacket.header().marker = buf.eof();
			dispatch(this, rtpPacket);
		}
	};

	void setHeaderLength(UInt8 l)	{ _headerLength = l; }
	void setPayloadType(UInt8 t)	{ _payloadType = t; }
	void setTimestamp(UInt32 t)		{ _timestamp =  t; }
	void setSSRC(UInt32 s)			{ _ssrc =  s; }

protected:
	UInt32 _sequenceNumber;
	UInt8  _headerLength;
	UInt8  _payloadType;
	UInt32 _timestamp;
	UInt32 _ssrc;
};


} // namespace RTP
} // namespace Sourcey 


#endif
