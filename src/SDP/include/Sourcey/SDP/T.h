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


#ifndef SOURCEY_SDP_LINE_T_H
#define SOURCEY_SDP_LINE_T_H


#include "Sourcey/SDP/Line.h"
#include "Sourcey/Util.h"


namespace Scy {
namespace SDP { 


class T: public Line {
public:
	T(const std::string& src);
	T(unsigned int startTime, unsigned int stopTime);
	~T();

	unsigned int startTime();	
	unsigned int stopTime();

	void setStartTime(unsigned int time);
	void setStopTime(unsigned int time);

	virtual std::string toString();

private:
	unsigned int _startTime;
	unsigned int _stopTime;
};


} // namespace Scy
} // namespace SDP 


#endif
