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
#include "Poco/Net/WebSocket.h"
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
	net::Socket(new WebSocketBase, false)
{
}


WebSocket::WebSocket(WebSocketBase* base, bool shared) : 
	net::Socket(base, shared) 
{
}


WebSocket::WebSocket(const Socket& socket) : 
	net::Socket(socket)
{
	if (!dynamic_cast<WebSocketBase*>(_base))
		throw Exception("Cannot assign incompatible socket");
}
	

WebSocketBase& WebSocket::base() const
{
	return static_cast<WebSocketBase&>(*_base);
}


const string WebSocket::WEBSOCKET_GUID("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
const string WebSocket::WEBSOCKET_VERSION("13");


// -------------------------------------------------------------------
//
WebSocketBase::WebSocketBase() : 
	_wsParser(this, true) // client socket = mustMaskPayload
{
	traceL("WebSocketBase", this) << "Creating" << endl;
	assert(_handle);
	_handle->data = this;
	connectReq.data = this;
}

	
WebSocketBase::~WebSocketBase() 
{	
	traceL("WebSocketBase", this) << "Destroying" << endl;
}

	
bool WebSocketBase::shutdown(UInt16 statusCode, const string& statusMessage)
{
	//uv::TCPBase::shutdown();	
	Poco::Buffer<char> buffer(statusMessage.size() + 2);
	Poco::MemoryOutputStream ostr(buffer.begin(), buffer.size());
	Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);
	writer << statusCode;
	writer.writeRaw(statusMessage);
	return send(buffer.begin(), static_cast<int>(ostr.charsWritten()), WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_CLOSE) > 0;
}


int WebSocketBase::send(const char* data, int len, int flags) 
{	
	assert(len <= net::MAX_TCP_PACKET_SIZE);	
	assert(initialized());
	traceL("WebSocketBase", this) << "WS Send: " << len << endl;
	
	// Send data to the WS parser for framing and sending	
	return _wsParser.sendFrame(data, len, flags);
}


//
// Callbacks
// 

void WebSocketBase::onRead(const char* data, int len)
{
	traceL("WebSocketBase", this) << "On WS Read: " << string(data, len) << endl;
	
	try {
		_buffer.position(0);
		_buffer.size(len);
		if (_wsParser.connected())
			_wsParser.receiveFrame(_buffer);
		else {
			if (_wsParser.handleHandshakeResponse())
				emitConnect();
			else
				// TODO: Resend authenticated request
				assert(0);
		}

	} catch(Exception& exc) {
		errorL("WebSocketBase", this) << "Handshake Failed: " << exc.displayText() << endl;

		assert(0);
		// TODO: Set and emit custom error message
		//setError();
	}
}


void WebSocketBase::onRecv(Buffer& buf)
{
	Log("WebSocketBase", this) << "On WS Recv: " 
		<< _buffer.position() << ": " << _buffer.size() << ": " << buf << endl;
	emitRecv(_buffer, peerAddress());
}


void WebSocketBase::onConnect(int status)
{
	traceL("WebSocketBase", this) << "On Connected" << endl;
	if (status) {
		setLastError();
		errorL("WebSocketBase", this) << "Connect Failed: " << errorMessage() << endl;
		return;
	}
	else
		readStart(); 
	
	_wsParser.sendHandshakeRequest();

	traceL("WebSocketBase", this) << "On Connected: OK" << endl;
}


// ---------------------------------------------------------------------
//
WebSocketParser::WebSocketParser(WebSocketBase* socketBase, bool mustMaskPayload) :
	_socketBase(socketBase),
	_frameFlags(0),
	_headerState(0),
	_mustMaskPayload(mustMaskPayload)
{
}


WebSocketParser::~WebSocketParser()
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


void WebSocketParser::sendHandshakeRequest()
{
	assert(_headerState == 0);

	// Send the initial request
	_key = createKey();
	http::Request request;
	request.set("Connection", "Upgrade");
	request.set("Upgrade", "websocket");
	request.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
	request.set("Sec-WebSocket-Key", _key);
	request.setChunkedTransferEncoding(false);
	request.setKeepAlive(true);

	ostringstream oss;
	request.write(oss);
	_socketBase->write(oss.str().data(), oss.str().length());
	_headerState++;
}


