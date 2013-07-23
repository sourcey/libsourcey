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


#ifndef SOURCEY_HTTP_ConnectionAdapter_H
#define SOURCEY_HTTP_ConnectionAdapter_H


#include "Sourcey/Signal.h"
#include "Sourcey/HTTP/Connection.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Format.h"
#include <sstream>


namespace scy { 
namespace http {
	
//#include "Sourcey/PacketStream.h"
//////#include "Poco/Net/Response.h"	
	//net::Socket& socket; //ConnectionAdapter(connection, type)socket(),  //socket(socket), //Socket& socket, 

// ---------------------------------------------------------------------
//
class ChunkedAdapter: public IPacketizer
{
public:
	std::string contentType;
	Connection& connection;

	ChunkedAdapter(Connection& connection) : 
		connection(connection), 
		contentType(connection.outgoingHeaders()->getContentType())
	{
		traceL("ChunkedAdapter", this) << "Creating" << std::endl;
	}
	
	virtual ~ChunkedAdapter() 
	{
		traceL("ChunkedAdapter", this) << "Destroying" << std::endl;
	}
	
	virtual void sendHeader() //std::ostringstream& ost
		// Sets HTTP headers for the initial response.
		// This method must not include the final carriage return. 
	{	
		connection.shouldSendHeaders(true);
				
		connection.response().set("Access-Control-Allow-Origin", "*");
		connection.response().set("Content-Type", "text/html");
		connection.response().set("Transfer-Encoding", "chunked");

		// Flushed on initial send ...

		/*
		std::ostringstream ost;
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
			<< "Content-Type: " << contentType << "\r\n";
		emit(this, RawPacket(ost.str().data(), ost.str().length()));
		*/
	}
	
	virtual void process(IPacket& packet)
	{
		traceL("ChunkedAdapter", this) << "Processing: " << packet.className() << std::endl;
		try {
			RawPacket& raw = dynamic_cast<RawPacket&>(packet);

			std::ostringstream ss;
			ss << std::hex << raw.size();
			emit(ss.str() + "\r\n");
			emit(raw);
			emit("\r\n", 2);
		}
		catch(const std::bad_cast&) {
			throw Exception("Incompatible packet type");
			// Cast failed
		}
		//RawPacket* raw = dynamic_cast<RawPacket*>(&packet);
		//if (raw)
		//	throw Exception("Incompatible packet type");
		
		// TODO: Better send API

		/*
		emit(this, RawPacket(ss.str().data(), ss.str().length()));
		emit(this, RawPacket("\r\n", 2));
		emit(this, RawPacket(raw->data(), raw->size()));
		emit(this, RawPacket("\r\n", 2)); // straight to Socket::send
		*/
	}
};


	/*
	ChunkedAdapter(const std::string& contentType) : 
		contentType(contentType)
	{
		traceL("ChunkedAdapter", this) << "Creating" << std::endl;
	}
	*/

	/*
	virtual void onSocketRecv(Buffer& buf, const net::Address& peerAddr)
	{
		assert(0 && "not implemented");
	}
	
	virtual bool accepts(RawPacket& packet) 
	{ 
	}
	*/
	
	/*

	virtual int send(char* data, int len) //const char* data, int len, int flags = 0
	{			
		traceL("ChunkedAdapter", this) << "Processing: " << len << std::endl;
		return len;
	}

	virtual void sendHeader(std::ostringstream& ost)
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
			<< "Content-Type: " << _connection << "\r\n";
	}
	*/

		
		/*		
	//virtual void process(IPacket& packet)

		// Encode and emit chunked data
		//int length = packet.size();
		//Buffer ibuf(len > 0 ? len : 1500);
		//packet.write(ibuf);
		//length = ibuf.available();

		obuf.put(ss.str());
		obuf.put("\r\n");
		obuf.put(data, len);
		obuf.put("\r\n");

		Buffer obuf;
		RawPacket opacket;
		opacket.read(obuf);
		socket->send(this, opacket);
		// Write the initial HTTP response header		
		if (_initial) {
			_initial = false;
			std::ostringstream header;
			sendHeader(header);
			header << "\r\n";
			std::string httpData(header.str());
			RawPacket httpHeader(httpData.data(), httpData.size());
			socket->send(this, httpHeader);
		}
		// TODO: optimize .. send packet parts as separate packets

		// Encode and emit chunked data
		int length = packet.size();
		Buffer ibuf(length > 0 ? length : 1500);
		packet.write(ibuf);
		length = ibuf.available();
		
		Buffer obuf;
		std::ostringstream ss;
		ss << std::hex << length;
		obuf.put(ss.str());
		obuf.put("\r\n");
		obuf.put(ibuf.data(), ibuf.available());
		obuf.put("\r\n");

		RawPacket opacket;
		opacket.read(obuf);
		socket->send(this, opacket);
		*/


// ---------------------------------------------------------------------
//
class MultipartAdapter: public IPacketizer
{
public:
	Connection& connection;
	std::string contentType;

