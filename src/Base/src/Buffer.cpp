//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/Buffer.h"
#include "Sourcey/Util.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace Sourcey {


static const int DEFAULT_SIZE = 4096;


Buffer::Buffer() 
{
	_pos   = 0;
	_end  = 0;
	_max = DEFAULT_SIZE;
	_bytes = new char[_max];

	// LogTrace() << this << ": Creating Buffer: " << _max << endl;
}


Buffer::Buffer(size_t len) 
{
	_pos   = 0;
	_end  = 0;
	_max = len;
	_bytes = new char[_max];

	// LogTrace() << this << ": Creating Buffer: " << _max << endl;
}


Buffer::Buffer(const char* bytes, size_t len) 
{
	_pos   = 0;
	_end  = len;
	_max = len;
	_bytes = new char[_max];
	memcpy(_bytes, bytes, _end);

	// LogTrace() << this << ": Creating Buffer: " << _max << endl;
}


Buffer::Buffer(const char* bytes) 
{
	_pos   = 0;
	_end  = strlen(bytes);
	_max = _end;
	_bytes = new char[_max];
	memcpy(_bytes, bytes, _end);

	// LogTrace() << this << ": Creating Buffer: " << _max << endl;
}


Buffer::~Buffer() 
{
	delete[] _bytes;
}


bool Buffer::readUInt8(UInt8& val) 
{
	return readBytes(reinterpret_cast<char*>(&val), 1);
}


bool Buffer::readUInt16(UInt16& val) 
{
	UInt16 v;
	if (!readBytes(reinterpret_cast<char*>(&v), 2)) {
		return false;
	} else {
		val = NetworkToHost16(v);
		return true;
	}
}


bool Buffer::readUInt24(UInt32& val) 
{
	UInt32 v = 0;
	if (!readBytes(reinterpret_cast<char*>(&v) + 1, 3)) {
		return false;
	} else {
		val = NetworkToHost32(v);
		return true;
	}
}


bool Buffer::readUInt32(UInt32& val) 
{
	UInt32 v;
	if (!readBytes(reinterpret_cast<char*>(&v), 4)) {
		return false;
	} else {
		val = NetworkToHost32(v);
		return true;
	}
}


bool Buffer::readUInt64(UInt64& val) 
{
	UInt64 v;
	if (!readBytes(reinterpret_cast<char*>(&v), 8)) {
		return false;
	} else {
		val = NetworkToHost64(v);
		return true;
	}
}


bool Buffer::readString(string& val, size_t len) 
{
	if (len > remaining()) {
		return false;
	} else {
		val.append(_bytes + _pos, len);
		_pos += len;
		return true;
	}
}


bool Buffer::readBytes(char* val, size_t len) 
{
	if (len > remaining()) {
		LogError() << this << ": Buffer: Unable to read passed boundary." << endl;
		return false;
	} else {
		memcpy(val, _bytes + _pos, len);
		_pos += len;
		return true;
	}
}


void Buffer::writeUInt8(UInt8 val) 
{
	writeBytes(reinterpret_cast<const char*>(&val), 1);
}


void Buffer::writeUInt16(UInt16 val) 
{
	UInt16 v = HostToNetwork16(val);
	writeBytes(reinterpret_cast<const char*>(&v), 2);
}


void Buffer::writeUInt24(UInt32 val) 
{
	UInt32 v = HostToNetwork32(val);
	writeBytes(reinterpret_cast<const char*>(&v) + 1, 3);
}


void Buffer::writeUInt32(UInt32 val) 
{
	UInt32 v = HostToNetwork32(val);
	writeBytes(reinterpret_cast<const char*>(&v), 4);
}


void Buffer::writeUInt64(UInt64 val) 
{
	UInt64 v = HostToNetwork64(val);
	writeBytes(reinterpret_cast<const char*>(&v), 8);
}


void Buffer::writeString(const string& val) 
{
	writeBytes(val.c_str(), val.size());
}


void Buffer::writeBytes(const char* val, size_t len) 
{
	if (_end + len > _max)
		reserve(max(_max + len, 3 * _max / 2));

	assert(_max >= len);
	memcpy(_bytes + _end, val, len);
	_end += len;
}


void Buffer::updateUInt8(UInt8 val, size_t pos) 
{
	updateBytes(reinterpret_cast<const char*>(&val), 1, pos);
}


