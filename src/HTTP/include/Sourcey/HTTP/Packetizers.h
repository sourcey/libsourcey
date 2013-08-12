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


#ifndef SOURCEY_HTTP_Packetizers_H
#define SOURCEY_HTTP_Packetizers_H


#include "Sourcey/Signal.h"
#include "Sourcey/HTTP/Connection.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Format.h"
#include <sstream>


namespace scy { 
namespace http {


//
// HTTP Chunked Adapter
//


class ChunkedAdapter: public IPacketizer
{
public:
	std::string contentType;
	Connection* connection;
	bool initial;
	bool nocopy;

	ChunkedAdapter(Connection* connection = nil) : 
		PacketProcessor(Emitter),
		connection(connection), 
		contentType(connection->outgoingHeader()->getContentType()),
		initial(true)
	{
	}

	ChunkedAdapter(const std::string& contentType, bool nocopy = true) : 
		PacketProcessor(Emitter),
		connection(nil), 
		contentType(contentType),
		initial(true),
		nocopy(nocopy)
	{
	}
	
	virtual ~ChunkedAdapter() 
	{
	}
	
	virtual void emitHeader()
		/// Sets HTTP headers for the initial response.
		/// This method must not include the final carriage return. 
	{	
		/// Flush connection headers if the connection is set.
		if (connection) {
			connection->shouldSendHeader(true);					
			connection->response().setChunkedTransferEncoding(true);
			connection->response().set("Cache-Control", "no-store, no-cache, max-age=0, must-revalidate");
			connection->response().set("Cache-Control", "post-check=0, pre-check=0, FALSE");
			connection->response().set("Access-Control-Allow-Origin", "*");
			connection->response().set("Transfer-Encoding", "chunked");
			connection->response().set("Content-Type", contentType);
			connection->response().set("Connection", "keep-alive");
			connection->response().set("Pragma", "no-cache");
			connection->response().set("Expires", "0");
			connection->sendHeader();
		}

		/// Otherwise make up the response.
		else {
			std::ostringstream hst;
			hst << "HTTP/1.1 200 OK\r\n"
				/// NOTE: If Cache-Control: no-store is not used Chrome's (27.0.1453.110) 
				/// memory usage grows exponentially for HTTP streaming:
				/// https://code.google.com/p/chromium/issues/detail?id=28035
				<< "Cache-Control: no-store, no-cache, max-age=0, must-revalidate\r\n"
				<< "Cache-Control: post-check=0, pre-check=0, FALSE\r\n"
				<< "Access-Control-Allow-Origin: *\r\n"
				<< "Connection: keep-alive\r\n"
				<< "Pragma: no-cache\r\n"
				<< "Expires: 0\r\n"
				<< "Transfer-Encoding: chunked\r\n"
				<< "Content-Type: " << contentType << "\r\n"
				<< "\r\n";
			emit(hst.str());
		}
	}
	
	virtual void process(IPacket& packet)
	{
		/// traceL("ChunkedAdapter", this) << "Processing: " << packet.className() << ": " << packet.size() << std::endl;
		
		if (!packet.hasArray())
			throw ArgumentException("Incompatible packet type");
		
		/// Emit HTTP response header		
		if (initial) {			
			initial = false;	
			emitHeader();
		}
		
		/// Get hex stream length
		std::ostringstream ost;
		ost << std::hex << packet.size();
		
		/// Emit separate packets for nocopy
		if (nocopy) {
			emit(ost.str());
			emit("\r\n", 2);
			emit(packet.array(), packet.size());
			emit("\r\n", 2);
		}
		
		/// Concat pieces for non fragmented
		else {
			ost << "\r\n";
			ost.write(packet.array(), packet.size());
			ost << "\r\n";
			emit(ost.str());
		}
	}
		
	PacketSignal Emitter;
};


//
// HTTP Multipart Adapter
//


class MultipartAdapter: public IPacketizer
{
public:
	Connection* connection;
	std::string contentType;
	bool isBase64;
	bool initial;

	MultipartAdapter(Connection* connection, bool base64 = false) :	
		IPacketizer(Emitter),
		connection(connection),
		contentType(connection->outgoingHeader()->getContentType()),
		isBase64(base64),
		initial(true)
	{
	}

	MultipartAdapter(const std::string& contentType, bool base64 = false) :	
		IPacketizer(Emitter),
		connection(nil),
		contentType(contentType),
		isBase64(base64),
		initial(true)
	{
	}
	
	virtual ~MultipartAdapter() 
	{
	}
		
	virtual void emitHeader()
	{	
		/// Flush connection headers if the connection is set.
		if (connection) {
			connection->shouldSendHeader(true);				
			connection->response().set("Content-Type", "multipart/x-mixed-replace; boundary=end");
			connection->response().set("Cache-Control", "no-store, no-cache, max-age=0, must-revalidate");
			connection->response().set("Cache-Control", "post-check=0, pre-check=0, FALSE");
			connection->response().set("Access-Control-Allow-Origin", "*");
			connection->response().set("Transfer-Encoding", "chunked");
			connection->response().set("Connection", "keep-alive");
			connection->response().set("Pragma", "no-cache");
			connection->response().set("Expires", "0");
			connection->sendHeader();
		}

		/// Otherwise make up the response.
		else {
			std::ostringstream hst;
			hst << "HTTP/1.1 200 OK\r\n"
				<< "Content-Type: multipart/x-mixed-replace; boundary=end\r\n"
				<< "Cache-Control: no-store, no-cache, max-age=0, must-revalidate\r\n"
				<< "Cache-Control: post-check=0, pre-check=0, FALSE\r\n"
				<< "Access-Control-Allow-Origin: *\r\n"
				<< "Pragma: no-cache\r\n"
				<< "Expires: 0\r\n"
				<< "\r\n";
			emit(hst.str());
		}
	}
	
	virtual void emitChunkHeader()
		/// Sets HTTP header for the current chunk.
	{	
		/// Write the chunk header
		std::ostringstream hst;

		hst << "--end\r\n"
			<< "Content-Type: " << contentType << "\r\n";
		if (isBase64)
			hst << "Content-Transfer-Encoding: base64\r\n";	
		hst << "\r\n";	
		
		emit(hst.str());	
	}
	
	virtual void process(IPacket& packet)
	{		
		/// Write the initial HTTP response header		
		if (initial) {			
			initial = false;	
			emitHeader();
		}
		
		/// Broadcast the HTTP header separately 
		/// so we don't need to copy any data.
		emitChunkHeader();

		/// Proxy the input packet.
		emit(packet);
	}
			
	PacketSignal Emitter;
};


} } // namespace scy::http


#endif