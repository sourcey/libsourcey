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


#ifndef SOURCEY_TURN_FiveTuple_H
#define SOURCEY_TURN_FiveTuple_H


#include "Sourcey/Net/IPacketSocket.h"

#include <sstream>


namespace Scy {
namespace TURN {


class FiveTuple 
	/// The 5-TUPLE consists of a local, a remote address, and the
	/// transport protocol used by the client to communicate with the server. 
	///
	///                                                               +---------+
	///                                                               |         |
	///                                                               | External|
	///                                                             / | Client  |
	///                                                           ///  |         |
	///                                                          /    |         |
	///                                                        ///     +---------+
	///                                                       /
	///                                                     ///
	///                     +-+                            /
	///                     | |                           /
	///                     | |                         ///
	///      +---------+    | |          +---------+   /              +---------+
	///      |         |    |N|          |         | ///               |         |
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
  FiveTuple(const Net::Address& remote, const Net::Address& local, Net::TransportProtocol transport);
  FiveTuple(const FiveTuple& r);

  const Net::Address& remote() const { return _remote; }
  const Net::Address& local() const { return _local; }
  const Net::TransportProtocol& transport() const { return _transport; }

  void remote(const Net::Address& remote) { _remote = remote; }
  void local(const Net::Address& local) { _local = local; }
  void transport(const Net::TransportProtocol& transport) { _transport = transport; }

  bool operator ==(const FiveTuple& r) const;
  bool operator <(const FiveTuple& r) const;

  std::string toString() const;

private:
  Net::Address _remote;
  Net::Address _local;
  Net::TransportProtocol _transport;
};


} } // namespace Scy::TURN


#endif // SOURCEY_TURN_FiveTuple_H