void Buffer::updateUInt16(UInt16 val, size_t pos) 
{
	UInt16 v = HostToNetwork16(val);
	updateBytes(reinterpret_cast<const char*>(&v), 2, pos);
}


void Buffer::updateUInt24(UInt32 val, size_t pos) 
{
	UInt32 v = HostToNetwork32(val);
	updateBytes(reinterpret_cast<const char*>(&v) + 1, 3, pos);
}


void Buffer::updateUInt32(UInt32 val, size_t pos) 
{
	UInt32 v = HostToNetwork32(val);
	updateBytes(reinterpret_cast<const char*>(&v), 4, pos);
}


void Buffer::updateUInt64(UInt64 val, size_t pos) 
{
	UInt64 v = HostToNetwork64(val);
	updateBytes(reinterpret_cast<const char*>(&v), 8, pos);
}


void Buffer::updateString(const string& val, size_t pos) 
{
	updateBytes(val.c_str(), val.size(), pos);
}


bool Buffer::updateBytes(const char* val, size_t len, size_t pos) 
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


void Buffer::setPosition(size_t pos)
{ 
	_pos = pos;
}


void Buffer::setSize(size_t size) 
{ 
	if (size > _max)
		reserve(size);
	else
		_end = size; 
}


void Buffer::reserve(size_t size) 
{
	size_t len = min(_end, size);
	char* newBytes = new char[size];
	memcpy(newBytes, _bytes, len);
	delete[] _bytes;

	_end	= len;
	_max	= size;
	_bytes	= newBytes;
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


size_t Buffer::skipToChar(char c) 
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


size_t Buffer::skipWhitespace() 
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

	
size_t Buffer::skipToNextLine() 
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


size_t Buffer::skipNextWord() 
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


size_t Buffer::readToNext(string& val, char c) 
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


size_t Buffer::readNextWord(string& val) 
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


size_t Buffer::readNextNumber(unsigned int& val) 
{	
	size_t len = skipWhitespace();
	while (_end > _pos + len && 
		_bytes[_pos + len] != ' ' && 
		_bytes[_pos + len] != '\t' && 
		_bytes[_pos + len] != '\n' && 
		_bytes[_pos + len] != '\r')
		len++;
	val = Util::atoi(string(_bytes + _pos, len));
	if (_end > _pos + len)
		_pos += len;
	else
		_pos = _end;
	return len;
}


size_t Buffer::readLine(string& val)
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


const char Buffer::peekChar() 
{
	if (_end > _pos)
		return (const char)_bytes[_pos];
	LogError() << "Peeking next character is NULL" << endl;
	return 0;
}


const UInt8 Buffer::peekUInt8()
{
	UInt8 v;
	if (readBytes(reinterpret_cast<char*>(&v), 1)) {
		_pos -= 1;
		return v;
	}
	
	LogWarn() << "Peeking UInt8: NULL" << endl;
	return 0;
}


const UInt16 Buffer::peekUInt16()
{	
	UInt16 v;
	if (readBytes(reinterpret_cast<char*>(&v), 2)) {
		_pos -= 2;
		return NetworkToHost16(v);
	}
	
	LogWarn() << "Peeking UInt16: NULL" << endl;
	return 0;
}


const UInt32 Buffer::peekUInt24()
{	
	UInt32 v;
	if (readBytes(reinterpret_cast<char*>(&v), 3)) {
		_pos -= 3;
		return NetworkToHost32(v);
	}
	
	LogWarn() << "Peeking UInt24: NULL" << endl;
	return 0;
}


const UInt32 Buffer::peekUInt32()
{
	UInt32 v;
	if (readBytes(reinterpret_cast<char*>(&v), 4)) {
		_pos -= 4;
		return NetworkToHost32(v);
	}
	
	LogWarn() << "Peeking UInt32: NULL" << endl;
	return 0;
}


const UInt64 Buffer::peekUInt64()
{
	UInt32 v;
	if (readBytes(reinterpret_cast<char*>(&v), 8)) {
		_pos -= 8;
		return NetworkToHost64(v);
	}
	
	LogWarn() << "Peeking UInt64: NULL" << endl;
	return 0;
}


} // namespace Sourcey


/*
 * libjingle
 * Copyright 2004--2006, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products 
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVS; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
