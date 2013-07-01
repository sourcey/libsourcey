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


#ifndef SOURCEY_PACKET_H
#define SOURCEY_PACKET_H


#include "Sourcey/Types.h"
#include "Sourcey/IPolymorphic.h"
#include "Sourcey/Buffer.h"
#include "Sourcey/Logger.h"

#include <list>


namespace scy {
	
	
struct IPacketInfo
	/// An abstract interface for packet sources to
	/// provide extra information about packets.
{ 
	IPacketInfo() {}; 
	virtual ~IPacketInfo() {}; 

	virtual IPacketInfo* clone() const = 0;
};


class IPacket: public IPolymorphic
	/// The base packet class which can be extended by each
	/// protocol implementation for polymorphic callbacks
	/// using the PacketEmitter and friends.
{ 
public:
	void* source;
		/// Optional packet source pointer reference so delegates
		/// along the signal chain can determine the packet origin.
		/// Usually a subclass of PacketEmitter.

	IPacketInfo* info;
		/// Optional extra information about the packet.
		/// This pointer is managed by the packet.

	void* opaque;
		/// Optional client data pointer.
		/// This pointer is not managed by the packet.
	
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

	virtual size_t size() const = 0;

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
	/// The default data packet type.
	/// Data packets consist of an optionally
	/// managed data pointer, and a size value.
{	
public:
	RawPacket(const char* data = NULL, size_t size = 0) : 
		_data(data), _size(size), freeOnDestroy(false)
	{
	}

	RawPacket(void* source, IPacketInfo* info, const char* data = NULL, size_t size = 0) : 
		IPacket(source, info), _data(data), _size(size), freeOnDestroy(false)
	{
	}

	RawPacket(const RawPacket& that) : 
		IPacket(that), 
		_data(that._data), 
		_size(that._size), 
		freeOnDestroy(that.freeOnDestroy)
	{
		// Clone data if memory is managed
		if (freeOnDestroy) {
			char* data = new char[that._size];
			std::memcpy(data, that._data, that._size);
			_data = data;
		}
	}
	
	virtual ~RawPacket() 
	{
		if (freeOnDestroy && _data)
			delete _data;
	}

	virtual IPacket* clone() const 
	{
		return new RawPacket(*this);
	}	

	virtual bool read(Buffer& buf) 
	{ 
		if (freeOnDestroy) {
			char* data = new char[buf.size()];
			std::memcpy(data, buf.begin(), buf.size());
			_data = data;
		}
		else {
			_data = (char*)buf.begin();
			_size = buf.size(); 
		}
		return true;
	}

	virtual void write(Buffer& buf) const 
	{	
		buf.write(_data, _size); 
	}

	virtual const char* data() const 
	{ 
		return _data; 
	}

	virtual size_t size() const 
	{ 
		return _size; 
	}
	
	virtual const char* className() const 
	{ 
		return "RawPacket"; 
	}

	bool freeOnDestroy;
		/// Set this flag to true to 
		/// delete packet data on destruction.
	
	const char* _data;
	size_t _size;
};


} // namespace scy


#endif // SOURCEY_PACKET_H
