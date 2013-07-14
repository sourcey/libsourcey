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


#ifndef SOURCEY_Buffer_H
#define SOURCEY_Buffer_H


#include "Sourcey/ByteOrder.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Types.h"

#include <string>
#include <algorithm>
#include <cassert>
#include <cstring>


namespace scy {


class Buffer 
{
public:
	Buffer();
	Buffer(size_t len);
	Buffer(const char* bytes, size_t len);
	explicit Buffer(const char* bytes);
	~Buffer();

	char* begin() const { return _bytes; }
	char* data() const { return _bytes + _pos; }
	char* end() const { return _bytes + _end; }

	size_t size() const { return _end; }
	size_t remaining() const { return _end - _pos; }
	size_t capacity() const { return _max; }
	size_t position() const { return _pos; }

	bool eof() const;
	bool eol() const;

	void reserve(size_t size);
		// Request a change in capacity

	void consume(size_t size);
	void shift(size_t size);
	void position(size_t pos);
	void size(size_t size); // sets size value, careful!
	void clear();
	
	bool read(char* val, size_t len);
	bool read(std::string& val, size_t len);
	bool readU8(UInt8& val);
	bool readU16(UInt16& val);
	bool readU24(UInt32& val);
	bool readU32(UInt32& val);
	bool readU64(UInt64& val);
	
	void write(const char* val, size_t len);
	void write(const std::string& val);
	void writeU8(UInt8 val);
	void writeU16(UInt16 val);
	void writeU24(UInt32 val);
	void writeU32(UInt32 val);
	void writeU64(UInt64 val);
	
	const char peek();
	const UInt8 peekU8();
	const UInt16 peekU16();
	const UInt32 peekU24();
	const UInt32 peekU32();
	const UInt64 peekU64();
	
	bool update(const char* val, size_t len, size_t pos);
	void update(const std::string& val, size_t pos);
	void updateU8(UInt8 val, size_t pos);
	void updateU16(UInt16 val, size_t pos);
	void updateU24(UInt32 val, size_t pos);
	void updateU32(UInt32 val, size_t pos);
	void updateU64(UInt64 val, size_t pos);

	int skipToChar(char c);
	int skipWhitespace();
	int skipToNextLine();
	int skipNextWord();
	int readNextWord(std::string& val);
	int readNextNumber(unsigned int& val);
	int readLine(std::string& val);
	int readToNext(std::string& val, char c);

	void operator++(int) { _pos++; }			// increment position by 1
	void operator--(int) { _pos--; }			// deincrement position by 1
	
	void operator+=(int val) { _pos += val; }	// increment position by x
	void operator-=(int val) { _pos -= val; }	// deincrement position by x

	std::string toString() { return std::string(data(), size()); }
	
    friend std::ostream& operator << (std::ostream& stream, const Buffer& buf) 
	{
		return stream.write(buf.data(), buf.size());
    }

protected:
	char* _bytes;
	size_t _max;	// size of internal buffer
	size_t _pos;	// current read position
	size_t _end;	// number of bytes written to buffer
};


} // namespace scy


#endif  // SOURCEY_Buffer_H



	

/*
class BufferBase
{
};
	

class RefCountedRawBuffer: public CountedObject
{
	RefCountedRawBuffer() {}

	char* data;
	size_t size;
};
*/