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


#ifndef SOURCEY_PacketEmitter_H
#define SOURCEY_PacketEmitter_H


#include "Sourcey/PacketSignal.h"
#include "Sourcey/IPacket.h"

#include <vector>


namespace scy {


struct PacketStreamState;


class PacketEmitter: public PacketSignal
	/// The packet dispatcher is a generic interface used
	/// extensively within LibSourcey by classes that emit 
	/// the IPacket type.
{ 
public:
	virtual void onStreamStateChange(const PacketStreamState&) 
		/// If this class is attached to a PacketStream this
		/// method will be called each time the stream state
		/// changes.
	{
	}
};


typedef std::vector<PacketEmitter*> PacketEmitterList;


} // namespace scy


#endif // SOURCEY_PacketEmitter_H

	

	/*
		//Poco::FastMutex::ScopedLock lock(_mutex);
		//_packetNumber = 0; 

	PacketEmitter() : _packetNumber(0) {};
	virtual ~PacketEmitter() {};

	virtual void operator += (const PacketDelegateBase& delegate)
	{ 
		PacketSignal::attach(delegate); 
	}
	
	virtual void operator -= (const PacketDelegateBase& delegate) 
	{ 
		PacketSignal::detach(delegate); 
	}
	
	virtual void operator -= (const void* klass) 
	{ 
		PacketSignal::detach(klass); 
	}

	virtual void attach(const PacketDelegateBase& delegate)
	{
		PacketSignal::attach(delegate);
	}

	virtual void detach(const PacketDelegateBase& delegate) 
	{
		PacketSignal::detach(delegate);
	}
	
	virtual void detach(const void* klass) 
	{
		PacketSignal::detach(klass);
	}
	*/

	/*
	virtual void emit(void* sender, IPacket& packet) 
	{
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_packetNumber++;
		}
		PacketSignal::emit(sender, packet); 
	}

	virtual UInt32 packetNumber() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _packetNumber;
	}

	virtual int refCount() const
	{
		return PacketSignal::refCount();
	}

protected:	
	UInt32 _packetNumber;

	//mutable Poco::FastMutex	_mutex;
	*/


	/*
	//PacketSignal BroadcastPacket;
		// Delegates listen in on this signal to receive 
		// broadcasted packets.
	*/