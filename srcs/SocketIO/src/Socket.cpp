//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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
#include "Sourcey/SocketIO/Packet.h"
#include "Sourcey/Logger.h"

#include "Poco/Format.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Sourcey {
namespace SocketIO {
	

Socket::Socket()
{
}


Socket::Socket(const Net::Address& srvAddr) : 
	_srvAddr(srvAddr)
{
}


Socket::~Socket() 
{
	close();
}


void Socket::connect(const Net::Address& srvAddr)
{	
	{
		FastMutex::ScopedLock lock(_mutex);
		_srvAddr = srvAddr;
	}
	connect();
}


void Socket::connect()
{
	FastMutex::ScopedLock lock(_mutex);

	assert(_srvAddr.valid());

	if (isActive())
		throw Exception("The SocketIO Socket is already active.");

	_uri = "ws://" + _srvAddr.toString() + "/socket.io/1/";	
	Log("debug") << "[SocketIO::Socket] Initializing on " << _uri.toString() << endl;

	if (sendInitialRequest()) {
				
		Timer::getDefault().start(TimerCallback<Socket>(this, &Socket::onHeartBeatTimer, 
			(_heartBeatTimeout * .75) * 1000, 
			(_heartBeatTimeout * .75) * 1000));
	
		// Receive SocketIO packets
		registerPacketType<SocketIO::Packet>(10);
		
		// Initialize the websocket
		_uri.setPath("/socket.io/1/websocket/" + _sessionID);
		WebSocket::connect(_uri);

	}
}


bool Socket::sendInitialRequest()
{	
	// Seems to be a strange bug where data copying from receiveBytes
	// into a std::iostream is invalid. This is causing Poco to throw
	// a NoMesageException when using the HTTPRequest object because 
	// the first byte == eof.
	// Copying into a std::string yields no such problem, so using 
	// std::string for now.
		
	StreamSocket socket;	
	socket.connect(_srvAddr);
	SocketStream ss(socket);
	
	// Request
	HTTPRequest request("POST", "/socket.io/1/");	
	for (NameValueCollection::ConstIterator it = _httpHeaders.begin(); it != _httpHeaders.end(); it++)
		request.set((*it).first, (*it).second);
	request.write(ss);
	ss.flush();
	assert(ss.good());	
	
	// Response
	char buffer[512];
	int size = socket.receiveBytes(buffer, sizeof(buffer));	
	string response(buffer, size);
	size_t pos = response.find(" "); pos++;
	int status = Util::atoi(response.substr(pos, response.find(" ", pos) + 1));
	pos = response.find("\r\n\r\n", pos); pos += 4;
	string body(response.substr(pos, response.length()));
	socket.shutdownSend();

	Log("debug") << "[SocketIO::Socket] Handshake Response:" 
		<< "\n\tStatus: " << status
		<< "\n\tResponse: " << response
		<< "\n\tBody: " << body
		<< endl;

	// The server can respond in three different ways:
    // 401 Unauthorized: If the server refuses to authorize the client to connect, based on the supplied information (eg: Cookie header or custom query components).
    // 503 Service Unavailable: If the server refuses the connection for any reason (eg: overload).
	// 200 OK: The handshake was successful.
	if (status != 200)
		throw Exception(format("SocketIO handshake failed with %d", status));
					
	// Parse the response response
	StringList respData = Util::split(body, ':', 4);
	if (respData.size() < 4)
		throw Exception(format("Invalid response: %s", body));
	
	_sessionID = respData[0];
	_heartBeatTimeout = Util::atoi(respData[1]);
	_connectionClosingTimeout = Util::atoi(respData[2]);
	_protocols = Util::split(respData[3], ',');

	// Check websockets are supported
	bool wsSupported = false;
	for (int i = 0; i < _protocols.size(); i++) {
		Log("debug") << "[SocketIO::Socket] Supports Protocol: " << _protocols[i] << endl;	
		if (_protocols[i] == "websocket") {
			wsSupported = true;
			break;
		}
	}

	if (!wsSupported)
		throw Exception("The SocketIO server does not support WebSockets");

	return true;
}


void Socket::close()
{			
	Log("debug") << "[SocketIO::Socket] Closing" << endl;	

	if (!isError())
		Timer::getDefault().stop(TimerCallback<Socket>(this, &Socket::onHeartBeatTimer));	
	
	WebSocket::close();
}


int Socket::sendConnect(const string& endpoint, const string& query)
{
	FastMutex::ScopedLock lock(_mutex);
	// (1) Connect
	// Only used for multiple sockets. Signals a connection to the endpoint. Once the server receives it, it's echoed back to the client.
	// 
	// Example, if the client is trying to connect to the endpoint /test, a message like this will be delivered:
	// 
	// '1::' [path] [query]
	// Example:
	// 
	// 1::/test?my=param
	string out = "1::";
	if (!endpoint.empty())
		out += "/" + endpoint;
	if (!query.empty())
		out += "?" + query;
	return WebSocket::send(out.data(), out.size());
}


int Socket::send(SocketIO::Packet::Type type, const string& data, bool ack)
{
	Packet packet(type, data, ack);
	return send(packet);
}


int Socket::send(const std::string& data, bool ack)
{
	Packet packet(data, ack);
	return send(packet);
}


int Socket::send(const JSON::Value& data, bool ack)
{
	Packet packet(data, ack);
	return send(packet);
}


int Socket::send(const SocketIO::Packet& packet)
{
	return WebSocket::send(packet);
}


int Socket::emit(const string& event, const JSON::Value& args, bool ack)
{
	Packet packet(event, args, ack);
	return send(packet);
}


void Socket::onHeartBeatTimer(TimerCallback<Socket>&) 
{
	Log("trace") << "[SocketIO::Socket] Heart Beat Timer" << endl;
	
	if (isConnected())
		sendHeartbeat();

	// Try to reconnect if the connection was closed in error
	else if (isError()) {	
		Log("trace") << "[SocketIO::Socket] Attempting to reconnect" << endl;	
		try {
			connect();
		} 
		catch (Poco::Exception& e) 
		{			
			Log("error") << "[SocketIO::Socket] Reconnection attempt failed: " << e.displayText() << endl;
		}	
	}
}


int Socket::sendHeartbeat()
{
	Log("trace") << "[SocketIO::Socket] Heart Beat" << endl;
	return WebSocket::send("2::", 3);
}


Poco::Net::NameValueCollection& Socket::httpHeaders()
{
	FastMutex::ScopedLock lock(_mutex);
	return _httpHeaders;
}


std::string Socket::sessionID() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _sessionID;
}


} } // namespace Sourcey::SocketIO