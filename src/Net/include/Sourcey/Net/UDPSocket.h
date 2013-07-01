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


#ifndef SOURCEY_Net_UDPSocket_H
#define SOURCEY_Net_UDPSocket_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/UDPBase.h"
#include "Sourcey/Net/Socket.h"	
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Address.h"


namespace scy {
namespace net {

	
typedef uv::UDPBase UDPBase;


class UDPSocket: public net::Socket
	/// UDPSocket is a disposable UDP socket wrapper
	/// for UDPBase which can be created on the stack.
	/// See UDPBase for implementation details.
{
public:	
	typedef UDPBase Base;
	typedef std::vector<UDPSocket> List;
	
	UDPSocket();
		/// Creates an unconnected UDP socket.

	UDPSocket(UDPBase* base, bool shared = false);
		/// Creates the Socket and attaches the given SocketBase.
		///
		/// The SocketBase must be a UDPBase, otherwise an
		/// exception will be thrown.

	UDPSocket(const Socket& socket);
		/// Creates the UDPSocket with the SocketBase
		/// from another socket. The SocketBase must be
		/// a UDPBase, otherwise an exception will be thrown.
	
	UDPBase& base() const;
		/// Returns the socket's SocketBase instance.
};


} } // namespace scy::uv


#endif // SOURCEY_Net_UDPSocket_H