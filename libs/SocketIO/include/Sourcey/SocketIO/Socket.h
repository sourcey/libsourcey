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


#ifndef SOURCEY_SOCKETIO_Socket_H
#define SOURCEY_SOCKETIO_Socket_H


#include "Sourcey/Net/WebSocket.h"
#include "Sourcey/SocketIO/Packet.h"
#include "Sourcey/SocketIO/Transaction.h"
#include "Sourcey/Timer.h"
#include "Sourcey/JSON/JSON.h"
#include "Poco/URI.h"
#include "Poco/Net/NameValueCollection.h"


namespace Sourcey {
namespace SocketIO {
	

class Socket: public Net::WebSocket
{
public:
	Socket();
	Socket(const Net::Address& srvAddr);
	virtual ~Socket();
	
	void connect(const Net::Address& srvAddr);	
	void connect();
	void close();
	
	virtual int sendConnect(const std::string& endpoint = "", const std::string& query = "");

	virtual int send(SocketIO::Packet::Type type, const std::string& data, bool ack = false);	
	virtual int send(const std::string& data, bool ack = false); // Message packet
	virtual int send(const JSON::Value& data, bool ack = false); // JSON packet
	virtual int send(const SocketIO::Packet& packet);
	virtual int emit(const std::string& event, const JSON::Value& data, bool ack = false);
		// Sends an Event packet
	
	Poco::Net::NameValueCollection& httpHeaders();
	std::string sessionID() const;
	
	//Signal<SocketIO::Packet&> AckResponse;

protected:
	virtual int sendHeartbeat();
	virtual void onHeartBeatTimer(TimerCallback<Socket>& timer);
	virtual bool sendInitialRequest();

	std::string		_sessionID;
	int				_heartBeatTimeout;
	int				_connectionClosingTimeout;
	StringList		_protocols;
	Net::Address	_srvAddr;
	Poco::Net::NameValueCollection _httpHeaders;
	mutable Poco::FastMutex	_mutex;
};


} } // namespace Sourcey::SocketIO


#endif //  SOURCEY_SOCKETIO_Socket_H