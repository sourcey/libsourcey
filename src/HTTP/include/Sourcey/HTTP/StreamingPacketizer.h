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


#ifndef SOURCEY_HTTP_StreamingPacketizer_H
#define SOURCEY_HTTP_StreamingPacketizer_H


#include "Sourcey/IPacketizer.h"
#include "Sourcey/Signal.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Format.h"
#include <sstream>


namespace Sourcey { 
namespace HTTP {


class StreamingPacketizer: public IPacketizer
	/// Implements HTTP Streaming for modern web browser clients 
	/// such as WebKit and Firefox (tested on 15.0.1).
{
public:
	StreamingPacketizer(const std::string& contentType) :
		_contentType(contentType),
		_initial(true)
	{
		LogTrace() << "[StreamingPacketizer:" << this << "] Creating" << std::endl;
	}
	
	virtual ~StreamingPacketizer() 
	{
		LogTrace() << "[StreamingPacketizer:" << this << "] Destroying" << std::endl;
	}
	
	virtual void writeInitialHTTPHeaders(std::ostringstream& ost)
		// Sets HTTP headers for the initial response.
		// This method must not include the final carriage return. 
	{	
		ost << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: " << _contentType << "\r\n"
			<< "Cache-Control: no-store, no-cache, max-age=0, must-revalidate\r\n"
			<< "Cache-Control: post-check=0, pre-check=0, FALSE\r\n"
			<< "Access-Control-Allow-Origin: *\r\n"
			//<< "Transfer-Encoding: chunked\r\n"
			<< "Pragma: no-cache\r\n"
			<< "Expires: 0\r\n";
	}

	virtual void process(IPacket& packet)
	{		
		//LogTrace() << "[StreamingPacketizer:" << this << "] Processing" << std::endl;
		
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

		emit(this, packet);

		//LogTrace() << "[StreamingPacketizer:" << this << "] Processing: OK" << std::endl;
	}

protected:
	std::string _contentType;
	bool _initial;
};


} } // namespace Sourcey::HTTP


#endif