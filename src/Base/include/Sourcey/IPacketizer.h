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


#ifndef SOURCEY_IPacketizer_H
#define SOURCEY_IPacketizer_H


#include "Sourcey/IPacketProcessor.h"


namespace Scy {


class IPacketizer: public IPacketProcessor
	/// This class is a virtual interface for implementing packetization
	/// of IPacket types.
{
public:
	IPacketizer(int mtu = 1450) :
		_mtu(mtu) {}

	virtual void process(IPacket& packet) = 0;
		/// Processes incoming packets and outputs packetized packets.

	virtual void setMTU(int n)
		/// MTU defines the maximum transmission unit, which is the size 
		/// of the largest packet that a network protocol can transmit. 
		/// For UDP the maximum Ethernet frame size is 1500. In is the
		/// responsibility of subclasses to ensure this is implemented. 
		/// Emitted packets by the packetizer must not exceed the MTU.
	{ _mtu =  n; }

protected:
	int _mtu;
};


} // namespace Scy


#endif // SOURCEY_IPacketizer_H