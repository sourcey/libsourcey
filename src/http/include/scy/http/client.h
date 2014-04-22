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


#ifndef SCY_HTTP_Client_H
#define SCY_HTTP_Client_H


#include "scy/net/socket.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/sslsocket.h"
#include "scy/net/network.h"
#include "scy/http/connection.h"
#include "scy/http/websocket.h"
#include "scy/timer.h"


namespace scy { 
namespace http {

	
class ProgressSignal: public Signal<const double&>
{
public:
	void* sender;
	UInt64 current;
	UInt64 total;

	ProgressSignal() :	
		sender(nullptr), current(0), total(0) {}

	double progress() const 
	{
		return (current / (total * 1.0)) * 100;
	}

	void update(int nread) 
	{
		current += nread;
		//assert(current <= total);
		emit(sender ? sender : this, progress());
	}
};


class Client;
class ClientConnection: public Connection
{
public:
	typedef std::shared_ptr<ClientConnection> Ptr;

    ClientConnection(const URL& url, const net::Socket::Ptr& socket = nullptr);
		// Create a standalone connection with the given host.

    virtual ~ClientConnection();

	virtual void send();
		// Sends the HTTP request.
		//
		// Calls connect() internally if the socket is not
		// already connecting or connected. The actual request 
		// will be sent when the socket is connected.
				
	virtual void send(http::Request& req);
		// Sends the given HTTP request.
		// The given request will overwrite the internal HTTP
		// request object.
		//
		// Calls connect() internally if the socket is not
		// already connecting or connected. The actual request 
		// will be sent when the socket is connected.
			
	virtual int send(const char* data, std::size_t len, int flags = 0);
	//virtual int send(const std::string& buf, int flags = 0);
	//virtual void sendData(const char* buf, std::size_t len); //, int flags = 0
	//virtual void sendData(const std::string& buf); //, int flags = 0
		// Sends raw data to the peer.
		// Calls send() internally.
	
	virtual void close();
		// Forcefully closes the HTTP connection.
		
	virtual void setReadStream(std::ostream* os);
		// Set the output stream for writing response data to.
		// The stream pointer is managed internally,
		// and will be freed along with the connection.		
		
	template<class T>
	T* readStream()
		// Returns the cast read stream pointer or nullptr.
	{
		return dynamic_cast<T*>(_readStream);
	}
		
	void* opaque;
		// Optional unmanaged client data pointer.
	
	//
	/// Internal callbacks

	virtual void onHeaders();
	virtual void onPayload(const MutableBuffer& buffer);
	virtual void onMessage();
	virtual void onComplete();
	virtual void onClose();

	//
	/// Status signals

	NullSignal Connect;						// Fires when the client socket is connected
	Signal<Response&> Headers;				// Fires when the response HTTP header has been received
	Signal<const Response&> Complete;		// Always on success or error response
	ProgressSignal IncomingProgress;		// Notifies on download progress
	ProgressSignal OutgoingProgress;		// Notifies on upload progress

protected:		
	virtual void connect();
		// Connects to the server endpoint.
		// All sent data is buffered until the connection is made.
					
	http::Client* client();
	http::Message* incomingHeader();	
	http::Message* outgoingHeader();
	
