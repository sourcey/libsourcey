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
#include "Sourcey/Types.h"


using namespace std;
using std::memcpy;
using std::min;


namespace scy {

static const int DEFAULT_SIZE = 4096;


Buffer::Buffer() 
{
	_pos   = 0;
	_end  = 0;
	_max = DEFAULT_SIZE;
	_bytes = new char[_max];
}


Buffer::Buffer(size_t len) 
{
	_pos   = 0;
	_end  = 0;
	_max = len;
	_bytes = new char[_max];
}


Buffer::Buffer(const char* bytes, size_t len) 
{
	_pos   = 0;
	_end  = len;
	_max = len;
	_bytes = new char[_max];
	memcpy(_bytes, bytes, _end);
}


Buffer::Buffer(const char* bytes) 
{
	_pos   = 0;
	_end  = strlen(bytes);
	_max = _end;
	_bytes = new char[_max];
	memcpy(_bytes, bytes, _end);
}


Buffer::~Buffer() 
{
	delete[] _bytes;
}


bool Buffer::readU8(UInt8& val) 
{
	return read(reinterpret_cast<char*>(&val), 1);
}


bool Buffer::readU16(UInt16& val) 
{
	UInt16 v;
	if (!read(reinterpret_cast<char*>(&v), 2)) {
		return false;
	} else {
		val = ntohs(v);
		return true;
	}
}


bool Buffer::readU24(UInt32& val) 
{
	UInt32 v = 0;
	if (!read(reinterpret_cast<char*>(&v) + 1, 3)) {
		return false;
	} else {
		val = ntohl(v);
		return true;
	}
}


bool Buffer::readU32(UInt32& val) 
{
	UInt32 v;
	if (!read(reinterpret_cast<char*>(&v), 4)) {
		return false;
	} else {
		val = ntohl(v);
		return true;
	}
}


bool Buffer::readU64(UInt64& val) 
{
	UInt64 v;
	if (!read(reinterpret_cast<char*>(&v), 8)) {
		return false;
	} else {
		val = ntohll(v);
		return true;
	}
}


bool Buffer::read(string& val, size_t len) 
{
	if (len > remaining()) {
		return false;
	} else {
		val.append(_bytes + _pos, len);
		_pos += len;
		return true;
	}
}


bool Buffer::read(char* val, size_t len) 
{
	if (len > remaining()) {
		return false;
	} else {
		memcpy(val, _bytes + _pos, len);
		_pos += len;
		return true;
	}
}


void Buffer::writeU8(UInt8 val) 
{
	write(reinterpret_cast<const char*>(&val), 1);
}


void Buffer::writeU16(UInt16 val) 
{
	UInt16 v = htons(val);
	write(reinterpret_cast<const char*>(&v), 2);
}


void Buffer::writeU24(UInt32 val) 
{
	UInt32 v = htonl(val);
	write(reinterpret_cast<const char*>(&v) + 1, 3);
}


void Buffer::writeU32(UInt32 val) 
{
	UInt32 v = htonl(val);
	write(reinterpret_cast<const char*>(&v), 4);
}


void Buffer::writeU64(UInt64 val) 
{
	UInt64 v = htonll(val);
	write(reinterpret_cast<const char*>(&v), 8);
}


void Buffer::write(const string& val) 
{
	write(val.c_str(), val.size());
}


void Buffer::write(const char* val, size_t len) 
{
	if (_end + len > _max)
		reserve(max(_max + len, 3 * _max / 2));

	assert(_max >= len);
	memcpy(_bytes + _end, val, len);
	_end += len;
}


void Buffer::updateU8(UInt8 val, size_t pos) 
{
	update(reinterpret_cast<const char*>(&val), 1, pos);
}


void Buffer::updateU16(UInt16 val, size_t pos) 
{
	UInt16 v = htons(val);
	update(reinterpret_cast<const char*>(&v), 2, pos);
}


void Buffer::updateU24(UInt32 val, size_t pos) 
{
	UInt32 v = htonl(val);
	update(reinterpret_cast<const char*>(&v) + 1, 3, pos);
}


void Buffer::updateU32(UInt32 val, size_t pos) 
{
	UInt32 v = htonl(val);
	update(reinterpret_cast<const char*>(&v), 4, pos);
}


void Buffer::updateU64(UInt64 val, size_t pos) 
{
	UInt64 v = htonll(val);
	update(reinterpret_cast<const char*>(&v), 8, pos);
}


void Buffer::update(const string& val, size_t pos) 
{
	update(val.c_str(), val.size(), pos);
}


bool Buffer::update(const char* val, size_t len, size_t pos) 
{	
	if ((pos + len) > remaining())
		return false;

	memcpy(_bytes + pos, val, len);
	return true;
}


bool Buffer::eof() const 
{ 
	return _end <= _pos; 
}


bool Buffer::eol() const 
{ 
	return _bytes[_pos] == '\r' 
		|| _bytes[_pos] == '\n'; 
}


void Buffer::position(size_t pos)
{ 
	_pos = pos;
}


void Buffer::size(size_t size) 
{ 
	if (size > _max)
		reserve(size);
	else
		_end = size; 
}

	
/*
void Buffer::resize(std::size_t size, bool preserve)
{
	char* ptr = new char[size];	
	if (preserve) {
		// memcpy current buffer content
		std::size_t n = size > _size ? _size : size;
		std::memcpy(ptr, _bytes, n);
	}
	delete [] _bytes;
	_bytes  = ptr;
	_size = size;
}
*/


void Buffer::reserve(size_t size) 
{
	//if (size > _max) {
		size_t len = min<size_t>(_end, size);
		char* newBytes = new char[size];
		memcpy(newBytes, _bytes, len);
		delete[] _bytes;

		_end	= len;
		_max	= size;
		_bytes	= newBytes;
	//}
}


void Buffer::consume(size_t size) 
{
	if (size > remaining())
		return;

	_pos += size;
}


void Buffer::shift(size_t size) 
{
	if (size > remaining())
		return;

	_end = remaining() - size;
	memmove(_bytes, _bytes + _pos + size, _end);
	//_pos = 0;
}


void Buffer::clear() 
{	
	_pos   = 0;
	_end   = 0;
	memset(_bytes, 0, _max);
}


const char Buffer::peek() 
{
	if (_end > _pos)
		return (const char)_bytes[_pos];
	errorL("Buffer", this) << "Peeking next character is NULL" << endl;
	return 0;
}


const UInt8 Buffer::peekU8()
{
	UInt8 v;
	if (read(reinterpret_cast<char*>(&v), 1)) {
		_pos -= 1;
		return v;
	}
	
	warnL("Buffer", this) << "Peeking UInt8: NULL" << endl;
	return 0;
}


const UInt16 Buffer::peekU16()
{	
	UInt16 v;
	if (read(reinterpret_cast<char*>(&v), 2)) {
		_pos -= 2;
		return ntohs(v);
	}
	
	warnL("Buffer", this) << "Peeking UInt16: NULL" << endl;
	return 0;
}


const UInt32 Buffer::peekU24()
{	
	UInt32 v;
	if (read(reinterpret_cast<char*>(&v), 3)) {
		_pos -= 3;
		return ntohl(v);
	}
	
	warnL("Buffer", this) << "Peeking UInt24: NULL" << endl;
	return 0;
}


const UInt32 Buffer::peekU32()
{
	UInt32 v;
	if (read(reinterpret_cast<char*>(&v), 4)) {
		_pos -= 4;
		return ntohl(v);
	}
	
	warnL("Buffer", this) << "Peeking UInt32: NULL" << endl;
	return 0;
}


const UInt64 Buffer::peekU64()
{
	UInt32 v;
	if (read(reinterpret_cast<char*>(&v), 8)) {
		_pos -= 8;
		return ntohll(v);
	}
	
	warnL("Buffer", this) << "Peeking UInt64: NULL" << endl;
	return 0;
}


//
// String Parser - make a separate class?
//

int Buffer::skipToChar(char c) 
{
	size_t len = 0;
	while (_end > _pos + len &&
		_bytes[_pos + len] != c)	
		len++;
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}


int Buffer::skipWhitespace() 
{
	size_t len = 0;
	while (_end > _pos + len &&
		_bytes[_pos + len] == ' ')	
		len++;
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}

	
int Buffer::skipToNextLine() 
{
	size_t len = 0;
	while (_end > _pos + len &&
		_bytes[_pos + len] != '\n') {
		len++;
	}	
	len++; // Advance passed newline
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}


int Buffer::skipNextWord() 
{	
	size_t len = skipWhitespace();
	while (_end > _pos + len &&
		_bytes[_pos + len] != ' ' && 
		_bytes[_pos + len] != '\t' && 
		_bytes[_pos + len] != '\n' && 
		_bytes[_pos + len] != '\r') 
		len++;
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}


int Buffer::readToNext(string& val, char c) 
{
	size_t len = 0;
	while (_end > _pos + len &&
		_bytes[_pos + len] != c)
		len++;
	val.append(_bytes + _pos, len);
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}


int Buffer::readNextWord(string& val) 
{	
	size_t len = skipWhitespace();
	while (_end > _pos + len && 
		_bytes[_pos + len] != ' ' && 
		_bytes[_pos + len] != '\t' && 
		_bytes[_pos + len] != '\n' && 
		_bytes[_pos + len] != '\r')
		len++;
	val.append(_bytes + _pos, len);
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}


int Buffer::readNextNumber(unsigned int& val) 
{	
	size_t len = skipWhitespace();
	while (_end > _pos + len && 
		_bytes[_pos + len] != ' ' && 
		_bytes[_pos + len] != '\t' && 
		_bytes[_pos + len] != '\n' && 
		_bytes[_pos + len] != '\r')
		len++;
	val = util::fromString<UInt32>(string(_bytes + _pos, len));
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}


int Buffer::readLine(string& val)
{	
	size_t len = 0;
	while (_end > _pos + len &&
		_bytes[_pos + len] != '\n')
		len++;
	val.append(_bytes + _pos, len);
	len++; // Advance passed newline
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}

} // namespace scy