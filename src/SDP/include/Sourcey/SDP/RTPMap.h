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


#ifndef SOURCEY_SDP_LINE_RTPMAP_H
#define SOURCEY_SDP_LINE_RTPMAP_H


#include "Sourcey/SDP/A.h"

#include <string>


namespace Scy {
namespace SDP { 


class RTPMap: public A {
public:
	RTPMap(const std::string& src);
	virtual ~RTPMap();

	virtual std::string toString();

	std::string payloadType() const { return _payloadType; };	
	std::string encodingName() const { return _encodingName; };
	unsigned int clockRate() const { return _clockRate; };
	std::string encodingParameters() const { return _encodingParameters; };

	void setPayloadType(const std::string& payload) { _payloadType = payload; };
	void setEncodingName(const std::string& name) { _encodingName = name; };
	void setClockRate(unsigned int rate) { _clockRate = rate; };
	void setEncodingParameters(const std::string& params) { _encodingParameters = params; };

private:
	std::string _payloadType;
	std::string _encodingName;
	unsigned int _clockRate;
	std::string _encodingParameters;

};


} // namespace Scy
} // namespace SDP 


#endif
