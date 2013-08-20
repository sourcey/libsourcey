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


#include "Sourcey/Buffer.h"
#include "Sourcey/Util.h"
#include "Sourcey/Logger.h"
#include "Sourcey/ByteOrder.h"

#include <cstddef>
#include <cstring>
#include <stdexcept>


namespace scy {
	
	
//
// Buffer
//


Buffer::Buffer(std::size_t s)
	: _free (true)
{
	_data = (s != 0 ? new char[s] : 0);
	_size = _capacity = s;
}


Buffer::Buffer(std::size_t s, std::size_t c)
	: _free (true)
{
	if (s > c)
		throw std::invalid_argument ("size greater than capacity");

	_data = (c != 0 ? new char[c] : 0);
	_size = s;
	_capacity = c;
}


Buffer::Buffer(const void* d, std::size_t s)
	: _free (true)
{
	if (s != 0)
	{
		_data = new char[s];
		std::memcpy (_data, d, s);
	}
	else
		_data = 0;

	_size = _capacity = s;
}


Buffer::Buffer(const void* d, std::size_t s, std::size_t c)
	: _free (true)
{
	if (s > c)
		throw std::invalid_argument ("size greater than capacity");

	if (c != 0)
	{
		_data = new char[c];

		if (s != 0)
			std::memcpy (_data, d, s);
	}
	else
		_data = 0;

	_size = s;
	_capacity = c;
}


Buffer::Buffer(void* d, std::size_t s, std::size_t c, bool own)
	: _data (static_cast<char*> (d)), _size (s), _capacity (c), _free (own)
{
	if (s > c)
		throw std::invalid_argument ("size greater than capacity");
}


Buffer::Buffer(const Buffer& x)
	: _free (true)
{
	if (x._capacity != 0)
	{
		_data = new char[x._capacity];

		if (x._size != 0)
			std::memcpy (_data, x._data, x._size);
	}
	else
		_data = 0;

	_size = x._size;
	_capacity = x._capacity;
}


Buffer::~Buffer()
{
	//if (_free)
	//	delete[] _data;
}


Buffer& Buffer::operator = (const Buffer& x)
{
	if (&x != this)
	{
		if (x._size > _capacity)
		{
			if (_free)
				delete[] _data;

			_data = new char[x._capacity];
			_capacity = x._capacity;
			_free = true;
		}

		if (x._size != 0)
			std::memcpy (_data, x._data, x._size);

		_size = x._size;
	}

	return *this;
}


void Buffer::swap(Buffer& x)
{
	char* d (x._data);
	std::size_t s (x._size);
	std::size_t c (x._capacity);
	bool f (x._free);

	x._data = _data;
	x._size = _size;
	x._capacity = _capacity;
	x._free = _free;

	_data = d;
	_size = s;
	_capacity = c;
	_free = f;
}


char* Buffer::detach()
{
	char* r (_data);

	_data = nullptr;
	_size = 0;
	_capacity = 0;

	return r;
}


void Buffer::assign(const void* d, std::size_t s)
{
	if (s > _capacity)
	{
		if (_free)
			delete[] _data;

		_data = new char[s];
		_capacity = s;
		_free = true;
	}

	if (s != 0)
		std::memcpy (_data, d, s);

	_size = s;
}


void Buffer::assign(void* d, std::size_t s, std::size_t c, bool own)
{
	if (_free)
		delete[] _data;

	_data = static_cast<char*> (d);
	_size = s;
	_capacity = c;
	_free = own;
}


void Buffer::append(const Buffer& b)
{
	append (b.data (), b.size ());
}


void Buffer::append(const void* d, std::size_t s)
{
	if (s != 0)
	{
		std::size_t ns (_size + s);

		if (_capacity < ns)
			capacity (ns);

		std::memcpy (_data + _size, d, s);
		_size = ns;
	}
}


void Buffer::fill(char v)
{
	if (_size > 0)
		std::memset (_data, v, _size);
}


std::size_t Buffer::size() const
{
	return _size;
}


bool Buffer::size(std::size_t s)
{
	bool r (false);

	if (_capacity < s)
		r = capacity (s);

	_size = s;
	return r;
}


std::size_t Buffer::capacity() const
{
	return _capacity;
}


bool Buffer::capacity(std::size_t c)
{	
	if (_capacity >= c)
		return false; // cannot be downsized

	char* d (new char[c]);

	if (_size != 0)
		std::memcpy (d, _data, _size);

	if (_free)
		delete[] _data;

	_data = d;
	_capacity = c;
	_free = true;

	return true;
}


bool Buffer::empty() const
{
	return _size == 0;
}


void Buffer::clear()
{
	_size = 0;
}


char* Buffer::data()
{
	return _data;
}


const char* Buffer::data() const
{
	return _data;
}


char& Buffer::operator [] (std::size_t i)
{
	return _data[i];
}


char Buffer::operator [] (std::size_t i) const
{
	return _data[i];
}


char& Buffer::at(std::size_t i)
{
	if (i >= _size)
		throw std::out_of_range("index out of range");

	return _data[i];
}


char Buffer::at(std::size_t i) const
{
	if (i >= _size)
		throw std::out_of_range("index out of range");

	return _data[i];
}


std::size_t Buffer::find(char v, std::size_t pos) const
{
	if (_size == 0 || pos >= _size)
		return npos;

	char* p (static_cast<char*> (std::memchr (_data + pos, v, _size - pos)));
	return p != 0 ? static_cast<std::size_t> (p - _data) : npos;
}


std::size_t Buffer::rfind(char v, std::size_t pos) const
{
	if (_size != 0)
	{
		std::size_t n (_size);

		if (--n > pos)
			n = pos;

		for (++n; n-- != 0; )
			if (_data[n] == v)
				return n;
	}

	return npos;
}


bool operator == (const Buffer& a, const Buffer& b)
{
	return a.size () == b.size () &&
		std::memcmp (a.data (), b.data (), a.size ()) == 0;
}


bool operator != (const Buffer& a, const Buffer& b)
{
	return !(a == b);
}


//
// BitReader
//


/*
BitReader::BitReader(std::size_t size, ByteOrder order)
{
	initConst(nullptr, size, order);
}


BitReader::BitReader(const char* bytes)
{
	initConst(bytes, strlen(bytes), ByteOrder::Network);
}


BitReader::BitReader(const MutableBuffer& pod, ByteOrder order)
{
	initMutable(bufferCast<char*>(pod), bufferSizeHelper(pod), order); // nocopy
}
*/


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

/*
void BitReader::initMutable(char* bytes, std::size_t size, ByteOrder order) 
{
	_mark = 0;
	_position = 0;
	_capacity = size;
	_order = order;
	_bytes = bytes;
	_free = false;
}


void BitReader::initConst(const char* bytes, std::size_t size, ByteOrder order) 
{
	_mark = 0;
	_position = 0;
	_capacity = size;
	_order = order;
	_bytes = new char[_capacity];
	_free = true;

	if (bytes) {
		_limit = size;
		memcpy(_bytes, bytes, _limit);
	} 
	else {
		_limit = 0;
	}
}


void BitReader::mark() 
{ 
	_mark = _position;
}


bool BitReader::reset() 
{ 
	if (_mark == 0)
		return false;

	_position = _mark;
	return true;
}


void BitReader::rewind() 
{ 
    _position = _mark = 0;
}

	if (_mark > 0 && _mark > val)
		_mark = 0;
*/


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
// String Parser - make a separate class?
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
	len++; // Advance passed newline
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
	len++; // Advance passed newline
	if (_limit > _position + len)
		_position += len;
	else
		_position = _limit;
	return len;
}




