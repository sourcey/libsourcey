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


#ifndef SCY_SocketIO_Client_H
#define SCY_SocketIO_Client_H


#include "scy/socketio/packet.h"
#include "scy/socketio/transaction.h"
#include "scy/http/websocket.h"
#include "scy/json/json.h"

//#include "scy/application.h"
//#include "Poco/Format.h"
//#include "Poco/URI.h"
#include "scy/collection.h"


namespace scy {
namespace sockio {


struct ClientState: public State 
{
	enum Type 
	{
		None		= 0x00,
		Connecting	= 0x01,
		Connected	= 0x02,
		Online		= 0x04,
		Error		= 0x08
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:			return "None";
		case Connecting:	return "Connecting";
		case Connected:		return "Connected";
		case Online:		return "Online";
		case Error:			return "Error";
		default: assert(false);
		}
		return "undefined"; 
	};
};


class Client: public Stateful<ClientState>, public PacketSignal
{
public:
	Client(net::SocketBase* socket);
	Client(net::SocketBase* socket, const std::string& host, UInt16 port);
	virtual ~Client();
	
	virtual void connect(const std::string& host, UInt16 port);
	virtual void connect();
	virtual void close();

	virtual int send(const std::string& data, bool ack = false); 
		// Sends a Message packet

	virtual int send(const json::Value& data, bool ack = false); 
		// Sends a JSON packet

	virtual int emit(const std::string& event, const json::Value& data, bool ack = false);
		// Sends an Event packet

	virtual int send(sockio::Packet::Type type, const std::string& data, bool ack = false);
		// Creates and sends packet from the given data

	virtual int send(const sockio::Packet& packet);
		// Sends the given packet
	
	virtual int sendConnect(const std::string& endpoint = "", const std::string& query = "");
		// Sends a Connect packet
	
	virtual Transaction* createTransaction(const sockio::Packet& request, long timeout = 10000);
		// Creates a packet transaction

	//uv::Loop* loop();
	http::WebSocket& socket();
	std::string sessionID() const;	
	Error error() const;
		
	bool isOnline() const;

	bool wasOnline() const;
		// Returns true if the client was in the Online state.
		// Useful for delegates handling the Closed state.

	virtual const char* className() const { return "SocketIOClient"; }

protected:
	virtual void setError(const Error& error);

	virtual void reset();
		// Resets variables and data at the beginning  
		// and end of each session.

	virtual int sendHeartbeat();
	
	virtual void sendHandshakeRequest();
	virtual void onHandshakeResponse(void*, const http::Response& response);

	virtual void onConnect();
	virtual void onOnline();
	virtual void onClose();
	virtual void onPacket(sockio::Packet& packet);
	
	virtual void onSocketConnect(void*);
	virtual void onSocketRecv(void*, net::SocketPacket& packet);
	virtual void onSocketError(void*, const Error& error);
	virtual void onSocketClose(void*);

	virtual void onHeartBeatTimer(void*);

protected:
	//mutable Mutex	_mutex;
	
	//uv::Loop* _loop;
	Error _error;
	std::vector<std::string> _protocols;
	std::string _sessionID;
	std::string _host;
	UInt16 _port;
	http::WebSocket _socket;
	int	_heartBeatTimeout;
	int	_connectionClosingTimeout;
	bool _wasOnline;
	bool _closing;
	Timer _timer;
};


//
// TCP Client
//


Client* createTCPClient(uv::Loop* loop = uv::defaultLoop());

class TCPClient: public Client
{
public:
	TCPClient(uv::Loop* loop = uv::defaultLoop());
};


//
// SSL Client
//


Client* createSSLClient(uv::Loop* loop = uv::defaultLoop());

class SSLClient: public Client
{
public:
	SSLClient(uv::Loop* loop = uv::defaultLoop());
};


} } // namespace scy::sockio


#endif //  SCY_SocketIO_Client_H