bool WebSocketParser::handleHandshakeResponse()
{	
	assert(_headerState == 1);

	istringstream iss;
	iss.str(_socketBase->buffer().data());

	// TODO: Update buffer read pos, and Recv body data

	http::Response response;
	response.read(iss);

	if (response.getStatus() == http::Response::HTTP_SWITCHING_PROTOCOLS)
	{
		// Complete handshake or throw
		completeHandshake(response);
		
		// Success
		_headerState++;
		assert(connected());
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

	
int WebSocketParser::sendFrame(const char* buffer, int length, int flags)
{
	traceL("WebSocketParser") << "sendFrame: BEFORE: " << string((const char*)buffer, length) << ": " << length << ": " << flags << endl;	
	assert(flags == WebSocket::FRAME_TEXT || flags == WebSocket::FRAME_BINARY);

	Poco::Buffer<char> frame(length + MAX_HEADER_LENGTH);
	Poco::MemoryOutputStream ostr(frame.begin(), frame.size());
	Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);
	
	writer << static_cast<Poco::UInt8>(flags);
	Poco::UInt8 lengthByte(0);
	if (_mustMaskPayload)
	{
		lengthByte |= FRAME_FLAG_MASK;
	}
	if (length < 126)
	{
		lengthByte |= static_cast<Poco::UInt8>(length);
		writer << lengthByte;
	}
	else if (length < 65536)
	{
		lengthByte |= 126;
		writer << lengthByte << static_cast<Poco::UInt16>(length);
	}
	else
	{
		lengthByte |= 127;
		writer << lengthByte << static_cast<Poco::UInt64>(length);
	}

	if (_mustMaskPayload)
	{
		const Poco::UInt32 mask = _rnd.next();
		const char* m = reinterpret_cast<const char*>(&mask);
		const char* b = reinterpret_cast<const char*>(buffer);
		writer.writeRaw(m, 4);
		char* p = frame.begin() + ostr.charsWritten();
		for (int i = 0; i < length; i++)
		{
			p[i] = b[i] ^ m[i % 4];
		}
	}
	else
	{
		memcpy(frame.begin() + ostr.charsWritten(), buffer, length);
	}
	
	// Send the frame	
	return _socketBase->write(frame.begin(), length + static_cast<int>(ostr.charsWritten()));
}

	
int WebSocketParser::receiveFrame(Buffer& buffer) //char* buffer, int length char* buffer, int length, int
{
	assert(connected());
	assert(buffer.position() == 0);
	assert(buffer.size() > MAX_HEADER_LENGTH);
	
	char header[MAX_HEADER_LENGTH];	
	buffer.read(header, 2);

	int length = buffer.size();
	Poco::UInt8 lengthByte = static_cast<Poco::UInt8>(header[1]);
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
	Poco::UInt8 flags;
	char mask[4];
	reader >> flags >> lengthByte;
	_frameFlags = flags;
	int payloadLength = 0;
	int payloadOffset = 2;
	if ((lengthByte & 0x7f) == 127)
	{
		Poco::UInt64 l;
		reader >> l;
		if (l > length)
			throw Poco::Net::WebSocketException(Poco::format("Insufficient buffer for payload size %Lu", l), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
		payloadOffset += 8;
	}
	else if ((lengthByte & 0x7f) == 126)
	{
		Poco::UInt16 l;
		reader >> l;
		if (l > length)
			throw Poco::Net::WebSocketException(Poco::format("Insufficient buffer for payload size %hu", l), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
		payloadOffset += 2;
	}
	else
	{
		Poco::UInt8 l = lengthByte & 0x7f;
		if (l > length)
			throw Poco::Net::WebSocketException(Poco::format("Insufficient buffer for payload size %u", unsigned(l)), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
		payloadLength = static_cast<int>(l);
	}
	if (lengthByte & FRAME_FLAG_MASK)
	{
		reader.readRaw(mask, 4);
		payloadOffset += 4;
	}

	int received = payloadOffset + payloadLength;		
	if (received > length)
		throw Poco::Net::WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);	

	if (lengthByte & FRAME_FLAG_MASK)
	{
		char* p = reinterpret_cast<char*>(buffer.data());
		for (int i = 0; i < received; i++)
		{
			p[i] ^= mask[i % 4];
		}
	}
		
	traceL("WebSocketParser", this) << "Recv: " << received << endl;
	buffer.position(payloadOffset);
	buffer.size(received);
	_socketBase->onRecv(buffer);

	return received;
}


void WebSocketParser::completeHandshake(http::Response& response) //, const string& key HTTPClientSession& cs, 
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


bool WebSocketParser::connected() const
{
	return _headerState == 2;
}


} } // namespace scy::http



	

/*
WebSocket::Mode WebSocketParser::mode() const
{
	return mustMaskPayload() ? WebSocket::WS_CLIENT : WebSocket::WS_SERVER;
}
*/

	/*
int WebSocketParser::readNBytes(char* buffer, int bytes)
{
	//traceL("WebSocketParser", this) << "readNBytes: " << bytes << ": " << buffer.size() << endl;

	//memcpy(buffer, header + payloadOffset, n - payloadOffset);
	//_recvBuffer.size()
	//_recvBuffer.read((char*)buffer, bytes);
	
	return 0;
	if (bytes > buffer.size())
		throw Poco::Net::WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);

	buffer.read((char*)buffer, bytes);
	return bytes;

	int received = _socketBase->receiveFrame(reinterpret_cast<char*>(buffer), bytes);
	while (received < bytes)
	{
		int n = _socketBase->receiveFrame(reinterpret_cast<char*>(buffer) + received, bytes - received);
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
		traceL("WebSocketParser", this) << "RECV: 1111 received: " << received << endl;
		received += (payloadLength - received);
		traceL("WebSocketParser", this) << "RECV: 222 received: " << received << endl;
	}
	assert(payloadOffset < n);

	//received += payloadLength;	
	//buffer.consume(payloadLength);
	//int received = = readNBytes(reinterpret_cast<char*>(buffer), payloadLength);
	//buffer.consume();
	
	
	traceL("WebSocketParser", this) << "RECV: buffer.position: " << buffer.position() << endl;
	traceL("WebSocketParser", this) << "RECV: buffer.size: " << buffer.size() << endl;
	traceL("WebSocketParser", this) << "RECV: received: " << received << endl;
	traceL("WebSocketParser", this) << "RECV: payloadLength: " << payloadLength << endl;
	traceL("WebSocketParser", this) << "RECV: payloadOffset: " << payloadOffset << endl;
	
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
 
	_wsParser.init(ssl);
	_wsParser.update();

	emitConnect();
	*/


/*
SocketImpl* WebSocketParser::acceptConnection(SocketAddress& clientAddr)
{
	throw Poco::InvalidAccessException("Cannot acceptConnection() on a WebSocketParser");
}


void WebSocketParser::connect(const SocketAddress& address)
{
	throw Poco::InvalidAccessException("Cannot connect() a WebSocketParser");
}


void WebSocketParser::connect(const SocketAddress& address, const Poco::Timespan& timeout)
{
	throw Poco::InvalidAccessException("Cannot connect() a WebSocketParser");
}


void WebSocketParser::connectNB(const SocketAddress& address)
{
	throw Poco::InvalidAccessException("Cannot connectNB() a WebSocketParser");
}


void WebSocketParser::bind(const SocketAddress& address, bool reuseAddress)
{
	throw Poco::InvalidAccessException("Cannot bind() a WebSocketParser");
}


void WebSocketParser::bind6(const SocketAddress& address, bool reuseAddress, bool ipV6Only)
{
	throw Poco::InvalidAccessException("Cannot bind6() a WebSocketParser");
}


void WebSocketParser::listen(int backlog)
{
	throw Poco::InvalidAccessException("Cannot listen() on a WebSocketParser");
}


void WebSocketParser::close()
{
	_socketBase->close();
	reset();
}


void WebSocketParser::shutdownReceive()
{
	_socketBase->shutdownReceive();
}


void WebSocketParser::shutdownSend()
{
	_socketBase->shutdownSend();
}

	
void WebSocketParser::shutdown()
{
	_socketBase->shutdown();
}


int WebSocketParser::sendTo(const char* buffer, int length, const SocketAddress& address, int flags)
{
	throw Poco::InvalidAccessException("Cannot sendTo() on a WebSocketParser");
}


int WebSocketParser::receiveFrom(char* buffer, int length, SocketAddress& address, int flags)
{
	throw Poco::InvalidAccessException("Cannot receiveFrom() on a WebSocketParser");
}


void WebSocketParser::sendUrgent(unsigned char data)
{
	throw Poco::InvalidAccessException("Cannot sendUrgent() on a WebSocketParser");
}


bool WebSocketParser::secure() const
{
	return _socketBase->secure();
}


void WebSocketParser::setSendTimeout(const Poco::Timespan& timeout)
{
	_socketBase->setSendTimeout(timeout);
}


Poco::Timespan WebSocketParser::getSendTimeout()
{
	return _socketBase->getSendTimeout();
}


void WebSocketParser::setReceiveTimeout(const Poco::Timespan& timeout)
{
	_socketBase->setReceiveTimeout(timeout);
}


Poco::Timespan WebSocketParser::getReceiveTimeout()
{
	return _socketBase->getReceiveTimeout();
}
*/





/*
void WebSocketBase::connect(const Address& peerAddress) 
{
	if (!_context) 
		throw Exception("Cannot connect without WS context");

	traceL("WebSocketBase", this) << "Connecting to " << peerAddress << endl;
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


int WebSocketBase::available() const
{
	assert(initialized());
	return _wsParser.available();
}


void WebSocketBase::close()
{
	traceL("WebSocketBase", this) << "Close" << endl;
	TCPBase::close();
}


bool WebSocketBase::shutdown()
{
	traceL("WebSocketBase", this) << "Shutdown" << endl;
	try {
		// Try to gracefully shutdown the WS connection
		_wsParser.shutdown();
	}
	catch (...) {}
	return TCPBase::shutdown();
}
*/