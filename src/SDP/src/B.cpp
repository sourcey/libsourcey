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


#include "Sourcey/SDP/B.h"


using namespace std;



namespace scy {
namespace SDP { 


B::B(const string& src) : 
	Line(Line::B, 9) {
	assert(src.substr(0, 2) == "b=");
	string::size_type index = src.find(":");
	if (index != string::npos) {
		_type = src.substr(0, index);
		_bandwidth = util::fromString<UInt32>(src.substr(index+1, src.length()));
	}
}


B::~B() {
}


string B::toString() {
	return "b=" + _type + ":" + util::toString(_bandwidth);
}


} // namespace scy
} // namespace SDP 