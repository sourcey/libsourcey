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


#include "Sourcey/Net/TCPService.h"

#include "Sourcey/Logger.h"
#include "assert.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Scy {
namespace Net {


TCPService::TCPService(TCPServerConnectionFactory* factory, unsigned short port) : 
	_factory(factory),
	_server(NULL),
	_port(0)
{
	if (port)
		_portList.push_back(port);
}


TCPService::TCPService(TCPServerConnectionFactory* factory, const std::vector<unsigned short> portList) : 
	_factory(factory),
	_server(NULL),
	_port(0),
	_portList(portList)
{
}


TCPService::~TCPService()
{
	stop();

	// Factory is instance is deleted by the server
	if (_server)
		delete _server;
	//if (_serverSock)
	//	delete _serverSock;
}


bool TCPService::start()
{
	return start(_portList);
}

bool TCPService::start(const vector<unsigned short> portList)
{
	_portList = portList;
	for (int i = 0; i < _portList.size(); i++) {
		if (start(_portList[i]))
			return true;
	}
	return false;
}


bool TCPService::start(unsigned short port)
{
	assert(!isConnected());

	try 
	{	
		LogDebug() << "[TCPService] Starting on " << port << endl;	
		_serverSock.bind(port);
		_serverSock.listen();

		if (_server == NULL) {
			TCPServerParams* params = new TCPServerParams;
			_server = new TCPServer(_factory, _serverSock, params);
		}

		_server->start();
		_port = port;
		setState(this, TCPServiceState::Connected);
		return true;
	} 
	catch (Poco::Exception& exc) 
	{
		LogError() << "[TCPService] Failed to initialize: " << exc.displayText() << endl;
		setState(this, TCPServiceState::Error, exc.displayText());
	}		

	return false;
}


void TCPService::stop()
{	
	LogDebug() << "[TCPService] Stopping" << endl;
	if (_server)
		_server->stop();

	setState(this, TCPServiceState::Disconnected);
}


// ---------------------------------------------------------------------
// Flash Policy Request Handler
//
FlashPolicyRequestHandler::FlashPolicyRequestHandler(const StreamSocket& s, bool isHTTP) : 
	TCPServerConnection(s), _isHTTP(isHTTP)
{
}


void FlashPolicyRequestHandler::run()
{		
	// Send an all access policy file by default
	string policy;
	if (_isHTTP)
		policy += "HTTP/1.1 200 OK\r\nContent-Type: text/x-cross-domain-policy\r\nX-Permitted-Cross-Domain-Policies: all\r\n\r\n";
	policy += "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>";
	socket().sendBytes(policy.data(), policy.length()+1);
	LogDebug() << "Sending policy file response: " << policy << endl;
}


// ---------------------------------------------------------------------
// Null Request Handler
//
NullRequestHandler::NullRequestHandler(const StreamSocket& s) : 
	TCPServerConnection(s)
{
}	


void NullRequestHandler::run() 
{
	LogDebug() << "Bad Request Handler" << endl;	
	socket().close();
}


} } // namespace Scy::Net