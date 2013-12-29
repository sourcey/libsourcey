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

	
class TransferSignal : public Signal<const double&>
{
public:
	void* sender;
	UInt64 current;
	UInt64 total;

	TransferSignal() :	
		sender(nullptr), current(0), total(0) {}

	double progress() const {
		return (current / (total * 1.0)) * 100;
	}

	void update(int nread) {
		current += nread;
		assert(current <= total);
		emit(sender ? sender : this, progress());
	}
};


class Client;
class ClientConnection: public Connection
{
public:
    ClientConnection(const URL& url,
		const net::Socket& socket = net::TCPSocket());
		// Create a standalone connection with the given host.
	
    ClientConnection(Client* client, const URL& url, 
		const net::Socket& socket = net::TCPSocket());
		// Create a managed connection with the given host.

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
			
	virtual void sendData(const char* buf, std::size_t len); //, int flags = 0
	virtual void sendData(const std::string& buf); //, int flags = 0
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
	virtual void onClose();

	//
	/// Status signals

	NullSignal Connect;						// Fires when the client socket is connected
	Signal<Response&> Headers;				// Fires when the response HTTP header has been received
	Signal<const Response&> Complete;		// Fires on success or error response	
	TransferSignal IncomingProgress;		// Notifies on download progress
	TransferSignal OutgoingProgress;		// Notifies on upload progress

protected:
    virtual ~ClientConnection();
		
	virtual void connect();
		// Connects to the server endpoint.
		// All sent data is buffered until the connection is made.
					
	http::Client* client();
	http::Message* incomingHeader();	
	http::Message* outgoingHeader();
	
	void onHostResolved(void*, const net::DNSResult& result);
	void onSocketConnect(void*);
	void onClientShutdown(void*);
	
protected:	
	http::Client* _client;
	URL _url;
	std::ostream* _readStream;
	std::vector<std::string> _outgoingBuffer;
	TransferSignal _incomingProgress;
	TransferSignal _outgoingProgress;
	bool _complete;
};


typedef std::vector<ClientConnection*> ClientConnectionList;
	

ClientConnection* createConnection(const URL& url, uv::Loop* loop = uv::defaultLoop());
	// Create a ClientConnection object.
	// The underlying ConnectionAdapter will be guessed
	// based on the URL scheme.


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


#if 0

// -------------------------------------------------------------------
//
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


// -------------------------------------------------------------------
//
class WebSocketClientConnection: public ClientConnection
{
public:
    WebSocketClientConnection(Client* client, const URL& url) : //, const net::Address& address
		ClientConnection(client, url) //, address
	{
		socket().replaceAdapter(new WebSocketConnectionAdapter(*this, WebSocket::ClientSide));	//&socket(), &request(), request(), request()
	}

	virtual ~WebSocketClientConnection() 
	{
	}
};


// -------------------------------------------------------------------
//
class WebSocketSecureClientConnection: public ClientConnection
{
public:
    WebSocketSecureClientConnection(Client* client, const URL& url) : //, const net::Address& address
		ClientConnection(client, url, net::SSLSocket()) //, address
	{
		socket().replaceAdapter(new WebSocketConnectionAdapter(*this, WebSocket::ClientSide)); //(&socket(), &request()
	}

	virtual ~WebSocketSecureClientConnection() 
	{
	}
};
#endif


//
// HTTP Client
//


class Client: public basic::Module
{
public:
	Client(uv::Loop* loop = uv::defaultLoop());
	virtual ~Client();

	void shutdown();
	
	ClientConnection* createConnection(const URL& url)
	{
		return createConnectionT<ClientConnection>(url);
	}
	
