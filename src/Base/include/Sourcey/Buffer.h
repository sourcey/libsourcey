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


#include "Sourcey/Types.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Types.h"

#include <string>
#include <algorithm>
#include <cassert>
#include <cstring>


namespace scy {


class Buffer 
	/// A buffer for reading/writing binary streams.
{
public:
	static const int DEFAULT_SIZE = 2048;
	static const int MAX_SIZE = 65536;

	enum ByteOrder
		/// Defines order of bytes in the buffer.
	{
		ORDER_NETWORK = 0,  // Default, use network byte order (big endian).
		ORDER_HOST,         // Use the native order of the host.
	};
	
	Buffer(size_t len = DEFAULT_SIZE);
	explicit Buffer(ByteOrder order);
	Buffer(const char* bytes, size_t len);
	Buffer(const char* bytes, size_t len, ByteOrder order);

	explicit Buffer(const char* bytes);
		/// Initializes buffer from a zero-terminated char array.

	~Buffer();

	bool consume(size_t size);
		// Moves current read position forward. Returns false if
		// there if reading past buffer limit. Consume doesn't
		// reset clear any data, so remembered read positions
		// are still valid after this call.
		
	void limit(size_t val);
		/// Sets this buffer's limit. If the position is larger
		/// than the new limit then it is set to the new limit. 
		/// If the mark is defined and larger than the new
		/// limit then it is discarded.

	size_t limit() const; // { return _limit; } // - _position
		/// Returns the read limit of this buffer.
		/// This of the limit as an alias for buffer.available()
			
	void position(size_t val);
		/// Sets this buffer's read position. 
		/// If the mark is defined and larger than the
	    /// new position then it is discarded.

	size_t position() const { return _position; }
		/// Returns the current position in the buffer
	
	void mark();
		/// Sets this buffer's mark at the current position.		

	bool reset();
		/// Resets this buffer's position to the previously-marked position.
		/// Returns false if no mark is set.
	
	void rewind();
		/// Rewinds this buffer. The position is set to zero and the mark is discarded.

	void clear();
		/// Clears the contents of the buffer. 
		/// The position is set to zero, the limit is set to the
		/// capacity, and the mark is discarded.

	void reserve(size_t len);
		/// Reserves the given number of bytes in the buffer.
		///
		/// An exception is thrown if the given size is greater 
		/// than Buffer::MAX_SIZE.

	size_t capacity() const;
		/// Returns the total storage capaciry of this buffer.

	size_t available() const;
		/// Returns the number of elements between the current position and the limit.

	//void resize(size_t size);
		// Resizes the buffer to the specified size.

	//void mark(size_t pos);
	
	char* begin() { return _bytes; }
	char* data() { return _bytes + _position; }

	const char* begin() const { return _bytes; }
	const char* data() const { return _bytes + _position; }
	
	size_t mark() const { return _mark; } // - _position

	ByteOrder order() const { return _order; }
	
	bool get(char* val, size_t len);
	bool get(std::string& val, size_t len);
	bool getU8(UInt8& val);
	bool getU16(UInt16& val);
	bool getU24(UInt32& val);
	bool getU32(UInt32& val);
	bool getU64(UInt64& val);
		/// Reads a value from the buffer. 
		/// Returns false if there isn't enough data left for the specified type.
	
	void put(const char* val, size_t len);
	void put(const std::string& val);
	void putU8(UInt8 val);
	void putU16(UInt16 val);
	void putU24(UInt32 val);
	void putU32(UInt32 val);
	void putU64(UInt64 val);
		/// Write value to the buffer. 
		/// Resizes the buffer when neccessary.
		
	const char peek();
	const UInt8 peekU8();
	const UInt16 peekU16();
	const UInt32 peekU24();
	const UInt32 peekU32();
	const UInt64 peekU64();
		/// Peeks data from the buffer. 
		/// Zero is returned if read past boundary.
	
	bool update(const char* val, size_t len, size_t pos);
	bool update(const std::string& val, size_t pos);
	bool updateU8(UInt8 val, size_t pos);
	bool updateU16(UInt16 val, size_t pos);
	bool updateU24(UInt32 val, size_t pos);
	bool updateU32(UInt32 val, size_t pos);
	bool updateU64(UInt64 val, size_t pos);

	int skipToChar(char c);
	int skipWhitespace();
	int skipToNextLine();
	int skipNextWord();
	int readNextWord(std::string& val);
	int readNextNumber(unsigned int& val);
	int readLine(std::string& val);
	int readToNext(std::string& val, char c);

	std::string toString();
	
    friend std::ostream& operator << (std::ostream& stream, const Buffer& buf) 
	{
		return stream.write(buf.data(), buf.available());
    }

private:
	void init(const char* bytes, size_t size, ByteOrder order);

	char* _bytes;
	size_t _capacity;
	size_t _position;
	size_t _limit;
	size_t _mark;
	ByteOrder _order;
};



} // namespace scy


#endif  // SOURCEY_Buffer_H


/*
class Buffer 
{
public:
	Buffer();
	Buffer(size_t len);
	Buffer(const char* bytes, size_t len);
	explicit Buffer(const char* bytes);
	~Buffer();

	char* begin() const { return _bytes; }
	char* data() const { return _bytes + _position; }
	char* end() const { return _bytes + _limit; }

	size_t size() const { return _limit; }
	size_t available() const { return _limit - _position; }
	size_t capacity() const { return _max; }
	size_t position() const { return _position; }

	bool eof() const;
	bool eol() const;

	void reserve(size_t size);
		// Request a change in capacity

	void consume(size_t size);
	void shift(size_t size);
	void position(size_t pos);
	void size(size_t size); // sets size value, careful!
	void clear();
	
	bool get(char* val, size_t len);
	bool get(std::string& val, size_t len);
	bool getU8(UInt8& val);
	bool getU16(UInt16& val);
	bool getU24(UInt32& val);
	bool getU32(UInt32& val);
	bool getU64(UInt64& val);
	
	void put(const char* val, size_t len);
	void put(const std::string& val);
	void putU8(UInt8 val);
	void putU16(UInt16 val);
	void putU24(UInt32 val);
	void putU32(UInt32 val);
	void putU64(UInt64 val);
	
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

	void operator++(int) { _position++; }			// increment position by 1
	void operator--(int) { _position--; }			// deincrement position by 1
	
	void operator+=(int val) { _position += val; }	// increment position by x
	void operator-=(int val) { _position -= val; }	// deincrement position by x

	std::string toString() { return std::string(data(), size()); }
	
    friend std::ostream& operator << (std::ostream& stream, const Buffer& buf) 
	{
		return stream.put(buf.data(), buf.limit());
    }

protected:
	char* _bytes;
	size_t _max;	// size of internal buffer
	size_t _position;	// current read position
	size_t _limit;	// number of bytes written to buffer
};
*/