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


#ifndef SOURCEY_RTCP_Packet_H
#define SOURCEY_RTCP_Packet_H


#include "Sourcey/RTP/Types.h"
#include "Sourcey/RTP/RTCP/Packet.h"
#include "Sourcey/IPacket.h"

#include <vector>


namespace scy {
namespace rtp {
namespace RTCP {

	
class Packet: public IPacket 
{
public:
    enum Type
        /// RTCP packet types registered with IANA.
	{
		Unknown	= 0,	///< Huh?
        SR		= 200,	///< Sender Packet.
        RR		= 201,	///< Receiver Packet.
        SDES	= 202,	///< Source DEScription.
        BYE		= 203,	///< End of participation.
        APP		= 204,	///< APPlication specific.
        FIR		= 192,	///< Full Intra-frame request.
        NACK	= 193,	///< Negative ACK.
        XR				///< Extended Packet.
    };

	Packet(Type type);
	Packet(const Packet& rhs);
	virtual ~Packet();

	virtual IPacket* clone() const;

	virtual bool read(Buffer& buffer);
	virtual void write(Buffer& buffer) const;
	
	virtual std::string toString() const;	
	virtual void print(std::ostream& os) const;	
	
	virtual Type getPacketType() const;
	virtual int getRtcpLength() const;
		// RTCP length is the header field length of an 
		// RTCP packet in 32 bit words minus one.
	
	UInt8	version;		// version (V): 2 bits
	bool	padding;		// padding (P): 1 bit
	UInt8	sources;		// reception report or source count (RC/SC): 5 bits
	UInt8	packetType;		// packet type (PT): 8 bits
	UInt16  length;			// length: 16 bits
		// This length value is only set when reading the 
		// packet from a buffer using the read() method.
		// After a successful read this value should match
		// the computedLength().

	virtual UInt16 computedLength() const;
		// The dynamically generated packet length.	This
		// method is used when writing the packet to a
		// buffer using the write() method.
		
	virtual size_t size() const { 
		// dynamically generated
		return 0;
	}
		
	virtual const char* className() const { return "RTCPPacket"; }
};


} // namespace RTCP
} // namespace rtp
} // namespace scy 


#endif