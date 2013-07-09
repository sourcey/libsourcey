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
#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/HTTP/WebSocket.h"
#include "Sourcey/Timer.h"
/*
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Format.h"
#include <sstream>
*/


namespace scy { 
namespace http {
	

class Client;
class ClientConnection: public Connection
{
public:
    ClientConnection(Client& client, const net::Address& address, 
		const net::Socket& socket = net::TCPSocket());

	virtual void send();
		/// Sends the HTTP request.
		///
		/// Calls connect() internally if the socket
		/// is not already connecting or connected.
	
	virtual bool flush();
		/// Flushes the HTTP request output stream.
	
	virtual void close();
		/// Forcefully closes the HTTP connection.
	
	//
	/// Internal callbacks
	virtual void onHeaders();
	virtual void onPayload(Buffer& buffer);
	virtual void onComplete();
	virtual void onClose();
		
	//
	/// Status signals
	Signal<Response&> Headers;
	Signal<Buffer&> Payload;	
	Signal<const Response&> Complete;
		/// May be success or error response		
	
protected:
    virtual ~ClientConnection();

	Poco::Net::HTTPMessage* incomingHeaders();
	Poco::Net::HTTPMessage* outgoingHeaders();
				
	Client& client();
	
	void onSocketConnect(void*);
	void onClientShutdown(void*);
	
protected:	
	Client& _client;
	net::Address _address;
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
	

// -------------------------------------------------------------------
//
class SecureClientConnection: public ClientConnection
{
public:
    SecureClientConnection(Client& client, const net::Address& address)
		: ClientConnection(client, address, net::SSLSocket())
	{
	}

	virtual ~SecureClientConnection() 
	{
	}
};


// -------------------------------------------------------------------
//
class WSClientConnection: public ClientConnection
{
public:
    WSClientConnection(Client& client, const net::Address& address) : 
		ClientConnection(client, address)
	{
		setAdapter(new WebSocketClientAdapter(&socket(), &request()));	
	}

	virtual ~WSClientConnection() 
	{
	}
};


// -------------------------------------------------------------------
//
class WSSClientConnection: public ClientConnection
{
public:
    WSSClientConnection(Client& client, const net::Address& address) : 
		ClientConnection(client, address, net::SSLSocket())
	{
		setAdapter(new WebSocketClientAdapter(&socket(), &request()));
	}

	virtual ~WSSClientConnection() 
	{
	}
};


// -------------------------------------------------------------------
//
class Client
	/// TODO: SSL: makeSSL via TCPSocket
{
public:
	Client();
	virtual ~Client();

	void shutdown();
	
	ClientConnection* createConnection(const net::Address& address)
	{
		return createConnectionT<ClientConnection>(address);
	}
	
	template<class ConnectionT>
	ConnectionT* createConnectionT(const net::Address& address)
	{
		return new ConnectionT(*this, address);
	}
	
	void onTimer(void*);

	virtual void addConnection(ClientConnection* conn);
	virtual void removeConnection(ClientConnection* conn);

	NullSignal Shutdown;

protected:	
	friend class ClientConnection;
	
	ClientConnectionList connections;
	Timer timer;
};


} } // namespace scy::http


#endif

		//conn->Complete += delegate(this, &Client::onConnectionComplete);	
	//void onConnectionComplete(void* sender, const Response& response);
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
		sendBuffer.write(data);
		return *this;
	}
	
	OutputStream& operator << (const char* data)
	{
		sendBuffer.write(data);
		return *this;
	}

	template<typename T>
	OutputStream& operator << (const T& data) {
		sendBuffer.write<T>(data);
		return *this;
	}

	OutputStream& operator << (std::ostream&(*f)(std::ostream&)) 
	{
		connection.sendRaw(sendBuffer.data(), sendBuffer.size());
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
				traceL("Client", this) << "Deleting Connection: " << (*it) << std::endl;
				delete *it;
				it = connections.erase(it);
			}
			else
				++it;
		}
	}
	
	void onClose(void* sender) 
	{
		traceL("Client", this) << "On Close" << std::endl;
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


