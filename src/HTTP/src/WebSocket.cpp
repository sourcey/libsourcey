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
#include "Sourcey/Crypto/Hash.h"
#include "Sourcey/Base64.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Numeric.h"
#include "Sourcey/Random.h"
#include <stdexcept>


using namespace std;


namespace scy {
namespace http {


const char* WebSocket::ProtocolGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
const char* WebSocket::ProtocolVersion = "13";


WebSocket::WebSocket() : 
	net::Socket(new net::TCPBase, false)
{
	setAdapter(new WebSocketAdapter(this, WebSocket::ClientSide, _request, _response));
}


WebSocket::WebSocket(net::SocketBase* base, bool shared) : 
	net::Socket(base, shared)
{
	setAdapter(new WebSocketAdapter(this, WebSocket::ClientSide, _request, _response));
}


WebSocket::WebSocket(const net::Socket& socket) : 
	net::Socket(socket)
{
	if (!dynamic_cast<WebSocketAdapter*>(_adapter))
		throw std::runtime_error("WebSocket error: Cannot assign incompatible socket adapter");
}

	
bool WebSocket::shutdown(UInt16 statusCode, const std::string& statusMessage)
{
	return adapter().shutdown(statusCode, statusMessage);
}


WebSocketAdapter& WebSocket::adapter() const
{
	assert(net::Socket::adapter());
	return *static_cast<WebSocketAdapter*>(net::Socket::adapter());
}


http::Request& WebSocket::request()
{
	return _request;
}


http::Response& WebSocket::response()
{
	return _response;
}


//
// WebSocket Adapter
//


WebSocketAdapter::WebSocketAdapter(net::Socket* socket, WebSocket::Mode mode, http::Request& request, http::Response& response) : 
	net::SocketAdapter(socket), 
	framer(mode), _request(request), _response(response)
{
	traceL("WebSocketAdapter", this) << "constructor" << endl;
}

	
WebSocketAdapter::WebSocketAdapter(WebSocket::Mode mode, http::Request& request, http::Response& response) : 
	net::SocketAdapter(nullptr),
	framer(mode), _request(request), _response(response)
{
	traceL("WebSocketAdapter", this) << "destructor" << endl;
}

	
WebSocketAdapter::~WebSocketAdapter() 
{	
}

	
bool WebSocketAdapter::shutdown(UInt16 statusCode, const std::string& statusMessage)
{
	char buffer[256];
	BitWriter writer(buffer, 256);
	writer.putU16(statusCode);
	writer.put(statusMessage);
	return socket->base().send(buffer, writer.position(), 
		unsigned(WebSocket::FrameFlags::Fin) |
		unsigned(WebSocket::Opcode::Close)) > 0;
}


int WebSocketAdapter::send(const char* data, int len, int flags) 
{	
	return send(data, len, socket->peerAddress(), flags);
}


int WebSocketAdapter::send(const char* data, int len, const net::Address& peerAddr, int flags) 
{	
	traceL("WebSocketAdapter", this) << "Send: " << std::string(data, len) << endl;
	assert(framer.handshakeComplete());

	// Set default text flag if none specified
	if (!flags)
		flags = WebSocket::SendFlags::Text;

	// Frame and send the data
	//std::vector<char> buffer(len + WebSocketFramer::MAX_HEADER_LENGTH);
	Buffer buffer;
	buffer.reserve(len + WebSocketFramer::MAX_HEADER_LENGTH);
	BitWriter writer(buffer);
	framer.writeFrame(data, len, flags, writer);
	
	return socket->base().send(writer.begin(), writer.position(), peerAddr, 0);
}

	
void WebSocketAdapter::sendClientRequest()
{
	framer.createHandshakeRequest(_request);

	ostringstream oss;
	_request.write(oss);
	traceL("WebSocketClientAdapter", this) << "Client request: " << oss.str() << endl;
	socket->base().send(oss.str().c_str(), oss.str().length());
}


void WebSocketAdapter::handleClientResponse(const MutableBuffer& buffer)
{
	traceL("WebSocketClientAdapter", this) << "Client response: " << buffer.size() << endl;
	http::Parser parser(&_response);
	if (!parser.parse(bufferCast<char *>(buffer), buffer.size())) {
		throw std::runtime_error("WebSocket error: Cannot parse response: Incomplete HTTP message");
	}
	
	// TODO: Handle resending request for authentication
	// Should we implement some king of callback for this?

	//traceL("WebSocketClientAdapter", this) << "Client response: parsed: " << _response << endl;

	// Parse and check the response
	if (framer.checkHandshakeResponse(_response)) {				
		traceL("WebSocketClientAdapter", this) << "Handshake success" << endl;
		SocketAdapter::onSocketConnect();
	}			
}

	
void WebSocketAdapter::handleServerRequest(const MutableBuffer& buffer)
{
	//http::Request request;
	http::Parser parser(&_request);
	if (parser.parse(bufferCast<char *>(buffer), buffer.size())) {
		throw std::runtime_error("WebSocket error: Cannot parse request: Incomplete HTTP message");
	}
	
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
	catch (std::exception& exc) {
		warnL("WebSocketClientAdapter", this) << "Handshake failed: " << exc.what() << endl;		
	}

	// Allow the application to override the response
	//PrepareServerResponse.emit(this, response);
				
	// Send response
	ostringstream oss;
	_response.write(oss);
	socket->base().send(oss.str().c_str(), oss.str().length());
}


void WebSocketAdapter::onSocketConnect()
{
	traceL("WebSocketAdapter", this) << "On connect" << endl;
	
	// Send the WS handshake request
	sendClientRequest();

	// Connect signal sent after handshake complete
	//SocketAdapter::onSocketConnect();
}


void WebSocketAdapter::onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddr)
{
	traceL("WebSocketAdapter", this) << "On recv: " << buffer.size() << endl; // << ": " << buffer

	//assert(buffer.position() == 0);

	if (framer.handshakeComplete()) {

		// Note: The spec wants us to buffer partial frames, but our
		// software does not require this feature, and furthermore
		// it goes against our nocopy where possible policy. 
		// This may need to change in the future, but for now
		// we just parse and emit packets as they arrive.
		//
		// Incoming frames may be joined, so we parse them
		// in a loop until the read buffer is empty.
		BitReader reader(buffer);
		int total = reader.available();
		int offset = reader.position();
		while (offset < total) {
			char* payload = nullptr;
			int payloadLength = 0;
			try {
				// Restore buffer state for next read
				//reader.position(offset);
				//reader.limit(total);
					
				/*
				traceL("WebSocketServerAdapter", this) << "Read frame at: " 
					 << "\n\tinputPosition: " << offset
					 << "\n\tinputLength: " << total
					 << "\n\tbufferPosition: " << reader.position() 
					 << "\n\tbufferAvailable: " << reader.available() 
					 << "\n\tbufferLimit: " << reader.limit() 
					 << "\n\tbuffer: " << std::string(reader.current(), reader.limit())
					 << endl;	
					 */
				
				// Parse a frame to throw
				//int payloadLength = framer.readFrame(reader);
				payloadLength = framer.readFrame(reader, payload);
				assert(payload);

				// Update the next frame offset
				offset = reader.position(); // + payloadLength; 
				if (offset < total)
					debugL("WebSocketAdapter", this) << "Splitting joined packet at "
						<< offset << " of " << total << endl;

				// Drop empty packets
				if (!payloadLength) {
					debugL("WebSocketAdapter", this) << "empty frame" << endl;
					continue;
				}
			} 
			catch (std::exception& exc) {
				warnL("WebSocketAdapter", this) << "Parser error: " << exc.what() << endl;		
				socket->setError(exc.what());	
				return;
			}
			
			// Emit the result packet
			//traceL("WebSocketAdapter", this) << "On recv: emit: " << buffer << endl;
			assert(payload);
			assert(payloadLength);
			SocketAdapter::onSocketRecv(mutableBuffer(payload, payloadLength), peerAddr);
		}
		assert(offset == total);
	}
	else {		
		try {
			if (framer.mode() == WebSocket::ClientSide)
				handleClientResponse(buffer);
			else
				handleServerRequest(buffer);
		} 
		catch (std::exception& exc) {
			warnL("WebSocketAdapter", this) << "Read error: " << exc.what() << endl;		
			socket->setError(exc.what());	
		}
		//traceL("WebSocketAdapter", this) << "After handshaking: " << buffer << endl;
		return;
	}	
}


void WebSocketAdapter::onSocketClose()
{
	// Reset state incase the connection is reused	
	_request.clear();
	_response.clear();
	framer._headerState = 0;
	framer._frameFlags = 0;

	// Emit closed event
	SocketAdapter::onSocketClose();
}


//
// WebSocket Connection Adapter
//


WebSocketConnectionAdapter::WebSocketConnectionAdapter(Connection& connection, WebSocket::Mode mode) : 
	WebSocketAdapter(&connection.socket(), mode, connection.request(), connection.response()), 
	_connection(connection)
{
}

	
WebSocketConnectionAdapter::~WebSocketConnectionAdapter() 
{	
}


//
// WebSocket Framer
//


WebSocketFramer::WebSocketFramer(WebSocket::Mode mode) : //bool mustMaskPayload
	_mode(mode),
	_frameFlags(0),
	_headerState(0),
	_maskPayload(mode == WebSocket::ClientSide)
{
}


WebSocketFramer::~WebSocketFramer()
{
}


std::string createKey()
{
	return base64::encode(util::randomString(16));
}


string computeAccept(const std::string& key)
{
	std::string accept(key);
	crypto::Hash engine("SHA1");
	engine.update(key + WebSocket::ProtocolGuid);
	return base64::encode(engine.digest());
}


void WebSocketFramer::createHandshakeRequest(http::Request& request)
{
	assert(_mode == WebSocket::ClientSide);
	assert(_headerState == 0);

	// Send the handshake request
	_key = createKey();
	request.setChunkedTransferEncoding(false);
	request.set("Connection", "Upgrade");
	request.set("Upgrade", "websocket");
	request.set("Sec-WebSocket-Version", WebSocket::ProtocolVersion);
	assert(request.has("Sec-WebSocket-Version"));
	request.set("Sec-WebSocket-Key", _key);
	assert(request.has("Sec-WebSocket-Key"));
	//traceL("WebSocketFramer", this) << "Sec-WebSocket-Version: " << request.get("Sec-WebSocket-Version") << endl;
	//traceL("WebSocketFramer", this) << "Sec-WebSocket-Key: " << request.get("Sec-WebSocket-Key") << endl;
	_headerState++;
}


bool WebSocketFramer::checkHandshakeResponse(http::Response& response)
{	
	assert(_mode == WebSocket::ClientSide);
	assert(_headerState == 1);
	if (response.getStatus() == http::StatusCode::SwitchingProtocols) 
	{
		// Complete handshake or throw
		completeHandshake(response);
		
		// Success
		_headerState++;
		assert(handshakeComplete());
		return true;
	}
	else if (response.getStatus() == http::StatusCode::NotAuthorized)
		assert(0 && "authentication not implemented");
	else
		throw std::runtime_error("WebSocket error: Cannot upgrade to WebSocket connection: " + response.getReason()); //, WebSocket::ErrorNoHandshake

	// Need to resend request
	return false;
}


void WebSocketFramer::acceptRequest(http::Request& request, http::Response& response)
{
	if (util::icompare(request.get("Connection", ""), "upgrade") == 0 && 
		util::icompare(request.get("Upgrade", ""), "websocket") == 0) {
		std::string version = request.get("Sec-WebSocket-Version", "");
		if (version.empty()) throw std::runtime_error("WebSocket error: Missing Sec-WebSocket-Version in handshake request"); //, WebSocket::ErrorHandshakeNoVersion
		if (version != WebSocket::ProtocolVersion) throw std::runtime_error("WebSocket error: Unsupported WebSocket version requested: " + version); //, WebSocket::ErrorHandshakeUnsupportedVersion
		std::string key = util::trim(request.get("Sec-WebSocket-Key", ""));
		if (key.empty()) throw std::runtime_error("WebSocket error: Missing Sec-WebSocket-Key in handshake request"); //, WebSocket::ErrorHandshakeNoKey
		
		response.setStatusAndReason(http::StatusCode::SwitchingProtocols);
		response.set("Upgrade", "websocket");
		response.set("Connection", "Upgrade");
		response.set("Sec-WebSocket-Accept", computeAccept(key));

		// Set headerState 2 since the handshake was accepted.
		_headerState = 2;
	}
	else throw std::runtime_error("WebSocket error: No WebSocket handshake"); //, WebSocket::ErrorNoHandshake
}

	
int WebSocketFramer::writeFrame(const char* data, int len, int flags, BitWriter& frame)
{
	assert(flags == WebSocket::SendFlags::Text || 
		flags == WebSocket::SendFlags::Binary);	
	assert(frame.position() == 0);
	//assert(frame.limit() >= std::size_t(len + MAX_HEADER_LENGTH));
			
	frame.putU8(static_cast<UInt8>(flags));
	UInt8 lenByte(0);
	if (_maskPayload) {
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

	if (_maskPayload) {
		auto mask = _rnd.next();
		auto m = reinterpret_cast<const char*>(&mask);
		auto b = reinterpret_cast<const char*>(data);
		frame.put(m, 4);
		//auto p = frame.current();
		for (int i = 0; i < len; i++) {
			//p[i] = b[i] ^ m[i % 4];
			frame.putU8(b[i] ^ m[i % 4]);
		}
	}
	else {
		//memcpy(frame.current(), data, len); // offset?
		frame.put(data, len);
	}
	
	// Update frame length to include payload plus header
	//frame.skip(len);

	/*
	traceL("WebSocketServerAdapter", this) << "Write frame: " 
		 << "\n\tinputLength: " << len
		 << "\n\tframePosition: " << frame.position() 
		 << "\n\tframeLimit: " << frame.limit() 
		 << "\n\tframeAvailable: " << frame.available() 
		 << endl;	*/

	return frame.position();
}

	
int WebSocketFramer::readFrame(BitReader& frame, char*& payload)
{
	assert(handshakeComplete());
	UInt64 limit = frame.limit();
	size_t offset = frame.position(); 
	//assert(offset == 0);
	
	// Read the frame header
	char header[MAX_HEADER_LENGTH];
	BitReader headerReader(header, MAX_HEADER_LENGTH);	
	frame.get(header, 2);
	UInt8 lengthByte = static_cast<UInt8>(header[1]);
	int maskOffset = 0;
	if (lengthByte & FRAME_FLAG_MASK) maskOffset += 4;
	lengthByte &= 0x7f;
	if (lengthByte + 2 + maskOffset < MAX_HEADER_LENGTH)
		frame.get(header + 2, lengthByte + maskOffset);
	else
		frame.get(header + 2, MAX_HEADER_LENGTH - 2);

	// Reserved fields
	frame.skip(2);	

	// Parse frame header
	UInt8 flags;
	char mask[4];
	headerReader.getU8(flags);	
	headerReader.getU8(lengthByte);
	_frameFlags = flags;
	UInt64 payloadLength = 0;
	int payloadOffset = 2;
	if ((lengthByte & 0x7f) == 127) {
		UInt64 l;
		headerReader.getU64(l);
		if (l > limit)
			throw std::runtime_error(util::format("WebSocket error: Insufficient buffer for payload size %" I64_FMT "u", l)); //, WebSocket::ErrorPayloadTooBig
		payloadLength = l;
		payloadOffset += 8;
	}
	else if ((lengthByte & 0x7f) == 126) {
		UInt16 l;
		headerReader.getU16(l);
		if (l > limit)
			throw std::runtime_error(util::format("WebSocket error: Insufficient buffer for payload size %u", unsigned(l))); //, WebSocket::ErrorPayloadTooBig
		payloadLength = l;
		payloadOffset += 2;
	}
	else {
		UInt8 l = lengthByte & 0x7f;
		if (l > limit)
			throw std::runtime_error(util::format("WebSocket error: Insufficient buffer for payload size %u", unsigned(l))); //, WebSocket::ErrorPayloadTooBig
		payloadLength = l;
	}
	if (lengthByte & FRAME_FLAG_MASK) {	
		headerReader.get(mask, 4);
		payloadOffset += 4;
	}

	if (payloadLength > limit) //length)
		throw std::runtime_error("WebSocket error: Incomplete frame received"); //, WebSocket::ErrorIncompleteFrame		

	// Get a reference to the start of the payload
	payload = reinterpret_cast<char*>(const_cast<char*>(frame.begin() + (offset + payloadOffset)));

	// Unmask the payload if required
	if (lengthByte & FRAME_FLAG_MASK) {
		auto p = reinterpret_cast<char*>(payload); //frame.data());
		for (UInt64 i = 0; i < payloadLength; i++) {
			p[i] ^= mask[i % 4];
		}
	}
	
	// Update frame length to include payload plus header
	frame.seek(offset + payloadOffset + payloadLength);
	//frame.limit(offset + payloadOffset + payloadLength);
	//int frameLength = (offset + payloadOffset);
	//assert(frame.position() == (offset + payloadOffset));

	return payloadLength;
}


void WebSocketFramer::completeHandshake(http::Response& response)
{
	std::string connection = response.get("Connection", "");
	if (util::icompare(connection, "Upgrade") != 0) 
		throw std::runtime_error("WebSocket error: No Connection: Upgrade header in handshake response"); //, WebSocket::ErrorNoHandshake
	std::string upgrade = response.get("Upgrade", "");
	if (util::icompare(upgrade, "websocket") != 0)
		throw std::runtime_error("WebSocket error: No Upgrade: websocket header in handshake response"); //, WebSocket::ErrorNoHandshake
	std::string accept = response.get("Sec-WebSocket-Accept", "");
	if (accept != computeAccept(_key))
		throw std::runtime_error("WebSocket error: Invalid or missing Sec-WebSocket-Accept header in handshake response"); //, WebSocket::ErrorNoHandshake
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
	return _maskPayload;
}


} } // namespace scy::http
