///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Buffer_H
#define SCY_Buffer_H


#include "scy/byteorder.h"
#include "scy/memory.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>


namespace scy {


typedef std::vector<char> Buffer;


//
// Mutable Buffer
//


/// The MutableBuffer class provides a safe representation of a
/// buffer that can be modified. It does not own the underlying
/// data, and so is cheap to copy or assign.
class MutableBuffer
{
public:
    /// Construct an empty buffer.
    MutableBuffer()
        : _data(0)
        , _size(0)
    {
    }

    /// Construct a buffer to represent the given memory range.
    MutableBuffer(void* data, std::size_t size)
        : _data(data)
        , _size(size)
    {
    }

    void* data() const { return _data; }
    std::size_t size() const { return _size; }

    /// String methods
    char* cstr() const { return reinterpret_cast<char*>(_data); }
    std::string str() const { return std::string(cstr(), size()); }

private:
    void* _data;
    std::size_t _size;
};


// Warning: The following functions permit violations of type safety,
// so uses of it in application code should be carefully considered.


template <typename T>
inline MutableBuffer mutableBuffer(T data, std::size_t size)
{
    return MutableBuffer(reinterpret_cast<void*>(data), size);
}

inline MutableBuffer mutableBuffer(std::string& str)
{
    return MutableBuffer(reinterpret_cast<void*>(&str[0]),
                         str.size()); // std::string is contiguous as of C++11
}


inline MutableBuffer mutableBuffer(const std::string& str)
{
    return MutableBuffer(reinterpret_cast<void*>(const_cast<char*>(&str[0])),
                         str.size()); // careful!
}


template <typename T>
inline MutableBuffer mutableBuffer(const std::vector<T>& vec)
{
    return MutableBuffer(reinterpret_cast<void*>(const_cast<T>(&vec[0])),
                         vec.size()); // careful!
}


inline MutableBuffer mutableBuffer(Buffer& buf)
{
    return MutableBuffer(reinterpret_cast<void*>(buf.data()), buf.size());
}


inline MutableBuffer mutableBuffer(const Buffer& buf)
{
    return MutableBuffer(reinterpret_cast<void*>(const_cast<char*>(buf.data())),
                         buf.size());
}


//
// Const Buffer
//

/// The ConstBuffer class provides a safe representation of a
/// buffer that cannot be modified. It does not own the underlying
/// data, and so is cheap to copy or assign.
class ConstBuffer
{
public:
    /// Construct an empty buffer.
    ConstBuffer()
        : _data(0)
        , _size(0)
    {
    }

    /// Construct a buffer to represent the given memory range.
    ConstBuffer(const void* data, std::size_t size)
        : _data(data)
        , _size(size)
    {
    }

    /// Construct a non-modifiable buffer from a modifiable one.
    ConstBuffer(const MutableBuffer& b)
        : _data(b.data())
        , _size(b.size())
    {
    }

    const void* data() const { return _data; }
    std::size_t size() const { return _size; }

    /// String methods
    const char* cstr() const { return reinterpret_cast<const char*>(_data); }
    std::string str() const { return std::string(cstr(), size()); }

private:
    const void* _data;
    std::size_t _size;
};


template <typename T> inline ConstBuffer constBuffer(T data, std::size_t size)
{
    return ConstBuffer(reinterpret_cast<const void*>(data), size);
}

inline ConstBuffer constBuffer(const std::string& str)
{
    return ConstBuffer(reinterpret_cast<const void*>(&str[0]),
                       str.size()); // careful!
}

template <typename T> inline ConstBuffer constBuffer(const std::vector<T>& vec)
{
    return ConstBuffer(reinterpret_cast<const void*>(&vec[0]),
                       vec.size()); // careful!
}

inline ConstBuffer constBuffer(const MutableBuffer& buf)
{
    return ConstBuffer(buf.data(), buf.size());
}

template <typename T> inline ConstBuffer constBuffer(Buffer& buf)
{
    return ConstBuffer(reinterpret_cast<void*>(buf.data()), buf.size());
}

template <typename T> inline ConstBuffer constBuffer(const Buffer& buf)
{
    return ConstBuffer(reinterpret_cast<void*>(const_cast<char*>(buf.data())),
                       buf.size());
}


//
// Buffer Cast
//


/// Cast a non-modifiable buffer to a specified pointer to POD type.
template <typename PointerToPodType>
inline PointerToPodType bufferCast(const MutableBuffer& b)
{
    return static_cast<PointerToPodType>(b.data());
}


/// Cast a non-modifiable buffer to a specified pointer to POD type.
template <typename PointerToPodType>
inline PointerToPodType bufferCast(const ConstBuffer& b)
{
    return static_cast<PointerToPodType>(b.data());
}


//
// Bit Reader
//


/// A BitReader for reading binary streams.
class BitReader
{
public:
    BitReader(const char* bytes, std::size_t size,
              ByteOrder order= ByteOrder::Network);
    BitReader(const Buffer& buf, ByteOrder order= ByteOrder::Network);
    BitReader(const ConstBuffer& pod, ByteOrder order= ByteOrder::Network);
    ~BitReader();

