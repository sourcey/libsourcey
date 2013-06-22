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


#ifndef SOURCEY_HTTP_Server_H
#define SOURCEY_HTTP_Server_H


#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Net/TCPService.h"

#include "Poco/Mutex.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"

	
namespace Scy { 
namespace HTTP {


class Server;
class ServerConnectionFactory: public Poco::Net::TCPServerConnectionFactory
	/// This implementation of a TCPServerConnectionFactory
	/// is used by HTTPServer to create HTTPServerConnection objects.
{
public:
	ServerConnectionFactory(Server* server, Poco::Net::HTTPServerParams::Ptr pParams, Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory);
		/// Creates the ServerConnectionFactory.

	~ServerConnectionFactory();
		/// Destroys the ServerConnectionFactory.

	Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket);
		/// Creates an instance of HTTPServerConnection
		/// using the given StreamSocket.
	
protected:
	Poco::Net::HTTPServerParams::Ptr          _pParams;
	Poco::Net::HTTPRequestHandlerFactory::Ptr _pFactory;
	Server* _server;
};


// ---------------------------------------------------------------------
//
class ServerConnectionHook
	/// Provides a hook access to incoming socket requests from Poco's
	/// frustratingly limited access HTTP server implementation.
{
public:
	virtual Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket, const std::string& rawRequest) = 0;
};

typedef std::vector<ServerConnectionHook*> ServerConnectionHookVec;


// ---------------------------------------------------------------------
//
class Server: public Poco::Net::TCPServer
	/// A subclass of TCPServer that implements a
	/// full-featured multithreaded HTTP server.
	///
	/// A HTTPRequestHandlerFactory must be supplied.
	/// The ServerSocket must be bound and in listening state.
	///
	/// To configure various aspects of the server, a HTTPServerParams
	/// object can be passed to the constructor.
	///
	/// The server supports:
	///   - HTTP/1.0 and HTTP/1.1
	///   - automatic handling of persistent connections.
	///   - automatic decoding/encoding of request/response message bodies
	///     using chunked transfer encoding.
	///
	/// Please see the TCPServer class for information about
	/// connection and thread handling.
	///
	/// See RFC 2616 <http://www.faqs.org/rfcs/rfc2616.html> for more
	/// information about the HTTP protocol.
{
public:
	Server(Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory, 
		const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams);
		/// Creates the HTTPServer, using the given ServerSocket.
		///
		/// New threads are taken from the default thread pool.

	Server(Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory, Poco::ThreadPool& threadPool, 
			const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams);
		/// Creates the HTTPServer, using the given ServerSocket.
		///
		/// New threads are taken from the given thread pool.

	virtual ~Server();
		/// Destroys the HTTPServer and its HTTPRequestHandlerFactory.

	virtual void addConnectionHook(ServerConnectionHook* hook);
		/// Adds a connection hook to process incoming socket requests
		/// before they hit the HTTP server.
	
	virtual Poco::Net::TCPServerConnection* handleSocketConnection(const Poco::Net::StreamSocket& socket);
		// Called by ServerConnectionFactory to process connection hooks.
	
	ServerConnectionHookVec connectionHooks() const;

protected:
	mutable Poco::FastMutex _mutex;
	Poco::Net::HTTPRequestHandlerFactory::Ptr _pFactory;
	ServerConnectionHookVec _connectionHooks;
};


// ---------------------------------------------------------------------
//
class FlashPolicyConnectionHook: public HTTP::ServerConnectionHook
{
public:
	Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket, const std::string& rawRequest)
	{		
		try 
		{			
			if (rawRequest.find("policy-file-request") != std::string::npos) {
				LogTrace("HTTPStreamingRequestHandlerFactory") << "Sending Flash Crossdomain XMLSocket Policy" << std::endl;
				return new Net::FlashPolicyRequestHandler(socket, false);
			}
			else if (rawRequest.find("crossdomain.xml") != std::string::npos) {
				LogTrace("HTTPStreamingRequestHandlerFactory") << "Sending Flash Crossdomain HTTP Policy" << std::endl;
				return new Net::FlashPolicyRequestHandler(socket, true);
			}			
		}
		catch (Exception& exc)
		{
			LogError("ServerConnectionHook") << "Bad Request: " << exc.displayText() << std::endl;
		}	
		return NULL;
	};
};


// ---------------------------------------------------------------------
//
class BadRequestHandler: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		response.send().flush();
	}
};


} } // namespace Scy::HTTP


#endif