/*
	//net::Address _serverAddr;
	//net::Socket _socket;
	//virtual net::Socket& socket();
	//virtual net::Address serverAddr() const;
	
	//Error _error;
	TCPClient(const std::string& host, UInt16 port, uv::Loop* loop = uv::defaultLoop()) :
		Client(new net::TCPBase, const std::string& host, UInt16 port, loop)//,
		//_socket(loop)
	{
	}

// ---------------------------------------------------------------------
//
template <class WebSocketBaseT>
class ClientBase: public Client
{
public:
	ClientBase(uv::Loop* loop = uv::defaultLoop()) :
		Client(_socket, loop)//,
		//_socket(loop)
	{
	}

	ClientBase(const net::Address& serverAddr, uv::Loop* loop = uv::defaultLoop()) :
		Client(_socket, serverAddr, loop)//,
		//_socket(loop)
	{
	}

protected:
	WebSocketBaseT _socket;
};


// ---------------------------------------------------------------------
//
typedef sockio::ClientBase<http::WebSocket> TCPClient;

	//virtual void onSocketConnect(void*);


// ---------------------------------------------------------------------
//
typedef sockio::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< Poco::Net::StreamSocket, Net::TCP, http::WebSocket >
		> 
	> 
> TCPClient;


// ---------------------------------------------------------------------
//
typedef sockio::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< Poco::Net::SecureStreamSocket, Net::SSLTCP, http::WebSocket >
		> 
	> 
> SSLClient;
*/


	//virtual void onError();//void*);

/*
// ---------------------------------------------------------------------
typedef sockio::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::TCPContext, http::WebSocket >  //sockio::ISocket 
		> 
	> 
> Client;


// ---------------------------------------------------------------------
typedef sockio::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::SSLContext, sockio::ISocket > 
		> 
	> 
> SSLClient;
*/



	
	//Signal<sockio::Packet> Packet;
		// Signals data received by the socket.
	//virtual http::WebSocket* createSocket() = 0;
		// Creates the underlying socket instance
	//Net::Reactor&	_reactor;


// ---------------------------------------------------------------------
//template <class WebSocketBaseT>
//class ClientBase: public WebSocketBaseT
//{
//public:
//	ClientBase(Net::Reactor& reactor) :
//		WebSocketBaseT(reactor),
//		_timer(null)
//	{
//	}
//
//
//	ClientBase(const net::Address& serverAddr) :
//		WebSocketBaseT(reactor),
//		_serverAddr(serverAddr),
//		_timer(null)
//	{
//	}
//
//
//	virtual ~ClientBase() 
//	{
//		close();		
//
//		if (_timer)
//			_timer.destroy();	
//	}
//
//
//	virtual void connect(const net::Address& serverAddr)
//	{	
//		{
//			//Mutex::ScopedLock lock(_mutex);
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
//		//Mutex::ScopedLock lock(_mutex);
//
//		assert(_serverAddr.valid());
//
//		if (isActive())
//			throw std::runtime_error("The SocketIO Socket is already active.");
//
//		Log("debug", this) << "Connecting to " << _uri.toString() << std::endl;
//
//		sendHandshakeRequest();
//
//		// Initialize the websocket
//
//		_uri = "ws://" + _serverAddr.toString() + "/socket.io/1/";	
//		if (_socket->transport() == Net::SSLTCP)
//			_uri.setScheme("wss");
//		_uri.setPath("/socket.io/1/websocket/" + _sessionID);
//		_socket->registerPacketType<sockio::Packet>(10);
//		_socket->connect();
//	}
//
//
//	virtual void sendHandshakeRequest()
//	{
//		// NOTE: No need for mutex lock because this method is called from connect()
//	
//		Log("trace", this) << "Send Handshake" << std::endl;	
//	
//		Poco::URI uri("http://" + _serverAddr.toString() + "/socket.io/1/");	
//		if (_socket->transport() == Net::SSLTCP)
//			uri.setScheme("https");
//
//		http::Request* request = new http::Request("POST", uri.toString());	
//		http::Transaction transaction(request);
//		http::Response& response = transaction.response();
//		transaction.send();
//
//		Log("trace", this) << "SocketIO Handshake Response:" 
//			<< "\n\tStatus: " << response.getStatus()
//			<< "\n\tReason: " << response.getReason()
//			<< "\n\tResponse: " << response.body.str()
//			<< std::endl;
//		
//		// The server can respond in three different ways:
//		// 401 NotAuthorized: If the server refuses to authorize the client to connect, 
//		//		based on the supplied information (eg: Cookie header or custom query components).
//		// 503 Service Unavailable: If the server refuses the connection for any reason (eg: overload).
//		// 200 OK: The handshake was successful.
//		if (response.getStatus() != 200)
//			throw std::runtime_error(Poco::format("SocketIO handshake failed: HTTP Error: %d %s", 
//				static_cast<int>(response.getStatus()), response.getReason()));
//
//		// Parse the response response
//		std::vector<std::string> respData = util::split(response.body.str(), ':', 4);
//		if (respData.size() < 4)
//			throw std::runtime_error(response.empty() ? 
//				"Invalid SocketIO handshake response." : Poco::format(
//				"Invalid SocketIO handshake response: %s", response.body.str()));
//	
//		_sessionID = respData[0];
//		_heartBeatTimeout = util::strtoi<UInt32>(respData[1]);
//		_connectionClosingTimeout = util::strtoi<UInt32>(respData[2]);
//		_protocols = util::split(respData[3], ',');
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
//			throw std::runtime_error("The SocketIO server does not support WebSockets.");
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
//			_timer.stop();	
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
//		//Mutex::ScopedLock lock(_mutex);
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
//		return _socket->send(out.c_str(), out.size());
//	}
//
//
//	virtual int send(sockio::Packet::Type type, const std::string& data, bool ack)
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
//	virtual int send(const json::Value& data, bool ack)
//	{
//		Packet packet(data, ack);
//		return send(packet);
//	}
//
//
//	virtual int send(const sockio::Packet& packet)
//	{
//		return _socket->send(packet);
//	}
//
//
//	virtual int emit(const std::string& event, const json::Value& args, bool ack)
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
//			catch (std::exception& exc) {			
//				Log("error", this) << "Reconnection attempt failed: " << exc.what() << std::endl;
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
//		//Mutex::ScopedLock lock(_mutex);
//		return _sessionID;
//	}
//	
//
//	virtual const char* className() const { return "sockio::SocketBase"; }
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
//			_timer.Timeout += delegate(this, &Socket::onHeartBeatTimer);
//		}
//		if (!_timer.running())
//			_timer.start();
//
//		_socket->onConnect();
//	}	
//	
//
//protected:
//	mutable Mutex	_mutex;
//	
//	net::Address	_serverAddr;
//	std::vector<std::string>		_protocols;
//	std::string		_sessionID;
//	int				_heartBeatTimeout;
//	int				_connectionClosingTimeout;
//	IWebSocket*     _socket;
//	Timer      _timer;
//};


