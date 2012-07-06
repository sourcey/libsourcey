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


#ifndef SOURCEY_SOCKETIO_Socket_H
#define SOURCEY_SOCKETIO_Socket_H


#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/SocketBase.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/Net/StatefulSocket.h"
#include "Sourcey/Net/WebSocket.h"
#include "Sourcey/SocketIO/Packet.h"
#include "Sourcey/SocketIO/Transaction.h"
#include "Sourcey/HTTP/Transaction.h"
#include "Sourcey/Timer.h"
#include "Sourcey/JSON/JSON.h"

#include "Poco/Format.h"
#include "Poco/URI.h"
#include "Poco/Net/NameValueCollection.h"


namespace Sourcey {
namespace SocketIO {

	
// ---------------------------------------------------------------------
//
class ISocket: public Net::ISocket
{
public:
	virtual ~ISocket() {};
	
	virtual void connect(const Net::Address& srvAddr) = 0;
	virtual void connect() = 0;
	virtual void close() = 0;
	
	virtual int sendConnect(const std::string& endpoint = "", const std::string& query = "") = 0;

	virtual int send(SocketIO::Packet::Type type, const std::string& data, bool ack = false) = 0;
	virtual int send(const std::string& data, bool ack = false) = 0; // Message packet
	virtual int send(const JSON::Value& data, bool ack = false) = 0; // JSON packet
	virtual int send(const SocketIO::Packet& packet) = 0;
	virtual int emit(const std::string& event, const JSON::Value& data, bool ack = false) = 0;
		// Sends an Event packet

	virtual std::string sessionID() const = 0;

protected:
	virtual int sendHeartbeat() = 0;
	virtual bool sendInitialRequest() = 0;
};


// ---------------------------------------------------------------------
//
template <class WebSocketBaseT>
class SocketBase: public WebSocketBaseT
{
public:
	SocketBase(Net::Reactor& reactor) :
		WebSocketBaseT(reactor)//,
		//_secure(false)
	{
	}


	SocketBase(Net::Reactor& reactor, const Net::Address& srvAddr) :
		WebSocketBaseT(reactor),
		_srvAddr(srvAddr)//,
		//_secure(false)
	{
	}


	virtual ~SocketBase() 
	{
		close();
	}


	void connect(const Net::Address& srvAddr)
	{	
		Log("trace", this) << "Connecting 1" << endl;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_srvAddr = srvAddr;
		}
		/*SocketBase::*/connect();
	}


	void connect()
	{
		Log("trace", this) << "Connecting 2" << endl;

		Poco::FastMutex::ScopedLock lock(_mutex);

		assert(_srvAddr.valid());

		if (isActive())
			throw Exception("The SocketIO Socket is already active.");

		_uri = "ws://" + _srvAddr.toString() + "/socket.io/1/";	
		if (transport() == Net::SSLTCP)
			_uri.setScheme("wss");

		Log("debug", this) << "Connecting to " << _uri.toString() << std::endl;

		if (sendInitialRequest()) {
				
			// Receive SocketIO packets
			registerPacketType<SocketIO::Packet>(10);
		
		
			Log("debug", this) << "Connecting on WS..." << std::endl;

			// Initialize the websocket
			_uri.setPath("/socket.io/1/websocket/" + _sessionID);
			WebSocketBaseT::connect();
		}
	}


	bool sendInitialRequest()
	{
		// NOTE: No need for mutex lock because this method is called from connect()
	
		Log("trace", this) << "Sending Handshake" << std::endl;	
	
		Poco::URI uri("http://" + _srvAddr.toString() + "/socket.io/1/");	
		if (transport() == Net::SSLTCP)
			uri.setScheme("https");

		HTTP::Request* request = new HTTP::Request("POST", uri.toString());	
		HTTP::Transaction transaction(request);
		HTTP::Response& response = transaction.response();

		// The server can respond in three different ways:
		// 401 Unauthorized: If the server refuses to authorize the client to connect, 
		//		based on the supplied information (eg: Cookie header or custom query components).
		// 503 Service Unavailable: If the server refuses the connection for any reason (eg: overload).
		// 200 OK: The handshake was successful.
		if (!transaction.send())
			throw Exception(Poco::format("SocketIO handshake failed: HTTP Error: %d %s", 
				static_cast<int>(response.getStatus()), response.getReason()));		

		Log("trace", this) << "Handshake Response:" 
			<< "\n\tStatus: " << response.getStatus()
			<< "\n\tReason: " << response.getReason()
			<< "\n\tResponse: " << response.body.str()
			<< std::endl;

		//if (status != 200)
		//	throw Exception(format("SocketIO handshake failed with %d", status));	

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
			Log("debug", this) << "Supports Protocol: " << _protocols[i] << std::endl;	
			if (_protocols[i] == "websocket") {
				wsSupported = true;
				break;
			}
		}

