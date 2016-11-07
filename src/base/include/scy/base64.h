///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{
// This file uses the public domain libb64 library: http://libb64.sourceforge.net/
//


#ifndef SCY_Base64_H
#define SCY_Base64_H


#include "scy/interface.h"
#include "scy/logger.h"
#include <cstdint>
#include <iostream>
#include <memory>


namespace scy {
namespace base64 {


const int BUFFER_SIZE = 16384;
const int LINE_LENGTH = 72;


//
// Base64 Encoder
//


namespace internal {


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
    int nullptrlterminate; // added
} encodestate;

void init_encodestate(internal::encodestate* state_in);

char encode_value(char value_in);

int encode_block(const char* readbuf_in, int length_in, char* code_out, internal::encodestate* state_in);

int encode_blockend(char* code_out, internal::encodestate* state_in);


} // namespace internal


struct Encoder: public basic::Encoder
{
    Encoder(int buffersize = BUFFER_SIZE) :
        _buffersize(buffersize)
    {
        internal::init_encodestate(&_state);
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
            nread = static_cast<int>(istrm.gcount());
            enclen = encode(readbuf, nread, encbuf);
            ostrm.write(encbuf, enclen);
        }
        while (istrm.good() && nread > 0);

        enclen = finalize(encbuf);
        ostrm.write(encbuf, enclen);

        internal::init_encodestate(&_state);

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

        internal::init_encodestate(&_state);

        delete [] encbuf;
    }

    std::size_t encode(const char* inbuf, std::size_t nread, char* outbuf)
    {
        return internal::encode_block(inbuf, nread, outbuf, &_state);
    }

    std::size_t finalize(char* outbuf)
    {
        return internal::encode_blockend(outbuf, &_state);
    }

    void setLineLength(int lineLength)
    {
        _state.linelength = lineLength;
    }

    internal::encodestate _state;
    int _buffersize;
};


/// Converts a STL container to Base64.
template<typename T>
inline std::string encode(const T& bytes, int lineLength = LINE_LENGTH)
{
    std::string res;
    res.reserve(bytes.size() * 2);
    std::unique_ptr<char[]> encbuf(new char[bytes.size() * 2]);

    internal::encodestate state;
    internal::init_encodestate(&state);
    state.linelength = lineLength;

    int enclen = internal::encode_block(reinterpret_cast<const char*>(&bytes[0]), bytes.size(), encbuf.get(), &state);
    res.append(encbuf.get(), enclen);

    enclen = internal::encode_blockend(encbuf.get(), &state);
    res.append(encbuf.get(), enclen);

    return res;
}


//
// Base64 Decoder
//


namespace internal {


typedef enum
{
    step_a, step_b, step_c, step_d
} decodestep;

typedef struct
{
    decodestep step;
    char plainchar;
} decodestate;

void init_decodestate(internal::decodestate* state_in);

int decode_value(char value_in);

int decode_block(const char* inbuf, const int nread, char* outbuf, internal::decodestate* state_in);


} // namespace internal


struct Decoder : public basic::Decoder
{
    Decoder(int buffersize = BUFFER_SIZE) :
        _buffersize(buffersize)
    {
        internal::init_decodestate(&_state);
    }

    int decode(char value_in)
    {
        return internal::decode_value(value_in);
    }

    std::size_t decode(const char* inbuf, std::size_t nread, char* outbuf)
    {
        return internal::decode_block(inbuf, nread, outbuf, &_state);
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
            declen = static_cast<int>(istrm.gcount());
            nread = decode(decbuf, declen, readbuf);
            ostrm.write((const char*)readbuf, nread);
        }
        while (istrm.good() && declen > 0);

        internal::init_decodestate(&_state);

        delete [] decbuf;
        delete [] readbuf;
    }

    internal::decodestate _state;
    int _buffersize;
};


/// Decodes a STL container from Base64.
template<typename T>
inline std::string decode(const T& bytes)
{
    std::string res;
    res.reserve(bytes.size() * 2);
    std::unique_ptr<char[]> encbuf(new char[bytes.size() * 2]);

    internal::decodestate state;
    internal::init_decodestate(&state);

    int enclen = internal::decode_block(reinterpret_cast<const char*>(&bytes[0]), bytes.size(), encbuf.get(), &state);
    res.append(encbuf.get(), enclen);

    return res;
}


} } // namespace scy::base64


#endif // SCY_Base64_H

/// @\}
