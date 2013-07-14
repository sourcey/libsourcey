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


#ifndef SOURCEY_Net_TCPSocket_H
#define SOURCEY_Net_TCPSocket_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Net/TCPBase.h"
#include "Sourcey/Net/Socket.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Types.h"


namespace scy {
namespace net {


class TCPSocket: public net::Socket
	/// TCPSocket is a disposable TCP socket wrapper
	/// for TCPBase which can be created on the stack.
	/// See TCPBase for implementation details.
{
public:	
	typedef TCPBase Base;
	typedef std::vector<TCPSocket> List;
	
	TCPSocket();
		/// Creates an unconnected TCP socket.

	TCPSocket(TCPBase* base, bool shared = false);
		/// Creates the Socket and attaches the given SocketBase.
		///
		/// The SocketBase must be a TCPBase, otherwise an
		/// exception will be thrown.

	TCPSocket(const Socket& socket);
		/// Creates the UDPSocket with the SocketBase
		/// from another socket. The SocketBase must be
		/// a UDPBase, otherwise an exception will be thrown.
	
	TCPBase& base() const;
		/// Returns the SocketBase for this socket.
};


} } // namespace scy::net


#endif // SOURCEY_Net_TCPSocket_H