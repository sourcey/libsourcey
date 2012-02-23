//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_PacketDispatcher_H
#define SOURCEY_PacketDispatcher_H


#include "Sourcey/PacketSignal.h"
#include "Sourcey/IPacket.h"

#include <vector>


namespace Sourcey {


struct PacketStreamState;


class PacketDispatcher: public PacketSignal
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


typedef std::vector<PacketDispatcher*> PacketBroadcasterList;


} // namespace Sourcey


#endif // SOURCEY_PacketDispatcher_H

	

	/*
		//Poco::FastMutex::ScopedLock lock(_mutex);
		//_packetNumber = 0; 

	PacketDispatcher() : _packetNumber(0) {};
	virtual ~PacketDispatcher() {};

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
		PacketSignal::detachAll(klass); 
	}

	virtual void attach(const PacketDelegateBase& delegate)
	{
		PacketSignal::attach(delegate);
	}

	virtual void detach(const PacketDelegateBase& delegate) 
	{
		PacketSignal::detach(delegate);
	}
	
	virtual void detachAll(const void* klass) 
	{
		PacketSignal::detachAll(klass);
	}
	*/

	/*
	virtual void dispatch(void* sender, IPacket& packet) 
	{
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_packetNumber++;
		}
		PacketSignal::dispatch(sender, packet); 
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