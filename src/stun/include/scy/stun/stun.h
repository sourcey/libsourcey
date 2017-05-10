///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @defgroup stun STUN module (rfc5389)
///
/// The `stun` module contains a STUN (rfc5389) implementation.
///
/// @addtogroup stun
/// @{


#ifndef SCY_STUN_H
#define SCY_STUN_H


#include "scy/base.h"
#include <cstdint>


// Shared library exports
#if defined(SCY_WIN) && defined(SCY_SHARED_LIBRARY)
    #if defined(STUN_EXPORTS)
        #define STUN_API __declspec(dllexport)
    #else
        #define STUN_API __declspec(dllimport)
    #endif
#else
    #define STUN_API // nothing
#endif


namespace scy {
namespace stun {


// Following values correspond to RFC5389.
const int kAttributeHeaderSize = 4;
const int kMessageHeaderSize = 20;
const int kTransactionIdOffset = 8;
const int kTransactionIdLength = 12;
const uint32_t kMagicCookie = 0x2112A442;
const int kMagicCookieLength = sizeof(kMagicCookie);


/// STUN address types as defined in RFC 5389.
/// NB: Undefined is not part of the STUN spec.
enum AddressFamily
{
    Undefined = 0,
    IPv4 = 1,
    IPv6 = 2
};


#if 0
inline bool isChannelData(uint16_t msgType)
{
  // The first two bits of a channel data message are 0b01.
  return ((msgType & 0xC000) == 0x4000);
}

inline bool isRequestType(int msgType) {
     return ((msgType & 0x0110) == 0x000);
}

inline bool isIndicationType(int msgType) {
    return ((msgType & 0x0110) == 0x010);
}

inline bool isSuccessResponseType(int msgType) {
    return ((msgType & 0x0110) == 0x100);
}

inline bool isErrorResponseType(int msgType) {
  return ((msgType & 0x0110) == 0x110);
}

inline int getSuccessResponseType(int reqType) {
    return isRequestType(reqType) ? (reqType | 0x100) : -1;
}

inline int getErrorResponseType(int reqType) {
    return isRequestType(reqType) ? (reqType | 0x110) : -1;
}


#define IS_STUN_REQUEST(msgType) (((msgType)&0x0110) == 0x0000)
#define IS_STUN_INDICATION(msgType) (((msgType)&0x0110) == 0x0010)
#define IS_STUN_SUCCESS_RESP(msgType) (((msgType)&0x0110) == 0x0100)
#define IS_STUN_ERR_RESP(msgType) (((msgType)&0x0110) == 0x0110)

#define GET_STUN_REQUEST(msgType) (msgType & 0xFEEF)
#define GET_STUN_INDICATION(msgType) ((msgType & 0xFEEF) | 0x0010)
#define GET_STUN_SUCCESS_RESP(msgType) ((msgType & 0xFEEF) | 0x0100)
#define GET_STUN_ERR_RESP(msgType) (msgType | 0x0110)

#define STUN_HEADER_LENGTH (20)
#define STUN_CHANNEL_HEADER_LENGTH (4)

#define STUN_MAX_USERNAME_SIZE (513)
#define STUN_MAX_REALM_SIZE (127)
#define STUN_MAX_NONCE_SIZE (127)
#define STUN_MAX_PWD_SIZE (127)

#define STUN_MAGIC_COOKIE (0x2112A442)

// Lifetimes:
#define STUN_DEFAULT_ALLOCATE_LIFETIME (600)
#define STUN_MIN_ALLOCATE_LIFETIME STUN_DEFAULT_ALLOCATE_LIFETIME
#define STUN_MAX_ALLOCATE_LIFETIME (3600)
#define STUN_CHANNEL_LIFETIME (600)
#define STUN_PERMISSION_LIFETIME (300)
#define STUN_NONCE_EXPIRATION_TIME (600)
#endif


} } // namespace scy:stun


#endif // SCY_STUN_H


/// @\}
