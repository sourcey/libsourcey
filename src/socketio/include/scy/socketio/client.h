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


class Client: 
	public Stateful<ClientState>, 
	public net::SocketAdapter, 
	public PacketSignal
{
public:
	Client(const net::Socket::Ptr& socket);
	Client(const net::Socket::Ptr& socket, const std::string& host, UInt16 port);
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
	http::ws::WebSocket& ws();
	std::string sessionID() const;	
	scy::Error error() const;
		
	bool isOnline() const;

	bool wasOnline() const;
		// Returns true if the client was in the Online state.
		// Useful for delegates handling the Closed state.

	//virtual const char* className() const { return "SocketIOClient"; }

protected:
	virtual void setError(const scy::Error& error);

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
	
	virtual void onSocketConnect();
	virtual void onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress);
	virtual void onSocketError(const scy::Error& error);
	virtual void onSocketClose();
	//virtual void onSocketConnect();
	//virtual void onSocketRecv(void*, const MutableBuffer& buffer, const net::Address& peerAddress);
	//virtual void onSocketError(void*, const Error& error);
	//virtual void onSocketClose(void*);

	virtual void onHeartBeatTimer(void*);

protected:
	//mutable Mutex	_mutex;
	
	//uv::Loop* _loop;
	Timer _timer;
	scy::Error _error;
	std::vector<std::string> _protocols;
	std::string _sessionID;
	std::string _host;
	UInt16 _port;
	http::ws::WebSocket _ws;
	int	_heartBeatTimeout;
	int	_connectionClosingTimeout;
	bool _wasOnline;
	//bool _closing;
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
