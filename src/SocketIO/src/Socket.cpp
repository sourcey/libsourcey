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


#include "Sourcey/SocketIO/Socket.h"

/*
#include "Sourcey/SocketIO/Packet.h"
#include "Sourcey/HTTP/Transaction.h"

#include "Poco/Format.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;
*/


namespace Sourcey {
namespace SocketIO {
	

//SocketBase::SocketBase(Net::Reactor& reactor) :
//	WebSocket(reactor),
//	_secure(false)
//{
//}
//
//
//SocketBase::SocketBase(Net::Reactor& reactor, const Net::Address& srvAddr) :
//	WebSocket(reactor),
//	_srvAddr(srvAddr),
//	_secure(false)
//{
//}
//
//
//SocketBase::~SocketBase() 
//{
//	close();
//}
//
//
//void SocketBase::connect(const Net::Address& srvAddr)
//{	
//	{
//		FastMutex::ScopedLock lock(_mutex);
//		_srvAddr = srvAddr;
//	}
//	connect();
//}
//
//
//void SocketBase::connect()
//{
//	FastMutex::ScopedLock lock(_mutex);
//
//	assert(_srvAddr.valid());
//
//	if (isActive())
//		throw Exception("The SocketIO Socket is already active.");
//
//	_uri = "ws://" + _srvAddr.toString() + "/socket.io/1/";	
//	if (_secure)
//		_uri.setScheme("wss");
//
//	Log("debug") << "[SocketIO::Socket]	Connecting to " << _uri.toString() << endl;
//
//	if (sendHandshakeRequest()) {
//				
//		Timer::getDefault().start(TimerCallback<Socket>(this, &SocketBase::onHeartBeatTimer, 
//			(_heartBeatTimeout * .75) * 1000, 
//			(_heartBeatTimeout * .75) * 1000));
//	
//		// Receive SocketIO packets
//		registerPacketType<SocketIO::Packet>(10);
//		
//		// Initialize the websocket
//		_uri.setPath("/socket.io/1/websocket/" + _sessionID);
//		WebSocketBase::connect(_uri);
//	}
//}
//
//
//bool SocketBase::sendHandshakeRequest()
//{
//	// NOTE: No need for mutex lock because this method is called from connect()
//	
//	Log("trace") << "[SocketIO::Socket] Sending Handshake" << endl;
//	
//	
//	URI uri("http://" + _srvAddr.toString() + "/socket.io/1/");	
//	if (_secure)
//		uri.setScheme("https");
//
//	HTTP::Request* request = new HTTP::Request("POST", uri.toString());	
//	HTTP::Transaction transaction(request);
//	HTTP::Response& response = transaction.response();
//
//	// The server can respond in three different ways:
//    // 401 Unauthorized: If the server refuses to authorize the client to connect, 
//	//		based on the supplied information (eg: Cookie header or custom query components).
//    // 503 Service Unavailable: If the server refuses the connection for any reason (eg: overload).
//	// 200 OK: The handshake was successful.
//	if (!transaction.send())
//		throw Exception(format("SocketIO handshake failed: HTTP Error: %d %s", 
//			static_cast<int>(response.getStatus()), response.getReason()));		
//
//	Log("trace") << "[SocketIO::Socket] Handshake Response:" 
//		<< "\n\tStatus: " << response.getStatus()
//		<< "\n\tReason: " << response.getReason()
//		<< "\n\tResponse: " << response.body.str()
//		<< endl;
//
//	//if (status != 200)
//	//	throw Exception(format("SocketIO handshake failed with %d", status));	
//
//	// Parse the response response
//	StringList respData = Util::split(response.body.str(), ':', 4);
//	if (respData.size() < 4)
//		throw Exception(response.empty() ? 
//			"Invalid SocketIO handshake response." : format(
//			"Invalid SocketIO handshake response: %s", response.body.str()));
//	
//	_sessionID = respData[0];
//	_heartBeatTimeout = Util::atoi(respData[1]);
//	_connectionClosingTimeout = Util::atoi(respData[2]);
//	_protocols = Util::split(respData[3], ',');
//
//	// Check websockets are supported
//	bool wsSupported = false;
//	for (int i = 0; i < _protocols.size(); i++) {
//		Log("debug") << "[SocketIO::Socket] Supports Protocol: " << _protocols[i] << endl;	
//		if (_protocols[i] == "websocket") {
//			wsSupported = true;
//			break;
//		}
//	}
//
//	if (!wsSupported)
//		throw Exception("The SocketIO server does not support WebSockets");
//
//	return true;
//}
//
//
//void SocketBase::close()
//{			
//	Log("trace") << "[SocketIO::Socket] Closing" << endl;	
//
//	if (!isError())
//		Timer::getDefault().stop(TimerCallback<Socket>(this, &SocketBase::onHeartBeatTimer));
//	
//	WebSocketBase::close();
//
//	Log("trace") << "[SocketIO::Socket] Closing: OK" << endl;	
//}
//
//
//int SocketBase::sendConnect(const string& endpoint, const string& query)
//{
//	FastMutex::ScopedLock lock(_mutex);
//	// (1) Connect
//	// Only used for multiple sockets. Signals a connection to the endpoint. Once the server receives it, it's echoed back to the client.
//	// 
//	// Example, if the client is trying to connect to the endpoint /test, a message like this will be delivered:
//	// 
//	// '1::' [path] [query]
//	// Example:
//	// 
//	// 1::/test?my=param
//	string out = "1::";
//	if (!endpoint.empty())
//		out += "/" + endpoint;
//	if (!query.empty())
//		out += "?" + query;
//	return WebSocketBase::send(out.data(), out.size());
//}
//
//
//int SocketBase::send(SocketIO::Packet::Type type, const string& data, bool ack)
//{
//	Packet packet(type, data, ack);
//	return send(packet);
//}
//
//
//int SocketBase::send(const string& data, bool ack)
//{
//	Packet packet(data, ack);
//	return send(packet);
//}
//
//
//int SocketBase::send(const JSON::Value& data, bool ack)
//{
//	Packet packet(data, ack);
//	return send(packet);
//}
//
//
//int SocketBase::send(const SocketIO::Packet& packet)
//{
//	return WebSocketBase::send(packet);
//}
//
//
//int SocketBase::emit(const string& event, const JSON::Value& args, bool ack)
//{
//	Packet packet(event, args, ack);
//	return send(packet);
//}
//
//
//void SocketBase::onHeartBeatTimer(TimerCallback<Socket>&) 
//{
//	Log("trace") << "[SocketIO::Socket] Heart Beat Timer" << endl;
//	
//	if (isConnected())
//		sendHeartbeat();
//
//	// Try to reconnect if the connection was closed in error
//	else if (isError()) {	
//		Log("trace") << "[SocketIO::Socket] Attempting to reconnect" << endl;	
//		try {
//			connect();
//		} 
//		catch (Poco::Exception& exc) {			
//			Log("error") << "[SocketIO::Socket] Reconnection attempt failed: " << exc.displayText() << endl;
//		}	
//	}
//}
//
//
//int SocketBase::sendHeartbeat()
//{
//	Log("trace") << "[SocketIO::Socket] Heart Beat" << endl;
//	return WebSocketBase::send("2::", 3);
//}
//
//
//void SocketBase::setSecure(bool flag)
//{
//	FastMutex::ScopedLock lock(_mutex);
//	_secure = flag;
//}
//
//
//Net::IWebSocket* SocketBase::socket()
//{
//	FastMutex::ScopedLock lock(_mutex);
//	return _socket;
//}
//
//
//Poco::Net::NameValueCollection& SocketBase::httpHeaders()
//{
//	FastMutex::ScopedLock lock(_mutex);
//	return _httpHeaders;
//}
//
//
//string SocketBase::sessionID() const 
//{
//	FastMutex::ScopedLock lock(_mutex);
//	return _sessionID;
//}


} } // namespace Sourcey::SocketIO

	
	/*
	int status = 503;
	string response;
	
	// HTTPS
	if (_secure) {
		HTTPSClientSession s(_srvAddr.host().toString(), _srvAddr.port());
		HTTPRequest req(HTTPRequest::HTTP_POST, "/socket.io/1/");	
		s.sendRequest(req);
		HTTPResponse res;
		istream& rstr = s.receiveResponse(res);
		ostringstream rdata;
		StreamCopier::copyStream(rstr, rdata);
		status = res.getStatus();
		response = rdata.str();
	}

	// HTTP
	else {
		HTTPClientSession s(_srvAddr.host().toString(), _srvAddr.port());
		HTTPRequest req(HTTPRequest::HTTP_POST, "/socket.io/1/");	
		s.sendRequest(req);
		HTTPResponse res;
		istream& rstr = s.receiveResponse(res);
		ostringstream rdata;
		StreamCopier::copyStream(rstr, rdata);
		status = res.getStatus();
		response = rdata.str();
	}
	*/


	/*

	HTTPClientSession s(_srvAddr);
	HTTPRequest request(HTTPRequest::HTTP_POST, "/socket.io/1/");	
	s.sendRequest(request);
	HTTPResponse response;
	istream& rs = s.receiveResponse(response);
	StreamCopier::copyStream(rs, body);
	int status = response.getStatus();
	
	// Seems to be a strange bug where data copying from receiveBytes
	// into a iostream is invalid. This is causing Poco to throw
	// a NoMesageException when using the HTTPRequest object because 
	// the first byte == eof.
	// Copying into a string yields no such problem, so using 
	// string for now.

	StreamSocket socket;	
	socket.connect(_srvAddr);
	SocketStream ss(socket);
	
	// Request
	HTTPRequest request("POST", "/socket.io/1/");	
	for (NameValueCollection::ConstIterator it = _httpHeaders.begin(); it != _httpHeaders.end(); it++)
		request.set((*it).first, (*it).second);
	//request.setContentLength(0);
	request.write(ss);
	ss.flush();
	assert(ss.good());	
	
	// Response
	char buffer[1024];
	int size = socket.receiveBytes(buffer, sizeof(buffer));	
	string response(buffer, size);
	size_t pos = response.find(" "); pos++;
	int status = Util::atoi(response.substr(pos, response.find(" ", pos) + 1));
	pos = response.find("\r\n\r\n", pos); pos += 4;
	string body(response.substr(pos, response.length()));
	socket.shutdownSend();

	Log("debug") << "[SocketIO::Socket] Handshake:" 
		//<< "\n\tRequest: " << ss.str()
		<< "\n\tStatus: " << status
		<< "\n\tResponse: " << response
		<< "\n\tResponse Len: " << response.size()
		<< "\n\tBody: " << body
		<< endl;

	string response = transaction.response().body.str();
		*/