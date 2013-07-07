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


#include "Sourcey/Logger.h"
#include "Sourcey/SDP/V.h"


using namespace std;



namespace scy {
namespace SDP { 


V::V(const string& src) :
	Line(Line::V, 1) 
{
	size_t len = src.length();
	if (src.substr(0, 2) != "v=") {
		errorL() << "ERROR: SDP line does not start with <v=>" << std::endl;
	}

	_version = util::fromString<UInt32>(src.substr(2, src.length()).c_str());
}


V::V(unsigned int ver) :
	Line(Line::V, 1) 
{
	_version = ver;
}


unsigned int V::version() 
{
	return _version;
}


void V::setVersion(unsigned int ver) 
{
	_version = ver;
}


string V::toString() 
{
	return "v=" + util::toString(_version);
}


} // namespace SDP 
} // namespace scy

