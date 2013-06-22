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


#ifndef SOURCEY_IDepacketizerR_H
#define SOURCEY_IDepacketizerR_H


#include "Sourcey/IPacketProcessor.h"


namespace scy {


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

	virtual void emit(IPacket& packet)
		// Sends a packet to the handler and increments
		// the sequence counter.
	{
		_sequenceNumber++;	
		IPacketProcessor::emit(this, packet);
	}

	void setSequenceNumber(int n) { _sequenceNumber =  n; }

protected:
	int _sequenceNumber;
};


} // namespace scy


#endif // SOURCEY_IDepacketizerR_H