		if (!wsSupported)
			throw Exception("The SocketIO server does not support WebSockets");

		return true;
	}


	void close()
	{			
		Log("trace", this) << "Closing" << std::endl;	

		if (!isError())
			Timer::getDefault().stop(TimerCallback<SocketBase>(this, &SocketBase::onHeartBeatTimer));
	
		WebSocketBaseT::close();

		Log("trace", this) << "Closing: OK" << std::endl;	
	}


	virtual int sendConnect(const std::string& endpoint, const std::string& query)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		// (1) Connect
		// Only used for multiple sockets. Signals a connection to the endpoint. Once the server receives it, it's echoed back to the client.
		// 
		// Example, if the client is trying to connect to the endpoint /test, a message like this will be delivered:
		// 
		// '1::' [path] [query]
		// Example:
		// 
		// 1::/test?my=param
		std::string out = "1::";
		if (!endpoint.empty())
			out += "/" + endpoint;
		if (!query.empty())
			out += "?" + query;
		return WebSocketBaseT::send(out.data(), out.size());
	}


	virtual int send(SocketIO::Packet::Type type, const std::string& data, bool ack)
	{
		Packet packet(type, data, ack);
		return send(packet);
	}


	virtual int send(const std::string& data, bool ack)
	{
		Packet packet(data, ack);
		return send(packet);
	}


	virtual int send(const JSON::Value& data, bool ack)
	{
		Packet packet(data, ack);
		return send(packet);
	}


	virtual int send(const SocketIO::Packet& packet)
	{
		return WebSocketBaseT::send(packet);
	}


	virtual int emit(const std::string& event, const JSON::Value& args, bool ack)
	{
		Packet packet(event, args, ack);
		return send(packet);
	}


	virtual void onHeartBeatTimer(TimerCallback<SocketBase>&) 
	{
		Log("trace", this) << "On Heart Beat Timer" << std::endl;
	
		if (isConnected())
			sendHeartbeat();

		// Try to reconnect if the connection was closed in error
		else if (isError()) {	
			Log("trace", this) << "Attempting to reconnect" << std::endl;	
			try {
				connect();
			} 
			catch (Poco::Exception& exc) {			
				Log("error", this) << "Reconnection attempt failed: " << exc.displayText() << std::endl;
			}	
		}
	}


	virtual int sendHeartbeat()
	{
		Log("trace", this) << "Heart Beat" << std::endl;
		return WebSocketBaseT::send("2::", 3);
	}


	virtual std::string sessionID() const 
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _sessionID;
	}
	

	virtual const char* className() const { return "SocketIO::SocketBase"; }

	
protected:
	virtual void onConnect()
	{
		Log("trace", this) << "SocketIO Socket Connected" << endl;
		Timer::getDefault().start(TimerCallback<SocketBase>(this, &SocketBase::onHeartBeatTimer, 
			(_heartBeatTimeout * .75) * 1000, 
			(_heartBeatTimeout * .75) * 1000));
		WebSocketBaseT::onConnect();
	}	
	

protected:
	mutable Poco::FastMutex	_mutex;
	
	Net::Address	_srvAddr;
	StringList		_protocols;
	std::string		_sessionID;
	int				_heartBeatTimeout;
	int				_connectionClosingTimeout;
};


// ---------------------------------------------------------------------
//
typedef SocketIO::SocketBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::TCPContext, SocketIO::ISocket> 
		> 
	> 
> Socket;

typedef SocketIO::SocketBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< ::SSLContext, SocketIO::ISocket> 
		> 
	> 
> SSLSocket;


} } // namespace Sourcey::SocketIO


#endif //  SOURCEY_SOCKETIO_Socket_H


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
	SocketBase(Net::ISocket& socket, Net::Reactor& reactor, const Net::Address& srvAddr);
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
	SocketBase(Net::ISocket& socket, Net::Reactor& reactor, const Net::Address& srvAddr);
	virtual ~SocketBase();
	
	void connect(const Net::Address& srvAddr);	
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