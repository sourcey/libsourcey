///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Packet_H
#define SCY_Packet_H


#include "scy/base.h"
#include "scy/bitwise.h"
#include "scy/buffer.h"
#include "scy/interface.h"
#include "scy/logger.h"

#include <cstdint>
#include <cstring> // memcpy


namespace scy {


/// An abstract interface for packet sources to
/// provide extra information about packets.
struct IPacketInfo
{
    IPacketInfo() = default;
    virtual ~IPacketInfo() = default;

    virtual IPacketInfo* clone() const = 0;
};


/// The basic packet type which is passed around the LibSourcey system.
/// IPacket can be extended for each protocol to enable polymorphic
/// processing and callbacks using PacketStream and friends.
class Base_API IPacket
{
public:
    IPacket(void* source = nullptr, void* opaque = nullptr,
            IPacketInfo* info = nullptr, unsigned flags = 0)
        : source(source)
        , opaque(opaque)
        , info(info)
        , flags(flags)
    {
    }

    IPacket(const IPacket& r)
        : source(r.source)
        , opaque(r.opaque)
        , info(r.info ? r.info->clone() : nullptr)
        , flags(r.flags)
    {
    }

    IPacket& operator=(const IPacket& r)
    {
        source = r.source;
        opaque = r.opaque;
        info = (r.info ? r.info->clone() : nullptr);
        flags = r.flags;
        return *this;
    }

    virtual IPacket* clone() const = 0;

    virtual ~IPacket()
    {
        if (info)
            delete info;
    }

    /// Packet source pointer reference which enables processors
    /// along the signal chain can determine the packet origin.
    /// Often a subclass of PacketStreamSource.
    void* source;

    /// Optional client data pointer.
    /// This pointer is not managed by the packet.
    void* opaque;

    /// Optional extra information about the packet.
    /// This pointer is managed by the packet.
    IPacketInfo* info;

    /// Provides basic information about the packet.
    Bitwise flags;

    /// Read/parse to the packet from the given input buffer.
    /// The number of bytes read is returned.
    virtual ssize_t read(const ConstBuffer&) = 0;

    /// Copy/generate to the packet given output buffer.
    /// The number of bytes written can be obtained from the buffer.
    ///
    /// Todo: It may be prefferable to use our pod types here
    /// instead of buffer input, but the current codebase requires
    /// that the buffer be dynamically resizable for some protocols...
    ///
    virtual void write(Buffer&) const = 0;

    /// The size of the packet in bytes.
    ///
    /// This is the nember of bytes that will be written on a call
    /// to write(), but may not be the number of bytes that will be
    /// consumed by read().
    virtual size_t size() const { return 0; };

    virtual bool hasData() const { return data() != nullptr; }

    /// The packet data pointer for buffered packets.
    virtual char* data() const { return nullptr; }

    /// The const packet data pointer for buffered packets.
    virtual const void* constData() const { return data(); }
    virtual const char* className() const = 0;
    virtual void print(std::ostream& os) const
    {
        os << className() << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& stream, const IPacket& p)
    {
        p.print(stream);
        return stream;
    }
};


/// Packet for sending bitwise flags along the packet stream.
class Base_API FlagPacket : public IPacket
{
public:
    FlagPacket(unsigned flags = 0)
        : IPacket(nullptr, nullptr, nullptr, flags)
    {
    }

    virtual IPacket* clone() const override
    {
        return new FlagPacket(*this);
    }

    FlagPacket(const FlagPacket& that)
        : IPacket(that)
    {
    }

    virtual ~FlagPacket() = default;

    virtual ssize_t read(const ConstBuffer&) override { return true; }

    virtual void write(Buffer&) const override {}

    virtual const char* className() const override { return "FlagPacket"; }
};


/// RawPacket is the default data packet type which consists
/// of an optionally managed char pointer and a size value.
class Base_API RawPacket : public IPacket
{
public:
    RawPacket(char* data = nullptr, size_t size = 0, unsigned flags = 0,
              void* source = nullptr, void* opaque = nullptr,
              IPacketInfo* info = nullptr)
        : IPacket(source, opaque, info, flags)
        , _data(data)
        , _size(size)
        , _free(false)
    {
    }

    RawPacket(const char* data, size_t size = 0, unsigned flags = 0,
              void* source = nullptr, void* opaque = nullptr,
              IPacketInfo* info = nullptr)
        : IPacket(source, opaque, info, flags)
        , _data(nullptr)
        , _size(size)
        , _free(true)
    {
        copyData(data, size); // copy const data
    }

    RawPacket(const RawPacket& that)
        : IPacket(that)
        , _data(nullptr)
        , _size(0)
        , _free(true)
    {
        // Copy assigned data and set the free flag
        // Todo: Use a simple reference counted buffer wrapper
        // so we don't need to force memcpy here.
        copyData(that._data, that._size);
    }

    virtual ~RawPacket()
    {
        if (_data && _free)
            delete[] _data;
    }

    virtual IPacket* clone() const override
    {
        return new RawPacket(*this);
    }

    // virtual void setData(char* data, size_t size)
    // {
    //    assert(size > 0);
    //    if (_free) // copy data if reuqested
    //        copyData(data, size);
    //    else { // otherwise just assign the pointer
    //        _data = data;
    //        _size = size;
    //    }
    // }

    virtual void copyData(const void* data, size_t size)
    {
        // traceL("RawPacket", this) << "Cloning: " << size << std::endl;
        // assert(_free);
        if (data && size > 0) {
            if (_data && _free)
                delete[] _data;
            _size = size;
            _data = new char[size];
            _free = true;
            std::memcpy(_data, data, size);
        }
    }

    virtual ssize_t read(const ConstBuffer& buf) override
    {
        copyData(bufferCast<const char*>(buf), buf.size());
        return buf.size();
    }

    virtual void write(Buffer& buf) const override
    {
        // buf.insert(a.end(), b.begin(), b.end());
        // buf.append(_data, _size);
        buf.insert(buf.end(), _data, _data + _size);
    }

    virtual char* data() const override { return _data; }

    // virtual char* cdata() const { return static_cast<char*>(_data); }

    virtual size_t size() const override { return _size; }

    virtual const char* className() const override { return "RawPacket"; }

    bool ownsBuffer() const { return _free; }

    void assignDataOwnership() { _free = true; }

protected:
    char* _data;
    size_t _size;
    bool _free;
};


inline RawPacket rawPacket(const MutableBuffer& buf, unsigned flags = 0,
                           void* source = nullptr, void* opaque = nullptr,
                           IPacketInfo* info = nullptr)
{
    return RawPacket(bufferCast<char*>(buf), buf.size(), flags, source, opaque, info);
}

inline RawPacket rawPacket(const ConstBuffer& buf, unsigned flags = 0,
                           void* source = nullptr, void* opaque = nullptr,
                           IPacketInfo* info = nullptr)
{
    return RawPacket(bufferCast<const char*>(buf), buf.size(), flags, source, opaque, info); // copy const data
}

inline RawPacket rawPacket(char* data = nullptr, size_t size = 0,
                           unsigned flags = 0, void* source = nullptr,
                           void* opaque = nullptr, IPacketInfo* info = nullptr)
{
    return RawPacket(data, size, flags, source, opaque, info);
}

inline RawPacket rawPacket(const char* data = nullptr, size_t size = 0,
                           unsigned flags = 0, void* source = nullptr,
                           void* opaque = nullptr, IPacketInfo* info = nullptr)
{
    return RawPacket(data, size, flags, source, opaque, info); // copy const data
}


} // namespace scy


#endif // SCY_Packet_H


/// @\}
