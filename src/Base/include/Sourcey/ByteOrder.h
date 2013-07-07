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


#ifndef SOURCEY_ByteOrder_H
#define SOURCEY_ByteOrder_H


#include "Sourcey/Types.h"


#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include <math.h>


namespace scy {


// Host to network 64
inline unsigned long long htonll(unsigned long long v) {
	union { unsigned long lv[2]; unsigned long long llv; } u;
	u.lv[0] = htonl(v >> 32);
	u.lv[1] = htonl(v & 0xFFFFFFFFULL);
	return u.llv;
}


// Network to host 64
inline unsigned long long ntohll(unsigned long long v) {
	union { unsigned long lv[2]; unsigned long long llv; } u;
	u.llv = v;
	return ((unsigned long long)ntohl(u.lv[0]) << 32) | (unsigned long long)ntohl(u.lv[1]);
}


// Check if the current host is big endian.
inline bool isHostBigEndian() {
	static const int number = 1;
	return (0 == *reinterpret_cast<const char*>(&number));
}


inline double IntToDouble(Int64 v) {
	if (v+v > 0xFFEULL<<52)
		return 0;
	return ldexp((double)((v&((1LL<<52)-1)) + (1LL<<52)) * (v>>63|1), (int)(v>>52&0x7FF)-1075);
}


inline float IntToFloat(Int32 v) {
	if (v+v > 0xFF000000U)
		return 0;
	return ldexp((float)((v&0x7FFFFF) + (1<<23)) * (v>>31|1), (int)(v>>23&0xFF)-150);
}


inline Int64 DoubleToInt(double d) {
	int e;
	if     ( !d) return 0;
	else if(d-d) return 0x7FF0000000000000LL + ((Int64)(d<0)<<63) + (d!=d);
	d = frexp(d, &e);
	return (Int64)(d<0)<<63 | (e+1022LL)<<52 | (Int64)((fabs(d)-0.5)*(1LL<<53));
}


inline Int32 FloatToInt(float d) {
	int e;
	if     ( !d) return 0;
	else if(d-d) return 0x7F800000 + ((d<0)<<31) + (d!=d);
	d = frexp(d, &e);
	return (d<0)<<31 | (e+126)<<23 | (Int32)((fabs(d)-0.5)*(1<<24));
}


// Reading and writing of little and big-endian numbers from memory
// TODO: Add HostEndian #defines (HE)
// TODO: Consider NetworkEndian as synonym for BigEndian, for clarity in use.
// TODO: Consider creating optimized versions, such as direct read/writes of
// integers in host-endian format, when the platform supports it.

inline void Set8(void* memory, size_t offset, UInt8 v) {
	static_cast<UInt8*>(memory)[offset] = v;
}

inline UInt8 Get8(const void* memory, size_t offset) {
	return static_cast<const UInt8*>(memory)[offset];
}

inline void SetBE16(void* memory, UInt16 v) {
	Set8(memory, 0, static_cast<UInt8>(v >>  8));
	Set8(memory, 1, static_cast<UInt8>(v >>  0));
}

inline void SetBE32(void* memory, UInt32 v) {
	Set8(memory, 0, static_cast<UInt8>(v >> 24));
	Set8(memory, 1, static_cast<UInt8>(v >> 16));
	Set8(memory, 2, static_cast<UInt8>(v >>  8));
	Set8(memory, 3, static_cast<UInt8>(v >>  0));
}

inline void SetBE64(void* memory, UInt64 v) {
	Set8(memory, 0, static_cast<UInt8>(v >> 56));
	Set8(memory, 1, static_cast<UInt8>(v >> 48));
	Set8(memory, 2, static_cast<UInt8>(v >> 40));
	Set8(memory, 3, static_cast<UInt8>(v >> 32));
	Set8(memory, 4, static_cast<UInt8>(v >> 24));
	Set8(memory, 5, static_cast<UInt8>(v >> 16));
	Set8(memory, 6, static_cast<UInt8>(v >>  8));
	Set8(memory, 7, static_cast<UInt8>(v >>  0));
}

inline UInt16 GetBE16(const void* memory) {
	return (static_cast<UInt16>(Get8(memory, 0)) << 8)
		| (static_cast<UInt16>(Get8(memory, 1)) << 0);
}

inline UInt32 GetBE32(const void* memory) {
	return (static_cast<UInt32>(Get8(memory, 0)) << 24)
		| (static_cast<UInt32>(Get8(memory, 1)) << 16)
		| (static_cast<UInt32>(Get8(memory, 2)) <<  8)
		| (static_cast<UInt32>(Get8(memory, 3)) <<  0);
}

inline UInt64 GetBE64(const void* memory) {
	return (static_cast<UInt64>(Get8(memory, 0)) << 56)
		| (static_cast<UInt64>(Get8(memory, 1)) << 48)
		| (static_cast<UInt64>(Get8(memory, 2)) << 40)
		| (static_cast<UInt64>(Get8(memory, 3)) << 32)
		| (static_cast<UInt64>(Get8(memory, 4)) << 24)
		| (static_cast<UInt64>(Get8(memory, 5)) << 16)
		| (static_cast<UInt64>(Get8(memory, 6)) <<  8)
		| (static_cast<UInt64>(Get8(memory, 7)) <<  0);
}

inline void SetLE16(void* memory, UInt16 v) {
	Set8(memory, 1, static_cast<UInt8>(v >>  8));
	Set8(memory, 0, static_cast<UInt8>(v >>  0));
}

inline void SetLE32(void* memory, UInt32 v) {
	Set8(memory, 3, static_cast<UInt8>(v >> 24));
	Set8(memory, 2, static_cast<UInt8>(v >> 16));
	Set8(memory, 1, static_cast<UInt8>(v >>  8));
	Set8(memory, 0, static_cast<UInt8>(v >>  0));
}

inline void SetLE64(void* memory, UInt64 v) {
	Set8(memory, 7, static_cast<UInt8>(v >> 56));
	Set8(memory, 6, static_cast<UInt8>(v >> 48));
	Set8(memory, 5, static_cast<UInt8>(v >> 40));
	Set8(memory, 4, static_cast<UInt8>(v >> 32));
	Set8(memory, 3, static_cast<UInt8>(v >> 24));
	Set8(memory, 2, static_cast<UInt8>(v >> 16));
	Set8(memory, 1, static_cast<UInt8>(v >>  8));
	Set8(memory, 0, static_cast<UInt8>(v >>  0));
}

inline UInt16 GetLE16(const void* memory) {
	return (static_cast<UInt16>(Get8(memory, 1)) << 8)
		| (static_cast<UInt16>(Get8(memory, 0)) << 0);
}

inline UInt32 GetLE32(const void* memory) {
	return (static_cast<UInt32>(Get8(memory, 3)) << 24)
		| (static_cast<UInt32>(Get8(memory, 2)) << 16)
		| (static_cast<UInt32>(Get8(memory, 1)) <<  8)
		| (static_cast<UInt32>(Get8(memory, 0)) <<  0);
}

inline UInt64 GetLE64(const void* memory) {
	return (static_cast<UInt64>(Get8(memory, 7)) << 56)
		| (static_cast<UInt64>(Get8(memory, 6)) << 48)
		| (static_cast<UInt64>(Get8(memory, 5)) << 40)
		| (static_cast<UInt64>(Get8(memory, 4)) << 32)
		| (static_cast<UInt64>(Get8(memory, 3)) << 24)
		| (static_cast<UInt64>(Get8(memory, 2)) << 16)
		| (static_cast<UInt64>(Get8(memory, 1)) <<  8)
		| (static_cast<UInt64>(Get8(memory, 0)) <<  0);
}


} // namespace scy


#endif // SOURCEY_ByteOrder_H