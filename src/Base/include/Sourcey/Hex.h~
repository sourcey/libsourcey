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
// This file uses the public domain libb64 library: http://libb64.sourceforge.net/
//


#ifndef SOURCEY_Hex_H
#define SOURCEY_Hex_H


#include "Sourcey/Interface.h"
#include "Sourcey/Exception.h"
#include "Sourcey/Logger.h"
#include <iostream>
#include <assert.h>
#include <cstring>


namespace scy {
namespace hex {
	

//
// Hex Encoder
//


struct Encoder: public basic::Encoder
{	
	Encoder() : 
		_linePos(0),
		_lineLength(72),
		_uppercase(0)
	{
	}
	
	virtual std::size_t encode(const char* inbuf, std::size_t nread, char* outbuf)
	{
		static const int eof = std::char_traits<char>::eof();
		static const char digits[] = "0123456789abcdef0123456789ABCDEF";
	
		char c;
		std::size_t nwrite = 0;
		for (unsigned i = 0; i < nread; i++) 
		{	
			c = inbuf[i];
			std::memcpy(outbuf + nwrite++, &digits[_uppercase + ((c >> 4) & 0xF)], 1);
			std::memcpy(outbuf + nwrite++, &digits[_uppercase + (c & 0xF)], 1);
			if (_lineLength > 0 && (_linePos += 2) >= _lineLength) { //++_linePos//++_linePos;
				_linePos = 0;
				std::memcpy(outbuf + nwrite++, "\n", 1);
			}
		}

		return nwrite;
	}

	virtual std::size_t finalize(char* /* outbuf */)
	{
		return 0;
	}

	void setUppercase(bool flag)
	{
		_uppercase = flag ? 16 : 0;
	}
	
	void setLineLength(int lineLength)
	{
		_lineLength = lineLength;
	}
	
	int _uppercase;
	int _linePos;
	int _lineLength;
};


template<typename T>
inline std::string encode(const T& bytes)
	// Converts the STL container to Hex.
{
	static const char digits[] = "0123456789abcdef";
	std::string res;
	res.reserve(bytes.size() * 2);
	for (typename  T::const_iterator it = bytes.begin(); it != bytes.end(); ++it) {
		const unsigned char c = static_cast<const unsigned char>(*it);
		res += digits[(c >> 4) & 0xF];
		res += digits[c & 0xF];
	}
	return res;
}
	

//
// Hex Decoder
//


struct Decoder: public basic::Decoder
{		
	Decoder() : lastbyte('\0') {}
	virtual ~Decoder() {} 

	virtual std::size_t decode(const char* inbuf, std::size_t nread, char* outbuf)
	{
		int n;
		char c;
		std::size_t rpos = 0;
		std::size_t nwrite = 0;	
		while (rpos < nread)
		{
			if (readnext(inbuf, nread, rpos, c))
				n = (nybble(c) << 4);

			else if (rpos >= nread) {	
				// Store the last byte to be
				// prepended on next decode()
				if (!iswspace(inbuf[rpos - 1]))
					std::memcpy(&lastbyte, &inbuf[rpos - 1], 1); 	
				break;
			}
			
			readnext(inbuf, nread, rpos, c);
			n = n | nybble(c);
			std::memcpy(outbuf + nwrite++, &n, 1);
		}
		return nwrite;
	}

	virtual std::size_t finalize(char* /* outbuf */)
	{
		return 0;
	}
	
	bool readnext(const char* inbuf, std::size_t nread, std::size_t& rpos, char& c)
	{
		if (rpos == 0 && lastbyte != '\0') {
			assert(!iswspace(lastbyte));
			c = lastbyte;
			lastbyte = '\0';
		}
		else {
			c = inbuf[rpos++];
			while (iswspace(c) && rpos < nread)
				c = inbuf[rpos++];
		}
		return rpos < nread;
	}

	int nybble(const int n)
	{
		if      (n >= '0' && n <= '9') return n - '0';
		else if (n >= 'A' && n <= 'F') return n - ('A' - 10);
		else if (n >= 'a' && n <= 'f') return n - ('a' - 10);
		else throw std::runtime_error("Invalid hex format");
	}

	bool iswspace(const char c)
	{
		return c == ' ' || c == '\r' || c == '\t' || c == '\n';
	}

	char lastbyte;
};


} } // namespace scy::hex


#endif // SOURCEY_Hex_H






/*
struct Decoder: public basic::Decoder
{		
	char lastbyte;
	char* remaining;
	int nremaining;

	Decoder() : remaining(nullptr), nremaining(0), lastbyte('\0') {} //

	~Decoder() 
	{
		if (remaining)
			delete [] remaining;
	} 

	virtual std::size_t finalize(char* outbuf)
	{
		return 0;
	}

	virtual std::size_t decode(const char* inbuf, std::size_t nread, char* outbuf)
	{
		int n;
		std::size_t nwrite = 0;	

		// copy last byte if available
		bool startodd = lastbyte != '\0';
		if (startodd) {
			n = (nybble(lastbyte) << 4);
			lastbyte = '\0';
		}
		
		for (std::size_t i = 0; i < nread; i++)
		{
			if (iswspace(inbuf[i])) {
				++nremaining; 
				continue;
			}

			if (!startodd || (startodd && i > 0))
				n = (nybble(inbuf[i++]) << 4);

			// store the lastbyte digits to be
			// flushed on next decode() or finalize()
			if (i >= nread) { //nremaining - 1nremaining && 
				std::memcpy(&lastbyte, &inbuf[i - 1], 1); 
				break;
			}
			
			n = n | nybble(inbuf[i]);
			std::memcpy(outbuf + nwrite++, &n, 1);
		}
		return nwrite;
	}

	bool readnext(const char* inbuf, std::size_t nread, std::size_t& rpos, char& ch)
	{
		//char ch;
		if (rpos == 0 && lastbyte != '\0') {
			ch = lastbyte;
			lastbyte = '\0';
		}
		else {
			ch = inbuf[rpos++];
			while ((ch == ' ' || ch == '\r' || ch == '\t' || ch == '\n') && rpos < nread)
				ch = inbuf[rpos++];
		}
		return rpos < nread;
	}

	bool iswspace(const char c)
	{
		return c == ' ' || c == '\r' || c == '\t' || c == '\n';
	}

	int nybble(const int n)
	{
		if      (n >= '0' && n <= '9') return n - '0';
		else if (n >= 'A' && n <= 'F') return n - ('A' - 10);
		else if (n >= 'a' && n <= 'f') return n - ('a' - 10);
		else throw std::runtime_error("Invalid Hex format"); //0; //return
	}
};

*/
	/*
	bool readnext(const char* inbuf, std::size_t nread, std::size_t& rpos, char& ch)
	{
		if (rpos == 0 && lastbyte != '\0') {
			ch = lastbyte;
			lastbyte = '\0';
		}
		else {
			assert(rpos < nread);
			ch = inbuf[rpos++];
			while (iswspace(ch) && rpos < nread)
				ch = inbuf[rpos++];
		}
		return rpos < nread;
	}
	*/

	//find out which X number is failing and solve from there