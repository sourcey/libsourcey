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


#include "Sourcey/HTTP/Server.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"

#include "Poco/Net/HTTPServerConnection.h"


using namespace std;


namespace Scy { 
namespace HTTP {

	
Server::Server(Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory, const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams):
	Poco::Net::TCPServer(new ServerConnectionFactory(this, pParams, pFactory), socket, pParams),
	_pFactory(pFactory)
{
}


Server::Server(Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory, Poco::ThreadPool& threadPool, 
		const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams):
	Poco::Net::TCPServer(new ServerConnectionFactory(this, pParams, pFactory), threadPool, socket, pParams),
	_pFactory(pFactory)
{
}


Server::~Server()
{
	{		
		Poco::FastMutex::ScopedLock lock(_mutex);
		Scy::Util::ClearVector(_connectionHooks);
	}
}


void Server::addConnectionHook(ServerConnectionHook* hook)
{	
	Poco::FastMutex::ScopedLock lock(_mutex);
	_connectionHooks.push_back(hook);
}

	
ServerConnectionHookVec Server::connectionHooks() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _connectionHooks;
}


Poco::Net::TCPServerConnection* Server::handleSocketConnection(const Poco::Net::StreamSocket& socket)
{	
	Poco::Net::TCPServerConnection* conn = NULL;
	/*
	// TODO: How does POCO handle these half-open requests
	try 
	{
		ServerConnectionHookVec hooks = connectionHooks();
		if (!hooks.empty()) {
			LogTrace("HTTPServer") << "Reading Request" << endl;
			Poco::Net::StreamSocket ss(socket);
			char buffer[4096];
			ss.setReceiveTimeout(Poco::Timespan(2,0));
			int size = ss.receiveBytes(buffer, sizeof(buffer), MSG_PEEK);
			string rawRequest(buffer, size);	
			for (int i = 0; i < hooks.size(); i++) {
				conn = hooks[i]->createConnection(socket, rawRequest);
				if (conn)
					break;
			}
		}
	}
	catch (Exception& exc) {
		LogError("HTTPServer") << "Hook Error: " << exc.displayText() << endl;
	}
	*/
	return conn;
}


// ---------------------------------------------------------------------
//
ServerConnectionFactory::ServerConnectionFactory(Server* server, Poco::Net::HTTPServerParams::Ptr pParams, Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory):
	_server(server),
	_pParams(pParams),
	_pFactory(pFactory)
{
	poco_check_ptr (pFactory);
}


ServerConnectionFactory::~ServerConnectionFactory()
{
}


Poco::Net::TCPServerConnection* ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket& socket)
{
	/*
	Poco::Net::TCPServerConnection* conn = _server->handleSocketConnection(socket);
	if (conn) {		
		LogTrace("ServerConnectionFactory") << "Hook Overriding" << endl;
		return conn;
	}
	*/
	return new Poco::Net::HTTPServerConnection(socket, _pParams, _pFactory);
}


} } // namespace Scy::HTTP