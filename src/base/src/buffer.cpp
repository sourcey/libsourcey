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
}


BitReader::BitReader(const char* bytes, std::size_t size, ByteOrder order)
{
    init(bytes, size, order);
}


void BitReader::init(const char* bytes, std::size_t size, ByteOrder order) 
{
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
    return std::string(current(), position()); 
}


size_t BitReader::available() const 
{
    return _limit - _position;
}


size_t BitReader::limit() const
{ 
    return _limit; 
} 


//
// Get methods
//

void BitReader::getU8(UInt8& val)
{
    get(reinterpret_cast<char*>(&val), 1);
}


void BitReader::getU16(UInt16& val)
{
    UInt16 v;
    get(reinterpret_cast<char*>(&v), 2);
    val = (_order == ByteOrder::Network) ? networkToHost16(v) : v;
}


void BitReader::getU24(UInt32& val)
{
    UInt32 v = 0;
    char* target = reinterpret_cast<char*>(&v);
    if (_order == ByteOrder::Network || isBigEndian())
        ++target;

    get(target, 3);
    val = (_order == ByteOrder::Network) ? networkToHost32(v) : v;
}


void BitReader::getU32(UInt32& val)
{
    UInt32 v;
    get(reinterpret_cast<char*>(&v), 4);
    val = (_order == ByteOrder::Network) ? networkToHost32(v) : v;
}


void BitReader::getU64(UInt64& val)
{
    UInt64 v;
    get(reinterpret_cast<char*>(&v), 8);
    val = (_order == ByteOrder::Network) ? networkToHost64(v) : v;
}


void BitReader::get(std::string& val, std::size_t len)
{
    if (len > limit())
        throw std::out_of_range("index out of range");

    val.append(_bytes + _position, len);
    _position += len;
}


void BitReader::get(char* val, std::size_t len)
{
    if (len > limit())
        throw std::out_of_range("index out of range");

    memcpy(val, _bytes + _position, len);
    _position += len;
}


//
// String parsing - make a separate class or free functions?
//

int BitReader::skipToChar(char c) 
{
    size_t len = 0;
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
    size_t len = 0;
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
    size_t len = 0;
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
    size_t len = skipWhitespace();
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
    size_t len = 0;
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
    size_t len = skipWhitespace();
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
    size_t len = skipWhitespace();
    while (_limit > _position + len && 
        _bytes[_position + len] != ' ' && 
        _bytes[_position + len] != '\t' && 
        _bytes[_position + len] != '\n' && 
        _bytes[_position + len] != '\r')
        len++;
    val = util::strtoi<UInt32>(std::string(_bytes + _position, len));
    if (_limit > _position + len)
        _position += len;
    else
        _position = _limit;
    return len;
}


int BitReader::readLine(std::string& val)
{    
    size_t len = 0;
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
    _buffer = &buf;
}


void BitWriter::init(char* bytes, std::size_t size, ByteOrder order) 
{

    //_vector = nullptr;
    _buffer = nullptr;
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


size_t BitWriter::available() const 
{
    return _limit - _position;
}


size_t BitWriter::limit() const
{ 
    return _limit; 
} 


//
// Write functions
//

void BitWriter::putU8(UInt8 val)
{
    put(reinterpret_cast<const char*>(&val), 1);
}


void BitWriter::putU16(UInt16 val) 
{
    UInt16 v = (_order == ByteOrder::Network) ? hostToNetwork16(val) : val;
    put(reinterpret_cast<const char*>(&v), 2);
}


void BitWriter::putU24(UInt32 val)
{
    UInt32 v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
    char* start = reinterpret_cast<char*>(&v);
    if (_order == ByteOrder::Network || isBigEndian())
        ++start;

    put(start, 3);
}


void BitWriter::putU32(UInt32 val) 
{
    UInt32 v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
    put(reinterpret_cast<const char*>(&v), 4);
}


void BitWriter::putU64(UInt64 val) 
{
    UInt64 v = (_order == ByteOrder::Network) ? hostToNetwork64(val) : val;
    put(reinterpret_cast<const char*>(&v), 8);
}


void BitWriter::put(const std::string& val) 
{
    put(val.c_str(), val.size());
}


void BitWriter::put(const char* val, std::size_t len) 
{        
    // Write to dynamic buffer
    if (_buffer) {
        //_buffer->resize(std::max<std::size_t>(3 * len / 2, 2048));    
        _buffer->insert(_buffer->end(), val, val + len); 
        _bytes = _buffer->data();        
        _limit = _buffer->size();    
        _position += len;    
    }
    
    // Write to fixed size buffer
    else {
        if ((_position + len) > _limit)
            throw std::out_of_range("insufficient buffer capacity");

        memcpy(_bytes + _position, val, len);
        _position += len;
    }
}


//
// Update functions
//


bool BitWriter::updateU8(UInt8 val, std::size_t pos) 
{
    return update(reinterpret_cast<const char*>(&val), 1, pos);
}


bool BitWriter::updateU16(UInt16 val, std::size_t pos) 
{
    UInt16 v = (_order == ByteOrder::Network) ? hostToNetwork16(val) : val;
    return update(reinterpret_cast<const char*>(&v), 2, pos);
}


bool BitWriter::updateU24(UInt32 val, std::size_t pos) 
{
    UInt32 v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
    char* start = reinterpret_cast<char*>(&v);
    if (_order == ByteOrder::Network || isBigEndian())
        ++start;

    return update(start, 3, pos);
}


bool BitWriter::updateU32(UInt32 val, std::size_t pos) 
{
    UInt32 v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
    return update(reinterpret_cast<const char*>(&v), 4, pos);
}


bool BitWriter::updateU64(UInt64 val, std::size_t pos) 
{
    UInt64 v = (_order == ByteOrder::Network) ? hostToNetwork64(val) : val;
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