/*
// ---------------------------------------------------------------------
typedef sockio::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::TCPContext, sockio::ISocket > 
		> 
	> 
> Client;

typedef sockio::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::SSLContext, sockio::ISocket > 
		> 
	> 
> SSLClient;
*/

/*
	//virtual void setSecure(bool flag) = 0;
		// Enables secure wss:// connection when true.
	
	virtual KVCollection& httpHeaders() = 0;
	KVCollection _httpHeaders;
	virtual KVCollection& httpHeaders()
	{
		//Mutex::ScopedLock lock(_mutex);
		return _httpHeaders;
	}


	virtual http::WebSocket* socket()
	{
		//Mutex::ScopedLock lock(_mutex);
		return _socket;
	}
	*/
//typedef sockio::SocketBase< Net::SocketBase< ::SSLContext, sockio::ISocket> > SSLSocket;
 //Net::SocketBase< ::TCPContext, sockio::ISocket>
	//bool			_secure;
	//http::WebSocket* _socket;


	/*
	void setSecure(bool flag)
	{
		//Mutex::ScopedLock lock(_mutex);
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
	SocketBase(Net::ISocket& socket, / Net::Reactor& reactor, const net::Address& serverAddr);
	*/



//typedef sockio::SocketBase<Net::SSLContext, sockio::ISocket> SSLSocket;


	/*
class Socket: public sockio::SocketBase<Net::TCPContext, sockio::ISocket>
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
class Socket: public sockio::SocketBase<SocketT, TransportT>, public SocketBase
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
	SocketBase(Net::ISocket& socket, // Net::Reactor& reactor, /const net::Address& serverAddr);
	virtual ~SocketBase();
	
	void connect(const net::Address& serverAddr);	
	void connect();
	void close();
	
	virtual int sendConnect(const std::string& endpoint = "", const std::string& query = "");

	virtual int send(sockio::Packet::Type type, const std::string& data, bool ack = false);	
	virtual int send(const std::string& data, bool ack = false); // Message packet
	virtual int send(const json::Value& data, bool ack = false); // JSON packet
	virtual int send(const sockio::Packet& packet);
	virtual int emit(const std::string& event, const json::Value& data, bool ack = false);
		// Sends an Event packet

	virtual void setSecure(bool flag);
		// Enables secure wss:// connection when true.
	
	//http::WebSocket* socket();
	KVCollection& httpHeaders();
	std::string sessionID() const;

protected:
	virtual int sendHeartbeat();
	virtual void onHeartBeatTimer(TimerCallback<Socket>& timer);
	virtual bool sendHandshakeRequest();
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