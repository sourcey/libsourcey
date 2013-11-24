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


#ifndef SCY_STUN_H
#define SCY_STUN_H


#include "scy/base.h"
#include "scy/types.h"


namespace scy {
namespace stun {


// The mask used to determine whether a STUN message is a request/response etc.
const UInt32 kTypeMask = 0x0110;

// Following values correspond to RFC5389.
const int kAttributeHeaderSize = 4;
const int kMessageHeaderSize = 20;
const int kTransactionIdOffset = 8;
const int kTransactionIdLength = 12;
const UInt32 kMagicCookie = 0x2112A442;
const int kMagicCookieLength = sizeof(kMagicCookie);

enum AddressFamily 		
	// STUN address types as defined in RFC 5389.
	// NB: Undefined is not part of the STUN spec.
{
	Undefined    = 0,
	IPv4         = 1,
	IPv6         = 2
};


#define IS_STUN_REQUEST(msg_type)       (((msg_type) & 0x0110) == 0x0000)
#define IS_STUN_INDICATION(msg_type)    (((msg_type) & 0x0110) == 0x0010)
#define IS_STUN_SUCCESS_RESP(msg_type)  (((msg_type) & 0x0110) == 0x0100)
#define IS_STUN_ERR_RESP(msg_type)      (((msg_type) & 0x0110) == 0x0110)


#define GET_STUN_REQUEST(msg_type)      (msg_type & 0xFEEF)
#define GET_STUN_INDICATION(msg_type)   ((msg_type & 0xFEEF)|0x0010)
#define GET_STUN_SUCCESS_RESP(msg_type) ((msg_type & 0xFEEF)|0x0100)
#define GET_STUN_ERR_RESP(msg_type)      (msg_type | 0x0110)

/*
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
*/


} } // namespace scy:stun


#endif // SCY_STUN_H
