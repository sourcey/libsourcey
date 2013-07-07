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


#ifndef SOURCEY_RTP_Types_H
#define SOURCEY_RTP_Types_H


#include "Sourcey/Types.h"


namespace scy { 
namespace rtp {
	
	
#define RTP_MAX_PACKET_LEN 1500

const int kRtpBaseHeaderSize = 12;
const UInt8 kRtpVersionNumber = 2;
const int kRtpMaxSources = 16;
const int kBytesPerCSRC = 4;


} } // namespace scy::RTP


#endif // SOURCEY_RTP_Types_H