//
// Bit Writer
//


/*
BitWriter::BitWriter(std::size_t size, ByteOrder order)
{
	initConst(nullptr, size, order);
}


BitWriter::BitWriter(const MutableBuffer& pod, ByteOrder order)
{
	initMutable(bufferCast<char*>(pod), bufferSizeHelper(pod), order); // nocopy
}


BitWriter::BitWriter(const char* bytes)
{
	initConst(bytes, strlen(bytes), ByteOrder::Network);
}
*/


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
}


void BitWriter::init(char* bytes, std::size_t size, ByteOrder order) 
{
	_position = 0;
	_limit = size;
	//_capacity = size;
	_order = order;
	_bytes = bytes;
	//_free = false;
}


/*
void BitWriter::initConst(const char* bytes, std::size_t size, ByteOrder order) 
{
	_mark = 0;
	_position = 0;
	_capacity = size;
	_order = order;
	_bytes = new char[_capacity];
	_free = true;

	if (bytes) {
		_limit = size;
		memcpy(_bytes, bytes, _limit);
	} 
	else {
		_limit = 0;
	}
}
	if (_free)
		delete[] _bytes;


void BitWriter::reserve(std::size_t size)
{
	if (size > MaxSize)
		throw std::runtime_error("BitWriter cannot reserve past maximum size");

	if (size > _capacity) {
		char* newBytes = new char[size];
		memcpy(newBytes, _bytes, size);
		delete[] _bytes;

		//_limit	= len;
		_capacity = size;
		_bytes	= newBytes;
		_free = true;
	}
}
*/


BitWriter::~BitWriter() 
{
}


