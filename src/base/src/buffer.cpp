//
// LibSourcey
// Copyright(C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or(at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "scy/buffer.h"
#include "scy/util.h"
#include "scy/logger.h"
#include "scy/byteorder.h"

#include <cstddef>
#include <cstring>
#include <stdexcept>


namespace scy {


//
// Bit Reader
//


BitReader::BitReader(const ConstBuffer& pod, ByteOrder order)
{
    init(bufferCast<const char*>(pod), pod.size(), order); // copied
}


BitReader::BitReader(const Buffer& buf, ByteOrder order)
{
    init(buf.data(), buf.size(), order);
    // _buffer = &buf;
}


BitReader::BitReader(const char* bytes, std::size_t size, ByteOrder order)
{
    init(bytes, size, order);
}


void BitReader::init(const char* bytes, std::size_t size, ByteOrder order)
{
    // _buffer = nullptr;
    //_mark = 0;
    _position = 0;
    //_capacity = size;
    _limit = size;
    _order = order;
    _bytes = bytes;
    //_free = false;
}


BitReader::~BitReader()
{
}


void BitReader::seek(std::size_t val)
{
    if (val > _limit)
        throw std::out_of_range("index out of range");

    _position = val;
}


void BitReader::skip(std::size_t val)
{
    if (val > _limit)
        throw std::out_of_range("index out of range");

    _position += val;
}


std::string BitReader::toString()
{
    return std::string(current(), available());
}


std::size_t BitReader::available() const
{
    return _limit - _position;
}


std::size_t BitReader::limit() const
{
    return _limit;
}


//
// Get methods
//

void BitReader::getU8(std::uint8_t& val)
{
    get(reinterpret_cast<char*>(&val), 1);
}


void BitReader::getU16(std::uint16_t& val)
{
    std::uint16_t v;
    get(reinterpret_cast<char*>(&v), 2);
    val = (_order == ByteOrder::Network) ? networkToHost16(v) : v;
}


void BitReader::getU24(std::uint32_t& val)
{
    std::uint32_t v = 0;
    char* target = reinterpret_cast<char*>(&v);
    if (_order == ByteOrder::Network || isBigEndian())
        ++target;

    get(target, 3);
    val = (_order == ByteOrder::Network) ? networkToHost32(v) : v;
}


void BitReader::getU32(std::uint32_t& val)
{
    std::uint32_t v;
    get(reinterpret_cast<char*>(&v), 4);
    val = (_order == ByteOrder::Network) ? networkToHost32(v) : v;
}


void BitReader::getU64(std::uint64_t& val)
{
    std::uint64_t v;
    get(reinterpret_cast<char*>(&v), 8);
    val = (_order == ByteOrder::Network) ? networkToHost64(v) : v;
}


void BitReader::get(std::string& val, std::size_t len)
{
    if (len > available())
        throw std::out_of_range("index out of range");

    val.append(_bytes + _position, len);
    _position += len;
}


void BitReader::get(char* val, std::size_t len)
{
    if (len > available())
        throw std::out_of_range("index out of range");

    memcpy(val, _bytes + _position, len);
    _position += len;
}


const char BitReader::peek()
{
  	if (_limit > _position)
  		  return (const char)_bytes[_position];
  	// DebugL << "Peeking next character is NULL" << std::endl;
  	return 0;
}


const std::uint8_t BitReader::peekU8()
{
    try {
      	std::uint8_t v;
      	getU8(v);
    		_position -= 1;
    		return v;
    }
    catch (std::out_of_range&) {
      	DebugL << "Peeking std::uint8_t: NULL" << std::endl;
    }
  	return 0;
}


const std::uint16_t BitReader::peekU16()
{
    try {
      	std::uint16_t v;
      	getU16(v);
    		_position -= 2;
    		return v;
    }
    catch (std::out_of_range&) {
      	DebugL << "Peeking std::uint16_t: NULL" << std::endl;
    }

  	return 0;
}


const std::uint32_t BitReader::peekU24()
{
    try {
      	std::uint32_t v;
      	getU24(v);
    		_position -= 3;
    		return v;
    }
    catch (std::out_of_range&) {
      	DebugL << "Peeking UInt24: NULL" << std::endl;
    }

  	return 0;
}


const std::uint32_t BitReader::peekU32()
{
    try {
      	std::uint32_t v;
      	getU32(v);
    		_position -= 4;
    		return v;
    }
    catch (std::out_of_range&) {
      	DebugL << "Peeking std::uint32_t: NULL" << std::endl;
    }

  	return 0;
}


const std::uint64_t BitReader::peekU64()
{
    try {
      	std::uint64_t v;
      	getU64(v);
    		_position -= 8;
    		return v;
    }
    catch (std::out_of_range&) {
      	DebugL << "Peeking std::uint64_t: NULL" << std::endl;
    }

  	return 0;
}


//
// String parsing
//

int BitReader::skipToChar(char c)
{
    std::size_t len = 0;
    while (_limit > _position + len &&
        _bytes[_position + len] != c)
        len++;
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


int BitReader::skipWhitespace()
{
    std::size_t len = 0;
    while (_limit > _position + len &&
        _bytes[_position + len] == ' ')
        len++;
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


int BitReader::skipToNextLine()
{
    std::size_t len = 0;
    while (_limit > _position + len &&
        _bytes[_position + len] != '\n') {
        len++;
    }
    len++; // advance past newline
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


int BitReader::skipNextWord()
{
    std::size_t len = skipWhitespace();
    while (_limit > _position + len &&
        _bytes[_position + len] != ' ' &&
        _bytes[_position + len] != '\t' &&
        _bytes[_position + len] != '\n' &&
        _bytes[_position + len] != '\r')
        len++;
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


int BitReader::readToNext(std::string& val, char c)
{
    std::size_t len = 0;
    while (_limit > _position + len &&
        _bytes[_position + len] != c)
        len++;
    val.append(_bytes + _position, len);
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


int BitReader::readNextWord(std::string& val)
{
    std::size_t len = skipWhitespace();
    while (_limit > _position + len &&
        _bytes[_position + len] != ' ' &&
        _bytes[_position + len] != '\t' &&
        _bytes[_position + len] != '\n' &&
        _bytes[_position + len] != '\r')
        len++;
    val.append(_bytes + _position, len);
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


int BitReader::readNextNumber(unsigned int& val)
{
    std::size_t len = skipWhitespace();
    while (_limit > _position + len &&
        _bytes[_position + len] != ' ' &&
        _bytes[_position + len] != '\t' &&
        _bytes[_position + len] != '\n' &&
        _bytes[_position + len] != '\r')
        len++;
    val = util::strtoi<std::uint32_t>(std::string(_bytes + _position, len));
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


int BitReader::readLine(std::string& val)
{
    std::size_t len = 0;
    while (_limit > _position + len &&
        _bytes[_position + len] != '\n')
        len++;
    val.append(_bytes + _position, len);
    len++; // advance past newline
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


//
// Bit Writer
//


BitWriter::BitWriter(MutableBuffer& pod, ByteOrder order)
{
    init(bufferCast<char*>(pod), pod.size(), order); // copied
}


BitWriter::BitWriter(char* bytes, std::size_t size, ByteOrder order)
{
    init(bytes, size, order);
}


BitWriter::BitWriter(Buffer& buf, ByteOrder order)
{
    init(buf.data(), buf.size(), order);
    // _buffer = &buf;
}


void BitWriter::init(char* bytes, std::size_t size, ByteOrder order)
{
    //_vector = nullptr;
    // _buffer = nullptr;
    _position = 0;
    _limit = size;
    //_capacity = size;
    _order = order;
    _bytes = bytes;
    //_free = false;
}


BitWriter::~BitWriter()
{
}


void BitWriter::skip(std::size_t val)
{
    if (_position + val > _limit)
        throw std::out_of_range("index out of range");

    _position += val;
}


void BitWriter::seek(std::size_t val)
{
    if (val > _limit)
        throw std::out_of_range("index out of range");

    _position = val;
}


std::string BitWriter::toString()
{
    return std::string(begin(), position());
}


std::size_t BitWriter::available() const
{
    return _limit - _position;
}


std::size_t BitWriter::limit() const
{
    return _limit;
}


//
// Write functions
//

void BitWriter::putU8(std::uint8_t val)
{
    put(reinterpret_cast<const char*>(&val), 1);
}


void BitWriter::putU16(std::uint16_t val)
{
    std::uint16_t v = (_order == ByteOrder::Network) ? hostToNetwork16(val) : val;
    put(reinterpret_cast<const char*>(&v), 2);
}


void BitWriter::putU24(std::uint32_t val)
{
    std::uint32_t v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
    char* start = reinterpret_cast<char*>(&v);
    if (_order == ByteOrder::Network || isBigEndian())
        ++start;

    put(start, 3);
}


void BitWriter::putU32(std::uint32_t val)
{
    std::uint32_t v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
    put(reinterpret_cast<const char*>(&v), 4);
}


void BitWriter::putU64(std::uint64_t val)
{
    std::uint64_t v = (_order == ByteOrder::Network) ? hostToNetwork64(val) : val;
    put(reinterpret_cast<const char*>(&v), 8);
}


void BitWriter::put(const std::string& val)
{
    put(val.c_str(), val.size());
}


void BitWriter::put(const char* val, std::size_t len)
{
    // // Write to dynamic buffer (not compatible with BitReader methods)
    // if (_buffer) {;
    //
    //     //_buffer->resize(std::max<std::size_t>(3 * len / 2, 2048));
    //     _buffer->insert(_buffer->end(), val, val + len);
    //     _bytes = _buffer->data();
    //     _limit = _buffer->size();
    //     _position += len;
    // }
    //
    // Write to fixed size buffer
    // else {
        if ((_position + len) > _limit)
            throw std::out_of_range("insufficient buffer capacity");

        std::cout << "put fixed " << _bytes << ", len " << len << std::endl;
        memcpy(_bytes + _position, val, len);
        _position += len;
    // }
}


//
// Update functions
//


bool BitWriter::updateU8(std::uint8_t val, std::size_t pos)
{
    return update(reinterpret_cast<const char*>(&val), 1, pos);
}


bool BitWriter::updateU16(std::uint16_t val, std::size_t pos)
{
    std::uint16_t v = (_order == ByteOrder::Network) ? hostToNetwork16(val) : val;
    return update(reinterpret_cast<const char*>(&v), 2, pos);
}


bool BitWriter::updateU24(std::uint32_t val, std::size_t pos)
{
    std::uint32_t v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
    char* start = reinterpret_cast<char*>(&v);
    if (_order == ByteOrder::Network || isBigEndian())
        ++start;

    return update(start, 3, pos);
}


bool BitWriter::updateU32(std::uint32_t val, std::size_t pos)
{
    std::uint32_t v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
    return update(reinterpret_cast<const char*>(&v), 4, pos);
}


bool BitWriter::updateU64(std::uint64_t val, std::size_t pos)
{
    std::uint64_t v = (_order == ByteOrder::Network) ? hostToNetwork64(val) : val;
    return update(reinterpret_cast<const char*>(&v), 8, pos);
}


bool BitWriter::update(const std::string& val, std::size_t pos)
{
    return update(val.c_str(), val.size(), pos);
}


bool BitWriter::update(const char* val, std::size_t len, std::size_t pos)
{
    if ((pos + len) > available())
        return false;

    memcpy(_bytes + pos, val, len);
    return true;
}


} // namespace scy
