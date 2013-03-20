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


#include "Sourcey/SocketIO/Client.h"

/*
#include "Sourcey/SocketIO/Packet.h"
#include "Sourcey/HTTP/Transaction.h"

#include "Poco/Format.h"


using namespace Poco;
using namespace Poco::Net;
*/
using namespace std;


namespace Sourcey {
namespace SocketIO {


Client::Client(Net::IWebSocket& socket, Runner& runner) :
	_timer(new TimerTask(runner)),
	_socket(socket),
	_runner(runner)
{
}


Client::Client(Net::IWebSocket& socket, Runner& runner, const Net::Address& serverAddr) :
	_timer(new TimerTask(runner)),
	_serverAddr(serverAddr),
	_socket(socket),
	_runner(runner)
{
	connect();
}


Client::~Client() 
{
	close();
	_timer->destroy();
}


void Client::connect(const Net::Address& serverAddr)
{	
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_serverAddr = serverAddr;
	}
	connect();
}


void Client::connect()
{
	log("trace") << "SocketIO Connecting" << endl;

	reset();

	if (!serverAddr().valid())
		throw Exception("The SocketIO server address is not valid.");

	if (socket().isConnected())
		throw Exception("The SocketIO client is already connected.");
		
	setState(this, ClientState::Connecting);
	
	sendInitialRequest();

	Poco::FastMutex::ScopedLock lock(_mutex);

	// Initialize the websocket
	_socket.Connected += delegate(this, &Client::onSocketConnect);
	_socket.Online += delegate(this, &Client::onSocketOnline);
	_socket.Closed += delegate(this, &Client::onSocketClose);
	_socket.Data += delegate(this, &Client::onSocketData);
	
	ostringstream uri;
	uri << (_socket.transport() == Net::SSLTCP ? "wss://" : "ws://")
		<< _serverAddr.toString()
		<< "/socket.io/1/websocket/"
		<< _sessionID;
	_socket.connect(uri.str());

}


void Client::close()
{			
	log("trace") << "Closing" << endl;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		
		// Cancel the timer if connection manually closed
		_timer->Timeout -= delegate(this, &Client::onHeartBeatTimer);
		_timer->cancel();	

		_socket.Connected -= delegate(this, &Client::onSocketConnect);
		_socket.Online -= delegate(this, &Client::onSocketOnline);
		_socket.Closed -= delegate(this, &Client::onSocketClose);
		_socket.Data -= delegate(this, &Client::onSocketData);
		_socket.close();
	}
	onClose();

	log("trace") << "Closing: OK" << endl;	
}


void Client::sendInitialRequest()
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	log("trace") << "Sending Handshake" << endl;	
	
	ostringstream uri;
	uri << (_socket.transport() == Net::SSLTCP ? "https://" : "http://")
		<< _serverAddr.toString()
		<< "/socket.io/1/";

	HTTP::Request* request = new HTTP::Request("POST", uri.str());	
	HTTP::Transaction transaction(request);
	HTTP::Response& response = transaction.response();
	transaction.send();

	log("trace") << "SocketIO Handshake Response:" 
		<< "\n\tStatus: " << response.getStatus()
		<< "\n\tReason: " << response.getReason()
		<< "\n\tResponse: " << response.body.str()
		<< endl;
		
	// The server can respond in three different ways:
	// 401 Unauthorized: If the server refuses to authorize the client to connect, 
	//		based on the supplied information (eg: Cookie header or custom query components).
	// 503 Service Unavailable: If the server refuses the connection for any reason (eg: overload).
	// 200 OK: The handshake was successful.
	if (response.getStatus() != 200)
		throw Exception(Poco::format("SocketIO handshake failed: HTTP Error: %d %s", 
			static_cast<int>(response.getStatus()), response.getReason()));

	// Parse the response response
	StringList respData = Util::split(response.body.str(), ':', 4);
	if (respData.size() < 4)
		throw Exception(response.empty() ? 
			"Invalid SocketIO handshake response." : Poco::format(
			"Invalid SocketIO handshake response: %s", response.body.str()));
	
	_sessionID = respData[0];
	_heartBeatTimeout = Util::atoi(respData[1]);
	_connectionClosingTimeout = Util::atoi(respData[2]);
	_protocols = Util::split(respData[3], ',');

	// Check websockets are supported
	bool wsSupported = false;
	for (int i = 0; i < _protocols.size(); i++) {
		log("trace") << "Supports Protocol: " << _protocols[i] << endl;
		if (_protocols[i] == "websocket") {
			wsSupported = true;
			break;
		}
	}

	if (!wsSupported)
		throw Exception("The SocketIO server does not support WebSockets.");
}


