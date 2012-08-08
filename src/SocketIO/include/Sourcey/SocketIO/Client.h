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


#ifndef SOURCEY_SOCKETIO_Client_H
#define SOURCEY_SOCKETIO_Client_H


#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/WebSocket.h"
#include "Sourcey/SocketIO/Packet.h"
#include "Sourcey/SocketIO/Transaction.h"
#include "Sourcey/HTTP/Transaction.h"
#include "Sourcey/JSON/JSON.h"

#include "Poco/Format.h"
#include "Poco/URI.h"
#include "Poco/Net/NameValueCollection.h"


namespace Sourcey {
namespace SocketIO {


struct ClientState: public State 
{
	enum Type 
	{
		None				= 0x00,
		Connecting			= 0x01,
		Connected			= 0x04,
		//Handshaking			= 0x08,
		Online				= 0x10,
		Disconnected		= 0x20
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:			return "None";
		case Connecting:	return "Connecting";
		case Connected:		return "Connected";
		//case Handshaking:	return "Handshaking";
		case Online:		return "Online";
		case Disconnected:	return "Disconnected";
		default: assert(false);
		}
		return "undefined"; 
	};
};

	
// ---------------------------------------------------------------------
//
class Client: public StatefulSignal<ClientState>, public PacketDispatcher, public ILoggable
{
public:
	Client(Net::IWebSocket& socket, Runner& runner);
	Client(Net::IWebSocket& socket, Runner& runner, const Net::Address& serverAddr);
	virtual ~Client();
	
	virtual void connect(const Net::Address& serverAddr);
	virtual void connect();
	virtual void close();

	virtual int send(const std::string& data, bool ack = false); 
		// Sends a Message packet

	virtual int send(const JSON::Value& data, bool ack = false); 
		// Sends a JSON packet

	virtual int emit(const std::string& event, const JSON::Value& data, bool ack = false);
		// Sends an Event packet

	virtual int send(SocketIO::Packet::Type type, const std::string& data, bool ack = false);
		// Creates and sends packet from the given data

	virtual int send(const SocketIO::Packet& packet);
		// Sends the given packet
	
	virtual int sendConnect(const std::string& endpoint = "", const std::string& query = "");
		// Sends a Connect packet
	
	virtual Transaction* createTransaction(const SocketIO::Packet& request, long timeout = 10000);
		// Creates a packet transaction

	virtual Runner& runner() const;
	virtual Net::IWebSocket& socket() const;
	virtual Net::Address serverAddr() const;
	virtual std::string sessionID() const;	
	virtual std::string error() const;
		
	virtual bool isOnline() const;

	const char* className() const { return "SocketIOClient"; }

protected:
	virtual int sendHeartbeat();
	virtual void sendInitialRequest();
	virtual void setError(const std::string& error);

	virtual void reset();
		/// Resets variables and data at the beginning  
		/// and end of each session.

	virtual void onConnect();
	virtual void onOnline();
	virtual void onClose();
	virtual void onPacket(SocketIO::Packet& packet);
	
	virtual void onHeartBeatTimer(void*);
	virtual void onSocketConnect(void*);
	virtual void onSocketOnline(void*);
	virtual void onSocketClose(void*);
	virtual void onSocketData(void*, Buffer& data, const Net::Address&);

protected:
	mutable Poco::FastMutex	_mutex;
	
	Net::IWebSocket& _socket;
	Runner&			_runner;
	Net::Address	_serverAddr;
	StringList		_protocols;
	std::string		_sessionID;
	std::string		_error;
	int				_heartBeatTimeout;
	int				_connectionClosingTimeout;
	TimerTask*      _timer;
};


// ---------------------------------------------------------------------
//
template <class WebSocketBaseT>
class ClientBase: public Client
{
public:
	ClientBase(Net::Reactor& reactor, Runner& runner = Runner::getDefault()) :
		_socket(reactor),
		Client(_socket, runner)
	{
	}

	ClientBase(Net::Reactor& reactor, const Net::Address& serverAddr, Runner& runner = Runner::getDefault()) :
		_socket(reactor),
		Client(_socket, runner, serverAddr)
	{
	}

protected:
	WebSocketBaseT _socket;
};


// ---------------------------------------------------------------------
//
typedef SocketIO::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< Poco::Net::StreamSocket, Net::TCP, Net::IWebSocket >
		> 
	> 
> TCPClient;


// ---------------------------------------------------------------------
//
typedef SocketIO::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< Poco::Net::SecureStreamSocket, Net::SSLTCP, Net::IWebSocket >
		> 
	> 
> SSLClient;


} } // namespace Sourcey::SocketIO


