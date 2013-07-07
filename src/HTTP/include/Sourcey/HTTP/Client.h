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
#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Response.h"
#include "Sourcey/HTTP/Parser.h"
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
    ClientConnection(Client& client, const net::Address& address);

	virtual void send(bool whiny = false);
		/// Sends the HTTP rerquest
		/// Calls connect() internally if thew socket
		/// is not already connecting or connected.
	
	virtual void close();
		/// Close the HTTP connection
		
	//
	/// Status signals
	Signal<Response&> ResponseHeaders;
	Signal<Buffer&> ResponseBody;	
	Signal<const Response&> ResponseComplete;
		/// May be success or error response		
	
protected:
    virtual ~ClientConnection();

	Poco::Net::HTTPMessage* incomingHeaders();
	Poco::Net::HTTPMessage* outgoingHeaders();
				
	Client& client();
	
	//
	/// Socket callbacks
	virtual void onSocketConnect();

	//
	/// Parser callbacks
    virtual void onParserHeadersDone();	
    virtual void onParserChunk(const char* buf, size_t len);
    virtual void onParserDone();

	//
	/// Client signals
	void onShutdown(void*);
	
protected:	
	Client& _client;
	net::Address _address;
};


typedef std::vector<ClientConnection*> ClientConnectionList;


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
	ClientConnection* createConnectionT(const net::Address& address)
	{
		//ConnectionT* conn = new ConnectionT(*this, address);
		//addConnection(conn);
		return new ConnectionT(*this, address); //conn;
	}
	
	void onTimer(void*);

	virtual void addConnection(ClientConnection* conn);
	virtual void removeConnection(ClientConnection* conn);

	NullSignal Shutdown;

protected:	
	friend class ClientConnection;
	
	ClientConnectionList connections;
	uv::Timer timer;
};




} } // namespace scy::http


#endif

		//conn->Complete += delegate(this, &Client::onConnectionComplete);	
	//void onConnectionComplete(void* sender, const Response& response);
		//addConnection(conn);



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
	uv::Timer timer;
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

	ClientResponser* createResponser(ClientConnection& conn)
	{
		// The initial HTTP request headers have already
		// been parsed by now, but the request body may 
		// be incomplete (especially if chunked).
		return factory->createResponser(conn);
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


