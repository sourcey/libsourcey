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


#ifndef SOURCEY_Base64_H
#define SOURCEY_Base64_H


#include "Sourcey/Encoder.h"
#include "Sourcey/Decoder.h" 
#include "Sourcey/Logger.h" 
#include "Sourcey/Types.h"
#include <iostream>


namespace scy {
namespace base64 {


const int BUFFER_SIZE = 16384;
const int LINE_LENGTH = 72;


//
// Encoder
//
	

typedef enum
{
	step_A, step_B, step_C
} encodestep;

typedef struct
{
	encodestep step;
	char result;
	int stepcount;
	int linelength; // added
	int nullterminate; // added
} encodestate;

void init_encodestate(base64::encodestate* state_in);

char encode_value(char value_in);

int encode_block(const char* readbuf_in, int length_in, char* code_out, base64::encodestate* state_in);

int encode_blockend(char* code_out, base64::encodestate* state_in);


struct Encoder: public scy::Encoder
{
	Encoder(int buffersize = BUFFER_SIZE) : 
		_buffersize(buffersize)
	{
		init_encodestate(&_state);
	}

	void encode(std::istream& istrm, std::ostream& ostrm)
	{
		const int N = _buffersize;
		char* readbuf = new char[N];
		char* encbuf = new char[2*N];
		int nread;
		int enclen;

		do
		{
			istrm.read(readbuf, N);
			nread = istrm.gcount();			
			enclen = encode(readbuf, nread, encbuf);
			ostrm.write(encbuf, enclen);
		}
		while (istrm.good() && nread > 0);

		enclen = finalize(encbuf);
		ostrm.write(encbuf, enclen);

		init_encodestate(&_state);

		delete [] encbuf;
		delete [] readbuf;
	}
		
	void encode(const std::string& in, std::string& out)
	{
		char* encbuf = new char[in.length() * 2];
		int enclen = encode(in.c_str(), in.length(), encbuf);
		out.append(encbuf, enclen);

		enclen = finalize(encbuf);
		out.append(encbuf, enclen);

		init_encodestate(&_state);

		delete [] encbuf;
	}

	std::size_t encode(const char* inbuf, std::size_t nread, char* outbuf)
	{
		return base64::encode_block(inbuf, nread, outbuf, &_state);
	}

	std::size_t finalize(char* outbuf)
	{		
		return base64::encode_blockend(outbuf, &_state);
	}
	
	void setLineLength(int lineLength)
	{
		_state.linelength = lineLength;
	}

	encodestate _state;
	int _buffersize;
};


//
// Decoder
//


typedef enum
{
	step_a, step_b, step_c, step_d
} decodestep;

typedef struct
{
	decodestep step;
	char plainchar;
} decodestate;

void init_decodestate(base64::decodestate* state_in);

int decode_value(char value_in);

int decode_block(const char* inbuf, const int nread, char* outbuf, base64::decodestate* state_in);


struct Decoder : public scy::Decoder
{
	Decoder(int buffersize = BUFFER_SIZE) : 
		_buffersize(buffersize)
	{
		init_decodestate(&_state);
	}

	int decode(char value_in)
	{
		return base64::decode_value(value_in);
	}

	std::size_t decode(const char* inbuf, std::size_t nread, char* outbuf)
	{
		return base64::decode_block(inbuf, nread, outbuf, &_state);
	}

	void decode(std::istream& istrm, std::ostream& ostrm)
	{
		const int N = _buffersize;
		char* decbuf = new char[N];
		char* readbuf = new char[N];
		int declen;
		int nread;

		do
		{
			istrm.read((char*)decbuf, N);
			declen = istrm.gcount();
			nread = decode(decbuf, declen, readbuf);
			ostrm.write((const char*)readbuf, nread);
		}
		while (istrm.good() && declen > 0);

		init_decodestate(&_state);

		delete [] decbuf;
		delete [] readbuf;
	}

	decodestate _state;
	int _buffersize;
};


} } // namespace scy::base64


#endif // SOURCEY_Base64_H