#endif //  SOURCEY_SOCKETIO_Client_H





	//virtual void onError();//void*);

/*
// ---------------------------------------------------------------------
//
typedef SocketIO::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::TCPContext, Net::IWebSocket >  //SocketIO::ISocket 
		> 
	> 
> Client;


// ---------------------------------------------------------------------
//
typedef SocketIO::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::SSLContext, SocketIO::ISocket > 
		> 
	> 
> SSLClient;
*/



	
	//Signal<SocketIO::Packet> Packet;
		/// Signals data received by the socket.
	//virtual Net::IWebSocket* createSocket() = 0;
		// Creates the underlying socket instance
	//Net::Reactor&	_reactor;


// ---------------------------------------------------------------------
//
//template <class WebSocketBaseT>
//class ClientBase: public WebSocketBaseT
//{
//public:
//	ClientBase(Net::Reactor& reactor) :
//		WebSocketBaseT(reactor),
//		_timer(NULL)
//	{
//	}
//
//
//	ClientBase(Net::Reactor& reactor, const Net::Address& serverAddr) :
//		WebSocketBaseT(reactor),
//		_serverAddr(serverAddr),
//		_timer(NULL)
//	{
//	}
//
//
//	virtual ~ClientBase() 
//	{
//		close();		
//
//		if (_timer)
//			_timer->destroy();	
//	}
//
//
//	virtual void connect(const Net::Address& serverAddr)
//	{	
//		{
//			Poco::FastMutex::ScopedLock lock(_mutex);
//			_serverAddr = serverAddr;
//		}
//		/*SocketBase::*/connect();
//	}
//
//
//	virtual void connect()
//	{
//		Log("trace", this) << "SocketIO Connecting" << endl;
//
//		Poco::FastMutex::ScopedLock lock(_mutex);
//
//		assert(_serverAddr.valid());
//
//		if (isActive())
//			throw Exception("The SocketIO Socket is already active.");
//
//		Log("debug", this) << "Connecting to " << _uri.toString() << std::endl;
//
//		sendInitialRequest();
//
//		// Initialize the websocket
//
//		_uri = "ws://" + _serverAddr.toString() + "/socket.io/1/";	
//		if (_socket->transport() == Net::SSLTCP)
//			_uri.setScheme("wss");
//		_uri.setPath("/socket.io/1/websocket/" + _sessionID);
//		_socket->registerPacketType<SocketIO::Packet>(10);
//		_socket->connect();
//	}
//
//
//	virtual void sendInitialRequest()
//	{
//		// NOTE: No need for mutex lock because this method is called from connect()
//	
//		Log("trace", this) << "Sending Handshake" << std::endl;	
//	
//		Poco::URI uri("http://" + _serverAddr.toString() + "/socket.io/1/");	
//		if (_socket->transport() == Net::SSLTCP)
//			uri.setScheme("https");
//
//		HTTP::Request* request = new HTTP::Request("POST", uri.toString());	
//		HTTP::Transaction transaction(request);
//		HTTP::Response& response = transaction.response();
//		transaction.send();
//
//		Log("trace", this) << "SocketIO Handshake Response:" 
//			<< "\n\tStatus: " << response.getStatus()
//			<< "\n\tReason: " << response.getReason()
//			<< "\n\tResponse: " << response.body.str()
//			<< std::endl;
//		
//		// The server can respond in three different ways:
//		// 401 Unauthorized: If the server refuses to authorize the client to connect, 
//		//		based on the supplied information (eg: Cookie header or custom query components).
//		// 503 Service Unavailable: If the server refuses the connection for any reason (eg: overload).
//		// 200 OK: The handshake was successful.
//		if (response.getStatus() != 200)
//			throw Exception(Poco::format("SocketIO handshake failed: HTTP Error: %d %s", 
//				static_cast<int>(response.getStatus()), response.getReason()));
//
//		// Parse the response response
//		StringList respData = Util::split(response.body.str(), ':', 4);
//		if (respData.size() < 4)
//			throw Exception(response.empty() ? 
//				"Invalid SocketIO handshake response." : Poco::format(
//				"Invalid SocketIO handshake response: %s", response.body.str()));
//	
//		_sessionID = respData[0];
//		_heartBeatTimeout = Util::atoi(respData[1]);
//		_connectionClosingTimeout = Util::atoi(respData[2]);
//		_protocols = Util::split(respData[3], ',');
//
//		// Check websockets are supported
//		bool wsSupported = false;
//		for (int i = 0; i < _protocols.size(); i++) {
//			Log("debug", this) << "Supports Protocol: " << _protocols[i] << std::endl;	
//			if (_protocols[i] == "websocket") {
//				wsSupported = true;
//				break;
//			}
//		}
//
//		if (!wsSupported)
//			throw Exception("The SocketIO server does not support WebSockets.");
//	}
//
//
//	virtual void close()
//	{			
//		Log("trace", this) << "Closing" << std::endl;	
//
//		// If the socket was closed in error we keep trying to reconnect.
//		if (!isError()) {
//			//Timer::getDefault().stop(TimerCallback<SocketBase>(this, &SocketBase::onHeartBeatTimer));
//			assert(_timer);
//			_timer->stop();	
//		}
//	
//		_socket->close();
//
//		Log("trace", this) << "Closing: OK" << std::endl;	
//	}
//
//
//	virtual int sendConnect(const std::string& endpoint, const std::string& query)
//	{
//		Poco::FastMutex::ScopedLock lock(_mutex);
//		// (1) Connect
//		// Only used for multiple sockets. Signals a connection to the endpoint. Once the server receives it, it's echoed back to the client.
//		// 
//		// Example, if the client is trying to connect to the endpoint /test, a message like this will be delivered:
//		// 
//		// '1::' [path] [query]
//		// Example:
//		// 
//		// 1::/test?my=param
//		std::string out = "1::";
//		if (!endpoint.empty())
//			out += "/" + endpoint;
//		if (!query.empty())
//			out += "?" + query;
//		return _socket->send(out.data(), out.size());
//	}
//
//
//	virtual int send(SocketIO::Packet::Type type, const std::string& data, bool ack)
//	{
//		Packet packet(type, data, ack);
//		return send(packet);
//	}
//
//
//	virtual int send(const std::string& data, bool ack)
//	{
//		Packet packet(data, ack);
//		return send(packet);
//	}
//
//
//	virtual int send(const JSON::Value& data, bool ack)
//	{
//		Packet packet(data, ack);
//		return send(packet);
//	}
//
//
//	virtual int send(const SocketIO::Packet& packet)
//	{
//		return _socket->send(packet);
//	}
//
//
//	virtual int emit(const std::string& event, const JSON::Value& args, bool ack)
//	{
//		Packet packet(event, args, ack);
//		return send(packet);
//	}
//
//	
//	//virtual void onHeartBeatTimer(TimerCallback<SocketBase>&) 
//	virtual void onHeartBeatTimer(void*)
//	{
//		Log("trace", this) << "On Heart Beat Timer" << std::endl;
//	
//		if (isConnected())
//			sendHeartbeat();
//
//		// Try to reconnect if the connection was closed in error
//		else if (isError()) {	
//			Log("trace", this) << "Attempting to reconnect" << std::endl;	
//			try {
//				connect();
//			} 
//			catch (Poco::Exception& exc) {			
//				Log("error", this) << "Reconnection attempt failed: " << exc.displayText() << std::endl;
//			}	
//		}
//	}
//
//
//	virtual int sendHeartbeat()
//	{
//		Log("trace", this) << "Heart Beat" << std::endl;
//		return _socket->send("2::", 3);
//	}
//
//
//	virtual std::string sessionID() const 
//	{
//		Poco::FastMutex::ScopedLock lock(_mutex);
//		return _sessionID;
//	}
//	
//
//	virtual const char* className() const { return "SocketIO::SocketBase"; }
//
//	
//protected:
//	virtual void onConnect()
//	{
//		Log("trace", this) << "SocketIO Connected" << endl;
//		//Timer::getDefault().start(TimerCallback<SocketBase>(this, &SocketBase::onHeartBeatTimer, 
//		//	(_heartBeatTimeout * .75) * 1000, 
//		//	(_heartBeatTimeout * .75) * 1000));		
//
//		// Start the heartbeat timer
//		if (!_timer) {
//			_timer = new TimerTask(
//				(_heartBeatTimeout * .75) * 1000, 
//				(_heartBeatTimeout * .75) * 1000);
//			_timer->Timeout += delegate(this, &Socket::onHeartBeatTimer);
//		}
//		if (!_timer->running())
//			_timer->start();
//
//		_socket->onConnect();
//	}	
//	
//
//protected:
//	mutable Poco::FastMutex	_mutex;
//	
//	Net::Address	_serverAddr;
//	StringList		_protocols;
//	std::string		_sessionID;
//	int				_heartBeatTimeout;
//	int				_connectionClosingTimeout;
//	IWebSocket*     _socket;
//	TimerTask*      _timer;
//};


