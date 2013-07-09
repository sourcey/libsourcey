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
#include "Sourcey/Logger.h"

#include "Poco/Net/NetException.h"
#include "Poco/Buffer.h"
#include "Poco/BinaryWriter.h"
#include "Poco/BinaryReader.h"
#include "Poco/MemoryStream.h"
#include "Poco/Format.h"

#include "Poco/Buffer.h"
#include "Poco/MemoryStream.h"
#include "Poco/NullStream.h"
#include "Poco/BinaryWriter.h"
#include "Poco/SHA1Engine.h"
#include "Poco/Base64Encoder.h"
#include "Poco/String.h"
#include "Poco/Random.h"
#include "Poco/StreamCopier.h"


using namespace std;
using namespace scy::net;


namespace scy {
namespace http {


WebSocket::WebSocket() : 
	net::Socket(new net::TCPBase, false, 
		new WebSocketAdapter(this, WebSocket::WS_CLIENT))
{
}


WebSocket::WebSocket(net::SocketBase* base, bool shared) : 
	net::Socket(base, shared, 
		new WebSocketAdapter(this, WebSocket::WS_CLIENT))
{
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
	return static_cast<WebSocketAdapter&>(Socket::adapter());
}


const string WebSocket::WEBSOCKET_GUID("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
const string WebSocket::WEBSOCKET_VERSION("13");


// -------------------------------------------------------------------
//
WebSocketAdapter::WebSocketAdapter(net::Socket* socket, WebSocket::Mode mode) : 
	net::SocketAdapter(socket, 100), framer(mode)
{
	traceL("WebSocketAdapter", this) << "Creating" << endl;
}

	
WebSocketAdapter::WebSocketAdapter(WebSocket::Mode mode) : 
	net::SocketAdapter(NULL, 100), framer(mode)
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
	buffer.writeU16(statusCode);
	buffer.write(statusMessage);
	return socket->base().send(buffer.begin(), buffer.size(), WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_CLOSE) > 0;
}


int WebSocketAdapter::send(const char* data, int len, int flags) 
{	
	//traceL("WebSocketAdapter", this) << "WS Send: " << len << endl;
	return send(data, len, socket->peerAddress(), flags);
}


int WebSocketAdapter::send(const char* data, int len, const net::Address& peerAddr, int flags) 
{	
	traceL("WebSocketAdapter", this) << "Send: " << string(data, len) << endl;
	assert(framer.handshakeComplete());

	// Set default binary flag if none specified
	if (!flags)
		flags = WebSocket::FRAME_BINARY;

	// Frame and send the data
	Buffer frame(len + WebSocketFramer::MAX_HEADER_LENGTH);
	framer.writeFrame(data, len, flags, frame);
	return socket->base().send(frame.begin(), frame.size(), peerAddr, 0);
}


// -------------------------------------------------------------------
//
WebSocketClientAdapter::WebSocketClientAdapter(net::Socket* socket, http::Request* request) : 
	WebSocketAdapter(socket, WebSocket::WS_CLIENT), request(request)
{
	traceL("WebSocketClientAdapter", this) << "Creating" << endl;
}

	
WebSocketClientAdapter::~WebSocketClientAdapter() 
{	
	traceL("WebSocketClientAdapter", this) << "Destroying" << endl;
}

	
void WebSocketClientAdapter::sendRequest(http::Request& request)
{
	framer.createHandshakeRequest(request);

	ostringstream oss;
	request.write(oss);
	socket->base().send(oss.str().data(), oss.str().length());
}


//
// Callbacks
// 

void WebSocketClientAdapter::onSocketConnect()
{
	traceL("WebSocketClientAdapter", this) << "On connect" << endl;
	
	// Send the WS handshake request
	if (this->request)
		sendRequest(*this->request);
	else {
		http::Request request;
		sendRequest(request);
	}

	/*
		request.setURI("/websocket");
	http::Request request;
	request
	framer.createHandshakeRequest(request);

	ostringstream oss;
	request.write(oss);
	socket->base().send(oss.str().data(), oss.str().length());
	*/
}


void WebSocketClientAdapter::onSocketRecv(Buffer& buffer, const net::Address& peerAddr)
{
	traceL("WebSocketClientAdapter", this) << "On recv: " << buffer << endl;

	try {
		assert(buffer.position() == 0);
		assert(buffer.size() > 0);

		if (framer.handshakeComplete()) {
			framer.readFrame(buffer);
			SocketAdapter::onSocketRecv(buffer, peerAddr);
		}

		else {
			http::Response response;
			http::Parser parser(&response);
			parser.parse(buffer.data(), buffer.size(), true);
			
			/*
			istringstream iss;
			iss.str(buffer.data());
			http::Response response;
			response.read(iss); // TODO: HTTP Parser
			*/


			// Parse and check the response
			if (framer.checkHandshakeResponse(response)) {
				
				traceL("WebSocketClientAdapter", this) << "Handshake success: Connected" << endl;
				SocketAdapter::onSocketConnect();
			}
				//emitConnect();
			
			// TODO: Resend request for authentication
			else
				assert(0 && "not implemented");
		}
	} 
	catch(Exception& exc) {
		warnL("WebSocketClientAdapter", this) << "Handshake failed: " << exc.displayText() << endl;

		socket->close();
		//assert(0);
		// TODO: Set and emit custom error message
		//setError();
	}
}


// -------------------------------------------------------------------
//
WebSocketServerAdapter::WebSocketServerAdapter(net::Socket* socket) : 
	WebSocketAdapter(socket, WebSocket::WS_SERVER)
{
	traceL("WebSocketServerAdapter", this) << "Creating" << endl;
}

	
WebSocketServerAdapter::~WebSocketServerAdapter() 
{	
	traceL("WebSocketServerAdapter", this) << "Destroying" << endl;
}


void WebSocketServerAdapter::onSocketRecv(Buffer& buffer, const net::Address& peerAddr)
{
	traceL("WebSocketServerAdapter", this) << "On recv: " << buffer << endl;
	
	assert(buffer.position() == 0);
	assert(buffer.size() > 0);

	if (framer.handshakeComplete()) {
		framer.readFrame(buffer);
		traceL("WebSocketServerAdapter", this) << "On frame: " << buffer.size() << ": " << buffer << endl;	
		SocketAdapter::onSocketRecv(buffer, peerAddr);
	}

	else {
		try {
			errorL("WebSocketServerAdapter", this) << "Verifying handshake: " << buffer << endl;

			http::Request request;
			http::Parser parser(&request);
			parser.parse(buffer.data(), buffer.size(), true);
			
			
			/*
			istringstream iss;
			iss.str(buffer.data());
			http::Request request;
			request.read(iss); // TODO: HTTP Parser
			*/
			
			http::Response response;

			// Verify the WebSocket handshake request or throw
			framer.acceptRequest(request, response);

			errorL("WebSocketServerAdapter", this) << "Handshake success" << endl;
				
			// Send response
			ostringstream oss;
			response.write(oss);
			socket->base().send(oss.str().data(), oss.str().length());
		} 
		catch(Exception& exc) {
			errorL("WebSocketServerAdapter", this) << "Handshake failed: " << exc.displayText() << endl;
			
			// Handshake failed, set error and close
			// TODO: Set and emit custom error message
			//setError();	
			assert(0);
			socket->close();		
		}
	}
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
		throw Poco::Net::WebSocketException("Cannot upgrade to WebSocket connection", response.getReason(), WebSocket::WS_ERR_NO_HANDSHAKE);

	// Need to resend request
	return false;
}


void WebSocketFramer::acceptRequest(http::Request& request, http::Response& response)
{
	if (request.hasToken("Connection", "upgrade") && Poco::icompare(request.get("Upgrade", ""), "websocket") == 0)
	{
		string version = request.get("Sec-WebSocket-Version", "");
		if (version.empty()) throw Poco::Net::WebSocketException("Missing Sec-WebSocket-Version in handshake request", WebSocket::WS_ERR_HANDSHAKE_NO_VERSION);
		if (version != WebSocket::WEBSOCKET_VERSION) throw Poco::Net::WebSocketException("Unsupported WebSocket version requested", version, WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION);
		string key = request.get("Sec-WebSocket-Key", "");
		Poco::trimInPlace(key);
		if (key.empty()) throw Poco::Net::WebSocketException("Missing Sec-WebSocket-Key in handshake request", WebSocket::WS_ERR_HANDSHAKE_NO_KEY);
		
		response.setStatusAndReason(http::Response::HTTP_SWITCHING_PROTOCOLS);
		response.set("Upgrade", "websocket");
		response.set("Connection", "Upgrade");
		response.set("Sec-WebSocket-Accept", computeAccept(key));

		// Set headerState 2 since the handskake was accepted.
		_headerState = 2;

		//response.setContentLength(0);
		//response.send().flush();		
		//return new WebSocketImpl(static_cast<StreamSocketImpl*>(static_cast<http::RequestImpl&>(request).detachSocket().impl()), false);
	}
	else throw Poco::Net::WebSocketException("No WebSocket handshake", WebSocket::WS_ERR_NO_HANDSHAKE);
}

	
int WebSocketFramer::writeFrame(const char* data, int len, int flags, Buffer& frame)
{
	assert(flags == WebSocket::FRAME_TEXT || 
		flags == WebSocket::FRAME_BINARY);

	assert(frame.capacity() >= len + MAX_HEADER_LENGTH);
	
	frame.writeU8(static_cast<UInt8>(flags));
	UInt8 lenByte(0);
	if (_mustMaskPayload)
	{
		lenByte |= FRAME_FLAG_MASK;
	}
	if (len < 126)
	{
		lenByte |= static_cast<UInt8>(len);
		frame.writeU8(lenByte);
	}
	else if (len < 65536)
	{
		lenByte |= 126;
		frame.writeU8(lenByte);
		frame.writeU16(static_cast<UInt16>(len));
	}
	else
	{
		lenByte |= 127;
		frame.writeU8(lenByte);
		frame.writeU64(static_cast<UInt16>(len));
	}

	if (_mustMaskPayload)
	{
		const UInt32 mask = _rnd.next();
		const char* m = reinterpret_cast<const char*>(&mask);
		const char* b = reinterpret_cast<const char*>(data);
		frame.write(m, 4);
		char* p = frame.begin() + frame.size();
		for (int i = 0; i < len; i++)
		{
			p[i] = b[i] ^ m[i % 4];
		}
	}
	else
	{
		memcpy(frame.begin() + frame.size(), data, len);
	}

	// Set the frame size
	frame.size(len + frame.size());

	return frame.size();
}

	
int WebSocketFramer::readFrame(Buffer& buffer)
{
	assert(handshakeComplete());
	assert(buffer.position() == 0);
	assert(buffer.capacity() > MAX_HEADER_LENGTH);
	
	char header[MAX_HEADER_LENGTH];	
	buffer.read(header, 2);

	int length = buffer.size();
	UInt8 lengthByte = static_cast<UInt8>(header[1]);
	int maskOffset = 0;
	if (lengthByte & FRAME_FLAG_MASK) maskOffset += 4;
	lengthByte &= 0x7f;
	if (lengthByte + 2 + maskOffset < MAX_HEADER_LENGTH)
	{
		buffer.read(header + 2, lengthByte + maskOffset);
	}
	else
	{
		buffer.read(header + 2, MAX_HEADER_LENGTH - 2);
	}

	assert(buffer.position() > 0);
	buffer.consume(2);

	Poco::MemoryInputStream istr(header, buffer.position());
	Poco::BinaryReader reader(istr, Poco::BinaryReader::NETWORK_BYTE_ORDER);
	UInt8 flags;
	char mask[4];
	reader >> flags >> lengthByte;
	_frameFlags = flags;
	int payloadLength = 0;
	int payloadOffset = 2;
	if ((lengthByte & 0x7f) == 127)
	{
		UInt64 l;
		reader >> l;
		if (l > length)
			throw Poco::Net::WebSocketException(Poco::format("Insufficient buffer for payload size %Lu", l), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
		payloadOffset += 8;
	}
	else if ((lengthByte & 0x7f) == 126)
	{
		UInt16 l;
		reader >> l;
		if (l > length)
			throw Poco::Net::WebSocketException(Poco::format("Insufficient buffer for payload size %hu", l), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
		payloadOffset += 2;
	}
	else
	{
		UInt8 l = lengthByte & 0x7f;
		if (l > length)
			throw Poco::Net::WebSocketException(Poco::format("Insufficient buffer for payload size %u", unsigned(l)), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
	}
	if (lengthByte & FRAME_FLAG_MASK)
	{	
		reader.readRaw(mask, 4);
		payloadOffset += 4;
	}

	if (payloadLength > length)
		throw Poco::Net::WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);	

	//traceL("WebSocketServerAdapter", this) << "Read frame at: " 
	//	<< payloadOffset << ": " << payloadLength << ": " << length << endl;	

	// Set the size and position to match payload
	buffer.position(payloadOffset);
	buffer.size(payloadLength);

	if (lengthByte & FRAME_FLAG_MASK)
	{
		char* p = reinterpret_cast<char*>(buffer.data());
		for (int i = 0; i < payloadLength; i++)
		{
			p[i] ^= mask[i % 4];
		}
	}

	return payloadLength;
}


void WebSocketFramer::completeHandshake(http::Response& response) //, const string& key HTTPClientSession& cs, 
{
	string connection = response.get("Connection", "");
	if (Poco::icompare(connection, "Upgrade") != 0) 
		throw Poco::Net::WebSocketException("No Connection: Upgrade header in handshake response", WebSocket::WS_ERR_NO_HANDSHAKE);
	string upgrade = response.get("Upgrade", "");
	if (Poco::icompare(upgrade, "websocket") != 0)
		throw Poco::Net::WebSocketException("No Upgrade: websocket header in handshake response", WebSocket::WS_ERR_NO_HANDSHAKE);
	string accept = response.get("Sec-WebSocket-Accept", "");
	if (accept != computeAccept(_key))
		throw Poco::Net::WebSocketException("Invalid or missing Sec-WebSocket-Accept header in handshake response", WebSocket::WS_ERR_NO_HANDSHAKE);
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
	//int received = payloadOffset + payloadLength;		
	//if (received > length)
	//	throw Poco::Net::WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);	
Buffer& WebSocketAdapter::recvBuffer()
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
Poco::MemoryOutputStream ostr(buffer.begin(), buffer.size());
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
	//traceL("WebSocketFramer", this) << "readNBytes: " << bytes << ": " << buffer.size() << endl;

	//memcpy(buffer, header + payloadOffset, n - payloadOffset);
	//_recvBuffer.size()
	//_recvBuffer.read((char*)buffer, bytes);
	
	return 0;
	if (bytes > buffer.size())
		throw Poco::Net::WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);

	buffer.read((char*)buffer, bytes);
	return bytes;

	int received = _socketBase->readFrame(reinterpret_cast<char*>(buffer), bytes);
	while (received < bytes)
	{
		int n = _socketBase->readFrame(reinterpret_cast<char*>(buffer) + received, bytes - received);
		if (n > 0)
			received += n;
		else
			throw Poco::Net::WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);
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
	traceL("WebSocketFramer", this) << "RECV: buffer.size: " << buffer.size() << endl;
	traceL("WebSocketFramer", this) << "RECV: received: " << received << endl;
	traceL("WebSocketFramer", this) << "RECV: payloadLength: " << payloadLength << endl;
	traceL("WebSocketFramer", this) << "RECV: payloadOffset: " << payloadOffset << endl;
	
	*/

	/*
	//cs.sendRequest(request);
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


void WebSocketFramer::connect(const SocketAddress& address, const Poco::Timespan& timeout)
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


void WebSocketFramer::setSendTimeout(const Poco::Timespan& timeout)
{
	_socketBase->setSendTimeout(timeout);
}


Poco::Timespan WebSocketFramer::getSendTimeout()
{
	return _socketBase->getSendTimeout();
}


void WebSocketFramer::setReceiveTimeout(const Poco::Timespan& timeout)
{
	_socketBase->setReceiveTimeout(timeout);
}


Poco::Timespan WebSocketFramer::getReceiveTimeout()
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
		throw Poco::Exception(uv_strerror(err)); // TODO: make exception setError option
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