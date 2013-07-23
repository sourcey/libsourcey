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


namespace scy {


Buffer::Buffer(size_t len)
{
	init(NULL, len, ORDER_NETWORK);
}


Buffer::Buffer(ByteOrder order)
{
	init(NULL, DEFAULT_SIZE, order);
}


Buffer::Buffer(const char* bytes, size_t len)
{
	init(bytes, len, ORDER_NETWORK);
}


Buffer::Buffer(const char* bytes, size_t len, ByteOrder order)
{
	init(bytes, len, order);
}


Buffer::Buffer(const char* bytes)
{
	init(bytes, strlen(bytes), ORDER_NETWORK);
}


void Buffer::init(const char* bytes, size_t len, ByteOrder order) 
{
	_mark = 0;
	_position = 0;
	_capacity = len;
	_order = order;
	_bytes = new char[_capacity];

	if (bytes) {
		_limit = len;
		memcpy(_bytes, bytes, _limit);
	} else {
		_limit = 0;
	}
}


Buffer::~Buffer() 
{
	delete[] _bytes;
}






/*
void Buffer::reserve(size_t len) 
{
	if (limit() + len > capacity()) {
		size_t oldPos = _position;
		resize(std::max<size_t>(limit() + len, 3 * _capacity / 2));
		_position = oldPos;
	}
}


void Buffer::resize(size_t size) 
{
	size_t len = std::min<size_t>(_limit - _position, size);
	if (size <= _capacity) {
		// Don't reallocate, just move data backwards
		memmove(_bytes, _bytes + _position, len);
	} else {
		// Reallocate a larger buffer.
		//_capacity = std::max<size_t>(size, 3 * _capacity / 2);
		char* newBytes = new char[size]; //_capacity
		memcpy(newBytes, _bytes + _position, len);
		delete [] _bytes;
		_bytes = newBytes;
		_capacity = size;
	}
	_position = 0;
	_limit = len;
	//++version_;
}
*/

/*
void Buffer::reserve(size_t size) 
{
	//if (size > std::max<size_t>) {
		size_t len = min<size_t>(_limit, size);
		char* newBytes = new char[size];
		memcpy(newBytes, _bytes, len);
		delete[] _bytes;

		_limit	= len;
		_capacity	= size;
		_bytes	= newBytes;
	//}
}
*/

void Buffer::reserve(size_t size)
{
	if (size > MAX_SIZE)
		throw Exception("Buffer cannot reserve past maximum size");

	if (size > _capacity) {
		//std::max<size_t>(limit() + len, 3 * _capacity / 2)
		//size_t len = min<size_t>(_end, size);
		//memcpy(newBytes, _bytes, len);
		char* newBytes = new char[size];
		memcpy(newBytes, _bytes, size);
		delete[] _bytes;

		//_limit	= len;
		_capacity = size;
		_bytes	= newBytes;
	}
}


void Buffer::mark() 
{ 
	_mark = _position;
}


bool Buffer::reset() 
{ 
	if (_mark == 0)
		return false;

	_position = _mark;
	return true;
}


void Buffer::rewind() 
{ 
    _position = _mark = 0;
}


void Buffer::limit(size_t val) 
{ 
	if (val > capacity())
		reserve(val);
	
    _limit = val;
    if (_position > val)
		_position = val;

    if (_mark > 0 && _mark > val)
		_mark = 0;
}


void Buffer::position(size_t val)
{ 
	_position = val;

	if (_mark > 0 && _mark > val)
		_mark = 0;
}


bool Buffer::consume(size_t val) 
{
	if (val > limit())
		return false;

	_position += val;
	return true;
}


void Buffer::clear() 
{
	memset(_bytes, 0, _capacity);
	_position = _mark = _limit = 0;
}


std::string Buffer::toString() 
{
	return std::string(data(), available()); 
}


size_t Buffer::capacity() const 
{ 
	return _capacity;
}


size_t Buffer::available() const 
{
	return _limit - _position;
}


size_t Buffer::limit() const
{ 
	return _limit; 
} 


//
// Get methods
//

bool Buffer::getU8(UInt8& val)
{
	return get(reinterpret_cast<char*>(&val), 1);
}


bool Buffer::getU16(UInt16& val)
{
	UInt16 v;
	if (!get(reinterpret_cast<char*>(&v), 2))
		return false;

	val = (_order == ORDER_NETWORK) ? networkToHost16(v) : v;
	return true;
}


bool Buffer::getU24(UInt32& val)
{
	UInt32 v = 0;
	char* target = reinterpret_cast<char*>(&v);
	if (_order == ORDER_NETWORK || isHostBigEndian())
		++target;

	if (!get(target, 3))
		return false;

	val = (_order == ORDER_NETWORK) ? networkToHost32(v) : v;
	return true;
}


bool Buffer::getU32(UInt32& val)
{
	UInt32 v;
	if (!get(reinterpret_cast<char*>(&v), 4)) {
		return false;
	} else {
		val = (_order == ORDER_NETWORK) ? networkToHost32(v) : v;
		return true;
	}
}


bool Buffer::getU64(UInt64& val)
{
	UInt64 v;
	if (!get(reinterpret_cast<char*>(&v), 8))
		return false;

	val = (_order == ORDER_NETWORK) ? networkToHost64(v) : v;
	return true;
}


bool Buffer::get(std::string& val, size_t len)
{
	if (len > limit())
		return false;	

	val.append(_bytes + _position, len);
	_position += len;
	return true;
}


bool Buffer::get(char* val, size_t len)
{
	if (len > limit())
		return false;

	memcpy(val, _bytes + _position, len);
	_position += len;
	return true;
}


void Buffer::putU8(UInt8 val)
{
	put(reinterpret_cast<const char*>(&val), 1);
}


void Buffer::putU16(UInt16 val) 
{
	UInt16 v = (_order == ORDER_NETWORK) ? hostToNetwork16(val) : val;
	put(reinterpret_cast<const char*>(&v), 2);
}


void Buffer::putU24(UInt32 val)
{
	UInt32 v = (_order == ORDER_NETWORK) ? hostToNetwork32(val) : val;
	char* start = reinterpret_cast<char*>(&v);
	if (_order == ORDER_NETWORK || isHostBigEndian())
		++start;

	put(start, 3);
}


void Buffer::putU32(UInt32 val) 
{
	UInt32 v = (_order == ORDER_NETWORK) ? hostToNetwork32(val) : val;
	put(reinterpret_cast<const char*>(&v), 4);
}


void Buffer::putU64(UInt64 val) 
{
	UInt64 v = (_order == ORDER_NETWORK) ? hostToNetwork64(val) : val;
	put(reinterpret_cast<const char*>(&v), 8);
}


void Buffer::put(const std::string& val) 
{
	put(val.c_str(), val.size());
}


void Buffer::put(const char* val, size_t len) 
{
	reserve(len);
	memcpy(_bytes + _limit, val, len);
	_limit += len;
}


//
// Update functions
//

bool Buffer::updateU8(UInt8 val, size_t pos) 
{
	return update(reinterpret_cast<const char*>(&val), 1, pos);
}


bool Buffer::updateU16(UInt16 val, size_t pos) 
{
	UInt16 v = (_order == ORDER_NETWORK) ? hostToNetwork16(val) : val;
	return update(reinterpret_cast<const char*>(&v), 2, pos);
}


bool Buffer::updateU24(UInt32 val, size_t pos) 
{
	UInt32 v = (_order == ORDER_NETWORK) ? hostToNetwork32(val) : val;
	char* start = reinterpret_cast<char*>(&v);
	if (_order == ORDER_NETWORK || isHostBigEndian())
		++start;

	return update(start, 3, pos);
}


bool Buffer::updateU32(UInt32 val, size_t pos) 
{
	UInt32 v = (_order == ORDER_NETWORK) ? hostToNetwork32(val) : val;
	return update(reinterpret_cast<const char*>(&v), 4, pos);
}


bool Buffer::updateU64(UInt64 val, size_t pos) 
{
	UInt64 v = (_order == ORDER_NETWORK) ? hostToNetwork64(val) : val;
	return update(reinterpret_cast<const char*>(&v), 8, pos);
}


bool Buffer::update(const std::string& val, size_t pos) 
{
	return update(val.c_str(), val.size(), pos);
}


bool Buffer::update(const char* val, size_t len, size_t pos) 
{	
	if ((pos + len) > available())
		return false;

	memcpy(_bytes + pos, val, len);
	return true;
}



//
// String Parser - make a separate class?
//

int Buffer::skipToChar(char c) 
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


int Buffer::skipWhitespace() 
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

	
int Buffer::skipToNextLine() 
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


int Buffer::skipNextWord() 
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


int Buffer::readToNext(std::string& val, char c) 
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


int Buffer::readNextWord(std::string& val) 
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


int Buffer::readNextNumber(unsigned int& val) 
{	
	size_t len = skipWhitespace();
	while (_limit > _position + len && 
		_bytes[_position + len] != ' ' && 
		_bytes[_position + len] != '\t' && 
		_bytes[_position + len] != '\n' && 
		_bytes[_position + len] != '\r')
		len++;
	val = util::fromString<UInt32>(std::string(_bytes + _position, len));
	if (_limit > _position + len)
		_position += len;
	else
		_position = _limit;
	return len;
}


int Buffer::readLine(std::string& val)
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


/*
Buffer::ReadPosition Buffer::GetReadPosition() const {
	return ReadPosition(_position, version_);
}


bool Buffer::SetReadPosition(const ReadPosition &position) {
	if (position.version_ != version_) {
		return false;
	}
	_position = position._position;
	return true;
}
*/


/*
Buffer::Buffer() 
{
	_position   = 0;
	_limit  = 0;
	std::max<size_t> = DEFAULT_SIZE;
	_bytes = new char[std::max<size_t>];
}


Buffer::Buffer(size_t len) 
{
	_position   = 0;
	_limit  = 0;
	std::max<size_t> = len;
	_bytes = new char[std::max<size_t>];
}


Buffer::Buffer(const char* bytes, size_t len) 
{
	_position   = 0;
	_limit  = len;
	std::max<size_t> = len;
	_bytes = new char[std::max<size_t>];
	memcpy(_bytes, bytes, _limit);
}


Buffer::Buffer(const char* bytes) 
{
	_position   = 0;
	_limit  = strlen(bytes);
	std::max<size_t> = _limit;
	_bytes = new char[std::max<size_t>];
	memcpy(_bytes, bytes, _limit);
}


Buffer::~Buffer() 
{
	delete[] _bytes;
}


bool Buffer::getU8(UInt8& val) 
{
	return get(reinterpret_cast<char*>(&val), 1);
}


bool Buffer::getU16(UInt16& val) 
{
	UInt16 v;
	if (!get(reinterpret_cast<char*>(&v), 2)) {
		return false;
	} else {
		val = networkToHost16(v);
		return true;
	}
}


bool Buffer::getU24(UInt32& val) 
{
	UInt32 v = 0;
	if (!get(reinterpret_cast<char*>(&v) + 1, 3)) {
		return false;
	} else {
		val = networkToHost32(v);
		return true;
	}
}


bool Buffer::getU32(UInt32& val) 
{
	UInt32 v;
	if (!get(reinterpret_cast<char*>(&v), 4)) {
		return false;
	} else {
		val = networkToHost32(v);
		return true;
	}
}


bool Buffer::getU64(UInt64& val) 
{
	UInt64 v;
	if (!get(reinterpret_cast<char*>(&v), 8)) {
		return false;
	} else {
		val = networkToHost64(v);
		return true;
	}
}


bool Buffer::get(string& val, size_t len) 
{
	if (len > available()) {
		return false;
	} else {
		val.append(_bytes + _position, len);
		_position += len;
		return true;
	}
}


bool Buffer::get(char* val, size_t len) 
{
	if (len > available()) {
		return false;
	} else {
		memcpy(val, _bytes + _position, len);
		_position += len;
		return true;
	}
}


void Buffer::putU8(UInt8 val) 
{
	put(reinterpret_cast<const char*>(&val), 1);
}


void Buffer::putU16(UInt16 val) 
{
	UInt16 v = hostToNetwork16(val);
	put(reinterpret_cast<const char*>(&v), 2);
}


void Buffer::putU24(UInt32 val) 
{
	UInt32 v = hostToNetwork32(val);
	put(reinterpret_cast<const char*>(&v) + 1, 3);
}


void Buffer::putU32(UInt32 val) 
{
	UInt32 v = hostToNetwork32(val);
	put(reinterpret_cast<const char*>(&v), 4);
}


void Buffer::putU64(UInt64 val) 
{
	UInt64 v = hostToNetwork64(val);
	put(reinterpret_cast<const char*>(&v), 8);
}


void Buffer::put(const string& val) 
{
	put(val.c_str(), val.size());
}


void Buffer::put(const char* val, size_t len) 
{
	if (_limit + len > std::max<size_t>)
		reserve(max(std::max<size_t> + len, 3 * std::max<size_t> / 2));

	assert(std::max<size_t> >= len);
	memcpy(_bytes + _limit, val, len);
	_limit += len;
}


void Buffer::updateU8(UInt8 val, size_t pos) 
{
	update(reinterpret_cast<const char*>(&val), 1, pos);
}


void Buffer::updateU16(UInt16 val, size_t pos) 
{
	UInt16 v = hostToNetwork16(val);
	update(reinterpret_cast<const char*>(&v), 2, pos);
}


void Buffer::updateU24(UInt32 val, size_t pos) 
{
	UInt32 v = hostToNetwork32(val);
	update(reinterpret_cast<const char*>(&v) + 1, 3, pos);
}


void Buffer::updateU32(UInt32 val, size_t pos) 
{
	UInt32 v = hostToNetwork32(val);
	update(reinterpret_cast<const char*>(&v), 4, pos);
}


void Buffer::updateU64(UInt64 val, size_t pos) 
{
	UInt64 v = hostToNetwork64(val);
	update(reinterpret_cast<const char*>(&v), 8, pos);
}


void Buffer::update(const string& val, size_t pos) 
{
	update(val.c_str(), val.size(), pos);
}


bool Buffer::update(const char* val, size_t len, size_t pos) 
{	
	if ((pos + len) > available())
		return false;

	memcpy(_bytes + pos, val, len);
	return true;
}


bool Buffer::eof() const 
{ 
	return _limit <= _position; 
}


bool Buffer::eol() const 
{ 
	return _bytes[_position] == '\r' 
		|| _bytes[_position] == '\n'; 
}


void Buffer::position(size_t pos)
{ 
	_position = pos;
}


void Buffer::size(size_t size) 
{ 
	if (size > std::max<size_t>)
		reserve(size);
	else
		_limit = size; 
}


void Buffer::reserve(size_t size) 
{
	//if (size > std::max<size_t>) {
		size_t len = min<size_t>(_limit, size);
		char* newBytes = new char[size];
		memcpy(newBytes, _bytes, len);
		delete[] _bytes;

		_limit	= len;
		std::max<size_t>	= size;
		_bytes	= newBytes;
	//}
}


void Buffer::consume(size_t size) 
{
	if (size > available())
		return;

	_position += size;
}


void Buffer::shift(size_t size) 
{
	if (size > available())
		return;

	_limit = available() - size;
	memmove(_bytes, _bytes + _position + size, _limit);
	//_position = 0;
}


void Buffer::clear() 
{	
	_position   = 0;
	_limit   = 0;
	memset(_bytes, 0, std::max<size_t>);
}


const char Buffer::peek() 
{
	if (_limit > _position)
		return (const char)_bytes[_position];
	errorL("Buffer", this) << "Peeking next character is NULL" << endl;
	return 0;
}


const UInt8 Buffer::peekU8()
{
	UInt8 v;
	if (get(reinterpret_cast<char*>(&v), 1)) {
		_position -= 1;
		return v;
	}
	
	warnL("Buffer", this) << "Peeking UInt8: NULL" << endl;
	return 0;
}


const UInt16 Buffer::peekU16()
{	
	UInt16 v;
	if (get(reinterpret_cast<char*>(&v), 2)) {
		_position -= 2;
		return ntohs(v);
	}
	
	warnL("Buffer", this) << "Peeking UInt16: NULL" << endl;
	return 0;
}


const UInt32 Buffer::peekU24()
{	
	UInt32 v;
	if (get(reinterpret_cast<char*>(&v), 3)) {
		_position -= 3;
		return ntohl(v);
	}
	
	warnL("Buffer", this) << "Peeking UInt24: NULL" << endl;
	return 0;
}


const UInt32 Buffer::peekU32()
{
	UInt32 v;
	if (get(reinterpret_cast<char*>(&v), 4)) {
		_position -= 4;
		return ntohl(v);
	}
	
	warnL("Buffer", this) << "Peeking UInt32: NULL" << endl;
	return 0;
}


const UInt64 Buffer::peekU64()
{
	UInt32 v;
	if (get(reinterpret_cast<char*>(&v), 8)) {
		_position -= 8;
		return networkToHost64(v);
	}
	
	warnL("Buffer", this) << "Peeking UInt64: NULL" << endl;
	return 0;
}


*/


	
/*
void Buffer::resize(std::size_t size, bool preserve)
{
	char* ptr = new char[size];	
	if (preserve) {
		// memcpy current buffer content
		std::size_t n = size > _capacity ? _capacity : size;
		std::memcpy(ptr, _bytes, n);
	}
	delete [] _bytes;
	_bytes  = ptr;
	_capacity = size;
}
*/

} // namespace scy