/*
// ---------------------------------------------------------------------
//
typedef SocketIO::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::TCPContext, SocketIO::ISocket > 
		> 
	> 
> Client;

typedef SocketIO::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::SSLContext, SocketIO::ISocket > 
		> 
	> 
> SSLClient;
*/

/*
	//virtual void setSecure(bool flag) = 0;
		// Enables secure wss:// connection when true.
	
	virtual Poco::Net::NameValueCollection& httpHeaders() = 0;
	Poco::Net::NameValueCollection _httpHeaders;
	virtual Poco::Net::NameValueCollection& httpHeaders()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _httpHeaders;
	}


	virtual Net::IWebSocket* socket()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _socket;
	}
	*/
//typedef SocketIO::SocketBase< Net::SocketBase< ::SSLContext, SocketIO::ISocket> > SSLSocket;
 //Net::SocketBase< ::TCPContext, SocketIO::ISocket>
	//bool			_secure;
	//Net::IWebSocket* _socket;


	/*
	void setSecure(bool flag)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_secure = flag;
	}
	*/

	/*
	//IWebSocket(Reactor& reactor) {};
	//IWebSocket(Reactor& reactor, const Poco::URI& uri) {};
	virtual ~ISocket() = 0;
	
	virtual void connect(const Poco::URI& uri) = 0;
	virtual void connect() = 0;
	virtual void close() = 0;
	
	virtual int send(const char* data, int size) = 0;
	virtual int send(const IPacket& packet) = 0;
	
	virtual void setProtocol(const std::string& proto) = 0;
	virtual void setCookie(const std::string& cookie) = 0;

	SocketBase(Net::ISocket& socket, Net::Reactor& reactor);
	SocketBase(Net::ISocket& socket, Net::Reactor& reactor, const Net::Address& serverAddr);
	*/



