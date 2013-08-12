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


#include "Sourcey/SDP/RTPMap.h"
#include "Sourcey/util.h"
#include "Sourcey/Logger.h"


using namespace std;



namespace scy {
namespace SDP { 


RTPMap::RTPMap(const string& src) : 
	A(src) {
	// Examples:
	// a=rtpmap:98 L16/16000/2
	// a=rtpmap:<payload type> <encoding name>/<clock rate>[/<encoding parameters>]

	size_t len = src.length();
	if (src.substr(0, 8) != "a=rtpmap") {
		errorL() << "ERROR: Contact sdp line does not start with <a=rtpmap>" << std::endl;
	}
	size_t i = 9;

	// payload type
	while (src[i] != ' ' && i < len)
		_payloadType += src[i++];
	while (src[i] == ' ' && i < len)
		i++;	

	// encoding name
	while (src[i] != '/' && i < len)
		_encodingName += src[i++];
	i++;	

	// clock rate
	string clockratestr = "";
	while (src[i] != ' ' && src[i] != '/' && i < len)
		clockratestr += src[i++];
	_clockRate = util::strtoi<UInt32>(clockratestr.c_str());

	if (src[i] == '/' && i < len) {
		// encoding parameters
		i++;
		while (src[i] != ' ' && i < len)
			_encodingParameters += src[i++];
	}
}


RTPMap::~RTPMap() 
{	
}


string RTPMap::toString() 
{

	string ret = "a=rtpmap:";

	if (_payloadType.empty())
		ret += "0";
	else
		ret += _payloadType;
	ret += " ";

	if (_encodingName.empty())
		ret += "-";
	else
		ret += _encodingName;
	ret += "/";

	if (!_clockRate)
		ret += "0";
	else
		ret += util::itostr(_clockRate);

	if (!_encodingParameters.empty()) {
		ret += "/";
		ret += _encodingParameters;
	}

	return ret;
}


} // namespace scy
} // namespace SDP 