	MultipartAdapter(Connection& connection, bool base64 = false) :		//const std::string& contentType, http_parser_type type, , bool dontFragment = true
		connection(connection),
		contentType(connection.outgoingHeaders()->getContentType()),
		//contentType(contentType),
		dontFragment(dontFragment),
		isBase64(base64)
	{
		traceL("MultipartAdapter", this) << "Creating" << std::endl;
	}
	
	virtual ~MultipartAdapter() 
	{
		traceL("MultipartAdapter", this) << "Destroying" << std::endl;
	}
		
	virtual void sendHeader()
	{	
		connection.shouldSendHeaders(true);
				
		connection.response().set("Content-Type", "text/html");
		connection.response().set("Transfer-Encoding", "chunked");
		connection.response().set("Access-Control-Allow-Origin", "*");

		// Flushed on initial send ...

		/*
		std::ostringstream ost;
		ost << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: multipart/x-mixed-replace; boundary=end\r\n"
			<< "Cache-Control: no-store, no-cache, max-age=0, must-revalidate\r\n"
			<< "Cache-Control: post-check=0, pre-check=0, FALSE\r\n"
			<< "Access-Control-Allow-Origin: *\r\n"
			<< "Pragma: no-cache\r\n"
			<< "Expires: 0\r\n";
		emit(this, RawPacket(ost.str().data(), ost.str().length()));
		*/
	}
	
	virtual void sendChunkHeaders(std::ostringstream& ost)
		// Sets HTTP headers for the current chunk.
		// This method must not include the final carriage return. 
	{	
		ost << "--end\r\n"
			<< "Content-Type: " << contentType << "\r\n";
		if (isBase64)
			ost << "Content-Transfer-Encoding: base64\r\n";			
	}

	virtual void process(IPacket& packet)
	{		
		/*
		std::ostringstream headers;

		// Write the chunk header
		sendChunkHeaders(headers);
		headers << "\r\n";
		
		if (!dontFragment) {

			// Broadcast the HTTP header separately 
			// so we don't need to copy any data.
			std::string httpData(headers.str());
			emit(this, RawPacket(httpData.data(), httpData.size()));

			// Proxy the input packet.
			emit(this, packet);
		}
		else {
			
			// Otherwise merge the headers and data packet.
			int contentLength = packet.size();
			Buffer ibuf(contentLength > 0 ? contentLength : 1500);
			packet.write(ibuf);
			
			Buffer obuf;
			obuf.put(headers.str());
			obuf.put(ibuf.data(), ibuf.available());

			RawPacket opacket;
			opacket.read(obuf);
			emit(this, opacket);
		}
		*/
	}

protected:
	bool dontFragment;
	bool isBase64;
};
		
	
	/*
	virtual void sendHeader(std::ostringstream& ost)
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
	*/
/*
// Write the initial HTTP response header		
if (_initial) {			
	_initial = false;	
	sendHeader(headers);
	headers << "\r\n";

	// Send the initial HTTP header.
	std::string httpData(headers.str());
	RawPacket httpHeader(httpData.data(), httpData.size());
	socket->send(this, httpHeader);
	headers.str("");
}
*/


/*
//bool _initial;
// ---------------------------------------------------------------------
//
class StreamingConnectionAdapter: public IPacketizer
	/// Implements HTTP Streaming for modern web browser clients 
	/// such as WebKit and Firefox (tested on 15.0.1).
{
public:
	StreamingConnectionAdapter(Connection& connection) :
		_connection(connection)//,
		//_initial(true)
	{
		traceL("StreamingConnectionAdapter", this) << "Creating" << std::endl;
	}
	
	virtual ~StreamingConnectionAdapter() 
	{
		traceL("StreamingConnectionAdapter", this) << "Destroying" << std::endl;
	}
	
	virtual void sendHeader(std::ostringstream& ost)
		// Sets HTTP headers for the initial response.
		// This method must not include the final carriage return. 
	{	
		ost << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: " << _connection << "\r\n"
			<< "Cache-Control: no-store, no-cache, max-age=0, must-revalidate\r\n"
			<< "Cache-Control: post-check=0, pre-check=0, FALSE\r\n"
			<< "Access-Control-Allow-Origin: *\r\n"
			//<< "Transfer-Encoding: chunked\r\n"
			<< "Pragma: no-cache\r\n"
			<< "Expires: 0\r\n";
	}

	virtual void process(IPacket& packet)
	{		
		//traceL("StreamingConnectionAdapter", this) << "Processing" << std::endl;
		
		// Write the initial HTTP response header		
		if (_initial) {
			_initial = false;
			std::ostringstream header;
			sendHeader(header);
			header << "\r\n";
			std::string httpData(header.str());
			RawPacket httpHeader(httpData.data(), httpData.size());
			socket->send(this, httpHeader);
		}

		socket->send(this, packet);
	}

protected:
	Connection& _connection;
	//bool _initial;
};
*/


} } // namespace scy::http


#endif