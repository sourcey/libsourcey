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


#ifndef SOURCEY_IDepacketizerR_H
#define SOURCEY_IDepacketizerR_H


#include "Sourcey/IPacketProcessor.h"


namespace Sourcey {


//template <class T>
class IDepacketizer: public IPacketProcessor
	/// This class is a virtual interface for implementing depacketization
	/// of IPacket types.
{
public:
	IDepacketizer() :
		_sequenceNumber(0) {}

	virtual void process(IPacket& packet) = 0;
		// Processes incoming packets.

	virtual void dispatch(IPacket& packet)
		// Sends a packet to the handler and increments
		// the sequence counter.
	{
		_sequenceNumber++;	
		IPacketProcessor::dispatch(this, packet);
	}

	void setSequenceNumber(int n) { _sequenceNumber =  n; }

protected:
	int _sequenceNumber;
};


} // namespace Sourcey


#endif // SOURCEY_IDepacketizerR_H