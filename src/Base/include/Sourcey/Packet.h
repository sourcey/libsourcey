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
#include "Sourcey/Polymorphic.h"
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


class IPacket: public Polymorphic
	/// The base packet class which can be extended by each
	/// protocol implementation for polymorphic callbacks
	/// using the PacketStreamSource and friends.
{ 
public:
	void* source;
		/// Optional packet source pointer reference so delegates
		/// along the signal chain can determine the packet origin.
		/// Usually a subclass of PacketStreamSource.

	IPacketInfo* info;
		/// Optional extra information about the packet.
		/// This pointer is managed by the packet.

	void* opaque;
		/// Optional client data pointer.
		/// This pointer is not managed by the packet.
	
	IPacket(void* source = nullptr, IPacketInfo* info = nullptr, void* opaque = nullptr) :
		source(source), info(info), opaque(opaque) {}
	
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
	/// RawPacket is the default data packet type which consists
	/// of an optionally managed data pointer and a size value.
	///
{	
public:
	RawPacket(char* data = nullptr, size_t size = 0, void* opaque = nullptr) : 
		IPacket(nullptr, nullptr, opaque), _data(data), _size(size), managePointer(false)
	{
	}

	RawPacket(const char* data, size_t size = 0, void* opaque = nullptr) : 
		IPacket(nullptr, nullptr, opaque), _data(nullptr), _size(size), managePointer(true)
	{
		// Copy const data
		cloneData(data, size);
	}

	RawPacket(void* source, IPacketInfo* info, void* opaque = nullptr, char* data = nullptr, size_t size = 0) : 
		IPacket(source, info, opaque), _data(data), _size(size), managePointer(false)
	{
	}

	RawPacket(const RawPacket& that) : 
		IPacket(that), _data(nullptr), _size(0), managePointer(true) //that.managePointer
	{		
		// If data pointer is assigned we memcpy
		//
		// TODO: Use some kind of reference counted buffer wrapper
		// so we don't need to force memcpy of referenced data.
		cloneData(that._data, that._size);
	}
	
	virtual ~RawPacket() 
	{
		if (managePointer && _data)
			delete [] _data;
	}

	virtual IPacket* clone() const 
	{
		return new RawPacket(*this);
	}

	virtual void setData(char* data, size_t size) 
	{
		assert(size > 0);

		// If managePointer is true we memcpy data
		if (managePointer)
			cloneData(data, size);

		// Otherwise we just reference the ptr
		else {
			_data = data;
			_size = size; 
		}
	}

	virtual void cloneData(const char* data, size_t size) 
	{
		traceL("RawPacket", this) << "Cloning: " << size << std::endl;

		assert(managePointer);
		assert(size > 0);
		if (_data)
			delete [] _data;
		_size = size;
		_data = new char[size];
		std::memcpy(_data, data, size);
	}	

	virtual bool read(Buffer& buf) 
	{ 
		setData(buf.data(), buf.available());
		return true;
	}

	virtual void write(Buffer& buf) const 
	{	
		buf.put(_data, _size); 
	}

	virtual char* data() const 
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

	bool managePointer;
		/// Set this flag to true to 
		/// delete packet data on destruction.
	
	char* _data;
	size_t _size;
};


} // namespace scy


#endif // SOURCEY_PACKET_H


		/*
		// Clone data if memory is managed
		if (managePointer) {
			char* data = new char[size];
			std::memcpy(data, that._data, that._size);
			_data = data;
		}
		*/

		/*
		if (managePointer) {
			char* data = new char[buf.available()];
			std::memcpy(data, buf.begin(), buf.available());
			_data = data;
		}
		else {
			_data = buf.begin();
			_size = buf.available(); 
		}
		*/

			/*
			// Delete old buffer
			if (_data)
				delete _data;			
			_data = new char[size];
			std::memcpy(_data, data, size);
			//char* data = new char[buf.available()];
			//std::memcpy(data, buf.begin(), buf.available());
			//_data = data;
			*/