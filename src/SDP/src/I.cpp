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


#include "Sourcey/SDP/I.h"


using namespace std;


namespace scy {
namespace SDP { 


I::I(const string& src) : Line(Line::I, 9) {
	assert(src.substr(0, 2) == "i=");
	string attr(src.substr(2, src.length()-2));
	util::trim(attr);
	_sessionInformation = attr;
}


I::~I() {
}


string I::sessionInformation() {
	return _sessionInformation;
}


void I::setSessionInformation(const string& s) {
	_sessionInformation = s;
}


string I::toString() {
	return "i=" + _sessionInformation;
}


} // namespace scy
} // namespace SDP 
