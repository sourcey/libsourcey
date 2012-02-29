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


#include "Sourcey/RTP/Packet.h"
#include "Sourcey/Logger.h"
#include "Sourcey/ByteOrder.h"


using namespace std;


namespace Sourcey {
namespace RTP {


Packet::Packet(const char* payload, int payloadLength,
			   UInt16 sequenceNumber, UInt32 timestamp, UInt32 ssrc) :
	_payload(NULL),
	_payloadLength(payloadLength),
	_extensionHeader(NULL),
	_extensionLength(0),
	_zrtpChecksum(0) 
{
    _header.version = 2;
    _header.sequenceNumber = sequenceNumber;
    _header.timestamp = timestamp;
    _header.ssrc = ssrc;

    if (_payloadLength){
		assert(_payloadLength > 0 && _payloadLength < 0xFFFF);
		_payload = new char[_payloadLength];
		memcpy(_payload, payload, _payloadLength);
    }
}


Packet::Packet(const Header& header, const char* payload, int payloadLength) : 
	_header(header),
	_payload(NULL),
	_payloadLength(0),
	_extensionHeader(NULL),
	_extensionLength(0),
	_zrtpChecksum(0) 
{
	// Check if packet contains an extension header. If yes
	//	* set pointer to extension header, compute length and
	//	* adjust payload pointer and payloadLength
	if (_header.extension && payloadLength >= 4) {
		_extensionLength = 4;
		payloadLength -= 4;	// Minimum size of extension header

		short tmp = *((short *)(payload+2));
		tmp = NetworkToHost16(tmp);
		tmp *= 4;		// ext. _header length is in words (4 bytes)
		_extensionLength += tmp;
		payloadLength -= tmp;

		if (payloadLength >= 0) {
			_extensionHeader = new char[_extensionLength];
			memcpy(_extensionHeader, payload, _extensionLength);
		}
	}
	_payloadLength = payloadLength;

	if (_payloadLength > 0) {
		_payload = new char[_payloadLength];
		memcpy(_payload, payload + _extensionLength, _payloadLength);
	}
}


Packet::Packet(const Packet& rhs) : 
	_header(rhs._header), 
	_zrtpChecksum(rhs._zrtpChecksum), 
	_payloadLength(rhs._payloadLength), 
	_extensionLength(rhs._extensionLength)
{
	_extensionHeader = new char[rhs._extensionLength];
	memcpy(_extensionHeader, rhs._extensionHeader, rhs._extensionLength);
	
	_payload = new char[rhs._payloadLength];
	memcpy(_payload, rhs._payload, rhs._payloadLength);
}


Packet::~Packet() 
{
	if (_payload != NULL)
		delete [] _payload;
	if (_extensionHeader != NULL) {
		delete [] _extensionHeader;
	}
}


IPacket* Packet::clone() const 
{
	return new Packet(*this);
}


void Packet::setExtHeader(const char* data, int length) 
{
	if (data == NULL || length == 0) {
		return;
	}

	_extensionHeader = new char[length];
	memcpy(_extensionHeader, data, length);

	_extensionLength = length;
	_header.extension = 1;
}


bool Packet::read(Buffer& buf) 
{
	//assert(buf.size() < RTP_MAX_PACKET_LEN);

	if (buf.size() == 0) {
		Log("error") << "RTP: Received empty packet." << endl;
		return false;
	}

	if (buf.size() < 12) {
		// Too small to contain an RTP header 
		Log("error") << "RTP: Packet too small to contain an RTP header." << endl;
		return false;
	}

	if (!_header.read(buf)) {
		Log("error") << "RTP: Unable to parse RTP header." << endl;
		return false;
	}
	
	int payloadLength = buf.remaining();

	// Check if packet contains an extension header. If yes
	//	* set pointer to extension header, compute buf.size() and
	//	* adjust payload pointer and payloadLength
	//
	if (_header.extension && payloadLength >= 4) {
		_extensionLength = 4;
		payloadLength -= 4;	// Minimum size of extension header
		
		short tmp = *((short *)(buf.data() + 2));
		tmp = NetworkToHost16(tmp);
		tmp *= 4;		// ext. header length is in words (4 bytes)
		_extensionLength += tmp;
		payloadLength -= tmp;

		if (payloadLength >= 0) {
			_extensionHeader = new char[_extensionLength];
			memcpy(_extensionHeader, buf.data(), _extensionLength);
		}
	}
	_payloadLength = payloadLength;

	if (_payloadLength > 0) {
		_payload = new char[_payloadLength];
		memcpy(_payload, buf.data() + _extensionLength, _payloadLength);
	}

	return true;
}


void Packet::write(Buffer& buf) const 
{
	assert(buf.capacity() >= size());

	/*
	char* hdr = _header.getBytes();
	int hdrSize = _header.size();
	buf.writeBytes(hdr, hdrSize);
	delete [] hdr;
	*/
	_header.write(buf);

	if (_extensionLength > 0) {
		buf.writeBytes(_extensionHeader, _extensionLength);
	}

	buf.writeBytes(_payload, _payloadLength);

	if (_zrtpChecksum) {
		UInt16 chkSum = computeChecksum((UInt16*)buf.bytes(), buf.size()-20);
		buf.writeUInt16(chkSum);
	}
}


size_t Packet::size() const
{
	return _header.size() + _payloadLength + _extensionLength + _zrtpChecksum;
}


void Packet::print(std::ostream& os) const 
{
	assert(_header.version == 2);

    os << "RTP Packet:" << endl;
    os << "  version = " << _header.version << endl;
    os << "  padding = " << _header.padding << endl;
    os << "  extension = " << _header.extension << endl;
    os << "  csrcCount = " << _header.csrcCount << endl;
    os << "  marker = " << _header.marker << endl;
    os << "  payloadType = " << _header.payloadType << endl;
    os << "  sequenceNumber = " << _header.sequenceNumber << endl;
    os << "  timestamp = " << _header.timestamp << endl;
    os << "  ssrc = " << _header.ssrc << endl;
    os << "  size = " << size() << endl;
}


bool Packet::checkZrtpChecksum(bool /* check */) 
{ 
	if (_payloadLength >= 2) {
		_payloadLength -= 2;
	}
	// TODO: Implement the real recompute and check of checksum
	return true; 
}


#define CKSUM_CARRY(x) (x = (x >> 16) + (x & 0xffff), (~(x + (x >> 16)) & 0xffff))
UInt16 Packet::computeChecksum(UInt16* data, int length) const
{
	UInt32 sum = 0;
	UInt16 ans = 0;

	while (length > 1) {
		sum += *data++;
		length -= 2;
	}
	if (length == 1) {
		//(UInt8 *)(&ans) = *(UInt8*)data;
		sum += ans;
	}

	UInt16 ret = CKSUM_CARRY(sum);

	// Return the inverted 16-bit result.
	return (ret);
}


} // namespace RTP
} // namespace Sourcey