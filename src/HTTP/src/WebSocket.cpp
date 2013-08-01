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


#include "Sourcey/HTTP/WebSocket.h"
#include "Sourcey/HTTP/Client.h"
#include "Sourcey/HTTP/Server.h"
#include "Sourcey/Logger.h"

#include "Poco/Buffer.h"
#include "Poco/MemoryStream.h"
#include "Poco/NullStream.h"
#include "Poco/BinaryWriter.h"
#include "Poco/SHA1Engine.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Format.h"
#include "Poco/String.h"
#include "Poco/Random.h"
#include "Poco/StreamCopier.h"


using namespace std;
//using namespace scy::net;


namespace scy {
namespace http {


WebSocket::WebSocket() : 
	net::Socket(new net::TCPBase, false)
{
	setAdapter(new WebSocketAdapter(this, WebSocket::WS_CLIENT, _request, _response));
}


WebSocket::WebSocket(net::SocketBase* base, bool shared) : 
	net::Socket(base, shared)
{
	setAdapter(new WebSocketAdapter(this, WebSocket::WS_CLIENT, _request, _response));
}


WebSocket::WebSocket(const Socket& socket) : 
	net::Socket(socket)
{
	if (!dynamic_cast<WebSocketAdapter*>(_adapter))
		throw Exception("Cannot assign incompatible socket adapter");
}

	
bool WebSocket::shutdown(UInt16 statusCode, const string& statusMessage)
{
	return adapter().shutdown(statusCode, statusMessage);
}


WebSocketAdapter& WebSocket::adapter() const
{
	assert(Socket::adapter());
	return *static_cast<WebSocketAdapter*>(Socket::adapter());
}


http::Request& WebSocket::request()
{
	return _request;
}


http::Response& WebSocket::response()
{
	return _response;
}


const string WebSocket::WEBSOCKET_GUID("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
const string WebSocket::WEBSOCKET_VERSION("13");


// -------------------------------------------------------------------
//
WebSocketAdapter::WebSocketAdapter(net::Socket* socket, WebSocket::Mode mode, http::Request& request, http::Response& response) : 
	net::SocketAdapter(socket), //, 100
	framer(mode), _request(request), _response(response)
{
	traceL("WebSocketAdapter", this) << "Creating" << endl;
}

	
WebSocketAdapter::WebSocketAdapter(WebSocket::Mode mode, http::Request& request, http::Response& response) : 
	net::SocketAdapter(nil), //, 100
	framer(mode), _request(request), _response(response)
{
	traceL("WebSocketAdapter", this) << "Creating" << endl;
}

	
WebSocketAdapter::~WebSocketAdapter() 
{	
	traceL("WebSocketAdapter", this) << "Destroying" << endl;
}

	
bool WebSocketAdapter::shutdown(UInt16 statusCode, const string& statusMessage)
{
	Buffer buffer;
	buffer.putU16(statusCode);
	buffer.put(statusMessage);
	return socket->base().send(buffer.begin(), buffer.available(), WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_CLOSE) > 0;
}


int WebSocketAdapter::send(const char* data, int len, int flags) 
{	
	traceL("WebSocketAdapter", this) << "WS Send: " << len << endl;
	return send(data, len, socket->peerAddress(), flags);
}


int WebSocketAdapter::send(const char* data, int len, const net::Address& peerAddr, int flags) 
{	
	traceL("WebSocketAdapter", this) << "Send: " << len << endl;
	assert(framer.handshakeComplete());

	// Set default text flag if none specified
	if (!flags)
		flags = WebSocket::FRAME_TEXT;

	// Frame and send the data
	Buffer frame(len + WebSocketFramer::MAX_HEADER_LENGTH);
	framer.writeFrame(data, len, flags, frame);
	return socket->base().send(frame.begin(), frame.available(), peerAddr, 0);
}


//
// Client side
//
	
void WebSocketAdapter::sendClientRequest()
{
	traceL("WebSocketClientAdapter", this) << "Send client request: " << _request << endl;

	//http::Request request;
	framer.createHandshakeRequest(_request);
	//PrepareClientRequest.emit(this, request);
	//prepareClientRequest(_request);

	ostringstream oss;
	_request.write(oss);
	traceL("WebSocketClientAdapter", this) << "Send client request: After: " << string(oss.str().c_str(), oss.str().length()) << endl;
	socket->base().send(oss.str().c_str(), oss.str().length());
}


void WebSocketAdapter::handleClientResponse(Buffer& buffer)
{
	//http::Response response;
	traceL("WebSocketClientAdapter", this) << "Handle client response: " << buffer << endl;
	http::Parser parser(&_response);
	parser.parse(buffer.data(), buffer.available(), true);
	
	// TODO: Handle resending request for authentication
	//VerifyClientResponse.emit(this, response);
	//verifyClientResponse(_response);
	traceL("WebSocketClientAdapter", this) << "Handle client response: Parsed: " << _response << endl;

	// Parse and check the response
	if (framer.checkHandshakeResponse(_response)) {				
		traceL("WebSocketClientAdapter", this) << "Handshake success" << endl;
		SocketAdapter::onSocketConnect();
	}			
}


/*
void WebSocketAdapter::prepareClientRequest(http::Request& request)
{	
	// Copy the WS request if available
	WebSocket* ws = dynamic_cast<WebSocket*>(this->socket);
	if (ws) {
		request = ws->request;
	}
}


void WebSocketAdapter::verifyClientResponse(http::Response& response)
{	
	// None by default
}
*/


//
// Server side
//
	
void WebSocketAdapter::handleServerRequest(Buffer& buffer)
{
	//http::Request request;
	http::Parser parser(&_request);
	parser.parse(buffer.data(), buffer.available(), true);
	
	traceL("WebSocketServerAdapter", this) << "Verifying handshake: " << _request << endl;

	// Allow the application to verify the incoming request.
	// TODO: Handle authentication
	//VerifyServerRequest.emit(this, request);
	
	// Verify the WebSocket handshake request
	//http::Response response;
	try {
		framer.acceptRequest(_request, _response);
		traceL("WebSocketServerAdapter", this) << "Handshake success" << endl;
	}
	catch(Exception& exc) {
		warnL("WebSocketClientAdapter", this) << "Handshake failed: " << exc.message() << endl;		
	}

	// Allow the application to override the response
	//PrepareServerResponse.emit(this, response);
				
	// Send response
	ostringstream oss;
	_response.write(oss);
	socket->base().send(oss.str().c_str(), oss.str().length());
}


//
// SocketAdapter events
//

void WebSocketAdapter::onSocketConnect()
{
	traceL("WebSocketAdapter", this) << "On connect" << endl;
	
	// Send the WS handshake request
	sendClientRequest();
}


void WebSocketAdapter::onSocketRecv(Buffer& buffer, const net::Address& peerAddr)
{
	traceL("WebSocketAdapter", this) << "On recv: " << buffer.available() << ": " << buffer << endl;

	assert(buffer.position() == 0);
	assert(buffer.available() > 0);

	if (framer.handshakeComplete()) {

		// NOTE: The spec wants us to buffer partial frames, but our
		// software does not require this feature, and furthermore
		// it goes against our nocopy where possible policy. 
		// This may need to change in the future, but for now
		// we just parse and emit packets as they arrive.
		//
		// Incoming frames may be joined, so we parse them
		// in a loop until the read buffer is empty.
		int offset = buffer.position();
		int total = buffer.available();
		while (offset < total) {
			try {
				// Restore buffer state for next read
				buffer.position(offset);
				buffer.limit(total);
					
				/*
				traceL("WebSocketServerAdapter", this) << "Read frame at: " 
					 << "\n\tinputPosition: " << offset
					 << "\n\tinputLength: " << total
					 << "\n\tbufferPosition: " << buffer.position() 
					 << "\n\tbufferAvailable: " << buffer.available() 
					 << "\n\tbufferLimit: " << buffer.limit() 
					 << endl;	
					 */
				
				// Parse a frame to throw
				int payloadLength = framer.readFrame(buffer);

				// Update the next frame offset
				offset = buffer.position() + payloadLength; 

				if (offset < total)
					debugL("WebSocketAdapter", this) << "Splitting joined packet at "
						<< offset << " of " << total << endl;

				// Drop empty packets
				if (!payloadLength) continue;
			} 
			catch(Exception& exc) {
				warnL("WebSocketAdapter", this) << "WebSocket read error: " << exc.message() << endl;		
				socket->setError(exc.message());	
				return;
			}
			
			// Emit the result packet
			traceL("WebSocketAdapter", this) << "On recv: Emitting: " << buffer << endl;
			SocketAdapter::onSocketRecv(buffer, peerAddr);
		}
		assert(offset == total);
	}
	else {		
		try {
			if (framer.mode() == WebSocket::WS_CLIENT)
				handleClientResponse(buffer);
			else
				handleServerRequest(buffer);
		} 
		catch(Exception& exc) {
			warnL("WebSocketAdapter", this) << "WebSocket read error: " << exc.message() << endl;		
			socket->setError(exc.message());	
		}
		traceL("WebSocketAdapter", this) << "After handshaking: " << buffer << endl;
		return;
	}
	
}


// -------------------------------------------------------------------
//
WebSocketConnectionAdapter::WebSocketConnectionAdapter(Connection& connection, WebSocket::Mode mode) : 
	WebSocketAdapter(&connection.socket(), mode, connection.request(), connection.response()), 
	_connection(connection)
{
	traceL("WebSocketConnectionAdapter", this) << "Creating" << endl;
}

	
WebSocketConnectionAdapter::~WebSocketConnectionAdapter() 
{	
	traceL("WebSocketConnectionAdapter", this) << "Destroying" << endl;
}


// ---------------------------------------------------------------------
//
WebSocketFramer::WebSocketFramer(WebSocket::Mode mode) : //bool mustMaskPayload
	_mode(mode),
	_frameFlags(0),
	_headerState(0),
	_mustMaskPayload(mode == WebSocket::WS_CLIENT)
{
}


WebSocketFramer::~WebSocketFramer()
{
}


string createKey()
{
	Poco::Random rnd;
	ostringstream ostr;
	Poco::Base64Encoder base64(ostr);
	Poco::BinaryWriter writer(base64);
	writer << rnd.next() << rnd.next() << rnd.next() << rnd.next();
	base64.close();
	return ostr.str();
}


string computeAccept(const string& key)
{
	string accept(key);
	accept += WebSocket::WEBSOCKET_GUID;
	Poco::SHA1Engine sha1;
	sha1.update(accept);
	Poco::DigestEngine::Digest d = sha1.digest();
	ostringstream ostr;
	Poco::Base64Encoder base64(ostr);
	base64.write(reinterpret_cast<const char*>(&d[0]), d.size());
	base64.close();
	return ostr.str();
}


void WebSocketFramer::createHandshakeRequest(http::Request& request)
{
	assert(_mode == WebSocket::WS_CLIENT);
	assert(_headerState == 0);

	// Send the handshake request
	_key = createKey();
	request.set("Connection", "Upgrade");
	request.set("Upgrade", "websocket");
	request.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
	request.set("Sec-WebSocket-Key", _key);
	request.setChunkedTransferEncoding(false);
	_headerState++;
}


bool WebSocketFramer::checkHandshakeResponse(http::Response& response)
{	
	assert(_mode == WebSocket::WS_CLIENT);
	assert(_headerState == 1);
	if (response.getStatus() == http::Response::HTTP_SWITCHING_PROTOCOLS)
	{
		// Complete handshake or throw
		completeHandshake(response);
		
		// Success
		_headerState++;
		assert(handshakeComplete());
		return true;
	}
	else if (response.getStatus() == http::Response::HTTP_UNAUTHORIZED)
	{
		assert(0 && "authentication not implemented");
	}
	else
		throw Exception("WebSocket error: Cannot upgrade to WebSocket connection", response.getReason(), WebSocket::WS_ERR_NO_HANDSHAKE);

	// Need to resend request
	return false;
}


void WebSocketFramer::acceptRequest(http::Request& request, http::Response& response)
{
	if (util::icompare(request.get("Connection", ""), "upgrade") == 0 && 
		util::icompare(request.get("Upgrade", ""), "websocket") == 0)
	{
		string version = request.get("Sec-WebSocket-Version", "");
		if (version.empty()) throw Exception("WebSocket error: Missing Sec-WebSocket-Version in handshake request", WebSocket::WS_ERR_HANDSHAKE_NO_VERSION);
		if (version != WebSocket::WEBSOCKET_VERSION) throw Exception("WebSocket error: Unsupported WebSocket version requested", version, WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION);
		string key = request.get("Sec-WebSocket-Key", "");
		Poco::trimInPlace(key);
		if (key.empty()) throw Exception("WebSocket error: Missing Sec-WebSocket-Key in handshake request", WebSocket::WS_ERR_HANDSHAKE_NO_KEY);
		
		response.setStatusAndReason(http::Response::HTTP_SWITCHING_PROTOCOLS);
		response.set("Upgrade", "websocket");
		response.set("Connection", "Upgrade");
		response.set("Sec-WebSocket-Accept", computeAccept(key));

		// Set headerState 2 since the handshake was accepted.
		_headerState = 2;
	}
	else throw Exception("WebSocket error: No WebSocket handshake", WebSocket::WS_ERR_NO_HANDSHAKE);
}

	
int WebSocketFramer::writeFrame(const char* data, int len, int flags, Buffer& frame)
{
	assert(flags == WebSocket::FRAME_TEXT || 
		flags == WebSocket::FRAME_BINARY);

	assert(frame.capacity() >= len + MAX_HEADER_LENGTH);
	
	frame.putU8(static_cast<UInt8>(flags));
	UInt8 lenByte(0);
	if (_mustMaskPayload) {
		lenByte |= FRAME_FLAG_MASK;
	}
	if (len < 126) {
		lenByte |= static_cast<UInt8>(len);
		frame.putU8(lenByte);
	}
	else if (len < 65536) {
		lenByte |= 126;
		frame.putU8(lenByte);
		frame.putU16(static_cast<UInt16>(len));
	}
	else {
		lenByte |= 127;
		frame.putU8(lenByte);
		frame.putU64(static_cast<UInt16>(len));
	}

	if (_mustMaskPayload) {
		const UInt32 mask = _rnd.next();
		const char* m = reinterpret_cast<const char*>(&mask);
		const char* b = reinterpret_cast<const char*>(data);
		frame.put(m, 4);
		char* p = frame.begin() + frame.available();
		for (int i = 0; i < len; i++)
		{
			p[i] = b[i] ^ m[i % 4];
		}
	}
	else
	{
		memcpy(frame.begin() + frame.available(), data, len);
	}

	// Set the frame length
	frame.limit(len + frame.available());

	return frame.available();
}

	
int WebSocketFramer::readFrame(Buffer& buffer)
{
	assert(handshakeComplete());
	size_t offset = buffer.position();
	assert(buffer.capacity() > MAX_HEADER_LENGTH);
	int length = buffer.limit();
	
	// TODO: Support Buffer::get(Buffer&) so we don't need to set limit here
	Buffer header(MAX_HEADER_LENGTH);
	header.limit(MAX_HEADER_LENGTH);
	
	// Read the frame header
	buffer.get(header.data(), 2);
	UInt8 lengthByte = static_cast<UInt8>(header.data()[1]);
	int maskOffset = 0;
	if (lengthByte & FRAME_FLAG_MASK) maskOffset += 4;
	lengthByte &= 0x7f;
	if (lengthByte + 2 + maskOffset < MAX_HEADER_LENGTH) {
		buffer.get(header.data() + 2, lengthByte + maskOffset);
	}
	else {
		buffer.get(header.data() + 2, MAX_HEADER_LENGTH - 2);
	}

	// Reserved fields
	buffer.consume(2);	

	// Parse frame header
	UInt8 flags;
	char mask[4];
	assert(header.getU8(flags));	
	assert(header.getU8(lengthByte));
	_frameFlags = flags;
	int payloadLength = 0;
	int payloadOffset = 2;
	if ((lengthByte & 0x7f) == 127) {
		UInt64 l;
		header.getU64(l);
		if (l > length)
			throw Exception(Poco::format("WebSocket error: Insufficient buffer for payload size %Lu", l), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
		payloadOffset += 8;
	}
	else if ((lengthByte & 0x7f) == 126) {
		UInt16 l;
		header.getU16(l);
		if (l > length)
			throw Exception(Poco::format("WebSocket error: Insufficient buffer for payload size %hu", l), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
		payloadOffset += 2;
	}
	else {
		UInt8 l = lengthByte & 0x7f;
		if (l > length)
			throw Exception(Poco::format("WebSocket error: Insufficient buffer for payload size %u", unsigned(l)), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
	}
	if (lengthByte & FRAME_FLAG_MASK) {	
		header.get(mask, 4);
		payloadOffset += 4;
	}

	if (payloadLength > length)
		throw Exception("WebSocket error: Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);		

	// Unmask the payload if required
	if (lengthByte & FRAME_FLAG_MASK) {
		char* p = reinterpret_cast<char*>(buffer.data());
		for (int i = 0; i < payloadLength; i++) {
			p[i] ^= mask[i % 4];
		}
	}

	// Set the buffer position and length to match payload
	buffer.position(offset + payloadOffset);
	buffer.limit(offset + payloadOffset + payloadLength);
	
	/*
	traceL("WebSocketServerAdapter", this) << "Read frame at: " 
		 << "\n\tinputPosition: " << offset
		 << "\n\tinputLength: " << length
		 << "\n\tpayloadOffset: " << payloadOffset
		 << "\n\tpayloadLength: " << payloadLength
		 << "\n\tframePosition: " << header.position() 
		 << "\n\tframeLimit: " << header.limit() 
		 << "\n\tframeAvailable: " << header.available() 
		 << "\n\tbufferPosition: " << buffer.position() 
		 << "\n\tbufferLimit: " << buffer.limit() 
		 << "\n\tbufferAvailable: " << buffer.available() 
		 << endl;	
		 */

	return payloadLength;
}


void WebSocketFramer::completeHandshake(http::Response& response)
{
	string connection = response.get("Connection", "");
	if (util::icompare(connection, "Upgrade") != 0) 
		throw Exception("WebSocket error: No Connection: Upgrade header in handshake response", WebSocket::WS_ERR_NO_HANDSHAKE);
	string upgrade = response.get("Upgrade", "");
	if (util::icompare(upgrade, "websocket") != 0)
		throw Exception("WebSocket error: No Upgrade: websocket header in handshake response", WebSocket::WS_ERR_NO_HANDSHAKE);
	string accept = response.get("Sec-WebSocket-Accept", "");
	if (accept != computeAccept(_key))
		throw Exception("WebSocket error: Invalid or missing Sec-WebSocket-Accept header in handshake response", WebSocket::WS_ERR_NO_HANDSHAKE);
}


WebSocket::Mode WebSocketFramer::mode() const
{
	return _mode;
}


bool WebSocketFramer::handshakeComplete() const
{
	return _headerState == 2;
}


int WebSocketFramer::frameFlags() const
{
	return _frameFlags;
}


bool WebSocketFramer::mustMaskPayload() const
{
	return _mustMaskPayload;
}


} } // namespace scy::http






/*
//
// Callbacks
// 

void WebSocketConnectionAdapter::onSocketConnect()
{
	traceL("WebSocketClientAdapter", this) << "On connect" << endl;
	
	// Send the WS handshake request
	sendClientRequest(_connection.request());
}


void WebSocketConnectionAdapter::onSocketRecv(Buffer& buffer, const net::Address& peerAddr)
{
	traceL("WebSocketClientAdapter", this) << "On recv: " << buffer << endl;

	try {
		assert(buffer.position() == 0);
		assert(buffer.available() > 0);

		if (framer.handshakeComplete()) {
			framer.readFrame(buffer);
			SocketAdapter::onSocketRecv(buffer, peerAddr);
		}

		else {
			http::Response response;
			http::Parser parser(&response);
			parser.parse(buffer.data(), buffer.available(), true);

			// Parse and check the response
			if (framer.checkHandshakeResponse(response)) {				
				traceL("WebSocketClientAdapter", this) << "Handshake success" << endl;
				SocketAdapter::onSocketConnect();
			}
			
			// TODO: Resend request for authentication
			else
				assert(0 && "not implemented");
		}
	} 
	catch(Exception& exc) {
		warnL("WebSocketClientAdapter", this) << "Handshake failed: " << exc.message() << endl;		
		socket->setError(exc.message());	
	}
}
*/


/*
// -------------------------------------------------------------------
//
WebSocketClientAdapter::WebSocketClientAdapter(ClientConnection& connection) : 
	WebSocketAdapter(&connection.socket(), WebSocket::WS_CLIENT), 
	_connection(connection)
{
	traceL("WebSocketClientAdapter", this) << "Creating" << endl;
}

	
WebSocketClientAdapter::~WebSocketClientAdapter() 
{	
	traceL("WebSocketClientAdapter", this) << "Destroying" << endl;
}

	
void WebSocketClientAdapter::sendClientRequest(http::Request& request)
{
	framer.createHandshakeRequest(request);

	ostringstream oss;
	request.write(oss);
	socket->base().send(oss.str().c_str(), oss.str().length());
}


//
// Callbacks
// 

void WebSocketClientAdapter::onSocketConnect()
{
	traceL("WebSocketClientAdapter", this) << "On connect" << endl;
	
	// Send the WS handshake request
	sendClientRequest(_connection.request());
}


void WebSocketClientAdapter::onSocketRecv(Buffer& buffer, const net::Address& peerAddr)
{
	traceL("WebSocketClientAdapter", this) << "On recv: " << buffer << endl;

	try {
		assert(buffer.position() == 0);
		assert(buffer.available() > 0);

		if (framer.handshakeComplete()) {
			framer.readFrame(buffer);
			SocketAdapter::onSocketRecv(buffer, peerAddr);
		}

		else {
			http::Response response;
			http::Parser parser(&response);
			parser.parse(buffer.data(), buffer.available(), true);

			// Parse and check the response
			if (framer.checkHandshakeResponse(response)) {				
				traceL("WebSocketClientAdapter", this) << "Handshake success" << endl;
				SocketAdapter::onSocketConnect();
			}
			
			// TODO: Resend request for authentication
			else
				assert(0 && "not implemented");
		}
	} 
	catch(Exception& exc) {
		warnL("WebSocketClientAdapter", this) << "Handshake failed: " << exc.message() << endl;		
		socket->setError(exc.message());	
	}
}


// -------------------------------------------------------------------
//
WebSocketServerAdapter::WebSocketServerAdapter(ServerConnection& connection) : 
	WebSocketAdapter(socket, WebSocket::WS_SERVER), 
	_connection(connection)
{
	traceL("WebSocketServerAdapter", this) << "Creating" << endl;
}

	
WebSocketServerAdapter::~WebSocketServerAdapter() 
{	
	traceL("WebSocketServerAdapter", this) << "Destroying" << endl;
}


void WebSocketServerAdapter::onSocketRecv(Buffer& buffer, const net::Address& peerAddr)
{
	traceL("WebSocketServerAdapter", this) << "On recv: " << buffer << endl; //.size()
	
	assert(buffer.position() == 0);
	assert(buffer.available() > 0);

	if (framer.handshakeComplete()) {
		framer.readFrame(buffer);
		traceL("WebSocketServerAdapter", this) << "On frame: " << buffer.available() << endl;	
		SocketAdapter::onSocketRecv(buffer, peerAddr);
	}

	else {
		try {
			http::Request request;
			http::Parser parser(&request);
			parser.parse(buffer.data(), buffer.available(), true);

			errorL("WebSocketServerAdapter", this) << "Verifying handshake: " << request << endl;

			// TODO: authentication

			// Verify the WebSocket handshake request or throw
			//http::Response response;
			//framer.acceptRequest(request, response);

			framer.acceptRequest(request, _connection.response());

			errorL("WebSocketServerAdapter", this) << "Handshake success" << endl;
				
			// Send response
			ostringstream oss;
			_connection.response().write(oss);
			socket->base().send(oss.str().c_str(), oss.str().length());
		} 
		catch(Exception& exc) {
			errorL("WebSocketServerAdapter", this) << "Handshake failed: " << exc.message() << endl;
			socket->setError(exc.message());
		}
	}
}
*/



/*
WebSocketClientAdapter::WebSocketClientAdapter(ClientConnection& connection) : //net::Socket* socket, http::Request* request
	WebSocketAdapter(&connection.socket(), WebSocket::WS_CLIENT), 
	_connection(connection) //, request(request)
{
	traceL("WebSocketClientAdapter", this) << "Creating" << endl;
}
*/


/*
void WebSocketAdapter::setRequest(http::Request* request)
{
	_request = request;
}
*/


/*
void WebSocketAdapter::setResponse(http::Response* response)
{
	_response = response;
}
*/
	/*
	if (this->request)
	else {
		http::Request request;
		sendClientRequest(request);
	}

		request.setURI("/websocket");
	http::Request request;
	request
	framer.createHandshakeRequest(request);

	ostringstream oss;
	request.write(oss);
	socket->base().send(oss.str().c_str(), oss.str().length());
	*/

/*
	//int received = payloadOffset + payloadLength;		
	//if (received > length)
	//	throw Exception("WebSocket error: Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);	
Buffer& WebSocketAdapter::incomingBuffer()
{
	return static_cast<net::TCPBase&>(_socket->base()).buffer();
}


void WebSocketAdapter::onSocketRecv(Buffer& buf)
{
	Log("WebSocketAdapter", this) << "On WS Recv: " << buf << endl;
	emitRecv(buf, peerAddress());
}
*/

/*
if (status) {
	setLastError();
	errorL("WebSocketAdapter", this) << "Connect Failed: " << error().message << endl;
	return;
}
else
	readStart(); 
*/

/*


void WebSocketFramer::sendHandshakeRequest()
{
}

void WebSocketFramer::createHandshakeRequest(Buffer& request)
{

}
*/

	// Send the frame	
	//return _socketBase->write(frame.begin(), len + static_cast<int>(ostr.charsWritten()));

//char* buffer, int length char* buffer, int length, int

/*
int WebSocket::send(const char* data, int len, int flags) 
{	
	traceL("WebSocketAdapter", this) << "WS Send: " << len << endl;
	return send(data, len, socket->peerAddress(), flags);
}


int WebSocket::send(const char* data, int len, const net::Address& peerAddr, int flags) 
{	
	// Frame and send the data
	Buffer frame(len + WebSocketFramer::MAX_HEADER_LENGTH);
	framer.writeFrame(data, len, flags, frame);
	return socket->send(frame.begin(), frame.size(), peerAddr, 0);
}
*/

	

/*
WebSocketAdapter& WebSocket::base() const
{
	return static_cast<WebSocketAdapter&>(*_base);
}

//uv::TCPBase::shutdown();	
Poco::Buffer<char> buffer(statusMessage.size() + 2);
Poco::MemoryOutputStream ostr(buffer.begin(), buffer.available());
Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);
writer << statusCode;
writer.writeRaw(statusMessage);
return send(buffer.begin(), static_cast<int>(ostr.charsWritten()), WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_CLOSE) > 0;

// client socket = mustMaskPayload this, 
	//assert(_handle);
	//_handle->data = this;
	//connectReq.data = this;
*/
	

/*
WebSocket::Mode WebSocketFramer::mode() const
{
	return mustMaskPayload() ? WebSocket::WS_CLIENT : WebSocket::WS_SERVER;
}
*/

	/*
int WebSocketFramer::readNBytes(char* buffer, int bytes)
{
	//traceL("WebSocketFramer", this) << "readNBytes: " << bytes << ": " << buffer.available() << endl;

	//memcpy(buffer, header + payloadOffset, n - payloadOffset);
	//_incomingBuffer.available()
	//_incomingBuffer.get((char*)buffer, bytes);
	
	return 0;
	if (bytes > buffer.available())
		throw Exception("WebSocket error: Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);

	buffer.get((char*)buffer, bytes);
	return bytes;

	int received = _socketBase->readFrame(reinterpret_cast<char*>(buffer), bytes);
	while (received < bytes)
	{
		int n = _socketBase->readFrame(reinterpret_cast<char*>(buffer) + received, bytes - received);
		if (n > 0)
			received += n;
		else
			throw Exception("WebSocket error: Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);
	}
	return received;
}
	*/
	
	/*
	if (payloadOffset < n)
	{
		memcpy(buffer, header + payloadOffset, n - payloadOffset);
		received = n - payloadOffset;
	}
	if (received < payloadLength)
	{
		received += readNBytes(reinterpret_cast<char*>(buffer) + received, payloadLength - received);
		traceL("WebSocketFramer", this) << "RECV: 1111 received: " << received << endl;
		received += (payloadLength - received);
		traceL("WebSocketFramer", this) << "RECV: 222 received: " << received << endl;
	}
	assert(payloadOffset < n);

	//received += payloadLength;	
	//buffer.consume(payloadLength);
	//int received = = readNBytes(reinterpret_cast<char*>(buffer), payloadLength);
	//buffer.consume();
	
	
	traceL("WebSocketFramer", this) << "RECV: buffer.position: " << buffer.position() << endl;
	traceL("WebSocketFramer", this) << "RECV: buffer.size: " << buffer.available() << endl;
	traceL("WebSocketFramer", this) << "RECV: received: " << received << endl;
	traceL("WebSocketFramer", this) << "RECV: payloadLength: " << payloadLength << endl;
	traceL("WebSocketFramer", this) << "RECV: payloadOffset: " << payloadOffset << endl;
	
	*/

	/*
	//cs.sendClientRequest(request);
	SSL* ssl = SSL_new(_context->sslContext());

	// TODO: Automatic WS session handling.
	// Maybe add a stored session to the network manager.
	if (_session)
		SSL_set_session(ssl, _session->sslSession());
 
	SSL_set_connect_state(ssl);
	SSL_do_handshake(ssl);
 
	framer.init(ssl);
	framer.update();

	emitConnect();
	*/


/*
SocketImpl* WebSocketFramer::acceptConnection(SocketAddress& clientAddr)
{
	throw Poco::InvalidAccessException("Cannot acceptConnection() on a WebSocketFramer");
}


void WebSocketFramer::connect(const SocketAddress& address)
{
	throw Poco::InvalidAccessException("Cannot connect() a WebSocketFramer");
}


void WebSocketFramer::connect(const SocketAddress& address, const Timespan& timeout)
{
	throw Poco::InvalidAccessException("Cannot connect() a WebSocketFramer");
}


void WebSocketFramer::connectNB(const SocketAddress& address)
{
	throw Poco::InvalidAccessException("Cannot connectNB() a WebSocketFramer");
}


void WebSocketFramer::bind(const SocketAddress& address, bool reuseAddress)
{
	throw Poco::InvalidAccessException("Cannot bind() a WebSocketFramer");
}


void WebSocketFramer::bind6(const SocketAddress& address, bool reuseAddress, bool ipV6Only)
{
	throw Poco::InvalidAccessException("Cannot bind6() a WebSocketFramer");
}


void WebSocketFramer::listen(int backlog)
{
	throw Poco::InvalidAccessException("Cannot listen() on a WebSocketFramer");
}


void WebSocketFramer::close()
{
	_socketBase->close();
	reset();
}


void WebSocketFramer::shutdownReceive()
{
	_socketBase->shutdownReceive();
}


void WebSocketFramer::shutdownSend()
{
	_socketBase->shutdownSend();
}

	
void WebSocketFramer::shutdown()
{
	_socketBase->shutdown();
}


int WebSocketFramer::sendTo(const char* buffer, int length, const SocketAddress& address, int flags)
{
	throw Poco::InvalidAccessException("Cannot sendTo() on a WebSocketFramer");
}


int WebSocketFramer::receiveFrom(char* buffer, int length, SocketAddress& address, int flags)
{
	throw Poco::InvalidAccessException("Cannot receiveFrom() on a WebSocketFramer");
}


void WebSocketFramer::sendUrgent(unsigned char data)
{
	throw Poco::InvalidAccessException("Cannot sendUrgent() on a WebSocketFramer");
}


bool WebSocketFramer::secure() const
{
	return _socketBase->secure();
}


void WebSocketFramer::setSendTimeout(const Timespan& timeout)
{
	_socketBase->setSendTimeout(timeout);
}


Timespan WebSocketFramer::getSendTimeout()
{
	return _socketBase->getSendTimeout();
}


void WebSocketFramer::setReceiveTimeout(const Timespan& timeout)
{
	_socketBase->setReceiveTimeout(timeout);
}


Timespan WebSocketFramer::getReceiveTimeout()
{
	return _socketBase->getReceiveTimeout();
}
*/





/*
void WebSocketAdapter::connect(const Address& peerAddress) 
{
	if (!_context) 
		throw Exception("Cannot connect without WS context");

	traceL("WebSocketAdapter", this) << "Connecting to " << peerAddress << endl;
	init();
	const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(peerAddress.addr());
	assert(connectReq.data == this);
	int r = uv_tcp_connect(&connectReq, (uv_tcp_t*)stream(), *addr, uv::onConnected);
	if (r) {
		uv_err_t err = uv_last_error(loop());
		setError(err);
		throw Exception(uv_strerror(err)); // TODO: make exception setError option
	}
}


int WebSocketAdapter::available() const
{
	assert(initialized());
	return framer.available();
}


void WebSocketAdapter::close()
{
	traceL("WebSocketAdapter", this) << "Close" << endl;
	TCPBase::close();
}


bool WebSocketAdapter::shutdown()
{
	traceL("WebSocketAdapter", this) << "Shutdown" << endl;
	try {
		// Try to gracefully shutdown the WS connection
		framer.shutdown();
	}
	catch (...) {}
	return TCPBase::shutdown();
}
*/