	void onSocketConnect();
	void onHostResolved(void*, const net::DNSResult& result);
	
protected:	
	URL _url;
	std::ostream* _readStream;
	std::vector<std::string> _outgoingBuffer;
	bool _connect;
	bool _complete;
};


typedef std::vector<ClientConnection::Ptr> ClientConnectionPtrVec;


//
// Client Connection Adapter
//


class ClientAdapter: public ConnectionAdapter
{
public:
    ClientAdapter(ClientConnection& connection) : 
		ConnectionAdapter(connection, HTTP_RESPONSE)
	{
	}
};


//
// HTTP Connection Helpers
//
	
	
template<class ConnectionT>
inline ClientConnection::Ptr createConnectionT(const URL& url, http::Client* client = nullptr, uv::Loop* loop = uv::defaultLoop())
{
	ClientConnection::Ptr conn;

	if (url.scheme() == "http") {			
		//conn = std::make_shared<ConnectionT>(url, std::make_shared<net::TCPSocket>(loop));
		conn = std::shared_ptr<ConnectionT>(
			new ConnectionT(url, std::make_shared<net::TCPSocket>(loop)), 
				deleter::Deferred<ConnectionT>());
	}
	else if (url.scheme() == "https") {
		//conn = std::make_shared<ConnectionT>(url, std::make_shared<net::SSLSocket>(loop));
		conn = std::shared_ptr<ConnectionT>(
			new ConnectionT(url, std::make_shared<net::SSLSocket>(loop)), 
				deleter::Deferred<ConnectionT>());
	}
	else if (url.scheme() == "ws") {
		//conn = std::make_shared<ConnectionT>(url, std::make_shared<net::TCPSocket>(loop));
		conn = std::shared_ptr<ConnectionT>(
			new ConnectionT(url, std::make_shared<net::TCPSocket>(loop)), 
				deleter::Deferred<ConnectionT>());
		conn->replaceAdapter(new ws::ConnectionAdapter(*conn, ws::ClientSide)); 
		//replaceAdapter(new ws::ws::ConnectionAdapter(*conn, ws::ClientSide));
	}
	else if (url.scheme() == "wss") {
		//conn = std::make_shared<ConnectionT>(url, std::make_shared<net::SSLSocket>(loop));
		conn = std::shared_ptr<ConnectionT>(
			new ConnectionT(url, std::make_shared<net::SSLSocket>(loop)), 
				deleter::Deferred<ConnectionT>());
		conn->replaceAdapter(new ws::ConnectionAdapter(*conn, ws::ClientSide));
	}
	else
		throw std::runtime_error("Unknown connection type for URL: " + url.str());

	if (client && conn)
		client->addConnection(conn);

	return conn;
}


inline ClientConnection::Ptr createConnection(const URL& url, http::Client* client = nullptr, uv::Loop* loop = uv::defaultLoop())
{
	return createConnectionT<ClientConnection>(url, client, loop);
}


//
// HTTP Client
//


class Client
{
public:
	Client();
	virtual ~Client();

	static Client& instance();
		// Returns the default HTTP Client singleton.

	void shutdown();
		// Shutdown the Client, and close all connections.

	template<class ConnectionT>
	ClientConnection::Ptr createConnectionT(const URL& url, uv::Loop* loop = uv::defaultLoop())
	{
		return http::createConnectionT<ConnectionT>(url, this, loop);
	}

	ClientConnection::Ptr createConnection(const URL& url, uv::Loop* loop = uv::defaultLoop())
	{
		return http::createConnectionT<ClientConnection>(url, this, loop);
	}

	virtual void addConnection(ClientConnection::Ptr conn);
	virtual void removeConnection(ClientConnection* conn);

	NullSignal Shutdown;

protected:		
	//void onConnectionTimer(void*);
	void onConnectionClose(void*);

	friend class ClientConnection;
	
	ClientConnectionPtrVec _connections;
	//Timer _timer;
};


#if 0
class SecureClientConnection: public ClientConnection
{
public:
    SecureClientConnection(Client* client, const URL& url) : //, const net::Address& address
		ClientConnection(client, url, net::SSLSocket()) //, address
	{
	}

	virtual ~SecureClientConnection() 
	{
	}
};


class WebSocketClientConnection: public ClientConnection
{
public:
    WebSocketClientConnection(Client* client, const URL& url) : //, const net::Address& address
		ClientConnection(client, url) //, address
	{
		socket().replaceAdapter(new ws::ConnectionAdapter(*this, ws::ClientSide));	//&socket(), &request(), request(), request()
	}

	virtual ~WebSocketClientConnection() 
	{
	}
};


class WebSocketSecureClientConnection: public ClientConnection
{
public:
    WebSocketSecureClientConnection(Client* client, const URL& url) : //, const net::Address& address
		ClientConnection(client, url, net::SSLSocket()) //, address
	{
		socket().replaceAdapter(new ws::ConnectionAdapter(*this, ws::ClientSide)); //(&socket(), &request()
	}

	virtual ~WebSocketSecureClientConnection() 
	{
	}
};
#endif


} } // namespace scy::http


#endif