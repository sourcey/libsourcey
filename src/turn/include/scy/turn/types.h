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


#ifndef SCY_TURN_Types_H
#define SCY_TURN_Types_H


#include "scy/net/socket.h"
#include "scy/net/address.h"
#include "scy/stun/message.h"


namespace scy {
namespace turn {


const int CLIENT_SOCK_BUF_SIZE = 65536;
const int SERVER_SOCK_BUF_SIZE = CLIENT_SOCK_BUF_SIZE * 32;


enum AuthenticationState 
{
	Authenticating	= 1,
	Authorized		= 2,
	QuotaReached	= 3,
	NotAuthorized	= 4
};


class Request: public stun::Message
{
public:
	net::TransportType transport;
	net::Address localAddress;
	net::Address remoteAddress;
	std::string hash; // for MessageIntegrity signing

	Request(const stun::Message& message, 
			net::TransportType transport,
			const net::Address& localAddress = net::Address(), 
			const net::Address& remoteAddress = net::Address()) :
		stun::Message(message), 
		transport(transport), 
		localAddress(localAddress), 
		remoteAddress(remoteAddress) {}
};


typedef std::vector<std::string> IPList;


} } // namespace scy::turn


#endif // SCY_TURN_Types_H
