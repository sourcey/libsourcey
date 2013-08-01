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


#ifndef SOURCEY_Packet_H
#define SOURCEY_Packet_H


#include "Sourcey/Types.h"
#include "Sourcey/Flaggable.h"
#include "Sourcey/Polymorphic.h"
#include "Sourcey/Buffer.h"
#include "Sourcey/Logger.h"

#include <list>


namespace scy {
	
	
struct IPacketInfo
	// An abstract interface for packet sources to
	// provide extra information about packets.
{ 
	IPacketInfo() {}; 
	virtual ~IPacketInfo() {}; 

	virtual IPacketInfo* clone() const = 0;
};


class IPacket: public Polymorphic
	// The base packet type which is passed around the LibSourcey system.
	// IPacket can be extended for each protocol to enable polymorphic
	// processing and callbacks using PacketStream and friends.
{ 
public:
	void* source;
		// Optional packet source pointer reference so delegates
		// along the signal chain can determine the packet origin.
		// Usually a subclass of PacketStreamSource.

	IPacketInfo* info;
		// Optional extra information about the packet.
		// This pointer is managed by the packet.

	void* opaque;
		// Optional client array pointer.
		// This pointer is not managed by the packet.

	Flaggable flags;
		// Packet flags are used to provide basic information
		// about the packet.	

	enum ProcessorFlags
		// Packet processor flags tell processors how to handle
		// the packet.
	{
		Fixed = 0x00,  // This packet should not be modified by downstream processors.
		Final = 0x01,  // This is that last packet.
		Managed = 0x02, // This packet will take ownership of any referenced memory.
	};
	
	IPacket(void* source = NULL, unsigned flags = 0, IPacketInfo* info = NULL, void* opaque = NULL) :
		source(source), flags(flags), info(info), opaque(opaque){}
	
	IPacket(const IPacket& r) : 
		source(r.source),
		flags(r.flags),
		info(r.info ? r.info->clone() : NULL),
		opaque(r.opaque)
	{
	}
		
	IPacket& operator = (const IPacket& r) 
	{
		source = r.source;
		flags = r.flags;
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
	
	virtual size_t size() const = 0;

	virtual char* array() const 
	{ 
		return 0;
	}

	virtual bool hasArray() const
	{
		return array() != 0;
	}

	virtual const char* className() const = 0;
	virtual void print(std::ostream& os) const 
	{ 
		os << className() << std::endl; 
	}
	
    friend std::ostream& operator << (std::ostream& stream, const IPacket& pkt) 
	{
		pkt.print(stream);
		return stream;
    }
};


class RawPacket: public IPacket 
	// RawPacket is the default data packet type which consists
	// of an optionally managed char pointer and a size value.
	//
{	
public:
	RawPacket(char* array = NULL, size_t size = 0, unsigned flags = 0) : 
		IPacket(NULL, flags), _array(array), _size(size)
	{
	}

	RawPacket(const char* array, size_t size = 0, unsigned flags = 0) : 
		IPacket(NULL, flags |= IPacket::Managed), _array(NULL), _size(size)
	{
		// Copy const array
		copyArray(array, size);
	}

	RawPacket(void* source, char* array = NULL, size_t size = 0, unsigned flags = 0, IPacketInfo* info = NULL, void* opaque = NULL) : 
		IPacket(source, flags, info, opaque), _array(array), _size(size)
	{
	}

	RawPacket(void* source, const char* array = NULL, size_t size = 0, unsigned flags = 0, IPacketInfo* info = NULL, void* opaque = NULL) : 
		IPacket(source, flags |= IPacket::Managed, info, opaque), _array(NULL), _size(size)
	{
		// Copy const array
		copyArray(array, size);
	}

	RawPacket(const RawPacket& that) : 
		IPacket(that), _array(NULL), _size(0)
	{		
		// Copy assigned data and set the Managed flag
		// TODO: Use a simple reference counted buffer wrapper
		// so we don't need to force memcpy here.
		setManaged();
		copyArray(that._array, that._size);
	}
	
	virtual ~RawPacket() 
	{
		if (isManaged() && _array)
			delete [] _array;
	}

	virtual IPacket* clone() const 
	{
		return new RawPacket(*this);
	}

	virtual void setArray(char* array, size_t size) 
	{
		assert(size > 0);

		// Copy data if the Managed flag is set
		if (isManaged())
			copyArray(array, size);

		// Otherwise just reference it
		else {
			_array = array;
			_size = size; 
		}
	}

	virtual void copyArray(const char* array, size_t size) 
	{
		//traceL("RawPacket", this) << "Cloning: " << size << std::endl;

		assert(isManaged());
		assert(size > 0);
		if (_array)
			delete [] _array;
		_size = size;
		_array = new char[size];
		std::memcpy(_array, array, size);
	}	

	virtual bool read(Buffer& buf) 
	{ 
		setArray(buf.data(), buf.available());
		return true;
	}

	virtual void write(Buffer& buf) const 
	{	
		buf.put(_array, _size); 
	}

	virtual char* array() const 
	{ 
		return _array; 
	}

	virtual char* data() const 
		// legacy compatibility
	{ 
		return _array; 
	}

	virtual size_t size() const 
	{ 
		return _size; 
	}
	
	virtual const char* className() const 
	{ 
		return "RawPacket"; 
	}

	virtual bool isManaged() const
	{
		return flags.has(IPacket::Managed);
	}

	virtual void setManaged()
	{
		flags.set(IPacket::Managed);
	}
	
	char* _array;
	size_t _size;
};


} // namespace scy


#endif // SOURCEY_Packet_H