    /// Reads a value from the BitReader.
    /// Returns false if there isn't enough data left for the specified type.
    /// Throws a std::out_of_range exception if reading past the limit.
    void get(char* val, std::size_t len);
    void get(std::string& val, std::size_t len);
    void getU8(std::uint8_t& val);
    void getU16(std::uint16_t& val);
    void getU24(std::uint32_t& val);
    void getU32(std::uint32_t& val);

    void getU64(std::uint64_t& val);

    const char peek();
    const std::uint8_t peekU8();
    const std::uint16_t peekU16();
    const std::uint32_t peekU24();
    const std::uint32_t peekU32();

    /// Peeks data from the BitReader.
    /// -1 is returned if reading past boundary.
    const std::uint64_t peekU64();

    /// String parsing methods.

    int skipToChar(char c);
    int skipWhitespace();
    int skipToNextLine();
    int skipNextWord();
    int readNextWord(std::string& val);
    int readNextNumber(unsigned int& val);
    int readLine(std::string& val);
    int readToNext(std::string& val, char c);

    /// Set position pointer to absolute position.
    /// Throws a std::out_of_range exception if the value exceeds the limit.
    void seek(std::size_t val);

    /// Set position pointer to relative position.
    /// Throws a std::out_of_range exception if the value exceeds the limit.
    void skip(std::size_t size);

    /// Returns the read limit.
    std::size_t limit() const;

    /// Returns the current read position.
    std::size_t position() const { return _position; }

    /// Returns the number of elements between the current position and the
    /// limit.
    std::size_t available() const;

    const char* begin() const { return _bytes; }
    const char* current() const { return _bytes + _position; }

    ByteOrder order() const { return _order; }

    std::string toString();

    friend std::ostream& operator<<(std::ostream& stream, const BitReader& buf)
    {
        return stream.write(buf.current(), buf.position());
    }

private:
    void init(const char* bytes, std::size_t size, ByteOrder order); // nocopy

    std::size_t _position;
    std::size_t _limit;
    const char* _bytes;
    ByteOrder _order;
    Buffer* _buffer;
};


//
// Bit Writer
//


/// A BitWriter for reading/writing binary streams.
///
/// Note that when using the constructor with the Buffer reference
/// as an argument, the writer will dynamically expand the given buffer
/// when writing passed the buffer capacity.
/// All other cases will throw a std::out_of_range error when writing
/// past the buffer capacity.
class BitWriter
{
public:
    BitWriter(char* bytes, std::size_t size,
              ByteOrder order= ByteOrder::Network);
    BitWriter(Buffer& buf, ByteOrder order= ByteOrder::Network);
    BitWriter(MutableBuffer& pod, ByteOrder order= ByteOrder::Network);
    ~BitWriter();

    /// Append bytes to the buffer.
    /// Throws a std::out_of_range exception if reading past the limit.
    void put(const char* val, std::size_t len);
    void put(const std::string& val);
    void putU8(std::uint8_t val);
    void putU16(std::uint16_t val);
    void putU24(std::uint32_t val);
    void putU32(std::uint32_t val);
    void putU64(std::uint64_t val);

    /// Update a byte range.
    /// Throws a std::out_of_range exception if reading past the limit.
    bool update(const char* val, std::size_t len, std::size_t pos);
    bool update(const std::string& val, std::size_t pos);
    bool updateU8(std::uint8_t val, std::size_t pos);
    bool updateU16(std::uint16_t val, std::size_t pos);
    bool updateU24(std::uint32_t val, std::size_t pos);
    bool updateU32(std::uint32_t val, std::size_t pos);
    bool updateU64(std::uint64_t val, std::size_t pos);

    /// Set position pointer to absolute position.
    /// Throws a std::out_of_range exception if the value exceeds the limit.
    void seek(std::size_t val);

    /// Set position pointer to relative position.
    /// Throws a std::out_of_range exception if the value exceeds the limit.
    void skip(std::size_t size);

    /// Returns the write limit.
    std::size_t limit() const;

    /// Returns the current write position.
    std::size_t position() const { return _position; }

    /// Returns the number of elements between the current write position and
    /// the limit.
    std::size_t available() const;

    char* begin() { return _bytes; }
    char* current() { return _bytes + _position; }

    const char* begin() const { return _bytes; }
    const char* current() const { return _bytes + _position; }

    /// Returns written bytes as a string.
    ByteOrder order() const { return _order; }
    std::string toString();

    friend std::ostream& operator<<(std::ostream& stream, const BitWriter& wr)
    {
        return stream.write(wr.begin(), wr.position());
    }

private:
    void init(char* bytes, std::size_t size, ByteOrder order); // nocopy

    std::size_t _position;
    std::size_t _limit;
    ByteOrder _order;
    char* _bytes;
};


} // namespace scy


#endif // SCY_Buffer_H


/// @\}
