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


#ifndef SOURCEY_IPacketDispatcher_H
#define SOURCEY_IPacketDispatcher_H


#include "Sourcey/PolymorphicSignal.h"
#include <vector>


namespace Sourcey {


struct PacketStreamState;


class IPacketDispatcher: protected PolymorphicSignal
	/// The packet dispatcher is a generic interface 
	///	used extensively within LibSourcey by classes
	/// that emits the IPacket type.
{ 
public:
	IPacketDispatcher() : _packetNumber(0) {};
	virtual ~IPacketDispatcher() {};

	virtual void operator += (const PolymorphicDelegateBase& delegate)
	{ 
		add(delegate); 
	}
	
	virtual void operator -= (const PolymorphicDelegateBase& delegate) 
	{ 
		remove(delegate); 
	}
	
	virtual void operator -= (const void* klass) 
	{ 
		remove(klass); 
	}

	virtual void attach(const PolymorphicDelegateBase& delegate)
	{
		add(delegate);
	}

	virtual void detach(const PolymorphicDelegateBase& delegate) 
	{
		remove(delegate);
	}
	
	virtual void detachs(const void* klass) 
	{
		remove(klass);
	}

	virtual void onStreamState(const PacketStreamState&) 
		// If this class belongs to a PacketStream this method
		// will be called each time the stream state changes.
	{ 
		//Poco::FastMutex::ScopedLock lock(_mutex);
		_packetNumber = 0; 
	}
	
	virtual void dispatch(void* sender, IPacket& packet) 
	{
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_packetNumber++;
		}
		PolymorphicSignal::dispatch(sender, packet); 
	}

	virtual UInt32 packetNumber() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _packetNumber;
	}

	virtual int refCount() const
	{
		return PolymorphicSignal::refCount();
	}

protected:	
	UInt32 _packetNumber;

	//mutable Poco::FastMutex	_mutex;
};


typedef std::vector<IPacketDispatcher*> PacketBroadcasterList;


} // namespace Sourcey


#endif // SOURCEY_IPacketDispatcher_H




	/*
	//PolymorphicSignal BroadcastPacket;
		// Delegates listen in on this signal to receive 
		// broadcasted packets.
	*/