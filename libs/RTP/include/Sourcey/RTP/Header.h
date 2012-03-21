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


#ifndef SOURCEY_RTP_HEADER_H
#define SOURCEY_RTP_HEADER_H


#include "Sourcey/RTP/Types.h"
#include "Sourcey/Buffer.h"
#include "Sourcey/RTP/Util.h"

#include <vector>


namespace Sourcey {
namespace RTP {


struct Header 
{
	Header();
	
	UInt8 version;
		// Ver, Version. 2 bits.
		// RTP version number. Always set to 2.

	bool padding;
		// P, Padding. 1 bit.
		// If set, this packet contains one or more additional padding bytes at the end which are not part of the payload. The last byte of the padding contains a count of how many padding bytes should be ignored. Padding may be needed by some encryption algorithms with fixed block sizes or for carrying several RTP packets in a lower-layer protocol data unit.

	bool extension;
		// X, Extension. 1 bit.
		// If set, the fixed header is followed by exactly one header extension.

	UInt8 csrcCount;
		// CC, CSRC count. 4 bits.
		// The number of CSRC identifiers that follow the fixed header.

	bool marker;
		// M, Marker. 1 bit.
		// The interpretation of the marker is defined by a profile. It is intended to allow significant events such as frame boundaries to be marked in the packet stream. A profile may define additional marker bits or specify that there is no marker bit by changing the number of bits in the payload type field.

	UInt8 payloadType;
		// PT, Payload Type. 7 bits.
		// Identifies the format of the RTP payload and determines its interpretation by the application. A profile specifies a default static mapping of payload type codes to payload formats. Additional payload type codes may be defined dynamically through non-RTP means. An RTP sender emits a single RTP payload type at any given time; this field is not intended for multiplexing separate media streams.

	UInt16 sequenceNumber;
		// Sequence Number. 16 bits.
		// The sequence number increments by one for each RTP data packet sent, and may be used by the receiver to detect packet loss and to restore packet sequence. The initial value of the sequence number is random (unpredictable) to make known-plaintext attacks on encryption more difficult, even if the source itself does not encrypt, because the packets may flow through a translator that does.

	UInt32 timestamp;
		// Timestamp. 32 bits.
		// The timestamp reflects the sampling instant of the first octet in the RTP data packet. The sampling instant must be derived from a clock that increments monotonically and linearly in time to allow synchronization and jitter calculations. The resolution of the clock must be sufficient for the desired synchronization accuracy and for measuring packet arrival jitter (one tick per video frame is typically not sufficient). The clock frequency is dependent on the format of data carried as payload and is specified statically in the profile or payload format specification that defines the format, or may be specified dynamically for payload formats defined through non-RTP means. If RTP packets are generated periodically, the nominal sampling instant as determined from the sampling clock is to be used, not a reading of the system clock. As an example, for fixed-rate audio the timestamp clock would likely increment by one for each sampling period. If an audio application reads blocks covering 160 sampling periods from the input device, the timestamp would be increased by 160 for each such block, regardless of whether the block is transmitted in a packet or dropped as silent.

	UInt32 ssrc;
		// SSRC, Synchronization source. 32 bits.
		// Identifies the synchronization source. The value is chosen randomly, with the intent that no two synchronization sources within the same RTP session will have the same SSRC. Although the probability of multiple sources choosing the same identifier is low, all RTP implementations must be prepared to detect and resolve collisions. If a source changes its source transport address, it must also choose a new SSRC to avoid being interpreted as a looped source.

	std::vector<int> csrcList; 
		// CSRC, Contributing source. 32 bits.
		// An array of 0 to 15 CSRC elements identifying the contributing sources for the payload contained in this packet. The number of identifiers is given by the CC field. If there are more than 15 contributing sources, only 15 may be identified. CSRC identifiers are inserted by mixers, using the SSRC identifiers of contributing sources. For example, for audio packets the SSRC identifiers of all sources that were mixed together to create a packet are listed, allowing correct talker indication at the receiver.

#ifdef USING_RTP_TCP
	bool	tcpFriendlyMode;
	UInt32	sendingTimestamp;
	UInt32	rttEstimate;
#endif
	
	virtual int read(const UInt8* buffer, int bufferSize);
	virtual void write(UInt8* buffer, int bufferSize) const;
	
	virtual bool read(Buffer& buffer);
	virtual void write(Buffer& buffer) const;

	size_t size() const;
};


} // namespace RTP
} // namespace Sourcey 


#endif

