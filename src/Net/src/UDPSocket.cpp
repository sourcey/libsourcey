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


#include "Sourcey/Net/UDPSocket.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Logger.h"


using namespace std;

//using scy::Address;
//using scy::Exception;


namespace scy {
namespace net {


UDPSocket::UDPSocket() : 
	net::Socket(new UDPBase, false)
{
}


UDPSocket::UDPSocket(UDPBase* base, bool shared) : 
	net::Socket(base, shared) 
{
}


UDPSocket::UDPSocket(const Socket& socket) : 
	net::Socket(socket)
{
	if (!dynamic_cast<UDPBase*>(_base))
		throw Exception("Cannot assign incompatible socket");
}
	

UDPBase& UDPSocket::base() const
{
	return static_cast<UDPBase&>(*_base);
}


} } // namespace scy::uv