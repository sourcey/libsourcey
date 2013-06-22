#include "ServerWebSocket.h"
#include "Sourcey/Util.h"
#include "Sourcey/Logger.h"

#include "Poco/Net/WebSocket.h"
#include "Poco/Net/WebSocketImpl.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/NetException.h"
#include "Poco/Buffer.h"
#include "Poco/MemoryStream.h"
#include "Poco/NullStream.h"
#include "Poco/BinaryWriter.h"
#include "Poco/SHA1Engine.h"
#include "Poco/MD5Engine.h"	
#include "Poco/Base64Encoder.h"
#include "Poco/String.h"
#include "Poco/Random.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Scy {
namespace Net {
	

const std::string ServerWebSocket::WEBSOCKET_GUID("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
const std::string ServerWebSocket::WEBSOCKET_VERSION("13");


/*
ServerWebSocket::ServerWebSocket(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) :
	Poco::Net::StreamSocket(accept(request, response))
{
} //, Poco::Net::HTTPServerResponse& response
*/

ServerWebSocket::ServerWebSocket(Poco::Net::StreamSocket& socket, Poco::Net::HTTPRequest& request) :
	Poco::Net::StreamSocket(accept(socket, request))
{
}


ServerWebSocket::ServerWebSocket(Poco::Net::WebSocketImpl* impl) :
	Poco::Net::StreamSocket(impl)
{
}


ServerWebSocket::~ServerWebSocket()
{
}


void ServerWebSocket::shutdown()
{
	shutdown(WS_NORMAL_CLOSE);
}


void ServerWebSocket::shutdown(Poco::UInt16 statusCode, const std::string& statusMessage)
{
	Poco::Buffer<char> buffer(statusMessage.size() + 2);
	Poco::MemoryOutputStream ostr(buffer.begin(), buffer.size());
	Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);
	writer << statusCode;
	writer.writeRaw(statusMessage);
	sendFrame(buffer.begin(), static_cast<int>(ostr.charsWritten()), FRAME_FLAG_FIN | FRAME_OP_CLOSE);
}


int ServerWebSocket::sendFrame(const void* buffer, int length, int flags)
{
	return static_cast<WebSocketImpl*>(impl())->sendBytes(buffer, length, flags);
}


int ServerWebSocket::receiveFrame(void* buffer, int length, int& flags)
{
	int n = static_cast<WebSocketImpl*>(impl())->receiveBytes(buffer, length, 0);
	flags = static_cast<WebSocketImpl*>(impl())->frameFlags();
	return n;
}


Poco::Net::WebSocketImpl* ServerWebSocket::accept(Poco::Net::StreamSocket& socket, Poco::Net::HTTPRequest& request) //HTTPServerRequest& request, HTTPServerResponse& response)
{
	LogTrace("ServerWebSocket") << "Accept" << endl;
	try
	{	
 		if (request.hasToken("Connection", "upgrade") && icompare(request.get("Upgrade", ""), "websocket") == 0)
		{		
			if (request.has("Sec-WebSocket-Key")) {				
				std::string version = request.get("Sec-WebSocket-Version", "");
				if (version.empty()) throw WebSocketException("Missing Sec-WebSocket-Version in handshake request", WS_ERR_HANDSHAKE_NO_VERSION);
				if (version != WEBSOCKET_VERSION) throw WebSocketException("Unsupported WebSocket version requested", version, WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION);
				std::string key = request.get("Sec-WebSocket-Key", "");
				Poco::trimInPlace(key);
				if (key.empty()) throw WebSocketException("Missing Sec-WebSocket-Key in handshake request", WS_ERR_HANDSHAKE_NO_KEY);
		
				Poco::Net::HTTPResponse response;
				response.setStatusAndReason(HTTPResponse::HTTP_SWITCHING_PROTOCOLS);
				response.set("Upgrade", "websocket");
				response.set("Connection", "Upgrade");
				response.set("Sec-WebSocket-Accept", computeAccept(key));
				response.setContentLength(0);	

				Poco::Net::SocketStream ss(socket);
				response.write(ss);
				ss.flush();

				return new WebSocketImpl(//socket.impl()
					static_cast<StreamSocketImpl*>(
					static_cast<HTTPServerRequestImpl&>(request).detachSocket().impl()
					), false);
			}
			else {
				std::string key1 = request.get("Sec-WebSocket-Key1", "");
				std::string key2 = request.get("Sec-WebSocket-Key2", "");
				Poco::trimInPlace(key1);
				Poco::trimInPlace(key2);
				if (key1.empty()) throw Poco::Net::WebSocketException("Missing Sec-WebSocket-Key1 in handshake request", Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY);
				if (key2.empty()) throw Poco::Net::WebSocketException("Missing Sec-WebSocket-Key1 in handshake request", Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY);

				Poco::MD5Engine engine;
				engine.update(
					Scy::Util::toString<UInt32>(ServerWebSocket::computeKeyPart(key1.data())) + 
					Scy::Util::toString<UInt32>(ServerWebSocket::computeKeyPart(key2.data())));
				std::string handshake = Poco::DigestEngine::digestToHex(engine.digest());
			
				Poco::Net::HTTPResponse response;
				response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_SWITCHING_PROTOCOLS);
				response.setReason("WebSocket Protocol Handshake");			
				response.set("Upgrade", "WebSocket");
				response.set("Connection", "Upgrade");			
				response.set("Sec-WebSocket-Location", "http://localhost:3000");
				response.set("Sec-WebSocket-Origin", "ws://localhost" + request.getURI());			
				response.setContentLength(16);
				
				char binary[16];
				ServerWebSocket::hexStringToBinary(handshake.data(), binary);
				Poco::Net::SocketStream ss(socket);
				response.write(ss);
				ss.write((const char*)&binary, 16);
				ss.flush();

				//response.send().flush();
				//response.sendBuffer(&binary, 16);
			
		
				//stringstream out;
				//response.write(out);
				//LogTrace("ServerWebSocket") << "SEND: " << out.str() << endl;

				return new Poco::Net::WebSocketImpl(//socket
					static_cast<Poco::Net::StreamSocketImpl*>(
					static_cast<Poco::Net::HTTPServerRequestImpl&>(request).detachSocket().impl()
					), false);
			}
			throw WebSocketException("Unknown WebSocket handshake", WS_ERR_NO_HANDSHAKE);
		}
		else throw WebSocketException("No WebSocket handshake", WS_ERR_NO_HANDSHAKE);	
	}
	catch (Poco::Net::WebSocketException& exc)
	{
		LogError("WebSocketRequestHandler") << "Error: " << exc.code() << ": " << exc.displayText() << endl;	
		Poco::Net::HTTPResponse response;		
		switch (exc.code())
		{
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
			response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
			// fallthrough
		case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.setContentLength(0);
			//response.send();
			break;
		default: exc.rethrow();
		}
			
		// Send error response
		Poco::Net::SocketStream ss(socket);
		response.write(ss);
		ss.flush();
	}
	throw Exception("Unable to create WebSocket");
}


std::string ServerWebSocket::createKey()
{
	Poco::Random rnd;
	std::ostringstream ostr;
	Poco::Base64Encoder base64(ostr);
	Poco::BinaryWriter writer(base64);
	writer << rnd.next() << rnd.next() << rnd.next() << rnd.next();
	base64.close();
	return ostr.str();
}


std::string ServerWebSocket::computeAccept(const std::string& key)
{
	std::string accept(key);
	accept += WEBSOCKET_GUID;
	Poco::SHA1Engine sha1;
	sha1.update(accept);
	Poco::DigestEngine::Digest d = sha1.digest();
	std::ostringstream ostr;
	Poco::Base64Encoder base64(ostr);
	base64.write(reinterpret_cast<const char*>(&d[0]), d.size());
	base64.close();
	return ostr.str();
}


} } // namespace Scy::Net