	template<class ConnectionT>
	ConnectionT* createConnectionT(const URL& url)
	{
		ClientConnection* conn = nullptr;

		if (url.scheme() == "http") {
			conn = new ConnectionT(url, net::TCPSocket(loop));
		}
		else if (url.scheme() == "https") {
			conn = new ConnectionT(url, net::SSLSocket(loop));
		}
		else if (url.scheme() == "ws") {
			conn = new ConnectionT(url, net::TCPSocket(loop));
			conn->socket().replaceAdapter(new WebSocketConnectionAdapter(*conn, WebSocket::ClientSide));
		}
		else if (url.scheme() == "wss") {
			conn = new ConnectionT(url, net::SSLSocket(loop));
			conn->socket().replaceAdapter(new WebSocketConnectionAdapter(*conn, WebSocket::ClientSide));
		}
		else
			throw std::runtime_error("Unknown connection type for URL: " + url.str());

		return conn; //new ConnectionT(this, url);
	}

	virtual void addConnection(ClientConnection* conn);
	virtual void removeConnection(ClientConnection* conn);
	
	void onTimer(void*);

	NullSignal Shutdown;
	
	virtual const char* className() const { return "HTTPClient"; }

protected:	
	friend class ClientConnection;
	
	ClientConnectionList connections;
	uv::Loop* loop;
	Timer timer;
};


} } // namespace scy::http


#endif

		//conn->Complete += delegate(this, &Client::onConnectionMessage);	
	//void onConnectionMessage(void* sender, const Response& response);
		//addConnection(conn);




/*
// -------------------------------------------------------------------
//
struct OutputStream//: public std::ostream
{
	Connection& connection;
	Buffer sendBuffer;

	OutputStream(Connection& connection) :
		connection(connection)
	{
	};
	
	OutputStream& operator << (const std::string& data)
	{
		sendBuffer.put(data);
		return *this;
	}
	
	OutputStream& operator << (const char* data)
	{
		sendBuffer.put(data);
		return *this;
	}

	template<typename T>
	OutputStream& operator << (const T& data) {
		sendBuffer.write<T>(data);
		return *this;
	}

	OutputStream& operator << (std::ostream&(*f)(std::ostream&)) 
	{
		connection.write(sendBuffer.data(), sendBuffer.available());
		return *this;
	}
};
*/




	/*
	void onAccept(void* sender, const net::TCPSocket& sock)
	{	
		traceL("Client", this) << "On Accept" << std::endl;
		ClientConnection* conn = createConnection(sock);
		if (conn)
			connections.push_back(conn);
	}

	void onTimer(void*)
	{
		for (ClientConnectionList::iterator it = connections.begin(); it != connections.end();) {
			if ((*it)->deleted()) {
				traceL("Client", this) << "Deleting connection: " << (*it) << std::endl;
				delete *it;
				it = connections.erase(it);
			}
			else
				++it;
		}
	}
	
	void onClose(void* sender) 
	{
		traceL("Client", this) << "On close" << std::endl;
		//assert(0 && "server socket closed");
	}
	*/


	/*
	Timer timer;
	void start()
	{	
		assert(socket.base().refCount() == 1);
		socket.bind(address);
		socket.listen();
		socket.base().AcceptConnection += delegate(this, &Client::onAccept);	
		socket.Close += delegate(this, &Client::onClose);

		traceL("Client", this) << "Client listening on " << port() << std::endl;		

		timer.Timeout += delegate(this, &Client::onTimer);
		timer.start(5000, 5000);
	}

	void stop() 
	{
		socket.close();
	}

	UInt16 port()
	{
		return socket.address().port();
	}

	ClientResponder* createResponder(ClientConnection& conn)
	{
		// The initial HTTP request headers have already
		// been parsed by now, but the request body may 
		// be incomplete (especially if chunked).
		return factory->createResponder(conn);
	}
	*/

/*
class ClientRequester
	// The abstract base class for HTTP ClientRequesters 
	// created by HTTP Client.
	//
	// Derived classes must override the handleRequest() method.
	//
	// A new HTTPClientRequester object will be created for
	// each new HTTP request that is received by the HTTP Client.
	//
{
public:
	ClientRequester(ClientConnection& connection) : 
		_connection(connection)
	{
	}

	ClientConnection& connection();
	Request& request();
	Response& response();

protected:
	ClientConnection& _connection;
};


typedef std::vector<ClientConnection*> ClientConnectionList;

class ClientRequester;
*/