//typedef SocketIO::SocketBase<Net::SSLContext, SocketIO::ISocket> SSLSocket;


	/*
class Socket: public SocketIO::SocketBase<Net::TCPContext, SocketIO::ISocket>
{
public:
	Socket(Reactor& reactor) : 
		SocketBase<SocketT, TransportT>(reactor)
		SocketBase(*this)
	{
	}


	//Socket(const Socket& r) : 
	//	SocketBase<SocketT>(r)
	//{
	//}
	
	
	virtual ~Socket()
	{
	}

};

, public SocketBase
template <class SocketBaseT, Net::TransportProtocol TransportT>
class Socket: public SocketIO::SocketBase<SocketT, TransportT>, public SocketBase
{
public:
	Socket(Reactor& reactor) : 
		SocketBase<SocketT, TransportT>(reactor)
		SocketBase(*this)
	{
	}


	//Socket(const Socket& r) : 
	//	SocketBase<SocketT>(r)
	//{
	//}
	
	
	virtual ~Socket()
	{
	}

};



	SocketBase(Net::ISocket& socket, Net::Reactor& reactor);
	SocketBase(Net::ISocket& socket, Net::Reactor& reactor, const Net::Address& serverAddr);
	virtual ~SocketBase();
	
	void connect(const Net::Address& serverAddr);	
	void connect();
	void close();
	
	virtual int sendConnect(const std::string& endpoint = "", const std::string& query = "");

	virtual int send(SocketIO::Packet::Type type, const std::string& data, bool ack = false);	
	virtual int send(const std::string& data, bool ack = false); // Message packet
	virtual int send(const JSON::Value& data, bool ack = false); // JSON packet
	virtual int send(const SocketIO::Packet& packet);
	virtual int emit(const std::string& event, const JSON::Value& data, bool ack = false);
		// Sends an Event packet

	virtual void setSecure(bool flag);
		// Enables secure wss:// connection when true.
	
	//Net::IWebSocket* socket();
	Poco::Net::NameValueCollection& httpHeaders();
	std::string sessionID() const;

protected:
	virtual int sendHeartbeat();
	virtual void onHeartBeatTimer(TimerCallback<Socket>& timer);
	virtual bool sendInitialRequest();
	*/




/*
template <class SocketT, Net::TransportProtocol TransportT>
class Socket: public Net::WebSocketBase<SocketT, TransportT>, public SocketBase
{
public:
	Socket(Reactor& reactor) : 
		SocketBase<SocketT, TransportT>(reactor)
		SocketBase(*this)
	{
	}


	//Socket(const Socket& r) : 
	//	SocketBase<SocketT>(r)
	//{
	//}
	
	
	virtual ~Socket()
	{
	}

};
*/