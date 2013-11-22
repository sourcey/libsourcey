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

#define ENBALE_MESSAGE_PADDING 1


} } // namespace scy:stun


#endif // SCY_STUN_H