int Client::sendConnect(const string& endpoint, const string& query)
{
	// (1) Connect
	// Only used for multiple sockets. Signals a connection to the endpoint. 
	// Once the server receives it, it's echoed back to the client.
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
	return socket().send(out.data(), out.size());
}


int Client::send(SocketIO::Packet::Type type, const string& data, bool ack)
{
	Packet packet(type, data, ack);
	return send(packet);
}


int Client::send(const string& data, bool ack)
{
	Packet packet(data, ack);
	return send(packet);
}


int Client::send(const JSON::Value& data, bool ack)
{
	Packet packet(data, ack);
	return send(packet);
}


int Client::send(const SocketIO::Packet& packet)
{
	return socket().send(packet);
}


int Client::emit(const string& event, const JSON::Value& args, bool ack)
{
	Packet packet(event, args, ack);
	return send(packet);
}


Transaction* Client::createTransaction(const SocketIO::Packet& request, long timeout)
{
	return new Transaction(*this, request, timeout);
}


int Client::sendHeartbeat()
{
	log("trace") << "Heart Beat" << endl;
	return socket().send("2::", 3);
}


Runner& Client::runner() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _runner;
}


Net::IWebSocket& Client::socket() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _socket;
}


string Client::sessionID() const 
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _sessionID;
}


Net::Address Client::serverAddr() const 
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _serverAddr;
}


string Client::error() const 
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _error;
}


bool Client::isOnline() const
{
	return stateEquals(ClientState::Online);
}


void Client::reset()
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_error = "";	
	_sessionID = "";	
	_heartBeatTimeout = 0;
	_connectionClosingTimeout = 0;
	_protocols.clear();
}


void Client::setError(const string& error)
{
	log("error") << "Error: " << error << std::endl;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_error = error;	
	}
	onClose();
	//setState(this, ClientState::Disconnected, error);
	//close();
}


void Client::onConnect()
{
	log("trace") << "Connected" << endl;	
			
	setState(this, ClientState::Connected);

	Poco::FastMutex::ScopedLock lock(_mutex);
	assert(_heartBeatTimeout);

	// Start the heartbeat timer if cancelled
	if (_timer->cancelled()) {	
		_timer->setTimeout((_heartBeatTimeout * .75) * 1000);
		_timer->setInterval((_heartBeatTimeout * .75) * 1000);
		_timer->Timeout += delegate(this, &Client::onHeartBeatTimer);
		_timer->start();
	}
}


void Client::onOnline()
{
	log("trace") << "Online" << endl;	
	setState(this, ClientState::Online);
}


void Client::onClose()
{
	log("trace") << "Disconnected" << endl;
	setState(this, ClientState::Disconnected, error());

	// Keep trying to reconnect via timer callbacks
}


void Client::onPacket(SocketIO::Packet& packet)
{
	log("trace") << "On Packet" << endl;		
	PacketDispatcher::dispatch(this, packet);
}

	
void Client::onHeartBeatTimer(void*)
{
	log("trace") << "On Heart Beat Timer" << endl;
	
	if (socket().isConnected())
		sendHeartbeat();

	// Try to reconnect if the connection was closed in error
	else if (socket().isError()) {	
		log("info") << "Attempting to reconnect" << endl;	
		try {
			connect();
		} 
		catch (Poco::Exception& exc) {			
			log("error") << "Reconnection attempt failed: " << exc.displayText() << endl;
		}	
	}
}


void Client::onSocketConnect(void*)
{
	onConnect();
}


void Client::onSocketOnline(void*)
{
	onOnline();
}


void Client::onSocketClose(void*)
{
	if (!socket().error().empty())
		setError(socket().error());
	else
		onClose();
}


void Client::onSocketData(void*, Buffer& data, const Net::Address&)
{	
	SocketIO::Packet packet;
	if (packet.read(data))
		onPacket(packet);
	else
		log("warn") << "Unable to create SocketIO packet." << endl;	
}



} } // namespace Sourcey::SocketIO





		/*
		Timer::getDefault().stop(TimerCallback<Client>(this, &Client::onHeartBeatTimer));
		if (_timer) {
			_timer->Timeout -= delegate(this, &Client::onHeartBeatTimer);
			_timer->destroy();	
			_timer = NULL;
		}
		*/
	
	
	
	/*
void Client::onHeartBeatTimer(TimerCallback<Socket>&) 
	Timer::getDefault().start(TimerCallback<Client>(this, &Client::onHeartBeatTimer, 
		(_heartBeatTimeout * .75) * 1000, 
		(_heartBeatTimeout * .75) * 1000));

	_timer = new TimerTask(
		(_heartBeatTimeout * .75) * 1000, 
		(_heartBeatTimeout * .75) * 1000);
	//if (!_timer) {
	//}
	//if (_timer->cancelled())
		*/
