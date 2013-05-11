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


#ifndef SOURCEY_Buffer_H
#define SOURCEY_Buffer_H


#include "Sourcey/ByteOrder.h"
#include "Sourcey/Base.h"

#include <string>
#include <algorithm>
#include <cassert>
#include <cstring>


namespace Scy {


class Buffer 
{
public:
	Buffer();
	Buffer(size_t len);
	Buffer(const char* bytes, size_t len);
	explicit Buffer(const char* bytes);
	~Buffer();

	char* bytes() const { return _bytes; }
	char* data() const { return _bytes + _pos; }

	size_t size() const { return _end; }
	size_t remaining() const { return _end - _pos; }
	size_t capacity() const { return _max; }
	size_t position() const { return _pos; }

	bool eof() const;
	bool eol() const;

	void setPosition(size_t pos);
	void setSize(size_t size); // sets size value, careful!

	void reserve(size_t size);
	void consume(size_t size);
	void shift(size_t size);
	void clear();

	bool readUInt8(UInt8& val);
	bool readUInt16(UInt16& val);
	bool readUInt24(UInt32& val);
	bool readUInt32(UInt32& val);
	bool readUInt64(UInt64& val);
	bool readString(std::string& val, size_t len);
	bool readBytes(char* val, size_t len);

	void writeUInt8(UInt8 val);
	void writeUInt16(UInt16 val);
	void writeUInt24(UInt32 val);
	void writeUInt32(UInt32 val);
	void writeUInt64(UInt64 val);
	void writeString(const std::string& val);
	void writeBytes(const char* val, size_t len);
	
	const char peekChar();
	const UInt8 peekUInt8();
	const UInt16 peekUInt16();
	const UInt32 peekUInt24();
	const UInt32 peekUInt32();
	const UInt64 peekUInt64();

	void updateUInt8(UInt8 val, size_t pos);
	void updateUInt16(UInt16 val, size_t pos);
	void updateUInt24(UInt32 val, size_t pos);
	void updateUInt32(UInt32 val, size_t pos);
	void updateUInt64(UInt64 val, size_t pos);
	void updateString(const std::string& val, size_t pos);
	bool updateBytes(const char* val, size_t len, size_t pos);

	size_t skipToChar(char c);
	size_t skipWhitespace();
	size_t skipToNextLine();
	size_t skipNextWord();
	size_t readNextWord(std::string& val);
	size_t readNextNumber(unsigned int& val);
	size_t readLine(std::string& val);
	size_t readToNext(std::string& val, char c);

	void operator++(int) { _pos++; }			// increment position by 1
	void operator--(int) { _pos--; }				// deincrement position by 1
	
	void operator+=(int val) { _pos += val; }	// increment position by x
	void operator-=(int val) { _pos -= val; }	// deincrement position by x
	
	//T& operator [] (std::size_t index)
	//const T& operator [] (std::size_t index) const

private:
	char* _bytes;
	size_t _max;	// size of internal buffer
	size_t _pos;	// current read position
	size_t _end;	// number of bytes written to buffer
};


} // namespace Scy


#endif  // SOURCEY_Buffer_H