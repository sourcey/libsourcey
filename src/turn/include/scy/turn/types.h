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

	
enum AuthenticationState 
{
	Authenticating	= 1,
	Authorized		= 2,
	QuotaReached	= 3,
	NotAuthorized	= 4
};


struct Request: public stun::Message
{
	net::Socket socket;
	net::Address localAddress;
	net::Address remoteAddr;

	Request(const net::Socket& socket, 
			const stun::Message& message, 
			const net::Address& localAddress = net::Address(), 
			const net::Address& remoteAddr = net::Address()) :
		stun::Message(message), 
		socket(socket), 
		localAddress(localAddress), 
		remoteAddr(remoteAddr) {}

	//Request(const Request& r) :
	//	stun::Message(r), 
	//	socket(r.socket), 
	//	localAddress(r.localAddress), 
	//	remoteAddr(r.remoteAddr) {}

//private:
	//Request& operator = (const Request&) {}
};


typedef std::vector<std::string> IPList;


} } // namespace scy::turn


#endif // SCY_TURN_Types_H
