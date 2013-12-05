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


#ifndef SCY_TURN_FiveTuple_H
#define SCY_TURN_FiveTuple_H


#include "scy/net/socket.h"

#include <sstream>


namespace scy {
namespace turn {


class FiveTuple 
	/// The 5-TUPLE consists of a local, a remote address, and the
	/// transport protocol used by the client to communicate with the server. 
	///
	///                                                               +---------+
	///                                                               |         |
	///                                                               | External|
	///                                                             / | Client  |
	///                                                           //  |         |
	///                                                          /    |         |
	///                                                        //     +---------+
	///                                                       /
	///                                                     //
	///                     +-+                            /
	///                     | |                           /
	///                     | |                         //
	///      +---------+    | |          +---------+   /              +---------+
	///      |         |    |N|          |         | //               |         |
	///      | TURN    |    | |          |         |/                 | External|
	///      | Client  |----|A|----------|   TURN  |------------------| Client  |
	///      |         |    | |^        ^|  Server |^                ^|         |
	///      |         |    |T||        ||         ||                ||         |
	///      +---------+    | ||        |+---------+|                |+---------+
	///         ^           | ||        |           |                |
	///         |           | ||        |           |                |
	///         |           +-+|        |           |                |
	///         |              |        |           |                |
	///         |
	///                    Internal     Internal    External         External
	///     Client         Remote       Local       Local            Remote
	///     Performing     Transport    Transport   Transport        Transport
	///     Allocations    Address      Address     Address          Address
	///
	///                        |          |            |                |
	///                        +-----+----+            +--------+-------+
	///                              |                          |
	///                              |                          |
	///
	///                            Internal                External
	///                            5-Tuple                 5-tuple
	///
{
public:
	FiveTuple();
	FiveTuple(const net::Address& remote, const net::Address& local, net::TransportType transport);
	FiveTuple(const FiveTuple& r);

	const net::Address& remote() const { return _remote; }
	const net::Address& local() const { return _local; }
	const net::TransportType& transport() const { return _transport; }

	void remote(const net::Address& remote) { _remote = remote; }
	void local(const net::Address& local) { _local = local; }
	void transport(const net::TransportType& transport) { _transport = transport; }

	bool operator ==(const FiveTuple& r) const;
	bool operator <(const FiveTuple& r) const;

	std::string toString() const;
		
	friend std::ostream& operator << (std::ostream& stream, const FiveTuple& tuple) 
	{
		stream << tuple.toString();
		return stream;
	}

private:
	net::Address _remote;
	net::Address _local;
	net::TransportType _transport;
};


} } // namespace scy::turn


#endif // SCY_TURN_FiveTuple_H
