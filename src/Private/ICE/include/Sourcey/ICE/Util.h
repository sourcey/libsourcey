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