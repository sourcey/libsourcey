//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_HTTP_MultipartPacketizer_H
#define SOURCEY_HTTP_MultipartPacketizer_H


#include "Sourcey/IPacketizer.h"
#include "Sourcey/Signal.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Format.h"
#include <sstream>


namespace Sourcey { 
namespace HTTP {


class MultipartPacketizer: public IPacketizer
{
public:
	MultipartPacketizer(const std::string& contentType, bool dontFragment = true, bool base64 = false) :
		_contentType(contentType),
		_dontFragment(dontFragment),
		_base64(base64),
		_initial(true)
	{
	}
	
	virtual void writeInitialHTTPHeaders(std::ostringstream& ost)
		// Sets HTTP headers for the initial response.
		// This method must not include the final carriage return. 
	{	
		ost << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: multipart/x-mixed-replace; boundary=end\r\n"
			<< "Cache-Control: no-store, no-cache, must-revalidate\r\n"
			<< "Cache-Control: post-check=0, pre-check=0, FALSE\r\n"
			<< "Access-Control-Allow-Origin: *\r\n"
			<< "Pragma: no-cache\r\n"
			<< "Expires: 0\r\n";
	}
	
	virtual void writeChunkHTTPHeaders(std::ostringstream& ost)
		// Sets HTTP headers for the current chunk.
		// This method must not include the final carriage return. 
	{	
		ost << "--end\r\n"
			<< "Content-Type: " << _contentType << "\r\n";
		if (_base64)
			ost << "Content-Transfer-Encoding: base64\r\n";			
	}

	virtual void process(IPacket& packet)
	{		
		std::ostringstream header;

		// Write the initial HTTP response header		
		if (_initial) {				
			writeInitialHTTPHeaders(header);
			header << "\r\n";
			_initial = false;
		}

		// Write the chunk header
		writeChunkHTTPHeaders(header);
		header << "\r\n";
		
		if (!_dontFragment) {

			// Broadcast the HTTP header as a seperate packet
			// so we don't need to copy packet data.
			std::string httpData(header.str());
			DataPacket httpHeader((unsigned char*)httpData.data(), httpData.size());
			dispatch(this, httpHeader);

			// Proxy the input packet.
			dispatch(this, packet);
		}
		else {
			
			int contentLength = packet.size();
			Buffer ibuf(contentLength > 0 ? contentLength : 1500);
			packet.write(ibuf);
			
			Buffer obuf;
			obuf.writeString(header.str());
			obuf.writeBytes(ibuf.data(), ibuf.size());

			DataPacket opacket;
			opacket.read(obuf);
			dispatch(this, opacket);
		}
	}

protected:
	std::string _contentType;
	bool _dontFragment;
	bool _base64;
	bool _initial;
};


} } // namespace Sourcey::HTTP


#endif