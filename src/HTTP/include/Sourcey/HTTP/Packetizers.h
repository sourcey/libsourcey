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


namespace scy { 
namespace http {

	
// ---------------------------------------------------------------------
//
class ChunkedPacketizer: public IPacketizer
{
public:
	ChunkedPacketizer(const std::string& contentType) :
		_contentType(contentType), _initial(true)
	{
		traceL("ChunkedPacketizer", this) << "Creating" << std::endl;
	}
	
	virtual ~ChunkedPacketizer() 
	{
		traceL("ChunkedPacketizer", this) << "Destroying" << std::endl;
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
			RawPacket httpHeader(httpData.data(), httpData.size());
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
		obuf.write(ss.str()); // content length in hexidecimal
		obuf.write("\r\n");
		obuf.write(ibuf.data(), ibuf.size());
		obuf.write("\r\n");

		RawPacket opacket;
		opacket.read(obuf);
		emit(this, opacket);
	}

protected:
	std::string _contentType;
	bool _initial;
};


// ---------------------------------------------------------------------
//
class MultipartPacketizer: public IPacketizer
{
public:
	MultipartPacketizer(const std::string& contentType, bool dontFragment = true, bool base64 = false) :
		_contentType(contentType),
		_dontFragment(dontFragment),
		_base64(base64),
		_initial(true)
	{
		traceL("MultipartPacketizer", this) << "Creating" << std::endl;
	}
	
	virtual ~MultipartPacketizer() 
	{
		traceL("MultipartPacketizer", this) << "Destroying" << std::endl;
	}
	
	virtual void writeInitialHTTPHeaders(std::ostringstream& ost)
		// Sets HTTP headers for the initial response.
		// This method must not include the final carriage return. 
	{	
		ost << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: multipart/x-mixed-replace; boundary=end\r\n"
			<< "Cache-Control: no-store, no-cache, max-age=0, must-revalidate\r\n"
			<< "Cache-Control: post-check=0, pre-check=0, FALSE\r\n"
			<< "Access-Control-Allow-Origin: *\r\n"
			//<< "Transfer-Encoding: chunked\r\n"
			<< "Pragma: no-cache\r\n"
			<< "Expires: 0\r\n"
			;
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
		std::ostringstream headers;
		
		// Write the initial HTTP response header		
		if (_initial) {			
			_initial = false;	
			writeInitialHTTPHeaders(headers);
			headers << "\r\n";

			// Send the initial HTTP header.
			std::string httpData(headers.str());
			RawPacket httpHeader(httpData.data(), httpData.size());
			emit(this, httpHeader);
			headers.str("");
		}

		// Write the chunk header
		writeChunkHTTPHeaders(headers);
		headers << "\r\n";
		
		if (!_dontFragment) {

			// Broadcast the HTTP header separately 
			// so we don't need to copy any data.
			std::string httpData(headers.str());
			RawPacket httpHeader(httpData.data(), httpData.size());
			emit(this, httpHeader);

			// Proxy the input packet.
			emit(this, packet);
		}
		else {
			
			// Otherwise merge the headers and data packet.
			int contentLength = packet.size();
			Buffer ibuf(contentLength > 0 ? contentLength : 1500);
			packet.write(ibuf);
			
			Buffer obuf;
			obuf.write(headers.str());
			obuf.write(ibuf.data(), ibuf.size());

			RawPacket opacket;
			opacket.read(obuf);
			emit(this, opacket);
		}
	}

protected:
	std::string _contentType;
	bool _dontFragment;
	bool _base64;
	bool _initial;
};


// ---------------------------------------------------------------------
//
class StreamingPacketizer: public IPacketizer
	/// Implements HTTP Streaming for modern web browser clients 
	/// such as WebKit and Firefox (tested on 15.0.1).
{
public:
	StreamingPacketizer(const std::string& contentType) :
		_contentType(contentType),
		_initial(true)
	{
		traceL("StreamingPacketizer", this) << "Creating" << std::endl;
	}
	
	virtual ~StreamingPacketizer() 
	{
		traceL("StreamingPacketizer", this) << "Destroying" << std::endl;
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
		//traceL("StreamingPacketizer", this) << "Processing" << std::endl;
		
		// Write the initial HTTP response header		
		if (_initial) {
			_initial = false;
			std::ostringstream header;
			writeInitialHTTPHeaders(header);
			header << "\r\n";
			std::string httpData(header.str());
			RawPacket httpHeader(httpData.data(), httpData.size());
			emit(this, httpHeader);
		}

		emit(this, packet);
	}

protected:
	std::string _contentType;
	bool _initial;
};


} } // namespace scy::http


#endif