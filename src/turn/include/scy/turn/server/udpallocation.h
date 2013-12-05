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


#ifndef SCY_TURN_SERVER_UDPAllocation_H
#define SCY_TURN_SERVER_UDPAllocation_H


#include "scy/turn/server/serverallocation.h"
#include "scy/net/packetsocket.h"
#include "scy/net/udpsocket.h"


namespace scy {
namespace turn {


class Server;
class IConnection;


class UDPAllocation: public ServerAllocation
{
public:
	UDPAllocation(
		Server& server,
		const FiveTuple& tuple, 
		const std::string& username, 
		const UInt32& lifetime);
	virtual ~UDPAllocation();

	//void onPacketReceived(void* sender, RawPacket& packet);
	void onPeerDataReceived(void*, net::SocketPacket& packet);
		
	bool handleRequest(Request& request);	
	void handleSendIndication(Request& request);

	int send(const char* data, int size, const net::Address& peerAddress);
	
	net::Address relayedAddress() const;

	//virtual const char* className() const { return "UDPAllocation"; };

private:
	//net::PacketSocket _relaySocket;
	net::UDPSocket _relaySocket;
};


} } //  namespace scy::turn


#endif // SCY_TURN_SERVER_UDPAllocation_H