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


#ifndef SOURCEY_ICE_Utilities
#define SOURCEY_ICE_Utilities


#include "Sourcey/Base.h"


namespace Sourcey {
namespace Util {


inline std::string CodecToSDP(const Media::Codec& codec)  
{
	std::ostringstream os;
	os << "a=rtpmap:" 
		<< codec.id << " " 
		<< codec.name << "/" 
		<< codec.sampleRate 
		<< "\r\n";
	return os.str();
}

inline Media::Codec SDPToCodec(const std::string& sdp)
{
	Media::Codec codec;
	std::string value = sdp;
	std::string::size_type start = 0;
	std::string::size_type end = 0;

	end = value.find("a=rtpmap:");
	if (end != std::string::npos) {
		start = end+9;
	}
		
	// codec id
	end = value.find(" ");
	if (end != std::string::npos) {
		codec.id = atoi(value.substr(start, end).c_str());
	}

	// codec name
	start = end+1;
	end = value.find("/");
	if (end != std::string::npos) {
		codec.name = value.substr(start, end-start);
	}

	// clock rate / sampling rate
	start = end+1;
	end = value.length(); 
	codec.sampleRate = atoi(value.substr(start, end-start).c_str());

	return codec;
}


} // namespace Util
} // namespace Sourcey


#endif // SOURCEY_BASE_Utilities