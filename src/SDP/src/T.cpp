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


#include "Sourcey/SDP/T.h"
#include "Sourcey/Logger.h"


using namespace std;



namespace Sourcey {
namespace SDP { 


T::T(const string& src) : Line(Line::T, 5) 
{
	size_t len = src.length();
	if (src.substr(0, 2) != "t=") {
		LogError() << "ERROR: SDP line does not start with <t=>" << std::endl;
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

	_startTime = Util::atoi(startstr.c_str());
	_stopTime  = Util::atoi(stopstr.c_str());
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
	return "t=" + Util::itoa(_startTime) + " " + Util::itoa(_stopTime);
}


} // namespace Sourcey
} // namespace SDP 

