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


#ifndef SOURCEY_IPacketProcessor_H
#define SOURCEY_IPacketProcessor_H


#include "Sourcey/PacketEmitter.h"


namespace Scy {


class IPacketProcessor: public PacketEmitter
	/// This class is a virtual interface for processors that
	/// process and emit the IPacket type. 
	/// IPacketProcessor derive classes generally belong to a 
	/// PacketStream.
{ 
public:
	virtual void process(IPacket& packet) = 0;
		/// This method performs processing on the given
		/// packet. Resultant packet's will be dispatched.
		/// NOTE: If packet processing is asynchronous packet 
		/// data must be copied and the dispatched packet's
		/// freeOnDestroy flag must be set.
		/// NOTE: The implementation must re-dispatch the 
		/// packet even if it was not accepted or processed.

	virtual bool accepts(IPacket&) { return true; };
		/// This method ensures compatibility with the given 
		/// packet type.

	virtual void operator << (IPacket& packet) { process(packet); };
		/// Stream operator alias for process()
};


} // namespace Scy


#endif // SOURCEY_IPacketProcessor_H