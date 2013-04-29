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


#ifndef SOURCEY_PACKET_H
#define SOURCEY_PACKET_H


#include "Sourcey/Base.h"
#include "Sourcey/IPolymorphic.h"
#include "Sourcey/Buffer.h"
#include "Sourcey/Logger.h"

#include <list>


namespace Sourcey {
	
	
struct IPacketInfo
	/// An abstract interface for packet sources to
	/// provide extra information about packets.
{ 
	IPacketInfo() {}; 
	virtual ~IPacketInfo() {}; 

	virtual IPacketInfo* clone() const = 0;
};


struct IPacket: public IPolymorphic
	/// The base packet class which can be extended by each
	/// protocol implementation for polymorphic callbacks
	/// using the PacketEmitter and friends.
{ 
	void* source;
		// Optional packet source pointer reference. 
		// Usually a subclass of PacketEmitter.

	IPacketInfo* info;
		// Optional extra information about the packet.
		// This pointer is managed by the packet.
		// @see IPacketInfo.

	void* opaque;
		// Optional client data pointer.
		// This pointer is not managed by the packet.
	
	IPacket(void* source = NULL, IPacketInfo* info = NULL) :
		source(source), info(info), opaque(NULL) {}
	
	IPacket(const IPacket& r) : 
		source(r.source),
		info(r.info ? r.info->clone() : NULL),
		opaque(r.opaque)
	{
	}
		
	IPacket& operator = (const IPacket& r) 
	{
		source = r.source;
		info = (r.info ? r.info->clone() : NULL);
		opaque = r.opaque;
		return *this;
	}

	virtual ~IPacket() 
	{
		if (info)
			delete info;
	}
	
	virtual IPacket* clone() const = 0;	
	virtual bool read(Buffer&) = 0;
	virtual void write(Buffer&) const = 0;

	virtual size_t size() const
		/// The size method is provided for packet types
		/// which can easily determine their data size. 
		/// Knowing the packet size beforehand will speed
		/// up buffer creation and copying.
		/// On the other hand some dynamically generated 
		/// packet types may choose not to override this 
		/// method because parsing the packet twice would
		/// be slower than allocating memory.
	{
		return 0;
	}

	virtual const char* className() const = 0;
	virtual void print(std::ostream& os) const 
	{ 
		os << className() << std::endl; 
	}
};


struct DataPacket: public IPacket 
	/// The default data packet type.
	/// Data packets consist of an optionally
	/// managed data pointer, and a size value.
{	
	DataPacket(unsigned char* data = NULL, size_t size = 0, bool freeOnDestroy = false) : 
		_data(data), _size(size), freeOnDestroy(freeOnDestroy)
	{
	}

	DataPacket(const DataPacket& r) : 
		IPacket(r), 
		_data(r._data), 
		_size(r._size), 
		freeOnDestroy(r.freeOnDestroy)
	{
	}
	
	virtual ~DataPacket() 
	{
		if (freeOnDestroy && _data)
			delete _data;
	}

	virtual IPacket* clone() const 
	{
		return new DataPacket(*this);
	}	

	virtual bool read(Buffer& buf) 
	{ 
		_data = (unsigned char*)buf.data(); 
		_size = buf.size(); 
		return true;
	}

	virtual void write(Buffer& buf) const 
	{	
		buf.writeBytes((const char*)_data, _size); 
	}

	virtual void setData(unsigned char* data) 
	{ 
		_data = data;
	}

	virtual unsigned char* data() const 
	{ 
		return _data; 
	}

	virtual void setSize(size_t size) 
	{ 
		_size = size;
	}

	virtual size_t size() const 
	{ 
		return _size; 
	}
	
	virtual void print(std::ostream& os) const 
	{ 
		os << className() << std::endl; 
	}
	
	virtual const char* className() const 
	{ 
		return "DataPacket"; 
	}

	bool freeOnDestroy;
		/// Set this flag to true to 
		/// delete packet data on destruction.
	
protected:
	unsigned char* _data;
	size_t _size;
};


} // namespace Sourcey


#endif // SOURCEY_PACKET_H
