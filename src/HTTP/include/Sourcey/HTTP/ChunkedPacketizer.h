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


#ifndef SOURCEY_HTTP_ChunkedPacketizer_H
#define SOURCEY_HTTP_ChunkedPacketizer_H


#include "Sourcey/IPacketizer.h"
#include "Sourcey/Signal.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Format.h"
#include <sstream>


namespace Scy { 
namespace HTTP {


class ChunkedPacketizer: public IPacketizer
{
public:
	ChunkedPacketizer(const std::string& contentType) :
		_contentType(contentType), _initial(true)
	{
		LogTrace() << "[ChunkedPacketizer:" << this << "] Creating" << std::endl;
	}
	
	virtual ~ChunkedPacketizer() 
	{
		LogTrace() << "[ChunkedPacketizer:" << this << "] Destroying" << std::endl;
	}
	
	virtual void writeInitialHTTPHeaders(std::ostringstream& ost)
		// Sets HTTP headers for the initial response.
		// This method must not include the final carriage return. 
	{	
		ost << "HTTP/1.1 200 OK\r\n"
			// NOTE: If Cache-Control: no-store is not used Chrome's (27.0.1453.110) 
			// memory usage grows exponentially for HTTP streaming:
			// https://code.google.com/p/chromium/issues/detail?id=28035
			<< "Cache-Control: no-store, no-cache, max-age=0, must-revalidate\r\n"
			<< "Cache-Control: post-check=0, pre-check=0, FALSE\r\n"
			<< "Connection: keep-alive\r\n"
			<< "Pragma: no-cache\r\n"
			<< "Expires: 0\r\n"
			<< "Access-Control-Allow-Origin: *\r\n"
			<< "Transfer-Encoding: chunked\r\n"
			<< "Content-Type: " << _contentType << "\r\n";
	}

	virtual void process(IPacket& packet)
	{			
		// Write the initial HTTP response header		
		if (_initial) {
			_initial = false;
			std::ostringstream header;
			writeInitialHTTPHeaders(header);
			header << "\r\n";
			std::string httpData(header.str());
			DataPacket httpHeader((unsigned char*)httpData.data(), httpData.size());
			emit(this, httpHeader);
		}

		// Encode and emit chunked data
		int contentLength = packet.size();
		Buffer ibuf(contentLength > 0 ? contentLength : 1500);
		packet.write(ibuf);
		contentLength = ibuf.size();
		
		Buffer obuf;
		std::ostringstream ss;
		ss << std::hex << contentLength;
		obuf.writeString(ss.str()); // content length in hexidecimal
		obuf.writeString("\r\n");
		obuf.writeBytes(ibuf.data(), ibuf.size());
		obuf.writeString("\r\n");

		DataPacket opacket;
		opacket.read(obuf);
		emit(this, opacket);
	}

protected:
	std::string _contentType;
	bool _initial;
};


} } // namespace Scy::HTTP


#endif