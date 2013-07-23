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


/*
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include <math.h>
*/
#ifdef POSIX
#include <arpa/inet.h>
#endif

#ifdef WIN32
#include <stdlib.h>
#endif


namespace scy {
	

/*
inline void set8(void* memory, size_t offset, UInt8 v) {
	static_cast<UInt8*>(memory)[offset] = v;
}

inline UInt8 get8(const void* memory, size_t offset) {
	return static_cast<const UInt8*>(memory)[offset];
}

inline void setBE16(void* memory, UInt16 v) {
	set8(memory, 0, static_cast<UInt8>(v >>  8));
	set8(memory, 1, static_cast<UInt8>(v >>  0));
}

inline void setBE32(void* memory, UInt32 v) {
	set8(memory, 0, static_cast<UInt8>(v >> 24));
	set8(memory, 1, static_cast<UInt8>(v >> 16));
	set8(memory, 2, static_cast<UInt8>(v >>  8));
	set8(memory, 3, static_cast<UInt8>(v >>  0));
}

inline void setBE64(void* memory, UInt64 v) {
	set8(memory, 0, static_cast<UInt8>(v >> 56));
	set8(memory, 1, static_cast<UInt8>(v >> 48));
	set8(memory, 2, static_cast<UInt8>(v >> 40));
	set8(memory, 3, static_cast<UInt8>(v >> 32));
	set8(memory, 4, static_cast<UInt8>(v >> 24));
	set8(memory, 5, static_cast<UInt8>(v >> 16));
	set8(memory, 6, static_cast<UInt8>(v >>  8));
	set8(memory, 7, static_cast<UInt8>(v >>  0));
}

inline UInt16 getBE16(const void* memory) {
	return (static_cast<UInt16>(get8(memory, 0)) << 8)
		| (static_cast<UInt16>(get8(memory, 1)) << 0);
}

inline UInt32 getBE32(const void* memory) {
	return (static_cast<UInt32>(get8(memory, 0)) << 24)
		| (static_cast<UInt32>(get8(memory, 1)) << 16)
		| (static_cast<UInt32>(get8(memory, 2)) <<  8)
		| (static_cast<UInt32>(get8(memory, 3)) <<  0);
}

inline UInt64 getBE64(const void* memory) {
	return (static_cast<UInt64>(get8(memory, 0)) << 56)
		| (static_cast<UInt64>(get8(memory, 1)) << 48)
		| (static_cast<UInt64>(get8(memory, 2)) << 40)
		| (static_cast<UInt64>(get8(memory, 3)) << 32)
		| (static_cast<UInt64>(get8(memory, 4)) << 24)
		| (static_cast<UInt64>(get8(memory, 5)) << 16)
		| (static_cast<UInt64>(get8(memory, 6)) <<  8)
		| (static_cast<UInt64>(get8(memory, 7)) <<  0);
}

inline void setLE16(void* memory, UInt16 v) {
	set8(memory, 1, static_cast<UInt8>(v >>  8));
	set8(memory, 0, static_cast<UInt8>(v >>  0));
}

inline void setLE32(void* memory, UInt32 v) {
	set8(memory, 3, static_cast<UInt8>(v >> 24));
	set8(memory, 2, static_cast<UInt8>(v >> 16));
	set8(memory, 1, static_cast<UInt8>(v >>  8));
	set8(memory, 0, static_cast<UInt8>(v >>  0));
}

inline void setLE64(void* memory, UInt64 v) {
	set8(memory, 7, static_cast<UInt8>(v >> 56));
	set8(memory, 6, static_cast<UInt8>(v >> 48));
	set8(memory, 5, static_cast<UInt8>(v >> 40));
	set8(memory, 4, static_cast<UInt8>(v >> 32));
	set8(memory, 3, static_cast<UInt8>(v >> 24));
	set8(memory, 2, static_cast<UInt8>(v >> 16));
	set8(memory, 1, static_cast<UInt8>(v >>  8));
	set8(memory, 0, static_cast<UInt8>(v >>  0));
}

inline UInt16 getLE16(const void* memory) {
	return (static_cast<UInt16>(get8(memory, 1)) << 8)
		| (static_cast<UInt16>(get8(memory, 0)) << 0);
}

inline UInt32 getLE32(const void* memory) {
	return (static_cast<UInt32>(get8(memory, 3)) << 24)
		| (static_cast<UInt32>(get8(memory, 2)) << 16)
		| (static_cast<UInt32>(get8(memory, 1)) <<  8)
		| (static_cast<UInt32>(get8(memory, 0)) <<  0);
}

inline UInt64 getLE64(const void* memory) {
	return (static_cast<UInt64>(get8(memory, 7)) << 56)
		| (static_cast<UInt64>(get8(memory, 6)) << 48)
		| (static_cast<UInt64>(get8(memory, 5)) << 40)
		| (static_cast<UInt64>(get8(memory, 4)) << 32)
		| (static_cast<UInt64>(get8(memory, 3)) << 24)
		| (static_cast<UInt64>(get8(memory, 2)) << 16)
		| (static_cast<UInt64>(get8(memory, 1)) <<  8)
		| (static_cast<UInt64>(get8(memory, 0)) <<  0);
}


// Check if the current host is big endian.
inline bool isHostBigEndian() {
	static const int number = 1;
	return (0 == *reinterpret_cast<const char*>(&number));
}


inline UInt16 hostToNetwork16(UInt16 n) {
	return htons(n);
}


inline UInt32 hostToNetwork32(UInt32 n) {
	return htonl(n);
}

inline unsigned long long htonll(unsigned long long v) {
    union { unsigned long lv[2]; unsigned long long llv; } u;
    u.lv[0] = htonl(v >> 32);
    u.lv[1] = htonl(v & 0xFFFFFFFFULL);
    return u.llv;
}

inline unsigned long long ntohll(unsigned long long v) {
    union { unsigned long lv[2]; unsigned long long llv; } u;
    u.llv = v;
    return ((unsigned long long)ntohl(u.lv[0]) << 32) | (unsigned long long)ntohl(u.lv[1]);
}


inline UInt64 hostToNetwork64(UInt64 n) {
	// If the host is little endian, 
	// getBE64 converts n to big network endian.
	return htonll(n); //isHostBigEndian() ? n : getBE64(&n);
}


inline UInt16 networkToHost16(UInt16 n) {
	return ntohs(n);
}


inline UInt32 networkToHost32(UInt32 n) {
	return ntohl(n);
}


inline UInt64 networkToHost64(UInt64 n) {
	// If the host is little endian, 
	// getBE64 converts n to little endian.
	return ntohll(n); //isHostBigEndian() ? n : getBE64(&n);
}


inline Int32 floatToInt(float d) {
	int e;
	if     ( !d) return 0;
	else if(d-d) return 0x7F800000 + ((d<0)<<31) + (d!=d);
	d = frexp(d, &e);
	return (d<0)<<31 | (e+126)<<23 | (Int32)((fabs(d)-0.5)*(1<<24));
}
*/


inline void set8(void* memory, size_t offset, UInt8 v) {
	static_cast<UInt8*>(memory)[offset] = v;
}

inline UInt8 get8(const void* memory, size_t offset) {
	return static_cast<const UInt8*>(memory)[offset];
}

inline void setBE16(void* memory, UInt16 v) {
	set8(memory, 0, static_cast<UInt8>(v >> 8));
	set8(memory, 1, static_cast<UInt8>(v >> 0));
}

inline void setBE32(void* memory, UInt32 v) {
	set8(memory, 0, static_cast<UInt8>(v >> 24));
	set8(memory, 1, static_cast<UInt8>(v >> 16));
	set8(memory, 2, static_cast<UInt8>(v >> 8));
	set8(memory, 3, static_cast<UInt8>(v >> 0));
}

inline void setBE64(void* memory, UInt64 v) {
	set8(memory, 0, static_cast<UInt8>(v >> 56));
	set8(memory, 1, static_cast<UInt8>(v >> 48));
	set8(memory, 2, static_cast<UInt8>(v >> 40));
	set8(memory, 3, static_cast<UInt8>(v >> 32));
	set8(memory, 4, static_cast<UInt8>(v >> 24));
	set8(memory, 5, static_cast<UInt8>(v >> 16));
	set8(memory, 6, static_cast<UInt8>(v >> 8));
	set8(memory, 7, static_cast<UInt8>(v >> 0));
}

inline UInt16 getBE16(const void* memory) {
	return static_cast<UInt16>((get8(memory, 0) << 8) |
		(get8(memory, 1) << 0));
}

inline UInt32 getBE32(const void* memory) {
	return (static_cast<UInt32>(get8(memory, 0)) << 24) |
		(static_cast<UInt32>(get8(memory, 1)) << 16) |
		(static_cast<UInt32>(get8(memory, 2)) << 8) |
		(static_cast<UInt32>(get8(memory, 3)) << 0);
}

inline UInt64 getBE64(const void* memory) {
	return (static_cast<UInt64>(get8(memory, 0)) << 56) |
		(static_cast<UInt64>(get8(memory, 1)) << 48) |
		(static_cast<UInt64>(get8(memory, 2)) << 40) |
		(static_cast<UInt64>(get8(memory, 3)) << 32) |
		(static_cast<UInt64>(get8(memory, 4)) << 24) |
		(static_cast<UInt64>(get8(memory, 5)) << 16) |
		(static_cast<UInt64>(get8(memory, 6)) << 8) |
		(static_cast<UInt64>(get8(memory, 7)) << 0);
}

inline void setLE16(void* memory, UInt16 v) {
	set8(memory, 0, static_cast<UInt8>(v >> 0));
	set8(memory, 1, static_cast<UInt8>(v >> 8));
}

inline void setLE32(void* memory, UInt32 v) {
	set8(memory, 0, static_cast<UInt8>(v >> 0));
	set8(memory, 1, static_cast<UInt8>(v >> 8));
	set8(memory, 2, static_cast<UInt8>(v >> 16));
	set8(memory, 3, static_cast<UInt8>(v >> 24));
}

inline void setLE64(void* memory, UInt64 v) {
	set8(memory, 0, static_cast<UInt8>(v >> 0));
	set8(memory, 1, static_cast<UInt8>(v >> 8));
	set8(memory, 2, static_cast<UInt8>(v >> 16));
	set8(memory, 3, static_cast<UInt8>(v >> 24));
	set8(memory, 4, static_cast<UInt8>(v >> 32));
	set8(memory, 5, static_cast<UInt8>(v >> 40));
	set8(memory, 6, static_cast<UInt8>(v >> 48));
	set8(memory, 7, static_cast<UInt8>(v >> 56));
}

inline UInt16 getLE16(const void* memory) {
	return static_cast<UInt16>((get8(memory, 0) << 0) |
		(get8(memory, 1) << 8));
}

inline UInt32 getLE32(const void* memory) {
	return (static_cast<UInt32>(get8(memory, 0)) << 0) |
		(static_cast<UInt32>(get8(memory, 1)) << 8) |
		(static_cast<UInt32>(get8(memory, 2)) << 16) |
		(static_cast<UInt32>(get8(memory, 3)) << 24);
}

inline UInt64 getLE64(const void* memory) {
	return (static_cast<UInt64>(get8(memory, 0)) << 0) |
		(static_cast<UInt64>(get8(memory, 1)) << 8) |
		(static_cast<UInt64>(get8(memory, 2)) << 16) |
		(static_cast<UInt64>(get8(memory, 3)) << 24) |
		(static_cast<UInt64>(get8(memory, 4)) << 32) |
		(static_cast<UInt64>(get8(memory, 5)) << 40) |
		(static_cast<UInt64>(get8(memory, 6)) << 48) |
		(static_cast<UInt64>(get8(memory, 7)) << 56);
}

// Check if the current host is big endian.
inline bool isHostBigEndian() {
	static const int number = 1;
	return 0 == *reinterpret_cast<const char*>(&number);
}

inline UInt16 hostToNetwork16(UInt16 n) {
	UInt16 result;
	setBE16(&result, n);
	return result;
}

inline UInt32 hostToNetwork32(UInt32 n) {
	UInt32 result;
	setBE32(&result, n);
	return result;
}

inline UInt64 hostToNetwork64(UInt64 n) {
	UInt64 result;
	setBE64(&result, n);
	return result;
}

inline UInt16 networkToHost16(UInt16 n) {
	return getBE16(&n);
}

inline UInt32 networkToHost32(UInt32 n) {
	return getBE32(&n);
}

inline UInt64 networkToHost64(UInt64 n) {
	return getBE64(&n);
}




} // namespace scy


#endif // SOURCEY_ByteOrder_H