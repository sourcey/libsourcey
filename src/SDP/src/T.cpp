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


#include "Sourcey/SDP/T.h"
#include "Sourcey/Logger.h"


using namespace std;



namespace scy {
namespace SDP { 


T::T(const string& src) : Line(Line::T, 5) 
{
	size_t len = src.length();
	if (src.substr(0, 2) != "t=") {
		errorL() << "ERROR: SDP line does not start with <t=>" << std::endl;
	}
	
	size_t i = 2;
	while (src[i] == ' ' && i < len)
		i++;

	string startstr = "";
	while (src[i] != ' ' && i < len)
		startstr += src[i++];

	while (src[i] == ' ' && i < len)
		i++;

	string stopstr = "";
	while (src[i] != ' ' && i < len)
		stopstr += src[i++];

	_startTime = util::fromString<UInt32>(startstr.c_str());
	_stopTime  = util::fromString<UInt32>(stopstr.c_str());
}


T::T(unsigned int start, unsigned int stop) :
	Line(Line::T, 5) 
{
	_startTime = start;
	_stopTime = stop;
}


T::~T() 
{
}


unsigned int T::startTime() 
{
	return _startTime;
}


void T::setStartTime(unsigned int start) 
{
	_startTime = start;
}


unsigned int T::stopTime() 
{
	return _stopTime;
}


void T::setStopTime(unsigned int stop) 
{
	_stopTime = stop;
}


string T::toString() 
{
	return "t=" + util::toString(_startTime) + " " + util::toString(_stopTime);
}


} // namespace scy
} // namespace SDP 

