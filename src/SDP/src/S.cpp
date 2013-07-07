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


#include "Sourcey/SDP/S.h"


using namespace std;


namespace scy {
namespace SDP { 


S::S(const string& src) : 
	Line(Line::S, 3) 
{
	assert(src.substr(0, 2) == "s=");
	string attr(src.substr(2, src.length()-2));
	util::trim(attr);
	_sessionName = attr;
}


S::~S() 
{
}


string S::sessionName() 
{
	return _sessionName;
}


void S::setSessionName(const string& s)
{
	_sessionName = s;
}


string S::toString() 
{
	return "s=" + _sessionName;
}


} // namespace scy
} // namespace SDP 