/*


	//_socket.registerPacketType<SocketIO::Packet>(10);

void Client::onError() 
{
	log("warn") << "On Error" << endl;
}
*/

	
	//if (!_socket) {
	//	_socket = createSocket();
	//}

	//if (!isError()) {
	// If the socket was closed in error we keep trying to reconnect.
		//Timer::getDefault().stop(TimerCallback<SocketBase>(this, &SocketBase::onHeartBeatTimer));
	//}	
	
	//if (_socket) {
		//delete _socket;
		//_socket = NULL;
	//}

	//if (!_socket)
	//	throw Exception("The SocketIO WebSocket pointer is NULL.");

	//Poco::URI uri("http://" + _serverAddr.toString() + "/socket.io/1/");	
	//if (_socket.transport() == Net::SSLTCP)
	//	uri.setScheme("https");	
	//Poco::URI uri("ws://" + _serverAddr.toString() + "/socket.io/1/");	
	//if (socket.transport() == Net::SSLTCP)
	//	uri.setScheme("wss");
	//uri.setPath("/socket.io/1/websocket/" + _sessionID);



	//Timer::getDefault().start(TimerCallback<SocketBase>(this, &SocketBase::onHeartBeatTimer, 
	//	(_heartBeatTimeout * .75) * 1000, 
	//	(_heartBeatTimeout * .75) * 1000));		


//SocketBase::SocketBase(Net::Reactor& reactor) :
//	WebSocket(reactor),
//	_secure(false)
//{
//}
//
//
//SocketBase::SocketBase(Net::Reactor& reactor, const Net::Address& serverAddr) :
//	WebSocket(reactor),
//	_serverAddr(serverAddr),
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
//void SocketBase::connect(const Net::Address& serverAddr)
//{	
//	{
//		FastMutex::ScopedLock lock(_mutex);
//		_serverAddr = serverAddr;
//	}
//	connect();
//}
//
//
//void SocketBase::connect()
//{
//	FastMutex::ScopedLock lock(_mutex);
//
//	assert(_serverAddr.valid());
//
//	if (isActive())
//		throw Exception("The SocketIO Socket is already active.");
//
//	_uri = "ws://" + _serverAddr.toString() + "/socket.io/1/";	
//	if (_secure)
//		_uri.setScheme("wss");
//
//	LogDebug() << "[SocketIO::Socket]	Connecting to " << _uri.toString() << endl;
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
//	LogTrace() << "[SocketIO::Socket] Sending Handshake" << endl;
//	
//	
//	URI uri("http://" + _serverAddr.toString() + "/socket.io/1/");	
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
//	LogTrace() << "[SocketIO::Socket] Handshake Response:" 
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
//		LogDebug() << "[SocketIO::Socket] Supports Protocol: " << _protocols[i] << endl;	
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
//	LogTrace() << "[SocketIO::Socket] Closing" << endl;	
//
//	if (!isError())
//		Timer::getDefault().stop(TimerCallback<Socket>(this, &SocketBase::onHeartBeatTimer));
//	
//	WebSocketBase::close();
//
//	LogTrace() << "[SocketIO::Socket] Closing: OK" << endl;	
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
//	LogTrace() << "[SocketIO::Socket] Heart Beat Timer" << endl;
//	
//	if (isConnected())
//		sendHeartbeat();
//
//	// Try to reconnect if the connection was closed in error
//	else if (isError()) {	
//		LogTrace() << "[SocketIO::Socket] Attempting to reconnect" << endl;	
//		try {
//			connect();
//		} 
//		catch (Poco::Exception& exc) {			
//			LogError() << "[SocketIO::Socket] Reconnection attempt failed: " << exc.displayText() << endl;
//		}	
//	}
//}
//
//
//int SocketBase::sendHeartbeat()
//{
//	LogTrace() << "[SocketIO::Socket] Heart Beat" << endl;
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




	/*
	int status = 503;
	string response;
	
	// HTTPS
	if (_secure) {
		HTTPSClientSession s(_serverAddr.host().toString(), _serverAddr.port());
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
		HTTPClientSession s(_serverAddr.host().toString(), _serverAddr.port());
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

	HTTPClientSession s(_serverAddr);
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
	socket.connect(_serverAddr);
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

	LogDebug() << "[SocketIO::Socket] Handshake:" 
		//<< "\n\tRequest: " << ss.str()
		<< "\n\tStatus: " << status
		<< "\n\tResponse: " << response
		<< "\n\tResponse Len: " << response.size()
		<< "\n\tBody: " << body
		<< endl;

	string response = transaction.response().body.str();
		*/