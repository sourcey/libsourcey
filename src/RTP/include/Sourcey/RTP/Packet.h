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


#ifndef SOURCEY_RTP_PACKET_H
#define SOURCEY_RTP_PACKET_H


#include "Sourcey/RTP/Header.h"
#include "Sourcey/IPacket.h"


namespace Scy {
namespace RTP {


class Packet: public IPacket 
	/// This class implements the RTP packet.
	/// 
	/// As a specific feature the RTP packet class handles the extension
	/// header feature of RTP. This is because the extension header is not
	/// standard RTP header but contains more application related data.
{
public:
	//Packet();
	Packet(const char* payload = NULL, int payloadLength = 0,
		UInt16 sequenceNumber = 0, UInt32 timestamp = 0, UInt32 ssrc = 0);
	Packet(const Header& header, const char* payload, int payloadLength);	
	Packet(const Packet& rhs);
	virtual ~Packet();

	virtual IPacket* clone() const;

	static Packet* readPacket(const char* buf, int size);

	Header& header() { return _header; }

	const char* payload() { return (_payloadLength > 0) ? _payload : NULL; }
	int payloadLength() const { return _payloadLength; }
	int extensionLength() const { return _extensionLength; };
		//
		// Get length of extension header in bytes (not in words).
		//
		// @return
		//   Length in bytes of extension header
		//

	const char* extensionHeader() { return _extensionHeader; };
		//
		// Get pointer to extension header data.
		//
		// The pointer is returned as unsigned char because the extension
		// header data structure is application depended.
		//
		// @return
		//   Returns the pointer to the extension header
		//

	void setExtHeader(const char* data, int length);
		//
		// Set extension header for this RTP packet.
		//
		// This method prepends the extension header to data thay may already
		// exist in this packet. The method updates the overall data _payload
		// as well as the length.
		//
		// @param data
		//   Pointer to extension header data.
		// @param length
		//   Length of extension header data in bytes.
		//

	void enableZrtpChecksum() { _zrtpChecksum = 2; }
		//
		// Set this variable if this packet is a ZRTP packet.
		//
		// ZRTP requires a checksum to be appended to the packet. The
		// value is the number of bytes of the checksum. This checksum
		// algorithm is the standard IP algorithm (ones' complement of
		// the sum of the data).
		//
		//

	bool checkZrtpChecksum(bool check);
		//
		// Recompute an check ZRTP checksum
		//
		// If the <em>check</em> parameter is true then
		// this method recomputes the ZRTP checksum and compares it
		// with the checksum contained in the packet. If the parameter
		// is set to false the method only adjusts the length of the
		// data.
		//
		// @param check
		//    if <code>true</code> recompute and check, otherwise
		//    adjust length of data only.
		// @return <code>true</code> if check is ok or only length was
		//    adjusted. If check fails returns <code>false</code>
		//

	UInt16 computeChecksum(UInt16* data, int length) const;

	virtual size_t size() const;

	virtual bool read(Buffer&);
	virtual void write(Buffer&) const;
	virtual void print(std::ostream& os) const;
		
	virtual const char* className() const { return "RTPPacket"; }

protected:
	Header _header;
	int _zrtpChecksum;
	int _payloadLength;
	char* _payload;
	int _extensionLength;
	char* _extensionHeader;
};


} // namespace RTP
} // namespace Scy 


#endif
