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


#ifndef SOURCEY_Writable_H
#define SOURCEY_Writable_H


#include "Sourcey/Packet.h"

#include <vector>


namespace scy {
	
	


} // namespace scy


#endif // SOURCEY_Writable_H

	

	/*
		//Mutex::ScopedLock lock(_mutex);
		//_packetNumber = 0; 

	Writable() : _packetNumber(0) {};
	virtual ~Writable() {};

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
			Mutex::ScopedLock lock(_mutex);
			_packetNumber++;
		}
		PacketSignal::emit(sender, packet); 
	}

	virtual UInt32 packetNumber() const
	{
		Mutex::ScopedLock lock(_mutex);
		return _packetNumber;
	}

	virtual int refCount() const
	{
		return PacketSignal::refCount();
	}

protected:	
	UInt32 _packetNumber;

	//mutable Mutex	_mutex;
	*/


	/*
	//PacketSignal BroadcastPacket;
		// Delegates listen in on this signal to receive 
		// broadcasted packets.
	*/