/*
void BitWriter::mark() 
{ 
	_mark = _position;
}


bool BitWriter::reset() 
{ 
	if (_mark == 0)
		return false;

	_position = _mark;
	return true;
}



void BitWriter::limit(std::size_t val) 
{ 
	if (val > capacity())
		reserve(val);
	
    _limit = val;
    if (_position > val)
		_position = val;

    if (_mark > 0 && _mark > val)
		_mark = 0;
}


void BitWriter::rewind() 
{ 
    _position = _mark = 0;
}


void BitWriter::clear() 
{
	memset(_bytes, 0, _limit);
	_position = 0;
}
*/


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
	if ((_position + len) > _limit)
		throw std::out_of_range("insufficient buffer capacity");

	memcpy(_bytes + _position, val, len);
	_position += len;
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


/*

//
// Bit Buffer
//



BitBuffer::BitBuffer(std::size_t size, ByteOrder order)
{
	initConst(nullptr, size, order);
}


BitBuffer::BitBuffer(const MutableBuffer& pod, ByteOrder order)
{
	initMutable(bufferCast<char*>(pod), bufferSizeHelper(pod), order); // nocopy
}


BitBuffer::BitBuffer(const ConstBuffer& pod, ByteOrder order)
{
	initConst(bufferCast<const char*>(pod), bufferSizeHelper(pod), order); // copied
}


BitBuffer::BitBuffer(const char* bytes, std::size_t size, ByteOrder order)
{
	initConst(bytes, size, order);
}


BitBuffer::BitBuffer(const char* bytes)
{
	initConst(bytes, strlen(bytes), ByteOrder::Network);
}


void BitBuffer::initMutable(char* bytes, std::size_t size, ByteOrder order) 
{
	_mark = 0;
	_position = 0;
	_limit = size;
	_capacity = size;
	_order = order;
	_bytes = bytes;
	_free = false;
}


void BitBuffer::initConst(const char* bytes, std::size_t size, ByteOrder order) 
{
	_mark = 0;
	_position = 0;
	_capacity = size;
	_order = order;
	_bytes = new char[_capacity];
	_free = true;

	if (bytes) {
		_limit = size;
		memcpy(_bytes, bytes, _limit);
	} 
	else {
		_limit = 0;
	}
}


BitBuffer::~BitBuffer() 
{
	if (_free)
		delete[] _bytes;
}


void BitBuffer::reserve(std::size_t size)
{
	if (size > MaxSize)
		throw std::runtime_error("BitBuffer cannot reserve past maximum size");

	if (size > _capacity) {
		char* newBytes = new char[size];
		memcpy(newBytes, _bytes, size);
		delete[] _bytes;

		//_limit	= len;
		_capacity = size;
		_bytes	= newBytes;
		_free = true;
	}
}


void BitBuffer::mark() 
{ 
	_mark = _position;
}


bool BitBuffer::reset() 
{ 
	if (_mark == 0)
		return false;

	_position = _mark;
	return true;
}


void BitBuffer::rewind() 
{ 
    _position = _mark = 0;
}


void BitBuffer::limit(std::size_t val) 
{ 
	if (val > capacity())
		reserve(val);
	
    _limit = val;
    if (_position > val)
		_position = val;

    if (_mark > 0 && _mark > val)
		_mark = 0;
}


void BitBuffer::position(std::size_t val)
{ 
	_position = val;

	if (_mark > 0 && _mark > val)
		_mark = 0;
}


bool BitBuffer::skip(std::size_t val) 
{
	if (val > limit())
		return false;

	_position += val;
	return true;
}


void BitBuffer::clear() 
{
	memset(_bytes, 0, _capacity);
	_position = _mark = _limit = 0;
}


std::string BitBuffer::toString() 
{
	return std::string(data(), available()); 
}


size_t BitBuffer::capacity() const 
{ 
	return _capacity;
}


size_t BitBuffer::available() const 
{
	return _limit - _position;
}


size_t BitBuffer::limit() const
{ 
	return _limit; 
} 


//
// Get methods
//


bool BitBuffer::getU8(UInt8& val)
{
	return get(reinterpret_cast<char*>(&val), 1);
}


bool BitBuffer::getU16(UInt16& val)
{
	UInt16 v;
	if (!get(reinterpret_cast<char*>(&v), 2))
		return false;

	val = (_order == ByteOrder::Network) ? networkToHost16(v) : v;
	return true;
}


bool BitBuffer::getU24(UInt32& val)
{
	UInt32 v = 0;
	char* target = reinterpret_cast<char*>(&v);
	if (_order == ByteOrder::Network || isBigEndian())
		++target;

	if (!get(target, 3))
		return false;

	val = (_order == ByteOrder::Network) ? networkToHost32(v) : v;
	return true;
}


bool BitBuffer::getU32(UInt32& val)
{
	UInt32 v;
	if (!get(reinterpret_cast<char*>(&v), 4)) {
		return false;
	} else {
		val = (_order == ByteOrder::Network) ? networkToHost32(v) : v;
		return true;
	}
}


bool BitBuffer::getU64(UInt64& val)
{
	UInt64 v;
	if (!get(reinterpret_cast<char*>(&v), 8))
		return false;

	val = (_order == ByteOrder::Network) ? networkToHost64(v) : v;
	return true;
}


bool BitBuffer::get(std::string& val, std::size_t len)
{
	if (len > limit())
		return false;	

	val.append(_bytes + _position, len);
	_position += len;
	return true;
}


bool BitBuffer::get(char* val, std::size_t len)
{
	if (len > limit())
		return false;

	memcpy(val, _bytes + _position, len);
	_position += len;
	return true;
}


void BitBuffer::putU8(UInt8 val)
{
	put(reinterpret_cast<const char*>(&val), 1);
}


void BitBuffer::putU16(UInt16 val) 
{
	UInt16 v = (_order == ByteOrder::Network) ? hostToNetwork16(val) : val;
	put(reinterpret_cast<const char*>(&v), 2);
}


void BitBuffer::putU24(UInt32 val)
{
	UInt32 v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
	char* start = reinterpret_cast<char*>(&v);
	if (_order == ByteOrder::Network || isBigEndian())
		++start;

	put(start, 3);
}


void BitBuffer::putU32(UInt32 val) 
{
	UInt32 v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
	put(reinterpret_cast<const char*>(&v), 4);
}


void BitBuffer::putU64(UInt64 val) 
{
	UInt64 v = (_order == ByteOrder::Network) ? hostToNetwork64(val) : val;
	put(reinterpret_cast<const char*>(&v), 8);
}


void BitBuffer::put(const std::string& val) 
{
	put(val.c_str(), val.size());
}


void BitBuffer::put(const char* val, std::size_t len) 
{
	reserve(len);
	memcpy(_bytes + _limit, val, len);
	_limit += len;
}


//
// Update functions
//


bool BitBuffer::updateU8(UInt8 val, std::size_t pos) 
{
	return update(reinterpret_cast<const char*>(&val), 1, pos);
}


bool BitBuffer::updateU16(UInt16 val, std::size_t pos) 
{
	UInt16 v = (_order == ByteOrder::Network) ? hostToNetwork16(val) : val;
	return update(reinterpret_cast<const char*>(&v), 2, pos);
}


bool BitBuffer::updateU24(UInt32 val, std::size_t pos) 
{
	UInt32 v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
	char* start = reinterpret_cast<char*>(&v);
	if (_order == ByteOrder::Network || isBigEndian())
		++start;

	return update(start, 3, pos);
}


bool BitBuffer::updateU32(UInt32 val, std::size_t pos) 
{
	UInt32 v = (_order == ByteOrder::Network) ? hostToNetwork32(val) : val;
	return update(reinterpret_cast<const char*>(&v), 4, pos);
}


bool BitBuffer::updateU64(UInt64 val, std::size_t pos) 
{
	UInt64 v = (_order == ByteOrder::Network) ? hostToNetwork64(val) : val;
	return update(reinterpret_cast<const char*>(&v), 8, pos);
}


bool BitBuffer::update(const std::string& val, std::size_t pos) 
{
	return update(val.c_str(), val.size(), pos);
}


bool BitBuffer::update(const char* val, std::size_t len, std::size_t pos) 
{	
	if ((pos + len) > available())
		return false;

	memcpy(_bytes + pos, val, len);
	return true;
}


//
// String Parser - make a separate class?
//


int BitBuffer::skipToChar(char c) 
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


int BitBuffer::skipWhitespace() 
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

	
int BitBuffer::skipToNextLine() 
{
	size_t len = 0;
	while (_limit > _position + len &&
		_bytes[_position + len] != '\n') {
		len++;
	}	
	len++; // Advance passed newline
	if (_limit > _position + len)
		_position += len;
	else
		_position = _limit;
	return len;
}


int BitBuffer::skipNextWord() 
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


int BitBuffer::readToNext(std::string& val, char c) 
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


int BitBuffer::readNextWord(std::string& val) 
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


int BitBuffer::readNextNumber(unsigned int& val) 
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


int BitBuffer::readLine(std::string& val)
{	
	size_t len = 0;
	while (_limit > _position + len &&
		_bytes[_position + len] != '\n')
		len++;
	val.append(_bytes + _position, len);
	len++; // Advance passed newline
	if (_limit > _position + len)
		_position += len;
	else
		_position = _limit;
	return len;
}


*/


} // namespace scy