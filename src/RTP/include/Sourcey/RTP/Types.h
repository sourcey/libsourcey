//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_RTP_Types_H
#define SOURCEY_RTP_Types_H


#include "Sourcey/Types.h"


namespace Sourcey { 
namespace RTP {
	
	
#define RTP_MAX_PACKET_LEN 1500

const int kRtpBaseHeaderSize = 12;
const UInt8 kRtpVersionNumber = 2;
const int kRtpMaxSources = 16;
const int kBytesPerCSRC = 4;


} } // namespace Sourcey::RTP


#endif // SOURCEY_RTP_Types_H
