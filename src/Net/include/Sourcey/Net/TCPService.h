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


#ifndef SOURCEY_NET_TCPService_H
#define SOURCEY_NET_TCPService_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/ServerSocket.h"

#include <map>


namespace Sourcey {
namespace Net {


struct TCPServiceState: public StateT 
{
	enum Type 
	{
		None			= 0,	///< The server is in no state.
		Connected		= 0x01, ///< The server is online and authenticated.
		Disconnected	= 0x02, ///< The server is in disconnected state.
		Error			= 0x04, ///< The server is in erroneous state.	
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:			return "None";
		case Connected:		return "Connected";
		case Disconnected:	return "Disconnected";
		case Error:			return "Error";
		}
		return "undefined"; 
	};
};


class TCPService: public StatefulSignal<TCPServiceState>
{
public:
	TCPService(Poco::Net::TCPServerConnectionFactory* factory, unsigned short port = 0);
	TCPService(Poco::Net::TCPServerConnectionFactory* factory, const std::vector<unsigned short> portList);
	virtual ~TCPService();
	
	virtual bool start();
	virtual bool start(unsigned short port);
	virtual bool start(const std::vector<unsigned short> portList);
	virtual void stop();

	virtual Poco::Net::TCPServerConnectionFactory* factory() const { return _factory; };
	virtual bool isConnected() const { return stateEquals(TCPServiceState::Connected); };
	virtual unsigned short port() const { return _port; };	

private:
	Poco::Thread*				_thread;
	Poco::Net::ServerSocket		_serverSock;
	Poco::Net::TCPServer*		_server;
	unsigned short				_port;
	std::vector<unsigned short> _portList;
	Poco::Net::TCPServerConnectionFactory* _factory;
};


// A map of bridge services indexed by their name.
typedef std::map<std::string, TCPService*> TCPServiceMap;


// ---------------------------------------------------------------------
//
// Flash Policy Request Handler
//
// ---------------------------------------------------------------------
class FlashPolicyRequestHandler: public Poco::Net::TCPServerConnection
{
public:
	FlashPolicyRequestHandler(const Poco::Net::StreamSocket& s);
	void run();
};


// ---------------------------------------------------------------------
//
// Request Handler
//
// ---------------------------------------------------------------------
class BadRequestHandler: public Poco::Net::TCPServerConnection
{
public:
	BadRequestHandler(const Poco::Net::StreamSocket& s);		
	void run();
};


} } // namespace Sourcey::Net

#endif // SOURCEY_NET_TCPService_H