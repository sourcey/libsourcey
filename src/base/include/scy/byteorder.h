///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_ByteOrder_H
#define SCY_ByteOrder_H


#include <cstdint>
#include <cstring>

#ifdef POSIX
#include <arpa/inet.h>
#endif

#ifdef SCY_WIN
#include <stdlib.h>
#endif


namespace scy {


enum class ByteOrder
{
    Network = 0, ///< Default, use network byte order (big endian).
    Host,        ///< Use the native order of the host.
};


inline void set8(void* memory, std::size_t offset, std::uint8_t v)
{
    static_cast<std::uint8_t*>(memory)[offset] = v;
}

inline std::uint8_t get8(const void* memory, std::size_t offset)
{
    return static_cast<const std::uint8_t*>(memory)[offset];
}

inline void setBE16(void* memory, std::uint16_t v)
{
    set8(memory, 0, static_cast<std::uint8_t>(v >> 8));
    set8(memory, 1, static_cast<std::uint8_t>(v >> 0));
}

inline void setBE32(void* memory, std::uint32_t v)
{
    set8(memory, 0, static_cast<std::uint8_t>(v >> 24));
    set8(memory, 1, static_cast<std::uint8_t>(v >> 16));
    set8(memory, 2, static_cast<std::uint8_t>(v >> 8));
    set8(memory, 3, static_cast<std::uint8_t>(v >> 0));
}

inline void setBE64(void* memory, std::uint64_t v)
{
    set8(memory, 0, static_cast<std::uint8_t>(v >> 56));
    set8(memory, 1, static_cast<std::uint8_t>(v >> 48));
    set8(memory, 2, static_cast<std::uint8_t>(v >> 40));
    set8(memory, 3, static_cast<std::uint8_t>(v >> 32));
    set8(memory, 4, static_cast<std::uint8_t>(v >> 24));
    set8(memory, 5, static_cast<std::uint8_t>(v >> 16));
    set8(memory, 6, static_cast<std::uint8_t>(v >> 8));
    set8(memory, 7, static_cast<std::uint8_t>(v >> 0));
}

inline std::uint16_t getBE16(const void* memory)
{
    return static_cast<std::uint16_t>((get8(memory, 0) << 8) |
                                      (get8(memory, 1) << 0));
}

inline std::uint32_t getBE32(const void* memory)
{
    return (static_cast<std::uint32_t>(get8(memory, 0)) << 24) |
           (static_cast<std::uint32_t>(get8(memory, 1)) << 16) |
           (static_cast<std::uint32_t>(get8(memory, 2)) << 8) |
           (static_cast<std::uint32_t>(get8(memory, 3)) << 0);
}

inline std::uint64_t getBE64(const void* memory)
{
    return (static_cast<std::uint64_t>(get8(memory, 0)) << 56) |
           (static_cast<std::uint64_t>(get8(memory, 1)) << 48) |
           (static_cast<std::uint64_t>(get8(memory, 2)) << 40) |
           (static_cast<std::uint64_t>(get8(memory, 3)) << 32) |
           (static_cast<std::uint64_t>(get8(memory, 4)) << 24) |
           (static_cast<std::uint64_t>(get8(memory, 5)) << 16) |
           (static_cast<std::uint64_t>(get8(memory, 6)) << 8) |
           (static_cast<std::uint64_t>(get8(memory, 7)) << 0);
}

inline void setLE16(void* memory, std::uint16_t v)
{
    set8(memory, 0, static_cast<std::uint8_t>(v >> 0));
    set8(memory, 1, static_cast<std::uint8_t>(v >> 8));
}

inline void setLE32(void* memory, std::uint32_t v)
{
    set8(memory, 0, static_cast<std::uint8_t>(v >> 0));
    set8(memory, 1, static_cast<std::uint8_t>(v >> 8));
    set8(memory, 2, static_cast<std::uint8_t>(v >> 16));
    set8(memory, 3, static_cast<std::uint8_t>(v >> 24));
}

inline void setLE64(void* memory, std::uint64_t v)
{
    set8(memory, 0, static_cast<std::uint8_t>(v >> 0));
    set8(memory, 1, static_cast<std::uint8_t>(v >> 8));
    set8(memory, 2, static_cast<std::uint8_t>(v >> 16));
    set8(memory, 3, static_cast<std::uint8_t>(v >> 24));
    set8(memory, 4, static_cast<std::uint8_t>(v >> 32));
    set8(memory, 5, static_cast<std::uint8_t>(v >> 40));
    set8(memory, 6, static_cast<std::uint8_t>(v >> 48));
    set8(memory, 7, static_cast<std::uint8_t>(v >> 56));
}

inline std::uint16_t getLE16(const void* memory)
{
    return static_cast<std::uint16_t>((get8(memory, 0) << 0) |
                                      (get8(memory, 1) << 8));
}

inline std::uint32_t getLE32(const void* memory)
{
    return (static_cast<std::uint32_t>(get8(memory, 0)) << 0) |
           (static_cast<std::uint32_t>(get8(memory, 1)) << 8) |
           (static_cast<std::uint32_t>(get8(memory, 2)) << 16) |
           (static_cast<std::uint32_t>(get8(memory, 3)) << 24);
}

inline std::uint64_t getLE64(const void* memory)
{
    return (static_cast<std::uint64_t>(get8(memory, 0)) << 0) |
           (static_cast<std::uint64_t>(get8(memory, 1)) << 8) |
           (static_cast<std::uint64_t>(get8(memory, 2)) << 16) |
           (static_cast<std::uint64_t>(get8(memory, 3)) << 24) |
           (static_cast<std::uint64_t>(get8(memory, 4)) << 32) |
           (static_cast<std::uint64_t>(get8(memory, 5)) << 40) |
           (static_cast<std::uint64_t>(get8(memory, 6)) << 48) |
           (static_cast<std::uint64_t>(get8(memory, 7)) << 56);
}

// Check if the current host is big endian.
inline bool isBigEndian()
{
    static const int number = 1;
    return 0 == *reinterpret_cast<const char*>(&number);
}

inline std::uint16_t hostToNetwork16(std::uint16_t n)
{
    std::uint16_t result;
    setBE16(&result, n);
    return result;
}

inline std::uint32_t hostToNetwork32(std::uint32_t n)
{
    std::uint32_t result;
    setBE32(&result, n);
    return result;
}

inline std::uint64_t hostToNetwork64(std::uint64_t n)
{
    std::uint64_t result;
    setBE64(&result, n);
    return result;
}

inline std::uint16_t networkToHost16(std::uint16_t n)
{
    return getBE16(&n);
}

inline std::uint32_t networkToHost32(std::uint32_t n)
{
    return getBE32(&n);
}

inline std::uint64_t networkToHost64(std::uint64_t n)
{
    return getBE64(&n);
}


} // namespace scy


#endif // SCY_ByteOrder_H


/// @\}
