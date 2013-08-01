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


#ifndef SOURCEY_HTTP_Client_H
#define SOURCEY_HTTP_Client_H


#include "Sourcey/Net/Socket.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/Net/Network.h"
#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/HTTP/WebSocket.h"
#include "Sourcey/Timer.h"


namespace scy { 
namespace http {

	
class TransferSignal : public Signal<const double&>
{
public:
	UInt64 current;
	UInt64 total;
	void* sender;

	TransferSignal() :	
		sender(nil), current(0), total(0) {}

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
		// Sends the internal HTTP request.
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
	
	virtual void close();
		// Forcefully closes the HTTP connection.
		
	virtual void setReadStream(std::ostream* os);
		// Sets the receiver stream for writing server response data.
		//
		// This given stream pointer may be a ofstream instance for 
		// writing to a file. The pointer is managed internally,
		// and will be deleted when the connection is closed.		
		
	template<class T>
	T* readStream()
		// Returns the cast read stream pointer or nil.
	{
		return dynamic_cast<T*>(_readStream);
	}
		
	void* opaque;
		// Optional client data pointer.
		//
		// The pointer is not initialized or managed
		// by the connection.
	
	//
	/// Internal callbacks
	virtual void onHeaders();
	virtual void onPayload(Buffer& buffer);
	virtual void onMessage();
	virtual void onClose();

	//
	/// Status signals
	NullSignal Connect;
		// Fires when the client socket is connected

	Signal<Response&> Headers;
		// Fires when the response HTTP header has been received

	Signal<const Response&> Complete;
		// Fires on success or error response
	
	TransferSignal IncomingProgress;
		// Notifies on download progress

	TransferSignal OutgoingProgress;
		// Notifies on upload progress

protected:
    virtual ~ClientConnection();
					
	http::Client* client();

	http::Message* incomingHeader();	
	http::Message* outgoingHeader();
		
	//virtual void setError(const Error& err);
	
	void onHostResolved(void*, const net::DNSResult& result);
	void onSocketConnect(void*);
	void onClientShutdown(void*);
	
protected:	
	http::Client* _client;
	URL _url;
	std::ostream* _readStream;
	TransferSignal _incomingProgress;
	TransferSignal _outgoingProgress;
};


typedef std::vector<ClientConnection*> ClientConnectionList;


// -------------------------------------------------------------------
//
class ClientAdapter: public ConnectionAdapter
{
public:
    ClientAdapter(ClientConnection& connection) : 
		ConnectionAdapter(connection, HTTP_RESPONSE)
	{
	}
};
	

/*
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
		socket().replaceAdapter(new WebSocketConnectionAdapter(*this, WebSocket::WS_CLIENT));	//&socket(), &request(), request(), request()
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
		socket().replaceAdapter(new WebSocketConnectionAdapter(*this, WebSocket::WS_CLIENT)); //(&socket(), &request()
	}

	virtual ~WebSocketSecureClientConnection() 
	{
	}
};
*/

	
inline ClientConnection* createConnection(const URL& url)
	/// Creates different connection types based on the URL scheme.
{
	ClientConnection* conn = 0;

	if (url.scheme() == "http") {
		conn = new ClientConnection(url);
	}
	else if (url.scheme() == "https") {
		conn = new ClientConnection(url, net::SSLSocket());
	}
	else if (url.scheme() == "ws") {
		conn = new ClientConnection(url);
		conn->socket().replaceAdapter(new WebSocketConnectionAdapter(*conn, WebSocket::WS_CLIENT));
	}
	else if (url.scheme() == "wss") {
		conn = new ClientConnection(url, net::SSLSocket());
		conn->socket().replaceAdapter(new WebSocketConnectionAdapter(*conn, WebSocket::WS_CLIENT));
	}
	else
		throw Exception("Unknown connection type for URL: " + url.str());

	return conn;
}


// -------------------------------------------------------------------
//
class Client: public Module
{
public:
	Client();
	virtual ~Client();

	void shutdown();
	
	ClientConnection* createConnection(const URL& url)
	{
		return createConnectionT<ClientConnection>(url);
	}
	
	template<class ConnectionT>
	ConnectionT* createConnectionT(const URL& url)
	{
		return new ConnectionT(this, url);
	}

	virtual void addConnection(ClientConnection* conn);
	virtual void removeConnection(ClientConnection* conn);
	
	void onTimer(void*);

	NullSignal Shutdown;
	
	virtual const char* className() const { return "HTTPClient"; }

protected:	
	friend class ClientConnection;
	
	ClientConnectionList connections;
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
	
	OutputStream& operator << (const string& data)
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
	/// The abstract base class for HTTP ClientRequesters 
	/// created by HTTP Client.
	///
	/// Derived classes must override the handleRequest() method.
	///
	/// A new HTTPClientRequester object will be created for
	/// each new HTTP request that is received by the HTTP Client.
	///
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


